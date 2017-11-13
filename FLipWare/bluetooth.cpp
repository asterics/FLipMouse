/* 
	FLipWare - AsTeRICS Foundation 2017
	Copyright (c) Benjamin Aigner
	For more info please visit: http://www.asterics-academy.net

	Module: bluetooth.cpp - using external Bluefruit EZ-KEY HID as BT communication

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.
  
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License: 
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/  

#include "bluetooth.h"

#define BT_DATA_REDUCTION 4    // reduce mouse report frequency in BT mode  !

uint8_t bt_available = 0;
uint8_t activeKeyCodes[6];
uint8_t activeModifierKeys = 0;
uint8_t activeMouseButtons = 0;



/*
 * 
 * name: mouseBT
 * @þaram x uint8_t relative movement x axis
 * @þaram y uint8_t relative movement y axis
 * @þaram scroll uint8_t relative scroll actions
 * @return --
 * 
 * this method sends a mouse command via the Bluetooth module.
 * Mouse movements, buttons and scroll wheel (possibly no functioning, not an official EZ-KEY feature)
 * Update: mouse wheel does not work.
 * The limit for the movement is +127/-127
 */
void mouseBT(int x, int y, uint8_t scroll)
{
    static int oldMouseButtons=0;
    static int sendCnt=0;
    static int accuX=0, accuY=0;
    
		if(DebugOutput == DEBUG_FULLOUTPUT)
		{
			Serial.println("BT mouse actions:");
			Serial.print("buttons: 0x");
			Serial.println(activeMouseButtons,HEX);
			Serial.print("x/y/scroll: ");
			Serial.print(x,DEC);
			Serial.print("/");
			Serial.print(y,DEC);
			Serial.print("/");
			Serial.println(scroll,DEC);
		}
		
		//according to:
		//https://learn.adafruit.com/introducing-bluefruit-ez-key-diy-bluetooth-hid-keyboard/sending-keys-via-serial

    sendCnt=(sendCnt+1) % BT_DATA_REDUCTION;
    accuX+=x;
    accuY+=y;
    
    if ((activeMouseButtons != oldMouseButtons) || (sendCnt==0))
    {    
  		//starting RAW HID mouse report
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

      sendCnt=0;
      accuX=0; accuY=0;
      oldMouseButtons=activeMouseButtons;
    }
}

/*
 * @name mouseBTPress
 * @param mousebutton uint8_t contains all buttons to be pressed (masked): (1<<0) left; (1<<1) right; (1<<2) middle
 * @return none
 */
void mouseBTPress(uint8_t mousebutton)
{
	activeMouseButtons |= mousebutton;
	mouseBT(0,0,0);
}

/*
 * @name mouseBTRelease
 * @param mousebutton uint8_t contains all buttons to be release (masked): (1<<0) left; (1<<1) right; (1<<2) middle
 * @return none
 */
void mouseBTRelease(uint8_t mousebutton)
{
	activeMouseButtons &= ~mousebutton;
	mouseBT(0,0,0);
}


/*
 * 
 * name: sendBTKeyboardReport
 * @param none
 * @return none
 * 
 * Sends a full keyboard report where all keys contained in activeKeyCodes
 * and activeModifierKeys will be sent
 */
