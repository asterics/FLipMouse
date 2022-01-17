/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: cirque.cpp - implementation of the cirque pinnacle trackpad interface, header file

     Based upon Cirque trackpad demo code, Copyright (c) 2018 Cirque Corp.
     Restrictions apply. See: www.cirque.com/sw-license
     Using a Cirque TM0XX0XX w/ Curved Overlay in I2C-mode,
     make sure that R1 is _not_ populated with a 470k resistor

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "FlipWare.h"
#include "i2c_t3.h"
#include "cirque.h"

// Hardware pin-number labels
#define SDA_PIN   23
#define SCL_PIN   22
#define DR_PIN    20
#define GND_PIN   21

// Cirque's 7-bit I2C Slave Address
#define SLAVE_ADDR  0x2A

// Masks for Cirque Register Access Protocol (RAP)
#define WRITE_MASK  0x80
#define READ_MASK   0xA0

// Register config values for this demo
#define SYSCONFIG_1   0x00
#define FEEDCONFIG_1  0x03
#define FEEDCONFIG_2  0x1F
#define Z_IDLE_COUNT  0x05

// Coordinate scaling values
#define PINNACLE_XMAX     2047    // max value Pinnacle can report for X
#define PINNACLE_YMAX     1535    // max value Pinnacle can report for Y
#define PINNACLE_X_LOWER  127     // min "reachable" X value
#define PINNACLE_X_UPPER  1919    // max "reachable" X value
#define PINNACLE_Y_LOWER  63      // min "reachable" Y value
#define PINNACLE_Y_UPPER  1471    // max "reachable" Y value
#define PINNACLE_X_RANGE  (PINNACLE_X_UPPER-PINNACLE_X_LOWER)
#define PINNACLE_Y_RANGE  (PINNACLE_Y_UPPER-PINNACLE_Y_LOWER)
#define ZONESCALE 256   // divisor for reducing x,y values to an array index for the LUT
#define ROWS_Y ((PINNACLE_YMAX + 1) / ZONESCALE)
#define COLS_X ((PINNACLE_XMAX + 1) / ZONESCALE)

// ADC-attenuation settings (held in BIT_7 and BIT_6)
// 1X = most sensitive, 4X = least sensitive
#define ADC_ATTENUATE_1X   0x00
#define ADC_ATTENUATE_2X   0x40
#define ADC_ATTENUATE_3X   0x80
#define ADC_ATTENUATE_4X   0xC0

// Convenient way to store and access measurements
typedef struct _absData
{
  uint16_t xValue;
  uint16_t yValue;
  uint16_t zValue;
  uint8_t buttonFlags;
  bool touchDown;
  bool hovering;
} absData_t;

absData_t touchData;

//const uint16_t ZONESCALE = 256;
//const uint16_t ROWS_Y = 6;
//const uint16_t COLS_X = 8;

// These values require tuning for optimal touch-response
// Each element represents the Z-value below which is considered "hovering" in that XY region of the sensor.
// The values present are not guaranteed to work for all HW configurations.
const uint8_t ZVALUE_MAP[ROWS_Y][COLS_X] =
{
  {0, 0,  0,  0,  0,  0, 0, 0},
  {0, 2,  3,  5,  5,  3, 2, 0},
  {0, 3,  5, 15, 15,  5, 2, 0},
  {0, 3,  5, 15, 15,  5, 3, 0},
  {0, 2,  3,  5,  5,  3, 2, 0},
  {0, 0,  0,  0,  0,  0, 0, 0},
};


void Pinnacle_EnableFeed(bool feedEnable);
void Pinnacle_ClearFlags();

/*  RAP Functions */

int RAP_Init()
{
  delay(200);
  // Set up I2C peripheral
  Wire1.begin();
  Wire1.setDefaultTimeout(10000); // 10ms
  Wire1.setSDA(SDA_PIN);
  Wire1.setSCL(SCL_PIN);
  Wire1.setClock(400000);
  Wire1.beginTransmission(SLAVE_ADDR);  
  return(Wire1.endTransmission());
}

// Reads <count> Pinnacle registers starting at <address>
void RAP_ReadBytes(byte address, byte * data, byte count)
{
  uint8_t cmdByte = READ_MASK | address;   // Form the READ command byte
  uint8_t i = 0;

  Wire1.beginTransmission(SLAVE_ADDR);   // Set up an I2C-write to the I2C slave (Pinnacle)
  Wire1.write(cmdByte);                  // Signal a RAP-read operation starting at <address>
  Wire1.endTransmission(true);           // I2C stop condition

  Wire1.requestFrom((uint8_t)SLAVE_ADDR, count, (uint8_t)true);  // Read <count> bytes from I2C slave
  while(Wire1.available())
  {
    data[i++] = Wire1.read();
  }
}

