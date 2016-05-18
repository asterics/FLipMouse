  
/* 
     FLipWare - built upon Assistive Button Interface (FABI) Version 2.0  - AsTeRICS Academy 2015 - http://www.asterics-academy.net
      allows control of HID functions via FLipmouse module and/or AT-commands  
   

   requirements:  Teensy 2.0++ or TeensyLC with external EEPROM
                  Teensyduino AddOn for Arduino IDE
                  USB type set to USB composite device (Serial + Keyboard + Mouse + Joystick)
        sensors:  3 momentary switches connected to GPIO pins / 4 force sensors + 1 pressure sensor connected to ADC pins 
       
   
   for a list of supported AT commands, see commands.h / commands.cpp
   
 */
 

#include "FlipWare.h"        //  FABI command definitions
#include <EEPROM.h>
#include <Wire.h>        // for the external EEPROM

// Constants and Macro definitions

#define DEFAULT_WAIT_TIME       5   // wait time for one loop interation in milliseconds
#define DEFAULT_DEBOUNCING_TIME 7   // debouncing interval for button-press / release

#define UP_BUTTON       3     // index numbers of the virtual buttons (not pin numbers in this case !)
#define DOWN_BUTTON     4
#define LEFT_BUTTON     5
#define RIGHT_BUTTON    6
#define SIP_BUTTON      7
#define SPECIAL_SIP_BUTTON  8
#define PUFF_BUTTON         9
#define SPECIAL_PUFF_BUTTON 10
#define SPECIAL_DOWN_BUTTON 11


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

  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={0,2,1};  //  maps physical button pins to button index 0,1,2
  uint8_t IR_SENSOR_PIN = 4;
  int8_t  led_map[NUMBER_OF_LEDS]={5,16,17};              //  maps leds pins   
  uint8_t LED_PIN = 13;                                   //  Led output pin, ATTENTION: if SPI (AUX header) is used, this pin is also SCK!!!
  uint8_t IR_LED_PIN = 6;                                 //  IR-Led output pin

#endif

struct slotGeneralSettings settings = {      // default settings valus, for type definition see fabi.h
    1,                                //  Mouse cursor movement active (not the alternative functions )
    60, 60, 20, 20, 500, 525, 3,      // accx, accy, deadzone x, deadzone y, threshold sip, threshold puff, wheel step,
    700, 300,                         // threshold special mode ´, threshold hold mode
    50, 50, 50, 50 ,                  // gain up / down / left / right
    0, 0                              // offset x / y
}; 

char slotName[MAX_SLOTNAME_LEN] = "empty";


struct slotButtonSettings buttons [NUMBER_OF_BUTTONS];                     // array for all buttons - type definition see fabi.h 
char keystringButton[NUMBER_OF_BUTTONS][MAX_KEYSTRING_LEN] = {"","","","","","","","","","","",""};
struct buttonDebouncerType buttonDebouncers [NUMBER_OF_BUTTONS];   // array for all buttonsDebouncers - type definition see fabi.h 

uint16_t calib_now = 1;                       // calibrate zeropoint right at startup !
uint8_t DebugOutput = DEBUG_NOOUTPUT;         // for chatty serial interface use: DEBUG_FULLOUTPUT (attention: not GUI compatible ..)
int waitTime=DEFAULT_WAIT_TIME;

int EmptySlotAddress = 0;
uint8_t reportSlotParameters = REPORT_NONE;
uint8_t reportRawValues = 0;

unsigned long currentTime;
unsigned long previousTime = 0;
float timeDifference;
uint32_t timeStamp = 0;
unsigned long time=0;

uint8_t actSlot=0;

int down;
int left;
int up;
int right;

int x, holdX;
int y, holdY;
int pressure, holdPressure, previousPressure=512;
uint8_t pressureRising=0, pressureFalling=0;

int16_t  cx=0,cy=0;

float accumXpos = 0.f;
float accumYpos = 0.f;
float accelFactor;

