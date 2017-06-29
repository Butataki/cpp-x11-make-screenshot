#include <iostream>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include <png.h>
#include <zlib.h>
#include <jpeglib.h>
#include <vector>
#include "screenshot.cpp"
using namespace std;


// main() is where program execution begins.

int main() {
    Display *display;
    Window root;
    int width = 0;
    int height = 0;
    XWindowAttributes gwa;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        cout << "No display can be aquired" << endl;
        return (1);
    }
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &gwa);
    width = gwa.width;
    height = gwa.height;
    cout << "Width: " <<  width << "; Height: " << height << endl;
    XImage *image = XGetImage(
        display,
        root,
        0,
        0,
        width,
        height,
        AllPlanes,
        ZPixmap
    );

    X11Screenshot screenshot = X11Screenshot(image);
    if (screenshot.save_to_jpeg("test.jpg"))
        cout << "saved jpeg" << endl;
    if (screenshot.save_to_png("test.png"))
        cout << "saved png" << endl;
    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
