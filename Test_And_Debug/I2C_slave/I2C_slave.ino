#include <Wire.h>

#define SLAVE_ADDRESS 0x08 // I2C address of this slave

void setup() {
  Wire.begin(SLAVE_ADDRESS); // Initialize I2C as slave
  Wire.onReceive(receiveEvent); // Register receive event handler
  Serial.begin(115200);
  Serial.println("I2C Slave initialized");
}

void loop() {
  // Nothing to do here, everything is handled in the receiveEvent function
}

void receiveEvent(int numBytes) {
  while (Wire.available()) {
    int receivedValue = Wire.read(); // Read the received value
    Serial.print("Received: ");
    Serial.println(receivedValue);
  }
}