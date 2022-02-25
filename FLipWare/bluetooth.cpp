/*
	FLipWare - AsTeRICS Foundation
	Copyright (c) Benjamin Aigner
	For more info please visit: http://www.asterics-academy.net

	Module: bluetooth.cpp - using external Bluetooth addon for mouse/keyboard control

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License:
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "bluetooth.h"

#define BT_MINIMUM_SENDINTERVAL 20     // reduce mouse reports in BT mode (in milliseconds) !

typedef enum {NONE, EZKEY, MINIBT01, MINIBT02} addontype_t;

uint8_t bt_available = 0;
addontype_t bt_esp32addon = NONE;
uint8_t activeKeyCodes[6];
uint8_t activeModifierKeys = 0;
uint8_t activeMouseButtons = 0;

long btsendTimestamp = millis();
long upgradeTimestamp = 0;          // eventually come back to AT mode from an unsuccessful BT module upgrade !
uint8_t readstate_f=0;              // needed to track the return value status during addon upgrade mode

/**
   @name mouseBT
   @param x relative movement x axis
   @param y relative movement y axis
   @param scroll relative scroll actions
   @return

   this method sends a mouse command via the Bluetooth module.
   Mouse movements, buttons and scroll wheel.
   The limit for the movement is +127/-127
*/
void mouseBT(int x, int y, uint8_t scroll)
{
  static int oldMouseButtons = 0;
  static int sendCnt = 0;
  static int accuX = 0, accuY = 0;

#ifdef DEBUG_OUTPUT_FULL
  Serial.println("BT mouse actions:");
  Serial.print("buttons: 0x");
  Serial.println(activeMouseButtons, HEX);
  Serial.print("x/y/scroll: ");
  Serial.print(x, DEC);
  Serial.print("/");
  Serial.print(y, DEC);
  Serial.print("/");
  Serial.println(scroll, DEC);
#endif

  accuX += x;
  accuY += y;

  if ((activeMouseButtons != oldMouseButtons) ||
      (btsendTimestamp + BT_MINIMUM_SENDINTERVAL <= millis()))
  {
    btsendTimestamp = millis();

    ///@todo refactor this code here, send more efficient mouse report data.
    //starting RAW HID mouse report
    //according to:
    //https://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/sending-keys-via-serial

    Serial_AUX.write((uint8_t)0xFD);

    //stuffing...
    Serial_AUX.write((uint8_t)0x00);
    Serial_AUX.write((uint8_t)0x03);

    //masked buttons:
    //left: (1<<0)
    //right: (1<<1)
    //middle: (1<<2) (not sure)
    Serial_AUX.write(activeMouseButtons);

    //send x/y relative movement
    Serial_AUX.write((uint8_t)accuX);
    Serial_AUX.write((uint8_t)accuY);

    //maybe the wheel? Not official by Adafruit... -> not working
    Serial_AUX.write((uint8_t)0x00);
    //some additional bytes...
    Serial_AUX.write((uint8_t)0x00);
    Serial_AUX.write((uint8_t)0x00);

    sendCnt = 0;
    accuX = 0; accuY = 0;
    oldMouseButtons = activeMouseButtons;
  }
}

/**
   @name mouseBTPress
   @param mousebutton uint8_t contains all buttons to be pressed (masked): (1<<0) left; (1<<1) right; (1<<2) middle
   @return none
*/
void mouseBTPress(uint8_t mousebutton)
{
  activeMouseButtons |= mousebutton;
  //update buttons without sending relative data for x/y/wheel
  mouseBT(0, 0, 0);
}

/**
   @name mouseBTRelease
   @param mousebutton uint8_t contains all buttons to be release (masked): (1<<0) left; (1<<1) right; (1<<2) middle
   @return none
*/
void mouseBTRelease(uint8_t mousebutton)
{
  activeMouseButtons &= ~mousebutton;
  //update buttons without sending relative data for x/y/wheel
  mouseBT(0, 0, 0);
}

/**
   @name isMouseBTPressed
   @param mousebutton buttons which should be polled
   @return boolan
*/
boolean isMouseBTPressed(uint8_t mousebutton)
{
  if (activeMouseButtons & mousebutton) return true;
  return false;
}


