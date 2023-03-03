/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

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
#define DRDY_PIN        21

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
 * @brief Fixed divider for the NAU raw values
 */
#define NAU_DIVIDER 120

/**
 * @brief Sensor Watchdog value (approx. milliseconds until device resets)
 */
#define SENSOR_WATCHDOG_TIMEOUT 3000



/**
 * @brief Sensorboard IDs for different signal processing parameters
 */
#define SENSORBOARD_SENSITIVITY_HIGH     0
#define SENSORBOARD_SENSITIVITY_MEDIUM   1
#define SENSORBOARD_SENSITIVITY_LOW      2
#define SENSORBOARD_SENSITIVITY_VERY_LOW 3
#define SENSORBOARD_NODMS_HIGH           4
#define SENSORBOARD_NODMS_MEDIUM         5
#define SENSORBOARD_NODMS_LOW            6
#define SENSORBOARD_REPORT_X             10   // enable / disable signal processing values reporting for X axis
#define SENSORBOARD_REPORT_Y             11   // enable / disable signal processing values reporting for Y axis

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

/**
   @name setSensorBoard
   @brief activates a certain parameters profile for signal processing, depending on the selected senosorboard ID
   @param sensorBoardID: the ID of the sensorboard signal processing profile  (e.g. SENSOR_BOARD_STRAINGAUGE) 
   @return none
*/
void setSensorBoard(int sensorBoardID);

/**
   @name checkSensorWatchdog
   @brief checks if an integer value which should be periodically reset when I2C-sensordata is ready exceeds a certain value
   @return true: value within normal range  false: value exceeded -> action must be taken
*/
uint8_t checkSensorWatchdog();


/**
   @name getSensorValues
   @brief called if new data from NAU7802 is available. 
          Reads NAU data, changes NAU channel and reads MPRLS data, updates global variables.
          Expected sampling rate ca. 64 Hz
   @return none
*/
void getSensorValues();

#endif /* _SENSORS_H_ */
