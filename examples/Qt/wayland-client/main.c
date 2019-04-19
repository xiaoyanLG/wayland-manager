#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "wayland-client.h"

static void
registry_handle_global(void *data, struct wl_registry *registry,
               uint32_t id, const char *interface, uint32_t version)
{
    printf("%s\n", interface);
    struct wl_display *display = data;
        if (strcmp(interface, "wl_compositor") == 0) {
            struct wl_compositor *compositor =
                wl_registry_bind(registry,
                         id, &wl_compositor_interface, 1);
            wl_compositor_create_surface(compositor);
        } else if (strcmp(interface, "wl_shm") == 0) {
            struct wl_shm *shm =
                wl_registry_bind(registry,
                         id, &wl_shm_interface, 1);


            int fd = open("/home/rootqt/QtProjects/build-wayland-client-Desktop_Qt_5_9_5_GCC_64bit-Release/test.txt", O_RDWR);
            struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, 50000);
            wl_shm_pool_create_buffer(pool, 0, 5, 5, 500, 0);
            wl_display_roundtrip(display);
        }
}

static void
registry_handle_global_remove(void *data, struct wl_registry *registry,
                  uint32_t name)
{
}

static const struct wl_registry_listener registry_listener = {
    registry_handle_global,
    registry_handle_global_remove
};

int main()
{
    printf("Wayland Client! 1111\n");
    struct wl_display *display = wl_display_connect(NULL);
	
printf("Wayland Client! line: %d\n", __LINE__);
    struct wl_registry *registry = wl_display_get_registry(display);
printf("Wayland Client! line: %d\n", __LINE__);
    wl_registry_add_listener(registry,
                     &registry_listener, display);
printf("Wayland Client! line: %d\n", __LINE__);
    wl_display_roundtrip(display);
printf("Wayland Client! line: %d\n", __LINE__);
    int ret = 1;
    while (ret) {
        ret = wl_display_dispatch(display);
    }
    return 0;
}
