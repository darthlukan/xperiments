#include <stdio.h>
#include <X11/Xlib.h>


int main() {
    // Open $DISPLAY
    Display *display = XOpenDisplay(NULL);
    // Get screen 0
    Screen *screen = XScreenOfDisplay(display, 0);
    int screen_w = XWidthOfScreen(screen);
    int screen_h = XHeightOfScreen(screen);

    printf("width\t: %i\n", screen_w);
    printf("height\t: %i\n", screen_h);

    return 0;
}
