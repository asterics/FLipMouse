  
/* 
     FLipWare - AsTeRICS Academy 2016
     For more info please visit: http://www.asterics-academy.net

     Module: modes.h
     
          
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
#define STICKMODE_JOYSTICK_XY      2
#define STICKMODE_JOYSTICK_ZR      3
#define STICKMODE_JOYSTICK_SLIDERS 4

void handleModeState(int x, int y, int pressure);

// extern uint8_t holdMode =HOLD_IDLE;


#endif

