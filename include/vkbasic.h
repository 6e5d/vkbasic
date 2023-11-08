#ifndef _VKBASIC_VKBASICH
#define _VKBASIC_VKBASICH

#include <stdint.h>
#include <wayland-client.h>
#include <vulkan/vulkan.h>

#include "../include/swapchain.h"
#include "../include/scsi.h"

typedef struct {
	VkInstance instance;
	VkCommandBuffer cbuf;
	VkDebugUtilsMessengerEXT messenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice pdev;
	VkFormat depth_format;
	VkPhysicalDeviceMemoryProperties pdev_memprop;
	VkDevice device;
	VkQueue queue;
	VkCommandPool cpool;
	VkbasicScsi scsi;
	VkbasicSwapchain* vs;
	VkSemaphore image_available;
	VkSemaphore render_finished;
	VkFence fence;
} Vkbasic;

Vkbasic* vkbasic_new(
	VkInstance instance,
	VkSurfaceKHR surface
);
void vkbasic_swapchain_update(
	Vkbasic* v,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);
void vkbasic_present(Vkbasic* vb, uint32_t* index);
void vkbasic_next_index(Vkbasic* vb, uint32_t* index);
void vkbasic_destroy(Vkbasic* v);

#endif
