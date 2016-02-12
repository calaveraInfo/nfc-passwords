#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SS 4
#define FIRST_BUTTON_PIN 2
#define MAX_RESPONSE_LENGTH 255

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
    if (selectPivApp())
      if (enterPin())
        decrypt();
    waitUntilButtonPushed();
  } else {
    if (lineLength > 128) {
      Serial.println(".");
      lineLength = 0;
    } else {
      Serial.print(".");
    }
    lineLength++;
  }
}

boolean selectPivApp() {
  uint8_t message[] = {0x00, 0xA4, 0x04, 0x00, 0x09, 0xA0, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x10, 0x00, 0x00};
  uint8_t responseLength = MAX_RESPONSE_LENGTH;
  return sendMessage(message, sizeof(message), &responseLength, 0x90, 0x00);
}

boolean enterPin() {
  uint8_t message[] = {0x00, 0x20, 0x00, 0x80, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0xFF, 0xFF};
  uint8_t responseLength = MAX_RESPONSE_LENGTH;
  sendMessage(message, sizeof(message), &responseLength, 0x90, 0x00);
}

#define MAX_MESSAGE_LENGTH 62
#define ENCRYPTED_MESSAGE_LENGTH 267
#define COMMAND_LENGTH 5

boolean decrypt() {
  uint8_t encrypted[] = {
    0x7C, 0x82, 0x01, 0x06, 0x82, 0x00, 0x81, 0x82, 0x01, 0x00,
    0x3a, 0xec, 0xd7, 0xd9, 0x72, 0x19, 0x42, 0x3b, 0x9e, 0x0d, 0x70, 0x99, 0xf3, 0xf4, 0x81, 0x7b,
    0x87, 0xad, 0x55, 0xe5, 0xd8, 0x37, 0x5a, 0x15, 0x25, 0x47, 0xf6, 0xb7, 0x1b, 0x04, 0x01, 0xe1,
    0x25, 0x3f, 0x97, 0xe0, 0x59, 0xf7, 0x29, 0xec, 0x91, 0x66, 0xac, 0x1a, 0x06, 0x40, 0xdf, 0x18,
    0x20, 0xa8, 0xe0, 0xdf, 0xf2, 0x5d, 0xfc, 0x60, 0x9d, 0x5a, 0x2d, 0xe8, 0x5c, 0x56, 0xe5, 0xa3,
    0x14, 0x75, 0x9b, 0xe1, 0x16, 0xdd, 0x3d, 0x14, 0x08, 0xb8, 0x70, 0xf4, 0x72, 0xb5, 0x2f, 0x8d,
    0x11, 0xad, 0x69, 0x00, 0x1b, 0xf0, 0xb7, 0xec, 0x2c, 0x54, 0x31, 0x97, 0xd4, 0x41, 0x53, 0x25,
    0x6d, 0x69, 0x5a, 0x6c, 0xe7, 0xd4, 0xa7, 0x3f, 0x7d, 0x21, 0x93, 0xaa, 0x77, 0x56, 0xbc, 0x81,
    0x59, 0xac, 0x48, 0x5e, 0x90, 0x89, 0x1a, 0x4c, 0xd3, 0xd1, 0x5a, 0x86, 0x81, 0x4d, 0xc6, 0x52,
    0x4c, 0x13, 0x1b, 0xdd, 0x63, 0x63, 0x86, 0xc4, 0xa9, 0x87, 0x6e, 0x2e, 0xdd, 0x9f, 0xac, 0x7c,
    0xb1, 0xdf, 0x40, 0xf8, 0xe6, 0x9d, 0x4b, 0xfe, 0x0a, 0x4f, 0x65, 0xbf, 0x61, 0xe3, 0x20, 0x61,
    0xa6, 0xee, 0x28, 0xc4, 0x78, 0x85, 0x63, 0x3e, 0xf2, 0x45, 0xd5, 0xc0, 0x0a, 0x1f, 0x01, 0xed,
    0xd2, 0xcc, 0xc3, 0xb8, 0x47, 0xc3, 0xc6, 0x8e, 0xea, 0x53, 0xdd, 0x29, 0x26, 0x2a, 0xcf, 0x0f,
    0x95, 0x6c, 0x0b, 0xbb, 0x98, 0x37, 0x4f, 0xf0, 0x73, 0xc0, 0xcf, 0x38, 0x9c, 0x98, 0x5e, 0x30,
    0xd9, 0x0f, 0x33, 0x35, 0x63, 0x7a, 0x46, 0xa0, 0xfc, 0xee, 0x31, 0xbc, 0xfd, 0x1b, 0xc0, 0xc1,
    0xd6, 0x60, 0xd4, 0xc0, 0xb2, 0xa7, 0xcf, 0x98, 0x6c, 0x5b, 0x25, 0xfe, 0x76, 0xd9, 0xd8, 0xe9,
    0x6a, 0x88, 0x25, 0xaa, 0x71, 0x8c, 0xe0, 0x26, 0x92, 0x03, 0xbd, 0xa0, 0x97, 0x18, 0x94, 0xfb,
    0x00
  };
  uint8_t responseLength = MAX_RESPONSE_LENGTH;
  uint8_t plen = MAX_MESSAGE_LENGTH - COMMAND_LENGTH;
  uint8_t numberOfMessages = ENCRYPTED_MESSAGE_LENGTH/plen;
  uint8_t remainderCount = ENCRYPTED_MESSAGE_LENGTH%plen;
  uint8_t chainMessage[] = {
    0x10, 0x87, 0x07, 0x9A, plen, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  //Serial.println(plen);
  //Serial.println(numberOfMessages);
  //Serial.println(remainderCount);
  
  for (uint8_t i = 0; i < numberOfMessages; i++) {
    copyBytes(encrypted, chainMessage, i*plen, COMMAND_LENGTH, plen);
    if (!sendMessage(chainMessage, MAX_MESSAGE_LENGTH, &responseLength, 0x90, 0x00)) {
      return false;
    }
    //nfc.PrintHex(chainMessage, MAX_MESSAGE_LENGTH);
  }
  chainMessage[0] = 0x00;
  chainMessage[4] = remainderCount;
  copyBytes(encrypted, chainMessage, ENCRYPTED_MESSAGE_LENGTH-remainderCount, COMMAND_LENGTH, remainderCount);
  // chainMessage[COMMAND_LENGTH+remainderCount] = 50;
  // return sendMessage(chainMessage, COMMAND_LENGTH+remainderCount+1, &responseLength, 0x61, 0x0B);
  return sendMessage(chainMessage, COMMAND_LENGTH+remainderCount, &responseLength, 0x61, 0x0B);
  //nfc.PrintHex(chainMessage, COMMAND_LENGTH+remainderCount);
}

void copyBytes(uint8_t from[], uint8_t to[], uint8_t startFrom, uint8_t startTo, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    to[startTo+i] = from[startFrom+i];
  }
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
