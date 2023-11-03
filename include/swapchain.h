#ifndef _VKBASIC_SWAPCHAINH
#define _VKBASIC_SWAPCHAINH

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "../include/framebuffer.h"
#include "../include/scsi.h"

typedef struct {
	VkSwapchainKHR swapchain;
	uint32_t image_count;
	VkbasicFramebufferImage* elements;
} VkbasicSwapchain;

void vkbasic_swapchain_destroy(
	VkbasicSwapchain *swapchain,
	VkDevice device,
	VkCommandPool cpool
);
VkbasicSwapchain* vkbasic_swapchain_new(
	VkbasicScsi* scsi,
	VkDevice device,
	VkSurfaceKHR surface,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);

#endif
