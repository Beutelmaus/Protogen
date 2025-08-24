#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <PCF8574.h>//https://github.com/RobTillaart/PCF8574
#include <Wire.h>

#include "hardware.h"
#include "MenueAndButtons.h"

#include "functions/rgbTEST.h"
#include "functions/draw_image_on_panel.h"
#include "functions/draw_animation.h"
#include "functions/programHistory.h"
#include "immages_rgb/immages.cpp"

String selected_colour_Name = "White";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("*****************************************************"));
  Serial.println(F("*              Protogen booting                     *"));
  Serial.println(F("*****************************************************"));

  Wire.begin();
  IO_Module_1.begin();
  scanI2C();
  initDisplays();

  currentProgram = 1;
}

void program3() {draw_image_on_panel(Smile_2_1);delay(200);
                draw_image_on_panel(Smile_2_2);delay(200);}

void loop() {
   MenueAndButtons(); // program switching + brightness
   updateProgramHistory();// Keep a history of selected programs

  // Add a small delay to prevent tight looping when webserver is active
  static unsigned long lastLoopTime = 0;
  extern bool serverRunning;
  
  if (serverRunning) {
    // When webserver is running, add slight delay to reduce CPU load
    unsigned long currentTime = millis();
    if (currentTime - lastLoopTime < 2) { // Minimum 2ms between main loops
      delay(1);
    }
    lastLoopTime = currentTime;
  }

  switch (currentProgram) {
    case  -1: currentProgram = 9; break;
    case 0: rgbTest(); break;
    case 1: draw_image_on_panel(Smile_static); break;// idle state
    case 2: draw_image_on_panel(Smile_static); break;
    case 3: program3(); break;
    case 4: draw_image_on_panel(Smile_1); break;
    case 5: draw_image_on_panel(Smile_2); break;
    case 6: draw_animation("Smile_blink", 100); break;
    case 7: draw_animation("Smile_blink", 150); break;
    case 8: draw_animation("Smile_blink", 200); break;
    case 9: draw_animation("Smile_blink", 100); break;
    case 10: currentProgram = 1; break;
  }
}

// Include .cpp files for the arduino compiler - IMPORTANT: Include .cpp files, not .h files
#include "functions/rgbTEST.cpp"
#include "functions/draw_image_on_panel.cpp"
#include "functions/draw_animation.cpp"
#include "functions/programHistory.cpp"
#include "Menue/Home_menu.cpp"
#include "Menue/Programs_menu.cpp"
#include "Menue/Colors_menu.cpp"
#include "Menue/Brightness_menu.cpp"
#include "Menue/Webpage_menu.cpp"
#include "Menue/Debug_menu.cpp"
#include "Menue/Standby_menu.cpp"
