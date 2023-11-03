#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vulkan/vulkan.h>

#include "../include/vkbasic.h"
#include "../include/device.h"
#include "../include/physdevice.h"
#include "../include/semaphore.h"
#include "../include/instance.h"
#include "../include/validation.h"
#include "../include/swapchain.h"
#include "../include/scsi.h"
#include "../include/common.h"

Vkbasic* vkbasic_new(
	VkInstance instance,
	VkSurfaceKHR surface
) {
	Vkbasic* v = malloc(sizeof(Vkbasic));
	vkbasic_validation_new(instance, &v->messenger);
	v->instance = instance;
	v->surface = surface;
	v->pdev = vkbasic_pdev_selector(v->instance);
	VkbasicDqc dqc = vkbasic_device(v->pdev);
	v->device = dqc.device;
	v->cpool = dqc.cpool;
	v->queue = dqc.queue;
	v->vs = NULL;
	v->scsi = vkbasic_scsi(v->pdev, v->surface);
	{
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = v->cpool,
			.commandBufferCount = 1,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		};
		vkAllocateCommandBuffers(v->device, &allocInfo, &v->command_buffer);
	}
	v->image_available = vkbasic_semaphore(v->device);
	v->render_finished = vkbasic_semaphore(v->device);
	v->fence = vkbasic_fence(v->device);
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
		.pCommandBuffers = &vb->command_buffer,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &vb->render_finished,
	};
	vkbasic_check(vkQueueSubmit(vb->queue, 1, &submitInfo, vb->fence));
	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vb->render_finished,
		.swapchainCount = 1,
		.pSwapchains = &vb->vs->swapchain,
		.pImageIndices = index,
	};
	VkResult result = vkQueuePresentKHR(vb->queue, &presentInfo);
	if (result != VK_SUBOPTIMAL_KHR) {
		vkbasic_check(result);
	}
}

void vkbasic_next_index(Vkbasic* vb, uint32_t* index) {
	vkbasic_check(vkWaitForFences(vb->device, 1, &vb->fence, 1, UINT64_MAX));
	vkbasic_check(vkResetFences(vb->device, 1, &vb->fence));
	VkResult result = vkAcquireNextImageKHR(
		vb->device,
		vb->vs->swapchain,
		UINT64_MAX,
		vb->image_available,
		NULL,
		index
	);
	if (result != VK_SUBOPTIMAL_KHR) {
		vkbasic_check(result);
	}
}

void vkbasic_swapchain_update(
	Vkbasic* v,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	if (v->vs != NULL) {
		vkbasic_swapchain_destroy(v->vs, v->device, v->cpool);
	}
	v->vs = vkbasic_swapchain_new(
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
	vkbasic_swapchain_destroy(v->vs, v->device, v->cpool);
	vkFreeCommandBuffers(v->device, v->cpool, 1, &v->command_buffer);
	vkDestroyCommandPool(v->device, v->cpool, NULL);
	vkDestroyDevice(v->device, NULL);
	vkDestroySurfaceKHR(v->instance, v->surface, NULL);
	vkbasic_validation_del(v->instance, v->messenger);
	vkDestroyInstance(v->instance, NULL);
	free(v->vs);
	free(v);
}
