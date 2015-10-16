#include <stdio.h>
#include <xcb/xcb.h>


int main() {
    // Boilerplate necessary to get a screen
    xcb_connection_t *connection = xcb_connect(NULL, NULL);
    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;

    // Create a window
    // First we need to generate an ID using this connection
    xcb_window_t window = xcb_generate_id(connection);
    // Then we define our window
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, 500, 500,
            2, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);
    // Finally, we draw it
    xcb_map_window(connection, window);

    // Not sure about this flush yet...
    xcb_flush(connection);

    // Keeps the window alive until Ctrl-C is pressed
    pause();

    // Cleanup
    xcb_disconnect(connection);
    return 0;
}