void sendBTKeyboardReport() 
{
		if(DebugOutput == DEBUG_FULLOUTPUT)
		{
			Serial.println("BT keyboard actions:");
			Serial.print("modifier: 0x");
			Serial.println(activeModifierKeys,HEX);
			Serial.println("activeKeyCodes: ");
			Serial.println(activeKeyCodes[0],HEX);
			Serial.println(activeKeyCodes[1],HEX);
			Serial.println(activeKeyCodes[2],HEX);
			Serial.println(activeKeyCodes[3],HEX);
			Serial.println(activeKeyCodes[4],HEX);
			Serial.println(activeKeyCodes[5],HEX);
		}
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

/*
 * 
 * name: keyboardBTPress
 * @param int key	Keycode which should be pressed. Keycodes are in Teensy format,
 * 					well be mapped here to the EZ-KEY keycode set
 * @return none
 * 
 *  Press a defined key code.
 *  keycodes and modifier codes are extracted and sent to EZ-Key module via UART
 *  for keylayouts see: https://github.com/PaulStoffregen/cores/blob/master/teensy/keylayouts.h 
 */
void keyboardBTPress(int key) 
{
	uint8_t currentIndex = 0;
  uint8_t keyCode = (uint8_t)(key & 0xff);

  if ((key >> 8) ==  0xE0)  // supported modifier key ?
  {
		// set bit in modifier key mask
		activeModifierKeys |= keyCode;
	} 
	else if ((key >> 8) ==  0xF0)  // supported key ?
  {
		// check the active key codes for a free slot or overwrite the last one
		while((activeKeyCodes[currentIndex] != 0) && (activeKeyCodes[currentIndex] != keyCode) && (currentIndex < 6)) 
		   currentIndex++;
		//set the key code to the array
		activeKeyCodes[currentIndex] = keyCode;
	}
	
	//send the new keyboard report
	sendBTKeyboardReport();
}

/*
 * 
 * name: keyboardBTRelease
 * @param int key	Keycode which should be released. Keycodes are in Teensy format,
 * 					well be mapped here to the EZ-KEY keycode set
 * @return none
 * 
 * Release a defined key code.
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
		while((activeKeyCodes[currentIndex] != keyCode) && (currentIndex < 6)) currentIndex++;
		//delete the key code from the array
		for(int i=currentIndex; i<5;i++)
			activeKeyCodes[i] = activeKeyCodes[i+1];
    activeKeyCodes[5] = 0;
 }
	
	//send the new keyboard report
	sendBTKeyboardReport();
}

/*
 * 
 * name: keyboardBTReleaseAll
 * @param none
 * @return none
 * 
 * Release all previous pressed keyboard keys
 */
void keyboardBTReleaseAll()
{
	//reset all activeKeyCodes to 0x00
	for(uint8_t i=0; i<=5; i++) activeKeyCodes[i] = 0;
	//reset all modifier keys
	activeModifierKeys = 0;
	//send a keyboard report (now empty)
	sendBTKeyboardReport();
}


/*
 * 
 * name: keyboardBTPrint
 * @param char* writeString	string to typed by the Bluetooth HID keyboard
 * @return none
 * 
 * This method prints out an ASCII string (no modifiers available!!!) via the
 * Bluetooth module
 */
void keyboardBTPrint(char * writeString)
{
	uint16_t i = 0;

  // print each char of the string
  while(writeString[i])
	{
    // Serial_AUX.write(writeString[i]);
    // improved for localization / keycodes (but: slower ...)
    
    int keycode=0, modifier=0;
   
    // Serial.print("key ="); Serial.print(writeString[i]);
    if (writeString[i]<128) {       // ASCII
        // Serial.print(" ASCII ="); Serial.println((int)writeString[i]);
        keycode=pgm_read_byte(keycodes_ascii + (writeString[i] - 0x20));
    }
    else  {  // ISO_8859
    #ifdef ISO_8859_1_A0
      // Serial.print(" ISO_8859 ="); Serial.println((int)writeString[i]);
      keycode=pgm_read_byte(keycodes_iso_8859_1 + (writeString[i] - 0xA0)); 
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
	}
}

/*
 * 
 * name: initBluetooth
 * @param none
 * @return none
 * 
 * Initialize the Bluetooth module on the external serial port.
 * If the module returns a valid version string, BT communication is
 * enabled.
 * 
 */
void initBluetooth()
{

  if(DebugOutput==DEBUG_FULLOUTPUT)
    Serial.println("init Bluetooth");

	//start the AUX serial port 9600 8N1
	Serial_AUX.begin(9600);
	//set a short timeout, that the FLipMouse does not freeze if no BT module is connected
	Serial_AUX.setTimeout(1000);
	//receive a reply string from the BT module (if available)
	String reply = Serial_AUX.readStringUntil('\n');
	//try 2 times, maybe there is a \n BEFORE the version string
	if(reply.length() < 2) reply = Serial_AUX.readStringUntil('\n');

	//test for the Bluefruit version string
	if(reply.indexOf("Adafruit Bluefruit HID") != -1)
	{
                bt_available=1;
		//if debug output is active, print out info and version string
		if(DebugOutput==DEBUG_FULLOUTPUT)
		{
			Serial.println("Bluetooth module found:");
			Serial.println(reply);
		}
	} else {
                bt_available=1;
		if(DebugOutput == DEBUG_FULLOUTPUT)
		{
			//wait 10s before startup, to have enough time to open the serial connection and see the output
			//delay(10000);
			Serial.println("No Bluetooth module found, reply:");
			Serial.println(reply);
                        Serial.println("Overriding anyway, Adafruit modules are buggy");
		}
	}
}

/*
 * 
 * name: isBluetoothAvailable
 * @param none
 * @return true, if the BT module is available, false if not
 * 
 * This method returns true, if the BT module is available and delivered
 * a valid version string
 * False will be returned otherwise
 */
bool isBluetoothAvailable()
{
	return bt_available;
}

