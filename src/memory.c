#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>

uint32_t vkbasic_memory_type_index(
	VkPhysicalDeviceMemoryProperties pdev_memprop,
	VkMemoryPropertyFlags props,
	uint32_t type
) {
	for (uint32_t i = 0; i < pdev_memprop.memoryTypeCount; i++) {
		if ((type & 1) == 1) {
			if ((pdev_memprop.memoryTypes[i].propertyFlags &
				props) == props) {
				return i;
			}
		}
		type >>= 1;
	}
	printf("no suitable memory type\n");
	exit(1);
}
