
#ifndef _FABI_H_
#define _FABI_H_

#include <Arduino.h>
#include <string.h>
#include <stdint.h>

#define VERSION_STRING "Flipmouse v2.0"

#define TEENSY               //  if Teensy2.0++ controller is used (FLipmouse V1)
//#define TEENSY_LC              //  if Teensy LC controller is used (FLipmouse V2)


#ifndef TEENSY
#ifndef TEENSY_LC
  #error "NO CHIP SELECTED, EITHER TEENSY OR TEENSY LC ARE SUPPORTED"
#endif
#endif

#define NUMBER_OF_BUTTONS 11          // number of connected or virtual switches
#define NUMBER_OF_PHYSICAL_BUTTONS 3  // number of connected switches
#define NUMBER_OF_LEDS      3         // number of connected leds
#define MAX_SLOTS          5          // maximum number of EEPROM memory slots

#define MAX_KEYSTRING_LEN 30          // maximum lenght for key identifiers / keyboard text
#define MAX_SLOTNAME_LEN  15          // maximum lenght for a slotname
#define MAX_CMDLEN MAX_KEYSTRING_LEN+3

#define DEBUG_NOOUTPUT 0
#define DEBUG_FULLOUTPUT 1

#define TOKEN_SEPERATOR "-,-"

#define TONE_ENTERSPECIAL 0
#define TONE_CALIB        1
#define TONE_CHANGESLOT   2
#define TONE_HOLD         3
#define TONE_EXITSPECIAL  4


// command identifiers

#define CMD_PRINT_ID                 0
#define CMD_BUTTON_MODE              1
#define CMD_MOUSE_CLICK_LEFT         2
#define CMD_MOUSE_CLICK_RIGHT        3
#define CMD_MOUSE_CLICK_MIDDLE       4
#define CMD_MOUSE_CLICK_DOUBLE       5
#define CMD_MOUSE_PRESS_LEFT         6
#define CMD_MOUSE_PRESS_RIGHT        7
#define CMD_MOUSE_PRESS_MIDDLE       8
#define CMD_MOUSE_RELEASE_LEFT       9
#define CMD_MOUSE_RELEASE_RIGHT     10
#define CMD_MOUSE_RELEASE_MIDDLE    11
#define CMD_MOUSE_WHEEL_UP          12
#define CMD_MOUSE_WHEEL_DOWN        13
#define CMD_MOUSE_WHEEL_STEP        14
#define CMD_MOUSE_MOVEX             15
#define CMD_MOUSE_MOVEY             16
#define CMD_KEY_WRITE               17
#define CMD_KEY_PRESS               18
#define CMD_KEY_RELEASE             19
#define CMD_RELEASE_ALL             20
#define CMD_SAVE_SLOT               21
#define CMD_LOAD_SLOT               22
#define CMD_LIST_SLOTS              23
#define CMD_NEXT_SLOT               24
#define CMD_DELETE_SLOTS            25
#define CMD_IDLE                    26 
#define CMD_MM                      27
#define CMD_LA                      28
#define CMD_TOGGLE_ALTERNATIVE      29
#define CMD_SR                      30
#define CMD_ER                      31
#define CMD_CALIBRATE               32
#define CMD_AX                      33
#define CMD_AY                      34
#define CMD_DX                      35
#define CMD_DY                      36
#define CMD_TS                      37
#define CMD_TP                      38
#define CMD_TT                      39
#define CMD_GU                      40
#define CMD_GD                      41
#define CMD_GL                      42
#define CMD_GR                      43

#define NUM_COMMANDS                44 


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
  uint16_t tt;     // threshold special/hold 
  uint8_t  gu;     // gain up 
  uint8_t  gd;     // gain down 
  uint8_t  gl;     // gain left 
  uint8_t  gr;     // gain right 
  int16_t  cx;     // calib x
  int16_t  cy;     // calib y
};


#define PARTYPE_NONE 0
#define PARTYPE_UINT  1
#define PARTYPE_INT   2
#define PARTYPE_STRING  3

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

void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement);
void saveToEEPROM(char * slotname);
void readFromEEPROM(char * slotname);
void deleteSlots();
void listSlots();
void printCurrentSlot();
void makeTone(uint8_t kind, uint8_t param);


void BlinkLed();
int freeRam ();
void parseByte (int newByte);

void setKeyValues(char* text); // presses individual keys
void releaseKeys(char* text);  // releases individual keys
void release_all();            // releases all previously pressed keys and buttons


#endif
