/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: gpio.h - functions for leds and buttons

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _SENSORS_H_
#define _SENSORS_H_

//we need the SensorData structure definition
#include "FlipWare.h"
//MPRLS pressure sensor is I2C
#include "Wire.h"

/**
 * @brief I2C address of the MPRLS pressure sensor 
 */
#define MPRLS_ADDR 0x18

/**
 * @brief Fixed divider for the MPRLS raw value.
 */
#define MPRLS_DIVIDER 15000

/**
   @name initSensors
   @brief initializes the sensors (pressure & force)
   @return none
*/
void initSensors();

/**
   @name readPressure
   @brief read current pressure sensor (either MPXV7007GP or MPRS)
   @note For the MPRLS sensor, it returns the previous measurement & triggers a new one!
   @return none
*/
void readPressure(struct SensorData *data);

/**
   @name readForce
   @brief read current force sensors (might be FSR or RES-DMS)
   @return none
*/
void readForce(struct SensorData *data);

#endif /* _SENSORS_H_ */