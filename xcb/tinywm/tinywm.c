/* TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 * 
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. 
 *
 * This xcb version adapted by me from the version https://github.com/rtyler/tinywm-ada/blob/master/tinywm.c 
 * by Ping-Hsun Chen
 */

#include <stdlib.h>
#include <xcb/xcb.h>

int main(void) {
    xcb_connection_t *dsp = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(dsp)) { return 1; }
    xcb_screen_t *scr = xcb_setup_roots_iterator(xcb_get_setup(dsp)).data;
    xcb_drawable_t root = scr->root;
    xcb_drawable_t win;
    xcb_generic_event_t *evt;
    xcb_get_geometry_reply_t *geo;
    uint32_t values[3];

    xcb_grab_key(dsp, 1, root, XCB_MOD_MASK_2, XCB_NO_SYMBOL, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
    xcb_grab_button(dsp, 0, root, XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
            XCB_GRAB_MODE_ASYNC, root, XCB_NONE, 1, XCB_MOD_MASK_1);
    xcb_grab_button(dsp, 0, root, XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
            XCB_GRAB_MODE_ASYNC, root, XCB_NONE, 3, XCB_MOD_MASK_1);
    xcb_flush(dsp);

    while((evt = xcb_wait_for_event(dsp))) {
        switch(evt->response_type & ~0x80) {
            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t *e = (xcb_button_press_event_t *)evt;
                win = e->child;
                values[0] = XCB_STACK_MODE_ABOVE;
                xcb_configure_window(dsp, win, XCB_CONFIG_WINDOW_STACK_MODE, values);
                geo = xcb_get_geometry_reply(dsp, xcb_get_geometry(dsp, win), NULL);
                if (e->detail == 1) {
                    values[2] = e->detail;
                    xcb_warp_pointer(dsp, XCB_NONE, win, 0, 0, 0, 0, 1, 1);
                } else if (values[2] == 3) {
                    xcb_warp_pointer(dsp, XCB_NONE, win, 0, 0, 0, 0, geo->width, geo->height);
                }
                xcb_grab_pointer(dsp, 0, root, XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION |
                        XCB_EVENT_MASK_POINTER_MOTION_HINT, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, 
                        root, XCB_NONE, XCB_CURRENT_TIME);
                break;
            } case XCB_MOTION_NOTIFY: {
                xcb_query_pointer_reply_t *pointer = xcb_query_pointer_reply(dsp, xcb_query_pointer(dsp, root), 0);
                if (values[2] == 1) {
                    geo = xcb_get_geometry_reply(dsp, xcb_get_geometry(dsp, win), NULL);
                    values[0] = (pointer->root_x + geo->width > scr->width_in_pixels) ? 
                        (scr->width_in_pixels - geo->width) : pointer->root_x;
                    values[1] = (pointer->root_y + geo->height > scr->height_in_pixels) ?
                        (scr->height_in_pixels - geo->height) : pointer->root_y;
                    xcb_configure_window(dsp, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
                } else if (values[2] == 3) {
                    geo = xcb_get_geometry_reply(dsp, xcb_get_geometry(dsp, win), NULL);
                    values[0] = pointer->root_x - geo->x;
                    values[1] = pointer->root_y - geo->y;
                    xcb_configure_window(dsp, win, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
                }
                break;
            } case XCB_BUTTON_RELEASE: {
                xcb_ungrab_pointer(dsp, XCB_CURRENT_TIME);
                break;
            }
            xcb_flush(dsp);
            free(evt);
            free(geo);
        }
    }
    return 0;
}
