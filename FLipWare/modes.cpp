  
/* 
     FLipWare - AsTeRICS Academy 2016
     For more info please visit: http://www.asterics-academy.net

     Module: modes.cpp: implementation of special modes
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License: 
   http://www.gnu.org/licenses/gpl-3.0.en.html
 
 */
 

#include "FlipWare.h"        

#define STRONGMODE_MOUSE_JOYSTICK_THRESHOLD  50
#define STRONGPUFF_MODE_PUFF_RELEASE 530
#define STRONGSIP_MODE_SIP_RELEASE   505
#define STRONG_MODE_STABLETIME        20
#define STRONG_MODE_EXIT_TIME        250

#define MODESTATE_IDLE                      0
#define MODESTATE_ENTER_STRONGPUFF_MODE     1
#define MODESTATE_STRONGPUFF_MODE_ACTIVE    2
#define MODESTATE_ENTER_STRONGSIP_MODE     10
#define MODESTATE_STRONGSIP_MODE_ACTIVE    11
#define MODESTATE_RELEASE                  12

#define SMOOTHING_VALUE 15.0f

uint8_t modeState = MODESTATE_IDLE;

uint8_t mouseMoveCount=0;
float accelFactor;
float accumXpos = 0.f;
float accumYpos = 0.f;
uint8_t pressureRising=0, pressureFalling=0;
int previousPressure=512;

int8_t moveX=0;       
int8_t moveY=0;

unsigned long currentTime;
unsigned long previousTime = 0;
float timeDifference;
uint32_t timeStamp = 0;
unsigned long time=0;


