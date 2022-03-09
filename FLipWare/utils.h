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

//needed for EEPROM address detection
#include <Wire.h>
//needed for the 2 different addresses of the EEPROM
#include "eeprom.h"

/**
 * @name getPCBVersion
 * @brief Used to determine the PCB version.
 * @return 2 for v2; 3 for v3;
 * @note Needs intialized Wire library.
*/
uint8_t getPCBVersion(void);

/**
   @name __ieee754_sqrtf
   @brief customized version of float square root function (saving flash memory)
   @param x a float value
   @return the square root of the value
*/
float __ieee754_sqrtf(float x);

/**
   @name freeRam
   @brief returns the free RAM byte
   @return number of free bytes in RAM
*/
int  freeRam ();


#endif
