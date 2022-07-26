/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: sensors.cpp - functions to read pressure & force sensors

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "sensors.h"

#define MPRLS_READ_TIMEOUT (20)     ///< millis
#define MPRLS_STATUS_POWERED (0x40) ///< Status SPI powered bit
#define MPRLS_STATUS_BUSY (0x20)    ///< Status busy bit
#define MPRLS_STATUS_FAILED (0x04)  ///< Status bit for integrity fail
#define MPRLS_STATUS_MATHSAT (0x01) ///< Status bit for math saturation
#define MPRLS_STATUS_MASK (0b01100101) ///< Sensor status mask: only these bits are set

/**
 * @brief Used pressure sensor type. We can use either the MPXV7007GP
 * sensor connected to an analog pin or the MPRLS sensor board with I2C
 */
typedef enum {MPXV, MPRLS} pressure_type_t;
pressure_type_t sensor_pressure = MPXV;

/**
 * @brief Used force sensor type. We can use the FSR sensors or possibly
 * in a future version the resistor gauge sensors.
 */
typedef enum {RES_I2C, NO_FORCE} force_type_t;
force_type_t sensor_force = NO_FORCE;

void initSensors()
{
  //detect if there is an MPRLS sensor connected to I2C (Wire)
  Wire.beginTransmission(MPRLS_ADDR);
  uint8_t result = Wire.endTransmission();
  //we found the MPRLS sensor, start the initialization
  if(result == 0) sensor_pressure = MPRLS;

  //TODO: initialize NAU7802 I2C gauge ADC.
  #warning "No sensor active!"
  sensor_force = NO_FORCE;
}


void readPressure(struct SensorData *data)
{
  uint8_t buffer[4]  = {0};
  switch(sensor_pressure)
  {
    case MPRLS:
      //request status byte
      Wire.requestFrom(MPRLS_ADDR,4);
      for(uint8_t i = 0; i<4; i++) buffer[i] = Wire.read();
      //any errors? set pressure value to 512, convert otherwise...
      if(buffer[0] & MPRLS_STATUS_BUSY)
      {
        //sensor is busy, cannot read data
        return;
      }
      if((buffer[0] & MPRLS_STATUS_MATHSAT) || (buffer[0] & MPRLS_STATUS_FAILED))
      {
        data->pressure = 512;
        //Serial.println("MPRLS:error");
      } else {
        uint32_t rawval = (uint32_t(buffer[1]) << 16) | (uint32_t(buffer[2]) << 8) | (uint32_t(buffer[3]));
        rawval = rawval / MPRLS_DIVIDER;
        //only procede if value != 0
        if(rawval)
        {
          //calibrate if requested
          if(data->calib_now)
          {
            data->cpressure = rawval;
          } else {
            //base value is 512; calculate difference between current & calibrated raw value
            data->pressure = 512 + (rawval - data->cpressure);
          }
        }
      }
      
      //trigger new conversion
      Wire.beginTransmission(MPRLS_ADDR);
      Wire.write(0xAA);
      Wire.write(0);
      Wire.write(0);
      Wire.endTransmission();
      
      break;
    case MPXV:
    default:
      data->pressure = analogRead(PRESSURE_SENSOR_PIN);
      break;
  }
}

void readForce(struct SensorData *data)
{
  switch(sensor_force)
  {
    case RES_I2C:
      //TODO: add NAU7802 reading here.
      break;
    case NO_FORCE:
    default:
      data->up =    512;
      data->down =  512;
      data->left =  512;
      data->right = 512;
      break;
  }
}
