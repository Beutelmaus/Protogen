#pragma once
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <PCF8574.h>
#include <Adafruit_SSD1327.h>

extern MatrixPanel_I2S_DMA *dma_display;
extern int currentProgram;
extern PCF8574 IO_Module_1;
extern Adafruit_SSD1327 display;

void initDisplays();
void scanI2C();