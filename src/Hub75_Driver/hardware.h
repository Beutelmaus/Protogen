#pragma once
#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

/*

HUB 75 PANEL       
+----------+
|  R1  B1  |  //B1 and G1 pins are swapped by my panel
|  G1  GND |  
|  R2  B2  |  
|  G2  E   |  
|   A  B   |  
|   C  D   |  
| CLK  LAT |  
|  OE  GND |  
+----------+
*/
// HUB75 Matrix Panel Pin Definitions
//Left
#define R1_PIN 13  // Red channel for top half
#define G1_PIN 12  // Green channel for top half
#define R2_PIN 14  // Red channel for bottom half
#define G2_PIN 27  // Green channel for bottom half
#define A_PIN 26   // Row address A
#define C_PIN 25    // Row address C
#define CLK_PIN 33 // Clock pin
#define OE_PIN 32  // Output enable pin

//Right
#define B1_PIN 15  // Blue channel for top half
        //GND
#define B2_PIN 2  // Blue channel for bottom half
#define E_PIN 4   // Row address E
#define B_PIN 16   // Row address B
#define D_PIN 17   // Row address D
#define LAT_PIN 5  // Latch pin

// Panel Configuration
#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define CHAIN_LENGTH 1

// Global HUB75 matrix display object
extern MatrixPanel_I2S_DMA *dma_display;

// Global variables for received I2C data
extern volatile int currentProgram;
extern uint8_t selected_colour_value[3];

// Function declarations
void initDisplays();
void scanI2C();
void receiveI2CData(int numBytes);