int8_t moveX=0;       
int8_t moveY=0;
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

#define HOLD_IDLE 0
#define HOLD_X    1
#define HOLD_Y    2
#define HOLD_PRESSURE 3

uint8_t modeState =0;
uint8_t holdMode =HOLD_IDLE;

int inByte=0;
char * keystring=0;
char * writeKeystring=0;


// function declarations 
void handlePress (int buttonIndex);      // a button was pressed
void handleRelease (int buttonIndex);    // a button was released
uint8_t handleButton(int i, uint8_t state);    // button debouncing and longpress detection  
void UpdateLeds();
void initDebouncers();
void handleModeState();

extern void handleCimMode(void);
extern void init_CIM_frame(void);
extern uint8_t CimParserActive;
extern uint8_t StandAloneMode;

// extern void init_CIM_frame (void);


////////////////////////////////////////
// Setup: program execution starts here
////////////////////////////////////////

void setup() {
   Serial.begin(115200);
    //while (!Serial) ;
   
   if (DebugOutput==DEBUG_FULLOUTPUT)  
     Serial.println("FLipMouse started, Flexible Assistive Button Interface ready !");

   #ifdef TEENSY_LC
     Wire.begin();
   #endif

   pinMode(LED_PIN,OUTPUT);
   digitalWrite(LED_PIN,LOW);

   pinMode(IR_LED_PIN,OUTPUT);
   digitalWrite(LED_PIN,LOW);

   for (int i=0; i<NUMBER_OF_PHYSICAL_BUTTONS; i++)   // initialize physical buttons and bouncers
      pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

   for (int i=0; i<NUMBER_OF_LEDS; i++)   // initialize physical buttons and bouncers
      pinMode (led_map[i], OUTPUT);   // configure the pins for input mode with pullup resistors


   initDebouncers();
   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttons[i].value=0;
      keystringButton[i][0]=0;
   }
   
   init_CIM_frame();  // for AsTeRICS CIM protocol compatibility
   initButtons();
      
   readFromEEPROMSlotNumber(0,true);  // read slot from first EEPROM slot if available !  

   blinkCount=10;  blinkStartTime=25;
   
   if (DebugOutput==DEBUG_FULLOUTPUT) 
   {   Serial.print("Free RAM:");  Serial.println(freeRam());}
   
}

///////////////////////////////
// Loop: the main program loop
///////////////////////////////