// Writes single-byte <data> to <address>
void RAP_Write(byte address, byte data)
{
  uint8_t cmdByte = WRITE_MASK | address;  // Form the WRITE command byte

  Wire1.beginTransmission(SLAVE_ADDR);   // Set up an I2C-write to the I2C slave (Pinnacle)
  Wire1.send(cmdByte);                   // Signal a RAP-write operation at <address>
  Wire1.send(data);                      // Write <data> to I2C slave
  Wire1.endTransmission(true);           // I2C stop condition
}



/*  ERA (Extended Register Access) Functions  */
// Reads <count> bytes from an extended register at <address> (16-bit address),
// stores values in <*data>
void ERA_ReadBytes(uint16_t address, uint8_t * data, uint16_t count)
{
  uint8_t ERAControlValue = 0xFF;

  Pinnacle_EnableFeed(false); // Disable feed

  RAP_Write(0x1C, (uint8_t)(address >> 8));     // Send upper byte of ERA address
  RAP_Write(0x1D, (uint8_t)(address & 0x00FF)); // Send lower byte of ERA address

  for(uint16_t i = 0; i < count; i++)
  {
    RAP_Write(0x1E, 0x05);  // Signal ERA-read (auto-increment) to Pinnacle

    // Wait for status register 0x1E to clear
    do
    {
      RAP_ReadBytes(0x1E, &ERAControlValue, 1);
    } while(ERAControlValue != 0x00);

    RAP_ReadBytes(0x1B, data + i, 1);

    Pinnacle_ClearFlags();
  }
}

// Writes a byte, <data>, to an extended register at <address> (16-bit address)
void ERA_WriteByte(uint16_t address, uint8_t data)
{
  uint8_t ERAControlValue = 0xFF;

  Pinnacle_EnableFeed(false); // Disable feed

  RAP_Write(0x1B, data);      // Send data byte to be written

  RAP_Write(0x1C, (uint8_t)(address >> 8));     // Upper byte of ERA address
  RAP_Write(0x1D, (uint8_t)(address & 0x00FF)); // Lower byte of ERA address

  RAP_Write(0x1E, 0x02);  // Signal an ERA-write to Pinnacle

  // Wait for status register 0x1E to clear
  do
  {
    RAP_ReadBytes(0x1E, &ERAControlValue, 1);
  } while(ERAControlValue != 0x00);

  Pinnacle_ClearFlags();
}



/*  Pinnacle-based TM0XX0XX Functions  */
int Pinnacle_Init()
{
  if (RAP_Init()) return(0);
  pinMode(DR_PIN, INPUT);

  // Host clears SW_CC flag
  Pinnacle_ClearFlags();

  // Host configures bits of registers 0x03 and 0x05
  RAP_Write(0x03, SYSCONFIG_1);
  RAP_Write(0x05, FEEDCONFIG_2);

  // Host enables preferred output mode (absolute)
  RAP_Write(0x04, FEEDCONFIG_1);

  // Host sets z-idle packet count to 5 (default is 30)
  RAP_Write(0x0A, Z_IDLE_COUNT);
  return(1);
}

// Reads XYZ data from Pinnacle registers 0x14 through 0x17
// Stores result in absData_t struct with xValue, yValue, and zValue members
void Pinnacle_GetAbsolute(absData_t * result)
{
  uint8_t data[6] = { 0,0,0,0,0,0 };
  RAP_ReadBytes(0x12, data, 6);

  Pinnacle_ClearFlags();

  result->buttonFlags = data[0] & 0x3F;
  result->xValue = data[2] | ((data[4] & 0x0F) << 8);
  result->yValue = data[3] | ((data[4] & 0xF0) << 4);
  result->zValue = data[5] & 0x3F;

  result->touchDown = result->xValue != 0;
}

// Checks touch data to see if it is a z-idle packet (all zeros)
bool Pinnacle_zIdlePacket(absData_t * data)
{
  return data->xValue == 0 && data->yValue == 0 && data->zValue == 0;
}

// Clears Status1 register flags (SW_CC and SW_DR)
void Pinnacle_ClearFlags()
{
  RAP_Write(0x02, 0x00);
  delayMicroseconds(50);
}

