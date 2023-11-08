#include <vulkan/vulkan.h>

#include "../include/image.h"
#include "../include/common.h"
#include "../include/memory.h"

void vkbasic_create_imageview(
	VkImageView* output,
	VkDevice device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags flags
) {
	VkImageSubresourceRange range = {
		.aspectMask = flags,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
	VkImageViewCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.subresourceRange = range,
		.image = image,
		.format = format,
	};
	vkbasic_check(vkCreateImageView(
		device,
		&createInfo,
		NULL,
		output
	));
}

void vkbasic_create_image(
	VkbasicImage* output,
	VkDevice device,
	uint32_t width,
	uint32_t height,
	VkFormat format
) {
	// image creation
	{
		VkImageCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = format,
			.extent = {width, height, 1},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};
		vkbasic_check(vkCreateImage(
			device, &info, NULL, &output->image));
	}

	// memory allocation
	VkMemoryRequirements reqs;
	vkGetImageMemoryRequirements(device, output->image, &reqs);
	VkMemoryAllocateInfo alloc = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = reqs.size,
		.memoryTypeIndex = vkbasic_memory_type_index(
			reqs.memoryTypeBits,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		),
	};
	vkbasic_check(vkAllocateMemory(device, &alloc, NULL, &output->memory));
	vkbasic_check(vkBindImageMemory(
		device,
		output->image,
		output->memory,
		0));

	vkbasic_create_imageview(
		&output->imageview,
		device,
		output->image,
		format,
		VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
	);
}
