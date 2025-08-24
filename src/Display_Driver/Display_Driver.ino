#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>//https://github.com/tidbyt/ESP32-HUB75-MatrixPanel-I2S-DMA
#include <Wire.h>

#include "hardware.h"
#include "functions/rgbTEST.h"
#include "functions/draw_image_on_panel.h"
#include "functions/draw_animation.h"
#include "immages_rgb/immages.cpp"

// I2C Configuration
#define DISPLAY_DRIVER_I2C_ADDRESS 0x20
#define I2C_SDA 21
#define I2C_SCL 22

// Remove these definitions (now in hardware.cpp)
// volatile int currentProgram = 1;
// uint8_t selected_colour_value[3] = {255, 255, 255};
volatile bool programChanged = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("*****************************************************"));
  Serial.println(F("*            Display_Driver Controller              *"));
  Serial.println(F("*****************************************************"));

  // Initialize I2C as slave
  Wire.begin(DISPLAY_DRIVER_I2C_ADDRESS, I2C_SDA, I2C_SCL);
  Wire.onReceive(receiveI2CData);
  
  // Initialize HUB75 LED matrix display
  initDisplays();
  
  Serial.print("Display driver ready on I2C address 0x");
  Serial.println(DISPLAY_DRIVER_I2C_ADDRESS, HEX);
  Serial.println("Waiting for commands from Logic_and_IO...");
}

void program3() {
  draw_image_on_panel(Smile_2_1);
  delay(200);
  draw_image_on_panel(Smile_2_2);
  delay(200);
}

void loop() {
  // Handle program display based on received I2C commands

  currentProgram = 0;//TEST
  switch (currentProgram) {
    case -1: currentProgram = 9; break;
    case 0: rgbTest(); break;
    case 1: draw_image_on_panel(Smile_static); break; // idle state
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
  
  // Check for program changes
  if (programChanged) {
    programChanged = false;
    Serial.print("Program updated to: ");
    Serial.print(currentProgram);
    Serial.print(" RGB(");
    Serial.print(selected_colour_value[0]);
    Serial.print(",");
    Serial.print(selected_colour_value[1]);
    Serial.print(",");
    Serial.print(selected_colour_value[2]);
    Serial.println(")");
  }
  
  delay(10); // Small delay for stability
}

// I2C receive interrupt handler
void receiveI2CData(int numBytes) {
  if (numBytes >= 5) { // Command + program + RGB values
    uint8_t command = Wire.read();
    
    if (command == 0x01) { // Program change command
      currentProgram = Wire.read();
      selected_colour_value[0] = Wire.read(); // Red
      selected_colour_value[1] = Wire.read(); // Green
      selected_colour_value[2] = Wire.read(); // Blue
      programChanged = true;
    }
  }
  
  // Clear any remaining bytes
  while (Wire.available()) {
    Wire.read();
  }
}

// Include only HUB75 display-related functions
#include "functions/rgbTEST.cpp"
#include "functions/draw_image_on_panel.cpp" 
#include "functions/draw_animation.cpp"
