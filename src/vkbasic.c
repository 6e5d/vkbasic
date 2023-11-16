#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../../vkhelper/include/semaphore.h"
#include "../../vkstatic/include/vkstatic.h"
#include "../include/swapchain.h"
#include "../include/vkbasic.h"

void vkbasic_init(Vkbasic* v, VkDevice device) {
	v->vs.swapchain = VK_NULL_HANDLE;
	v->image_available = vkhelper_semaphore(device);
	v->render_finished = vkhelper_semaphore(device);
	v->fence = vkhelper_fence(device);
}

void vkbasic_present(
	Vkbasic* vb,
	VkQueue queue,
	VkCommandBuffer cbuf,
	const uint32_t* index
) {
	const VkPipelineStageFlags wait_stage =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vb->image_available,
		.pWaitDstStageMask = &wait_stage,
		.commandBufferCount = 1,
		.pCommandBuffers = &cbuf,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &vb->render_finished,
	};
	assert(0 == vkQueueSubmit(queue, 1, &submitInfo, vb->fence));
	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vb->render_finished,
		.swapchainCount = 1,
		.pSwapchains = &vb->vs.swapchain,
		.pImageIndices = index,
	};
	VkResult result = vkQueuePresentKHR(queue, &presentInfo);
	if (result != VK_SUBOPTIMAL_KHR) {
		assert(0 == result);
	}
}

void vkbasic_next_index(
	Vkbasic* vb,
	VkDevice device,
	uint32_t* index
) {
	assert(0 == vkWaitForFences(device, 1, &vb->fence, 1, UINT64_MAX));
	assert(0 == vkResetFences(device, 1, &vb->fence));
	VkResult result = vkAcquireNextImageKHR(
		device,
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
	Vkstatic* vks,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	assert(width > 0 && height > 0);
	if (v->vs.swapchain != VK_NULL_HANDLE) {
		vkbasic_swapchain_destroy(&v->vs, vks->device, vks->cpool);
		vkhelper_image_deinit(&v->vs.depth, vks->device);
	}
	vkhelper_image_new(
		&v->vs.depth,
		vks->device,
		vks->memprop,
		width,
		height,
		vks->depth_format,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
			VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
		VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
	);
	vkbasic_swapchain_new(&v->vs, vks, renderpass, width, height);
}

void vkbasic_deinit(Vkbasic* v, VkDevice device, VkCommandPool cpool) {
	vkDestroyFence(device, v->fence, NULL);
	vkDestroySemaphore(device, v->image_available, NULL);
	vkDestroySemaphore(device, v->render_finished, NULL);
	vkhelper_image_deinit(&v->vs.depth, device);
	vkbasic_swapchain_destroy(&v->vs, device, cpool);
}
