#pragma once

#include <Arduino.h>
#include <PCF8574.h>

// Declare global variables
extern PCF8574 IO_Module_1; // IO module

// Button states
extern bool Button_Up_Pressed;
extern bool Button_Down_Pressed;

// Declare functions
void scanI2C();                          // Function to scan the I2C bus
void handleButtons();                    // Function to update button states
void SendDataToSlave(int program);       // Function to send data to the I2C slave