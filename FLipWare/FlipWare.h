
/* 
     FLipWare - built upon Assistive Button Interface (FABI) Version 2.0  - AsTeRICS Academy 2015 - http://www.asterics-academy.net
      allows control of HID functions via FLipmouse module and/or AT-commands  
   

   requirements:  Teensy 2.0++ or TeensyLC with external EEPROM
                  Teensyduino AddOn for Arduino IDE
                  USB type set to USB composite device (Serial + Keyboard + Mouse + Joystick)
        sensors:  3 momentary switches connected to GPIO pins / 4 force sensors + 1 pressure sensor connected to ADC pins 
       
   
   for a list of supported AT commands, see commands.h / commands.cpp
   
 */



#ifndef _FLIPWARE_H_
#define _FLIPWARE_H_

#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include "commands.h"


#define VERSION_STRING "Flipmouse v2.0"

//#define TEENSY               //  if Teensy2.0++ controller is used (FLipmouse V1)
#define TEENSY_LC            //  if Teensy LC controller is used (FLipmouse V2)

#ifndef TEENSY
#ifndef TEENSY_LC
  #error "NO CHIP SELECTED, EITHER TEENSY OR TEENSY LC ARE SUPPORTED"
#endif
#endif

#define MAX_SLOTS          5          // maximum number of EEPROM memory slots
#define NUMBER_OF_BUTTONS  12         // number of connected or virtual switches
#define NUMBER_OF_PHYSICAL_BUTTONS 3  // number of connected switches
#define NUMBER_OF_LEDS     3          // number of connected leds

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

#define TONE_ENTERSPECIAL 0
#define TONE_CALIB        1
#define TONE_CHANGESLOT   2
#define TONE_HOLD         3
#define TONE_EXITSPECIAL  4

#define DEFAULT_CLICK_TIME      8    // time for mouse click (loop iterations from press to release)
#define DOUBLECLICK_MULTIPLIER  5    // CLICK_TIME factor for double clicks

#define DEBUG_NOOUTPUT 0
#define DEBUG_FULLOUTPUT 0


struct settingsType {
  char slotname[MAX_SLOTNAME_LEN];     // EEPROM data is stored only until ths string's end
  uint8_t  mouseOn;// mouse or alternative functions 
  uint8_t  ax;     // acceleration x
  uint8_t  ay;     // acceleration y
  int16_t  dx;     // deadzone x
  int16_t  dy;     // deadzone y
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff 
  uint8_t  ws;     // wheel stepsize  
  uint16_t sm;     // threshold special mode 
  uint16_t hm;     // threshold hold mode 
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

struct buttonType {                      // holds settings for a button function 
  int mode;
  int value;
  char keystring[MAX_KEYSTRING_LEN];
};

struct buttonDebouncerType {              // holds working data for button debouncing and longpress detection 
  uint8_t bounceCount;
  uint8_t bounceState;
  uint8_t stableState;
  uint8_t longPressed;
  uint32_t timestamp;
} ; 

extern uint8_t DebugOutput;
extern uint8_t actSlot;
extern uint8_t reportSlotParameters;
extern uint8_t reportRawValues;
extern struct settingsType settings;
extern int EmptySlotAddress;
extern struct buttonType buttons[NUMBER_OF_BUTTONS];
extern struct buttonDebouncerType buttonDebouncers[NUMBER_OF_BUTTONS];
extern const struct atCommandType atCommands[];
extern int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS];

extern uint16_t calib_now;
extern int16_t  cx;
extern int16_t  cy;
extern uint8_t leftMouseButton;
extern uint8_t middleMouseButton;
extern uint8_t rightMouseButton;
extern uint8_t leftClickRunning;
extern uint8_t rightClickRunning;
extern uint8_t middleClickRunning;
extern uint8_t doubleClickRunning;
extern char * writeKeystring;
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

void setKeyValues(char* text); // presses individual keys
void releaseKeys(char* text);  // releases individual keys
void release_all();            // releases all previously pressed keys and buttons

void record_IR_command(char * name);
void play_IR_command(char * name);
void list_IR_commands();
void delete_IR_command(char * name);

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
