#include <vulkan/vulkan.h>
#include <stdio.h>

#include "../include/common.h"

#define GET_EXTENSION_FUNCTION(_id) \
	((PFN_##_id)(vkGetInstanceProcAddr(instance, #_id)))

static VkBool32 on_error(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData
) {
	switch (severity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		printf("(V");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		printf("(I");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		printf("(W");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		printf("(E");
		break;
	default:
		printf("(?");
	}
	switch (type) {
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		printf("g)");
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		printf("v) ");
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		printf("p)");
		break;
	default:
		printf("?)");
	}

	printf("%s\n", callbackData->pMessage);
	return 0;
}

VkDebugUtilsMessengerEXT vkbasic_validation_new(VkInstance instance) {
	VkDebugUtilsMessengerEXT ext;
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {0};
	createInfo.sType =
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = on_error;
	vkbasic_check(
		GET_EXTENSION_FUNCTION(vkCreateDebugUtilsMessengerEXT)
		(instance, &createInfo, NULL, &ext)
	);
	return ext;
}

void vkbasic_validation_destroy(
	VkInstance instance,
	VkDebugUtilsMessengerEXT msg
) {
	GET_EXTENSION_FUNCTION(vkDestroyDebugUtilsMessengerEXT)
		(instance, msg, NULL);
}

#undef GET_EXTENSION_FUNCTION
