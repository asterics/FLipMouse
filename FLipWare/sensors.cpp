/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: sensors.cpp - functions to read pressure & force sensors

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "sensors.h"

Adafruit_NAU7802 nau;

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
typedef enum {MPXV, MPRLS, NO_PRESSURE} pressure_type_t;
pressure_type_t sensor_pressure = NO_PRESSURE;

/**
 * @brief Used force sensor type. We can use the FSR sensors or possibly
 * in a future version the resistor gauge sensors.
 */
typedef enum {NAU7802, NO_FORCE} force_type_t;
force_type_t sensor_force = NO_FORCE;

void initSensors()
{
  //detect if there is an MPRLS sensor connected to I2C (Wire)
  Wire.beginTransmission(MPRLS_ADDR);
  uint8_t result = Wire.endTransmission();
  //we found the MPRLS sensor, start the initialization
  if(result == 0) sensor_pressure = MPRLS;

  //NAU7802 init
  if (!nau.begin()) {
    Serial.println("SEN: no force sensor found");
    sensor_force = NO_FORCE;
    return;
  } else {
    sensor_force = NAU7802;
    #ifdef DEBUG_OUTPUT_SENSORS
      Serial.println("SEN: Found NAU7802");
    #endif
  }
  //set LDO: TBA: what value is the best?
  nau.setLDO(NAU7802_2V7);
  //nau.setLDO(NAU7802_2V4);
  //nau.setLDO(NAU7802_3V0);
  
  //set gain: TBA: select good value
  nau.setGain(NAU7802_GAIN_128);
  //nau.setGain(NAU7802_GAIN_64);
  //nau.setGain(NAU7802_GAIN_32);
  
  //set sampling rate
  //nau.setRate(NAU7802_RATE_320SPS);
  nau.setRate(NAU7802_RATE_80SPS);
  //nau.setRate(NAU7802_RATE_40SPS);
  
  //flush NAU7802
  for (uint8_t i=0; i<10; i++) {
    while (!nau.available()) delay(1);
    nau.read();
  }

  while (!nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.println("SEN: error calibrating int offset");
    delay(1000);
  }
  #ifdef DEBUG_OUTPUT_SENSORS
    Serial.println("SEN: Calibrated internal offset");
  #endif

  while (!nau.calibrate(NAU7802_CALMOD_OFFSET)) {
    Serial.println("SEN: Failed to calibrate system offset, retrying!");
    delay(1000);
  }
  #ifdef DEBUG_OUTPUT_SENSORS
    Serial.println("Calibrated system offset");
  #endif
  
  nau.enable(true);
}


void readPressure(struct SensorData *data)
{
  static uint64_t lastRead = millis();
  uint8_t buffer[4]  = {0};
  switch(sensor_pressure)
  {
    case MPRLS:
      //according to datasheet, minimum sample rate is 161Sps -> ~7ms
      if(abs((long int)(millis() - lastRead)) > 7)
      {
        lastRead = millis();
        //request status byte
        Wire.requestFrom(MPRLS_ADDR,1);
        buffer[0] = Wire.read();
        //any errors? set pressure value to 512, convert otherwise...
        if(buffer[0] & MPRLS_STATUS_BUSY)
        {
          //sensor is busy, cannot read data
          #ifdef DEBUG_OUTPUT_SENSORS
            Serial.println("MPRLS: busy");
          #endif
          return;
        }
        if((buffer[0] & MPRLS_STATUS_MATHSAT) || (buffer[0] & MPRLS_STATUS_FAILED))
        {
          //sensor failed or saturated
          #ifdef DEBUG_OUTPUT_SENSORS
            Serial.println("MPRLS:failed or saturated");
          #endif
          return;
        } else {
          //request all 4 bytes
          Wire.requestFrom(MPRLS_ADDR,4);
          for(uint8_t i = 0; i<4; i++) buffer[i] = Wire.read();
          uint32_t rawval = (uint32_t(buffer[1]) << 16) | (uint32_t(buffer[2]) << 8) | (uint32_t(buffer[3]));
          //only procede if value != 0
          if(rawval)
          {
            //calibrate if requested
            if(data->calib_now)
            {
              #ifdef DEBUG_OUTPUT_SENSORS
                Serial.print("MPRLS: calib: ");
                Serial.println(rawval);
              #endif
              data->cpressure = rawval;
            } else {
              //base value is 512; calculate difference between current & calibrated raw value.
              int32_t diff = ((int32_t)rawval - (int32_t)data->cpressure) / MPRLS_DIVIDER;
              //clamp to 0
              if(diff < -512) data->pressure = 0;
              else data->pressure = 512 + diff;
              //clamp value
              if(data->pressure > 1023) data->pressure = 1023;
            }
          }
        }
        
        //trigger new conversion
        Wire.beginTransmission(MPRLS_ADDR);
        Wire.write(0xAA);
        Wire.write(0);
        Wire.write(0);
        Wire.endTransmission();
      }
      break;
    case NO_PRESSURE:
      data->pressure = 512;
      break;
    case MPXV:
    default:
      data->pressure = analogRead(PRESSURE_SENSOR_PIN);
      break;
  }
}

void readForce(struct SensorData *data)
{
  static bool channel1 = true;
  static uint64_t lastRead = millis();
  
  switch(sensor_force)
  {
    case NAU7802:
      //every 10ms one channel reading -> 20ms for both channels -> 50Hz update rate
      if(abs((long int)(millis() - lastRead)) > 10)
      {
        lastRead = millis();
        //interleave between channel 1 & 2
        if(channel1) {
          //wait for available data on channel 1 & read to "up"
          if(!nau.available()) return;
          data->left = nau.read() / slotSettings.dividerLeft;
          if(data->left > 1023) data->left = 1023;
          if(data->left < -1023) data->left = -1023;
          data->right = 1024-data->left;
          //switch to channel 2
          nau.setChannel(NAU7802_CHANNEL2);
          nau.calibrate(NAU7802_CALMOD_OFFSET);
          nau.calibrate(NAU7802_CALMOD_GAIN);
          channel1 = false;
        } else {
          //wait for available data on channel 2 & read to "left"
          if(!nau.available()) return;
          data->up = nau.read() / slotSettings.dividerUp;
          if(data->up > 1023) data->up = 1023;
          if(data->up < -1023) data->up = -1023;
          data->down = 1024-data->up;
          //switch back to channel 1
          nau.setChannel(NAU7802_CHANNEL1);
          nau.calibrate(NAU7802_CALMOD_OFFSET);
          nau.calibrate(NAU7802_CALMOD_GAIN);
          channel1 = true;
        }
      }
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
