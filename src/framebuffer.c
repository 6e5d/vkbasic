#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../include/common.h"
#include "../include/image.h"
#include "../include/framebuffer.h"

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
	assert(NULL != elements);
	for (uint32_t i = 0; i < *image_count; i++) {
		elements[i].image = images[i];
		vkbasic_create_imageview(
			&elements[i].imageview,
			device,
			images[i],
			format,
			VK_IMAGE_ASPECT_COLOR_BIT
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
