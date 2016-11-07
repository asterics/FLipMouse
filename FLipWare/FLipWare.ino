  
/* 
     FLipWare - AsTeRICS Academy 2016
     For more info please visit: http://www.asterics-academy.net

     Module: FLipWare.ino  (main module)
     
        This firmware allows control of HID functions via FLipmouse module and/or AT-commands
        For a description of the supported commands see: commands.h

        requirements:  Teensy 2.0++ or TeensyLC with external EEPROM
                       Teensyduino AddOn for Arduino IDE
                       USB-type set to USB composite device (Serial + Keyboard + Mouse + Joystick)
        sensors:  3 momentary switches connected to GPIO pins / 4 force sensors
                  1 pressure sensor connected to ADC pins 
          
   For a list of supported AT commands, see commands.h / commands.cpp

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License: 
   http://www.gnu.org/licenses/gpl-3.0.en.html
 
 */
 

#include "FlipWare.h"        //  FABI command definitions
#include <EEPROM.h>
#include <Wire.h>        // for the external EEPROM

// Constants and Macro definitions

#define DEFAULT_WAIT_TIME       5   // wait time for one loop interation in milliseconds


// global variables

#ifdef TEENSY
  //Analog input pins (4FSRs + 1 pressure sensor)
  #define PRESSURE_SENSOR_PIN A0
  #define DOWN_SENSOR_PIN     A7
  #define LEFT_SENSOR_PIN     A5
  #define UP_SENSOR_PIN       A6
  #define RIGHT_SENSOR_PIN    A4
  //Piezo Pin (for tone generation)  
  #define TONE_PIN  16

  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={13,2,3};  //  maps physical button pins to button index 0,1,2  
  int8_t  led_map[NUMBER_OF_LEDS]={18,19,20};              //  maps leds pins   
  uint8_t LED_PIN = 6;                                     //  Led output pin
  uint8_t IR_LED_PIN = 6;                                  //  IR-Led output pin
#endif

#ifdef TEENSY_LC
  //Analog input pins (4FSRs + 1 pressure sensor)
  #define PRESSURE_SENSOR_PIN A0
  #define DOWN_SENSOR_PIN     A6
  #define LEFT_SENSOR_PIN     A9
  #define UP_SENSOR_PIN       A7
  #define RIGHT_SENSOR_PIN    A8
  //Piezo Pin (for tone generation)
  #define TONE_PIN  9
  //if a Bluetooth addon is installed, there is no space on the main PCB...
  #define TONE_PIN_EXTERNAL 13

  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={0,2,1};  	//  maps physical button pins to button index 0,1,2
  uint8_t IR_SENSOR_PIN = 4;								//  input pin of the TSOP IR receiver
  int8_t  led_map[NUMBER_OF_LEDS]={5,16,17};              	//  maps leds pins   
  uint8_t LED_PIN = 13;                                   	//  Led output pin, ATTENTION: if SPI (AUX header) is used, this pin is also SCK!!!
  uint8_t IR_LED_PIN = 6;                                 	//  IR-Led output pin
#endif

struct slotGeneralSettings settings = {      // default settings valus, for type definition see fabi.h
    1,                                // stickMode: Mouse cursor movement active
    60, 60, 20, 20, 500, 525, 3,      // accx, accy, deadzone x, deadzone y, threshold sip, threshold puff, wheel step,
    800, 10,                          // threshold strong puff, threshold strong sip
    50, 50, 50, 50 ,                  // gain up / down / left / right
    0, 0                              // offset x / y
}; 

char IRName[MAX_SLOTNAME_LEN];
char slotName[MAX_SLOTNAME_LEN] = "empty";
int EmptySlotAddress = 0;
uint8_t reportSlotParameters = REPORT_NONE;
uint8_t reportRawValues = 0;
uint8_t actSlot=0;

uint16_t calib_now = 1;                       // calibrate zeropoint right at startup !
											
//for chatty serial interface use: DEBUG_FULLOUTPUT (attention: not GUI compatible...)
//if set to DEBUG_FULLOUTPUT please activate the following preprocessor warning
uint8_t DebugOutput = DEBUG_NOOUTPUT;       
//uint8_t DebugOutput = DEBUG_FULLOUTPUT;       
//#warning DEACTIVATE DEBUG_FULLOUTPUT AGAIN!!!
int waitTime=DEFAULT_WAIT_TIME;


int up,down,left,right;
int x,y;
int pressure;

int16_t  cx=0,cy=0;

uint8_t leftMouseButton=0,old_leftMouseButton=0;
uint8_t middleMouseButton=0,old_middleMouseButton=0;
uint8_t rightMouseButton=0,old_rightMouseButton=0;
uint8_t leftClickRunning=0;
uint8_t rightClickRunning=0;
uint8_t middleClickRunning=0;
uint8_t doubleClickRunning=0;

uint8_t blinkCount=0;
uint8_t blinkTime=0;
uint8_t blinkStartTime=0;

