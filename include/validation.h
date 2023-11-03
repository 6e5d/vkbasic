#include <vulkan/vulkan.h>

void vkbasic_validation_new(
	VkInstance instance,
	VkDebugUtilsMessengerEXT* msg);
void vkbasic_validation_del(
	VkInstance instance,
	VkDebugUtilsMessengerEXT msg);
