
/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org
     https://github.com/asterics/FLipMouse

     Module: FLipWare.ino  (main module)

        This is the firmware for the FlipMouse module, 
        it supports HID device emulation via USB and/or Bluetooth via connected sensors and/or serial AT-commands
        For a description of the supported commands see: commands.h

        HW-requirements:
                  TeensyLC with external EEPROM (see board schematics)
                  4 FSR force sensors connected via voltage dividers to ADC pins A6-A9
                  1 pressure sensor connected to ADC pin A0
                  3 momentary switches connected to GPIO pins 0,1,2
                  3 slot indication LEDs connected to GPIO pins 5,16,17
                  1 TSOP 38kHz IR-receiver connected to GPIO pin 4
                  1 high current IR-LED connected to GPIO pin 6 via MOSEFT
                  optional: Bluetooth daughter board connected to 10-pin expansion port
                  optional: SSD1306 display (connected via I2C-1)

        SW-requirements:
                  Teensyduino AddOn for Arduino IDE, see https://www.pjrc.com/teensy/td_download.html
                  USB-type set to USB composite device (Serial + Keyboard + Mouse + Joystick)
                  SSD1306Ascii-library by Bill Greiman, see https://github.com/greiman/SSD1306Ascii

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

/**
   device name for ID string & BT-pairing
*/
char moduleName[]="Flipmouse";   

/**
   default values for empty configuration slot 
*/
const struct SlotSettings defaultSlotSettings = {      // default slotSettings valus, for type definition see fabi.h
  "mouse",                          // initial slot name
  0,                                // initial keystringbuffer length
  1,                                // stickMode: Mouse cursor movement active
  40, 40, 20, 20, 50, 20,           // accx, accy, deadzone x, deadzone y, maxspeed, acceleration time
  400, 600, 3,                      // threshold sip, threshold puff, wheel step,
  800, 10,                          // threshold strong puff, threshold strong sip
  40, 20, 40, 20 ,                  // gain and range drift compenstation( vertical, horizontal)
  0, 0,                             // offset x / y
  0,                                // orientation
  1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need daughter board))
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
  .calib_now=1,    // calibrate zeropoint right at startup !
  .cx=0, .cy=0,
  .xDriftComp=0, .yDriftComp=0,
  .xLocalMax=0, .yLocalMax=0
};


struct SlotSettings slotSettings;             // contains all slot settings
uint8_t workingmem[WORKINGMEM_SIZE];          // working memory (command parser, IR-rec/play)
uint8_t actSlot = 0;                          // number of current slot
unsigned long lastInteractionUpdate;          // timestamp for HID interaction updates
uint8_t addonUpgrade = BTMODULE_UPGRADE_IDLE; // if not "idle": we are upgrading the addon module


// forward declaration of functions for sensor data processing
void applyCalibration(); 
void applyDriftCorrection();
void applyDeadzone();


/**
   @name setup
   @brief setup function, program execution starts here
   @return none
*/
void setup() {
  //load slotSettings
  memcpy(&slotSettings,&defaultSlotSettings,sizeof(struct SlotSettings));

  //initialise BT module, if available (must be done early!)
  initBluetooth();

  // iniitialize other peripherals
  Serial.begin(115200);
  delay(1000);  // allow some time for serial interface to come up
  Wire.begin();
  Wire.setClock(400000);  // use 400kHz I2C clock
  initGPIO();
  initSensors();
  initIR();
  initButtons();
  initDebouncers();
  init_CIM_frame();  // for AsTeRICS CIM protocol compatibility

  initStorage();   // initialize storage if necessary
  readFromEEPROMSlotNumber(0, true); // read slot from first EEPROM slot if available !

  initBlink(10,25);  // first signs of life!

  setBTName(moduleName);             // if BT-module installed: set advertising name
  
  displayInstalled=displayInit(0);   // check if i2c-display connected, if possible: init
  displayUpdate();
 
#ifdef DEBUG_OUTPUT_FULL
  Serial.print("Free RAM:");  Serial.println(freeRam());
  Serial.print(moduleName); Serial.println(" ready !");
#endif

  lastInteractionUpdate = millis();  // get first timestamp
}

