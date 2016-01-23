#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SS   (4)
#define FIRST_BUTTON_PIN (2)
#define PN532DEBUG (1)
#define MAX_RESPONSE_LENGTH (255)

Adafruit_PN532 nfc(PN532_SS);

void setup() {
  Serial.begin(115200);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.println("Did not find the shield - locking up");
    while (true) {
    }
  }

  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  nfc.begin();
  Serial.print("Listening...");
}

uint8_t lineLength = 0;
uint8_t responseMessage[MAX_RESPONSE_LENGTH];

void loop(void) {
  if (nfc.inListPassiveTarget()) {
    Serial.println("Something's there...");
    selectPivApp();
    waitUntilButtonPushed();
  } else {
    if (lineLength > 20) {
      Serial.println(".");
      lineLength = 0;
    } else {
      Serial.print(".");
    }
    lineLength++;
  }
}

void selectPivApp() {
  uint8_t message[] = {0x00, 0xA4, 0x04, 0x00, 0x09, 0xA0, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x10, 0x00, 0x00};
  uint8_t responseLength = MAX_RESPONSE_LENGTH;
  if (sendMessage(message, sizeof(message), &responseLength, 0x90, 0x00)) {
    enterPin();
  }
}

void enterPin() {
  uint8_t message[] = {0x00, 0x20, 0x00, 0x80, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0xFF, 0xFF};
  uint8_t responseLength = MAX_RESPONSE_LENGTH;
  sendMessage(message, sizeof(message), &responseLength, 0x90, 0x00);
}

boolean sendMessage(uint8_t message[], uint8_t messageLength, uint8_t *responseLength, uint8_t expectedSw1, uint8_t expectedSw2) {
  Serial.println("Outgoing:");
  nfc.PrintHex(message, messageLength);
  boolean result = nfc.inDataExchange(message, messageLength, responseMessage, responseLength);
  if (result) {
    Serial.println("Incoming:");
    nfc.PrintHex(responseMessage, (*responseLength));
  }
  if (result
      && (*responseLength) >= 2
      && responseMessage[(*responseLength) - 2] == expectedSw1 
      && responseMessage[(*responseLength) - 1] == expectedSw2) {
    delay(10);
    return true;
  } else {
    return false;
  }
}

void waitUntilButtonPushed(void) {
  Serial.println("Push button to continue...");
  while (digitalRead(FIRST_BUTTON_PIN) == LOW) {}
  Serial.print("Listening...");
}
