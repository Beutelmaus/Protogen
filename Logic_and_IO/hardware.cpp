#include "hardware.h"
#include <Wire.h>

// Initialize the IO module
PCF8574 IO_Module_1(0x20); // Replace 0x20 with the actual I2C address

// Button states
static bool Button_input[4] = {};              // Current button states
static bool Button_input_last_Cycle[4] = {};  // Button states from the last cycle
bool Button_input_Pressed[4] = {};            // Button press detection

// Debounce timestamps
static unsigned long Button_LastTime[4] = {}; // Last press time for each button

// Debounce delay (in milliseconds)
const unsigned long debounceDelay = 200;

// Global program number
extern int programNumber;

void scanI2C() {
  Serial.println("Scanning I2C bus...");
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  Serial.println("I2C scan complete");
}

void handleButtons() {
  // Get the current time
  unsigned long currentTime = millis();

  // Update button states
  for (int i = 0; i < 4; i++) {
    Button_input_last_Cycle[i] = Button_input[i];
    Button_input[i] = IO_Module_1.read(i);

    // Detect button presses with debouncing
    if (Button_input[i] && !Button_input_last_Cycle[i] && (currentTime - Button_LastTime[i] > debounceDelay)) {
      Button_input_Pressed[i] = true;
      Button_LastTime[i] = currentTime; // Update the last press time
    } else {
      Button_input_Pressed[i] = false;
    }
  }

  // Handle program number increment and decrement
  if (Button_input_Pressed[3]) { // Button 3 is "Up"
    programNumber++;
    Serial.print("Program number increased to: ");
    Serial.println(programNumber);
  }

  if (Button_input_Pressed[2]) { // Button 2 is "Down"
    programNumber--;
    Serial.print("Program number decreased to: ");
    Serial.println(programNumber);
  }
}

void SendDataToSlave(int program) {
  static int colorCounter = 0; // Counter for cycling through colors
  
  // Define some test colors (R, G, B)
  uint8_t colors[][3] = {
    {255, 0, 0},    // Red
    {0, 255, 0},    // Green
    {0, 0, 255},    // Blue
    {255, 255, 0},  // Yellow
    {255, 0, 255},  // Magenta
    {0, 255, 255},  // Cyan
    {255, 255, 255} // White
  };
  int numColors = sizeof(colors) / sizeof(colors[0]);

  // Start I2C transmission to the slave
  Wire.beginTransmission(0x08); // Replace 0x08 with the actual slave address
  Wire.write(0x01);             // Command byte
  Wire.write(program);          // Send the program number

  // Send the 3 color values (R, G, B)
  colorCounter = program % numColors; // Test
  Wire.write(colors[colorCounter][0]);     // Red
  Wire.write(colors[colorCounter][1]);     // Green
  Wire.write(colors[colorCounter][2]);     // Blue

  uint8_t result = Wire.endTransmission(); // End the transmission

  if (result == 0) {
    Serial.print("Sent program: ");
    Serial.print(program);
    Serial.print(" with color RGB(");
    Serial.print(colors[colorCounter][0]);
    Serial.print(", ");
    Serial.print(colors[colorCounter][1]);
    Serial.print(", ");
    Serial.print(colors[colorCounter][2]);
    Serial.println(")");
  } else {
    Serial.print("I2C transmission failed with error: ");
    Serial.println(result);
  }
}