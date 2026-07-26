#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

unsigned int back_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
unsigned int* gpu_framebuffer = (unsigned int*) 0xFD000000;

void draw_pixel(int x, int y, unsigned int color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        back_buffer[y * SCREEN_WIDTH + x] = color;
    }
}

void swap_buffers(void) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        gpu_framebuffer[i] = back_buffer[i];
    }
}