/**
   @name sendBTKeyboardReport
   @param none
   @return none

   Sends a full keyboard report where all keys contained in activeKeyCodes
   and activeModifierKeys will be sent

   @todo Should we send with a different API here when upgrading to ESP32miniBT v0.2?
*/
void sendBTKeyboardReport()
{
#ifdef DEBUG_OUTPUT_FULL
  Serial.println("BT keyboard actions:");
  Serial.print("modifier: 0x");
  Serial.println(activeModifierKeys, HEX);
  Serial.println("activeKeyCodes: ");
  Serial.println(activeKeyCodes[0], HEX);
  Serial.println(activeKeyCodes[1], HEX);
  Serial.println(activeKeyCodes[2], HEX);
  Serial.println(activeKeyCodes[3], HEX);
  Serial.println(activeKeyCodes[4], HEX);
  Serial.println(activeKeyCodes[5], HEX);
#endif

  Serial_AUX.write(0xFD);       			//raw HID
  Serial_AUX.write(activeModifierKeys);  	//modifier keys
  Serial_AUX.write(0x00);
  Serial_AUX.write(activeKeyCodes[0]);	//key 1
  Serial_AUX.write(activeKeyCodes[1]);   	//key 2
  Serial_AUX.write(activeKeyCodes[2]);   	//key 3
  Serial_AUX.write(activeKeyCodes[3]);   	//key 4
  Serial_AUX.write(activeKeyCodes[4]);   	//key 5
  Serial_AUX.write(activeKeyCodes[5]);   	//key 6
}

/**
   @name keyboardBTPress
   @param int key	Keycode which should be pressed. Keycodes are in Teensy format
   @return none

    Press a defined key code.
    keycodes and modifier codes are extracted and sent to EZ-Key module via UART
    for keylayouts see: https://github.com/PaulStoffregen/cores/blob/master/teensy/keylayouts.h
*/
void keyboardBTPress(int key)
{
  uint8_t currentIndex = 0;
  uint8_t keyCode = (uint8_t)(key & 0xff);

  if ((key >> 8) ==  0xE0)  // supported modifier key ?
  {
    // set bit in modifier key mask
    activeModifierKeys |= keyCode;
  } else if ((key >> 8) ==  0xF0) { // supported key ?
    // check the active key codes for a free slot or overwrite the last one
    while ((activeKeyCodes[currentIndex] != 0) && (activeKeyCodes[currentIndex] != keyCode) && (currentIndex < 6))
      currentIndex++;
    //set the key code to the array
    activeKeyCodes[currentIndex] = keyCode;
  }

  //send the new keyboard report
  sendBTKeyboardReport();
}

/**
   @name keyboardBTRelease
   @param int key	Keycode which should be released. Keycodes are in Teensy format (16bit, divided into consumer keys, systemkeys & keyboard keys)
   @return none

   Release a defined key code.
*/
void keyboardBTRelease(int key)
{
  uint8_t currentIndex = 0;
  uint8_t keyCode = (uint8_t)(key & 0xff);

  if ((key >> 8) ==  0xE0)  // supported modifier key (see Teensy keylayouts.h)
  {
    // clear bit in modifier key mask
    activeModifierKeys &= ~keyCode;
  } else {
    //if not, check the active key codes for the pressed key
    while ((activeKeyCodes[currentIndex] != keyCode) && (currentIndex < 6)) currentIndex++;
    //delete the key code from the array
    for (int i = currentIndex; i < 5; i++)
      activeKeyCodes[i] = activeKeyCodes[i + 1];
    activeKeyCodes[5] = 0;
  }

  //send the new keyboard report
  sendBTKeyboardReport();
}

/**
   @name keyboardBTReleaseAll
   @param none
   @return none

   Release all previous pressed keyboard keys
*/
void keyboardBTReleaseAll()
{
  //reset all activeKeyCodes to 0x00
  for (uint8_t i = 0; i <= 5; i++) activeKeyCodes[i] = 0;
  //reset all modifier keys
  activeModifierKeys = 0;
  //send a keyboard report (now empty)
  sendBTKeyboardReport();
}


