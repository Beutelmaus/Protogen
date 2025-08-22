#include <Arduino.h>
#include "hardware.h"

#include <Adafruit_SSD1327.h>
extern Adafruit_SSD1327 display;

#include "functions/Menues.h"

void MenueAndButtons() {

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
  // Menue Screen

  // Title
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Protogen - Menu");
  
  // Program status
  display.setCursor(0, 100);
  display.print("Program: ");
  display.println(getProgramName(currentProgram));

  // Display a color name
  display.print("Color: ");
  display.println(getColorName(2)); // Shows "Red"

  // Get RGB values for a color
  uint8_t r, g, b;
  getColorRGB(2, &r, &g, &b); // Gets RGB for "Red" (255, 0, 0)

  // All buttons in one compact line
  display.setCursor(0, 116);
  display.print( "0");display.print(Button_input[0] ? "L" : "H");
  display.print(" 1");display.print(Button_input[1] ? "L" : "H");
  display.print(" 2");display.print(Button_input[2] ? "L" : "H");
  display.print(" 3");display.print(Button_input[3] ? "L" : "H");

  display.display();
}
