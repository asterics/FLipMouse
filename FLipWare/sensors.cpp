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
LoadcellSensor XS,YS,PS;

#define MPRLS_READ_TIMEOUT (20)     ///< millis
#define MPRLS_STATUS_POWERED (0x40) ///< Status SPI powered bit
#define MPRLS_STATUS_BUSY (0x20)    ///< Status busy bit
#define MPRLS_STATUS_FAILED (0x04)  ///< Status bit for integrity fail
#define MPRLS_STATUS_MATHSAT (0x01) ///< Status bit for math saturation
#define MPRLS_STATUS_MASK (0b01100101) ///< Sensor status mask: only these bits are set


uint8_t channel, newData=0;
int32_t nau_x=0, nau_y=0;
uint32_t mprls_rawval=512;

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


void configureNAU() {
  nau.setLDO(NAU7802_3V0);   // NAU7802_2V7, NAU7802_2V4 
  nau.setGain(NAU7802_GAIN_128);  // NAU7802_GAIN_64, NAU7802_GAIN_32
  nau.setRate(NAU7802_RATE_320SPS);  // NAU7802_RATE_80SPS

  // trigger internal calibration 
  while (! nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.println("Failed to set internal calibration, retrying!");
    delay(1000);
  }
  
  // flush ADC
  for (uint8_t i=0; i<10; i++) {
    while (! nau.available()) delay(1);
    nau.read();
  }  
}

void getValueMPRLS() {
  uint8_t buffer[4]  = {0};
  Wire1.requestFrom(MPRLS_ADDR,1);
  buffer[0] = Wire1.read();
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
    Wire1.requestFrom(MPRLS_ADDR,4);
    for(uint8_t i = 0; i<4; i++) buffer[i] = Wire1.read();
    mprls_rawval = (uint32_t(buffer[1]) << 16) | (uint32_t(buffer[2]) << 8) | (uint32_t(buffer[3]));
  }
  //trigger new conversion
  Wire1.beginTransmission(MPRLS_ADDR);
  Wire1.write(0xAA);
  Wire1.write(0);
  Wire1.write(0);
  Wire1.endTransmission();
}  


void getValuesISR() {
  static int32_t xChange=0,yChange=0;
  
  if (channel==1) {
      xChange=(XS.process(nau.read())-nau_x)/2;
      nau.setChannel(NAU7802_CHANNEL2);
      nau_x+=xChange; nau_y+=yChange;
      channel=2;
      getValueMPRLS();
      newData=1;
  }
  else {
      yChange=(YS.process(nau.read())-nau_y)/2;
      nau.setChannel(NAU7802_CHANNEL1);
      nau_x+=xChange; nau_y+=yChange;
      channel=1;
      getValueMPRLS();
      newData=1;
  }
}

void initSensors()
{
  //detect if there is an MPRLS sensor connected to I2C (Wire)
  Wire1.beginTransmission(MPRLS_ADDR);
  uint8_t result = Wire1.endTransmission();
  //we found the MPRLS sensor, start the initialization
  if(result == 0) sensor_pressure = MPRLS;

  //NAU7802 init
  if (!nau.begin(&Wire1)) {
    Serial.println("SEN: no force sensor found");
    sensor_force = NO_FORCE;
    return;
  } else {
    sensor_force = NAU7802;
    #ifdef DEBUG_OUTPUT_SENSORS
      Serial.println("SEN: Found NAU7802");
    #endif
  }

  pinMode (DRDY_PIN, INPUT);
  nau.setChannel(NAU7802_CHANNEL1);
  configureNAU();
  nau.setChannel(NAU7802_CHANNEL2);
  configureNAU();
  nau.setChannel(NAU7802_CHANNEL1);
  channel=1;

  attachInterrupt(digitalPinToInterrupt(DRDY_PIN), getValuesISR, RISING);

  #ifdef DEBUG_OUTPUT_SENSORS
    Serial.println("SEN: Calibrated internal offset");
  #endif

}


void readPressure(struct I2CSensorValues *data)
{
  static int32_t mprls_filtered=512;
  
  switch(sensor_pressure)
  {
    case MPRLS:          
      //only proceed if value != 0
      if(mprls_rawval)
      {
        //calibrate offset in the middle of the calibration period
        if(data->calib_now==CALIBRATION_PERIOD/2)
        {
          #ifdef DEBUG_OUTPUT_SENSORS
            Serial.print("MPRLS: calib: ");
            Serial.println(mprls_rawval);
          #endif
          PS.calib();
        }
        
        // calculate filtered pressure value, apply signal conditioning
        mprls_filtered=PS.process(mprls_rawval / 10);
        data->pressure = 512 + mprls_filtered / MPRLS_DIVIDER;

        // clamp to 1/1022 (allows disabling strong sip/puff)
        if(data->pressure < 1) data->pressure = 1;
        if(data->pressure > 1022) data->pressure = 1022;

        // during calibration period: bypass activities
        if(data->calib_now) data->pressure=512;

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

void readForce(struct I2CSensorValues *data)
{
  static int32_t currentX=0,currentY=0;
  
  switch(sensor_force)
  {
    case NAU7802:

        //calibrate offset in the middle of the calibration period
        if(data->calib_now==CALIBRATION_PERIOD/2) {
          XS.calib();
          YS.calib();
        }

        // update x/y-values 
        if (newData) {
            newData=0;
            currentX=nau_x/200;
            currentY=nau_y/200;
        }

        // during calibration period: bypass activities
        if(data->calib_now) data->xRaw=data->yRaw=0;
        else {
          data->xRaw =  currentX;
          data->yRaw =  currentY;
        }
        break;
    case NO_FORCE:
    default:
      data->xRaw=0;
      data->yRaw=0;
      break;
  }
}
