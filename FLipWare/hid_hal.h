/*
	FLipWare - AsTeRICS Foundation
	For more info please visit: https://www.asterics-foundation.org

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

#define DRAG_RECORDING_IDLE 0
#define DRAG_RECORDING_ACTIVE 1

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t dragRecordingState;
extern int16_t dragRecordingX;
extern int16_t dragRecordingY;

/*
   @name keyboardPrint
   @param char* keyString string to be typed by keyboard
   @return none

   This method prints out an ASCII string (no modifiers available!)
*/
void keyboardPrint(char * keyString);

/*
   @name keyboardPress
   @param int key keycode to be typed by keyboard
   @return none

   This method presses a key of given keycode (modifiers available!)
*/
void keyboardPress(int key);

/*
   @name keyboardRelease
   @param int key keycode to be released by keyboard
   @return none

   This method releases a key of given keycode (modifiers available!)
*/
void keyboardRelease(int key);

/*
   @name keyboardReleaseAll
   @param none
   @return none

   This method releases all currently pressed keys
*/
void keyboardReleaseAll();

/*
   @name mousePress
   @param uint8_t button	button code which should be pressed.
   @return none

   Press a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
*/
void mousePress(uint8_t button);

/*
   @name mouseToggle
   @param uint8_t button  button code which should be toggled.
   @return none

   Toggle a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
*/
void mouseToggle(uint8_t button);

/*
   @name mouseRelease
   @param uint8_t button	button code which should be released.
   @return none

   Release a defined mouse button (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
*/
void mouseRelease(uint8_t button);


/*
   @name mouseScroll
   @param int8_t steps  steps to scroll.
   @return none

   scrolls a number of steps
*/
void mouseScroll(int8_t steps);


/*
   @name mouseMove
   @param int x	 movement in x direction
   @param int y	 movement in y direction
   @return none

   moves the mouse cursor a defined nmber of steps. (if x or y are > 127, multiple HID commands are generated)
*/
void mouseMove(int x, int y);


/*
   @name joystickAxis
   @param int axis1       new value for axis 1 (either X,Z or sliderLeft; set by param select)
   @param int axis2       new value for axis 2 (either Y,Zrotate or sliderRight; set by param select)
   @param uint8_t select  define axis for values (0: X/Y; 1: Z/Zrotate; 2: sliderLeft/sliderRight)

   Updates 2 joystick axis with new values.
   Which axis are sent depends on the select parameter.
   
   @note The range for axis1 & axis2 is 0-1023.
   @note If an axis is set to -1, it will not be updated.
*/
void joystickAxis(int axis1, int axis2, uint8_t select);


/*
   @name joystickButton
   @param uint8_t nr    button number (1-32)
   @param int     val   state for button, 0 released; != 0 pressed

   Update joystick buttons.
*/
void joystickButton(uint8_t nr, int val);


/*
   @name joystickHat
   @param int     val   Hat position, 0-360 or -1

   Update joystick hat: 0-360 for position (mapped to 8 positions); -1 is rest position
*/
void joystickHat(int val);
#endif
