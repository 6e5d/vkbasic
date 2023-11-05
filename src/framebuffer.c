#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../include/framebuffer.h"
#include "../include/common.h"

static VkImageView create_imageview(
	VkDevice device,
	VkImage image,
	VkFormat format
) {
	VkImageView imageview;
	VkImageViewCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
		.subresourceRange.baseMipLevel = 0,
		.subresourceRange.levelCount = 1,
		.subresourceRange.baseArrayLayer = 0,
		.subresourceRange.layerCount = 1,
		.image = image,
		.format = format,
		.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
	};
	vkbasic_check(vkCreateImageView(
		device,
		&createInfo,
		NULL,
		&imageview
	));
	return imageview;
}

static VkFramebuffer create_framebuffer(
	VkDevice device,
	VkRenderPass renderpass,
	VkImageView imageview,
	uint32_t width,
	uint32_t height
) {
	VkFramebuffer framebuffer;
	VkFramebufferCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = renderpass,
		.attachmentCount = 1,
		.pAttachments = &imageview,
		.width = width,
		.height = height,
		.layers = 1,
	};
	vkbasic_check(vkCreateFramebuffer(
		device, &createInfo, NULL, &framebuffer));
	return framebuffer;
}

VkbasicFramebufferImage* vkbasic_framebuffer(
	VkDevice device,
	VkSwapchainKHR swapchain,
	VkRenderPass renderpass,
	VkFormat format,
	uint32_t* image_count,
	uint32_t width,
	uint32_t height
) {
	vkbasic_check(vkGetSwapchainImagesKHR(
		device, swapchain, image_count, NULL));
	VkImage* images = malloc(sizeof(VkImage) * *image_count);
	vkbasic_check(vkGetSwapchainImagesKHR(
		device, swapchain, image_count, images));
	VkbasicFramebufferImage* elements =
		malloc(*image_count * sizeof(VkbasicFramebufferImage));
	for (uint32_t i = 0; i < *image_count; i++) {
		elements[i].image = images[i];
		elements[i].imageview = create_imageview(
			device,
			images[i],
			format
		);
		elements[i].framebuffer = create_framebuffer(
			device,
			renderpass,
			elements[i].imageview,
			width,
			height
		);
	}
	free(images);
	return elements;
}
