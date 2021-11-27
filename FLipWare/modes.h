/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: modes.cpp - implementation of stick operation and special modes, header file


   For a list of supported AT commands, see commands.h / commands.cpp

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _MODES_H_
#define _MODES_H_

#define HOLD_IDLE 0
#define HOLD_X    1
#define HOLD_Y    2
#define HOLD_PRESSURE 3

#define STICKMODE_ALTERNATIVE      0
#define STICKMODE_MOUSE            1
#define STICKMODE_PAD              2  // TBD: use correct values 5,6.. / update GUI
#define STICKMODE_PAD_ALTERNATIVE  3     
#define STICKMODE_JOYSTICK_XY      4  // TBD: joystick modes should stay 2,3,4
#define STICKMODE_JOYSTICK_ZR      5
#define STICKMODE_JOYSTICK_SLIDERS 6

void handleModeState(int x, int y, int pressure);
int useAbsolutePadValues();
void handleModeState(int x, int y, int pressure);

// extern uint8_t holdMode =HOLD_IDLE;


#endif
