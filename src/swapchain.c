#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/swapchain.h"
#include "../include/scsi.h"
#include "../include/framebuffer.h"
#include "../include/common.h"

VkbasicSwapchain* vkbasic_swapchain_new(
	VkbasicScsi* scsi,
	VkDevice device,
	VkSurfaceKHR surface,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
) {
	VkbasicSwapchain* result = malloc(sizeof(VkbasicSwapchain));
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
	vkbasic_check(vkCreateSwapchainKHR(
		device, &info, NULL, &result->swapchain));
	result->elements = vkbasic_framebuffer(
		device,
		result->swapchain,
		renderpass,
		scsi->format.format,
		&min_image_count,
		width,
		height
	);
	result->image_count = min_image_count;
	return result;
}

void vkbasic_swapchain_destroy(
	VkbasicSwapchain* vs,
	VkDevice device,
	VkCommandPool cpool
) {
	VkbasicFramebufferImage* elements = vs->elements;
	for (uint32_t i = 0; i < vs->image_count; i++) {
		vkDestroyFramebuffer(device, elements[i].framebuffer, NULL);
		vkDestroyImageView(device, elements[i].imageview, NULL);
	}
	free(elements);
	vkDestroySwapchainKHR(device, vs->swapchain, NULL);
	free(vs);
}
