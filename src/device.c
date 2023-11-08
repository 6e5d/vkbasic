#include <vulkan/vulkan.h>
#include <stdlib.h>

#include "../include/device.h"
#include "../include/common.h"

void vkbasic_device(
	VkDevice* device,
	VkQueue* queue,
	VkCommandPool* cpool,
	VkPhysicalDevice pdev,
	uint32_t family_idx
) {
	const char* const exts[] = { "VK_KHR_swapchain" };
	const char* const layers[] = { "VK_LAYER_KHRONOS_validation" };

	float priority = 1;
	VkDeviceQueueCreateInfo qinfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = family_idx,
		.queueCount = 1,
		.pQueuePriorities = &priority,
	};
	VkDeviceCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &qinfo,
		.enabledExtensionCount = sizeof(exts) / sizeof(const char*),
		.ppEnabledExtensionNames = exts,
		.enabledLayerCount = sizeof(layers) / sizeof(const char*),
		.ppEnabledLayerNames = layers,
	};
	vkbasic_check(vkCreateDevice(pdev, &info, NULL, device));
	vkGetDeviceQueue(*device, family_idx, 0, queue);
	VkCommandPoolCreateInfo cpool_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = family_idx,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	vkbasic_check(vkCreateCommandPool(
		*device,
		&cpool_info,
		NULL,
		cpool
	));
}
