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


typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;

# define GET_FLOAT_WORD(i,d)                                        \
do {                                                                \
  ieee_float_shape_type gf_u;                                        \
  gf_u.value = (d);                                                \
  (i) = gf_u.word;                                                \
} while (0)

# define SET_FLOAT_WORD(d,i)                                        \
do {                                                                \
  ieee_float_shape_type sf_u;                                        \
  sf_u.word = (i);                                                \
  (d) = sf_u.value;                                                \
} while (0)

float __ieee754_sqrtf(float x)
{
  float z;
  __uint32_t r,hx;
  __int32_t ix,s,q,m,t,i;

  GET_FLOAT_WORD(ix,x);
  
  hx = ix&0x7fffffff;

  if(x==0) return 0;/* sqrt(+-0) = +-0 */
  if(x<0)  return (x-x)/(x-x);    /* sqrt(-ve) = sNaN */

    /* normalize x */
  m = (ix>>23);
  m -= 127; /* unbias exponent */
  ix = (ix&0x007fffffL)|0x00800000L;
  if(m&1) /* odd m, double x to make it even */
      ix += ix;
  m >>= 1;  /* m = [m/2] */

    /* generate sqrt(x) bit by bit */
  ix += ix;
  q = s = 0;    /* q = sqrt(x) */
  r = 0x01000000L;    /* r = moving bit from right to left */

  while(r!=0) {
    t = s+r; 
    if(t<=ix) { 
      s    = t+r; 
      ix  -= t; 
      q   += r; 
    } 
    ix += ix;
    r>>=1;
  }

  float one = 1.0, tiny=1.0e-30;
  /* use floating add to find out rounding direction */
  if(ix!=0) {
      z = one-tiny; /* trigger inexact flag */
      if (z>=one) {
        z = one+tiny;
        if (z>one)
           q += 2;
        else
           q += (q&1);
      }
  }
  ix = (q>>1)+0x3f000000L;
  ix += (m <<23);
  SET_FLOAT_WORD(z,ix);
  return z;
}



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

uint8_t modeState = MODESTATE_IDLE;

uint8_t mouseMoveCount=0;
float accelFactor;
float accumXpos = 0;
float accumYpos = 0;
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
    static float accelMaxForce=0,lastAngle=0;
    int strongDirThreshold;
    float moveValX,moveValY;

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
  
                if (force==0) { 
                  accelFactor=0; 
                  accelMaxForce=0; 
                  lastAngle=0;
                }
                else {

                  if (force>accelMaxForce) accelMaxForce=force;

                  if (force > accelMaxForce * 0.8f)
                  {
                      if (accelFactor < 1.0f)
                          accelFactor += ((float)settings.ac/5000000.0f);                    
                  }
                  else if (accelMaxForce > 0) accelMaxForce *= 0.99f;

                  if (force < accelMaxForce * 0.6f)  accelFactor *= 0.995f;
                  if (force < accelMaxForce * 0.4f)  accelFactor *= 0.99f;

                  float dampingFactor=fabsf(x-xo)+fabsf(y-yo);
                  accelFactor *= (1.0f-dampingFactor/1000.0f);
                  lastAngle=angle;
                  xo=x;yo=y;
                }
                
                moveValX=x*(float)settings.ax*accelFactor;
                moveValY=y*(float)settings.ay*accelFactor;

                float actSpeed=  __ieee754_sqrtf (moveValX*moveValX + moveValY*moveValY);
                float max_speed= settings.ms / 10;
                if (actSpeed > max_speed) {
                   moveValX *= (max_speed / actSpeed);
                   moveValY *= (max_speed / actSpeed);
                   accelFactor *= 0.98f;
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
              else accumYpos = 512+(float)y*settings.ay/50;
              
              if (x==0) accumXpos = 512;
              else  accumXpos = 512+(float)x*settings.ax/50;
  
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
