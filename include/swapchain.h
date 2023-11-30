#ifndef INCLUDEGUARD_VKBASIC_SWAPCHAINH
#define INCLUDEGUARD_VKBASIC_SWAPCHAINH

#include <vulkan/vulkan.h>

#include "../../vkhelper/include/vkhelper.h"
#include "../../vkhelper2/include/vkhelper2.h"
#include "../../vkstatic/include/vkstatic.h"

typedef struct {
	VkSwapchainKHR swapchain;
	uint32_t image_count;
	VkhelperFramebufferImage* elements;
	Vkhelper2Image depth;
} VkbasicSwapchain;

void vkbasic_swapchain_destroy(
	VkbasicSwapchain *vs,
	VkDevice device,
	VkCommandPool cpool
);

void vkbasic_swapchain_new(
	VkbasicSwapchain* vs,
	Vkstatic* vks,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);

#endif
