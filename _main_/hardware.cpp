#include "hardware.h"
#include <Arduino.h>
#include <PCF8574.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_SSD1327.h>

// Add OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define OLED_RESET -1
#define OLED_ADDRESS 0x3D

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

PCF8574 IO_Module_1(0x20);
Adafruit_SSD1327 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000UL);

MatrixPanel_I2S_DMA *dma_display = nullptr;
int currentProgram = 0;
uint8_t selected_colour_value[3] = {255, 255, 255};

void initDisplays() {
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

  // Configure for better WiFi compatibility
  mxconfig.clkphase = false;
  mxconfig.latch_blanking = 4;  // Increase latch blanking for stability
  mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M; // Reduce I2S speed to minimize interference
  
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->setBrightness8(50);

  if (!dma_display->begin()) {
    Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
  }

  // Clear any potential artifacts
  dma_display->clearScreen();
  dma_display->flipDMABuffer();

  //////////////////////////////////////////////////////////////////////////////
  //1.5 Inch small OLED display
  if (!display.begin(OLED_ADDRESS)) {
    Serial.println("SSD1327 allocation failed");
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1327_WHITE);
  display.setCursor(0, 0);
  display.println("Protogen - Menu");
  display.display();
}

void scanI2C() {
  Serial.println("Scanning I2C bus for devices...");
  byte error, address;
  int deviceCount = 0;
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      deviceCount++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
}