#include <Arduino.h>
#include "../hardware.h"

void handleSerialInput() {
  static char buf[16];
  static uint8_t idx = 0;

  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\r') continue;

    if (c == '\n') {
      // Terminate and reset index
      buf[idx] = '\0';
      idx = 0;

      // Trim leading/trailing spaces
      char *s = buf;
      while (*s == ' ') s++;
      char *e = s + strlen(s);
      while (e > s && *(e - 1) == ' ') *(--e) = '\0';

      if (*s == '\0') return; // empty line

      // Brightness command: e.g., "50%"
      size_t len = strlen(s);
      if (len >= 2 && s[len - 1] == '%') {
        s[len - 1] = '\0';
        int percent = atoi(s);
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;
        int brightness = map(percent, 0, 100, 0, 255);
        dma_display->setBrightness8(brightness);
        Serial.print("Brightness set to ");
        Serial.print(percent);
        Serial.println("%");
        return;
      }

      // Program change: 0..5
      int newProgram = atoi(s);
      if (newProgram >= 0 && newProgram <= 6) {
        currentProgram = newProgram;
        Serial.print("Switched to Program ");
        Serial.println(currentProgram);
      } else {
        Serial.println("Invalid input. Use 0–6 for program or e.g. 50% for brightness.");
      }
      return;
    }

    // Accumulate until newline or buffer full
    if (idx < sizeof(buf) - 1) buf[idx++] = c;
  }
}