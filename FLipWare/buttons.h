  
/* 
      FLipWare - AsTeRICS Foundation 2017
     For more info please visit: http://www.asterics-academy.net

     Module: buttons.h - header file for the button handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License: 
   http://www.gnu.org/licenses/gpl-3.0.en.html

 */
 

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

// Constants and Macro definitions
#define MAX_KEYSTRINGBUFFER_LEN 900  // maximum length for all keystring parameters

#define NUMBER_OF_BUTTONS  19         // number of physical + virtual switches
#define NUMBER_OF_PHYSICAL_BUTTONS 3  // number of physical switches
#define NUMBER_OF_LEDS     3          // number of connected leds

#define DEFAULT_DEBOUNCING_TIME 5   // debouncing interval for button-press / release

// define index numbers of the virtual buttons (not pin numbers in this case !)
// (buttons 0-2 are the physical switches on the device)

#define UP_BUTTON                3
#define DOWN_BUTTON              4
#define LEFT_BUTTON              5
#define RIGHT_BUTTON             6

#define SIP_BUTTON               7
#define STRONGSIP_BUTTON         8
#define PUFF_BUTTON              9
#define STRONGPUFF_BUTTON       10

#define STRONGSIP_UP_BUTTON     11
#define STRONGSIP_DOWN_BUTTON   12
#define STRONGSIP_LEFT_BUTTON   13
#define STRONGSIP_RIGHT_BUTTON  14

#define STRONGPUFF_UP_BUTTON    15
#define STRONGPUFF_DOWN_BUTTON  16
#define STRONGPUFF_LEFT_BUTTON  17
#define STRONGPUFF_RIGHT_BUTTON 18


// data structures for the buttons

struct slotButtonSettings {                     // holds settings for a button function
  uint16_t mode;
  uint16_t value;
};

struct buttonDebouncerType {              // holds working data for button debouncing and longpress detection 
  uint8_t bounceCount;
  uint8_t bounceState;
  uint8_t stableState;
  uint8_t longPressed;
  uint32_t timestamp;
} ; 

extern struct slotButtonSettings buttons[NUMBER_OF_BUTTONS];
extern char* keystringButtons[NUMBER_OF_BUTTONS];
extern uint16_t keystringBufferLen;
// extern char keystringBuffer[MAX_KEYSTRINGBUFFER_LEN];
// extern struct buttonDebouncerType buttonDebouncers[NUMBER_OF_BUTTONS];

// function declarations 
uint16_t storeKeystringButton(uint8_t buttonIndex, char * text);
uint16_t deleteKeystringButton(uint8_t buttonIndex);
void handlePress (int buttonIndex);      // a button was pressed
void handleRelease (int buttonIndex);    // a button was released
uint8_t handleButton(int i, uint8_t state);    // button debouncing and longpress detection  
uint8_t inHoldMode (int i);
void initDebouncers();

#endif
