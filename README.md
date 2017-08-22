# cpp-c11-make-screenshot
Capture screen image with X11 and save it to jpg or png file
Includes library to e in project and standalone executable

### Makeshot installation

Install dev packages required, for example on ubuntu

```
$ sudo apt install libjpeg-dev libpng-dev libx11-dev
```

Build DAB package and install

```sh
$ cpack ./
```

or build with cmake

```sh
$ cmake ./
$ make
```

### Makeshot usage

Most basic usage

```sh
$ makeshot /tmp/screenshot.jpg
```

Use ```--help``` to learn additional options

### Screenshot lib usage

```c++
#include <iostream>
#include "screenshot.h"

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
if (screenshot.save_to_jpeg("original.jpg", 30))
    cout << "saved original jpeg" << endl;
if (screenshot.save_to_png("original.png"))
    cout << "saved original png" << endl;
X11Screenshot thumbnail = X11Screenshot(image, 1920, 540);
if (thumbnail.save_to_jpeg("linear_downscale.jpg", 50))
    cout << "saved linear downscaled jpeg" << endl;
if (thumbnail.save_to_png("linear_downscale.png"))
    cout << "saved linear downscaled png" << endl;
X11Screenshot upscale = X11Screenshot(image, 7680, 2160);
if (upscale.save_to_jpeg("linear_upscale.jpg", 30))
    cout << "saved linear upscale jpeg" << endl;
if (upscale.save_to_png("linear_upscale.png"))
    cout << "saved linear upscale png" << endl;
X11Screenshot bl_thumbnail = X11Screenshot(image, 2540, 720, "bilinear");
if (bl_thumbnail.save_to_jpeg("bilinear_downscale.jpg", 30))
    cout << "saved bilinear downscale jpeg" << endl;
if (bl_thumbnail.save_to_png("bilinear_downscale.png"))
    cout << "saved bilinear downscale png" << endl;
X11Screenshot b_upscale = X11Screenshot(image, 7680, 2160, "bilinear");
if (b_upscale.save_to_jpeg("bilinear_upscale.jpg", 30))
    cout << "saved bilinear upscale jpeg" << endl;
if (b_upscale.save_to_png("b_upscale_test.png"))
    cout << "saved bilinear upscale png" << endl;

XDestroyImage(image);
XCloseDisplay(display);
```
