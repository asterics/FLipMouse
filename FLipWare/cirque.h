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

/**
   constant definitions for touch state
*/
#define CIRQUE_STATE_INVALID  0
#define CIRQUE_STATE_LIFTOFF  1
#define CIRQUE_STATE_HOVERING 2
#define CIRQUE_STATE_VALID    3

/**
   constant definitions for drag mode / drag parameters
*/
#define DRAG_DISABLED    0
#define DRAG_NORMAL      1
#define DRAG_ACTION_TIMELIMIT 500
#define DRAG_ACTION_FORCE 20

#define DRAG_AUTOMOVE_DISTANCE 20
#define DRAG_AUTOMOVE_SPEED 1

/**
   constant definitions for drag gesture directions
*/
#define DRAG_ACTION_UP    1
#define DRAG_ACTION_DOWN  2
#define DRAG_ACTION_LEFT  3
#define DRAG_ACTION_RIGHT 4

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t cirqueInstalled;

/**
   @name updateCirquePad
   @brief get current x and y movement from trackpad
   @param x the current relative, rotation corrected x movement value will be stored here
   @param y the current relative, rotation corrected y movement value will be stored here
   @return the current trackpad state: CIRQUE_STATE_LIFTOFF, CIRQUE_STATE_HOVERING, 
                                       CIRQUE_STATE_VALID or CIRQUE_STATE_INVALID
*/
int updateCirquePad(int * x, int * y);

/**
   @name initCirquePad
   @brief initialize the i2C interface and the data structures for the trackpad
   @return true if cirquepad could be detected, false if not
*/
int initCirque();


/**
   @name handleTapClicks
   @brief detects taps and performs tap gesture actions if detected
   @param state the current state of the trackpad
   @param tapTime minimum tap time threshold
   @return none
*/
void handleTapClicks(int state,int tapTime);

#endif
