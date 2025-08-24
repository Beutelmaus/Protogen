#include <Arduino.h>
#include "../hardware.h"

void rgbTest() {
  static int testState = 0;  // 0=RED, 1=GREEN, 2=BLUE, 3=WHITE
  static unsigned long stateStartTime = 0;
  static int lastTestState = -1; // Track when state changes
  
  const unsigned long STATE_DURATION = 1500;
  
  unsigned long currentTime = millis();
  
  // Reset if program changed
  if (currentProgram != 0) {
    testState = 0;
    lastTestState = -1;
    return; // Exit immediately
  }
  
  // Check if state changed and reset timer
  if (testState != lastTestState) {
    stateStartTime = currentTime;
    lastTestState = testState;
    
    // Display the new color
    switch (testState) {
      case 0: // RED
        Serial.println("Filling screen with RED...");
        dma_display->fillScreenRGB888(50, 0, 0);
        break;
        
      case 1: // GREEN
        Serial.println("Filling screen with GREEN...");
        dma_display->fillScreenRGB888(0, 50, 0);
        break;
        
      case 2: // BLUE
        Serial.println("Filling screen with BLUE...");
        dma_display->fillScreenRGB888(0, 0, 50);
        break;
        
      case 3: // WHITE
        Serial.println("Filling screen with WHITE...");
        dma_display->fillScreenRGB888(50, 50, 50);
        break;
    }
  }
  
  // Check if time to advance to next state
  unsigned long elapsedTime = currentTime - stateStartTime;
  if (elapsedTime >= STATE_DURATION) {
    testState++;
    if (testState > 3) {
      Serial.println("RGB test cycle complete.");
      testState = 0; // Reset to start over
    }
  }
}