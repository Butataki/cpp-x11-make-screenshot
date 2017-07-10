#include <iostream>
#include <cstdlib>
#include <png.h>
#include <jpeglib.h>
#include "screenshot.h"
using namespace std;

X11Screenshot::X11Screenshot(XImage * image) {
    this->width = image->width;
    this->height = image->height;
    this->image_data = this->process_rgb_image(image);
};

vector<vector<unsigned char>> X11Screenshot::process_rgb_image(XImage * image) {
    vector<vector<unsigned char>> image_data;
    vector<unsigned char> image_data_row;
    unsigned long red_mask = image->red_mask;
    unsigned long green_mask = image->green_mask;
    unsigned long blue_mask = image->blue_mask;

    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            unsigned long pixel = XGetPixel(image, x, y);

            unsigned char blue = pixel & blue_mask;
            unsigned char green = (pixel & green_mask) >> 8;
            unsigned char red = (pixel & red_mask) >> 16;

            image_data_row.push_back(red);
            image_data_row.push_back(green);
            image_data_row.push_back(blue);
        }
        image_data.push_back(image_data_row);
        image_data_row.clear();
    }

    return image_data;
};

bool X11Screenshot::save_to_png(const char * path) {
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row;

    fp = fopen(path, "wb");
    if (!fp) {
        cout << "Failed to create file " << path << endl;
        return false;
    }

    png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );
    if (!png_ptr) {
        cout << "Failed to create PNG write structure" << endl;
        fclose(fp);
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        cout << "Failed to create PNG info structure" << endl;
        fclose(fp);
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return false;
    }

    // Setup png lib error handling
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    png_init_io(png_ptr, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png_ptr,
        info_ptr,
        this->width,
        this->height,
        8,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    // set compression level
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    // write info header
    png_write_info(png_ptr, info_ptr);
    for(std::vector<vector<unsigned char>>::size_type i = 0; i != this->image_data.size(); i++) {
        row = (png_bytep) reinterpret_cast<unsigned char*>(this->image_data[i].data());
        // write byterow
        png_write_row(png_ptr, row);
    }
    // end writing file
    png_write_end(png_ptr, NULL);
    // cleanup
    if (fp != NULL) fclose(fp);
    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    return true;
};

bool X11Screenshot::save_to_jpeg(const char * path, int quality){
    FILE *fp = NULL;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY row;

    fp = fopen(path, "wb");
    if (!fp) {
        cout << "Failed to create file " << path << endl;
        return false;
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = this->width;
    cinfo.image_height = this->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality (&cinfo, quality, true);
    jpeg_start_compress(&cinfo, true);
    for(std::vector<vector<unsigned char>>::size_type i = 0; i != this->image_data.size(); i++) {
        row = (JSAMPARRAY) &this->image_data[i];
        jpeg_write_scanlines(&cinfo, row, 1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    if (fp != NULL) fclose(fp);
    return true;
};
