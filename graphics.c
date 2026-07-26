#include "draw.h"

// Back Buffer in RAM memory to prevent screen tearing/flicker
uint32_t back_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

// Hardware VESA Framebuffer address pointing to physical GPU VRAM
uint32_t* gpu_framebuffer = (uint32_t*) 0xFD000000;

// Embedded $8 \times 8$ Font Bitmap Data for basic ASCII rendering
extern const uint8_t font8x8_basic[128][8];

/**
 * @brief Draws a single color pixel to the back buffer
 */
void draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        back_buffer[y * SCREEN_WIDTH + x] = color;
    }
}

/**
 * @brief Draws a filled solid rectangle
 */
void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            draw_pixel(x + j, y + i, color);
        }
    }
}

/**
 * @brief Draws a hollow rectangular border outline
 */
void draw_rect_outline(int x, int y, int width, int height, uint32_t color) {
    for (int j = 0; j < width; j++) {
        draw_pixel(x + j, y, color);                  // Top border
        draw_pixel(x + j, y + height - 1, color);     // Bottom border
    }
    for (int i = 0; i < height; i++) {
        draw_pixel(x, y + i, color);                  // Left border
        draw_pixel(x + width - 1, y + i, color);      // Right border
    }
}

/**
 * @brief Draws a single character using an 8x8 bitmap font
 */
void draw_char(int x, int y, char c, uint32_t color) {
    uint8_t ascii_index = (uint8_t)c;
    if (ascii_index > 127) return;

    for (int row = 0; row < 8; row++) {
        uint8_t byte = font8x8_basic[ascii_index][row];
        for (int col = 0; col < 8; col++) {
            if (byte & (1 << col)) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

/**
 * @brief Renders a full text string
 */
void draw_string(int x, int y, const char* str, uint32_t color) {
    int cur_x = x;
    int cur_y = y;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            cur_x = x;
            cur_y += 10; // Move to next line (8 pixels height + 2 padding)
        } else {
            draw_char(cur_x, cur_y, str[i], color);
            cur_x += 8;  // Move character cursor 8 pixels right
        }
    }
}

/**
 * @brief Renders a complete Windows XP style application window frame
 */
void draw_window(int x, int y, int width, int height, const char* title) {
    // 1. Draw Main Window Background
    draw_rect(x, y, width, height, COLOR_LIGHT_GRAY);
    
    // 2. Draw 3D Window Outer Border Outline
    draw_rect_outline(x, y, width, height, COLOR_DARK_GRAY);

    // 3. Draw Top Title Bar Header
    draw_rect(x + 2, y + 2, width - 4, 20, COLOR_XP_BLUE);

    // 4. Render Window Title Text
    draw_string(x + 8, y + 6, title, COLOR_WHITE);

    // 5. Draw Top-Right Close Button ('X')
    draw_rect(x + width - 20, y + 4, 16, 16, COLOR_RED);
    draw_char(x + width - 15, y + 8, 'X', COLOR_WHITE);
}

/**
 * @brief Fills the entire screen back buffer with a solid background color
 */
void clear_screen(uint32_t color) {
    draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

/**
 * @brief Copies the completed back buffer frame to physical GPU memory
 */
void swap_buffers(void) {
    for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        gpu_framebuffer[i] = back_buffer[i];
    }
}
