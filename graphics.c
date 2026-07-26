#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

// Direct pointer to video memory provided by VESA
unsigned int* framebuffer = (unsigned int*) 0xFD000000;

/**
 * @brief The ONE function that powers the entire GUI.
 */
void draw_pixel(int x, int y, unsigned int color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y * SCREEN_WIDTH + x] = color;
    }
}
