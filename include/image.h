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

#endif
