#include "Programs_menu.h"
#include "../hardware.h"
#include <Adafruit_SSD1327.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];

const char* programNames[10] = {
  "RGB Test", "Smile Static", "Gray Fill", "Smile Blink", "Smile 1", "Smile 2",
  "Smile Anim Green", "Smile Anim White", "Smile Anim Red", "Smile Anim Green"
};

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);

// Helper function to get program name by index
const char* getProgramName(int programIndex) {
  if (programIndex >= 0 && programIndex < 10) {
    return programNames[programIndex];
  }
  return "Unknown";
}

void Programs_menu() {
  if (Button_input_Pressed[0]) { // Enter - select program
    currentProgram = menu_cursor; // Set current program to selected item
    Serial.print("Program selected: ");
    Serial.println(getProgramName(currentProgram)); // Debug output for selected program
    selected_menu = 0; // Return to home menu
    menu_cursor = 0;
  }
  
  // Display menu
  drawMenuWithCursor(programNames, 10, menu_cursor, "Programs");
}
