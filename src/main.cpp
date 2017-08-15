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
#include <string>
#include <vector>
#include "screenshot.h"
using namespace std;

static const string VERSION = "17.08.15.1";

struct cmd_options {
    const char * path;
    const char * type;
    int width;
    int height;
    int quality;
    bool verbose;
    cmd_options() = default;
};

void print_help() {
    printf("help message\n");
};

vector<string> process_opt_equal_sign(string arg) {
    string value = "";
    string name = "";
    size_t equal_sign_position = arg.find_first_of("=");
    if (equal_sign_position > arg.length()) { // not = sign in argument
        return {arg, value};
    }
    value = arg.substr(equal_sign_position + 1);
    name = arg.substr(0, equal_sign_position);
    return {name, value};
};

bool check_opt(string short_name, string long_name, string target) {
    if (target.length() == 2 and target == short_name) return true;
    if (target.length() > 2 and target == long_name) return true;
    return false;
};

cmd_options process_options(int argc, char ** argv) {
    cmd_options opts;
    // init defaults
    opts.width = 0;
    opts.height = 0;
    opts.quality = 100;
    opts.verbose = false;
    opts.path = "";
    opts.type = "";
    // process optional parameters
    if (argc <= 1) {
        print_help();
        exit(0);
    }
    //
    int i;
    for (i=1; i<argc; i++) {
        cout << "argc " << i << " argv " << argv[i] << endl;
        string sArgValue = string(argv[i]);
        vector<string> opt = process_opt_equal_sign(sArgValue);
        // check special commands first
        if (check_opt("", "--version", opt[0])) {
            cout << "makeshot version is " << VERSION << endl;
            exit(0);
        }
        if (check_opt("-?", "--help", opt[0])) {
            print_help();
            exit(0);
        }
        // optional params
        if (check_opt("-w", "--width", opt[0])) {
            if (opt[1].length() == 0 and argv[i+1] != NULL) {
                opt[1] = string(argv[i+1]);
            }
            opts.width = atoi(opt[1].c_str());
            if (opts.width < 0) opts.width = opts.width * (-1);
            continue;
        }
        if (check_opt("-h", "--height", opt[0])) {
            if (opt[1].length() == 0 and argv[i+1] != NULL) {
                opt[1] = string(argv[i+1]);
            }
            opts.height = atoi(opt[1].c_str());
            if (opts.height < 0) opts.height = opts.height * (-1);
            continue;
        }
        if (check_opt("-q", "--quality", opt[0])) {
            if (opt[1].length() == 0 and argv[i+1] != NULL) {
                opt[1] = string(argv[i+1]);
            }
            opts.quality = atoi(opt[1].c_str());
            if (opts.quality < 0) opts.quality = opts.quality * (-1);
            if (opts.quality > 100) opts.quality = 100;
            continue;
        }
        if (check_opt("-v", "--verbose", opt[0])) {
            opts.verbose = true;
            continue;
        }
        // positional param
        if (i == 1) {
            // pick positional argument `path` to where save screenshot
            const char * path = argv[i];
            // if path empty show help message
            if (path == NULL) print_help();
            opts.path = path;
            // get file extension and validate path
            string sPath = string(path);
            size_t dot_position = sPath.find_last_of(".");
            size_t slash_position = sPath.find_last_of("/");
            string file_name = sPath.substr(
                slash_position + 1, dot_position - slash_position - 1
            );
            if (dot_position > sPath.length()) {
                cerr << path << " - malformed path to file" << endl;
                exit(0);
            }
            if (file_name.length() == 0 ) {
                cerr << path << " - malformed path to file" << endl;
                exit(0);
            }
            string extension = sPath.substr(dot_position + 1);
            if (extension.length() == 0) {
                cerr << path << " - malformed path to file" << endl;
                exit(0);
            }
            if (extension == "jpeg" || extension == "jpg") opts.type = "jpg";
            else if (extension == "png") opts.type = "png";
            else {
                cerr << extension << " - is not supported file extension" << endl;
                exit(0);
            }
        }
    }

    return opts;
};

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

    if (!(opts.path and opts.type)) {
        print_help();
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
