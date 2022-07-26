/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: gpio.h - functions for leds and buttons

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <Adafruit_NeoPixel.h>

/**
   constant definitions
*/
#define NUMBER_OF_PHYSICAL_BUTTONS 3  // number of physical switches
#define NUMBER_OF_LEDS     3          // number of connected leds

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS];  // maps the button number to physical pin

/**
   @name initGPIO
   @brief initializes data direction of gpio pins
   @return none
*/
void initGPIO();

/**
   @name initBlink
   @brief initializes an LED blinking sequence
   @return none
*/
void initBlink(uint8_t count, uint8_t startTime);


/**
   @name updateLeds
   @brief update leds according to current slot number / blinking actions
   @return none
*/
void updateLeds();

/**
   @name setLeds
   @brief set the LEDs (or the Neopixel directly)
   @return none
   @param LEDs (bits 0-3; Neopixel equals bit 0 R; bit 1 G; bit 2 B)
   @note Only used in CIM mode.
*/
void setLeds(uint8_t leds);

#endif
