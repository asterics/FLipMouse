  
/* 
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: FLipWare.ino  (main module)
     
        This firmware allows control of HID functions via FLipmouse module and/or AT-commands
        For a description of the supported commands see: commands.h

        HW-requirements:  
                  TeensyLC with external EEPROM (see FlipMouse board schematics)
                  4 FSR force sensors connected via voltage dividers to ADC pins A6-A9
                  1 pressure sensor connected to ADC pin A0
                  3 momentary switches connected to GPIO pins 0,1,2
                  3 slot indication LEDs connected to GPIO pins 5,16,17
                  1 TSOP 38kHz IR-receiver connected to GPIO pin 4
                  1 high current IR-LED connected to GPIO pin 6 via MOSEFT
                  optional: FlipMouse Bluetooth daughter board

        SW-requirements:  
                  Teensyduino AddOn for Arduino IDE
                  USB-type set to USB composite device (Serial + Keyboard + Mouse + Joystick)
          
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
#include "math.h" 

// Constants and Macro definitions

#define DEFAULT_WAIT_TIME       5   // wait time for one loop interation in milliseconds

// Global variables

// Analog input pins (4FSRs + 1 pressure sensor)
#define PRESSURE_SENSOR_PIN A0
#define DOWN_SENSOR_PIN     A6
#define LEFT_SENSOR_PIN     A9
#define UP_SENSOR_PIN       A7
#define RIGHT_SENSOR_PIN    A8

//Piezo Pin (for tone generation)
#define TONE_PIN  9

int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={0,2,1};  	//  maps physical button pins to button index 0,1,2
uint8_t IR_SENSOR_PIN = 4;								//  input pin of the TSOP IR receiver
int8_t  led_map[NUMBER_OF_LEDS]={5,16,17};              	//  maps leds pins   
uint8_t LED_PIN = 13;                                   	//  Led output pin, ATTENTION: if SPI (AUX header) is used, this pin is also SCK!!!
uint8_t IR_LED_PIN = 6;                                 	//  IR-Led output pin

struct slotGeneralSettings settings = {      // default settings valus, for type definition see fabi.h
    1,                                // stickMode: Mouse cursor movement active
    40, 40, 20, 20, 50, 20,           // accx, accy, deadzone x, deadzone y, maxspeed, acceleration time
    400, 600, 3,                      // threshold sip, threshold puff, wheel step,
    800, 10,                          // threshold strong puff, threshold strong sip
    40, 20, 40, 20 ,                  // gain and range drift compenstation( vertical, horizontal)
    0, 0,                             // offset x / y
    0,                                // orientation
    1,                                // bt-mode 1: USB, 2: Bluetooth, 3: both (2 & 3 need daughter board)) 
    "",                               // no ir idle code
}; 


uint8_t workingmem[WORKINGMEM_SIZE];     // working memory (command parser, IR-rec/play)

char slotName[MAX_NAME_LEN] = "empty";
int EmptySlotAddress = 0;
uint8_t reportSlotParameters = REPORT_NONE;
uint8_t reportRawValues = 0;
uint8_t actSlot=0;

uint16_t calib_now = 1;                       // calibrate zeropoint right at startup !
											
int waitTime=DEFAULT_WAIT_TIME;

unsigned long updateStandaloneTimestamp;

int up,down,left,right,tmp;
int x,y;
int pressure;
float dz=0,force=0,angle=0;
int xLocalMax=0, yLocalMax=0;
int16_t  cx=0,cy=0;

uint8_t blinkCount=0;
uint8_t blinkTime=0;
uint8_t blinkStartTime=0;

int inByte=0;
char * keystring=0;

// function declarations 
void UpdateLeds();
void UpdateTones();
void reportValues();
void applyDeadzone();

extern void handleCimMode(void);
extern void init_CIM_frame(void);
extern uint8_t StandAloneMode;
extern uint8_t CimMode;


////////////////////////////////////////
// Setup: program execution starts here
////////////////////////////////////////

void setup() {
   Serial.begin(115200);
   
   //initialise BT module, if available
   initBluetooth();
   
   #ifdef DEBUG_OUTPUT_FULL
     Serial.println("FLipMouse started, Flexible Assistive Button Interface ready !");
   #endif

   Wire.begin();
   pinMode(IR_SENSOR_PIN,INPUT);
   analogWriteFrequency(IR_LED_PIN, 38000);  // TBD: flexible carrier frequency for IR, not only 38kHz !
   

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
      keystringButtons[i]=0;
      keystringBufferLen=0;
   }
   
   init_CIM_frame();  // for AsTeRICS CIM protocol compatibility
   initButtons();
   
   bootstrapSlotAddresses();
   readFromEEPROMSlotNumber(0,true);  // read slot from first EEPROM slot if available !  

   blinkCount=10;  blinkStartTime=25;

   #ifdef DEBUG_OUTPUT_FULL
      Serial.print("Free RAM:");  Serial.println(freeRam());
   #endif

   updateStandaloneTimestamp=millis();
}

///////////////////////////////
// Loop: the main program loop
///////////////////////////////

void loop() { 
 
    pressure = analogRead(PRESSURE_SENSOR_PIN);
    
    up =       analogRead(UP_SENSOR_PIN);
    down =     analogRead(DOWN_SENSOR_PIN);
    left =     analogRead(LEFT_SENSOR_PIN);
    right =    analogRead(RIGHT_SENSOR_PIN);

    switch (settings.ro) {
      case 90: tmp=up; up=left; left=down; down=right; right=tmp; break;
      case 180: tmp=up; up=down; down=tmp; tmp=right; right=left; left=tmp; break;
      case 270: tmp=up; up=right; right=down; down=left; left=tmp;break;
    }

    while (Serial.available() > 0) {
      // get incoming byte:
      inByte = Serial.read();
      parseByte (inByte);      // implemented in parser.cpp
    }

    if (StandAloneMode && (millis() >= updateStandaloneTimestamp+waitTime))  {

          updateStandaloneTimestamp=millis();    
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
                 xLocalMax=0; yLocalMax=0;
              }
          }    

          applyDriftCorrection();
          reportValues();     // send live data to serial 
          applyDeadzone();
          handleModeState(x, y, pressure);  // handle all mouse / joystick / button activities
          UpdateLeds();
          UpdateTones();
    }  

    if (CimMode) {
      handleCimMode();   // create periodic reports if running in AsTeRICS CIM compatibility mode
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

void applyDriftCorrection() 
{
    // apply drift correction
    if (((x<0) && (xLocalMax>0)) || ((x>0) && (xLocalMax<0)))  xLocalMax=0;
    if (abs(x)>abs(xLocalMax)) {
      xLocalMax=x;
      //Serial.print("xLocalMax=");
      //Serial.println(xLocalMax);
    }
    if (xLocalMax>settings.rh) xLocalMax=settings.rh;
    if (xLocalMax<-settings.rh) xLocalMax=-settings.rh;
   
    if (((y<0) && (yLocalMax>0)) || ((y>0) && (yLocalMax<0)))  yLocalMax=0;
    if (abs(y)>abs(yLocalMax)) {
      yLocalMax=y;
      //Serial.print("yLocalMax=");
      //Serial.println(yLocalMax);
    }
    if (yLocalMax>settings.rv) yLocalMax=settings.rv;
    if (yLocalMax<-settings.rv) yLocalMax=-settings.rv;

    x-=xLocalMax*((float)settings.gh/250);
    y-=yLocalMax*((float)settings.gv/250);
}

void applyDeadzone()
{
    if (settings.stickMode == STICKMODE_ALTERNATIVE) {

          // rectangular deadzone for alternative modes
        
          if (x<-settings.dx) x+=settings.dx;  // apply deadzone values x direction
          else if (x>settings.dx) x-=settings.dx;
          else x=0;

          if (y<-settings.dy) y+=settings.dy;  // apply deadzone values y direction
          else if (y>settings.dy) y-=settings.dy;
          else y=0;

    } else {     
      
      //  circular deadzone for mouse control

      force=__ieee754_sqrtf(x*x+y*y);
      if (force!=0) {
        angle = atan2f ((float)y/force, (float)x/force );
        dz= settings.dx * (fabsf((float)x)/force) + settings.dy * (fabsf((float)y)/force);
      }
      else { angle=0; dz=settings.dx; }
  
      if (force<dz) force=0; else force-=dz;      
  
      float x2,y2;
      y2=force*sinf(angle);
      x2=force*cosf(angle);

      x=int(x2);
      y=int(y2);
    }
}

void release_all()  // releases all previously pressed keys
{
    release_all_keys();
    mouseRelease(MOUSE_LEFT);
    mouseRelease(MOUSE_MIDDLE);
    mouseRelease(MOUSE_RIGHT);
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

uint16_t toneHeight;
uint16_t toneOnTime;
uint16_t toneOffTime;
uint16_t toneCount=0;

void UpdateTones()
{  
  static uint16_t toneState=0;
  static uint16_t cnt=0;

  if (!toneCount) return;

  uint8_t tonePin = TONE_PIN;

  switch(toneState) {
      case 0:
            tone(tonePin, toneHeight, toneOnTime);
            toneState++;
            break;
      case 1:
            if (++cnt > (toneOnTime+toneOffTime)/5 )  {
              toneCount--;
              toneState=0;
              cnt=0;
            }
            break;
    }
  }


void makeTone(uint8_t kind, uint8_t param)
{
	uint8_t tonePin = TONE_PIN;
		
    switch (kind) {
		case TONE_ENTER_STRONGPUFF: 
			tone(tonePin, 400, 200);
            break;
		case TONE_EXIT_STRONGPUFF: 
			tone(tonePin, 400, 100);
            break;
		case TONE_CALIB: 
			tone(tonePin, 200, 400);
            break;
		case TONE_CHANGESLOT:
          if (!toneCount) {
            toneHeight=2000+200*param;
            toneOnTime=150;
            toneOffTime=50;
            toneCount=param+1;
          }
          break;
		case TONE_ENTER_STRONGSIP:
  		tone(tonePin, 300, 200); 
  				break;
    case TONE_EXIT_STRONGSIP:
      tone(tonePin, 300, 100); 
          break;
		case TONE_IR:
			tone(tonePin, 2500, 30);
			break;
    case TONE_IR_REC:
      tone(tonePin, 350, 500);
      break;
    case TONE_INDICATE_SIP:
      tone(tonePin, 5000, 5);
      break;
    case TONE_INDICATE_PUFF:
      tone(tonePin, 4000, 5);
      break;
    case TONE_BT_PAIRING:
      tone(tonePin, 230, 4000);
      break;
     }
}

//source of code for free ram:
//https://github.com/mpflaga/Arduino-MemoryFree/blob/master/MemoryFree.cpp
#ifdef __arm__
	// should use uinstd.h to define sbrk but Due causes a conflict
	extern "C" char* sbrk(int incr);
#else  // __ARM__
	extern char *__brkval;
#endif  // __arm__

int freeRam()
{
	char top;
	#ifdef __arm__
		return &top - reinterpret_cast<char*>(sbrk(0));
	#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
		return &top - __brkval;
	#else  // __arm__
		return __brkval ? &top - __brkval : &top - __malloc_heap_start;
	#endif  // __arm__
}
