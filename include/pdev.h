#include <stdint.h>
#include <vulkan/vulkan.h>

VkPhysicalDevice vkbasic_pdev_selector(
	VkInstance instance,
	VkSurfaceKHR surface,
	uint32_t* result_idx
);
void vkbasic_pdev_print(VkPhysicalDevice pdev);
