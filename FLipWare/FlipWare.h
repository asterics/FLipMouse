
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
#include <string.h>
#include <stdint.h>
#include "commands.h"
#include "eeprom.h"
#include "buttons.h"
#include "modes.h"
#include "bluetooth.h"
#include "hid_hal.h"

#define VERSION_STRING "Flipmouse v2.8"

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

#define WORKINGMEM_SIZE    400        // reserved RAM for working memory (command parser, IR-rec/play)
#define MAX_SLOTS          7          // maximum number of EEPROM memory slots
#define MAX_KEYSTRING_LEN (WORKINGMEM_SIZE-3)   // maximum length for AT command parameters
#define MAX_NAME_LEN  15              // maximum length for a slotname or ir name
#define MAX_KEYSTRINGBUFFER_LEN 700   // maximum length for all string parameters of one slot

#define PARTYPE_NONE  0
#define PARTYPE_UINT  1
#define PARTYPE_INT   2
#define PARTYPE_STRING  3

#define REPORT_NONE  0  
#define REPORT_ONE_SLOT  1
#define REPORT_ALL_SLOTS 2

#define TONE_CALIB            1
#define TONE_CHANGESLOT       2
#define TONE_ENTER_STRONGSIP  3
#define TONE_EXIT_STRONGSIP   4
#define TONE_ENTER_STRONGPUFF 5
#define TONE_EXIT_STRONGPUFF  6
#define TONE_INDICATE_SIP     7
#define TONE_INDICATE_PUFF    8
#define TONE_IR			          9
#define TONE_BT_PAIRING      10
#define TONE_IR_REC          11

#define BUTTON1_PRESS_TIME_FOR_PAIRING 800

#define DEFAULT_CLICK_TIME      8    // time for mouse click (loop iterations from press to release)
// #define DOUBLECLICK_MULTIPLIER  5    // CLICK_TIME factor for double clicks

#define DEBUG_NOOUTPUT 0
#define DEBUG_FULLOUTPUT 1

extern uint8_t workingmem[WORKINGMEM_SIZE];    // working memory  (command parser, IR-rec/play)

struct slotGeneralSettings {
  uint8_t  stickMode;  // alternative (0) mouse (1) or joystick (2,3,4) mode
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
  uint8_t  gu;     // gain up 
  uint8_t  gd;     // gain down 
  uint8_t  gl;     // gain left 
  uint8_t  gr;     // gain right 
  int16_t  cx;     // calib x
  int16_t  cy;     // calib y
  uint16_t ro;     // orientation (0,90,180,270)
  uint8_t  bt;     // bt-mode (0,1,2)
  char     ii[MAX_NAME_LEN]; // infrared idle sequence name
};



struct atCommandType {                      // holds settings for a button function 
  char atCmd[3];
  uint8_t  partype;
};

extern uint8_t DebugOutput;
extern uint8_t actSlot;
extern uint8_t reportSlotParameters;
extern uint8_t reportRawValues;
extern struct slotGeneralSettings settings;
extern char slotName[MAX_NAME_LEN];
extern int EmptySlotAddress;

extern const struct atCommandType atCommands[];
extern int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS];

extern uint16_t calib_now;
extern int16_t  cx;
extern int16_t  cy;
extern int8_t moveX;       
extern int8_t moveY;
extern double force;
extern double angle;

void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement);
void saveToEEPROM(char * slotname);
void readFromEEPROM(char * slotname);
void deleteSlots();
void listSlots();
void initButtons();
void printCurrentSlot();
void initBlink(uint8_t count, uint8_t startTime);
void makeTone(uint8_t kind, uint8_t param);


void BlinkLed();
int  freeRam ();
void parseByte (int newByte);

void pressKeys(char* text); // presses individual keys
void holdKeys(char* text);  // holds individual keys
void toggleKeys(char* text);  // toggles individual keys
void releaseKeys(char* text);  // releases individual keys
void release_all_keys();       // releases all previously pressed keys
void release_all();            // releases all previously pressed keys and buttons

void record_IR_command(char * name);
void play_IR_command(char * name);
void hold_IR_command(char * name);
void stop_IR_command();
void list_IR_commands();
void delete_IR_command(char * name);
void set_IR_timeout(uint16_t ms);
void wipe_IR_commands();

//set the correct strcpy/strcmp functions for TeensyLC / ARM)
#define strcpy_FM   strcpy
#define strcmp_FM   strcmp
typedef char* uint_farptr_t_FM;

#endif
