#include <Arduino.h>
#include "../hardware.h"

PCF8574 IO_Module_1(0x20); // I2C Adress

void handleSerialInput() {

  static bool Button_input[4] = {};
  static bool Button_input_last_Cycle[4] = {};
  static bool Button_input_Pressed[4] = {};

  for (int i = 0; i < 4; i++) {//Detect rising edge
    Button_input_last_Cycle[i] = Button_input[i];
    Button_input[i] = IO_Module_1.read(i);

    Button_input_Pressed[i] = Button_input[i] && !Button_input_last_Cycle[i];
  }

  //Logic Buttons 1 and 2
  if (Button_input_Pressed[0]) {currentProgram++;}
  if (Button_input_Pressed[1]) {currentProgram--; }

  if (currentProgram == -1) currentProgram = 6;
  if (currentProgram == 7) currentProgram = 0;





  ///////////////////////////////////////////////////////////////////////////////////
  // Print currentProgram every 500ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    Serial.print("Current Program: ");
    Serial.print(currentProgram);

    Serial.print("    Button States: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(Button_input[i]);
    }
  Serial.println("");
  }
}
