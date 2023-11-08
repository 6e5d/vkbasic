#ifndef VKBASIC_IMAGEH
#define VKBASIC_IMAGEH

#include <vulkan/vulkan.h>

void vkbasic_create_imageview(
	VkImageView* output,
	VkDevice device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags flags
);

typedef struct {
	VkImage image;
	VkDeviceMemory memory;
	VkImageView imageview;
} VkbasicImage;

void vkbasic_image_new(
	VkbasicImage* output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties pdev_memprop,
	uint32_t width,
	uint32_t height,
	VkFormat format
);

void vkbasic_image_destroy(VkbasicImage* image, VkDevice device);

#endif
