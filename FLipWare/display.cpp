/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: display.cpp - implementation of the Oled display functions 

     Note: This code is only relevant for the FlipPad device (the FlipMouse has no Oled display)
     The utilized library is https://github.com/greiman/SSD1306Ascii

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "FlipWare.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define SCREEN_ADDRESS 0x3C
SSD1306AsciiWire oled(Wire1);


void initDisplay (void) {
  oled.begin(&Adafruit128x32, SCREEN_ADDRESS);
  oled.setFont(Adafruit5x7);
  //oled.setFont(Arial_bold_14);
  oled.set2X();
  oled.print(" FlipPad ");
}

void updateDisplayMessage(char * msg) {
    oled.clear();

    oled.set2X();
    oled.setCursor(5,1);
    oled.print(msg);  

    oled.set1X();
    oled.setCursor(100,0);
    switch (settings.stickMode) {
      case 0:
      case 6: oled.print("Alt"); break;
      case 1: oled.print("Stk"); break;
      case 2:
      case 3:
      case 4: oled.print("Joy"); break;
      case 5: oled.print("Pad"); break;
    }

    oled.setCursor(100,3);
    switch (settings.bt) {
      case 1: oled.print("USB"); break;
      case 2: oled.print("BT"); break;
      case 3: oled.print("both"); break;
    }

}
