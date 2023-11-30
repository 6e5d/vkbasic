#ifndef INCLUDEGUARD_VKBASIC
#define INCLUDEGUARD_VKBASIC

#include <vulkan/vulkan.h>
#include <wayland-client.h>

#include "../../vkstatic/include/vkstatic.h"
#include "../include/swapchain.h"

typedef struct {
	Vkhelper2Image depthstencil;
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
	const uint32_t* index
);
void vkbasic_submit(
	Vkbasic* vb,
	VkQueue queue,
	VkCommandBuffer cbuf,
	const uint32_t* index
);
VkFramebuffer vkbasic_next_index(Vkbasic* vb, VkDevice device, uint32_t* index);
void vkbasic_deinit(Vkbasic* v, VkDevice device, VkCommandPool cpool);

#endif
