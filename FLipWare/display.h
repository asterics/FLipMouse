/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: display.h - implementation of the Oled display functions 

     Note: Oled display connects to I2C-1 (Wire) for FlipMouse of I2C-2 (Wire1) for FlipPad
     The utilized library is https://github.com/greiman/SSD1306Ascii

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

extern uint8_t displayInstalled;

uint8_t displayInit (uint8_t useWire1);
void displayClear(void);
void displayUpdate(void);
void displayMessage(char * msg);

#endif
