#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>


const int x = 1920;
const int y = 24;
const int w = 500;
const int h = 250;
const int bw = 2;


int main() {

    xcb_connection_t    *conn        = xcb_connect(NULL, NULL);
    xcb_screen_t        *screen      = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    xcb_drawable_t      win          = screen->root;
    xcb_gcontext_t      fg           = xcb_generate_id(conn);

    uint32_t    mask        = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t    values[2]   = { screen->black_pixel, 0 };

    xcb_create_gc(conn, fg, win, mask, values);

    win         = xcb_generate_id(conn);
    mask        = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0]   = screen->white_pixel;
    values[1]   = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS;

    xcb_create_window(conn, XCB_COPY_FROM_PARENT, win, screen->root, x, y, w, h, bw,
            XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);

    xcb_map_window(conn, win);
    xcb_flush(conn);

    xcb_generic_event_t *event;
    // xcb_poll_for_event does not block, so this will only catch the EXPOSE event then die out.
    // If we used xcb_wait_for_event, we'd block, thus continuing the loop and would actually
    // have a window to receive a possible XCB_BUTTON_PRESS.
    while ((event = xcb_poll_for_event(conn))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE:
                printf("Expose event\n");
                xcb_flush(conn);
                break;
            case XCB_BUTTON_PRESS:
                printf("Button press\n");
                xcb_flush(conn);
                break;
            default:
                printf("Some otherrr event\n");
                break;
        }
        free(event);
    }
    return 0;
}
