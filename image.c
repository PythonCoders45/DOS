#include "image.h"

extern void draw_pixel(int x, int y, uint32_t color);

/**
 * @brief Draws a raw uncompressed image to the screen buffer
 * @param pos_x X coordinate on screen for top-left corner
 * @param pos_y Y coordinate on screen for top-left corner
 * @param img Pointer to the RawImage structure
 */
void draw_image(int pos_x, int pos_y, const RawImage* img) {
    if (!img || !img->pixel_data) return;

    for (uint32_t y = 0; y < img->height; y++) {
        for (uint32_t x = 0; x < img->width; x++) {
            // Calculate index in 1D pixel array: (row * width) + column
            uint32_t pixel_color = img->pixel_data[y * img->width + x];

            // Render pixel to screen coordinate
            draw_pixel(pos_x + x, pos_y + y, pixel_color);
        }
    }
}
