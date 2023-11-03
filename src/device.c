#include <vulkan/vulkan.h>
#include <stdlib.h>

#include "../include/device.h"
#include "../include/common.h"

VkbasicDqc vkbasic_device(VkPhysicalDevice pdev) {
	VkbasicDqc dqc;
	uint32_t queue_family_index = 0;
	const char* const exts[] = { "VK_KHR_swapchain" };
	const char* const layers[] = { "VK_LAYER_KHRONOS_validation" };
	uint32_t queueFamilyCount;
	typedef VkQueueFamilyProperties Qfp;
	vkGetPhysicalDeviceQueueFamilyProperties(pdev, &queueFamilyCount, NULL);
	Qfp* families = malloc(sizeof(Qfp) * queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
		pdev, &queueFamilyCount, families);
	for (uint32_t i = 0; i < queueFamilyCount; i++) {
		VkBool32 present = 0;
		if (present && (
			families[i].queueFlags &
			VK_QUEUE_GRAPHICS_BIT
		)) {
			queue_family_index = i;
			break;
		}
	}
	free(families);
	float priority = 1;
	VkDeviceQueueCreateInfo queueCreateInfo = {0};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queue_family_index;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &priority;
	VkDeviceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueCreateInfo,
		.enabledExtensionCount = sizeof(exts) / sizeof(const char*),
		.ppEnabledExtensionNames = exts,
		.enabledLayerCount = sizeof(layers) / sizeof(const char*),
		.ppEnabledLayerNames = layers,
	};
	vkbasic_check(vkCreateDevice(pdev, &createInfo, NULL, &dqc.device));
	vkGetDeviceQueue(dqc.device, queue_family_index, 0, &dqc.queue);
	VkCommandPoolCreateInfo cpool_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.queueFamilyIndex = queue_family_index,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
	};
	vkbasic_check(vkCreateCommandPool(
		dqc.device,
		&cpool_info,
		NULL,
		&dqc.cpool
	));
	return dqc;
}
