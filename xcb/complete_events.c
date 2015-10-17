#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <xcb/xcb.h>


void print_modifiers(uint32_t mask) {
    const char *MODIFIERS[] = {
        "Shift", "Lock", "Ctrl", "Alt",
        "Mod2", "Mod3", "Mod4", "Mod5",
        "Button1", "Button2", "Button3",
        "Button4", "Button4", "Button5"
    };

    printf("Modifier mask: ");
    for (const char **modifier = MODIFIERS; mask; mask >>= 1, ++modifier) {
        if (mask & 1) {
            printf(*modifier);
        }
    }
    printf("\n");
}

int main() {
    xcb_connection_t *conn = xcb_connect(NULL, NULL);
    xcb_screen_t *scr = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    
    xcb_window_t win = xcb_generate_id(conn);

    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = { scr->white_pixel,
        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE
    };

    xcb_create_window(conn, 0, win, scr->root, 0, 0, 500, 500, 2,
            XCB_WINDOW_CLASS_INPUT_OUTPUT, scr->root_visual, mask, values);

    xcb_map_window(conn, win);
    xcb_flush(conn);

    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(conn))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE: {
                xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
                printf("Window %"PRIu32" exposed. Redrawing region at (%"PRIu16", %"
                        PRIu16"), dimensions (%"PRIu16", %"PRIu16")\n",
                        expose->window, expose->x, expose->y, expose->width, expose->height );
                break;
            } case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
                print_modifiers(bp->state);
                
                switch(bp->detail) {
                    case 4:
                        printf("MWB Up in window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                                bp->event, bp->event_x, bp->event_y );
                        break;
                    case 5:
                        printf("MWB Down in window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                                bp->event, bp->event_x, bp->event_y );
                        break;
                    default:
                        printf("Button press in window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                                bp->event, bp->event_x, bp->event_y );
                        break;
                }
                break;
            } case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
                print_modifiers(br->state);
                printf("Button %"PRIu32" release in window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                                br->detail, br->event, br->event_x, br->event_y );
                        break;
            } case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
                printf("Mouse moved in window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                        motion->event, motion->event_x, motion->event_y);
                break;
            } case XCB_ENTER_NOTIFY: {
                xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *)event;
                printf("Mouse entered window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                        enter->event, enter->event_x, enter->event_y);
                break;
            } case XCB_LEAVE_NOTIFY: {
                xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *)event;
                printf("Mouse left window %"PRIu32" at coords (%"PRIi16", %"PRIi16")\n",
                        leave->event, leave->event_x, leave->event_y);
                break;
            } case XCB_KEY_PRESS: {
                xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
                print_modifiers(kp->state);
                printf("Key pressed in window %"PRIu32"\n", kp->event);
                break;
            } case XCB_KEY_RELEASE: {
                xcb_key_release_event_t *kr = (xcb_key_release_event_t *)event;
                print_modifiers(kr->state);
                printf("Key released in window %"PRIu32"\n", kr->state);
                break;
            } default: {
                printf("Unknown event: %"PRIu8"\n", event->response_type);
                break;
            }
        }
        free(event);
    }
    return 0;
}
