#ifndef INCLUDEGUARD_VKBASIC
#define INCLUDEGUARD_VKBASIC

#include <vulkan/vulkan.h>
#include <wayland-client.h>

#include "../../vkhelper/build/vkhelper.h"
#include "../../vkhelper2/build/vkhelper2.h"
#include "../../vkstatic/build/vkstatic.h"

typedef struct {
	VkSwapchainKHR swapchain;
	uint32_t image_count;
	Vkhelper(FramebufferImage)* elements;
	Vkhelper2(Image) depth;
} Vkbasic(Swapchain);

typedef struct {
	Vkhelper2(Image) depthstencil;
	Vkbasic(Swapchain) vs;
	VkSemaphore image_available;
	VkSemaphore render_finished;
	VkFence fence;
} Vkbasic();

void vkbasic(init)(Vkbasic()* v, VkDevice device);
void vkbasic(swapchain_update)(
	Vkbasic()* v,
	Vkstatic()* vks,
	VkRenderPass renderpass,
	uint32_t width,
	uint32_t height
);
void vkbasic(present)(
	Vkbasic()* vb,
	VkQueue queue,
	uint32_t* index
);
void vkbasic(submit)(
	Vkbasic()* vb,
	VkQueue queue,
	VkCommandBuffer cbuf
);
VkFramebuffer vkbasic(next_index)(Vkbasic()* vb, VkDevice device, uint32_t* index);
void vkbasic(deinit)(Vkbasic()* v, VkDevice device);

#endif
