#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/physdevice.h"

VkPhysicalDevice vkbasic_pdev_selector(VkInstance instance) {
	uint32_t devcount;
	vkEnumeratePhysicalDevices(instance, &devcount, NULL);
	VkPhysicalDevice* phys_devices = malloc(devcount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(
		instance,
		&devcount,
		phys_devices
	);
	VkPhysicalDevice result = VK_NULL_HANDLE;
	uint32_t bestScore = 0;
	for (uint32_t i = 0; i < devcount; i++) {
		VkPhysicalDevice device = phys_devices[i];
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);
		uint32_t score;
		switch (properties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			score = 1;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			score = 4;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			score = 5;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			score = 3;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			score = 2;
			break;
		default:
			continue;
		}
		if (score > bestScore) {
			result = device;
			bestScore = score;
		}
	}
	free(phys_devices);
	return result;
}

void vkbasic_pdev_dprint(VkPhysicalDevice pdev) {
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(pdev, &prop);
	// printf("api version: %d\n", prop.apiVersion);
	// printf("driver version: %d\n", prop.driverVersion);
	// printf("vendor id: %d\n", prop.vendorID);
	// printf("device id: %d\n", prop.deviceID);
	printf("device name: %s\n", prop.deviceName);
}
