/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: utils.h - utility functions 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _UTILS_H_
#define _UTILS_H_

float __ieee754_sqrtf(float x);
int  freeRam ();

void initBlink(uint8_t count, uint8_t startTime);
void makeTone(uint8_t kind, uint8_t param);
void BlinkLed();


#endif
