/* 
	FLipWare - AsTeRICS Foundation 2017
	For more info please visit: http://www.asterics-academy.net

	Module: hid_hal.h - covers USB HID and BT HID send routines

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.
  
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License: 
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/ 


#ifndef _HID_HAL_H_
#define _HID_HAL_H_

#include "FlipWare.h"

/*
 * 
 * name: keyboardPrint
 * @param char* keyString string to be typed by keyboard
 * @return none
 * 
 * This method prints out an ASCII string (no modifiers available!) 
 */
void keyboardPrint(char * keyString);

/*
 * 
 * name: keyboardPress
 * @param int key keycode to be typed by keyboard
 * @return none
 * 
 * This method presses a key of given keycode (modifiers available!) 
 */
void keyboardPress(int key);

/*
 * 
 * name: keyboardRelease
 * @param int key keycode to be released by keyboard
 * @return none
 * 
 * This method releases a key of given keycode (modifiers available!) 
 */
void keyboardRelease(int key);

/*
 * 
 * name: keyboardReleaseAll
 * @param none
 * @return none
 * 
 * This method releases all currently pressed keys 
 */
void keyboardReleaseAll();

/*
 * 
 * name: mousePress
 * @param uint8_t button	button code which should be pressed. 
 * @return none
 * 
 * Press a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE) 
 */
void mousePress(uint8_t button);

/*
 * 
 * name: mouseRelease
 * @param uint8_t button	button code which should be released. 
 * @return none
 * 
 * Release a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE) 
 */
void mouseRelease(uint8_t button);


/*
 * 
 * name: mouseScroll
 * @param int8_t steps  steps to scroll. 
 * @return none
 * 
 * scrolls a number of steps 
 */
void mouseScroll(int8_t steps);


/*
 * 
 * name: mouseMove
 * @param int x	 movement in x direction 
 * @param int y	 movement in y direction 
 * @return none
 * 
 * moves the mouse cursor a defined nmber of steps. (if x or y are > 127, multiple HID commands are generated)  
 */
void mouseMove(int x, int y);



#endif
