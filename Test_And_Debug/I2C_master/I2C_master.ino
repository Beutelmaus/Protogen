#include <Wire.h>

#define SLAVE_ADDRESS 0x08 // I2C address of the slave

void setup() {
  Wire.begin(); // Initialize I2C as master
  Serial.begin(115200);
  Serial.println("I2C Master initialized");

  // Perform an I2C scan
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

void loop() {
  static int counter = 0; // Counter to send to the slave
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
  
  // Condition to determine when to send data (always true in this example)
  bool shouldSend = true; // You can change this condition as needed
  
  if (shouldSend) {
    // Start I2C transmission to the slave
    Wire.beginTransmission(SLAVE_ADDRESS);
    
    // Send command byte (0x01 for program change)
    Wire.write(0x01);
    
    // Send the program number (cycling through 0-3)
    int programNumber = counter % 9;
    Wire.write(programNumber);
    
    // Send the 3 color values (R, G, B)
    Wire.write(colors[colorCounter][0]); // Red
    Wire.write(colors[colorCounter][1]); // Green
    Wire.write(colors[colorCounter][2]); // Blue
    
    uint8_t result = Wire.endTransmission(); // End the transmission
    
    if (result == 0) {
      Serial.print("Sent program: ");
      Serial.print(programNumber);
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
    
    // Increment counters
    counter++;
    colorCounter = (colorCounter + 1) % numColors; // Cycle through colors
  } else {
    Serial.println("Condition false - not sending data");
  }

  delay(2000); // Wait for 2 seconds
}