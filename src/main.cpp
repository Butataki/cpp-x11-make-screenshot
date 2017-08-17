#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <typeinfo>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include <png.h>
#include <zlib.h>
#include <jpeglib.h>
#include <sys/stat.h>
#include "screenshot.h"
#include "cmdopts.h"
using namespace std;

// main() is where program execution begins.

int main(int argc, char ** argv) {
    Display * display;
    Window root;
    int width = 0;
    int height = 0;
    XWindowAttributes gwa;
    X11Screenshot screenshot;
    // options
    cmd_options opts = process_options(argc, argv);
    if (opts.verbose) {
        cout << "Options" << endl;
        cout << "width: " << opts.width << endl;
        cout << "Height: " << opts.height << endl;
        cout << "Quality: " << opts.quality << endl;
        cout << "Verbose: " << opts.verbose << endl;
        cout << "Path: " << opts.path << endl;
        cout << "Type: " << opts.type << endl;
    }

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        cerr << "No display can be aquired" << endl;
        exit(1);
    }
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &gwa);
    width = gwa.width;
    height = gwa.height;

    if (opts.verbose) {
        cout << "Original screen width: " << width << endl;
        cout << "Original screen height: " << height << endl;
    }

    XImage * image = XGetImage(
        display,
        root,
        0,
        0,
        width,
        height,
        AllPlanes,
        ZPixmap
    );

    if (string(opts.path).length() == 0) {
        cerr << "Invalid arguments, use --help to see details." << endl;
        exit(0);
    };

    if ((opts.width != 0 and opts.width != width) and (opts.height != 0 and opts.height != height)) {
        screenshot = X11Screenshot(image, opts.width, opts.height, "bilinear");
        if (opts.verbose) cout << "Process with resizing" << endl;
    } else {
        screenshot = X11Screenshot(image);
        if (opts.verbose) cout << "Process without resizing" << endl;
    }

    if (string(opts.type) == "png") {
        if (opts.verbose) cout << "Saving as png" << endl;
        if (screenshot.save_to_png(opts.path)) {
            if (opts.verbose) cout << "Succesfully saved to " << opts.path << endl;
        }
    }

    if (string(opts.type) == "jpg") {
        if (opts.verbose) cout << "Saving as jpg" << endl;
        if (screenshot.save_to_jpeg(opts.path, opts.quality)) {
            if (opts.verbose) cout << "Succesfully saved to " << opts.path << endl;
        }
    }

    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
