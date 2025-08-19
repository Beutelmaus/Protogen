#include <Arduino.h>
#include "../hardware.h"
#include "handleSerialInput.h"

#define PANE_WIDTH  (64 * 1)
#define PANE_HEIGHT 32

void rgbTest() {
  // --- RED ---
  Serial.println("Filling screen with RED...");
  for (int y = 0; y < PANE_HEIGHT; y++) {
    for (int x = 0; x < PANE_WIDTH; x++) {
      handleSerialInput();
      if (currentProgram != 0) return; // exit immediately if program changed
      dma_display->drawPixelRGB888(x, y, 255, 0, 0);
      delay(5);
    }
  }
  delay(100);
  dma_display->fillScreenRGB888(0, 0, 0);
  delay(100);

  // --- GREEN ---
  Serial.println("Filling screen with GREEN...");
  for (int y = 0; y < PANE_HEIGHT; y++) {
    for (int x = 0; x < PANE_WIDTH; x++) {
      handleSerialInput();
      if (currentProgram != 0) return;
      dma_display->drawPixelRGB888(x, y, 0, 255, 0);
      delay(5);
    }
  }
  delay(100);
  dma_display->fillScreenRGB888(0, 0, 0);
  delay(100);

  // --- BLUE ---
  Serial.println("Filling screen with BLUE...");
  for (int y = 0; y < PANE_HEIGHT; y++) {
    for (int x = 0; x < PANE_WIDTH; x++) {
      handleSerialInput();
      if (currentProgram != 0) return;
      dma_display->drawPixelRGB888(x, y, 0, 0, 255);
      delay(5);
    }
  }
  delay(100);
  dma_display->fillScreenRGB888(0, 0, 0);
  delay(100);

  // --- White ---
  Serial.println("Filling screen with BLUE...");
  for (int y = 0; y < PANE_HEIGHT; y++) {
    for (int x = 0; x < PANE_WIDTH; x++) {
      handleSerialInput();
      if (currentProgram != 0) return;
      dma_display->drawPixelRGB888(x, y, 255, 255, 255);
      delay(10);
    }
  }
  delay(1000);
  dma_display->fillScreenRGB888(0, 0, 0);
  Serial.println("RGB test cycle complete.");
  delay(1000);
}255