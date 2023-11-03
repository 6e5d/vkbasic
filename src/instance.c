#include <vulkan/vulkan.h>

#include "../include/common.h"
#include "../include/instance.h"

VkInstance vkbasic_instance(void) {
	const char* const exts[] = {
		"VK_EXT_debug_utils",
		"VK_KHR_surface",
		"VK_KHR_wayland_surface"
	};

	const char* const layerNames[] = {
		"VK_LAYER_KHRONOS_validation"
	};

	VkApplicationInfo appInfo = {0};
	const char* appName = "vulkan";
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.pEngineName = appName;
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = sizeof(exts) / sizeof(const char*);
	createInfo.ppEnabledExtensionNames = exts;
	createInfo.enabledLayerCount = sizeof(layerNames) / sizeof(const char*);
	createInfo.ppEnabledLayerNames = layerNames;

	VkInstance instance = VK_NULL_HANDLE;
	vkbasic_check(vkCreateInstance(&createInfo, NULL, &instance));
	return instance;
}
