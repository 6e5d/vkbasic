#include <vulkan/vulkan.h>

#include "../include/cbuf.h"

VkCommandBuffer vkbasic_cbuf_new(VkCommandPool cpool, VkDevice device) {
	VkCommandBuffer result;
	VkCommandBufferAllocateInfo info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = cpool,
		.commandBufferCount = 1,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	};
	vkAllocateCommandBuffers(device, &info, &result);
	return result;
}
