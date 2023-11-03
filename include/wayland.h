#include <vulkan/vulkan.h>
#include <wayland-client.h>

#include "../include/vkbasic.h"

VkSurfaceKHR vkbasic_wayland_surface(
	VkInstance instance,
	struct wl_display* display,
	struct wl_surface* surface
);

Vkbasic* vkbasic_new_wayland(
	struct wl_display* display,
	struct wl_surface* wsur
);