// Enables/Disables the feed
void Pinnacle_EnableFeed(bool feedEnable)
{
  uint8_t temp;

  RAP_ReadBytes(0x04, &temp, 1);  // Store contents of FeedConfig1 register

  if(feedEnable)
  {
    temp |= 0x01;                 // Set Feed Enable bit
    RAP_Write(0x04, temp);
  }
  else
  {
    temp &= ~0x01;                // Clear Feed Enable bit
    RAP_Write(0x04, temp);
  }
}


/*  Curved Overlay Functions  */
// Adjusts the feedback in the ADC, effectively attenuating the finger signal
// By default, the the signal is maximally attenuated (ADC_ATTENUATE_4X for use with thin, flat overlays
void setAdcAttenuation(uint8_t adcGain)
{
  uint8_t temp = 0x00;
  ERA_ReadBytes(0x0187, &temp, 1);
  temp &= 0x3F; // clear top two bits
  temp |= adcGain;
  ERA_WriteByte(0x0187, temp);
  ERA_ReadBytes(0x0187, &temp, 1);
}

// Changes thresholds to improve detection of fingers
void tuneEdgeSensitivity()
{
  ERA_WriteByte(0x0149,  0x04);  // xAxis.WideZMin
  ERA_WriteByte(0x0168,  0x03);  // yAxis.WideZMin
}

// This function identifies when a finger is "hovering" so your system can choose to ignore them.
// Explanation: Consider the response of the sensor to be flat across it's area. The Z-sensitivity of the sensor projects this area
// a short distance upwards above the surface of the sensor. Imagine it is a solid cylinder (wider than it is tall)
// in which a finger can be detected and tracked. Adding a curved overlay will cause a user's finger to dip deeper in the middle, and higher
// on the perimeter. If the sensitivity is tuned such that the sensing area projects to the highest part of the overlay, the lowest
// point will likely have excessive sensitivity. This means the sensor can detect a finger that isn't actually contacting the overlay in the shallower area.
// ZVALUE_MAP[][] stores a lookup table in which you can define the Z-value and XY position that is considered "hovering". Experimentation/tuning is required.
// NOTE: Z-value output decreases to 0 as you move your finger away from the sensor, and it's maximum value is 0x63 (6-bits).
void Pinnacle_CheckValidTouch(absData_t * touchData)
{
  uint32_t zone_x, zone_y;
  //eliminate hovering
  zone_x = touchData->xValue / ZONESCALE;
  zone_y = touchData->yValue / ZONESCALE;
  touchData->hovering = !(touchData->zValue > ZVALUE_MAP[zone_y][zone_x]);
}



/*  Logical Scaling Functions */
// Clips raw coordinates to "reachable" window of sensor
// NOTE: values outside this window can only appear as a result of noise
void ClipCoordinates(absData_t * coordinates)
{
  if(coordinates->xValue < PINNACLE_X_LOWER)
  {
    coordinates->xValue = PINNACLE_X_LOWER;
  }
  else if(coordinates->xValue > PINNACLE_X_UPPER)
  {
    coordinates->xValue = PINNACLE_X_UPPER;
  }
  if(coordinates->yValue < PINNACLE_Y_LOWER)
  {
    coordinates->yValue = PINNACLE_Y_LOWER;
  }
  else if(coordinates->yValue > PINNACLE_Y_UPPER)
  {
    coordinates->yValue = PINNACLE_Y_UPPER;
  }
}

// Scales data to desired X & Y resolution
void ScaleData(absData_t * coordinates, uint16_t xResolution, uint16_t yResolution)
{
  uint32_t xTemp = 0;
  uint32_t yTemp = 0;

  ClipCoordinates(coordinates);

  xTemp = coordinates->xValue;
  yTemp = coordinates->yValue;

  // translate coordinates to (0, 0) reference by subtracting edge-offset
  xTemp -= PINNACLE_X_LOWER;
  yTemp -= PINNACLE_Y_LOWER;

  // scale coordinates to (xResolution, yResolution) range
  coordinates->xValue = (uint16_t)(xTemp * xResolution / PINNACLE_X_RANGE);
  coordinates->yValue = (uint16_t)(yTemp * yResolution / PINNACLE_Y_RANGE);
}

void AssertSensorLED(bool state)
{
//  digitalWrite(LED_0, !state);
}

bool DR_Asserted()
{
  return digitalRead(DR_PIN);
}

