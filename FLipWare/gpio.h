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

#define NUMBER_OF_PHYSICAL_BUTTONS 3  // number of physical switches
#define NUMBER_OF_LEDS     3          // number of connected leds

extern int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS];

void initGPIO();
void UpdateLeds();

#endif
