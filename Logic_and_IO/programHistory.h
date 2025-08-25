#pragma once
#include <Arduino.h>

// Function declarations
void updateProgramHistory();
int getPreviousProgram(int index);
bool wasRecentlyUsed(int program, int withinLast);
void displayProgramHistory(); // Add this new function