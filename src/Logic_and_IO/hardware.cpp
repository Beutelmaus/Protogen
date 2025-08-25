#include "hardware.h"
#include <Wire.h>
#include <Adafruit_SSD1327.h>

// I2C Addresses
#define OLED_ADDRESS 0x3D
#define IO_Module_1_ADDRESS 0x20

// New I2C pins for the display
#define DISPLAY_SDA 32 // Replace with your chosen SDA pin
#define DISPLAY_SCL 33 // Replace with your chosen SCL pin

// Create a new I2C instance for the display
TwoWire displayWire = TwoWire(1);

// Initialize the IO module
PCF8574 IO_Module_1(IO_Module_1_ADDRESS); // I/O extender

// Debounce timestamps
static unsigned long Button_LastTime[4] = {}; // Last press time for each button


// Variables for Display Driver
int currentProgram = 1; // Define currentProgram
uint8_t selected_colour_value[3] = {50, 50, 50}; // Define selected_colour_value

void scanI2C() {
  Serial.println("Scanning first I2C bus...");
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  Serial.println("Scanning second I2C bus...");
  for (uint8_t address = 1; address < 127; address++) {
    displayWire.beginTransmission(address);
    if (displayWire.endTransmission() == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
}



// Send data to slave device
void SendDataToSlave(int program) {
  // Start I2C transmission to the slave
  Wire.beginTransmission(0x08); // Replace 0x08 with the actual slave address
  Wire.write(0x01);             // Command byte
  Wire.write(program);          // Send the program number

  // Send the selected color values (R, G, B)
  Wire.write(selected_colour_value[0]); // Red
  Wire.write(selected_colour_value[1]); // Green
  Wire.write(selected_colour_value[2]); // Blue

  uint8_t result = Wire.endTransmission(); // End the transmission

  if (result == 0) {
    Serial.print("Sent program: ");
    Serial.print(program);
    Serial.print(" with color RGB(");
    Serial.print(selected_colour_value[0]);Serial.print(", ");
    Serial.print(selected_colour_value[1]);Serial.print(", ");
    Serial.print(selected_colour_value[2]);Serial.println(")");
  } else {
    Serial.print("I2C transmission failed with error: ");
    Serial.println(result);
  }
}


void initDisplay() {
  // Initialize the new I2C bus for the display
  displayWire.begin(DISPLAY_SDA, DISPLAY_SCL);

  // Initialize the display
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
// Define the display object
Adafruit_SSD1327 display = Adafruit_SSD1327(128, 128, &displayWire);

