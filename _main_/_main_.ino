#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "hardware.h"

#include "functions/handleSerialInput.h"
#include "functions/rgbTEST.h"
#include "functions/Smile_1.h"
#include "functions/draw_image_on_panel.h"
#include "immages_rgb/immages.cpp" // <-- add this line

#define PANE_WIDTH  (64 * 1)
#define PANE_HEIGHT 32

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println(F("*****************************************************"));
  Serial.println(F("*   ESP32-HUB75-MatrixPanel-I2S-DMA Program Switch  *"));
  Serial.println(F("*****************************************************"));

 //pinMode(22, INPUT_PULLDOWN);

  initDisplay();

  Serial.println("Booting...");
  delay(1000);

  // Start with program 0 (RGB test)
  currentProgram = 0;
  Serial.println("Starting with Program 0 (RGB Test)");
}

// Placeholder programs (simple solid fills for now)
void program1() { dma_display->fillScreenRGB888(255, 0, 0);   Serial.println("Running Program 1..."); delay(250); }
void program2() { dma_display->fillScreenRGB888(0, 255, 0);   Serial.println("Running Program 2..."); delay(250); }
void program3() {Serial.println("Running Program 3...");
                  draw_image_on_panel(Smile_2_1, PANE_WIDTH, PANE_HEIGHT);delay(200);
                  draw_image_on_panel(Smile_2_2, PANE_WIDTH, PANE_HEIGHT);delay(200); 
                  }

void loop() {
  handleSerialInput(); // program switching + brightness



  switch (currentProgram) {
    case 0: rgbTest(); break;
    case 1: program1(); break;
    case 2: program2(); break;
    case 3: program3(); break;
    case 4: draw_image_on_panel(Smile_1, PANE_WIDTH, PANE_HEIGHT); break;
    case 5: draw_image_on_panel(Smile_2, PANE_WIDTH, PANE_HEIGHT); break;
    case 6: drawSmile1(255, 0, 255); break;//Pass the colour
  }
}



// Include .cpp for the arduino compiler
#include "functions/handleSerialInput.cpp"
#include "functions/rgbTEST.cpp"
#include "functions/Smile_1.cpp"
#include "functions/draw_image_on_panel.cpp"