#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"

void vkbasic_check(VkResult ret) {
	if (ret != VK_SUCCESS) {
		printf("check explicit fail\n");
		exit(1);
	}
}
