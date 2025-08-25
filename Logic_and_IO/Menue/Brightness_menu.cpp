#include "Brightness_menu.h"
#include "../hardware.h"
#include <Adafruit_SSD1327.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);

void Brightness_menu() {
  if (Button_input_Pressed[0]) { // Enter - set brightness
    uint8_t brightnessValue = 0;
    switch (menu_cursor) {
      case 0: brightnessValue = 64;  break;  // 25% of 255
      case 1: brightnessValue = 128; break;  // 50% of 255
      case 2: brightnessValue = 192; break;  // 75% of 255
      case 3: brightnessValue = 255; break;  // 100% of 255
    }
  
    selected_menu = 0; // Return to home menu
    menu_cursor = 2; // Return to brightness menu position
  }
  
  // Display menu
  const char* brightnessMenu[] = {"25%", "50%", "75%", "100%"};
  drawMenuWithCursor(brightnessMenu, 4, menu_cursor, "Brightness");
}