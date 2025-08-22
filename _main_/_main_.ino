#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <PCF8574.h>//https://github.com/RobTillaart/PCF8574
#include <Wire.h>

#include "hardware.h"
#include "MenueAndButtons.h"
#include "functions/Menues.cpp"

#include "functions/rgbTEST.h"
#include "functions/rgbTEST.cpp"
#include "functions/draw_image_on_panel.h"
#include "functions/draw_image_on_panel.cpp"
#include "functions/draw_animation.h"
#include "functions/draw_animation.cpp"

#include "immages_rgb/immages.cpp"



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

  // Start with program 0 (RGB test)
  currentProgram = 1;
  Serial.println("Starting with Program 0 (RGB Test)");
}

// Placeholder programs (simple solid fills for now)
void program1() { dma_display->fillScreenRGB888(255, 0, 0);}
void program2() { dma_display->fillScreenRGB888(50, 50, 50);}
void program3() {draw_image_on_panel(Smile_2_1);delay(200);
                draw_image_on_panel(Smile_2_2);delay(200);}

void loop() {
   MenueAndButtons(); // program switching + brightness

  switch (currentProgram) {
    case 0: rgbTest(); break;
    case 1: draw_image_on_panel(Smile_static); break;
    case 2: program2(); break;
    case 3: program3(); break;
    case 4: draw_image_on_panel(Smile_1); break;
    case 5: draw_image_on_panel(Smile_2); break;
    case 6:  draw_animation("Smile", 125, 255, 50, 100); break; //
    case 7: draw_animation("Smile", 255, 255, 255, 150); break; // White Smile, 150ms per frame
    case 8: draw_animation("Smile", 255, 0, 0, 200); break; // Red Smile, 200ms per frame
    case 9: draw_animation("Smile", 0, 255, 0, 100); break; // Green Smile, 100ms per frame
  }
}



