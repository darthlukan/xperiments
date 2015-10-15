#include <stdio.h>
#include <inttypes.h>
#include <xcb/xcb.h>


int main() {
    // Connect to $DISPLAY and screen 0
    xcb_connection_t *connection = xcb_connect(NULL, NULL);

    // Get a setup pointer using the connection
    // Must be a constant?
    const xcb_setup_t *setup = xcb_get_setup(connection);

    // Since *setup holds a list of screens, we access by iteration?
    // TODO: Did I misunderstand the documentation on ths one?
    xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;

    // Print the height and width in pixels of the screen
    printf("width\t: %"PRIu16"\n", screen->width_in_pixels);
    printf("height\t: %"PRIu16"\n", screen->height_in_pixels);

    xcb_disconnect(connection);

    return 0;
}
