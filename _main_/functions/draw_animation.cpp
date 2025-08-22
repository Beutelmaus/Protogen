// REMOVE THIS LINE: #include "functions/draw_animation.cpp"  <-- DELETE THIS
#include "../hardware.h"
#include "../Animations/animations.cpp"

void draw_animation(const char* animationName, uint8_t r, uint8_t g, uint8_t b, int frameDelay) {
    static unsigned long lastFrameTime = 0;
    static int currentFrame = 0;
    
    // Check if it's time to advance to the next frame
    if (millis() - lastFrameTime >= frameDelay) {
        lastFrameTime = millis();
        
        // Select animation by name
        if (strcmp(animationName, "Smile") == 0) {
            // Draw current frame of Smile animation (12 frames)
            for (int y = 0; y < 32; y++) {
                for (int x = 0; x < 64; x++) {
                    uint8_t brightness = Smile[currentFrame][y][x];
                    uint8_t scaled_r = (uint16_t(r) * brightness) / 255;
                    uint8_t scaled_g = (uint16_t(g) * brightness) / 255;
                    uint8_t scaled_b = (uint16_t(b) * brightness) / 255;
                    dma_display->drawPixelRGB888(x, y, scaled_r, scaled_g, scaled_b);
                }
            }
            
            // Advance to next frame (loop back to 0 after frame 11)
            currentFrame++;
            if (currentFrame >= 12) {
                currentFrame = 0;
            }
        }
    }
}