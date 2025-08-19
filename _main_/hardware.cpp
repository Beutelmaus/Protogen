#include "hardware.h"
#include <Arduino.h>

// Pin mapping (matches your hardware)
#define R1   25
#define G1   27
#define BL1  26
#define R2   14
#define G2   13
#define BL2  12
#define CH_E 32
#define CH_A 23
#define CH_B 19
#define CH_C  5
#define CH_D 17
#define CLK  16
#define LAT   4
#define OE   15

#define PANEL_WIDTH   64
#define PANEL_HEIGHT  32
#define PANELS_NUMBER 1

MatrixPanel_I2S_DMA *dma_display = nullptr;
int currentProgram = 0;

void initDisplay() {
  HUB75_I2S_CFG mxconfig;
  mxconfig.mx_height = PANEL_HEIGHT;
  mxconfig.chain_length = PANELS_NUMBER;
  mxconfig.gpio.e = CH_E;

  mxconfig.gpio.r1 = R1;
  mxconfig.gpio.g1 = G1;
  mxconfig.gpio.b1 = BL1;
  mxconfig.gpio.r2 = R2;
  mxconfig.gpio.g2 = G2;
  mxconfig.gpio.b2 = BL2;

  mxconfig.gpio.a = CH_A;
  mxconfig.gpio.b = CH_B;
  mxconfig.gpio.c = CH_C;
  mxconfig.gpio.d = CH_D;
  mxconfig.gpio.clk = CLK;
  mxconfig.gpio.lat = LAT;
  mxconfig.gpio.oe = OE;

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->setBrightness8(50);

  if (!dma_display->begin()) {
    Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
  }
}