// This script erases the entire memory of an ESP32.
// Compatible with the Arduino IDE.

#include <SPIFFS.h>
#include <EEPROM.h>

void eraseEEPROM() {
  // Initialize EEPROM with arbitrary size
  if (!EEPROM.begin(512)) {
    Serial.println("Failed to initialize EEPROM.");
    return;
  }

  // Write 0xFF to every byte in EEPROM
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);
  }

  // Commit the changes to EEPROM
  EEPROM.commit();
  Serial.println("EEPROM erased.");
}

void eraseFlash() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to initialize SPIFFS.");
    return;
  }

  // Format the SPIFFS file system (essentially erases it)
  if (SPIFFS.format()) {
    Serial.println("SPIFFS flash erased.");
  } else {
    Serial.println("Failed to erase SPIFFS flash.");
  }
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting memory erase...");

  eraseEEPROM();
  eraseFlash();

  Serial.println("Memory erase complete.");
}

void loop() {
  // Do nothing in loop
}
