#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <wayland-client.h>

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


            int fd = open("test.txt", O_RDWR | O_APPEND);
            struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, 50000);
            wl_shm_pool_create_buffer(pool, 0, 5, 5, 500, 0);
            wl_display_roundtrip(display);
        }
}

static void
registry_handle_global_remove(void *data, struct wl_registry *registry,
                  uint32_t name)
{
    printf("func: %s, line: %d\n", __FUNCTION__, __LINE__);
}

static const struct wl_registry_listener registry_listener = {
    registry_handle_global,
    registry_handle_global_remove
};

static void
call_back_handle_done(void *data,
                      struct wl_callback *wl_callback,
                      uint32_t callback_data)
{
    struct wl_event_queue {
        struct wl_list event_list;
        struct wl_display *display;
    };

    struct wl_event_queue *newq = data;

    wl_display_roundtrip_queue(newq->display, newq);
    printf("func: %s, line: %d\n", __FUNCTION__, __LINE__);
}

static const struct wl_callback_listener call_back_listener = {
    call_back_handle_done
};

int main()
{
    printf("build time: %s-%s, func: %s, line: %d\n", __DATE__, __TIME__, __FUNCTION__, __LINE__);

    struct wl_display *display = wl_display_connect(NULL);

    struct wl_registry *registry = wl_display_get_registry(display);

    // 如果指定了自己的事件队列，那么wl_display_dispatch派发的时候就不会派发该队列的事件，
    // 需要自己调用wl_display_dispatch_queue明确指定需要派发的事件队列，所有的代理对象（wl_proxy）
    // 都有一个事件队列，如果没有明确指定都是从创建该代理对象的父对象继承过来的，最原始的是wl_display
    // 对象的事件队列，也就是wl_display->default_queue队列
    struct wl_event_queue *newq = wl_display_create_queue(display);
    wl_proxy_set_queue((struct wl_proxy *)registry, newq);

    wl_registry_add_listener(registry,
                     &registry_listener, display);

    struct wl_callback *cl = wl_display_sync(display);
    wl_callback_add_listener(cl, &call_back_listener, newq);

    // 如果使用下面的代码，那么就不会触发任何事件的派发了，就因为指定了另外的事件队列
//    newq = wl_display_create_queue(display);
//    wl_proxy_set_queue((struct wl_proxy *)cl, newq);

    wl_display_roundtrip(display);

    int ret = 1;
    do {
        ret = wl_display_dispatch(display);
    }while (ret);

    return 0;
}
