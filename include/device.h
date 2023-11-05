#include <vulkan/vulkan.h>

typedef struct {
	VkDevice device;
	VkQueue queue;
	VkCommandPool cpool;
} VkbasicDqc;

VkbasicDqc vkbasic_device(VkPhysicalDevice pdev, uint32_t family_idx);
