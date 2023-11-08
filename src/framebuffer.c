#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../include/common.h"
#include "../include/image.h"
#include "../include/framebuffer.h"

VkFramebuffer create_framebuffer(
	VkDevice device,
	VkRenderPass renderpass,
	VkImageView* attachments,
	uint32_t width,
	uint32_t height
) {
	VkFramebuffer framebuffer;
	VkFramebufferCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = renderpass,
		.attachmentCount = 2,
		.pAttachments = attachments,
		.width = width,
		.height = height,
		.layers = 1,
	};
	vkbasic_check(vkCreateFramebuffer(
		device, &createInfo, NULL, &framebuffer));
	return framebuffer;
}
