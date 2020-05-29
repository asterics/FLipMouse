/* 
	FLipWare - AsTeRICS Foundation
	Copyright (c) Benjamin Aigner
	For more info please visit: http://www.asterics-academy.net

	Module: bluetooth.h - using external Bluetooth addon for mouse/keyboard control

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

/**
 * 
 * name: mouseBT
 * @þaram x relative movement x axis
 * @þaram y relative movement y axis
 * @þaram scroll relative scroll actions
 * @return 
 * 
 * this method sends a mouse command via the Bluetooth module.
 * Mouse movements, buttons and scroll wheel.
 * The limit for the movement is +127/-127
 */
void mouseBT(int x, int y, uint8_t scroll);


/**
 * @name mouseBTPress
 * @param mousebutton uint8_t contains all buttons to be pressed (masked): (1<<0) left; (1<<1) right; (1<<2) middle
 * @return none
 */
void mouseBTPress(uint8_t mousebutton);

/**
 * @name mouseBTRelease
 * @param mousebutton uint8_t contains all buttons to be release (masked): (1<<0) left; (1<<1) right; (1<<2) middle
 * @return none
 */
void mouseBTRelease(uint8_t mousebutton);


/**
 * @name isMouseBTPressed
 * @param mousebutton buttons which should be polled 
 * @return boolan 
 */
boolean isMouseBTPressed(uint8_t mousebutton);
 
/**
 * 
 * name: initBluetooth
 * @param none
 * @return none
 * 
 * Initialize the Bluetooth module on the external serial port.
 * If the module returns a valid version string, BT communication is
 * enabled (bt_enable is set to 1).
 * 
 * @see bt_enable
 * 
 */
void initBluetooth();

/**
 * 
 * name: keyboardBTPrint
 * @param char* writeString	string to typed by the Bluetooth HID keyboard
 * @return none
 * 
 * This method prints out an ASCII string (no modifiers available!!!) via the
 * Bluetooth module
 * 
 * @todo We should use the keyboard maps from ESP32, can store all of them. But how to handle any multibyte strings?
 */
void keyboardBTPrint(char * writeString);

/**
 * 
 * name: keyboardBTReleaseAll
 * @param none
 * @return none
 * 
 * Release all previous pressed keyboard keys
 */
void keyboardBTReleaseAll();

/**
 * 
 * name: keyboardBTPress
 * @param int key	Keycode which should be pressed. Keycodes are in Teensy format
 * @return none
 * 
 *  Press a defined key code.
 *  keycodes and modifier codes are extracted and sent to EZ-Key module via UART
 *  for keylayouts see: https://github.com/PaulStoffregen/cores/blob/master/teensy/keylayouts.h 
 */
void keyboardBTPress(int key);

/**
 * 
 * name: keyboardBTRelease
 * @param int key	Keycode which should be released. Keycodes are in Teensy format (16bit, divided into consumer keys, systemkeys & keyboard keys)
 * @return none
 * 
 * Release a defined key code.
 */
void keyboardBTRelease(int key);

/**
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

/**
 * 
 * name: startBTPairing
 * @param none
 * @return none
 * @note Not implemented
 */
bool startBTPairing();


/**
 * name: joystickBTAxis
 * @param axistype Determine axis for these values: 
 * 			- STICKMODE_JOYSTICK_XY Assign value1 to X, and value2 to Y
 * 			- STICKMODE_JOYSTICK_ZR Assign value1 to Z, and value2 to Zrotate
 * 			- STICKMODE_JOYSTICK_SLIDERS Assign value1 to slider left, and value2 to slider right
 * @param value1 Value 1 for the selected axis
 * @param value2 Value 2 for the selected axis
 * @note If one parameter is set to -1, it won't be updated.
 * set a pair of joystick axis to the defined values.
 */
void joystickBTAxis(uint8_t axistype, unsigned int value1, unsigned int  value2);

/**
 * name: joystickBTButtons
 * @param buttonnr Number of button to be pressed/released
 * @param value State of button (false/0 for released, true/1 for pressed)
 * @note Button number is limited to 31! (32buttons)
 * This function sets the state of all buttons for the joystick.
 */
void joystickBTButtons(uint8_t buttonnr, bool value);

/**
 * name: joystickHat
 * @param angle Value of hat, possible values: 0,45,90,135,180,225,270,315,-1
 * 
 * This functions sets the hat position. Center/released position is -1
 */
void joystickBTHat(int16_t angle);

/**
 * 
 * name: sendBTJoystickReport
 * @param none
 * @return none
 * 
 * Sends a full joystick report with all buttons and axes
 */
void sendBTJoystickReport();


#endif
