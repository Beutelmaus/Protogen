#include <Wire.h>
#include "hardware.h"


#include "MenueAndButtons.h"
#include "Menue/Home_menu.h"
#include "Menue/Programs_menu.h"
#include "Menue/Brightness_menu.h"

#define SLAVE_ADDRESS 0x08 // I2C address of the slave

void setup() {
  Serial.begin(115200);
  Serial.println(F("*****************************************************"));
  Serial.println(F("*              Protogen booting                     *"));
  Serial.println(F("*****************************************************"));

  Wire.begin(); // Initialize I2C as master
  scanI2C();
  IO_Module_1.begin();
  initDisplay();
}

void loop() {
  // Update the menu system
  MenueAndButtons(); // Replace with the actual menu update function

  // Send data to slave if program changed or 10s passed
  static int lastProgramNumber = -1;
  static unsigned long lastSendTime = 0; 
  unsigned long currentTime = millis();
  if (currentProgram != lastProgramNumber || (currentTime - lastSendTime >= 10000)) {
    SendDataToSlave(currentProgram);
    lastProgramNumber = currentProgram;
    lastSendTime = currentTime;
  }
}


#include "Menue/Home_menu.cpp"
#include "Menue/Programs_menu.cpp"
#include "Menue/Colors_menu.cpp"
#include "Menue/Brightness_menu.cpp"
#include "Menue/Webpage_menu.cpp"
#include "Menue/Debug_menu.cpp"
#include "Menue/Standby_menu.cpp"