void loop() { 
  
  static uint8_t valueReportCount =0, mouseMoveCount=0;

        pressure = analogRead(PRESSURE_SENSOR_PIN);
        if (pressure>previousPressure) pressureRising=1; else pressureRising=0;
        if (pressure<previousPressure) pressureFalling=1; else pressureFalling=0;
        previousPressure=pressure;
        
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
          currentTime = millis();
          timeDifference = currentTime - previousTime;
          previousTime = currentTime;
          accelFactor= timeDifference / 10000.0f;      
              
          if (calib_now == 0)  {
              x = (left-right) - cx;
              y = (up-down) - cy;

              if (holdMode == HOLD_X) x=holdX;
              if (holdMode == HOLD_Y) y=holdY;
          }
          else  {
              calib_now--;           // wait for calibration
              if (calib_now ==0) {   // calibrate now !!
                 settings.cx = (left-right);                                                   
                 settings.cy = (up-down);
                 cx=settings.cx;
                 cy=settings.cy;
              }
          }    

          if (reportRawValues)   { 
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
          
          if (abs(x)<= settings.dx) x=0;
          if (abs(y)<= settings.dy) y=0;



          handleModeState();
          
          if (modeState==0)
          {  
                for (int i=0;i<NUMBER_OF_PHYSICAL_BUTTONS;i++)    // update button press / release events
                    handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);
          
                if (holdMode == HOLD_PRESSURE)   {
                   handleButton(SIP_BUTTON, holdPressure < settings.ts ? 1 : 0); 
                   handleButton(PUFF_BUTTON, holdPressure > settings.tp ? 1 : 0);
                }
                else  {
                   if (pressure > settings.tp) 
                   {  
                      if (!pressureRising)
                        handleButton(PUFF_BUTTON, 1);
                   } else handleButton(PUFF_BUTTON, 0);

                   if (pressure < settings.ts) 
                   {  
                      if (!pressureFalling)
                       handleButton(SIP_BUTTON, 1); 
                   } else handleButton(SIP_BUTTON, 0);
               }
              
                
                if (settings.mouseOn == 1) {
                  if (y>settings.dy) { 
                     accumYpos += ((float)(y-settings.dy)*settings.ay) * accelFactor;
                  } 
                  else if (y<-settings.dy) {
                     accumYpos +=  ((float)(y+settings.dy)*settings.ay) * accelFactor; 
                  }
                  if (x>settings.dx) {
                     accumXpos += ((float)(x-settings.dx)*settings.ax) * accelFactor; 
                  }
                  else if (x<-settings.dx) {
                     accumXpos += ((float)(x+settings.dx)*settings.ax) * accelFactor; 
                  }
                
                  int xMove = (int)accumXpos;
                  int yMove = (int)accumYpos;
                  
                  Mouse.move(xMove, yMove);
                  accumXpos -= xMove;
                  accumYpos -= yMove;
                }
                else  { // handle alternative joystick actions
                  handleButton(UP_BUTTON,  y<-settings.dy ? 1 : 0);
                  handleButton(DOWN_BUTTON,  y>settings.dy ? 1 : 0);
                  handleButton(LEFT_BUTTON,  x<-settings.dx ? 1 : 0);
                  handleButton(RIGHT_BUTTON,  x>settings.dx ? 1 : 0);
                }
          
                if ((moveX!=0) || (moveY!=0))   // movement induced by button actions  
                {
                  if (mouseMoveCount++%4==0)
                    Mouse.move(moveX, moveY);
                }
          
          }
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
             if (leftMouseButton) Mouse.press(MOUSE_LEFT); else Mouse.release(MOUSE_LEFT);
             old_leftMouseButton=leftMouseButton;
          }
          if  (middleMouseButton!=old_middleMouseButton) {
             if (middleMouseButton) Mouse.press(MOUSE_MIDDLE); else Mouse.release(MOUSE_MIDDLE);
             old_middleMouseButton=middleMouseButton;
          }
          if  (rightMouseButton!=old_rightMouseButton)  {
             if (rightMouseButton) Mouse.press(MOUSE_RIGHT); else Mouse.release(MOUSE_RIGHT);
             old_rightMouseButton=rightMouseButton;
         }
        
         // handle Keyboard output (single key press/release is done seperately via setKeyValues() ) 
         if (writeKeystring) {
            Keyboard.print(writeKeystring);
            writeKeystring=0;
        }    
           
        delay(waitTime);  // to limit move movement speed. TBD: remove delay, use millis() !
    }  // standAloneMode
    else
    {
        handleCimMode();   // create periodic reports if running in AsTeRICS CIM compatibility mode
    }
    UpdateLeds();
}


#define SPECIALMODE_XY_THRESHOLD 200
#define SPECIALMODE_PUFF_RELEASE 530
#define SPECIALMODE_SIP_RELEASE  505
#define SPECIALMODE_STABLETIME   20
#define SPECIALMODE_STABLEEXIT   250

#define MODESTATE_IDLE 0
#define MODESTATE_ENTER_SPECIALMODE   1
#define MODESTATE_SPECIALMODE_ACTIVE  2
#define MODESTATE_ENTER_HOLDMODE     10
#define MODESTATE_DETECT_HOLDSTATE   11
#define MODESTATE_HOLDMODE_ACTIVE    12
#define MODESTATE_RELEASE            13
#define MODESTATE_RETURN_TO_ILDE     99

