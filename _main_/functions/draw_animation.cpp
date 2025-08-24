#include "../hardware.h"
#include "../Animations/animations.cpp"

extern uint8_t selected_colour_value[3];
extern bool serverRunning; // Access webserver status

void draw_animation(const char* animationName, int frameDelay) {
    static unsigned long lastFrameTime = 0;
    static int currentFrame = 0;
    
    // Adjust frame delay when webserver is running to maintain smooth display
    int adjustedDelay = frameDelay;
    if (serverRunning) {
        adjustedDelay = max(frameDelay, 50); // Minimum 50ms between frames when server is active
    }
    
    // Check if it's time to advance to the next frame
    if (millis() - lastFrameTime >= adjustedDelay) {
        lastFrameTime = millis();
        
        // Select animation by name
        if (strcmp(animationName, "Smile_blink") == 0) {
            // Draw current frame of Smile_blink animation (12 frames)
            for (int y = 0; y < 32; y++) {
                for (int x = 0; x < 64; x++) {
                    uint8_t brightness = Smile_blink[currentFrame][y][x];
                    
                    // Use selected_colour_value instead of passed parameters
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
            
            // Advance to next frame (loop back to 0 after frame 11)
            currentFrame++;
            if (currentFrame >= 12) {
                currentFrame = 0;
            }
        }
    }
}