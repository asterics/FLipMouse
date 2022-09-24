
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
                  SSD1306Ascii-library by Bill Greiman, see https://github.com/greiman/SSD1306Ascii
                  Arduino settings: "Flash Size: 15MB Sketch, 1MB FS"

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
#include "utils.h"       
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
  40, 40, 20, 20, 100, 100, 50, 20, // accx, accy, deadzone x, deadzone y, divider x, divider y, maxspeed, acceleration time
  400, 600, 3,                      // threshold sip, threshold puff, wheel step,
  800, 10,                          // threshold strong puff, threshold strong sip
  40, 20, 40, 20 ,                  // gain and range drift compenstation( vertical, horizontal)
  0, 0,                             // offset x / y
  0,                                // orientation
  1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need daughter board))
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
  .up=0, .down=0, .left=0, .right=0,
  .calib_now=100,    // calibrate zeropoint right at startup !
  .cx=0, .cy=0, .cpressure=0,
  .xDriftComp=0, .yDriftComp=0,
  .xLocalMax=0, .yLocalMax=0
};


struct SlotSettings slotSettings;             // contains all slot settings
uint8_t workingmem[WORKINGMEM_SIZE];          // working memory (command parser, IR-rec/play)
uint8_t actSlot = 0;                          // number of current slot
unsigned long lastInteractionUpdate;          // timestamp for HID interaction updates
uint8_t addonUpgrade = BTMODULE_UPGRADE_IDLE; // if not "idle": we are upgrading the addon module


// forward declaration of functions for sensor data processing
void applyDeadzone();


/**
   @name setup
   @brief setup function, program execution of core1 starts here
   @return none
*/
void setup() {
  //load slotSettings
  memcpy(&slotSettings,&defaultSlotSettings,sizeof(struct SlotSettings));

  //initialise BT module, if available (must be done early!)
  initBluetooth();

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

  initBlink(10,25);  // first signs of life!

  // NOTE: changed for RP2040!  TBD: why does setBTName damage the console UART TX ??
  // setBTName(moduleName);             // if BT-module installed: set advertising name 

  setKeyboardLayout(slotSettings.kbdLayout); //load keyboard layout from slot
  
  displayInstalled=displayInit(0);   // check if i2c-display connected, if possible: init
  displayUpdate();
#ifdef DEBUG_OUTPUT_FULL
  Serial.print("Free RAM:");  Serial.println(freeRam());
  Serial.print(moduleName); Serial.println(" ready !");
#endif
  lastInteractionUpdate = millis();  // get first timestamp

}

/**
   @name setup1
   @brief setup1 function, program execution of core2 starts here
   @return none
*/
void setup1() {

  Wire.begin();
  Wire.setClock(400000);  // use 400kHz I2C clock
  initSensors();
}

/**
   @name loop1
   @brief loop1 function, periodically called from core2 after setup1()
   @return none
*/
void loop1() {
  static unsigned long lastInteractionUpdate1=0;     
  static int first_calib=1;
	
  if (millis() >= lastInteractionUpdate1 + UPDATE_INTERVAL)  {
    lastInteractionUpdate1 = millis();

    if (StandAloneMode) {

      // update calibration counter
      if (sensorData.calib_now>0) sensorData.calib_now--;
      else if (first_calib) {sensorData.calib_now=200;first_calib=0;}  // TBD

      // get current sensor values
      readPressure(&sensorData);
      readForce(&sensorData);
      if (sensorData.calib_now) {sensorData.xRaw=sensorData.yRaw=0;}
      
    }
  }
}



/**
   @name loop
   @brief loop function, periodically called from core1 after setup()
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

    if (StandAloneMode) {


/*
      // apply rotation if needed
      switch (slotSettings.ro) {
        int32_t tmp;
        case 90: tmp=sensorData.xRaw;sensorData.xRaw=sensorData.yRaw;sensorData.yRaw=tmp;
                break;
        case 180: sensorData.xRaw=-sensorData.xRaw;sensorData.yRaw=-sensorData.yRaw;
                  break;
        case 270: tmp=sensorData.xRaw;sensorData.xRaw=-sensorData.yRaw;sensorData.yRaw=tmp;
                  break;
      }
*/
      // calculate angular direction and force
      sensorData.forceRaw = __ieee754_sqrtf(sensorData.xRaw * sensorData.xRaw + sensorData.yRaw * sensorData.yRaw);
      if (sensorData.forceRaw !=0) {
        sensorData.angle = atan2f ((float)sensorData.yRaw / sensorData.forceRaw, (float)sensorData.xRaw / sensorData.forceRaw );
        
        // get 8 directions
        sensorData.dir=(180+22+(int)(sensorData.angle*57.29578))/45+1;  // translate rad to deg and make 8 sections
        if (sensorData.dir>8) sensorData.dir=1;  
      }

      // calculate updated x/y/force values according to deadzone
      applyDeadzone();

      handleUserInteraction();  // handle all mouse / joystick / button activities

      reportValues();     // send live data to serial
      updateLeds();
      UpdateTones();
    }
    if (CimMode) {
      handleCimMode();   // create periodic reports if running in AsTeRICS CIM compatibility mode
    }
  }
}


/**
   @name applyDeadzone
   @brief calculates deadzone and respective x/y/force values (in sensorData struct)
   @return none
*/
void applyDeadzone()
{
  if (slotSettings.stickMode == STICKMODE_ALTERNATIVE) {

    // rectangular deadzone for alternative modes
    if (sensorData.xRaw < -slotSettings.dx) 
      sensorData.x = sensorData.xRaw + slotSettings.dx; // apply deadzone values x direction
    else if (sensorData.xRaw > slotSettings.dx) 
      sensorData.x = sensorData.xRaw - slotSettings.dx;
    else sensorData.x = 0;

    if (sensorData.yRaw < -slotSettings.dy)
      sensorData.y = sensorData.yRaw + slotSettings.dy; // apply deadzone values y direction
    else if (sensorData.yRaw > slotSettings.dy)
      sensorData.y = sensorData.yRaw - slotSettings.dy;
    else sensorData.y = 0;

  } else {

    //  circular deadzone for mouse control
    if (sensorData.forceRaw != 0) {     
      float a= slotSettings.dx>0 ? slotSettings.dx : 1 ;
      float b= slotSettings.dy>0 ? slotSettings.dy : 1 ;      
      float s=sinf(sensorData.angle);
      float c=cosf(sensorData.angle);
      sensorData.deadZone =  a*b / __ieee754_sqrtf(a*a*s*s + b*b*c*c);  // ellipse equation, polar form
    }
    else sensorData.deadZone = slotSettings.dx;

    sensorData.force = (sensorData.forceRaw < sensorData.deadZone) ? 0 : sensorData.forceRaw - sensorData.deadZone;
    sensorData.x = (int) (sensorData.force * cosf(sensorData.angle));
    sensorData.y = (int) (sensorData.force * sinf(sensorData.angle));
  }
}
