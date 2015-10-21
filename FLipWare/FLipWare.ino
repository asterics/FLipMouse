  
/* 
     FLipWare - built upon Assistive Button Interface (FABI) Version 2.0  - AsTeRICS Academy 2015 - http://www.asterics-academy.net
      allows control of HID functions via FLipmouse module and/or AT-commands  
   

   requirements:  USB HID capable Arduino (Leonardo / Micro / Pro Micro)
                  or Teensy 2.0++ with Teensyduino AddOn setup as USB composite device (Mouse + Keyboard + Serial)
       optional:  Momentary switches connected to GPIO pins / force sensors connected to ADC pins
       
   
   Supported AT-commands:  
   (sent via serial interface, use spaces between parameters and Enter (<cr>, ASCII-code 0x0d) to finish a command)
   
          AT                returns "OK"
          AT ID             identification string will be returned (e.g. "FLipMouse V2.0")
          AT BM <num>       button mode setting for a button (e.g. AT BM 2 -> next command defines the new function for button 2)

          AT CL             click left mouse button  
          AT CR             click right mouse button  
          AT CD             click double with left mouse button
          AT CM             click middle mouse button  

          AT PL             press/hold the left mouse button  
          AT PR             press/hold the right mouse button
          AT PM             press/hold the middle mouse button 
  
          AT RL             release the left mouse button  
          AT RR             release the right mouse button
          AT RM             release the middle mouse button 
          
          AT WU             move mouse wheel up  
          AT WD             move mouse wheel down  
          AT WS <num>       set mouse wheel stepsize (e.g. AT WS 3 sets the wheel stepsize to 3 rows)
   
          AT MX <num>       move mouse in x direction (e.g. AT X 4 moves 4 pixels to the right)  
          AT MY <num>       move mouse in y direction (e.g. AT Y -10 moves 10 pixels up)  

          AT KW <text>      keyboard write text (e.g. AT KW Hello! writes "Hello!")    
          AT KP <text>      key press: press/hold aat listll keys identified in text 
                            (e.g. AT KP KEY_UP presses the "Cursor-Up" key, AT KP KEY_CTRL KEY_ALT KEY_DELETE presses all three keys)
                            for a list of supported key idientifier strings see below ! 
                            
          AT KR <text>      key release: releases all keys identified in text    
          AT RA             release all: releases all currently pressed keys and buttons    
          
          AT SAVE <name>    save settings and current button modes to next free eeprom slot under given name (e.g. AT SAVE mouse1)
          AT LOAD <name>    load button modes from eeprom slot (e.g. AT LOAD mouse1 -> loads profile named "mouse1")
          AT LIST           list all saved mode names 
          AT NEXT           next mode will be loaded (wrap around after last slot)
          AT CLEAR          clear EEPROM content (delete all stored slots)
          AT IDLE           idle command (no operation)
          
    FLipMouse-specific commands:

          AT MM           mouse movements on, alternative functions off
          AT AF           alternative functions on, mouse movements off 
          AT SW           switch between mouse movement and alternative functions
          AT SR           start reporting raw values
          AT ER           end reporting raw values
          AT CA           calibration of zeropoint
          AT AX <num>     acceleration x-axis 
          AT AY <num>     acceleration y-axis 
          AT DX <num>     deadzone x-axis 
          AT DY <num>     deadzone y-axis
          AT TS <num>     treshold sip action
          AT TP <num>     treshold puff action
          AT TT <num>     treshold special mode / hold mode

   supported key identifiers for key press command (AT KP):
 
    KEY_A   KEY_B   KEY_C   KEY_D    KEY_E   KEY_F   KEY_G   KEY_H   KEY_I   KEY_J    KEY_K    KEY_L
    KEY_M   KEY_N   KEY_O   KEY_P    KEY_Q   KEY_R   KEY_S   KEY_T   KEY_U   KEY_V    KEY_W    KEY_X 
    KEY_Y   KEY_Z   KEY_1   KEY_2    KEY_3   KEY_4   KEY_5   KEY_6   KEY_7   KEY_8    KEY_9    KEY_0
    KEY_F1  KEY_F2  KEY_F3  KEY_F4   KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  KEY_F11  KEY_F12	
    
    KEY_RIGHT   KEY_LEFT       KEY_DOWN        KEY_UP      KEY_ENTER    KEY_ESC   KEY_BACKSPACE   KEY_TAB	
    KEY_HOME    KEY_PAGE_UP    KEY_PAGE_DOWN   KEY_DELETE  KEY_INSERT   KEY_END	  KEY_NUM_LOCK    KEY_SCROLL_LOCK
    KEY_SPACE   KEY_CAPS_LOCK  KEY_PAUSE       KEY_SHIFT   KEY_CTRL     KEY_ALT   KEY_RIGHT_ALT   KEY_GUI 
    KEY_RIGHT_GUI
    
*/

