#include <vulkan/vulkan.h>

VkDebugUtilsMessengerEXT vkbasic_validation_new(VkInstance instance);
void vkbasic_validation_destroy(
	VkInstance instance,
	VkDebugUtilsMessengerEXT msg
);
