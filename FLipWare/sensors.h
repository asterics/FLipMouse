/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: sensors.cpp - functions to read pressure & force sensors

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "FlipWare.h"        // we need the SensorData and SlotSettings structure definitions
#include "Wire.h"            // MPRLS pressure sensor and NAU7802 sensor use I2C
#include <LoadcellSensor.h>  // for signal conditioning
#include <Adafruit_NAU7802.h>  //NAU7802 library (Benjamin Aigner's fork with channel change feature)

/** Data ready pin of NAU chip */
#define DRDY_PIN        28

/** Analog sensor input if a analog pressure sensor is used */
#define PRESSURE_SENSOR_PIN A3

/**
 * @brief I2C address of the MPRLS pressure sensor 
 */
#define MPRLS_ADDR 0x18

/**
 * @brief Fixed divider for the MPRLS raw value.
 */
#define MPRLS_DIVIDER 50

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
void readPressure(struct I2CSensorValues *data);

/**
   @name readForce
   @brief read current force sensors (might be FSR or RES-DMS)
   @return none
*/
void readForce(struct I2CSensorValues *data);


/**
   @name calculateDirection
   @brief calculates angular direction and force for current x/y sensor values
   @return none
*/
void calculateDirection(struct SensorData * sensorData);

/**
   @name applyDeadzone
   @brief applies deadzone algorithm (elliptical or rectangular) to raw x/y sensor data
   @return none
*/
void applyDeadzone(struct SensorData * sensorData, struct SlotSettings * slotSettings);

#endif /* _SENSORS_H_ */
