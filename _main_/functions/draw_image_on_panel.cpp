#include "../hardware.h"

extern uint8_t selected_colour_value[3]; // Access the global RGB array
extern bool serverRunning; // Access webserver status

void draw_image_on_panel(const uint8_t image[][64][3]) {
    static unsigned long lastDrawTime = 0;
    static bool lastImageSame = false;
    static const uint8_t* lastImagePtr = nullptr;
    
    // If webserver is running, throttle drawing to reduce interference
    if (serverRunning) {
        unsigned long currentTime = millis();
        if (currentTime - lastDrawTime < 16) { // Limit to ~60 FPS when server is active
            return;
        }
        lastDrawTime = currentTime;
        
        // Skip redraw if it's the same static image
        if (lastImagePtr == (const uint8_t*)image && lastImageSame) {
            return;
        }
    }
    
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
        
        // Yield CPU periodically when webserver is running
        if (serverRunning && (y % 8 == 0)) {
            yield();
        }
    }
    
    lastImagePtr = (const uint8_t*)image;
    lastImageSame = true;
}