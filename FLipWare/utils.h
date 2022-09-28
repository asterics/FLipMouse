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
