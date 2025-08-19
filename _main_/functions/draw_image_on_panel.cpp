#include "../hardware.h"

void draw_image_on_panel(const uint8_t image[][64][3], int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            dma_display->drawPixelRGB888(x, y, image[y][x][0], image[y][x][1], image[y][x][2]);
        }
    }
}