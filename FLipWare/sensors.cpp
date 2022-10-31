/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: sensors.cpp - functions to read pressure & force sensors

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "sensors.h"
#include "modes.h"
#include "utils.h"

Adafruit_NAU7802 nau;
LoadcellSensor XS,YS,PS;
int sensorWatchdog=-1;


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
 * @brief Global variables for passing sensor data from the ISR
 */
uint8_t channel, newData=0;
int32_t nau_x=0, nau_y=0;
uint32_t mprls_rawval=512;
uint8_t reportValues=0;


/**
 * @brief Used force sensor type. We can use the FSR sensors or possibly
 * in a future version the resistor gauge sensors.
 */
typedef enum {NAU7802, NO_FORCE} force_type_t;
force_type_t sensor_force = NO_FORCE;


/**
   @name configureNAU
   @brief initialises the NAU7802 chip for desired sampling rate and gain
   @return none
*/
void configureNAU() {
  nau.setLDO(NAU7802_3V0);   // NAU7802_2V7, NAU7802_2V4 
  nau.setGain(NAU7802_GAIN_128);  // NAU7802_GAIN_64, NAU7802_GAIN_32
  nau.setRate(NAU7802_RATE_320SPS);  // NAU7802_RATE_80SPS
  nau.setPGACap(NAU7802_CAP_OFF); //disable PGA capacitor on channel 2

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

/**
   @name getValueMPRLS
   @brief reads a current value from the MPRLS pressure sensor (polling)
   @return nonw
*/
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


/**
   @name getValuesISR
   @brief called via pin-change interrupt if new data from NAU7802 is available. 
          Reads NAU data, changes NAU channel and reads MPRLS data, updates global variables.
          Expected sampling rate ca. 64 Hz
   @return none
*/
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
  sensorWatchdog=0;   // we got data, reset watchdog counter!
}

/**
   @name initSensors
   @brief initialises I2C interface, prepares NAU and MPRLS readouts. [called from core 1]
   @return none
*/
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

    pinMode (DRDY_PIN, INPUT);
    nau.setChannel(NAU7802_CHANNEL1);
    configureNAU();
    nau.setChannel(NAU7802_CHANNEL2);
    configureNAU();
    nau.setChannel(NAU7802_CHANNEL1);
    channel=1;
  
    setSensorBoard(SENSOR_BOARD_DEFAULT);  // default settings for x/y sensor signal processing
    PS.setGain(0.1);                           // but: adjust gain for MPRLS pressure sensor        

    attachInterrupt(digitalPinToInterrupt(DRDY_PIN), getValuesISR, RISING);  // start processing data ready signals!
  }

  #ifdef DEBUG_OUTPUT_SENSORS
    Serial.println("SEN: Calibrated internal offset");
  #endif

}

/**
   @name readPressure
   @brief updates and processes new pressure sensor values form MPRLS. [called from core 1]
   @param data: pointer to I2CSensorValues struct, used by core1
   @return none
*/
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
        mprls_filtered=PS.process(mprls_rawval);
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

/**
   @name readForce
   @brief updates and processes new  x/y sensor values from NAU7802. [called from core 1]
   @param data: pointer to I2CSensorValues struct, used by core1
   @return none
*/
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
            currentX=nau_x / NAU_DIVIDER;
            currentY=nau_y / NAU_DIVIDER;
            if (reportValues)  {
              XS.printValues(0x07, 15000);    
              YS.printValues(0x07, 15000);    
              Serial.println("");
            }
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


/**
   @name calculateDirection
   @brief calculates angular direction and force for current x/y sensor values. [called from core 0]
   @param sensorData: pointer to SensorData struct, used by core0
   @return none
*/
void calculateDirection(struct SensorData * sensorData)
{
  sensorData->forceRaw = __ieee754_sqrtf(sensorData->xRaw * sensorData->xRaw + sensorData->yRaw * sensorData->yRaw);
  if (sensorData->forceRaw !=0) {
    sensorData->angle = atan2f ((float)sensorData->yRaw / sensorData->forceRaw, (float)sensorData->xRaw / sensorData->forceRaw );
    
    // get 8 directions
    sensorData->dir=(180+22+(int)(sensorData->angle*57.29578))/45+1;  // translate rad to deg and make 8 sections
    if (sensorData->dir>8) sensorData->dir=1;  
  }
}

