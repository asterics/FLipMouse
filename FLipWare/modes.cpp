  
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

#define STRONG_MODE_XY_THRESHOLD     200
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

    currentTime = millis();
    timeDifference = currentTime - previousTime;
    previousTime = currentTime;
    accelFactor= timeDifference / 10000.0f;      

    if (pressure>previousPressure) pressureRising=1; else pressureRising=0;
    if (pressure<previousPressure) pressureFalling=1; else pressureFalling=0;
    previousPressure=pressure;

     switch (modeState)  {
      
        case MODESTATE_IDLE:   // IDLE
           if (pressure > settings.sp) { 
               modeState=MODESTATE_ENTER_STRONGPUFF_MODE;
               makeTone(TONE_ENTER_STRONGPUFF,0 );             
               //initDebouncers();
               //release_all();
               }
           if (pressure < settings.ss ) { 
                 modeState=MODESTATE_ENTER_STRONGSIP_MODE;       // enter hold mode detection
                 makeTone(TONE_STRONGSIP,0 );             
                 //initDebouncers();
                 //release_all();
             }
             break;
             
        case MODESTATE_ENTER_STRONGPUFF_MODE:   // puffed strong, wait for release          
            if (pressure < STRONGPUFF_MODE_PUFF_RELEASE)
               waitStable++;
            else waitStable=0;
            if (waitStable>=STRONG_MODE_STABLETIME)
                modeState=MODESTATE_STRONGPUFF_MODE_ACTIVE;
            break;

        case MODESTATE_STRONGPUFF_MODE_ACTIVE:  // strong puff mode active
           if (handleButton(STRONGPUFF_UP_BUTTON, (y<-STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else if (handleButton(STRONGPUFF_LEFT_BUTTON, (x<-STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else if (handleButton(STRONGPUFF_RIGHT_BUTTON, (x>STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else if (handleButton(STRONGPUFF_DOWN_BUTTON, (y>STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           //else if (handleButton(STRONGPUFF_SIP_BUTTON, pressure < settings.ts  ? 1 : 0)) modeState=MODESTATE_RELEASE;
           //else if (handleButton(STRONGPUFF_PUFF_BUTTON, pressure > settings.tp  ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else { 
                  waitStable++; 
                  if (waitStable>STRONG_MODE_EXIT_TIME) { 
                    waitStable=0; 
                    handlePress(STRONGPUFF_BUTTON); 
                    handleRelease(STRONGPUFF_BUTTON);
                    makeTone(TONE_EXIT_STRONGPUFF,0 ); 
                    modeState=MODESTATE_RELEASE; 
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
           if (handleButton(STRONGSIP_UP_BUTTON, (y<-STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else if (handleButton(STRONGSIP_LEFT_BUTTON, (x<-STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else if (handleButton(STRONGSIP_RIGHT_BUTTON, (x>STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else if (handleButton(STRONGSIP_DOWN_BUTTON, (y>STRONG_MODE_XY_THRESHOLD) ? 1 : 0)) modeState=MODESTATE_RELEASE;
           //else if (handleButton(STRONGSIP_SIP_BUTTON, pressure < settings.ts  ? 1 : 0)) modeState=MODESTATE_RELEASE;
           //else if (handleButton(STRONGSIP_PUFF_BUTTON, pressure > settings.tp  ? 1 : 0)) modeState=MODESTATE_RELEASE;
           else {
             waitStable++; 
             if (waitStable>STRONG_MODE_EXIT_TIME) { 
                waitStable=0; 
                handlePress(STRONGSIP_BUTTON); 
                handleRelease(STRONGSIP_BUTTON);
                modeState=MODESTATE_RELEASE;
                makeTone(TONE_STRONGSIP,1 );
              }
           }
           break;
             
        case MODESTATE_RELEASE:   // end strong modes, enter idle again           
                //initDebouncers();
                //release_all();
                modeState=MODESTATE_IDLE;
             break;
        default: break; 
      }


      if (modeState==MODESTATE_IDLE)
      {  
            for (int i=0;i<NUMBER_OF_PHYSICAL_BUTTONS;i++)    // update button press / release events
                handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);
      

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
          
             if (settings.stickMode == STICKMODE_MOUSE) {   // handle mouse mode

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
            else if (settings.stickMode==STICKMODE_ALTERNATIVE){ // handle alternative actions mode
                handleButton(UP_BUTTON,  y<-settings.dy ? 1 : 0);
                handleButton(DOWN_BUTTON,  y>settings.dy ? 1 : 0);
                handleButton(LEFT_BUTTON,  x<-settings.dx ? 1 : 0);
                handleButton(RIGHT_BUTTON,  x>settings.dx ? 1 : 0);
            }
            else  {  // handle joystick modes 2,3 and 4
              if (y>settings.dy) { 
                 accumYpos = 512+((float)(y-settings.dy)*settings.ay/50.0f);
              } else if (y<-settings.dy)  {  
                 accumYpos = 512+((float)(y+settings.dy)*settings.ay/50.0f);
              } else accumYpos = 512;

              if (x>settings.dx) { 
                 accumXpos = 512+((float)(x-settings.dx)*settings.ax/50.0f);
              } else if (x<-settings.dx)  {  
                 accumXpos = 512+((float)(x+settings.dx)*settings.ax/50.0f);
              } else accumXpos = 512;

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
      
            if ((moveX!=0) || (moveY!=0))   // movement induced by button actions  
            {
              if (mouseMoveCount++%4==0)
                Mouse.move(moveX, moveY);
            }
      
      }
}


