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
uint8_t activeModifierKeys;
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
		
		//maybe the wheel? Not official by Adafruit...
		Serial_AUX.write(scroll); 
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
 * name: sendBTKeyboardReport
 * @param none
 * @return none
 * 
 * Sends a full keyboard report where all keys contained in activeKeyCodes
 * and activeModifierKeys will be sent
 */
void sendBTKeyboardReport() 
{
	return; //TODO: activate if keycodes work...
	//test if the external serial interface is available
	if(bt_available && Serial_AUX.available() > 0)
	{
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
	//map key, check if it is a modifier key
	uint8_t btKeyCode = key; //todo...
	uint8_t isModifier = 0;
	uint8_t currentIndex = 0;
	
	//todo: map key to btKeyCode and set is modifier
	
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
	//delete from buffer
	//press remaining keys, determine if modifier or normal key
	
	//TODO!!!
	
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

