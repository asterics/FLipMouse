/*
	FLipWare - AsTeRICS Foundation
	Copyright (c) Benjamin Aigner
	For more info please visit: https://www.asterics-foundation.org

	Module: bluetooth.cpp - using external Bluetooth addon for mouse/keyboard control

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License:
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "bluetooth.h"
//necessary to include keyboard / keyboardlayout.h to have access to the key definitions
#include <Keyboard.h>
#include <KeyboardLayout.h>
//we fetch the keyboard layout map via keys.h
#include "keys.h"

#define BT_MINIMUM_SENDINTERVAL 20     // reduce mouse reports in BT mode (in milliseconds) !

typedef enum {NONE, EZKEY, MINIBT01, MINIBT02} addontype_t;

static char macaddress[] = "00 00 00 00 00 00";  // len: 18
uint8_t bt_connected = 0;

uint8_t bt_available = 0;
addontype_t bt_esp32addon = NONE;
uint8_t activeKeyCodes[6];
uint8_t activeModifierKeys = 0;
uint8_t activeMouseButtons = 0;

long btsendTimestamp = millis();
unsigned long upgradeTimestamp = 0;          // eventually come back to AT mode from an unsuccessful BT module upgrade !
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
      ((uint32_t)abs((long int)(millis()-btsendTimestamp)) > BT_MINIMUM_SENDINTERVAL ))
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

    //wheel. Unsupported by EZKey, but implement in our ESP32 module
    Serial_AUX.write((uint8_t)scroll);
    //some additional bytes...
    Serial_AUX.write((uint8_t)0x00);
    Serial_AUX.write((uint8_t)0x00);

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

  Serial_AUX.write((uint8_t)0xFD);       			//raw HID
  Serial_AUX.write((uint8_t)activeModifierKeys);  	//modifier keys
  Serial_AUX.write((uint8_t)0x00);
  Serial_AUX.write((uint8_t)activeKeyCodes[0]);	//key 1
  Serial_AUX.write((uint8_t)activeKeyCodes[1]);   	//key 2
  Serial_AUX.write((uint8_t)activeKeyCodes[2]);   	//key 3
  Serial_AUX.write((uint8_t)activeKeyCodes[3]);   	//key 4
  Serial_AUX.write((uint8_t)activeKeyCodes[4]);   	//key 5
  Serial_AUX.write((uint8_t)activeKeyCodes[5]);   	//key 6
}

/**
   @name keyboardBTPress
   @param int k	Key to be pressed
   @return none

   Press a key, value is the same as in Keyboard.press().
   Because the Keyboard library does not export the raw keycodes or
   the full report, we copy the code of the Keyboard library to here.
*/
void keyboardBTPress(int k)
{
  uint8_t currentIndex = 0;
  const uint8_t *_asciimap = getKeyboardLayout();
  if(_asciimap == 0) return; //invalid layout pointer.
  
  if (k >= 136) {			// it's a non-printing key (not a modifier)
    k = k - 136;
  } else if (k >= 128) {	// it's a modifier key
    activeModifierKeys |= (1<<(k-128));
    k = 0;
  } else {				// it's a printing key
    k = pgm_read_byte(_asciimap + k);
    if (!k) {
      return;
    }
    if ((k & ALT_GR) == ALT_GR) {
      activeModifierKeys |= 0x40;   // AltGr = right Alt
      k &= 0x3F;
    } else if ((k & SHIFT) == SHIFT) {
      activeModifierKeys |= 0x02;	// the left shift modifier
      k &= 0x7F;
    }
    if (k == ISO_REPLACEMENT) {
      k = ISO_KEY;
    }
  }

  // check the active key codes for a free slot or overwrite the last one
  while ((activeKeyCodes[currentIndex] != 0) && (activeKeyCodes[currentIndex] != k) && (currentIndex < 6))
    currentIndex++;
  //set the key code to the array
  activeKeyCodes[currentIndex] = k;
  //send the new keyboard report
  sendBTKeyboardReport();
}

