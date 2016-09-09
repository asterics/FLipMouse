/* 
	FLipWare - AsTeRICS Academy 2016
	Copyright (c) Benjamin Aigner
	For more info please visit: http://www.asterics-academy.net

	Module: blueooth.cpp - using external Bluefruit EZ-KEY HID as BT communication

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.
  
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License: 
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/  

#include "bluetooth.h"


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
void mouseBT(uint8_t x, uint8_t y, uint8_t scroll)
{
	//test if the external serial interface is available
	if(bt_available && Serial_AUX.available() > 0)
	{
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
		Serial_AUX.write(x);
		Serial_AUX.write(y);
		
		//maybe the wheel? Not official by Adafruit... -> not working
		Serial_AUX.write((uint8_t)0x00); 
		//some additional bytes...
		Serial_AUX.write((uint8_t)0x00);
		Serial_AUX.write((uint8_t)0x00);
	} else {
		if(DebugOutput == DEBUG_FULLOUTPUT) 
		{
			Serial.println("BT not available");
		}
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
 * name: mapArduinoToBTKey
 * @param int key key number from the arduino keyboard lib
 * @param uint8_t* isModifier reference to a variable, which will be changed if the key is a modifier
 * @return key number for the EZ Key Bluetooth HID
 * 
 * Returns 0 if key is not a valid key, the key code otherwise.
 * If the key is a modifier, the isModifier reference will be set to 1
 */
