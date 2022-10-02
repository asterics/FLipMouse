
/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org
     https://github.com/asterics/FLipMouse

     Module: FLipWare.ino  (main module)

        This is the firmware for the FlipMouse module, 
        it supports HID device emulation via USB and/or Bluetooth via connected sensors and/or serial AT-commands
        For a description of the supported commands see: commands.h

        HW-requirements:
                  Arduino Nano RP2040 Connect
                  Sensor board with NAU7802 strain gauge ADC
                  1 pressure sensor connected to ADC pin A0 OR an MPRLS I2C pressure sensor
                  3 momentary switches connected to GPIO pins (
                  Neopixel LED
                  1 TSOP 38kHz IR-receiver
                  1 high current IR-LED, driven with a MOSFET
                  optional: SSD1306 display

        SW-requirements:
                  arduino-pico core (https://github.com/earlephilhower/arduino-pico), installable via board manager
                  Adafruit Neopixel library, installable via library manager
                  https://github.com/benjaminaigner/Adafruit_NAU7802
                  https://github.com/ChrisVeigl/LoadcellSensor
                  SSD1306Ascii-library by Bill Greiman, see https://github.com/greiman/SSD1306Ascii
                  Arduino settings: Tools->Board:"Arduino Nano RP2040 Connect",  "Tools->Flash Size: "15MB Sketch, 1MB FS" 

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "gpio.h"      
#include "sensors.h"      
#include "infrared.h"      
#include "display.h"       // for SSD1306 I2C-Oled display
#include "modes.h"
#include "tone.h"
#include "parser.h"  
#include "reporting.h"
#include "cim.h"
#include "keys.h"       

/**
   device name for ID string & BT-pairing
*/
char moduleName[]="FLipmouse";   

/**
   default values for empty configuration slot 
*/
const struct SlotSettings defaultSlotSettings = {      // default slotSettings valus, for type definition see Flipware.h
  "mouse",                          // initial slot name
  0,                                // initial keystringbuffer length
  1,                                // stickMode: Mouse cursor movement active
  40, 40, 20, 20, 50, 20,           // accx, accy, deadzone x, deadzone y, maxspeed, acceleration time
  400, 600, 3,                      // threshold sip, threshold puff, wheel step,
  800, 10,                          // threshold strong puff, threshold strong sip
  40, 20, 40, 20 ,                  // gain and range drift compenstation( vertical, horizontal)
  0,                                // orientation
  1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need daughter board))
  0,                                // sensorboard profile ID 0 
  "en_US",                          // en_US as default keyboard layout.
};


/**
   static variables and data structures for settings and sensor data management
*/
struct SensorData sensorData {        
  .x=0, .y=0, .xRaw=0, .yRaw=0, .pressure=0, 
  .deadZone=0, .force=0, .forceRaw=0, .angle=0,
  .dir=0,
  .autoMoveX=0, .autoMoveY=0,
  .xDriftComp=0, .yDriftComp=0,
  .xLocalMax=0, .yLocalMax=0
};

struct I2CSensorValues sensorValues {        
  .xRaw=0, .yRaw=0, .pressure=0, 
  .calib_now=CALIBRATION_PERIOD     // calibrate sensors after startup !
};


mutex_t sensorDataMutex;                      // for synchronizsation of data access between cores
struct SlotSettings slotSettings;             // contains all slot settings
uint8_t workingmem[WORKINGMEM_SIZE];          // working memory (command parser, IR-rec/play)
uint8_t actSlot = 0;                          // number of current slot
unsigned long lastInteractionUpdate;          // timestamp for HID interaction updates
uint8_t addonUpgrade = BTMODULE_UPGRADE_IDLE; // if not "idle": we are upgrading the addon module


