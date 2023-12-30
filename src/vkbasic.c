#include <vulkan/vulkan.h>

#include "../include/vkbasic.h"
const static uint64_t e10 = 10000000000;

static Vkhelper(FramebufferImage)* vkbasic(framebuffer)(
	VkDevice device,
	Vkbasic(Swapchain)* vs,
	VkRenderPass renderpass,
	VkFormat format,
	VkImageView depthstencil,
	uint32_t* image_count,
	uint32_t width,
	uint32_t height
) {
	assert(0 == vkGetSwapchainImagesKHR(
		device, vs->swapchain, image_count, NULL));
	VkImage* images = malloc(sizeof(VkImage) * *image_count);
	assert(0 == vkGetSwapchainImagesKHR(
		device, vs->swapchain, image_count, images));
	Vkhelper(FramebufferImage)* elements =
		malloc(*image_count * sizeof(Vkhelper(FramebufferImage)));
	assert(NULL != elements);
	for (uint32_t i = 0; i < *image_count; i += 1) {
		elements[i].image = images[i];
		vkhelper2(image_create_imageview)(
			&elements[i].attachments[0],
			device,
			images[i],
			format,
			VK_IMAGE_ASPECT_COLOR_BIT,
			1
		);
		elements[i].attachments[1] = depthstencil;
		elements[i].framebuffer = vkhelper(framebuffer_create)(
			device,
			renderpass,
			elements[i].attachments,
			width,
			height
		);
	}
	free(images);
	return elements;
}

static void swapchain_init(
	Vkbasic(Swapchain)* vs,
	Vkstatic()* vks,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	VkSurfaceCapabilitiesKHR* caps = &vks->surface_caps;
	uint32_t min_image_count;
	if (caps->minImageCount + 1 < caps->maxImageCount) {
		min_image_count = caps->minImageCount + 1;
	} else {
		min_image_count = caps->minImageCount;
	}
	VkSwapchainCreateInfoKHR info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vks->surface,
		.minImageCount = min_image_count,
		.imageFormat = vks->surface_format.format,
		.imageColorSpace = vks->surface_format.colorSpace,
		.imageExtent = {
			.width = width,
			.height = height,
		},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
			VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
			VK_IMAGE_USAGE_SAMPLED_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = caps->currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
		.clipped = 1,
	};
	assert(0 == vkCreateSwapchainKHR(vks->device, &info, NULL, &vs->swapchain));
	vs->elements = vkbasic(framebuffer)(
		vks->device,
		vs,
		renderpass,
		vks->surface_format.format,
		vs->depth.imageview,
		&min_image_count,
		width,
		height
	);
	vs->image_count = min_image_count;
}

static void swapchain_deinit(Vkbasic(Swapchain)* vs, VkDevice device) {
	Vkhelper(FramebufferImage)* elements = vs->elements;
	for (uint32_t i = 0; i < vs->image_count; i += 1) {
		vkDestroyFramebuffer(device, elements[i].framebuffer, NULL);
		vkDestroyImageView(device, elements[i].attachments[0], NULL);
	}
	free(elements);
	vkDestroySwapchainKHR(device, vs->swapchain, NULL);
}

void vkbasic(init)(Vkbasic()* v, VkDevice device) {
	v->vs.swapchain = VK_NULL_HANDLE;
	v->image_available = vkhelper(semaphore)(device);
	v->render_finished = vkhelper(semaphore)(device);
	v->fence = vkhelper(fence)(device);
}

void vkbasic(submit)(
	Vkbasic()* vb,
	VkQueue queue,
	VkCommandBuffer cbuf
) {
	VkPipelineStageFlags wait_stage =
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
}

void vkbasic(present)(
	Vkbasic()* vb,
	VkQueue queue,
	uint32_t* index
) {
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

VkFramebuffer vkbasic(next_index)(
	Vkbasic()* vb,
	VkDevice device,
	uint32_t* index
) {
	assert(0 == vkWaitForFences(device, 1, &vb->fence, 1, e10));
	assert(0 == vkResetFences(device, 1, &vb->fence));
	VkResult result = vkAcquireNextImageKHR(
		device,
		vb->vs.swapchain,
		e10, // 10 secs
		vb->image_available,
		NULL,
		index
	);
	if (result != VK_SUBOPTIMAL_KHR) {
		assert(0 == result);
	}
	return vb->vs.elements[*index].framebuffer;
}

void vkbasic(swapchain_update)(
	Vkbasic()* v,
	Vkstatic()* vks,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	assert(width > 0 && height > 0);
	if (v->vs.swapchain != VK_NULL_HANDLE) {
		swapchain_deinit(&v->vs, vks->device);
		vkhelper2(image_deinit)(&v->vs.depth, vks->device);
	}
	vkhelper2(image_new_depthstencil)(
		&v->vs.depth,
		vks->device,
		vks->memprop,
		width,
		height,
		vks->depth_format,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
			VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
	swapchain_init(&v->vs, vks, renderpass, width, height);
}

void vkbasic(deinit)(Vkbasic()* v, VkDevice device) {
	vkDestroyFence(device, v->fence, NULL);
	vkDestroySemaphore(device, v->image_available, NULL);
	vkDestroySemaphore(device, v->render_finished, NULL);
	vkhelper2(image_deinit)(&v->vs.depth, device);
	swapchain_deinit(&v->vs, device);
}
