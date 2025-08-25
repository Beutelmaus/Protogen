#pragma once

#include <Arduino.h>
#include <PCF8574.h>
#include <Adafruit_SSD1327.h>

// Declare global variables
extern PCF8574 IO_Module_1; // IO module

extern int currentProgram; // Declare currentProgram globally
extern uint8_t selected_colour_value[3]; // Declare selected_colour_value globally

// Declare display as an external global variable
extern Adafruit_SSD1327 display;

// Declare functions
void scanI2C();                          // Function to scan the I2C bus
void handleButtons();                    // Function to update button states
void SendDataToSlave(int program);       // Function to send data to the I2C slave
void initDisplay();