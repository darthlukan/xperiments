#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>


int main() {

    // Yay rectangle definitions
    xcb_rectangle_t rectangles[] = {
        { 10, 50, 40, 20 },
        { 80, 50, 10, 40 }
    };

    xcb_connection_t *connection = xcb_connect(NULL, NULL);
    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;

    // Create the context
    // Get something to draw on
    xcb_drawable_t window = screen->root;
    // Allocate an ID for our context
    xcb_gcontext_t foreground = xcb_generate_id(connection);
    // Set the mask to our graphics context foreground
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    // ... the value of which will be black
    uint32_t values[2] = { screen->black_pixel, 0};
    // Finally, crreate the graphics context
    xcb_create_gc(connection, foreground, window, mask, values);

    // This is the actual window we want to draw on, the previous one was for context definition
    window = xcb_generate_id(connection);
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE;
    // Now generate the window (like before)
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, 500, 500,
            2, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);

    // Map and Flush
    // This gives us a window on the screen
    xcb_map_window(connection, window);
    xcb_flush(connection);
   
    // setup listening for events
    xcb_generic_event_t *event;
    // Loop for events
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            // The event val
            case XCB_EXPOSE:
                // Draw a rectangle (queue it up)
                xcb_poly_rectangle(connection, window, foreground, 2, rectangles);
                // Flush the queue, actually processes it
                xcb_flush(connection);
                break;
            // An event we don't care about, which is, all but one at this point
            default:
                break;

        }
        // Clean up
        free(event);
    }
    return 0;
}
