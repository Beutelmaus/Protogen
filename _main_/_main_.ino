#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <PCF8574.h>//https://github.com/RobTillaart/PCF8574
#include <Wire.h>

#include "hardware.h"
#include "MenueAndButtons.h"

#include "functions/rgbTEST.h"
#include "functions/draw_image_on_panel.h"
#include "functions/draw_animation.h"
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

  // Start with program 1
  currentProgram = 1;
  Serial.println("Starting with Program 1");
}

// Placeholder programs (simple solid fills for now)
void program1() { dma_display->fillScreenRGB888(255, 0, 0);}
void program2() { dma_display->fillScreenRGB888(50, 50, 50);}
void program3() {draw_image_on_panel(Smile_2_1);delay(200);
                draw_image_on_panel(Smile_2_2);delay(200);}

void loop() {
   MenueAndButtons(); // program switching + brightness

  switch (currentProgram) {
    case  -1: currentProgram = 9; break;
    case 0: rgbTest(); break;
    case 1: draw_image_on_panel(Smile_static); break;
    case 2: program2(); break;
    case 3: program3(); break;
    case 4: draw_image_on_panel(Smile_1); break;
    case 5: draw_image_on_panel(Smile_2); break;
    case 6:  draw_animation("Smile", 100); break;
    case 7: draw_animation("Smile", 150); break;
    case 8: draw_animation("Smile", 200); break;
    case 9: draw_animation("Smile", 100); break;
    case 10: currentProgram = 1; break;
  }
}

// Include .cpp for the arduino compiler
#include "functions/rgbTEST.cpp"
#include "functions/draw_image_on_panel.cpp"
#include "functions/draw_animation.cpp"
