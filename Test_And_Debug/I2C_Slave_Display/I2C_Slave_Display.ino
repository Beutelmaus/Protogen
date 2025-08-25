#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Wire.h>
#include "hardware.h"
#include "drawColoredNumber.h"//Test graphics

// I2C Configuration
#define DISPLAY_DRIVER_I2C_ADDRESS 0x08

volatile bool programChanged = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("*****************************************************"));
  Serial.println(F("*            Display_Driver Controller              *"));
  Serial.println(F("*****************************************************"));


  // Initialize I2C as slave
  Wire.begin(DISPLAY_DRIVER_I2C_ADDRESS);
  Wire.onReceive(receiveI2CData);
  Serial.print("I2C Slave initialized at address 0x");
  Serial.println(DISPLAY_DRIVER_I2C_ADDRESS, HEX);

  // Initialize HUB75 LED matrix display
  initDisplays();
  
  Serial.println("Display driver ready");
  Serial.println("Waiting for commands from Logic_and_IO...");
}

void loop() {
  // Handle program display based on received I2C commands
  switch (currentProgram) {
    case -1: currentProgram = 1; break;
    case 0: drawColoredNumber(0,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 1: drawColoredNumber(1,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 2: drawColoredNumber(2,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 3: drawColoredNumber(3,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 4: drawColoredNumber(4,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 5: drawColoredNumber(5,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 6: drawColoredNumber(6,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 7: drawColoredNumber(7,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 8: drawColoredNumber(8,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 9: drawColoredNumber(9,selected_colour_value[0],selected_colour_value[1],selected_colour_value[2]); break;
    case 10: currentProgram = 1; break;
  }
  
  // Check for program changes
  if (programChanged) {
    programChanged = false;
    Serial.print("I2C Command, Program: ");
    Serial.print(currentProgram);
    Serial.print(" RGB(");
    Serial.print(selected_colour_value[0]);
    Serial.print(",");
    Serial.print(selected_colour_value[1]);
    Serial.print(",");
    Serial.print(selected_colour_value[2]);
    Serial.println(")");
  }
}








void receiveI2CData(int numBytes) {
  if (numBytes >= 5) { // Command + program + RGB values
    uint8_t command = Wire.read();
    
    if (command == 0x01) { // Program change command
      currentProgram = Wire.read();
      selected_colour_value[0] = Wire.read(); // Red
      selected_colour_value[1] = Wire.read(); // Green
      selected_colour_value[2] = Wire.read(); // Blue
      programChanged = true;
    }
  } else {
    Serial.println("Invalid data length");
    // Read all received bytes for debugging
    while (Wire.available()) {
      int receivedValue = Wire.read();
      Serial.print("Received byte: ");
      Serial.println(receivedValue);
    }
  }

  // Clear any remaining bytes
  while (Wire.available()) {
    int byte = Wire.read();
    Serial.print("Extra byte: ");
    Serial.println(byte);
  }
}
