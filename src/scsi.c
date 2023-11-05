#include <assert.h>
#include <stdbool.h>
#include <vulkan/vulkan.h>

#include "../include/scsi.h"
#include "../include/common.h"

VkbasicScsi vkbasic_scsi(
	VkPhysicalDevice pdev,
	VkSurfaceKHR surface
) {
	VkbasicScsi result;
	vkbasic_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		pdev, surface, &result.caps));
	uint32_t formatCount;
	vkbasic_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
		pdev, surface, &formatCount, NULL));
	VkSurfaceFormatKHR formats[formatCount];
	vkbasic_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
		pdev, surface, &formatCount, formats));
	bool flag = false;
	for (uint32_t i = 0; i < formatCount; i++) {
		if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM) {
			result.format = formats[i];
			flag = true;
			break;
		}
	}
	assert(flag);
	return result;
}