void handleModeState()
{         
    static int waitStable=0;
    static uint8_t rememberMouseOn; 
    
         switch (modeState)  {
            case MODESTATE_IDLE:   // IDLE
               if (pressure > settings.sm) { 
                   modeState=MODESTATE_ENTER_SPECIALMODE;
                   makeTone(TONE_ENTERSPECIAL,0 );             
                   initDebouncers();
                   release_all();
                   }
               if (pressure < settings.hm ) { 
                   if (holdMode==HOLD_IDLE)
                   {
                     modeState=MODESTATE_ENTER_HOLDMODE;       // enter hold mode detection
                     makeTone(TONE_HOLD,0 );             
                     initDebouncers();
                     release_all();
                   } 
                   else  
                   {
                     settings.mouseOn=rememberMouseOn;
                     modeState=MODESTATE_RELEASE;   // exit hold mode
                     makeTone(TONE_HOLD,2 );             
                   } 
                 }
                 break;
            case MODESTATE_ENTER_SPECIALMODE:   // puffed, wait for release          
                if (pressure < SPECIALMODE_PUFF_RELEASE)
                   waitStable++;
                else waitStable=0;
                if (waitStable>=SPECIALMODE_STABLETIME)
                    modeState=MODESTATE_SPECIALMODE_ACTIVE;
                  break; 
            case MODESTATE_SPECIALMODE_ACTIVE:  // specialmode active
               if (handleButton(0, (y<-SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RETURN_TO_ILDE;
               else if (handleButton(1, (x<-SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RETURN_TO_ILDE;
               else if (handleButton(2, (x>SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RETURN_TO_ILDE;
               else if (handleButton(SPECIAL_DOWN_BUTTON, (y>SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RETURN_TO_ILDE;
               else if (handleButton(SPECIAL_SIP_BUTTON, pressure < settings.ts  ? 1 : 0)) modeState=MODESTATE_RETURN_TO_ILDE;
               else if (handleButton(SPECIAL_PUFF_BUTTON, pressure > settings.tp  ? 1 : 0)) modeState=MODESTATE_RETURN_TO_ILDE;
               else { waitStable++; if (waitStable>SPECIALMODE_STABLEEXIT) { waitStable=0; modeState=MODESTATE_RELEASE; makeTone(TONE_EXITSPECIAL,0 ); } }
               break;

            case MODESTATE_ENTER_HOLDMODE:   // sipped, wait for release to enter detect hold state          
                if (pressure > SPECIALMODE_SIP_RELEASE)
                   waitStable++;
                else waitStable=0;
                if (waitStable>=SPECIALMODE_STABLETIME)
                    modeState=MODESTATE_DETECT_HOLDSTATE;
                 break; 
            case MODESTATE_DETECT_HOLDSTATE:  // detect hold state
               rememberMouseOn=settings.mouseOn;
               if ((y<-settings.dy) || (y>settings.dy)) {holdMode=HOLD_Y; holdY=y; settings.mouseOn=0; modeState=MODESTATE_HOLDMODE_ACTIVE; }
               else if ((x<-settings.dx) || (x>settings.dx)) {holdMode=HOLD_X; holdX=x; settings.mouseOn=0; modeState=MODESTATE_HOLDMODE_ACTIVE; }
               else if ((pressure<settings.ts) || (pressure>settings.tp)) {holdMode=HOLD_PRESSURE; holdPressure=pressure; modeState=MODESTATE_HOLDMODE_ACTIVE; }
               else { waitStable++; if (waitStable>SPECIALMODE_STABLEEXIT) { waitStable=0; modeState=MODESTATE_RELEASE; makeTone(TONE_HOLD,2 ); } }
               break;

            case MODESTATE_HOLDMODE_ACTIVE:   // hold mode selected          
                    makeTone(TONE_HOLD,1 );             
                    modeState=MODESTATE_RETURN_TO_ILDE;
                break; 
            case MODESTATE_RELEASE:   // wait for release to exit hold mode          
                if (pressure > SPECIALMODE_SIP_RELEASE)  
                {
                    holdMode=HOLD_IDLE;
                    modeState=MODESTATE_RETURN_TO_ILDE;
                }
                break; 

            default:  // end special mode, enter idle again           
                    initDebouncers();
                    release_all();
                    modeState=MODESTATE_IDLE;
                 break; 
          }
}

void handlePress (int buttonIndex)   // a button was pressed
{   
    performCommand(buttons[buttonIndex].mode,buttons[buttonIndex].value,keystringButton[buttonIndex],1);
}

void handleRelease (int buttonIndex)    // a button was released: deal with "sticky"-functions
{
   switch(buttons[buttonIndex].mode) {
     case CMD_PL: leftMouseButton=0; break;
     case CMD_PR: rightMouseButton=0; break;
     case CMD_PM: middleMouseButton=0; break;
     case CMD_MX: moveX=0; break;      
     case CMD_MY: moveY=0; break;      
     case CMD_KP: releaseKeys(keystringButton[buttonIndex]); break; 
   }
}

uint8_t inHoldMode (int i)
{
   if ((buttons[i].mode == CMD_PL) ||
       (buttons[i].mode == CMD_PR) || 
       (buttons[i].mode == CMD_PM) || 
       (buttons[i].mode == CMD_MX) || 
       (buttons[i].mode == CMD_MY) || 
       (buttons[i].mode == CMD_KP))
   return(1);
   else return(0); 
}
  

uint8_t handleButton(int i, uint8_t state)    // button debouncing and longpress detection  
{                                                 //   (if button i is pressed long and index l>=0, virtual button l is activated !)
   if ( buttonDebouncers[i].bounceState == state) {
     if (buttonDebouncers[i].bounceCount < DEFAULT_DEBOUNCING_TIME) {
       buttonDebouncers[i].bounceCount++;
       if (buttonDebouncers[i].bounceCount == DEFAULT_DEBOUNCING_TIME) {
          if (state != buttonDebouncers[i].stableState)
          { 
            buttonDebouncers[i].stableState=state;
            if (state == 1) {      // new stable state: pressed !
              if (inHoldMode(i)) 
                handlePress(i); 
              buttonDebouncers[i].timestamp=millis();   // start measuring time
            }
            else {   // new stable state: released !
                 if (!inHoldMode(i)) 
                   handlePress(i); 
                 handleRelease(i);
                 return(1);         // indicate that button action has been performed !
            }
          }
       }
     }
     else {  // in stable state
     }
   }
   else {
     buttonDebouncers[i].bounceState = state;
     buttonDebouncers[i].bounceCount=0;     
   }
   return(0);
}   


void initDebouncers()
{
   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttonDebouncers[i].bounceState=0;
      buttonDebouncers[i].stableState=0;
      buttonDebouncers[i].bounceCount=0;
      buttonDebouncers[i].longPressed=0;
   }
}

void release_all()  // releases all previously pressed keys
{
    Keyboard.releaseAll();
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
   switch (kind) {
		case TONE_ENTERSPECIAL: 
			tone(TONE_PIN, 4000, 200);
             break;
		case TONE_EXITSPECIAL: 
			tone(TONE_PIN, 4000, 100);
            break;
		case TONE_CALIB: 
			tone(TONE_PIN, 100, 600);
            break;
		case TONE_CHANGESLOT:
            tone(TONE_PIN, 2000+200*param, 200);
            break;
		case TONE_HOLD:
			switch (param) {
				case 0: tone(TONE_PIN, 3000, 500); break;
				case 1: tone(TONE_PIN, 3500, 100); break;
				case 2: tone(TONE_PIN, 3000, 100); break;
			}
			break;
     }
}



int freeRam ()
{
//    extern int __heap_start, *__brkval;
//    int v;
//    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
return(1);
}
