#include "Menues.h"

// Array of program names (matches the switch cases in _main_.ino)
const char* programNames[NUM_PROGRAMS] = {
  "RGB Test",           // case 0
  "Smile Static",       // case 1  
  "Gray Fill",          // case 2
  "Smile Blink",        // case 3
  "Smile 1",            // case 4
  "Smile 2",            // case 5
  "Smile Anim Green",   // case 6
  "Smile Anim White",   // case 7
  "Smile Anim Red",     // case 8
  "Smile Anim Green"    // case 9
};

// Array of color names
const char* colorNames[NUM_COLORS] = {
  "Black",
  "White", 
  "Red",
  "Green",
  "Blue",
  "Yellow",
  "Cyan",
  "Magenta",
  "Orange",
  "Purple",
  "Pink",
  "Lime",
  "Teal",
  "Navy",
  "Maroon",
  "Gray"
};

// RGB values for the color names (matches colorNames array)
const uint8_t colorValues[NUM_COLORS][3] = {
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

// Helper functions
const char* getProgramName(int programIndex) {
  if (programIndex >= 0 && programIndex < NUM_PROGRAMS) {
    return programNames[programIndex];
  }
  return "Unknown";
}

const char* getColorName(int colorIndex) {
  if (colorIndex >= 0 && colorIndex < NUM_COLORS) {
    return colorNames[colorIndex];
  }
  return "Unknown";
}

void getColorRGB(int colorIndex, uint8_t* r, uint8_t* g, uint8_t* b) {
  if (colorIndex >= 0 && colorIndex < NUM_COLORS) {
    *r = colorValues[colorIndex][0];
    *g = colorValues[colorIndex][1];
    *b = colorValues[colorIndex][2];
  } else {
    *r = *g = *b = 0; // Default to black
  }
}