/**
   @name keyboardBTRelease
   @param int k	Key to be released
   @return none

   Release a key, value is the same as in Keyboard.release().
   Because the Keyboard library does not export the raw keycodes or
   the full report, we copy the code of the Keyboard library to here.
*/
void keyboardBTRelease(int k)
{
	uint8_t currentIndex = 0;
  const uint8_t *_asciimap = getKeyboardLayout();
  if(_asciimap == 0) return; //invalid layout pointer.
  
	if (k >= 136) {			// it's a non-printing key (not a modifier)
		k = k - 136;
	} else if (k >= 128) {	// it's a modifier key
		activeModifierKeys &= ~(1<<(k-128));
		k = 0;
	} else {				// it's a printing key
		k = pgm_read_byte(_asciimap + k);
		if (!k) {
			return;
		}
		if ((k & ALT_GR) == ALT_GR) {
			activeModifierKeys &= ~(0x40);   // AltGr = right Alt
			k &= 0x3F;
		} else if ((k & SHIFT) == SHIFT) {
			activeModifierKeys &= ~(0x02);	// the left shift modifier
			k &= 0x7F;
		}
		if (k == ISO_REPLACEMENT) {
			k = ISO_KEY;
		}
	}
  //check the active key codes for the pressed key
  while ((activeKeyCodes[currentIndex] != k) && (currentIndex < 6)) currentIndex++;
  //delete the key code from the array
  for (int i = currentIndex; i < 5; i++)
    activeKeyCodes[i] = activeKeyCodes[i + 1];
  activeKeyCodes[5] = 0;

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

   This method prints out an ASCII string via the
   Bluetooth module
*/
void keyboardBTPrint(char * writeString)
{
  uint16_t i = 0;

  // print each char of the string
  while (writeString[i])
  {
    keyboardBTPress(writeString[i]);
    delay(10);
    keyboardBTRelease(writeString[i]);
    delay(10);
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
  Serial_AUX.begin(115200);  // NOTE: changed for RP2040! TODO: maybe get from revision number (>=3)?
  
  resetBTModule(0);  // start ESP32 module!
  delay (500);
  Serial_AUX.flush();
  
  bt_available = 1;

  ///@todo send identifier to BT module & check response. With BT addon this is much faster and reliable
  bt_esp32addon = EZKEY;

}

/**
   @name detectBTResponse
   @param int c: incoming character from BT module
   @return outgoming character (value for c is propagated)

   detects certain replies from the BT module (eg. if a paired connection was returned after sendind $GC)
*/
int detectBTResponse (int c)
{
  #define STATE_FIND_MESSAGE 0
  #define STATE_GET_ARGUMENT 1
  
  static char messageConnected[] = "CONNECTED:";
  static unsigned int checkpos=0;
  static int state=STATE_FIND_MESSAGE;

  switch (state) {
    case  STATE_FIND_MESSAGE:    
          if (c==messageConnected[checkpos++]) {
            if (checkpos == strlen(messageConnected)) {
              state=STATE_GET_ARGUMENT;
              checkpos=0;
            }
          }
          else checkpos=0;
          break;
    case  STATE_GET_ARGUMENT:
          macaddress[checkpos++]=(char)c;
          if (checkpos>=sizeof(macaddress)-1) {
            // Serial.println("pairing found:"); Serial.println(macaddress);
            checkpos=0;
            bt_connected=1;
            state=STATE_FIND_MESSAGE;
          }
          break;
   //TBD: Eventually assign a BLE device to one slot, MAC addresses are accessible via "$GP" (read) / "$SW" (select device)

  }
  return(c);  
}

/**
   @name updateBTConnectionState
   @return none

   periodically polls the BT modue for connections
*/
void updateBTConnectionState () {
  static uint32_t timestamp=0;
  if (millis()-timestamp >= 2000)  {  // every 2 seconds
      timestamp=millis();      
      if (isBluetoothAvailable()) {
        Serial_AUX.write("$GC\n");
        // digitalWrite (6,!digitalRead (6));
        bt_connected=0;  // will be updated in case the BT-module sends back connection/mac address
      }
  }
}


/**
   @name isBluetoothConnected
   @param none
   @return true, if the BT module is connected (paired) false if not

   This method returns true, if the BT module is currently paired to a host device
   False will be returned otherwise
*/
bool isBluetoothConnected() {
  return(bt_connected);
}


/**
   @name setBTName
   @param char * BTName: module name for BT-advertising
   @return none

   sets the BT module name for advertising/pairing.

*/
void setBTName(char * BTName) {
  //set module name for BT advertising
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
      if((uint32_t)abs((long int)(millis()-upgradeTimestamp)) > 20000) {
        addonUpgrade = BTMODULE_UPGRADE_IDLE;
        Serial_AUX.begin(115200); //switch to lower speed...   // NOTE: changed for RP2040! 
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
            Serial_AUX.begin(115200); //switch to lower speed...  // NOTE: changed for RP2040! 
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


// NOTE: changed for RP2040! 
/**
   @name resetBTModule
   @param downloadMode if true, ESP32 is put in FW download mode
   @return none

   resets the ESP32 connected to the RP2020 on the ArduinoNanoConnect board
*/
void resetBTModule (int downloadMode)
{
  pinMode (6,OUTPUT); digitalWrite (6, HIGH);  // orange led  on ArduinoNano2040Connect

  if (downloadMode) {
     Serial.println ("ESP32 put into download mode!");    
     pinMode (2,OUTPUT); digitalWrite (2, LOW);   // ESP32 GPIO0 pin on ArduinoNano2040Connect
     delay(100);
     pinMode (3,OUTPUT); digitalWrite (3, LOW);   // ESP32 reset pin on ArduinoNano2040Connect
     delay(100);
     digitalWrite (3, HIGH);  // release reset
     delay(1000);
     digitalWrite (2, HIGH);  // release GPIO0
  } else {
    Serial.println ("ESP32 reset!"); 
    pinMode (3,OUTPUT);
    digitalWrite (3, LOW);   // ESP32 reset pin on ArduinoNano2040Connect
    delay(100);
    digitalWrite (3, HIGH);  // release reset
  }
 
  digitalWrite (6, LOW);
}
