#include <Arduino.h>
#include "hardware.h"
#include "programHistory.h"
#include "Menue/Home_menu.h"
#include "Menue/Programs_menu.h"
#include "Menue/Colors_menu.h"
#include "Menue/Brightness_menu.h"
#include "Menue/Webpage_menu.h"
#include "Menue/Debug_menu.h"
#include "Menue/Standby_menu.h"

#include <Adafruit_SSD1327.h>
extern Adafruit_SSD1327 display;

// Global menu variables
int menu_cursor = 0;
int selected_menu = 0;
bool showingHistory = false;
bool Button_input_Pressed[4] = {};

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title) {
  const int MAX_VISIBLE_ITEMS = 9;

  display.setCursor(0, 0);
  display.println(title);
  
  int scrollOffset = 0;
  if (menuSize > MAX_VISIBLE_ITEMS) {
    if (cursor >= MAX_VISIBLE_ITEMS - 1) {
      scrollOffset = cursor - MAX_VISIBLE_ITEMS + 2;
    }
    if (scrollOffset > menuSize - MAX_VISIBLE_ITEMS) {
      scrollOffset = menuSize - MAX_VISIBLE_ITEMS;
    }
  }
  
  int visibleItems = min(menuSize, MAX_VISIBLE_ITEMS);
  for (int i = 0; i < visibleItems; i++) {
    int itemIndex = i + scrollOffset;
    if (itemIndex >= menuSize) break;
    
    display.setCursor(0, 16 + (i * 10));
    
    if (itemIndex == cursor) {
      display.print("-> ");
    } else {
      display.print("   ");
    }
    
    display.println(menuItems[itemIndex]);
  }
  
  if (menuSize > MAX_VISIBLE_ITEMS) {
    if (scrollOffset > 0) {
      display.setCursor(115, 20);
      display.print("^");
    }
    if (scrollOffset + MAX_VISIBLE_ITEMS < menuSize) {
      display.setCursor(115, 80);
      display.print("v");
    }

    display.setCursor(0, 106);
    display.print(cursor + 1);
    display.print("/");
    display.print(menuSize);
  }
}

void displayProgramHistoryOnScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 0);
  display.println("Program History");
  display.println("(Press any button to exit)");
  
  for (int i = 0; i < 5; i++) {
    int program = getPreviousProgram(i);
    
    display.setCursor(0, 24 + (i * 12));
    display.print(i + 1);
    display.print(". ");
    
    if (program != -1) {
      if (program >= 0 && program < 10) {
        // Get program name from Programs_menu.cpp
        display.print(getProgramName(program)); // Use helper function instead
      } else {
        display.print("Unknown");
      }
    } else {
      display.print("(No program)");
    }
  }
  
  display.setCursor(0, 100);
  display.print("Current: ");
  if (currentProgram >= 0 && currentProgram < 10) {
    display.print(getProgramName(currentProgram)); // Use helper function instead
  } else {
    display.print("Unknown");
  }
}

void MenueAndButtons() {
  static bool Button_input[4] = {};
  static bool Button_input_last_Cycle[4] = {};

  for (int i = 0; i < 4; i++) {
    Button_input_last_Cycle[i] = Button_input[i];
    Button_input[i] = IO_Module_1.read(i);
    Button_input_Pressed[i] = Button_input[i] && !Button_input_last_Cycle[i];
  }

  // Check if we're showing history
  if (showingHistory) {
    if (Button_input_Pressed[0] || Button_input_Pressed[1] || 
        Button_input_Pressed[2] || Button_input_Pressed[3]) {
      showingHistory = false;
      selected_menu = 0;
      menu_cursor = 4;
    }
    
    displayProgramHistoryOnScreen();
    display.display();
    return;
  }

  // Get max items for current menu
  int maxItems = 0;
  switch (selected_menu) {
    case 0: maxItems = 5; break;  // Home Screen (0-5)
    case 1: maxItems = 9; break;  // Programs (0-9)
    case 2: maxItems = 15; break; // Colors (0-15)
    case 3: maxItems = 3; break;  // Brightness menu (0-3)
    case 4: maxItems = 2; break;  // Webpage menu (0-2)
    case 5: maxItems = 3; break;  // Debug menu (0-3)
    case 6: maxItems = 2; break;  // Standby menu (0-2)
  }

  // Universal cursor navigation
  if (Button_input_Pressed[2]) {
    menu_cursor--;
    if (menu_cursor < 0) menu_cursor = maxItems;
  }
  if (Button_input_Pressed[3]) {
    menu_cursor++;
    if (menu_cursor > maxItems) menu_cursor = 0;
  }

  // Back button handling
  if (Button_input_Pressed[1]) {
    if (selected_menu != 0) {
      selected_menu = 0;
      menu_cursor = 0;
    }
  }

  // Menu Screen
  display.clearDisplay();
  display.setTextSize(1);
  
  // Single function call per menu
  switch (selected_menu) {
    case 0: Home_menu(); break;
    case 1: Programs_menu(); break;
    case 2: Colors_menu(); break;
    case 3: Brightness_menu(); break;
    case 4: Webpage_menu(); break;
    case 5: Debug_menu(); break;
    case 6: Standby_menu(); break;
  }
  
  display.display();
}