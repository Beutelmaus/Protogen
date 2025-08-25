#include "Home_menu.h"
#include "../hardware.h"
#include <Adafruit_SSD1327.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];

const char* Home_Screen[6] = {"Programs", "Colour", "Brightness", "Webpage", "Debug", "Standby"};

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);

void Home_menu() {
  if (Button_input_Pressed[0]) { // Enter - go to selected menu item
    selected_menu = menu_cursor + 1; // Go to submenu (1-6)
    menu_cursor = 0; // Reset cursor for new menu
  }
  
  // Display menu
  drawMenuWithCursor(Home_Screen, 6, menu_cursor, "Protogen - Main Menu");
}