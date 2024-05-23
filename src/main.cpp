#include "Arduino.h"

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"

#include "SPI.h"
#include "SD.h"

#include "constants.h"

MCUFRIEND_kbv tft;

// colors of the squares
uint16_t c1;
uint16_t c2;
uint16_t c3;

void no_config() {

  // print the error on the Serial Monitor
  Serial.println("Config not found");

  // set the text color, size and location and print the error on the display
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Config not found");

  while (1);
}

void invalid_config() {

  // print the error on the Serial Monitor
  Serial.println("Config in Improper Format");

  // set the text color, size and location and print the error on the display
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Config in Improper Format");

  while (1);
}

void parse_config(File *cfg) {

  // store the key and value on the current line
  char key[32];
  char value[32];

  // add null-termination
  key[31] = 0;
  value[31] = 0;

  for (;;) {

    // skip till the start of the key
    for (;;) {

      int c = cfg->read();

      if (c == -1) {
        // there is no property, so return;
        return;
      }
      else if (c == '"') {
        break;
      }
    }

    // read the key name
    for (int i = 0; i < 31; ++i) {

      int c = cfg->read();

      if (c == -1) {
        invalid_config();
      }
      else if (c == '"') {
        key[i] = 0;
        break;
      }

      key[i] = (char)c;
    }

    // skip till the equals sign
    for (;;) {

      int c = cfg->read();

      if (c == -1) {
        invalid_config();
      }
      else if (c == '=') {
        break;
      }
    }

    // skip till the start of the value
    for (;;) {

      int c = cfg->read();

      if (c == -1) {
        invalid_config();
      }
      else if (c == '"') {
        break;
      }
    }

    // read the value name
    for (int i = 0; i < 31; ++i) {

      int c = cfg->read();

      if (c == -1) {
        invalid_config();
      }
      else if (c == '"') {
        value[i] = 0;
        break;
      }

      value[i] = (char)c;
    }

    // print the key and value on the Serial Monitor for debugging
    Serial.print(key);
    Serial.print(" : ");
    Serial.println(value);

    if (strncmp(key, "square-1-color", 32) == 0) {
	    c1 = 0;
      for (int i = 0; value[i] != 0; ++i) {
        if ('0' <= value[i] && value[i] <= '9') {
          c1 = (c1 * 16) + value[i] - '0';
        }
        else {
          value[i] |= 32;
          if ('a' <= value[i] && value[i] <= 'f') {
            c1 = (c1 * 16) + value[i] - 'a' + 10;
          }
        }
      }
    }
    else if (strncmp(key, "square-2-color", 32) == 0) {
	    c2 = 0;
      for (int i = 0; value[i] != 0; ++i) {
        if ('0' <= value[i] && value[i] <= '9') {
          c2 = (c2 * 16) + value[i] - '0';
        }
        else {
          value[i] |= 32;
          if ('a' <= value[i] && value[i] <= 'f') {
            c2 = (c2 * 16) + value[i] - 'a' + 10;
          }
        }
      }
    }
    else if (strncmp(key, "square-3-color", 32) == 0) {
	    c3 = 0;
      for (int i = 0; value[i] != 0; ++i) {
        if ('0' <= value[i] && value[i] <= '9') {
          c3 = (c3 * 16) + value[i] - '0';
        }
        else {
          value[i] |= 32;
          if ('a' <= value[i] && value[i] <= 'f') {
            c3 = (c3 * 16) + value[i] - 'a' + 10;
          }
        }
      }
    }
  }
}

void setup() {

  Serial.begin(9600);

  tft.begin(0x9486);
  tft.fillScreen(BLACK);

  if (!SD.begin(10)) {
    no_config();
  }

  File cfg = SD.open("CONFIG.TXT", FILE_READ);
  if (!cfg) {
    no_config();
  }
  parse_config(&cfg);
  cfg.close();

  tft.fillRect(100, 20, 120, 120, c1);
  tft.fillRect(100, 180, 120, 120, c2);
  tft.fillRect(100, 340, 120, 120, c3);
}

void loop() {
}
