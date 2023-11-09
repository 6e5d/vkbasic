#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <vulkan/vulkan.h>

#include "../include/vkbasic.h"
#include "../include/swapchain.h"
#include "../../vkhelper/include/device.h"
#include "../../vkhelper/include/semaphore.h"
#include "../../vkhelper/include/cbuf.h"
#include "../../vkhelper/include/scsi.h"
#include "../../vkhelper/include/validation.h"
#include "../../vkhelper/include/instance.h"
#include "../../vkhelper/include/pdev.h"

Vkbasic* vkbasic_new(
	VkInstance instance,
	VkSurfaceKHR surface
) {
	Vkbasic* v = malloc(sizeof(Vkbasic));
	v->messenger = vkhelper_validation_new(instance);
	v->instance = instance;
	v->surface = surface;
	uint32_t family_idx;
	v->pdev = vkhelper_pdev_selector(instance, surface, &family_idx);
	vkhelper_depth_format(v->pdev, &v->depth_format);
	vkGetPhysicalDeviceMemoryProperties(v->pdev, &v->pdev_memprop);
	v->scsi = vkhelper_scsi(v->pdev, v->surface);
	vkhelper_device(&v->device, &v->queue, &v->cpool, v->pdev, family_idx);

	v->vs.swapchain = VK_NULL_HANDLE;
	v->cbuf = vkhelper_cbuf_new(v->cpool, v->device);
	v->image_available = vkhelper_semaphore(v->device);
	v->render_finished = vkhelper_semaphore(v->device);
	v->fence = vkhelper_fence(v->device);
	return v;
}

void vkbasic_present(Vkbasic* vb, uint32_t* index) {
	const VkPipelineStageFlags wait_stage =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vb->image_available,
		.pWaitDstStageMask = &wait_stage,
		.commandBufferCount = 1,
		.pCommandBuffers = &vb->cbuf,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &vb->render_finished,
	};
	assert(0 == vkQueueSubmit(vb->queue, 1, &submitInfo, vb->fence));
	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vb->render_finished,
		.swapchainCount = 1,
		.pSwapchains = &vb->vs.swapchain,
		.pImageIndices = index,
	};
	VkResult result = vkQueuePresentKHR(vb->queue, &presentInfo);
	if (result != VK_SUBOPTIMAL_KHR) {
		assert(0 == result);
	}
}

void vkbasic_next_index(Vkbasic* vb, uint32_t* index) {
	assert(0 == vkWaitForFences(vb->device, 1, &vb->fence, 1, UINT64_MAX));
	assert(0 == vkResetFences(vb->device, 1, &vb->fence));
	VkResult result = vkAcquireNextImageKHR(
		vb->device,
		vb->vs.swapchain,
		UINT64_MAX,
		vb->image_available,
		NULL,
		index
	);
	if (result != VK_SUBOPTIMAL_KHR) {
		assert(0 == result);
	}
}

void vkbasic_swapchain_update(
	Vkbasic* v,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	if (v->vs.swapchain != VK_NULL_HANDLE) {
		vkbasic_swapchain_destroy(&v->vs, v->device, v->cpool);
		vkhelper_image_destroy(&v->vs.depth, v->device);
	}
	vkhelper_image_new(
		&v->vs.depth,
		v->device,
		v->pdev_memprop,
		width,
		height,
		v->depth_format
	);
	vkbasic_swapchain_new(
		&v->vs,
		&v->scsi,
		v->device,
		v->surface,
		renderpass,
		width,
		height
	);
}

void vkbasic_destroy(Vkbasic* v) {
	vkDestroyFence(v->device, v->fence, NULL);
	vkDestroySemaphore(v->device, v->image_available, NULL);
	vkDestroySemaphore(v->device, v->render_finished, NULL);
	vkhelper_image_destroy(&v->vs.depth, v->device);
	vkbasic_swapchain_destroy(&v->vs, v->device, v->cpool);
	vkFreeCommandBuffers(v->device, v->cpool, 1, &v->cbuf);
	vkDestroyCommandPool(v->device, v->cpool, NULL);
	vkDestroyDevice(v->device, NULL);
	vkDestroySurfaceKHR(v->instance, v->surface, NULL);
	vkhelper_validation_destroy(v->instance, v->messenger);
	vkDestroyInstance(v->instance, NULL);
	free(v);
}
