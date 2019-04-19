#include <stdio.h>
#include "wayland-server.h"

const struct wl_compositor_interface compositor_interface;

static void
compositor_bind(struct wl_client *client,
        void *data, uint32_t version, uint32_t id)
{
    struct wl_display *display = data;
    struct wl_resource *resource;

    resource = wl_resource_create(client, &wl_compositor_interface,
                      version, id);
    if (resource == NULL) {
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(resource, &compositor_interface,
                       display, NULL);
}

int main()
{
    printf("Wayland Server!\n");
    struct wl_display *display = wl_display_create();
    if (!wl_global_create(display, &wl_compositor_interface, 4,
                      display, compositor_bind))
            return -1;

    wl_display_init_shm(display);
    wl_display_add_socket_auto(display);
    wl_display_run(display);
    return 0;
}
