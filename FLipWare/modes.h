/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: modes.h - implementation of stick operation and special modes, header file


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

/**
   constant definitions of sip/puff and stick modes
*/
#define STRONGMODE_MOUSE_JOYSTICK_THRESHOLD  200
#define STRONGMODE_STABLETIME        15
#define STRONGMODE_EXIT_TIME         150
#define STRONGMODE_IDLE_TIME         120
#define SIP_PUFF_SETTLE_TIME         5
#define MIN_HOLD_TIME                3

#define SIP_PUFF_STATE_IDLE        0
#define SIP_PUFF_STATE_STARTED     1
#define SIP_PUFF_STATE_PRESSED     2

#define STRONG_MODE_IDLE                 0
#define STRONG_MODE_ENTER_STRONGPUFF     1
#define STRONG_MODE_STRONGPUFF_ACTIVE    2
#define STRONG_MODE_ENTER_STRONGSIP      3
#define STRONG_MODE_STRONGSIP_ACTIVE     4
#define STRONG_MODE_RELEASE              5
#define STRONG_MODE_RETURN_TO_IDLE       6

#define HOLD_IDLE     0
#define HOLD_X        1
#define HOLD_Y        2
#define HOLD_PRESSURE 3

#define STICKMODE_ALTERNATIVE      0
#define STICKMODE_MOUSE            1
#define STICKMODE_JOYSTICK_XY      2
#define STICKMODE_JOYSTICK_ZR      3
#define STICKMODE_JOYSTICK_SLIDERS 4

/**
   @name handleUserInteraction
   @brief applies all movement / action handling according to movement data and button modes of current slot
   @return none
*/
void handleUserInteraction();

#endif
