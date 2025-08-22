#include <Arduino.h>
#include "hardware.h"

#include <Adafruit_SSD1327.h>
extern Adafruit_SSD1327 display;

// Move all arrays to global scope
const char* programNames[10] = {
  "RGB Test","Smile Static","Gray Fill","Smile Blink","Smile 1","Smile 2",
  "Smile Anim Green","Smile Anim White","Smile Anim Red","Smile Anim Green"};

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

const char* Home_Screen[6] = {"Programs", "Colour","Brightness","Webpage","Debug","Standby"};

// Helper functions - MOVE THESE BEFORE MenueAndButtons()
const char* getProgramName(int programIndex) {
  if (programIndex >= 0 && programIndex < 10) {
    return programNames[programIndex];
  }
  return "Unknown";
}

const char* getColorName(int colorIndex) {
  if (colorIndex >= 0 && colorIndex < 16) {
    return colorNames[colorIndex];
  }  
  return "Unknown";
}

void getColorRGB(int colorIndex, uint8_t* r, uint8_t* g, uint8_t* b) {
  if (colorIndex >= 0 && colorIndex < 16) {
    *r = colorValues[colorIndex][0];
    *g = colorValues[colorIndex][1];
    *b = colorValues[colorIndex][2];
  } else {
    *r = *g = *b = 0; // Default to black
  }
}

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title) {
  const int MAX_VISIBLE_ITEMS = 9; // Reserve space for title and status

  display.setCursor(0, 0);
  display.println(title);
  
  // Calculate scroll offset
  int scrollOffset = 0;
  if (menuSize > MAX_VISIBLE_ITEMS) {
    if (cursor >= MAX_VISIBLE_ITEMS - 1) {
      scrollOffset = cursor - MAX_VISIBLE_ITEMS + 2;
    }
    if (scrollOffset > menuSize - MAX_VISIBLE_ITEMS) {
      scrollOffset = menuSize - MAX_VISIBLE_ITEMS;
    }
  }
  
  // Draw visible menu items
  int visibleItems = min(menuSize, MAX_VISIBLE_ITEMS);
  for (int i = 0; i < visibleItems; i++) {
    int itemIndex = i + scrollOffset;
    if (itemIndex >= menuSize) break;
    
    display.setCursor(0, 16 + (i * 10)); // Start at y=16, 10 pixels between lines
    
    // Draw cursor for selected item
    if (itemIndex == cursor) {
      display.print("-> ");
    } else {
      display.print("   "); // 3 spaces for alignment
    }
    
    display.println(menuItems[itemIndex]);
  }
  
  // Show scroll indicators if needed
  if (menuSize > MAX_VISIBLE_ITEMS) {
    if (scrollOffset > 0) {
      display.setCursor(115, 20);
      display.print("^"); /*Up arrow*/
    }
    if (scrollOffset + MAX_VISIBLE_ITEMS < menuSize) {
      display.setCursor(115, 80);
      display.print("v"); /* Down arrow */
    }

    // Show position indicator
    display.setCursor(0, 106);
    display.print(cursor + 1);
    display.print("/");
    display.print(menuSize);
  }
}

