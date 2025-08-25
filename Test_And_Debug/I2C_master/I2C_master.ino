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

  // Start I2C transmission to the slave
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(counter); // Send the counter value
  Wire.endTransmission(); // End the transmission

  Serial.print("Sent: ");
  Serial.println(counter);

  counter++; // Increment the counter
  delay(1000); // Wait for 1 second
}