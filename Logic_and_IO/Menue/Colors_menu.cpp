#include "Colors_menu.h"
#include "../hardware.h"
#include <Adafruit_SSD1327.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];

const char* colorNames[16] = {
  "Black", "White", "Red", "Green", "Blue", "Yellow",
  "Cyan", "Magenta", "Orange", "Purple", "Pink", "Lime",
  "Teal", "Navy", "Maroon", "Gray"
};

const uint8_t colorValues[16][3] = {
  {0, 0, 0},       // Black
  {255, 255, 255}, // White
  {255, 0, 0},     // Red
  {0, 255, 0},     // Green
  {0, 0, 255},     // Blue
  {255, 255, 0},   // Yellow
  {0, 255, 255},   // Cyan
  {255, 0, 255},   // Magenta
  {255, 165, 0},   // Orange
  {128, 0, 128},   // Purple
  {255, 192, 203}, // Pink
  {50, 205, 50},   // Lime
  {0, 128, 128},   // Teal
  {0, 0, 128},     // Navy
  {128, 0, 0},     // Maroon
  {128, 128, 128}  // Gray
};

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);

void getColorRGB(int colorIndex, uint8_t* r, uint8_t* g, uint8_t* b) {
  if (colorIndex >= 0 && colorIndex < 16) {
    *r = colorValues[colorIndex][0];
    *g = colorValues[colorIndex][1];
    *b = colorValues[colorIndex][2];
  } else {
    *r = *g = *b = 0; // Default to black
  }
}

void Colors_menu() {
  if (Button_input_Pressed[0]) { // Enter - select color
    getColorRGB(menu_cursor, &selected_colour_value[0], &selected_colour_value[1], &selected_colour_value[2]);
    selected_menu = 0; // Return to home menu
    menu_cursor = 1; // Return to color menu position
  }
  
  // Display menu
  drawMenuWithCursor(colorNames, 16, menu_cursor, "Colors");
}