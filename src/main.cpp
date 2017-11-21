#include <iostream>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include "screenshot.h"
#include "cmdopts.h"

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
        std::cout << "Options" << std::endl;
        std::cout << "width: " << opts.width << std::endl;
        std::cout << "Height: " << opts.height << std::endl;
        std::cout << "Quality: " << opts.quality << std::endl;
        std::cout << "Verbose: " << opts.verbose << std::endl;
        std::cout << "Path: " << opts.path << std::endl;
        std::cout << "Type: " << opts.type << std::endl;
    }

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::cerr << "No display can be aquired" << std::endl;
        exit(1);
    }
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &gwa);
    width = gwa.width;
    height = gwa.height;

    if (opts.verbose) {
        std::cout << "Original screen width: " << width << std::endl;
        std::cout << "Original screen height: " << height << std::endl;
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

    if (std::string(opts.path).length() == 0) {
        std::cerr << "Invalid arguments, use --help to see details." << std::endl;
        exit(0);
    };

    if ((opts.width != 0 and opts.width != width) and (opts.height != 0 and opts.height != height)) {
        screenshot = X11Screenshot(image, opts.width, opts.height, "bilinear");
        if (opts.verbose) std::cout << "Process with resizing" << std::endl;
    } else {
        screenshot = X11Screenshot(image);
        if (opts.verbose) std::cout << "Process without resizing" << std::endl;
    }

    if (std::string(opts.type) == "png") {
        if (opts.verbose) std::cout << "Saving as png" << std::endl;
        if (screenshot.save_to_png(opts.path)) {
            if (opts.verbose) std::cout << "Succesfully saved to " << opts.path << std::endl;
        }
    }

    if (std::string(opts.type) == "jpg") {
        if (opts.verbose) std::cout << "Saving as jpg" << std::endl;
        if (screenshot.save_to_jpeg(opts.path, opts.quality)) {
            if (opts.verbose) std::cout << "Succesfully saved to " << opts.path << std::endl;
        }
    }

    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
