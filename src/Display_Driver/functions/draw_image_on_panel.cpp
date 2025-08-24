#include "../hardware.h"

extern uint8_t selected_colour_value[3]; // Access the global RGB array

void draw_image_on_panel(const uint8_t image[][64][3]) {
    static unsigned long lastDrawTime = 0;
    static bool lastImageSame = false;
    static const uint8_t* lastImagePtr = nullptr;
    
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            // Get the original image brightness values
            uint8_t orig_r = image[y][x][0];
            uint8_t orig_g = image[y][x][1];
            uint8_t orig_b = image[y][x][2];
            
            // Calculate average brightness of the original pixel
            uint8_t brightness = (orig_r + orig_g + orig_b) / 3;
            
            // Apply selected color with original brightness
            uint8_t scaled_r = (uint16_t(selected_colour_value[0]) * brightness) / 255;
            uint8_t scaled_g = (uint16_t(selected_colour_value[1]) * brightness) / 255;
            uint8_t scaled_b = (uint16_t(selected_colour_value[2]) * brightness) / 255;
            
            dma_display->drawPixelRGB888(x, y, scaled_r, scaled_g, scaled_b);
        }
    }
    
    lastImagePtr = (const uint8_t*)image;
    lastImageSame = true;
}