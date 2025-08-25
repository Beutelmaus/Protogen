#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Wire.h>
#include "hardware.h"

// Include the header files
#include "functions/rgbTEST.h"
#include "functions/draw_image_on_panel.h"
#include "functions/draw_animation.h"

// Include the implementation files to link them properly
#include "functions/rgbTEST.cpp"
#include "functions/draw_image_on_panel.cpp"
#include "functions/draw_animation.cpp"

// Include the image and animation data
#include "immages_rgb/immages.cpp"
#include "Animations/immages.cpp"

// I2C Configuration
#define DISPLAY_DRIVER_I2C_ADDRESS 0x08

volatile bool programChanged = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("*****************************************************"));
  Serial.println(F("*            Display_Driver Controller              *"));
  Serial.println(F("*****************************************************"));

  // Initialize I2C as slave
  Wire.begin(DISPLAY_DRIVER_I2C_ADDRESS);
  Wire.onReceive(receiveI2CData);
  Serial.print("I2C Slave initialized at address 0x");
  Serial.println(DISPLAY_DRIVER_I2C_ADDRESS, HEX);

  // Initialize HUB75 LED matrix display
  initDisplays();
  
  Serial.println("Display driver ready");
  Serial.println("Waiting for commands from Logic_and_IO...");
}

void loop() {
  // Handle program display based on received I2C commands
    switch (currentProgram) {
    case 0: rgbTest(); break;
    case 1: draw_image_on_panel(Smile_static); break;// idle state
    case 2: draw_image_on_panel(Smile_static); break;
    case 3: draw_image_on_panel(Smile_static); break;
    case 4: draw_image_on_panel(Smile_1); break;
    case 5: draw_image_on_panel(Smile_2); break;
    case 6: draw_animation("Smile_blink", 100); break;
    case 7: draw_animation("Smile_blink", 150); break;
    case 8: draw_animation("Smile_blink", 200); break;
    case 9: draw_animation("Smile_blink", 100); break;
    default:
      dma_display->fillScreen(dma_display->color565(255, 0, 0));
      break;
  }
  
  // Check for program changes
  if (programChanged) {
    programChanged = false;
    Serial.print("I2C Command, Program: ");
    Serial.print(currentProgram);
    Serial.print(" RGB(");
    Serial.print(selected_colour_value[0]);
    Serial.print(",");
    Serial.print(selected_colour_value[1]);
    Serial.print(",");
    Serial.print(selected_colour_value[2]);
    Serial.println(")");
  }
}








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
  } else {
    Serial.println("Invalid data length");
    // Read all received bytes for debugging
    while (Wire.available()) {
      int receivedValue = Wire.read();
      Serial.print("Received byte: ");
      Serial.println(receivedValue);
    }
  }

  // Clear any remaining bytes
  while (Wire.available()) {
    int byte = Wire.read();
    Serial.print("Extra byte: ");
    Serial.println(byte);
  }
}
