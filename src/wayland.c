#include <wayland-client.h>
#include <assert.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_wayland.h>

#include "../include/wayland.h"
#include "../include/vkbasic.h"
#include "../../vkhelper/include/instance.h"

VkSurfaceKHR vkbasic_wayland_surface(
	VkInstance instance,
	struct wl_display* display,
	struct wl_surface* surface
) {
	VkSurfaceKHR vksurface;
	VkWaylandSurfaceCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
		.display = display,
		.surface = surface,
	};
	assert(0 == vkCreateWaylandSurfaceKHR(
		instance, &createInfo, NULL, &vksurface));
	return vksurface;
}

Vkbasic* vkbasic_new_wayland(
	struct wl_display* display,
	struct wl_surface* wsur
) {
	VkInstance instance = vkhelper_instance();
	VkSurfaceKHR vsur = vkbasic_wayland_surface(instance, display, wsur);
	Vkbasic* vb = vkbasic_new(instance, vsur);
	return vb;
}
