#include <assert.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/swapchain.h"
#include "../../vkhelper/include/framebuffer.h"
#include "../../vkhelper/include/scsi.h"

static VkhelperFramebufferImage* vkbasic_framebuffer(
	VkDevice device,
	VkbasicSwapchain* vs,
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
	VkhelperFramebufferImage* elements =
		malloc(*image_count * sizeof(VkhelperFramebufferImage));
	assert(NULL != elements);
	for (uint32_t i = 0; i < *image_count; i++) {
		elements[i].image = images[i];
		vkhelper_create_imageview(
			&elements[i].attachments[0],
			device,
			images[i],
			format,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
		elements[i].attachments[1] = depthstencil;
		elements[i].framebuffer = create_framebuffer(
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

void vkbasic_swapchain_new(
	VkbasicSwapchain* vs,
	VkhelperScsi* scsi,
	VkDevice device,
	VkSurfaceKHR surface,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	uint32_t min_image_count;
	if (scsi->caps.minImageCount + 1 < scsi->caps.maxImageCount) {
		min_image_count = scsi->caps.minImageCount + 1;
	} else {
		min_image_count = scsi->caps.minImageCount;
	}
	VkSwapchainCreateInfoKHR info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = min_image_count,
		.imageFormat = scsi->format.format,
		.imageColorSpace = scsi->format.colorSpace,
		.imageExtent.width = width,
		.imageExtent.height = height,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = scsi->caps.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
		.clipped = 1,
	};
	assert(0 == vkCreateSwapchainKHR(device, &info, NULL, &vs->swapchain));
	vs->elements = vkbasic_framebuffer(
		device,
		vs,
		renderpass,
		scsi->format.format,
		vs->depth.imageview,
		&min_image_count,
		width,
		height
	);
	vs->image_count = min_image_count;
}

void vkbasic_swapchain_destroy(
	VkbasicSwapchain* vs,
	VkDevice device,
	VkCommandPool cpool
) {
	VkhelperFramebufferImage* elements = vs->elements;
	for (uint32_t i = 0; i < vs->image_count; i++) {
		vkDestroyFramebuffer(device, elements[i].framebuffer, NULL);
		vkDestroyImageView(device, elements[i].attachments[0], NULL);
	}
	free(elements);
	vkDestroySwapchainKHR(device, vs->swapchain, NULL);
}
