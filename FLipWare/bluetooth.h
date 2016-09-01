/* 
	FLipWare - AsTeRICS Academy 2016
	Copyright (c) Benjamin Aigner
	For more info please visit: http://www.asterics-academy.net

	Module: blueooth.h - using external Bluefruit EZ-KEY HID as BT communication

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.
  
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License: 
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/ 


#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "FlipWare.h"


//RX/TX3 are used to communicate with an addon board (mounted on AUX header)
#define Serial_AUX Serial3

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
void mouseBT(uint8_t x, uint8_t y, uint8_t scroll);


/*
 * @name mouseBTPress
 * @param mousebutton uint8_t contains all buttons to be pressed (masked): (1<<0) left; (1<<1) right; (1<<2) middle
 * @return none
 */
void mouseBTPress(uint8_t mousebutton);

/*
 * @name mouseBTRelease
 * @param mousebutton uint8_t contains all buttons to be release (masked): (1<<0) left; (1<<1) right; (1<<2) middle
 * @return none
 */
void mouseBTRelease(uint8_t mousebutton);

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
void initBluetooth();

/*
 * 
 * name: keyboardBTPrint
 * @param char* writeString	string to typed by the Bluetooth HID keyboard
 * @return none
 * 
 * This method prints out an ASCII string (no modifiers available!!!) via the
 * Bluetooth module
 */
void keyboardBTPrint(char * writeString);

/*
 * 
 * name: keyboardBTReleaseAll
 * @param none
 * @return none
 * 
 * Release all previous pressed keyboard keys
 */
void keyboardBTReleaseAll();

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
void keyboardBTPress(int key);

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
void keyboardBTRelease(int key);

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
bool isBluetoothAvailable();


#endif