int inByte=0;
char * keystring=0;
char * writeKeystring=0;


// function declarations 
void UpdateLeds();
void handleMouseClicks();
void reportValues();

extern void handleCimMode(void);
extern void init_CIM_frame(void);
extern uint8_t StandAloneMode;


////////////////////////////////////////
// Setup: program execution starts here
////////////////////////////////////////

void setup() {
   Serial.begin(115200);
   
   //initialise BT module, if available
   initBluetooth();
   
   if (DebugOutput==DEBUG_FULLOUTPUT)  
     Serial.println("FLipMouse started, Flexible Assistive Button Interface ready !");

   #ifdef TEENSY_LC
     Wire.begin();
     pinMode(IR_SENSOR_PIN,INPUT);
     analogWriteFrequency(IR_LED_PIN, 38000);
   #endif
   

   pinMode(LED_PIN,OUTPUT);
   digitalWrite(LED_PIN,LOW);

   pinMode(IR_LED_PIN,OUTPUT);
   digitalWrite(LED_PIN,LOW);

   for (int i=0; i<NUMBER_OF_PHYSICAL_BUTTONS; i++)   // initialize physical buttons and bouncers
      pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

   for (int i=0; i<NUMBER_OF_LEDS; i++)   // initialize physical buttons and bouncers
      pinMode (led_map[i], OUTPUT);   // configure the pins for input mode with pullup resistors

   release_all();
   initDebouncers();
   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttons[i].value=0;
      keystringButton[i][0]=0;
   }
   
   init_CIM_frame();  // for AsTeRICS CIM protocol compatibility
   initButtons();
   
   bootstrapSlotAddresses();
   readFromEEPROMSlotNumber(0,true);  // read slot from first EEPROM slot if available !  

   blinkCount=10;  blinkStartTime=25;
   
   if (DebugOutput==DEBUG_FULLOUTPUT) 
   {   Serial.print("Free RAM:");  Serial.println(freeRam());}
   
}

///////////////////////////////
// Loop: the main program loop
///////////////////////////////

void loop() { 
 
    pressure = analogRead(PRESSURE_SENSOR_PIN);
    
    up =       (uint16_t)((uint32_t)analogRead(UP_SENSOR_PIN)  * settings.gd/50); if (up>1023) up=1023; if (up<0) up=0;
    down =     (uint16_t)((uint32_t)analogRead(DOWN_SENSOR_PIN) * settings.gu/50); if (down>1023) down=1023; if (down<0) down=0;
    left =     (uint16_t)((uint32_t)analogRead(LEFT_SENSOR_PIN) * settings.gr/50); if (left>1023) left=1023; if (left<0) left=0;
    right =    (uint16_t)((uint32_t)analogRead(RIGHT_SENSOR_PIN) * settings.gl/50); if (right>1023) right=1023; if (right<0) right=0;

    while (Serial.available() > 0) {
      // get incoming byte:
      inByte = Serial.read();
      parseByte (inByte);      // implemented in parser.cpp
    }

    if (StandAloneMode)  {              
          if (calib_now == 0)  {      // no calibration, use current values for x and y offset !
              x = (left-right) - cx;
              y = (up-down) - cy;
          }
          else  {
              calib_now--;           // wait for calibration
              if (calib_now ==0) {   // calibrate now !! get new offset values
                 settings.cx = (left-right);                                                   
                 settings.cy = (up-down);
                 cx=settings.cx;
                 cy=settings.cy;
              }
          }    

          reportValues();     // send live data to serial 
          
          if (x<-settings.dx) x+=settings.dx;  // apply deadzone values x direction
          else if (x>settings.dx) x-=settings.dx;
          else x=0;
          
          if (y<-settings.dy) y+=settings.dy;  // apply deadzone values y direction
          else if (y>settings.dy) y-=settings.dy;
          else y=0;

          handleModeState(x, y, pressure);  // handle all mouse / joystick / button activities
          handleMouseClicks();              // update mouse click activities
        
          // handle Keyboard output (single key press/release is done seperately via setKeyValues() ) 
          if (writeKeystring) {
            Keyboard.print(writeKeystring);
            keyboardBTPrint(writeKeystring);
            writeKeystring=0;
          }    
           
          delay(waitTime);  // to limit move movement speed. TBD: remove delay, use millis() !
    }  
    else 
       handleCimMode();   // create periodic reports if running in AsTeRICS CIM compatibility mode

    UpdateLeds();
}

