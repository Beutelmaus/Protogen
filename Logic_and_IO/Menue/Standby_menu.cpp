#include "Standby_menu.h"
#include "../hardware.h"
#include <Adafruit_SSD1327.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);

void Standby_menu() {
  if (Button_input_Pressed[0]) { // Enter - execute selected action
    // Add specific action logic here
    selected_menu = 0; // Return to home menu
    menu_cursor = 5; // Return to Standby menu position
  }
  
  const char* standbyMenu[] = {"Enter Standby", "Auto Standby: ON", "Cancel"};
  drawMenuWithCursor(standbyMenu, 3, menu_cursor, "Standby");
}