#include "Debug_menu.h"
#include "../hardware.h"
#include "../functions/programHistory.h"
#include <Adafruit_SSD1327.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];
extern bool showingHistory;

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);
void scanI2C();

void Debug_menu() {
  if (Button_input_Pressed[0]) { // Enter - execute selected action
    switch (menu_cursor) {
      case 0: // Serial Output
        Serial.println("=== Serial Output Debug ===");
        break;
      case 1: // I2C Scan
        Serial.println("=== I2C Scan ===");
        scanI2C();
        break;
      case 2: // Memory Info
        Serial.println("=== Memory Info ===");
        Serial.print("Free heap: ");
        Serial.println(ESP.getFreeHeap());
        break;
      case 3: // Program History
        showingHistory = true; // Set flag to show history
        break;
    }
    if (!showingHistory) { // Only return to menu if not showing history
      selected_menu = 0; // Return to home menu
      menu_cursor = 4; // Return to Debug menu position
    }
  }
  
  // Display menu
  const char* debugMenu[] = {"Serial Output", "I2C Scan", "Memory Info", "Program History"};
  drawMenuWithCursor(debugMenu, 4, menu_cursor, "Debug");
}