/**
   @name keyboardBTPrint
   @param char* writeString	string to typed by the Bluetooth HID keyboard
   @return none

   This method prints out an ASCII string (no modifiers available!!!) via the
   Bluetooth module

   @todo We should use the keyboard maps from ESP32, can store all of them. But how to handle any multibyte strings?
*/
void keyboardBTPrint(char * writeString)
{
  uint16_t i = 0;

  // print each char of the string
  while (writeString[i])
  {
    // Serial_AUX.write(writeString[i++]);

    // improved for localization / keycodes (but: slower ...)

    int keycode = 0, modifier = 0;

    // Serial.print("key ="); Serial.print(writeString[i]);
    if (writeString[i] < 128) {     // ASCII
      // Serial.print(" ASCII ="); Serial.println((int)writeString[i]);
      keycode = pgm_read_byte(keycodes_ascii + (writeString[i] - 0x20));
    }
    else  {  // ISO_8859
#ifdef ISO_8859_1_A0
      // Serial.print(" ISO_8859 ="); Serial.println((int)writeString[i]);
      keycode = pgm_read_byte(keycodes_iso_8859_1 + (writeString[i] - 0xA0));
#endif
    }

    if (keycode & 0x40) {  // SHIFT
      // Serial.print("SHIFT+");
      keycode &= ~0x40;
      modifier = 0xe002;
    } else if (keycode & 0x80) {  // ALTGR
      // Serial.print("ALTGR+");
      keycode &= ~0x80;
      modifier = 0xe040;
    }
    // Serial.print("HID =");
    // Serial.println(keycode);

    if (modifier) keyboardBTPress(modifier);
    keyboardBTPress(keycode | 0xf000);
    keyboardBTRelease(keycode | 0xf000);
    if (modifier) keyboardBTRelease(modifier);
    i++;
  }
}

/**
   @name initBluetooth
   @param none
   @return none

   Initialize the Bluetooth module on the external serial port.
   If the module returns a valid version string, BT communication is
   enabled (bt_enable is set to 1).

   @see bt_enable

*/
void initBluetooth()
{
#ifdef DEBUG_OUTPUT_FULL
  Serial.println("init Bluetooth");
#endif

  //start the AUX serial port 9600 8N1
  Serial_AUX.begin(9600);
  bt_available = 1;

  ///@todo send identifier to BT module & check response. With BT addon this is much faster and reliable
  bt_esp32addon = EZKEY;

}

/**
   @name setBTName
   @param char * BTName: module name for BT-advertising
   @return none

   sets the BT module name for advertising/pairing.

*/
void setBTName(char * BTName) {
  //set moduel name for BT advertising
  Serial_AUX.print("$NAME ");
  Serial_AUX.println(BTName);  
}

/**
   @name unpairAllBT
   @return none

   forget all paired devices

*/
void unpairAllBT() {
  Serial_AUX.println("$DP");
}


/**
   @name isBluetoothAvailable
   @param none
   @return true, if the BT module is available, false if not

   This method returns true, if the BT module is available and delivered
   a valid version string
   False will be returned otherwise
*/
bool isBluetoothAvailable()
{
  return bt_available;
}

/**
   @name startBTPairing
   @param none
   @return none
   @note Not implemented
*/
bool startBTPairing()
{
  //we will send a command to the BT addon board here.
  ///@todo which command & implement on BT addon
  return true;
}


/**
   @name performAddonUpgrade
   @param none
   @return none

   handle states and data transfer for BT-Addon firmware update
*/
void performAddonUpgrade() 
{
  //update start
  if(addonUpgrade == BTMODULE_UPGRADE_START)
  {
    Serial_AUX.end();
    Serial_AUX.begin(500000); //switch to higher speed...
    Serial.flush();
    Serial_AUX.flush();
    //remove everything from buffers...
    while(Serial.available()) Serial.read();
    while(Serial_AUX.available()) Serial_AUX.read();
    addonUpgrade = BTMODULE_UPGRADE_RUNNING;
    upgradeTimestamp=millis();
    return;
  }

  if(addonUpgrade == BTMODULE_UPGRADE_RUNNING)
  {
    if(Serial.available()) upgradeTimestamp=millis();   // incoming data: assume working upgrade!
    else {
      // 20 seconds no data -> return to AT mode !
      if (millis()-upgradeTimestamp > 20000) {
        addonUpgrade = BTMODULE_UPGRADE_IDLE;
        Serial_AUX.begin(9600); //switch to lower speed...
        Serial.flush();
        Serial_AUX.flush();
        return;
      }
    }
        
    while(Serial.available()) Serial_AUX.write(Serial.read());
    while(Serial_AUX.available()) {
      int inByte = Serial_AUX.read();
      Serial.write(inByte);
      switch (readstate_f) {
        case 0:
            if (inByte=='$') readstate_f++;
           break;
        case 1:
            if (inByte=='F') readstate_f++; else readstate_f=0;
          break;
        case 2:
            if (inByte=='I') readstate_f++; else readstate_f=0;
          break;
        case 3:
            if (inByte=='N') {
            addonUpgrade = BTMODULE_UPGRADE_IDLE;
            bt_available = 1;
            readstate_f=0;
            delay(50);
            Serial_AUX.begin(9600); //switch to lower speed...
            Serial.flush();
            Serial_AUX.flush();
            } else readstate_f=0;
          break;
        default: 
        readstate_f=0;
      }
    }
    return;
  }
}
