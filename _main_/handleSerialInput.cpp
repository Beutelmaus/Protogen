#include <Arduino.h>
#include "hardware.h"

#include <Adafruit_SSD1327.h>
extern Adafruit_SSD1327 display;

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

  if (currentProgram == -1) currentProgram = 9;
  if (currentProgram == 10) currentProgram = 0;





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


  ///////////////////////////////////////////////////////////////////////////////////
  // Print Screen

    // Title
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Button Status:");
  
  // Button states
  display.setCursor(0, 16);
  display.print("BTN0: ");
  display.println(Button_input[0] ? "ON " : "OFF");
  
  display.setCursor(0, 26);
  display.print("BTN1: ");
  display.println(Button_input[1] ? "ON " : "OFF");
  
  display.setCursor(0, 36);
  display.print("BTN2: ");
  display.println(Button_input[2] ? "ON " : "OFF");
  
  display.setCursor(0, 46);
  display.print("BTN3: ");
  display.println(Button_input[3] ? "ON " : "OFF");
  
  // Output status
  display.setCursor(0, 56);
  display.print("OUT7: ");
  display.println(!Button_input[0] ? "HIGH" : "LOW");
  display.display();
}
