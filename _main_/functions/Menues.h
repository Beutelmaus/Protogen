#pragma once
#include <Arduino.h>

// Number of programs and colors
#define NUM_PROGRAMS 10
#define NUM_COLORS 16

// External declarations for the arrays
extern const char* programNames[NUM_PROGRAMS];
extern const char* colorNames[NUM_COLORS];
extern const uint8_t colorValues[NUM_COLORS][3];

// Helper functions
const char* getProgramName(int programIndex);
const char* getColorName(int colorIndex);
void getColorRGB(int colorIndex, uint8_t* r, uint8_t* g, uint8_t* b);