int initCirque()
{
  pinMode(GND_PIN, OUTPUT);
  digitalWrite(GND_PIN,LOW);
  delay (100);  // time to startup cirque module

  if (!Pinnacle_Init()) return(0);
  // These functions are required for use with thick overlays (curved)
  setAdcAttenuation(ADC_ATTENUATE_2X);  
  tuneEdgeSensitivity();
  Pinnacle_EnableFeed(true);
  return(1);
}

#define MAG_THRESHOLD 2300
#define MAG_HYSTERESIS 50
#define PRINT_MAGVALUES_PERIOD 100


int getHallSensorState() {
 /*
  static uint32_t printADCTimestamp=0;
  static int hallSensorState=0;
  static int magValue=0

  //  analogReadRes(12);

  magValue=analogRead(A1);
  if (magValue>MAG_THRESHOLD-MAG_HYSTERESIS) {
     if (millis()-printADCTimestamp > PRINT_MAGVALUES_PERIOD) {
      printADCTimestamp=millis();
      Serial.print("Mag. ADC-Value A1=");
      Serial.println(magValue);
    }      
  }
  
  if (magValue>MAG_THRESHOLD) {
    if (!(hallSensorState & 1)) {
      hallSensorState|=1;
      Mouse.set_buttons(1, 0, 0);
    }
  } else if (magValue<MAG_THRESHOLD-MAG_HYSTERESIS) {
    if (hallSensorState & 1) {
      Mouse.set_buttons(0, 0, 0);
      hallSensorState&=~1;
    }
  }
  */
}


int getPadData(int * xRaw, int * yRaw) {
  int state=CIRQUE_STATE_INVALID;
  
  getHallSensorState();
  
  if(DR_Asserted())
  {
    Pinnacle_GetAbsolute(&touchData);
    Pinnacle_CheckValidTouch(&touchData);     // Checks for "hover" caused by curved overlays
    ScaleData(&touchData, 1500, 1500);      // Scale coordinates to arbitrary X, Y resolution
    
    if(Pinnacle_zIdlePacket(&touchData)) state=CIRQUE_STATE_LIFTOFF;
    else if(touchData.hovering) state=CIRQUE_STATE_HOVERING;
    else state=CIRQUE_STATE_VALID ;
    *xRaw=touchData.xValue;
    *yRaw=touchData.yValue;
  }
  return(state);
}

// Serial.print(touchData.buttonFlags);
// AssertSensorLED(touchData.touchDown);

int resetPadDirectionStates=0;

int updateCirquePad(int *x, int * y) {
  int padXraw,padYraw;
  int state= getPadData(&padXraw,&padYraw);
  
  if (state) {
    static int lastState=-1,setZeroPoint=1;
    static int x0=-1,y0=-1;
    switch (state) {
      case CIRQUE_STATE_LIFTOFF:
      case CIRQUE_STATE_HOVERING: 
         setZeroPoint=1;
         *x=0;*y=0;
         resetPadDirectionStates=1;
        break;
  
      case CIRQUE_STATE_VALID :
        if (setZeroPoint && (!useAbsolutePadValues())) {
          x0=padXraw;
          y0=padYraw;
          setZeroPoint=0;
          // Serial.print("ZP:");Serial.print(x0);Serial.print(",");Serial.println(y0);
        }
        if (lastState==CIRQUE_STATE_VALID) {
          *x=padXraw-x0;  *y=padYraw-y0;
          // Serial.print(padX);Serial.print(",");Serial.println(padY);
        }
        if (useAbsolutePadValues()) {
          x0=padXraw;
          y0=padYraw;      
        }
        break;              
    }
    lastState=state;    
  }
  return(state);
}

void handleTapClicks(int state,int tapTime) {
   static uint32_t liftTimeStamp=0;
   static uint32_t setTimeStamp=0;
   static uint8_t lastState=0;

   switch(state) {
      case   CIRQUE_STATE_LIFTOFF:
          if (lastState!=CIRQUE_STATE_LIFTOFF) {
            // Serial.println("liftoff");
            liftTimeStamp=millis();
            if (liftTimeStamp - setTimeStamp < tapTime)  {
              // Serial.println("click!");
              mousePress(MOUSE_LEFT);
              delay(DEFAULT_CLICK_TIME);
              mouseRelease(MOUSE_LEFT);
            }
          }
          break;
      case   CIRQUE_STATE_HOVERING:
          // Serial.print("-");
          break;
      case   CIRQUE_STATE_VALID:
          if (lastState !=CIRQUE_STATE_VALID) {
            // Serial.println("valid");
            setTimeStamp=millis();
          }
          break;
   }
   if (state) lastState=state;  
}
