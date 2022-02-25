/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: gpio.cpp - functions for leds and buttons

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "gpio.h"

int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS] = {0, 2, 1};    //  maps physical button pins to button index 0,1,2
int8_t  led_map[NUMBER_OF_LEDS] = {5, 16, 17};                //  maps leds pins

uint8_t blinkCount = 0;
uint8_t blinkTime = 0;
uint8_t blinkStartTime = 0;

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t actSlot;

void initGPIO() 
{
  for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++) // initialize physical buttons and bouncers
  pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

  for (int i = 0; i < NUMBER_OF_LEDS; i++) // initialize physical buttons and bouncers
  pinMode (led_map[i], OUTPUT);   // configure the pins for input mode with pullup resistors
}

void initBlink(uint8_t  count, uint8_t startTime)
{
  blinkCount = count;
  blinkStartTime = startTime;
}

void updateLeds()
{
  if (blinkCount == 0) {
    if ((actSlot + 1) & 1) digitalWrite (led_map[0], LOW); else digitalWrite (led_map[0], HIGH);
    if ((actSlot + 1) & 2) digitalWrite (led_map[1], LOW); else digitalWrite (led_map[1], HIGH);
    if ((actSlot + 1) & 4) digitalWrite (led_map[2], LOW); else digitalWrite (led_map[2], HIGH);
  }
  else {
    if (blinkTime == 0)
    {
      blinkTime = blinkStartTime;
      blinkCount--;
      if (blinkCount % 2) {
        digitalWrite (led_map[0], LOW); digitalWrite (led_map[1], LOW); digitalWrite (led_map[2], LOW);
      }
      else {
        digitalWrite (led_map[0], HIGH); digitalWrite (led_map[1], HIGH); digitalWrite (led_map[2], HIGH);
      }
    } else blinkTime--;
  }
}
