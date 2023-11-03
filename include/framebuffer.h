#ifndef _VKBASIC_FRAMEBUFFERH
#define _VKBASIC_FRAMEBUFFERH

#include <vulkan/vulkan.h>

typedef struct {
	VkImage image;
	VkImageView imageview;
	VkFramebuffer framebuffer;
} VkbasicFramebufferImage;

VkbasicFramebufferImage* vkbasic_framebuffer(
	VkDevice device,
	VkSwapchainKHR swapchain,
	VkRenderPass renderpass,
	VkFormat format,
	// input min_image_count, modified into actual
	uint32_t* image_count,
	uint32_t width,
	uint32_t height
);

#endif