void MenueAndButtons() {

  static bool Button_input[4] = {};
  static bool Button_input_last_Cycle[4] = {};
  static bool Button_input_Pressed[4] = {};
  static int menu_cursor = 0;
  static int selected_menu = 0;

  for (int i = 0; i < 4; i++) {//Detect rising edge
    Button_input_last_Cycle[i] = Button_input[i];
    Button_input[i] = IO_Module_1.read(i);
    Button_input_Pressed[i] = Button_input[i] && !Button_input_last_Cycle[i];
  }

  // Get max items for current menu
  int maxItems = 0;
  switch (selected_menu) {
    case 0: maxItems = 5; break;  // Home Screen (0-5)
    case 1: maxItems = 9; break;  // Programs (0-9)
    case 2: maxItems = 15; break; // Colors (0-15)
    case 3: case 4: case 5: case 6: maxItems = 3; break; // Other menus (0-3)
  }

  // Universal cursor navigation (buttons 2 and 3)
  if (Button_input_Pressed[2]) {
    menu_cursor--;
    if (menu_cursor < 0) menu_cursor = maxItems;
  }
  if (Button_input_Pressed[3]) {
    menu_cursor++;
    if (menu_cursor > maxItems) menu_cursor = 0;
  }



  ///////////////////////////////////////////////////////////////////////////////////
  // Print status every 500ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    Serial.print("Selected Menu: ");
    Serial.print(selected_menu);
    Serial.print("    Menu Cursor: ");
    Serial.print(menu_cursor);
    Serial.print("    Button States: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(Button_input[i]);
    }
    Serial.println("");
  }

  ///////////////////////////////////////////////////////////////////////////////////
  display.clearDisplay();
  display.setTextSize(1);
  //Render Menues
  switch (selected_menu) {
    case 0:{drawMenuWithCursor(Home_Screen, 6, menu_cursor, "Protogen - Main Menu");}break;// Home Screen Menu
    case 1:{drawMenuWithCursor(programNames, 10, menu_cursor, "Programs");}break;// Programs menu
    case 2:{drawMenuWithCursor(colorNames, 16, menu_cursor, "Colors");}break;// Colour menu
    case 3:{ const char* brightnessMenu[] = {"25%", "50%", "75%", "100%"};       
        drawMenuWithCursor(brightnessMenu, 4, menu_cursor, "Brightness");}break; // Brightness menu
    case 4:{ const char* webpageMenu[] = {"Start Server", "Stop Server", "Show IP"};
        drawMenuWithCursor(webpageMenu, 3, menu_cursor, "Webpage");}break; // Webpage menu
    case 5:{ const char* debugMenu[] = {"Serial Output", "I2C Scan", "Memory Info"};
        drawMenuWithCursor(debugMenu, 3, menu_cursor, "Debug");}break; // Debug menu
    case 6:{const char* standbyMenu[] = {"Enter Standby", "Auto Standby: ON", "Cancel"};
        drawMenuWithCursor(standbyMenu, 3, menu_cursor, "Standby");}break; // Standby menu
  }
  
    // Case selects the logic depending on the selected menu
    // List al functions that are executed when enter is pressed
  switch (selected_menu) {
    case 0: // Home Screen Menu
      if (Button_input_Pressed[0]) { // Enter - go to selected menu item
        selected_menu = menu_cursor + 1; // Go to submenu (1-6)
        menu_cursor = 0;} // Reset cursor for new menu
        break;

    case 1: // Programs/Animations menu
      if (Button_input_Pressed[0]) { // Enter - select program
        currentProgram = menu_cursor; // Set current program to selected item
        selected_menu = 0; // Return to home menu
        menu_cursor = 0;}
      break;

    case 2: // Colour menu
      if (Button_input_Pressed[0]) { // Enter - select color
        // Set selected color based on menu_cursor
        getColorRGB(menu_cursor, &selected_colour_value[0], &selected_colour_value[1], &selected_colour_value[2]);
        selected_menu = 0;menu_cursor = 1;} // Return to home menu
         break;
      
    case 3:// Brightness menu
     if (Button_input_Pressed[0]) { // Enter - set brightness
      uint8_t brightnessValue = 0;
      switch (menu_cursor) {
        case 0: brightnessValue = 64;  break;  // 25% of 255
        case 1: brightnessValue = 128; break;  // 50% of 255
        case 2: brightnessValue = 192; break;  // 75% of 255
        case 3: brightnessValue = 255; break;} // 100% of 255

      dma_display->setBrightness8(brightnessValue);
      Serial.print("Brightness set to: ");
      Serial.print((brightnessValue * 100) / 255);
      Serial.println("%");
      selected_menu = 0;menu_cursor = 2;}// Return to home menu
      break;

    case 4: // Webpage menu
    case 5: // Debug menu
    case 6: // Standby menu
      if (Button_input_Pressed[0]) { // Enter - execute selected action
        // Add specific action logic for each menu here
      }
      break;
  }

if (Button_input_Pressed[1]) {selected_menu = 0;menu_cursor = 0;} // Back - return to home screen


  display.display();
}