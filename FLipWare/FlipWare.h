
/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: FlipWare.h  - main header file

        This firmware allows control of HID functions via FLipmouse module and/or AT-commands
        For a description of the supported commands see: commands.h

        HW-requirements:
                  TeensyLC with external EEPROM (see FlipMouse board schematics)
                  4 FSR force sensors connected via voltage dividers to ADC pins A6-A9
                  1 pressure sensor connected to ADC pin A0
                  3 momentary switches connected to GPIO pins 0,1,2
                  3 slot indication LEDs connected to GPIO pins 5,16,17
                  1 TSOP 38kHz IR-receiver connected to GPIO pin 4
                  1 high current IR-LED connected to GPIO pin 6 via MOSEFT
                  optional: FlipMouse Bluetooth daughter board

        SW-requirements:
                  Teensyduino AddOn for Arduino IDE
                  USB-type set to USB composite device (Serial + Keyboard + Mouse + Joystick)

   For a list of supported AT commands, see commands.h / commands.cpp

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _FLIPWARE_H_
#define _FLIPWARE_H_

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <string.h>
#include <stdint.h>
#include "commands.h"
#include "eeprom.h"
#include "buttons.h"
#include "bluetooth.h"
#include "hid_hal.h"

#define VERSION_STRING "v2.12"

//  V2.12: added support for cirque trackpads as alternative to FSR sensor daughterboard
//  V2.11: eeprom access optimization and support for deletion / update of individual slots
//  V2.10: code size reduction: using floating point math, removed debug level control via AT E0, AT E1 and AT E2
//          added macro command description to the user manual
//  V2.9:  implemented drift correction for small deadzones, removed gain up/down/left/right,
//          added AT commands for drift correction, modified calculation of acceleration
//  V2.8.3: switched to semantic version numbering, increased acceleration factors
//  V2.8.2: corrected memory bugs (index / heap overflows), added slot copy feature (in GUI)
//  V2.8.1: corrected bug in deadzone calculation for keyboard actions, improved stable time for strong sip/puff functions
//  V2.8: improved cursor control by using polar coordinates and damping
//  V2.7: improved IR command recording and playback (IR hold repeats codes, optionally append off-sequence)
//  V2.6: updated API for KEY commands (added KT, changed KP) and Mouse Click commands (added toggle clicks)
//  V2.5: added stick rotation options, improved acoustic slot feedback, improved keycode handling,
//        removed Teensy2.0++ support, new AT commands: clear IR memory, route HID to BT/USB/both
//  V2.4: added support for acceleration, maximum speed and command macros
//	V2.3: added support for internal Bluetooth Addon
//  V2.2: added new EEPROM handling and IR-Command support
//  V2.0: extended AT command set, TeensyLC support, external EEPROM
//  V1.0: extended AT command set, GUI compatibility


// Optional Debug Output Control

// #define DEBUG_OUTPUT_FULL      // if full debug output is desired
// #define DEBUG_OUTPUT_BASIC     // if basic debug output is desired (for eeprom)

#define DEFAULT_CLICK_TIME      8    // time for mouse click (loop iterations from press to release)

#define WORKINGMEM_SIZE    300        // reserved RAM for working memory (command parser, IR-rec/play)
#define MAX_KEYSTRING_LEN (WORKINGMEM_SIZE-3)   // maximum length for AT command parameters
#define MAX_NAME_LEN  15              // maximum length for a slotname or ir name
#define MAX_KEYSTRINGBUFFER_LEN 500   // maximum length for all string parameters of one slot

// direction identifiers
#define DIR_E   1   // east
#define DIR_NE  2   // north-east
#define DIR_N   3   // north
#define DIR_NW  4   // north-west
#define DIR_W   5   // west
#define DIR_SW  6   // sout-west
#define DIR_S   7   // south
#define DIR_SE  8   // south-east

struct SlotSettings {

  char slotName[MAX_NAME_LEN];   // slotname
  uint16_t keystringBufferLen;   
  
  uint8_t  stickMode;  // alternative(0), mouse(1), joystick (2,3,4) or pad (5,6) mode
  uint8_t  ax;     // acceleration x
  uint8_t  ay;     // acceleration y
  int16_t  dx;     // deadzone x
  int16_t  dy;     // deadzone y
  uint16_t ms;     // maximum speed
  uint16_t ac;     // acceleration time
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff
  uint8_t  ws;     // wheel stepsize
  uint16_t sp;     // threshold strong puff
  uint16_t ss;     // threshold strong sip
  uint8_t  gv;     // gain vertical drift compensation
  uint8_t  rv;     // range vertical drift compensation
  uint8_t  gh;     // gain horizontal drift compensation
  uint8_t  rh;     // range horizontal drift compensation
  int16_t  cx;     // calib x
  int16_t  cy;     // calib y
  uint16_t ro;     // orientation (0,90,180,270)
  uint8_t  bt;     // bt-mode (0,1,2)
};

struct SensorData {
  int x, y;
  int pressure;
  float dz, force, angle;
  uint8_t dir;
  int8_t autoMoveX,autoMoveY;
  int up, down, left, right;
  uint8_t calib_now;
  int16_t  cx, cy;
  int xDriftComp, yDriftComp;
  int xLocalMax, yLocalMax;  
};
  
extern char moduleName[];
extern uint8_t actSlot;
extern uint8_t addonUpgrade;

extern struct SensorData sensorData;
extern struct SlotSettings slotSettings; 
extern const struct SlotSettings defaultSlotSettings;

extern uint8_t workingmem[WORKINGMEM_SIZE];            // working memory  (command parser, IR-rec/play)
extern char keystringBuffer[MAX_KEYSTRINGBUFFER_LEN];  // storage for all button string parameters of a slot

//set the correct strcpy/strcmp functions for TeensyLC / ARM)
#define strcpy_FM   strcpy
#define strcmp_FM   strcmp
typedef char* uint_farptr_t_FM;

 #endif