#include "fabi.h"        //  Bounce library used for button debouncing
#include <EEPROM.h>
#include <Wire.h>

// Constants and Macro definitions


#define DEFAULT_WAIT_TIME       5   // wait time for one loop interation in milliseconds
#define DEFAULT_CLICK_TIME      8    // time for mouse click (loop iterations from press to release)
#define DOUBLECLICK_MULTIPLIER  5    // CLICK_TIME factor for double clicks
#define DEFAULT_DEBOUNCING_TIME 7   // debouncing interval for button-press / release

#define UP_BUTTON       3     // index numbers of the virtual buttons (not pin numbers)
#define DOWN_BUTTON     4
#define LEFT_BUTTON     5
#define RIGHT_BUTTON    6
#define SIP_BUTTON      7
#define LONGSIP_BUTTON  8
#define PUFF_BUTTON     9
#define LONGPUFF_BUTTON 10


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
#endif

#ifdef TEENSY_LC
  //Analog input pins (4FSRs + 1 pressure sensor)
  #define PRESSURE_SENSOR_PIN A0
  #define DOWN_SENSOR_PIN     A6
  #define LEFT_SENSOR_PIN     A9
  #define UP_SENSOR_PIN       A7
  #define RIGHT_SENSOR_PIN    A8
  //Piezo Pin (for tone generation), TBD: change PCB
  #define TONE_PIN  9

  int8_t  input_map[NUMBER_OF_PHYSICAL_BUTTONS]={0,1,2};  //  maps physical button pins to button index 0,1,2  
  int8_t  led_map[NUMBER_OF_LEDS]={5,16,17};              //  maps leds pins   
  uint8_t LED_PIN = 13;                                     //  Led output pin, ATTENTION: if SPI (AUX header) is used, this pin is also SCK!!!
#endif

struct settingsType settings = {         // type definition see fabi.h
    "empty",
    1,                                   //  Mouse cursor movement active (not the alternative functions )
    40, 40, 60, 60, 500, 525, 3, 620,    // accx, accy, deadzone x, deadzone y, threshold sip, threshold puff, wheel step, threshold special/hold mode
    50, 50, 50, 50 ,                     // gain up / down / left / right
    0, 0                                 // offset x / y
}; 

struct buttonType buttons [NUMBER_OF_BUTTONS];                     // array for all buttons - type definition see fabi.h 
struct buttonDebouncerType buttonDebouncers [NUMBER_OF_BUTTONS];   // array for all buttonsDebouncers - type definition see fabi.h 

uint16_t calib_now = 1;                           // calibrate zeropoint right at startup !
uint8_t DebugOutput = DEFAULT_DEBUGLEVEL;        // default: very chatty at the serial interface ...
int clickTime=DEFAULT_CLICK_TIME;
int waitTime=DEFAULT_WAIT_TIME;

int EmptySlotAddress = 0;
uint8_t reportSlotParameters = 0;
uint8_t reportRawValues = 0;

unsigned long currentTime;
unsigned long previousTime = 0;
float timeDifference;
uint32_t timeStamp = 0;
unsigned long time=0;

uint8_t actButton=0;
uint8_t actSlot=0;

int down;
int left;
int up;
int right;

int x, holdX;
int y, holdY;
int pressure, holdPressure;


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

uint8_t specialMode =0;
uint8_t holdMode =HOLD_IDLE;

int inByte=0;
char * keystring=0;
char * writeKeystring=0;
uint8_t cnt =0,cnt2=0;


