  
/* 
      FLipWare - AsTeRICS Academy 2016
     For more info please visit: http://www.asterics-academy.net

     Module: buttons.cpp - implementation of the button handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License: 
   http://www.gnu.org/licenses/gpl-3.0.en.html

 */
 

#include "FlipWare.h"        //  FABI command definitions

// Constants and Macro definitions

#define DEFAULT_DEBOUNCING_TIME 7   // debouncing interval for button-press / release


struct slotButtonSettings buttons [NUMBER_OF_BUTTONS];   // array for all buttons - type definition see FlipWare.h 
char keystringButton[NUMBER_OF_BUTTONS][MAX_KEYSTRING_LEN] = {"","","","","","","","","","","",""};
struct buttonDebouncerType buttonDebouncers [NUMBER_OF_BUTTONS];   // array for all buttonsDebouncers - type definition see fabi.h 


void initButtons() {
     buttons[0].mode=CMD_NE;  // default function for first button: switch to next slot
     buttons[1].mode=CMD_KP; strcpy(keystringButton[1],"KEY_ESC ");;
     buttons[2].mode=CMD_NC;  // no command
     buttons[3].mode=CMD_KP; strcpy(keystringButton[3],"KEY_UP ");
     buttons[4].mode=CMD_KP; strcpy(keystringButton[4],"KEY_DOWN ");
     buttons[5].mode=CMD_KP; strcpy(keystringButton[5],"KEY_LEFT ");
     buttons[6].mode=CMD_KP; strcpy(keystringButton[6],"KEY_RIGHT ");
     buttons[7].mode=CMD_PL;   // press left mouse button
     buttons[8].mode=CMD_NC;   // no command 
     buttons[9].mode=CMD_CR;   // click right                        
     buttons[10].mode=CMD_CA;  // calibrate      
     buttons[11].mode=CMD_NC;  // no command      
     buttons[12].mode=CMD_NC;      
     buttons[13].mode=CMD_NC;      
     buttons[14].mode=CMD_NC;      
     buttons[15].mode=CMD_NC;      
     buttons[16].mode=CMD_NC;      
     buttons[17].mode=CMD_NC;      
     buttons[18].mode=CMD_NC;      
}


void handlePress (int buttonIndex)   // a button was pressed
{   
    performCommand(buttons[buttonIndex].mode,buttons[buttonIndex].value,keystringButton[buttonIndex],1);
}

void handleRelease (int buttonIndex)    // a button was released: deal with "sticky"-functions
{
   switch(buttons[buttonIndex].mode) {
     case CMD_PL: 
               Mouse.release(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<0);
               break;
     case CMD_PR:
               Mouse.release(MOUSE_RIGHT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<1);
               break;
     case CMD_PM:
               Mouse.release(MOUSE_MIDDLE); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<2);
               break;
     case CMD_JP: Joystick.button(buttons[buttonIndex].value,0); break;
     case CMD_MX: moveX=0; break;      
     case CMD_MY: moveY=0; break;      
     case CMD_KP: releaseKeys(keystringButton[buttonIndex]); break; 
   }
}
  

uint8_t handleButton(int i, uint8_t state)    // button debouncing and longpress detection  
{                                                 //   (if button i is pressed long and index l>=0, virtual button l is activated !)
   if ( buttonDebouncers[i].bounceState == state) {
     if (buttonDebouncers[i].bounceCount < DEFAULT_DEBOUNCING_TIME) {
       buttonDebouncers[i].bounceCount++;
       if (buttonDebouncers[i].bounceCount == DEFAULT_DEBOUNCING_TIME) {
        
          if (state != buttonDebouncers[i].stableState)  // entering stable state
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

uint8_t inHoldMode (int i)
{
   if ((buttons[i].mode == CMD_PL) ||
       (buttons[i].mode == CMD_PR) || 
       (buttons[i].mode == CMD_PM) || 
       (buttons[i].mode == CMD_JP) || 
       (buttons[i].mode == CMD_MX) || 
       (buttons[i].mode == CMD_MY) || 
       (buttons[i].mode == CMD_KP))
   return(1);
   else return(0); 
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