/**
   @name setup
   @brief setup function, program execution of core0 starts here
   @return none
*/
void setup() {

  // prepare synchronizsation of sensor data exchange between cores
  mutex_init(&sensorDataMutex);

 //load slotSettings
  memcpy(&slotSettings,&defaultSlotSettings,sizeof(struct SlotSettings));

  //initialise BT module, if available (must be done early!)
  initBluetooth();   //   TBD: find out why this interferes with I2C !?!

  // initialize peripherals
  Serial.begin(115200);
  
  #ifdef DEBUG_DELAY_STARTUP
    delay(3000);  // allow some time for serial interface to come up
  #endif
  
  initGPIO();
  initIR();
  initButtons();
  initDebouncers();
  init_CIM_frame();  // for AsTeRICS CIM protocol compatibility
  initStorage();   // initialize storage if necessary
  readFromEEPROMSlotNumber(0, true); // read slot from first EEPROM slot if available !

  // NOTE: changed for RP2040!  TBD: why does setBTName damage the console UART TX ??
  // setBTName(moduleName);             // if BT-module installed: set advertising name 

  setKeyboardLayout(slotSettings.kbdLayout); //load keyboard layout from slot
  
  // displayInstalled=displayInit(0);   // check if i2c-display connected   TBD: missing i2c core2 synchronisation!
  // displayUpdate();
  
#ifdef DEBUG_OUTPUT_FULL
  Serial.print("Free RAM:");  Serial.println(freeRam());
  Serial.print(moduleName); Serial.println(" ready !");
#endif
  lastInteractionUpdate = millis();  // get first timestamp

}


/**
   @name loop
   @brief loop function, periodically called from core0 after setup()
   @return none
*/
void loop() {

  //check if we should go into addon upgrade mode
	if(addonUpgrade != BTMODULE_UPGRADE_IDLE) {
    performAddonUpgrade();
    return;
	}

  // handle incoming serial data (AT-commands)
  while (Serial.available() > 0) {
    // send incoming bytes to parser
    parseByte (Serial.read());      // implemented in parser.cpp
  }
  
  // if incoming data from BT-addOn: forward it to host serial interface
  while (Serial_AUX.available() > 0) {
    Serial.write(Serial_AUX.read());
  }

  // perform periodic updates  
  if (millis() >= lastInteractionUpdate + UPDATE_INTERVAL)  {
    lastInteractionUpdate = millis();

    // get current sensor data from core1
    mutex_enter_blocking(&sensorDataMutex);
    sensorData.xRaw=sensorValues.xRaw;
    sensorData.yRaw=sensorValues.yRaw;
    sensorData.pressure=sensorValues.pressure;
    mutex_exit(&sensorDataMutex);

    if (StandAloneMode) {

      // apply rotation if needed
      switch (slotSettings.ro) {
        int32_t tmp;
        case 90: tmp=sensorData.xRaw;sensorData.xRaw=sensorData.yRaw;sensorData.yRaw=tmp;
                break;
        case 180: sensorData.xRaw=-sensorData.xRaw;sensorData.yRaw=-sensorData.yRaw;
                  break;
        case 270: tmp=sensorData.xRaw;sensorData.xRaw=sensorData.yRaw;sensorData.yRaw=-tmp;
                  break;
      }

      calculateDirection(&sensorData);            // calculate angular direction / force form x/y sensor data
      applyDeadzone(&sensorData, &slotSettings);  // calculate updated x/y/force values according to deadzone
      handleUserInteraction();                    // handle all mouse / joystick / button activities

      reportValues();   // send live data to serial
      updateLeds();     // mode indication via front facing neopixel LEDs
      UpdateTones();    // mode indication via audio signals (buzzer)
    }
    if (CimMode) {
      handleCimMode();   // create periodic reports if running in AsTeRICS CIM compatibility mode
    }
  }
  delay(1);  // core0: sleep a bit ...  
}


//
//   Following code is running on second core (core1)
//

/**
   @name setup1
   @brief setup1 function, program execution of core1 starts here (for I2C sensor updates)
   @return none
*/
void setup1() {

  Wire1.begin();
  Wire1.setClock(400000);  // use 400kHz I2C clock
  initSensors();
  initBlink(10,20);  // first signs of life!
  makeTone(TONE_CALIB, 0);
}

/**
   @name loop1
   @brief loop1 function, periodically called from core1 after setup1(), performs I2C sensor updates
   @return none
*/
void loop1() {
  static unsigned long lastUpdate=0;     

  // check if there is a message from the other core (sensorboard change, profile ID)
  if (rp2040.fifo.available()) {
      makeTone(TONE_CALIB, 0);
      setSensorBoard(rp2040.fifo.pop());  
  }

  if (millis() >= lastUpdate + UPDATE_INTERVAL)  {
    lastUpdate = millis();

    // get current sensor values
    mutex_enter_blocking(&sensorDataMutex);
    readPressure(&sensorValues);
    readForce(&sensorValues);
    mutex_exit(&sensorDataMutex);

    // update calibration counter (if calibration running)
    if (sensorValues.calib_now) sensorValues.calib_now--;
    
  }
  delay(1);  // core1: sleep a bit ...  
}

