#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"

// Raw Uncompressed RGB Image Data Structure
typedef struct {
    uint32_t width;
    uint32_t height;
    const uint32_t* pixel_data; // Array of 32-bit Hex Colors (0x00RRGGBB)
} RawImage;

void draw_image(int pos_x, int pos_y, const RawImage* img);

#endif // IMAGE_H
