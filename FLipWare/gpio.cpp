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
int8_t  led_map[NUMBER_OF_LEDS] = {5, 16, 17};                //  maps leds pins (only v2!)

uint8_t blinkCount = 0;
uint8_t blinkTime = 0;
uint8_t blinkStartTime = 0;

Adafruit_NeoPixel pixels(1, 5, NEO_GRB + NEO_KHZ800);

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t actSlot;

void initGPIO() 
{
  if(getPCBVersion() == 2)
  {
	  for (int i = 0; i < NUMBER_OF_LEDS; i++) // initialize physical buttons and bouncers
    pinMode (led_map[i], OUTPUT);   // configure the pins for output mode (LEDs)
  }
  if(getPCBVersion() == 3)
  {
    pixels.begin();
    pixels.setBrightness(127);
  }
  
	for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++) // initialize physical buttons and bouncers
	pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors
}

void initBlink(uint8_t  count, uint8_t startTime)
{
  blinkCount = count;
  blinkStartTime = startTime;
}

void updateLeds()
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  
  if(getPCBVersion() == 3)
  {
    if (blinkCount == 0) {
      if ((actSlot + 1) & 1) r = 255;
      if ((actSlot + 1) & 2) g = 255;
      if ((actSlot + 1) & 4) b = 255;
    }
    else {
      if (blinkTime == 0)
      {
        blinkTime = blinkStartTime;
        blinkCount--;
      } else blinkTime--;
      
      if(blinkCount % 2)
      {
        r = g = b = ((blinkStartTime - blinkTime) / blinkStartTime) * 255;
      } else {
        r = g = b = 255 - ((blinkStartTime - blinkTime) / blinkStartTime) * 255;
      }
    }
    pixels.setPixelColor(0,r,g,b);
    pixels.show();
  }
  
  if(getPCBVersion() == 2)
  {
    if(blinkCount == 0) {
      if((actSlot + 1) & 1) digitalWrite (led_map[0], LOW); else digitalWrite (led_map[0], HIGH);
      if((actSlot + 1) & 2) digitalWrite (led_map[1], LOW); else digitalWrite (led_map[1], HIGH);
      if((actSlot + 1) & 4) digitalWrite (led_map[2], LOW); else digitalWrite (led_map[2], HIGH);
    }
    else {
      if(blinkTime == 0)
      {
        blinkTime = blinkStartTime;
        blinkCount--;
        if(blinkCount % 2) {
          digitalWrite (led_map[0], LOW); digitalWrite (led_map[1], LOW); digitalWrite (led_map[2], LOW);
        } else {
          digitalWrite (led_map[0], HIGH); digitalWrite (led_map[1], HIGH); digitalWrite (led_map[2], HIGH);
        }
      } else blinkTime--;
    }
  }
}

void setLeds(uint8_t leds)
{
  if(getPCBVersion() == 2)
  {
    if(leds & (1<<0)) digitalWrite(led_map[0], LOW); else digitalWrite(led_map[0], HIGH);
    if(leds & (1<<1)) digitalWrite(led_map[1], LOW); else digitalWrite(led_map[1], HIGH);
    if(leds & (1<<2)) digitalWrite(led_map[2], LOW); else digitalWrite(led_map[2], HIGH);
  }
  if(getPCBVersion() == 3)
  {
    uint8_t r,g,b = 0;
    if(leds & (1<<0)) r = 255;
    if(leds & (1<<1)) g = 255;
    if(leds & (1<<2)) b = 255;
    pixels.setPixelColor(0,r,g,b);
    pixels.show();
  }
}
