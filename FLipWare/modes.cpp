/* 
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: modes.cpp: implementation of stick operation and special modes
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License: 
   http://www.gnu.org/licenses/gpl-3.0.en.html
 
 */
 

#include "FlipWare.h"        

#define STRONGMODE_MOUSE_JOYSTICK_THRESHOLD  200
#define STRONGMODE_STABLETIME        20
#define STRONGMODE_EXIT_TIME        200
#define STRONGMODE_IDLE_TIME         150
#define SIP_PUFF_SETTLE_TIME         15
#define MIN_HOLD_TIME                10

#define MODESTATE_IDLE                      0
#define MODESTATE_ENTER_STRONGPUFF_MODE     1
#define MODESTATE_STRONGPUFF_MODE_ACTIVE    2
#define MODESTATE_ENTER_STRONGSIP_MODE      3
#define MODESTATE_STRONGSIP_MODE_ACTIVE     4
#define MODESTATE_RELEASE                   5
#define MODESTATE_RETURN_TO_IDLE            6

#define ACCELTIME_MAX 40000

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
    static int xo=0,yo=0;      
    static int waitStable=0;
    static int checkPairing=0;
    static uint8_t puffActive=0, sipActive=0, puffCount=0, sipCount=0;
    static double accelGain=0, accelMaxForce=0,lastAngle=0;
    int strongDirThreshold;
    float moveValX,moveValY;

    currentTime = millis();
    timeDifference = currentTime - previousTime;
    previousTime = currentTime;
    accelGain= timeDifference / 100000000.0f;       

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
               }
           if (pressure < settings.ss ) { 
                 modeState=MODESTATE_ENTER_STRONGSIP_MODE;      
                 makeTone(TONE_ENTER_STRONGSIP,0 );             
             }
             break;
             
        case MODESTATE_ENTER_STRONGPUFF_MODE:     // puffed strong, wait for release          
            if (pressure < settings.tp)
               waitStable++;
            else waitStable=0;
            if (waitStable>=STRONGMODE_STABLETIME)
                modeState=MODESTATE_STRONGPUFF_MODE_ACTIVE;
            break;

        case MODESTATE_STRONGPUFF_MODE_ACTIVE:    // strong puff mode active
           if (y<-strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGPUFF,0 );
             handlePress(STRONGPUFF_UP_BUTTON); handleRelease(STRONGPUFF_UP_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
           }
           else if (x<-strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGPUFF,0 );
             handlePress(STRONGPUFF_LEFT_BUTTON); handleRelease(STRONGPUFF_LEFT_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
           }
           else if (x>strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGPUFF,0 ); 
             handlePress(STRONGPUFF_RIGHT_BUTTON); handleRelease(STRONGPUFF_RIGHT_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
           }
           else if (y>strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGPUFF,0 ); 
             handlePress(STRONGPUFF_DOWN_BUTTON); handleRelease(STRONGPUFF_DOWN_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
          }
           else { 
                  waitStable++; 
                  if (waitStable>STRONGMODE_EXIT_TIME) {  // no stick movement occurred: perform strong puff action
                    waitStable=0; 
                    handlePress(STRONGPUFF_BUTTON); 
                    handleRelease(STRONGPUFF_BUTTON);
                    modeState=MODESTATE_RETURN_TO_IDLE;
                  } 
                }
           break;

        case MODESTATE_ENTER_STRONGSIP_MODE:   // sipped strong, wait for release          
            if (pressure > settings.ts)
               waitStable++;
            else waitStable=0;
            if (waitStable>=STRONGMODE_STABLETIME)
                modeState=MODESTATE_STRONGSIP_MODE_ACTIVE;
            break;
  
        case MODESTATE_STRONGSIP_MODE_ACTIVE:   // strong sip mode active          
           if (y<-strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGSIP,0 );
             handlePress(STRONGSIP_UP_BUTTON); handleRelease(STRONGSIP_UP_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
           }
           else if (x<-strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGSIP,0 );
             handlePress(STRONGSIP_LEFT_BUTTON); handleRelease(STRONGSIP_LEFT_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
           }
           else if (x>strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGSIP,0 ); 
             handlePress(STRONGSIP_RIGHT_BUTTON); handleRelease(STRONGSIP_RIGHT_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
           }
           else if (y>strongDirThreshold) { 
             makeTone(TONE_EXIT_STRONGSIP,0 ); 
             handlePress(STRONGSIP_DOWN_BUTTON); handleRelease(STRONGSIP_DOWN_BUTTON);
             modeState=MODESTATE_RETURN_TO_IDLE;
             waitStable=0;
          }
          else {
             waitStable++; 
             if (waitStable>STRONGMODE_EXIT_TIME) {    // no stick movement occurred: perform strong sip action
                waitStable=0; 
                handlePress(STRONGSIP_BUTTON); 
                handleRelease(STRONGSIP_BUTTON);
                modeState=MODESTATE_RETURN_TO_IDLE;
              }
           }
           break;

       case MODESTATE_RETURN_TO_IDLE:
             waitStable++;
             if (waitStable>STRONGMODE_IDLE_TIME)
             {
                waitStable=0;
                modeState=MODESTATE_IDLE;
                initDebouncers();
                puffActive=0;sipActive=0;
             }
            break;             
        default: break; 
      }


      if (modeState==MODESTATE_IDLE)
      {  
           for (int i=0;i<NUMBER_OF_PHYSICAL_BUTTONS;i++)    // update button press / release events
              handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);

           if (digitalRead(input_map[0]) == LOW) {
            checkPairing++;
            if (checkPairing == BUTTON1_PRESS_TIME_FOR_PAIRING) {
                    makeTone(TONE_BT_PAIRING, 0);
                    startBTPairing();
                    checkPairing=0;
            }
           } else checkPairing=0;

           switch (puffActive)  {
            case 0:
               if (pressure > settings.tp)   // handle single puff actions
               {  
                 makeTone(TONE_INDICATE_PUFF,0);
                 puffActive=1;puffCount=0;
               }  
               break;
            case 1:  
                 if (!pressureRising)
                 { 
                    if (puffCount++>SIP_PUFF_SETTLE_TIME) 
                    {
                       puffActive=2;
                       puffCount=MIN_HOLD_TIME; 
                       handlePress(PUFF_BUTTON); 
                    }
                 } else if (puffCount) puffCount--;
                 break;

            case 2:
                 if (puffCount) puffCount--;
                 if ((pressure < settings.tp)&&(!puffCount)) { 
                    handleRelease(PUFF_BUTTON); 
                    puffActive=0; 
                 }
           }

 
           switch (sipActive)  {
            case 0:
               if (pressure < settings.ts)   // handle single sip actions
               {  
                 makeTone(TONE_INDICATE_SIP,0);
                 sipActive=1;sipCount=0;
               }  
               break;
            case 1:  
                 if (!pressureFalling)
                 { 
                    if (sipCount++>SIP_PUFF_SETTLE_TIME) 
                    {
                       sipActive=2;
                       sipCount=MIN_HOLD_TIME; 
                       handlePress(SIP_BUTTON); 
                    }
                 } else if (sipCount) sipCount--;
                 break;

            case 2:
                 if (sipCount) sipCount--;
                 if ((pressure > settings.ts) && (!sipCount)) { 
                    handleRelease(SIP_BUTTON); 
                    sipActive=0; 
                 }
           }

           if ((moveX!=0) || (moveY!=0))   // handle movement induced by button actions  
           {
              if (mouseMoveCount++%4==0)
                 mouseMove(moveX, moveY);
           }
        
           if (settings.stickMode == STICKMODE_MOUSE) {   // handle mouse mode

                float max_speed= settings.ms / 10.0f;
  
                if (force==0) { accelFactor=0; accelMaxForce=20; lastAngle=0;}
                else {

                  if (force>accelMaxForce) accelMaxForce=force;

                  if (force > accelMaxForce * 0.8)
                  {
                      if (accelFactor < ACCELTIME_MAX)
                          accelFactor +=settings.ac;                    
                  }
                  else if (accelMaxForce > 20) accelMaxForce *= 0.99;

                  if (force < accelMaxForce * 0.7)  accelFactor *= 0.999;
                  if (force < accelMaxForce * 0.3)  accelFactor *= 0.994;

//                  if (lastAngle != 0) {
//                     double dampingFactor=fabs(fabs(angle)-fabs(lastAngle));
//                     if (dampingFactor>0.1) dampingFactor=0.1;
//                     accelFactor *= (1.0-dampingFactor/7);
                     double dampingFactor=fabs(x-xo)+fabs(y-yo);
                     accelFactor *= (1.0-dampingFactor/2000.0);
                     //Serial.println((int)(dampingFactor*100));
//                  } 
                  lastAngle=angle;
                  xo=x;yo=y;
                }

                moveValX=x*(float)settings.ax*accelFactor*accelGain;
                moveValY=y*(float)settings.ay*accelFactor*accelGain;

                float actSpeed= sqrt (moveValX*moveValX + moveValY*moveValY);
                if (actSpeed > max_speed) {
                   moveValX *= (max_speed / actSpeed);
                   moveValY *= (max_speed / actSpeed);
                }
                
                accumXpos+=moveValX;
                accumYpos+=moveValY;
                
                int xMove = (int)accumXpos;
                int yMove = (int)accumYpos;

                if ((xMove != 0) || (yMove != 0))
                   mouseMove(xMove, yMove);

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
