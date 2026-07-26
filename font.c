extern void draw_pixel(int x, int y, unsigned int color);

// 8x8 ASCII font matrix table (256 characters x 8 bytes)
extern const unsigned char font8x8[256][8]; 

void draw_char(int x, int y, char c, unsigned int color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((font8x8[(unsigned char)c][row] >> (7 - col)) & 1) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void draw_string(int x, int y, const char* str, unsigned int color) {
    for (int i = 0; str[i] != '\0'; i++) {
        draw_char(x + (i * 8), y, str[i], color);
    }
}