uint8_t mapArduinoToBTKey(int key, uint8_t *isModifier)
{
	switch(key)
	{
		case KEY_A: return 0x04;
		case KEY_B: return 0x05;
		case KEY_C: return 0x06;
		case KEY_D: return 0x07;
		case KEY_E: return 0x08;
		case KEY_F: return 0x09;
		case KEY_G: return 0x0A;
		case KEY_H: return 0x0B;
		case KEY_I: return 0x0C;
		case KEY_J: return 0x0D;
		case KEY_K: return 0x0E;
		case KEY_L: return 0x0F;
		case KEY_M: return 0x10;
		case KEY_N: return 0x11;
		case KEY_O: return 0x12;
		case KEY_P: return 0x13;
		case KEY_Q: return 0x14;
		case KEY_R: return 0x15;
		case KEY_S: return 0x16;
		case KEY_T: return 0x17;
		case KEY_U: return 0x18;
		case KEY_V: return 0x19;
		case KEY_W: return 0x1A;
		case KEY_X: return 0x1B;
		case KEY_Y: return 0x1C;
		case KEY_Z: return 0x1D;
		
		case KEY_1: return 0x1E;
		case KEY_2: return 0x1F;
		case KEY_3: return 0x20;
		case KEY_4: return 0x21;
		case KEY_5: return 0x22;
		case KEY_6: return 0x23;
		case KEY_7: return 0x24;
		case KEY_8: return 0x25;
		case KEY_9: return 0x26;
		case KEY_0: return 0x27;
		
		case KEY_F1: return 0x3A;
		case KEY_F2: return 0x3B;
		case KEY_F3: return 0x3C;
		case KEY_F4: return 0x3D;
		case KEY_F5: return 0x3E;
		case KEY_F6: return 0x3F;
		case KEY_F7: return 0x40;
		case KEY_F8: return 0x41;
		case KEY_F9: return 0x42;
		case KEY_F10: return 0x43;
		case KEY_F11: return 0x44;
		case KEY_F12: return 0x45;
		
		case KEY_INSERT: return 0x49;
		case KEY_HOME: return 0x4A;
		case KEY_PAGE_UP: return 0x4B;
		case KEY_DELETE: return 0x4C;
		case KEY_END: return 0x4D;
		case KEY_PAGE_DOWN: return 0x4E;
		
		case KEY_SCROLL_LOCK: return 0x47;
		case KEY_PAUSE: return 0x48;
		case KEY_NUM_LOCK: return 0x53;
		case KEY_PRINTSCREEN: return 0x46;
		
		case KEY_LEFT_SHIFT: *isModifier = 1; return (1<<1);
		case KEY_LEFT_CTRL: *isModifier = 1; return (1<<0);
		case KEY_LEFT_ALT: *isModifier = 1; return (1<<2);
		case KEY_RIGHT_ALT: *isModifier = 1; return (1<<6);
		case KEY_LEFT_GUI: *isModifier = 1; return (1<<3);
		case KEY_RIGHT_GUI: *isModifier = 1; return (1<<7);
		case KEY_UP: return 0x52;
		case KEY_DOWN: return 0x51;
		case KEY_LEFT: return 0x50;
		case KEY_RIGHT: return 0x4F;
		
		case KEY_ENTER: return 0x28;
		case KEY_SPACE: return 0x2C;
		case KEY_ESC: return 0x29;
		case KEY_TAB: return 0x2B;
		case KEY_BACKSPACE: return 0x2A;
		case KEY_CAPS_LOCK: return 0x39;
		
		default: return 0x00;
	}
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
	//test if the external serial interface is available
	if(bt_available && Serial_AUX.available() > 0)
	{
		if(DebugOutput == DEBUG_FULLOUTPUT)
		{
			Serial.println("BT keyboard actions:");
			Serial.print("modifier: 0x");
			Serial.println(activeModifierKeys,HEX);
			Serial.print("activeKeyCodes: ");
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
}

/*
 * 
 * name: keyboardBTPress
 * @param int key	Keycode which should be pressed. Keycodes are in Teensy format,
 * 					well be mapped here to the EZ-KEY keycode set
 * @return none
 * 
 * Press a defined key code. There is a major difference between Teensy/Arduino keycodes
 * and the used keycodes for the EZ-Key. The mapping from Arduino/Tennsy will be done here.
 */
void keyboardBTPress(int key) 
{
	uint8_t isModifier = 0; //will be set by mapArduinotoBTKey()
	uint8_t btKeyCode = mapArduinoToBTKey(key,&isModifier); //map the arduino to the BT key
	uint8_t currentIndex = 0;
	
	//check if it is a modifier key
	if(isModifier)
	{
		//if it is, change bit in modifier key mask
		activeModifierKeys |= btKeyCode;
	} else {
		//if not, check the active key codes for a free slot or overwrite the last one
		while(activeKeyCodes[currentIndex] != 0 && currentIndex < 6) currentIndex++;
		//set the key code to the array
		activeKeyCodes[currentIndex] = btKeyCode;
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
 * Release a defined key code. There is a major difference between Teensy/Arduino keycodes
 * and the used keycodes for the EZ-Key. The mapping from Arduino/Tennsy will be done here.
 */
void keyboardBTRelease(int key) 
{
	uint8_t isModifier = 0; //will be set by mapArduinotoBTKey()
	uint8_t btKeyCode = mapArduinoToBTKey(key,&isModifier); //map the arduino to the BT key
	uint8_t currentIndex = 0;
	
	//check if it is a modifier key
	if(isModifier)
	{
		//if it is, change bit in modifier key mask
		activeModifierKeys &= ~btKeyCode;
	} else {
		//if not, check the active key codes for the pressed key
		while(activeKeyCodes[currentIndex] != btKeyCode && currentIndex < 6) currentIndex++;
		//delete the key code from the array
		activeKeyCodes[currentIndex] = 0x00;
		//resave all keys after the deleted one to close a fragmented array
		for(int i=currentIndex; i<5;i++)
		{
			activeKeyCodes[i] = activeKeyCodes[i+1];
		}
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
	//test if the external serial interface is available
	if(bt_available && Serial_AUX.available() > 0)
	{
		//print each char until string is terminated
		while(*(writeString+i) != 0)
		{
			Serial_AUX.write(*(writeString+i));
			i++;
		}
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
	//start the AUX serial port 9600 8N1
	Serial_AUX.begin(9600);
	//set a short timeout, that the FLipMouse does not freeze if no BT module is connected
	Serial_AUX.setTimeout(1000);
	//receive a reply string from the BT module (if available)
	String reply = Serial_AUX.readStringUntil('\n');
	//try 2 times, maybe there is a \n BEFORE the version string
	if(reply.length() < 2) reply = Serial_AUX.readStringUntil('\n');
	
	   
	//wait 10s before startup, to have enough time to open the serial connection and see the output
	if(DebugOutput == DEBUG_FULLOUTPUT) delay(10000);
	
	//test for the Bluefruit version string
	if(reply.indexOf("Adafruit Bluefruit HID") != -1)
	{
		//if available, enable bluetooth
		bt_available = 1;
		
		//if debug output is active, print out info and version string
		if(DebugOutput==DEBUG_FULLOUTPUT)
		{
			Serial.println("Bluetooth module found:");
			Serial.println(reply);
		}
	} else {
		if(DebugOutput == DEBUG_FULLOUTPUT)
		{
			Serial.println("No Bluetooth module found, reply:");
			Serial.println(reply);
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

