#include "types.h"

int mouse_x = 512; // Start at center of 1024x768 screen
int mouse_y = 384;
uint8_t mouse_bytes[3];
uint8_t mouse_cycle = 0;

extern void draw_rect(int x, int y, int w, int h, uint32_t color);

/**
 * @brief Handles PS/2 Mouse Interrupt (IRQ 12)
 */
void mouse_handler(uint8_t scancode) {
    switch (mouse_cycle) {
        case 0:
            mouse_bytes[0] = scancode;
            mouse_cycle++;
            break;
        case 1:
            mouse_bytes[1] = scancode;
            mouse_cycle++;
            break;
        case 2:
            mouse_bytes[2] = scancode;
            
            // Calculate relative offset
            int rel_x = (int8_t)mouse_bytes[1];
            int rel_y = (int8_t)mouse_bytes[2];

            mouse_x += rel_x;
            mouse_y -= rel_y; // Invert Y for screen space

            // Clamp mouse inside screen bounds
            if (mouse_x < 0) mouse_x = 0;
            if (mouse_y < 0) mouse_y = 0;
            if (mouse_x > 1023) mouse_x = 1023;
            if (mouse_y > 767) mouse_y = 767;

            mouse_cycle = 0;
            break;
    }
}

/**
 * @brief Draws a simple $5 \times 5$ square mouse cursor
 */
void draw_mouse_cursor(void) {
    draw_rect(mouse_x, mouse_y, 5, 5, 0x00FF0000); // Red Cursor Pointer
}
