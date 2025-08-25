#include "drawColoredNumber.h"
#include "hardware.h"

void drawColoredNumber(int number, uint8_t red, uint8_t green, uint8_t blue) {
  dma_display->fillScreen(0); // Black background
  dma_display->setTextSize(4);
  dma_display->setTextWrap(false);
  
  // Use the provided RGB color values
  uint16_t color = dma_display->color565(red, green, blue);
  dma_display->setTextColor(color);
  
  // Center the number on the panel
  int16_t x1, y1;
  uint16_t w, h;
  String numStr = String(number);
  dma_display->getTextBounds(numStr, 0, 0, &x1, &y1, &w, &h);
  
  int centerX = (dma_display->width() - w) / 2;
  int centerY = (dma_display->height() - h) / 2;
  
  dma_display->setCursor(centerX, centerY);
  dma_display->print(numStr); // Draw the number
}