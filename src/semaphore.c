#include <vulkan/vulkan.h>

#include "../include/common.h"

VkSemaphore vkbasic_semaphore(VkDevice device) {
	VkSemaphore result;
	VkSemaphoreCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	vkbasic_check(vkCreateSemaphore(device, &createInfo, NULL, &result));
	return result;
}

VkFence vkbasic_fence(VkDevice device) {
	VkFence result;
	VkFenceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	vkbasic_check(vkCreateFence(device, &createInfo, NULL, &result));
	return result;
}
