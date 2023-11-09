#ifndef _VKBASIC_SWAPCHAINH
#define _VKBASIC_SWAPCHAINH

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "../../vkhelper/include/framebuffer.h"
#include "../../vkhelper/include/image.h"
#include "../../vkhelper/include/scsi.h"

typedef struct {
	VkSwapchainKHR swapchain;
	uint32_t image_count;
	VkhelperFramebufferImage* elements;
	VkhelperImage depth;
} VkbasicSwapchain;

void vkbasic_swapchain_destroy(
	VkbasicSwapchain *swapchain,
	VkDevice device,
	VkCommandPool cpool
);

void vkbasic_swapchain_new(
	VkbasicSwapchain* vs,
	VkhelperScsi* scsi,
	VkDevice device,
	VkSurfaceKHR surface,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);

#endif