/**
   @name applyDeadzone
   @brief calculates deadzone and respective x/y/force values (in sensorData struct). [called from core 0]
   @param sensorData: pointer to SensorData struct, used by core0
   @param slotSettings: pointer to SlotSettings struct, used by core0
   @return none
*/
void applyDeadzone(struct SensorData * sensorData, struct SlotSettings * slotSettings)
{
  if (slotSettings->stickMode == STICKMODE_ALTERNATIVE) {

    // rectangular deadzone for alternative modes
    if (sensorData->xRaw < -slotSettings->dx) 
      sensorData->x = sensorData->xRaw + slotSettings->dx; // apply deadzone values x direction
    else if (sensorData->xRaw > slotSettings->dx) 
      sensorData->x = sensorData->xRaw - slotSettings->dx;
    else sensorData->x = 0;

    if (sensorData->yRaw < -slotSettings->dy)
      sensorData->y = sensorData->yRaw + slotSettings->dy; // apply deadzone values y direction
    else if (sensorData->yRaw > slotSettings->dy)
      sensorData->y = sensorData->yRaw - slotSettings->dy;
    else sensorData->y = 0;

  } else {

    //  circular deadzone for mouse control
    if (sensorData->forceRaw != 0) {     
      float a= slotSettings->dx>0 ? slotSettings->dx : 1 ;
      float b= slotSettings->dy>0 ? slotSettings->dy : 1 ;      
      float s=sinf(sensorData->angle);
      float c=cosf(sensorData->angle);
      sensorData->deadZone =  a*b / __ieee754_sqrtf(a*a*s*s + b*b*c*c);  // ellipse equation, polar form
    }
    else sensorData->deadZone = slotSettings->dx;

    sensorData->force = (sensorData->forceRaw < sensorData->deadZone) ? 0 : sensorData->forceRaw - sensorData->deadZone;
    sensorData->x = (int) (sensorData->force * cosf(sensorData->angle));
    sensorData->y = (int) (sensorData->force * sinf(sensorData->angle));
  }
}


/**
   @name setSensorBoard
   @brief activates a certain parameters profile for signal processing, depending on the selected senosorboard ID
   @param sensorBoardID: the ID of the sensorboard signal processing profile (e.g. SENSOR_BOARD_STRAINGAUGE) 
   @return none
*/
void setSensorBoard(int sensorBoardID) 
{

  detachInterrupt(digitalPinToInterrupt(DRDY_PIN));
  switch (sensorBoardID) {
    case SENSOR_BOARD_DEFAULT:        /* sensorboard default settings */
      XS.setGain(1);                      YS.setGain(1);
      XS.setCompensationFactor(0.05);     YS.setCompensationFactor(0.05);
      XS.setCompensationDecay (0.95);     YS.setCompensationDecay(0.95);
      XS.setMovementThreshold(1000);      YS.setMovementThreshold(1000);
      XS.setIdleDetectionPeriod(1000);    YS.setIdleDetectionPeriod(1000);
      XS.setIdleDetectionThreshold(3000); YS.setIdleDetectionThreshold(3000);
    break;    
    case SENSOR_BOARD_10K:            /* 10K sensorboard settings */
      XS.setGain(0.8);                    YS.setGain(0.8);
      XS.setCompensationFactor(0.06);     YS.setCompensationFactor(0.06);
      XS.setCompensationDecay (0.96);     YS.setCompensationDecay(0.96);
      XS.setMovementThreshold(1500);      YS.setMovementThreshold(1500);
      XS.setIdleDetectionPeriod(1000);    YS.setIdleDetectionPeriod(1000);
      XS.setIdleDetectionThreshold(3500); YS.setIdleDetectionThreshold(3500);
    break;
    case SENSOR_BOARD_100K:           /* 100K sensorboard settings */
      XS.setGain(0.7);                    YS.setGain(0.7);
      XS.setCompensationFactor(0.02);     YS.setCompensationFactor(0.02);
      XS.setCompensationDecay (0.98);     YS.setCompensationDecay(0.98);
      XS.setMovementThreshold(2000);      YS.setMovementThreshold(2000); 
      XS.setIdleDetectionPeriod(1000);    YS.setIdleDetectionPeriod(1000);
      XS.setIdleDetectionThreshold(4000); YS.setIdleDetectionThreshold(4000);
    break;
    case SENSOR_BOARD_STRAINGAUGE:    /* strain gauge sensorboard settings */
      XS.setGain(0.08);                   YS.setGain(0.08);
      XS.setCompensationFactor(0);        YS.setCompensationFactor(0);
      XS.setMovementThreshold(1500);      YS.setMovementThreshold(1500);
      XS.setIdleDetectionPeriod(100);     YS.setIdleDetectionPeriod(100);
      XS.setIdleDetectionThreshold(50);   YS.setIdleDetectionThreshold(50);
      // XS.setBaselineLowpass(0.1);         YS.setBaselineLowpass(0.1);
      // XS.setNoiseLowpass(3);              YS.setNoiseLowpass(3);
      // XS.setActivityLowpass(2);           YS.setActivityLowpass(2);
    break;
    case SENSOR_BOARD_REPORTVALUES:
      reportValues=!reportValues;
    break;
  }
  attachInterrupt(digitalPinToInterrupt(DRDY_PIN), getValuesISR, RISING);
  
  sensorValues.calib_now = CALIBRATION_PERIOD;  // initiate calibration fro new sensorboard profile!
}

/**
   @name checkSensorWatchdog
   @brief checks if an integer value which should be periodically reset when I2C-sensordata is ready exceeds a certain value
   @return true: value within normal range  false: value exceeded -> action must be taken
*/
uint8_t checkSensorWatchdog() {
  //if we never received any valid values, proceed
  if(sensorWatchdog == -1) return(true);
  //if we received at least one time values, but then it stops,
  //check the value and reset after ~1s (SENSOR_WATCHDOG_TIMEOUT)
  if (sensorWatchdog++ > SENSOR_WATCHDOG_TIMEOUT)
    return(false);
  return(true); 
}
