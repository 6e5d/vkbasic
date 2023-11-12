#ifndef INCLUDEGUARD_VKBASIC_VKBASICH
#define INCLUDEGUARD_VKBASIC_VKBASICH

#include <stdint.h>
#include <wayland-client.h>
#include <vulkan/vulkan.h>

#include "../include/swapchain.h"
#include "../../vkstatic/include/vkstatic.h"

typedef struct {
	VkhelperImage depthstencil;
	VkbasicSwapchain vs;
	VkSemaphore image_available;
	VkSemaphore render_finished;
	VkFence fence;
} Vkbasic;

void vkbasic_init(Vkbasic* v, VkDevice device);
void vkbasic_swapchain_update(
	Vkbasic* v,
	Vkstatic* vks,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);
void vkbasic_present(
	Vkbasic* vb,
	VkQueue queue,
	VkCommandBuffer cbuf,
	uint32_t* index
);
void vkbasic_next_index(Vkbasic* vb, VkDevice device, uint32_t* index);
void vkbasic_deinit(Vkbasic* v, VkDevice device, VkCommandPool cpool);

#endif
