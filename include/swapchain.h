#ifndef INCLUDEGUARD_VKBASIC_SWAPCHAINH
#define INCLUDEGUARD_VKBASIC_SWAPCHAINH

#include <stdint.h>
#include <vulkan/vulkan.h>

#include "../../vkhelper/include/framebuffer.h"
#include "../../vkhelper/include/image.h"
#include "../../vkhelper/include/scsi.h"
#include "../../vkstatic/include/vkstatic.h"

typedef struct {
	VkSwapchainKHR swapchain;
	uint32_t image_count;
	VkhelperFramebufferImage* elements;
	VkhelperImage depth;
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