void handleMouseClicks()
{
      // handle running clicks or double clicks
      if (leftClickRunning)
          if (--leftClickRunning==0)  leftMouseButton=0; 
      
      if (rightClickRunning)
          if (--rightClickRunning==0)  rightMouseButton=0; 
   
      if (middleClickRunning)
          if (--middleClickRunning==0)  middleMouseButton=0; 
  
      if (doubleClickRunning)
      {
          doubleClickRunning--;
          if (doubleClickRunning==DEFAULT_CLICK_TIME*2)  leftMouseButton=0; 
          else if (doubleClickRunning==DEFAULT_CLICK_TIME)    leftMouseButton=1; 
          else if (doubleClickRunning==0)    leftMouseButton=0; 
      }
 
	// if any changes were made, update the Mouse buttons
	if(leftMouseButton!=old_leftMouseButton) {
		if (leftMouseButton) { 
			Mouse.press(MOUSE_LEFT); 
			mouseBTPress(1<<0);
		} else { 
			Mouse.release(MOUSE_LEFT); 
			mouseBTRelease(1<<0);
		}
		old_leftMouseButton=leftMouseButton;
	}
	if(middleMouseButton!=old_middleMouseButton) {
		if (middleMouseButton) {
			Mouse.press(MOUSE_MIDDLE); 
			mouseBTPress(1<<2);
		} else { 
			Mouse.release(MOUSE_MIDDLE); 
			mouseBTRelease(1<<2);
		}
		old_middleMouseButton=middleMouseButton;
	}
	if(rightMouseButton!=old_rightMouseButton) {
		if (rightMouseButton) { 
			Mouse.press(MOUSE_RIGHT); 
			mouseBTPress(1<<1);
		} else { 
			Mouse.release(MOUSE_RIGHT); 
			mouseBTRelease(1<<1);
		}
		old_rightMouseButton=rightMouseButton;
	}
}


void reportValues()
{
    static uint8_t valueReportCount =0; 
    if (!reportRawValues)   return; 
    
    if (valueReportCount++ > 10) {                    // report raw values !
      Serial.print("VALUES:");Serial.print(pressure);Serial.print(",");
      Serial.print(up);Serial.print(",");Serial.print(down);Serial.print(",");
      Serial.print(left);Serial.print(",");Serial.print(right);Serial.print(",");
      Serial.print(x);Serial.print(",");Serial.println(y);
      /*
      Serial.print("AnalogRAW:");
      Serial.print(analogRead(UP_SENSOR_PIN));
      Serial.print(",");
      Serial.print(analogRead(DOWN_SENSOR_PIN));
      Serial.print(",");
      Serial.print(analogRead(LEFT_SENSOR_PIN));
      Serial.print(",");
      Serial.println(analogRead(RIGHT_SENSOR_PIN));
      */
      valueReportCount=0;
    }
}

void release_all()  // releases all previously pressed keys
{
    release_all_keys(); 
    leftMouseButton=0;
    rightMouseButton=0;
    middleMouseButton=0;
    moveX=0;
    moveY=0;
}

void initBlink(uint8_t  count, uint8_t startTime)
{
    blinkCount=count;
    blinkStartTime=startTime;
}

void UpdateLeds()
{  
  if (StandAloneMode)
  {
       digitalWrite(LED_PIN,LOW); 
       
       if (blinkCount==0) {
         if ((actSlot+1) & 1) digitalWrite (led_map[0],LOW); else digitalWrite (led_map[0],HIGH); 
         if ((actSlot+1) & 2) digitalWrite (led_map[1],LOW); else digitalWrite (led_map[1],HIGH); 
         if ((actSlot+1) & 4) digitalWrite (led_map[2],LOW); else digitalWrite (led_map[2],HIGH); 
        }
        else {
          if (blinkTime==0)
          {
             blinkTime=blinkStartTime;
             blinkCount--;
             if (blinkCount%2) { 
                 digitalWrite (led_map[0],LOW); digitalWrite (led_map[1],LOW); digitalWrite (led_map[2],LOW); }
              else { 
                 digitalWrite (led_map[0],HIGH); digitalWrite (led_map[1],HIGH); digitalWrite (led_map[2],HIGH); }
          } else blinkTime--;
       }
  }
  else
     digitalWrite(LED_PIN,HIGH);       
}



void makeTone(uint8_t kind, uint8_t param)
{
	uint8_t tonePin = TONE_PIN;
	if(isBluetoothAvailable()) tonePin = TONE_PIN_EXTERNAL;
		
    switch (kind) {
		case TONE_ENTER_STRONGPUFF: 
			tone(tonePin, 4000, 200);
            break;
		case TONE_EXIT_STRONGPUFF: 
			tone(tonePin, 4000, 100);
            break;
		case TONE_CALIB: 
			tone(tonePin, 100, 600);
            break;
		case TONE_CHANGESLOT:
            tone(tonePin, 2000+200*param, 200);
            break;
		case TONE_STRONGSIP:
			switch (param) {
				case 0: tone(tonePin, 3000, 500); break;
				case 1: tone(tonePin, 3500, 100); break;
				case 2: tone(tonePin, 3000, 100); break;
			}
			break;
		case TONE_IR:
			tone(tonePin, 500, 600);
			break;
     }
}

int freeRam ()  // TBD: has to be adapted for TeensyLC ...
{
//    extern int __heap_start, *__brkval;
//    int v;
//    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
      return(1);
}
