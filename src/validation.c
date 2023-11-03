#include <vulkan/vulkan.h>
#include <stdio.h>

#include "../include/common.h"

#define GET_EXTENSION_FUNCTION(_id) ((PFN_##_id)(vkGetInstanceProcAddr(instance, #_id)))

static VkBool32 on_error(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData
) {
	printf("Vulkan ");
	switch (type) {
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT :
		printf("general ");
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT :
		printf("validation ");
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT :
		printf("performance ");
		break;
	}

	switch (severity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT :
		printf("(verbose): ");
		break;
	default :
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT :
		printf("(info): ");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT :
		printf("(warning): ");
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT :
		printf("(error): ");
		break;
	}

	printf("%s\n", callbackData->pMessage);
	return 0;
}

void vkbasic_validation_new(VkInstance instance, VkDebugUtilsMessengerEXT* msg) {
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = on_error;
	vkbasic_check(
		GET_EXTENSION_FUNCTION(vkCreateDebugUtilsMessengerEXT)
		(instance, &createInfo, NULL, msg)
	);


}

void vkbasic_validation_del(VkInstance instance, VkDebugUtilsMessengerEXT msg) {
	GET_EXTENSION_FUNCTION(vkDestroyDebugUtilsMessengerEXT)
		(instance, msg, NULL);
}

#undef GET_EXTENSION_FUNCTION
