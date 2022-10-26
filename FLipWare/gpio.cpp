/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: gpio.cpp - functions for leds and buttons

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "FlipWare.h"
#include "gpio.h"

int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS] = {17, 20, 21};      //  NOTE: changed for RP2040!

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
  static uint32_t oldValue=0;
  uint8_t colCode=actSlot+1;
  static uint16_t fadeCount=0;

	if (blinkCount == 0) {         // normal mode / not blinking

    if (slotSettings.sc==0) {    // no user defined slotcolor -> use fixed slot colors
      if (colCode & 1) g = 255;
      if (colCode & 2) b = 255;
      if (colCode & 4) r = 255;
    } else {   // user defined slotcolor available
      r=(slotSettings.sc>>16) & 0xff;
      g=(slotSettings.sc>>8) & 0xff;
      b= slotSettings.sc & 0xff;
    }

    // perform fading LED animation in case BT slot active but no device paired
    if ((slotSettings.bt&2) && (!isBluetoothConnected()))    {
      fadeCount++; if (fadeCount>255) fadeCount=0;
      r = (r*fadeCount)>>8;
      g = (g*fadeCount)>>8;
      b = (b*fadeCount)>>8;
    }
    
	} else {   // blinking mode (e.g. to indicate calibration)
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
  
  uint32_t ledValue=(uint32_t)r + ((uint32_t)g<<8) + ((uint32_t)b<<16);
  if (ledValue!=oldValue) {
    oldValue=ledValue;
    pixels.setPixelColor(0,r,g,b);
    pixels.show();
  }  
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
