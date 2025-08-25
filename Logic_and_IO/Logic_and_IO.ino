#include <Wire.h>
#include "hardware.h"

#define SLAVE_ADDRESS 0x08 // I2C address of the slave

int programNumber = 0; // Declare programNumber here

void setup() {
  Serial.begin(115200);
  Serial.println(F("*****************************************************"));
  Serial.println(F("*              Protogen booting                     *"));
  Serial.println(F("*****************************************************"));

  Wire.begin(); // Initialize I2C as master
  scanI2C();
  IO_Module_1.begin();  
}

void loop() {
  

  handleButtons(); // Update button states and handle program number changes

  // Send data to slave if program changed or 5s passed
  static int lastProgramNumber = -1;
  static unsigned long lastSendTime = 0; 
  unsigned long currentTime = millis();
  if (programNumber != lastProgramNumber || (currentTime - lastSendTime >= 5000)) {
    SendDataToSlave(programNumber);
    lastProgramNumber = programNumber;
    lastSendTime = currentTime;
  }
}