/**
   @name loop
   @brief loop function, periodically called after setup()
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

  // get current sensor values
  readPressure(&sensorData);
  readForce(&sensorData);

  // apply rotation if needed
  switch (slotSettings.ro) {
    int tmp;
    case 90: tmp = sensorData.up; sensorData.up = sensorData.left; sensorData.left = sensorData.down; 
             sensorData.down = sensorData.right; sensorData.right = tmp; 
             break;
    case 180: tmp = sensorData.up; sensorData.up = sensorData.down; sensorData.down = tmp; tmp = sensorData.right; 
              sensorData.right = sensorData.left; sensorData.left = tmp; 
              break;
    case 270: tmp = sensorData.up; sensorData.up = sensorData.right; sensorData.right = sensorData.down; 
              sensorData.down = sensorData.left; sensorData.left = tmp; 
              break;
  }

  // perform periodic updates  
  if (StandAloneMode && (millis() >= lastInteractionUpdate + UPDATE_INTERVAL))  {
    lastInteractionUpdate = millis();
    
    // apply calibration and drift correction
    if (sensorData.calib_now)
      applyCalibration();              
    else  {   // no new calibration, use current values for x and y offset !
      sensorData.xRaw = (sensorData.left - sensorData.right) - sensorData.cx;
      sensorData.yRaw = (sensorData.up - sensorData.down) - sensorData.cy;
      applyDriftCorrection();
    } 

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

/**
   @name applyCalibration
   @brief gets calibration coordinates (cx and cy in slotSettings struct) if caribration time reaches 0
   @return none
*/
void applyCalibration() 
{
  sensorData.xRaw=sensorData.yRaw=0;
  sensorData.calib_now--;           // wait for calibration moment
  if (sensorData.calib_now == 0) {  // calibrate now !! get new offset values
    slotSettings.cx = (sensorData.left - sensorData.right);
    slotSettings.cy = (sensorData.up - sensorData.down);
    sensorData.cx = slotSettings.cx;
    sensorData.cy = slotSettings.cy;
    sensorData.xLocalMax = 0; sensorData.yLocalMax = 0;
  }
}


/**
   @name applyDriftCorrection
   @brief calculates and applies drift correction values for FSR x and y values (in sensorData struct)
   @return none
*/
void applyDriftCorrection()
{
  // apply drift correction
  if (((sensorData.xRaw < 0) && (sensorData.xLocalMax > 0)) || ((sensorData.xRaw > 0) && (sensorData.xLocalMax < 0)))  
     sensorData.xLocalMax = 0;
  if (abs(sensorData.xRaw) > abs(sensorData.xLocalMax)) {
    sensorData.xLocalMax = sensorData.xRaw;
    //Serial.print("xLocalMax=");
    //Serial.println(xLocalMax);
  }
  if (sensorData.xLocalMax > slotSettings.rh) sensorData.xLocalMax = slotSettings.rh;
  if (sensorData.xLocalMax < -slotSettings.rh) sensorData.xLocalMax = -slotSettings.rh;

  if (((sensorData.yRaw < 0) && (sensorData.yLocalMax > 0)) || ((sensorData.yRaw > 0) && (sensorData.yLocalMax < 0)))
    sensorData.yLocalMax = 0;
  if (abs(sensorData.yRaw) > abs(sensorData.yLocalMax)) {
    sensorData.yLocalMax = sensorData.yRaw;
    //Serial.print("yLocalMax=");
    //Serial.println(yLocalMax);
  }
  if (sensorData.yLocalMax > slotSettings.rv) sensorData.yLocalMax = slotSettings.rv;
  if (sensorData.yLocalMax < -slotSettings.rv) sensorData.yLocalMax = -slotSettings.rv;

  sensorData.xDriftComp = sensorData.xLocalMax * ((float)slotSettings.gh / 250);
  sensorData.yDriftComp = sensorData.yLocalMax * ((float)slotSettings.gv / 250);
  sensorData.xRaw -= sensorData.xDriftComp;
  sensorData.yRaw -= sensorData.yDriftComp;
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