// function declarations 
void handlePress (int buttonIndex);      // a button was pressed
void handleRelease (int buttonIndex);    // a button was released
uint8_t handleButton(int i, uint8_t state);    // button debouncing and longpress detection  
void UpdateLeds();
void initDebouncers();
void handleSpecialMode();

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

   for (int i=0; i<NUMBER_OF_PHYSICAL_BUTTONS; i++)   // initialize physical buttons and bouncers
      pinMode (input_map[i], INPUT_PULLUP);   // configure the pins for input mode with pullup resistors

   for (int i=0; i<NUMBER_OF_LEDS; i++)   // initialize physical buttons and bouncers
      pinMode (led_map[i], OUTPUT);   // configure the pins for input mode with pullup resistors


   initDebouncers();
   for (int i=0; i<NUMBER_OF_BUTTONS; i++)   // initialize button array
   {
      buttons[i].value=0;
      buttons[i].keystring[0]=0;
   }
   
   init_CIM_frame();  // for AsTeRICS CIM protocol compatibility
   
   buttons[0].mode=CMD_SW;            // befault for button 1: switch alternative / mouse cursur control mode
   buttons[1].mode=CMD_MOUSE_WHEEL_UP;
   buttons[2].mode=CMD_MOUSE_WHEEL_DOWN;
   buttons[3].mode=CMD_KEY_PRESS; strcpy(buttons[3].keystring,"KEY_UP ");
   buttons[4].mode=CMD_KEY_PRESS; strcpy(buttons[4].keystring,"KEY_DOWN ");
   buttons[5].mode=CMD_KEY_PRESS; strcpy(buttons[5].keystring,"KEY_LEFT ");
   buttons[6].mode=CMD_KEY_PRESS; strcpy(buttons[6].keystring,"KEY_RIGHT ");
   buttons[7].mode=CMD_MOUSE_PRESS_LEFT;
   buttons[8].mode=CMD_IDLE;    
   buttons[9].mode=CMD_MOUSE_CLICK_RIGHT;                          
   buttons[10].mode=CMD_CA;                               // calibrate    
   
   readFromEEPROM(0);  // read slot from first EEPROM slot if available !  

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
          currentTime = millis();
          timeDifference = currentTime - previousTime;
          previousTime = currentTime;
          accelFactor= timeDifference / 10000.0f;      
  
          if (reportRawValues)   { 
            if (cnt++ > 10) {                    // report raw values !
              Serial.print("AT RR ");Serial.print(pressure);Serial.print(",");
              Serial.print(up);Serial.print(",");Serial.print(down);Serial.print(",");
              Serial.print(left);Serial.print(",");Serial.println(right);

              Serial.print("AnalogRAW:");
              Serial.print(analogRead(UP_SENSOR_PIN));
              Serial.print(",");
              Serial.print(analogRead(DOWN_SENSOR_PIN));
              Serial.print(",");
              Serial.print(analogRead(LEFT_SENSOR_PIN));
              Serial.print(",");
              Serial.println(analogRead(RIGHT_SENSOR_PIN));
              cnt=0;
            }
          }

              
          if (calib_now == 0)  {
              x = (left-right) - settings.cx;
              y = (up-down) - settings.cy;
              if (holdMode == HOLD_X) x=holdX;
              if (holdMode == HOLD_Y) y=holdY;
          }
          else  {
              calib_now--;           // wait for calibration
              if (calib_now ==0) {   // calibrate now !!
                 settings.cx = (left-right);                                                   
                 settings.cy = (up-down);
              }
          }    
          
          if (abs(x)<= settings.dx) x=0;
          if (abs(y)<= settings.dy) y=0;


          handleSpecialMode();
          
          if (specialMode==0)
          {  
                for (int i=0;i<NUMBER_OF_PHYSICAL_BUTTONS;i++)    // update button press / release events
                    handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);
          
                if (holdMode == HOLD_PRESSURE)   {
                   handleButton(SIP_BUTTON, holdPressure < settings.ts ? 1 : 0); 
                   handleButton(PUFF_BUTTON, holdPressure > settings.tp ? 1 : 0);
                }
                else  {
                   handleButton(SIP_BUTTON, pressure < settings.ts ? 1 : 0); 
                   handleButton(PUFF_BUTTON, pressure > settings.tp ? 1 : 0);
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
                  if (cnt2++%4==0)
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
              if (doubleClickRunning==clickTime*2)  leftMouseButton=0; 
              else if (doubleClickRunning==clickTime)    leftMouseButton=1; 
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


#define SPECIALMODE_XY_THRESHOLD 450
#define SPECIALMODE_PUFF_RELEASE 530
#define SPECIALMODE_SIP_RELEASE  505
#define SPECIALMODE_STABLETIME   20
#define SPECIALMODE_STABLEEXIT   250

void handleSpecialMode()
{         
    static int waitStable=0;
    
         switch (specialMode)  {
            case 0:   // IDLE
               if (pressure > settings.tt) { 
                   specialMode=1;
                   makeTone(TONE_ENTERSPECIAL,0 );             
                   initDebouncers();
                   release_all();
                   }
               if (pressure < 1024-settings.tt ) { 
                   if (holdMode==HOLD_IDLE)
                   {
                     specialMode=10;       // enter hold mode detection
                     makeTone(TONE_HOLD,0 );             
                     initDebouncers();
                     release_all();
                   } 
                   else  
                   {
                     specialMode=13;   // exit hold mode
                     makeTone(TONE_HOLD,2 );             
                   } 
                 }
                 break;
            case 1:   // puffed, wait for release          
                if (pressure < SPECIALMODE_PUFF_RELEASE)
                   waitStable++;
                else waitStable=0;
                if (waitStable>=SPECIALMODE_STABLETIME)
                    specialMode=2;
                  break; 
            case 2:  // specialmode active
               if (handleButton(0, (y<-SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) specialMode=99;
               else if (handleButton(1, (x<-SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) specialMode=99;
               else if (handleButton(2, (x>SPECIALMODE_XY_THRESHOLD) ? 1 : 0)) specialMode=99;
               else if (handleButton(LONGSIP_BUTTON, pressure < settings.ts  ? 1 : 0)) specialMode=99;
               else if (handleButton(LONGPUFF_BUTTON, pressure > settings.tp  ? 1 : 0)) specialMode=99;
               else { waitStable++; if (waitStable>SPECIALMODE_STABLEEXIT) { waitStable=0; specialMode=13; makeTone(TONE_EXITSPECIAL,0 ); } }
               break;

            case 10:   // sipped, wait for release to enter detect hold state          
                if (pressure > SPECIALMODE_SIP_RELEASE)
                   waitStable++;
                else waitStable=0;
                if (waitStable>=SPECIALMODE_STABLETIME)
                    specialMode=11;
                 break; 
            case 11:  // detect hold state
               if ((y<-settings.dy) || (y>settings.dy)) {holdMode=HOLD_Y; holdY=y; specialMode=12; }
               else if ((x<-settings.dx) || (x>settings.dx)) {holdMode=HOLD_X; holdX=x; specialMode=12; }
               else if ((pressure<settings.ts) || (pressure>settings.tp)) {holdMode=HOLD_PRESSURE; holdPressure=pressure; specialMode=12; }
               else { waitStable++; if (waitStable>SPECIALMODE_STABLEEXIT) { waitStable=0; specialMode=13; makeTone(TONE_HOLD,2 ); } }
               break;

            case 12:   // hold mode selected          
                    makeTone(TONE_HOLD,1 );             
                    specialMode=99;
                break; 
            case 13:   // sipped, wait for release to exit hold mode          
                if (pressure > SPECIALMODE_SIP_RELEASE)  
                {
                    holdMode=HOLD_IDLE;
                    specialMode=99;
                }
                break; 

            default:  // end special mode, enter idle again           
                    initDebouncers();
                    release_all();
                    specialMode=0;
                 break; 
          }
}

void handlePress (int buttonIndex)   // a button was pressed
{   
    performCommand(buttons[buttonIndex].mode,buttons[buttonIndex].value,buttons[buttonIndex].keystring,1);
}

void handleRelease (int buttonIndex)    // a button was released
{
   switch(buttons[buttonIndex].mode) {
     case CMD_MOUSE_PRESS_LEFT: leftMouseButton=0; break;
     case CMD_MOUSE_PRESS_RIGHT: rightMouseButton=0; break;
     case CMD_MOUSE_PRESS_MIDDLE: middleMouseButton=0; break;
     case CMD_MOUSE_MOVEX: moveX=0; break;      
     case CMD_MOUSE_MOVEY: moveY=0; break;      
     case CMD_KEY_PRESS: releaseKeys(buttons[buttonIndex].keystring); break; 
   }
}

uint8_t inHoldMode (int i)
{
   if ((buttons[i].mode == CMD_MOUSE_PRESS_LEFT) ||
       (buttons[i].mode == CMD_MOUSE_PRESS_RIGHT) || 
       (buttons[i].mode == CMD_MOUSE_PRESS_MIDDLE) || 
       (buttons[i].mode == CMD_MOUSE_MOVEX) || 
       (buttons[i].mode == CMD_MOUSE_MOVEY) || 
       (buttons[i].mode == CMD_KEY_PRESS))
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

  

// perform a command  (called from parser.cpp)
//   cmd: command identifier
//   par1: optional numeric parameter
//   periodicMouseMovement: if true, mouse will continue moving - if false: only one movement
void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement)
{
  if (actButton != 0)
  {
      if (DebugOutput==DEBUG_FULLOUTPUT)
      {  
        Serial.print("got new mode for button "); Serial.print(actButton);Serial.print(":");
        Serial.print(cmd);Serial.print(",");Serial.print(par1);Serial.print(",");Serial.println(keystring);
      }
      buttons[actButton-1].mode=cmd;
      buttons[actButton-1].value=par1;
      if (keystring==0) buttons[actButton-1].keystring[0]=0;
      else strcpy(buttons[actButton-1].keystring,keystring);
      actButton=0;
      return;
  }
  
  switch(cmd) {
      case CMD_PRINT_ID:
             Serial.println("FLipMouse V2.0"); 
          break;
      case CMD_BUTTON_MODE:
             release_all();
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.print("set mode for button "); Serial.println(par1);
             if ((par1>=0) && (par1<=NUMBER_OF_BUTTONS))
                 actButton=par1;
             else  Serial.println("?");
          break;
      
      case CMD_MOUSE_CLICK_LEFT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("click left");
             leftMouseButton=1;  leftClickRunning=clickTime;
             break;
      case CMD_MOUSE_CLICK_RIGHT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("click right");
             rightMouseButton=1; rightClickRunning=clickTime;
             break;
      case CMD_MOUSE_CLICK_DOUBLE:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("click double");
             leftMouseButton=1;  doubleClickRunning=clickTime*DOUBLECLICK_MULTIPLIER;
             break;
      case CMD_MOUSE_CLICK_MIDDLE:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("click middle");
             middleMouseButton=1; middleClickRunning=clickTime;
            break;
      case CMD_MOUSE_PRESS_LEFT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("press left");
             leftMouseButton=1; 
             break;
      case CMD_MOUSE_PRESS_RIGHT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("press right");
             rightMouseButton=1; 
             break;
      case CMD_MOUSE_PRESS_MIDDLE:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("press middle");
             middleMouseButton=1; 
             break;
      case CMD_MOUSE_RELEASE_LEFT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("release left");
             leftMouseButton=0;
             break; 
      case CMD_MOUSE_RELEASE_RIGHT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("release right");
             rightMouseButton=0;
             break; 
      case CMD_MOUSE_RELEASE_MIDDLE:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("release middle");
             middleMouseButton=0;
             break; 
      case CMD_MOUSE_WHEEL_UP:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("wheel up");
             #ifndef ARDUINO_PRO_MICRO
               Mouse.scroll(-settings.ws); 
             #else
               Mouse.move (0,0,-settings.ws); 
             #endif
          break;
      case CMD_MOUSE_WHEEL_DOWN:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("wheel down");
             #ifndef ARDUINO_PRO_MICRO
               Mouse.scroll(settings.ws); 
             #else
               Mouse.move (0,0,settings.ws); 
             #endif
          break;
      case CMD_MOUSE_WHEEL_STEP:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("wheel step");
             settings.ws=par1;
          break;
      case CMD_MOUSE_MOVEX:
             if (DebugOutput==DEBUG_FULLOUTPUT) 
             {  Serial.print("mouse move x "); Serial.println(par1); }
             Mouse.move(par1, 0);
             if (periodicMouseMovement) moveX=par1;
          break;
      case CMD_MOUSE_MOVEY:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
             {  Serial.print("mouse move y "); Serial.println(par1); }
             Mouse.move(0, par1);
             if (periodicMouseMovement) moveY=par1;
          break;
      case CMD_KEY_WRITE:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
             {  Serial.print("keyboard write: "); Serial.println(keystring); }
             writeKeystring=keystring;
             break;
      case CMD_KEY_PRESS:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
             {  Serial.print("key press: "); Serial.println(keystring); }
             if (keystring[strlen(keystring)-1] != ' ') strcat(keystring," ");
             setKeyValues(keystring);
             break;
      case CMD_KEY_RELEASE:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
             {  Serial.print("key release: ");  Serial.println(keystring); }
             strcat(keystring," ");
             releaseKeys(keystring);             
             break;
      case CMD_RELEASE_ALL:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.print("release all");
             release_all();             
             break;
            
      case CMD_SAVE_SLOT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
             {  Serial.print("save slot ");  Serial.println(keystring); }
             release_all();
             saveToEEPROM(keystring); 
          break;
      case CMD_LOAD_SLOT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
             {  Serial.print("load slot: "); Serial.println(keystring); }
             release_all();
             if (keystring)
                readFromEEPROM(keystring);
             else printCurrentSlot();
          break;
      case CMD_LIST_SLOTS:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("list slots: ");
             release_all();
             reportSlotParameters=1;   // connection to host: start reporting slot parameters !
             listSlots();
          break;
      case CMD_NEXT_SLOT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.print("load next slot");
             // blinkCount=10;  blinkStartTime=15;  
             release_all();
             readFromEEPROM(0);
             break;
      case CMD_DELETE_SLOTS:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("delete slots"); 
             release_all();
             deleteSlots(); 
          break;


      case CMD_MM:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("mouse function on");
             settings.mouseOn=1;
          break;
      case CMD_AF:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("alternative functions on");
             settings.mouseOn=0;
          break;
      case CMD_SW:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("switch mouse / alternative function");
             blinkCount=6;  blinkStartTime=15;
             if (settings.mouseOn==0) 
             {
               settings.mouseOn=1;
             }
             else {
               settings.mouseOn=0;
             }
          break;
      case CMD_CA:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("start calibration");
             blinkCount=10;  blinkStartTime=20;
             calib_now=100;
             makeTone(TONE_CALIB,0);
          break;
      case CMD_AX:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set acc x");
             settings.ax=par1;
          break;
      case CMD_AY:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set acc y");
             settings.ay=par1;
          break;
      case CMD_DX:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set deadzone x");
             settings.dx=par1;
          break;
      case CMD_DY:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set deadzone y");
             settings.dy=par1;
          break;
      case CMD_TS:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set threshold sip");
             settings.ts=par1;
          break;
      case CMD_TP:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set threshold puff");
             settings.tp=par1;
          break;
      case CMD_SR:
            reportRawValues=1;
          break;
      case CMD_ER:
            reportRawValues=0;
          break;
      case CMD_TT:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set special threshold");
             settings.tt=par1;
          break;
      case CMD_GU:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set up gain");
             settings.gu=par1;
          break;
      case CMD_GD:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set down gain");
             settings.gd=par1;
          break;
      case CMD_GL:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set left gain");
             settings.gl=par1;
          break;
      case CMD_GR:
             if (DebugOutput==DEBUG_FULLOUTPUT)  
               Serial.println("set right gain");
             settings.gr=par1;
          break;
  }
}


void UpdateLeds()
{  
  if (StandAloneMode)
  {
       digitalWrite(LED_PIN,LOW); 
       
       if (blinkCount==0) {
         if (actSlot & 1) digitalWrite (led_map[0],LOW); else digitalWrite (led_map[0],HIGH); 
         if (actSlot & 2) digitalWrite (led_map[1],LOW); else digitalWrite (led_map[1],HIGH); 
         if (actSlot & 4) digitalWrite (led_map[2],LOW); else digitalWrite (led_map[2],HIGH); 
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
  #ifdef TEENSY
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
  #endif
}



int freeRam ()
{
//    extern int __heap_start, *__brkval;
//    int v;
//    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
return(1);
}
