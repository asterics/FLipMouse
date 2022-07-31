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

//int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS] = {D2, D8, D9};    //  maps physical button pins to button index 0,1,2  // TBD: changed for RP2040!
int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS] = {19, 20, 21};      //  NOTE: changed for RP2040!

uint8_t blinkCount = 0;
uint8_t blinkTime = 0;
uint8_t blinkStartTime = 0;

Adafruit_NeoPixel pixels(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t actSlot;

void initGPIO() 
{
  for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++) // initialize physical buttons and bouncers
  pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

  pixels.begin();
  pixels.setBrightness(127);
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

	if (blinkCount == 0) {
	  if ((actSlot + 1) & 1) r = 255;
	  if ((actSlot + 1) & 2) g = 255;
	  if ((actSlot + 1) & 4) b = 255;
	} else {
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

void setLeds(uint8_t leds)
{
  uint8_t r,g,b = 0;
  if(leds & (1<<0)) r = 255;
  if(leds & (1<<1)) g = 255;
  if(leds & (1<<2)) b = 255;
  pixels.setPixelColor(0,r,g,b);
  pixels.show();
}
