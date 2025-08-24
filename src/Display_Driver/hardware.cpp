#include "hardware.h"

// HUB75 Matrix Panel Configuration
HUB75_I2S_CFG mxconfig(
  PANEL_RES_X,   // Panel width
  PANEL_RES_Y,   // Panel height
  CHAIN_LENGTH   // Number of panels chained together
);

// HUB75 Matrix display object
MatrixPanel_I2S_DMA *dma_display = nullptr;

// Define global variables for received I2C data
volatile int currentProgram = 1;
uint8_t selected_colour_value[3] = {255, 255, 255};

void initDisplays() {
  Serial.println("Initializing HUB75 Matrix Display...");
  
  // Configure HUB75 pins
  mxconfig.gpio.r1 = R1_PIN;
  mxconfig.gpio.g1 = G1_PIN;
  mxconfig.gpio.b1 = B1_PIN;
  mxconfig.gpio.r2 = R2_PIN;
  mxconfig.gpio.g2 = G2_PIN;
  mxconfig.gpio.b2 = B2_PIN;
  mxconfig.gpio.a = A_PIN;
  mxconfig.gpio.b = B_PIN;
  mxconfig.gpio.c = C_PIN;
  mxconfig.gpio.d = D_PIN;
  mxconfig.gpio.e = E_PIN;  // Use -1 if not used
  mxconfig.gpio.lat = LAT_PIN;
  mxconfig.gpio.oe = OE_PIN;
  mxconfig.gpio.clk = CLK_PIN;
  
  // Initialize matrix display
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(50);
  dma_display->clearScreen();
  
  // Startup test pattern
  dma_display->fillScreen(dma_display->color565(50, 0, 0));
  delay(300);
  dma_display->fillScreen(dma_display->color565(0, 50, 0));
  delay(300);
  dma_display->fillScreen(dma_display->color565(0, 0, 50));
  delay(300);
  dma_display->clearScreen();
  
  Serial.println("HUB75 Matrix Display initialized successfully");
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