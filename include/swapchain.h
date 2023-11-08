#ifndef _VKBASIC_SWAPCHAINH
#define _VKBASIC_SWAPCHAINH

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "../include/framebuffer.h"
#include "../include/image.h"
#include "../include/scsi.h"

typedef struct {
	VkSwapchainKHR swapchain;
	uint32_t image_count;
	VkbasicFramebufferImage* elements;
	VkbasicImage depth;
} VkbasicSwapchain;

void vkbasic_swapchain_destroy(
	VkbasicSwapchain *swapchain,
	VkDevice device,
	VkCommandPool cpool
);

void vkbasic_swapchain_new(
	VkbasicSwapchain* vs,
	VkbasicScsi* scsi,
	VkDevice device,
	VkSurfaceKHR surface,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);

VkbasicFramebufferImage* vkbasic_framebuffer(
	VkDevice device,
	VkbasicSwapchain* vs,
	VkRenderPass renderpass,
	VkFormat format,
	VkImageView depthstencil,
	// input min_image_count, modified into actual
	uint32_t* image_count,
	uint32_t width,
	uint32_t height
);

#endif
