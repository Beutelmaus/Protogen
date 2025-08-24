#include "programHistory.h"
#include "../hardware.h"

// Array to track last 5 selected programs
int programHistory[5] = {-1, -1, -1, -1, -1}; 
int lastCurrentProgram = -2; // Track previous value (start with different value to trigger first update)

// External arrays from MenueAndButtons.cpp
extern const char* programNames[10];

// Function to update program history when currentProgram changes
void updateProgramHistory() {
  if (currentProgram != lastCurrentProgram) {
    // Shift array to the right (newest at index 0)
    for (int i = 4; i > 0; i--) {
      programHistory[i] = programHistory[i - 1];
    }
    
    programHistory[0] = currentProgram;// Add new value at the beginning
    
    lastCurrentProgram = currentProgram;// Update the last known value
  }
}

// Function to get a specific previous program value
// index 0 = most recent, index 4 = oldest
int getPreviousProgram(int index) {
  if (index >= 0 && index < 5) {
    return programHistory[index]; // Direct index (newest at beginning)
  }
  return -1; // Invalid index
}

// Function to check if a program was recently used
bool wasRecentlyUsed(int program, int withinLast) {
  if (withinLast > 5) withinLast = 5; // Maximum 5 history entries
  
  for (int i = 0; i < withinLast; i++) {
    if (programHistory[i] == program) {
      return true;
    }
  }
  return false;
}

// Function to display program history with names
void displayProgramHistory() {
  Serial.println("Last 5 programs (newest → oldest):");
  
  for (int i = 0; i < 5; i++) {
    Serial.print(i + 1);
    Serial.print(". ");
    
    if (programHistory[i] != -1) {
      // Display program number and name
      Serial.print("Program ");
      Serial.print(programHistory[i]);
      Serial.print(": ");
      
      if (programHistory[i] >= 0 && programHistory[i] < 10) {
        Serial.println(programNames[programHistory[i]]);
      } else {
        Serial.println("Unknown");
      }
    } else {
      Serial.println("(No program)");
    }
  }
  
  Serial.print("Current program: ");
  Serial.print(currentProgram);
  Serial.print(" (");
  if (currentProgram >= 0 && currentProgram < 10) {
    Serial.print(programNames[currentProgram]);
  } else {
    Serial.print("Unknown");
  }
  Serial.println(")");
}