void handleModeState(int x, int y, int pressure)
{         
    static int waitStable=0;
    int strongDirThreshold;

    currentTime = millis();
    timeDifference = currentTime - previousTime;
    previousTime = currentTime;
    accelFactor= timeDifference / 7000.0f;      

    if (pressure>previousPressure) pressureRising=1; else pressureRising=0;
    if (pressure<previousPressure) pressureFalling=1; else pressureFalling=0;
    previousPressure=pressure;

    if (settings.stickMode == STICKMODE_ALTERNATIVE) 
      strongDirThreshold=0;
    else strongDirThreshold= STRONGMODE_MOUSE_JOYSTICK_THRESHOLD;
    
     switch (modeState)  {                                   // handle strong sip and puff actions
      
        case MODESTATE_IDLE:   // IDLE
           if (pressure > settings.sp) { 
               modeState=MODESTATE_ENTER_STRONGPUFF_MODE;
               makeTone(TONE_ENTER_STRONGPUFF,0 );             
               //initDebouncers();
               }
           if (pressure < settings.ss ) { 
                 modeState=MODESTATE_ENTER_STRONGSIP_MODE;      
                 makeTone(TONE_STRONGSIP,0 );             
                 //initDebouncers();
             }
             break;
             
        case MODESTATE_ENTER_STRONGPUFF_MODE:     // puffed strong, wait for release          
            if (pressure < STRONGPUFF_MODE_PUFF_RELEASE)
               waitStable++;
            else waitStable=0;
            if (waitStable>=STRONG_MODE_STABLETIME)
                modeState=MODESTATE_STRONGPUFF_MODE_ACTIVE;
            break;

        case MODESTATE_STRONGPUFF_MODE_ACTIVE:    // strong puff mode active
           if (handleButton(STRONGPUFF_UP_BUTTON, (y<-strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else if (handleButton(STRONGPUFF_LEFT_BUTTON, (x<-strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else if (handleButton(STRONGPUFF_RIGHT_BUTTON, (x>strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else if (handleButton(STRONGPUFF_DOWN_BUTTON, (y>strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else { 
                  waitStable++; 
                  if (waitStable>STRONG_MODE_EXIT_TIME) {  // no stick movement occurred: perform strong puff action
                    waitStable=0; 
                    handlePress(STRONGPUFF_BUTTON); 
                    handleRelease(STRONGPUFF_BUTTON);
                    makeTone(TONE_EXIT_STRONGPUFF,0 ); 
                    modeState=MODESTATE_IDLE; 
                  } 
                }
           break;

        case MODESTATE_ENTER_STRONGSIP_MODE:   // sipped strong, wait for release          
            if (pressure > STRONGSIP_MODE_SIP_RELEASE)
               waitStable++;
            else waitStable=0;
            if (waitStable>=STRONG_MODE_STABLETIME)
                modeState=MODESTATE_STRONGSIP_MODE_ACTIVE;
            break;
  
        case MODESTATE_STRONGSIP_MODE_ACTIVE:   // strong sip mode active          
           if (handleButton(STRONGSIP_UP_BUTTON, (y<-strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else if (handleButton(STRONGSIP_LEFT_BUTTON, (x<-strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else if (handleButton(STRONGSIP_RIGHT_BUTTON, (x>strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           else if (handleButton(STRONGSIP_DOWN_BUTTON, (y>strongDirThreshold) ? 1 : 0)) modeState=MODESTATE_IDLE;
           //else if (handleButton(STRONGSIP_SIP_BUTTON, pressure < settings.ts  ? 1 : 0)) modeState=MODESTATE_IDLE;
           else {
             waitStable++; 
             if (waitStable>STRONG_MODE_EXIT_TIME) {    // no stick movement occurred: perform strong sip action
                waitStable=0; 
                handlePress(STRONGSIP_BUTTON); 
                handleRelease(STRONGSIP_BUTTON);
                modeState=MODESTATE_IDLE;
                makeTone(TONE_STRONGSIP,1 );
              }
           }
           break;
             
        default: break; 
      }


      if (modeState==MODESTATE_IDLE)
      {  
            for (int i=0;i<NUMBER_OF_PHYSICAL_BUTTONS;i++)    // update button press / release events
                handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);
      

             if (pressure > settings.tp)   // handle single sip/puff actions
             {  
                if (!pressureRising)
                  handleButton(PUFF_BUTTON, 1);
             } else handleButton(PUFF_BUTTON, 0);

             if (pressure < settings.ts) 
             {  
                if (!pressureFalling)
                 handleButton(SIP_BUTTON, 1); 
             } else handleButton(SIP_BUTTON, 0);

             if ((moveX!=0) || (moveY!=0))   // handle movement induced by button actions  
             {
                if (mouseMoveCount++%4==0)
                   Mouse.move(moveX, moveY);
                   if(isBluetoothAvailable()) mouseBT(moveX,moveY,0);
             }
          
             if (settings.stickMode == STICKMODE_MOUSE) {   // handle mouse mode

                float d;    // apply smooth acceleration curve
                 
                if (abs(x)<SMOOTHING_VALUE) d=abs(x) * (float)x/SMOOTHING_VALUE;
                else d=x;
                accumXpos+=d*settings.ax*accelFactor;

                if (abs(y)<SMOOTHING_VALUE) d=abs(y) * (float)y/SMOOTHING_VALUE;
                else d=y;
                accumYpos+=d*settings.ay*accelFactor;
                
                int xMove = (int)accumXpos;
                int yMove = (int)accumYpos;
                
                Mouse.move(xMove, yMove);
                if(isBluetoothAvailable() && (xMove != 0 || yMove != 0)) mouseBT(xMove,yMove,0);
                accumXpos -= xMove;
                accumYpos -= yMove;
            }
            else if (settings.stickMode==STICKMODE_ALTERNATIVE){ // handle alternative actions mode
                handleButton(UP_BUTTON,  y<0 ? 1 : 0);
                handleButton(DOWN_BUTTON,  y>0 ? 1 : 0);
                handleButton(LEFT_BUTTON,  x<0 ? 1 : 0);
                handleButton(RIGHT_BUTTON,  x>0 ? 1 : 0);
            }
            else  {                                          // handle joystick modes 2,3 and 4
              if (y==0) accumYpos = 512;
              else accumYpos = 512+(float)y*settings.ay/50.0f;
              
              if (x==0) accumXpos = 512;
              else  accumXpos = 512+(float)x*settings.ax/50.0f;

              if (accumXpos<0) accumXpos=0; if (accumXpos>1023) accumXpos=1023;
              if (accumYpos<0) accumYpos=0; if (accumYpos>1023) accumYpos=1023;
              
              switch (settings.stickMode) {
                case STICKMODE_JOYSTICK_XY:
                  Joystick.X (accumXpos);
                  Joystick.Y (accumYpos);
                  break;
                case STICKMODE_JOYSTICK_ZR:
                  Joystick.Z (accumXpos);
                  Joystick.Zrotate (accumYpos);
                  break;
                case STICKMODE_JOYSTICK_SLIDERS:
                  Joystick.sliderLeft (accumXpos);
                  Joystick.sliderRight (accumYpos);
                  break;
                }
            }      
      }
}


