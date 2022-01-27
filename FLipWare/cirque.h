/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: cirque.h - implementation of the cirque pinnacle trackpad interface, header file

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _CIRQUE_H_
#define _CIRQUE_H_

#define CIRQUE_STATE_INVALID  0
#define CIRQUE_STATE_LIFTOFF  1
#define CIRQUE_STATE_HOVERING 2
#define CIRQUE_STATE_VALID    3

#define DRAG_DISABLED    0
#define DRAG_NORMAL      1

#define DRAG_ACTION_UP    1
#define DRAG_ACTION_DOWN  2
#define DRAG_ACTION_LEFT  3
#define DRAG_ACTION_RIGHT 4

#define DRAG_ACTION_DISTANCE 75
#define DRAG_AUTOMOVE_SPEED 3
#define DRAG_ACTION_TIMELIMIT 500

extern uint8_t cirqueInstalled;

int updateCirquePad(int * x, int * y);
int initCirque();
int useAbsolutePadValues();
uint8_t handleTapClicks(int state,int tapTime);

#endif
