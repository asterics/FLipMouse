
/* 
     FLipWare - AsTeRICS Academy 2016
     For more info please visit: http://www.asterics-academy.net

     Module: FlipWare.h  - main header file
     
        This firmware allows control of HID functions via FLipmouse module and/or AT-commands  
        For a description of the supported commands see: commands.h

        requirements:  Teensy 2.0++ or TeensyLC with external EEPROM
                       Teensyduino AddOn for Arduino IDE
                       USB-type set to USB composite device (Serial + Keyboard + Mouse + Joystick)
        sensors:  3 momentary switches connected to GPIO pins / 4 force sensors
                  1 pressure sensor connected to ADC pins 
          
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

#define VERSION_STRING "Flipmouse v2.4"   

//  V2.4: added support for acceleration and maximum speed
//	V2.3: added support for internal Bluetooth Addon
//  V2.2: added new EEPROM handling and IR-Command support
//  V2.0: extended AT command set, TeensyLC support, external EEPROM
//  V1.0: extended AT command set, GUI compatibility


//#define TEENSY               //  if Teensy2.0++ controller is used (FLipmouse V1)
#define TEENSY_LC            //  if Teensy LC controller is used (FLipmouse V2)

#ifndef TEENSY
#ifndef TEENSY_LC
  #error "NO CHIP SELECTED, EITHER TEENSY OR TEENSY LC ARE SUPPORTED"
#endif
#endif 

#define MAX_SLOTS          7          // maximum number of EEPROM memory slots

#define MAX_KEYSTRING_LEN 30          // maximum length for key identifiers / keyboard text
#define MAX_SLOTNAME_LEN  15          // maximum length for a slotname
#define MAX_CMDLEN MAX_KEYSTRING_LEN+3

#define PARTYPE_NONE  0
#define PARTYPE_UINT  1
#define PARTYPE_INT   2
#define PARTYPE_STRING  3

#define REPORT_NONE  0  
#define REPORT_ONE_SLOT  1
#define REPORT_ALL_SLOTS 2

#define TONE_ENTER_STRONGPUFF 0
#define TONE_CALIB            1
#define TONE_CHANGESLOT       2
#define TONE_STRONGSIP        3
#define TONE_EXIT_STRONGPUFF  4
#define TONE_IR			          5

#define DEFAULT_CLICK_TIME      8    // time for mouse click (loop iterations from press to release)
#define DOUBLECLICK_MULTIPLIER  5    // CLICK_TIME factor for double clicks

#define DEBUG_NOOUTPUT 0
#define DEBUG_FULLOUTPUT 1



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
extern char slotName[MAX_SLOTNAME_LEN];
extern char IRName[MAX_SLOTNAME_LEN];
extern int EmptySlotAddress;
extern struct slotButtonSettings buttons[NUMBER_OF_BUTTONS];
extern char keystringButton[NUMBER_OF_BUTTONS][MAX_KEYSTRING_LEN];
extern struct buttonDebouncerType buttonDebouncers[NUMBER_OF_BUTTONS];
extern const struct atCommandType atCommands[];
extern int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS];

extern uint16_t calib_now;
extern int16_t  cx;
extern int16_t  cy;
extern int8_t moveX;       
extern int8_t moveY;

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
int freeRam ();
void parseByte (int newByte);

void pressKeys(char* text); // presses individual keys
void holdKeys(char* text); // presses individual keys
void releaseKeys(char* text);  // releases individual keys
void release_all_keys();       // releases all previously pressed keys
void release_all();            // releases all previously pressed keys and buttons

void record_IR_command(char * name);
void play_IR_command(char * name);
void list_IR_commands();
void delete_IR_command(char * name);
void set_IR_timeout(uint16_t ms);

//set the correct strcpy/strcmp functions (either for AVR or ARM)
#ifdef TEENSY
    #define strcpy_FM   strcpy_PF
    #define strcmp_FM   strcmp_PF
    typedef uint_farptr_t uint_farptr_t_FM;
#endif

#ifdef TEENSY_LC
    #define strcpy_FM   strcpy
    #define strcmp_FM   strcmp
    typedef char* uint_farptr_t_FM;
#endif

#endif
