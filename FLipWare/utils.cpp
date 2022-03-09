/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: utils.cpp - utility functions 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "utils.h"


typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;

# define GET_FLOAT_WORD(i,d)                                        \
  do {                                                                \
    ieee_float_shape_type gf_u;                                        \
    gf_u.value = (d);                                                \
    (i) = gf_u.word;                                                \
  } while (0)

# define SET_FLOAT_WORD(d,i)                                        \
  do {                                                                \
    ieee_float_shape_type sf_u;                                        \
    sf_u.word = (i);                                                \
    (d) = sf_u.value;                                                \
  } while (0)

float __ieee754_sqrtf(float x)
{
  float z;
  __uint32_t r, hx;
  __int32_t ix, s, q, m, t, i;

  GET_FLOAT_WORD(ix, x);

  hx = ix & 0x7fffffff;

  if (x == 0) return 0; /* sqrt(+-0) = +-0 */
  if (x < 0)  return (x - x) / (x - x); /* sqrt(-ve) = sNaN */

  /* normalize x */
  m = (ix >> 23);
  m -= 127; /* unbias exponent */
  ix = (ix & 0x007fffffL) | 0x00800000L;
  if (m & 1) /* odd m, double x to make it even */
    ix += ix;
  m >>= 1;  /* m = [m/2] */

  /* generate sqrt(x) bit by bit */
  ix += ix;
  q = s = 0;    /* q = sqrt(x) */
  r = 0x01000000L;    /* r = moving bit from right to left */

  while (r != 0) {
    t = s + r;
    if (t <= ix) {
      s    = t + r;
      ix  -= t;
      q   += r;
    }
    ix += ix;
    r >>= 1;
  }

  float one = 1.0, tiny = 1.0e-30;
  /* use floating add to find out rounding direction */
  if (ix != 0) {
    z = one - tiny; /* trigger inexact flag */
    if (z >= one) {
      z = one + tiny;
      if (z > one)
        q += 2;
      else
        q += (q & 1);
    }
  }
  ix = (q >> 1) + 0x3f000000L;
  ix += (m << 23);
  SET_FLOAT_WORD(z, ix);
  return z;
}


//source of code for free ram:
//https://github.com/mpflaga/Arduino-MemoryFree/blob/master/MemoryFree.cpp
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeRam()
{
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

uint8_t getPCBVersion(void)
{
	static uint8_t version = 0;
  uint8_t result;
	switch(version)
	{
		case 0: //not tested
      Wire.beginTransmission(EEPROM_I2C_ADDR_v2);
      result = Wire.endTransmission();
      if(result == 0) //found v2
      {
          version = 2;
          return version;
      }
      
      Wire.beginTransmission(EEPROM_I2C_ADDR_v3);
      result = Wire.endTransmission();
      if(result == 0) //found v3
      {
          version = 3;
          return version;
      }
      
      //oh no, no EEPROM?!?
      version = -1;
      return 0;
      
			break;
    case 2: //PCB v2
    case 3: //PCB v3
      return version;
      break;
    default:
      return 0;
      break;
  }
}
