/*
     FLipWare - AsTeRICS Foundation 2017
     For more info please visit: http://www.asterics-academy.net

     Module: commands.cpp - implementation of the AT-commands
     For a description of the supported commands see: commands.h

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License: 
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"

uint8_t actButton=0;

extern void parseCommand (char * cmdstr);

const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE }, 
    {"E1"  , PARTYPE_NONE },  {"E0"  , PARTYPE_NONE }, {"MM"  , PARTYPE_UINT },  
    {"SW"  , PARTYPE_NONE },  {"SR"  , PARTYPE_NONE }, {"ER"  , PARTYPE_NONE }, {"CA"  , PARTYPE_NONE },  
    {"AX"  , PARTYPE_UINT },  {"AY"  , PARTYPE_UINT }, {"DX"  , PARTYPE_UINT }, {"DY"  , PARTYPE_UINT },  
    {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"SP"  , PARTYPE_UINT }, {"SS"  , PARTYPE_UINT },  
    {"GU"  , PARTYPE_UINT },  {"GD"  , PARTYPE_UINT }, {"GL"  , PARTYPE_UINT }, {"GR"  , PARTYPE_UINT },
    {"IR"  , PARTYPE_STRING}, {"IP"  , PARTYPE_STRING},{"IC"  , PARTYPE_STRING},{"IL"  , PARTYPE_NONE },
    {"E2"  , PARTYPE_NONE },  {"JX"  , PARTYPE_INT  }, {"JY"  , PARTYPE_INT  }, {"JZ"  , PARTYPE_INT  }, 
    {"JT"  , PARTYPE_INT  },  {"JS"  , PARTYPE_INT  }, {"JP"  , PARTYPE_INT  }, {"JR"  , PARTYPE_INT  },
    {"JH"  , PARTYPE_INT  },  {"IT"  , PARTYPE_UINT  },{"KH"  , PARTYPE_STRING},{"MS"  , PARTYPE_UINT },
    {"AC"  , PARTYPE_UINT },  {"MA"  , PARTYPE_STRING},{"WA"  , PARTYPE_UINT  },{"RO"  , PARTYPE_UINT },
};

void printCurrentSlot()
{
        Serial.print("Slot:");  Serial.println(slotName);
        Serial.print("AT AX "); Serial.println(settings.ax); 
        Serial.print("AT AY "); Serial.println(settings.ay);
        Serial.print("AT DX "); Serial.println(settings.dx);
        Serial.print("AT DY "); Serial.println(settings.dy);
        Serial.print("AT MS "); Serial.println(settings.ms);
        Serial.print("AT AC "); Serial.println(settings.ac);
        Serial.print("AT TS "); Serial.println(settings.ts);
        Serial.print("AT TP "); Serial.println(settings.tp);
        Serial.print("AT WS "); Serial.println(settings.ws);
        Serial.print("AT SP "); Serial.println(settings.sp);
        Serial.print("AT SS "); Serial.println(settings.ss);
        Serial.print("AT MM "); Serial.println(settings.stickMode);
        Serial.print("AT GU "); Serial.println(settings.gu);
        Serial.print("AT GD "); Serial.println(settings.gd);
        Serial.print("AT GL "); Serial.println(settings.gl);
        Serial.print("AT GR "); Serial.println(settings.gr);
        Serial.print("AT RO "); Serial.println(settings.ro);
        
        for (int i=0;i<NUMBER_OF_BUTTONS;i++) 
        {
           Serial.print("AT BM "); 
           if (i<9) Serial.print("0");
           Serial.println(i+1); 
           Serial.print("AT "); 
           int actCmd = buttons[i].mode;
           char cmdStr[4];
           strcpy_FM(cmdStr,(uint_farptr_t_FM)atCommands[actCmd].atCmd);
           Serial.print(cmdStr);
            switch (pgm_read_byte_near(&(atCommands[actCmd].partype))) 
            {
               case PARTYPE_UINT: 
               case PARTYPE_INT:  Serial.print(" ");Serial.print(buttons[i].value); break;
               case PARTYPE_STRING: Serial.print(" ");Serial.print(keystringButtons[i]); break;
            }
            Serial.println("");
        }
}


  

// perform a command  (called from parser.cpp)
//   cmd: command identifier
//   par1: optional numeric parameter
//   periodicMouseMovement: if true, mouse will continue moving - if false: only one movement
void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement)
{
    if (actButton != 0)  // if last command was BM (set buttonmode): store current command for this button !!
    {
        if (DebugOutput==DEBUG_FULLOUTPUT)
        {  
          Serial.print("got new mode for button "); Serial.print(actButton);Serial.print(":");
          Serial.print(cmd);Serial.print(",");Serial.print(par1);Serial.print(",");Serial.println(keystring);
        }
        buttons[actButton-1].mode=cmd;
        buttons[actButton-1].value=par1;
        deleteKeystringButton(actButton-1);
        if (keystring!=0) storeKeystringButton(actButton-1,keystring);
        actButton=0;
        return;  // do not actually execute the command (just store it)
    }
    
    switch(cmd) {
        case CMD_ID:
               Serial.println(VERSION_STRING); 
            break;
        case CMD_BM:
               release_all();
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("set mode for button "); Serial.println(par1); }
               if ((par1>0) && (par1<=NUMBER_OF_BUTTONS))
                   actButton=par1;
               else  Serial.println("?");
            break;
        
        case CMD_CL:
               Mouse.press(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTPress(1<<0);
               delay(DEFAULT_CLICK_TIME);
               Mouse.release(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<0);
               break;
        case CMD_CR:
               Mouse.press(MOUSE_RIGHT); 
               if(isBluetoothAvailable()) mouseBTPress(1<<1);
               delay(DEFAULT_CLICK_TIME);
               Mouse.release(MOUSE_RIGHT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<1);
               break;
        case CMD_CD:
               Mouse.press(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTPress(1<<0);
               delay(DEFAULT_CLICK_TIME);
               Mouse.release(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<0);
               delay(DEFAULT_CLICK_TIME);
               Mouse.press(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTPress(1<<0);
               delay(DEFAULT_CLICK_TIME);
               Mouse.release(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<0);
               break;
        case CMD_CM:
               Mouse.press(MOUSE_MIDDLE); 
               if(isBluetoothAvailable()) mouseBTPress(1<<2);
               delay(DEFAULT_CLICK_TIME);
               Mouse.release(MOUSE_MIDDLE); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<2);
              break;
        case CMD_PL:
               Mouse.press(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTPress(1<<0);
               break;
        case CMD_PR:
               Mouse.press(MOUSE_RIGHT); 
               if(isBluetoothAvailable()) mouseBTPress(1<<1);
               break;
        case CMD_PM:
               Mouse.press(MOUSE_MIDDLE); 
               if(isBluetoothAvailable()) mouseBTPress(1<<2);
               break;
        case CMD_RL:
               Mouse.release(MOUSE_LEFT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<0);
               break; 
        case CMD_RR:
               Mouse.release(MOUSE_RIGHT); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<1);
               break; 
        case CMD_RM:
               Mouse.release(MOUSE_MIDDLE); 
               if(isBluetoothAvailable()) mouseBTRelease(1<<2);
               break; 
        case CMD_WU:
                 Mouse.scroll(-settings.ws);
                 if(isBluetoothAvailable()) mouseBT(0,0,-settings.ws);
            break;
        case CMD_WD:
                 Mouse.scroll(settings.ws); 
                 if(isBluetoothAvailable()) mouseBT(0,0,settings.ws);
            break;
        case CMD_WS:
               settings.ws=par1;
            break;
        case CMD_MX:
               if (periodicMouseMovement) moveX=par1;
               else {
                 while (par1<-128) { Mouse.move(-128, 0); par1+=128; }
                 while (par1>127) { Mouse.move(127, 0); par1-=127; }
                 Mouse.move(par1, 0);
                 if(isBluetoothAvailable()) mouseBT(par1,0,0);
               }
            break;
        case CMD_MY:
               if (periodicMouseMovement) moveY=par1;
               else {
                 while (par1<-128) { Mouse.move(0, -128); par1+=128; }
                 while (par1>127) { Mouse.move(0, 127); par1-=127; }
                 Mouse.move(0, par1);
                 if(isBluetoothAvailable()) mouseBT(0,par1,0);
               }
            break;
        case CMD_JX:
               Joystick.X(par1);
            break;
        case CMD_JY:
               Joystick.Y(par1);
            break;
        case CMD_JZ:
               Joystick.Z(par1);
            break;
        case CMD_JT:
               Joystick.Zrotate(par1);
            break;
        case CMD_JS:
               Joystick.sliderLeft(par1);
            break;
        case CMD_JP:
               Joystick.button(par1,1);
            break;
        case CMD_JR:
               Joystick.button(par1,0);
            break;
        case CMD_JH:
               Joystick.hat(par1);
            break;
        
        case CMD_KW:
               //Keyboard.print(keystring);
               //Serial.println("keyboard write");
               for (int i=0; i<strlen(keystring); i++)
               {
                  // Serial.println(keystring[i]);
                  Keyboard.press(keystring[i]);
                  Keyboard.release(keystring[i]);
               }
               if(isBluetoothAvailable()) keyboardBTPrint(keystring);  // TODO: check ISO8859-compatibility
               break;
        case CMD_KP:
               if (keystring[strlen(keystring)-1] != ' ') strcat(keystring," ");
               pressKeys(keystring);
               break;
        case CMD_KH:
               if (keystring[strlen(keystring)-1] != ' ') strcat(keystring," ");
               holdKeys(keystring);
               break;
        case CMD_KR:
               strcat(keystring," ");
               releaseKeys(keystring);             
               break;
        case CMD_RA:
               release_all();             
               break;
              
        case CMD_SA:
               release_all();
               saveToEEPROM(keystring); 
            break;
        case CMD_LO:
               if (keystring) {
                 release_all();
                 // reportSlotParameters=REPORT_ONE_SLOT;
                 readFromEEPROM(keystring);
                 reportSlotParameters=REPORT_NONE;
                 if ((settings.gu!=50)||(settings.gd!=50)||(settings.gl!=50)||(settings.gr!=50)) // TBD: improve ! 
                 { cx=settings.cx; cy=settings.cy; }   // update calibration settings if gain settings are not default 
               }
            break;
        case CMD_LA:
               release_all();
               reportSlotParameters=REPORT_ALL_SLOTS;
               //necessary, because we load via slot numbers
               bootstrapSlotAddresses();
               for(uint8_t i = 0; i<EEPROM_COUNT_SLOTS; i++)
               {
        					readFromEEPROMSlotNumber(i,false);
				       }
               reportSlotParameters=REPORT_NONE;
               readFromEEPROMSlotNumber(0,true);
            break;
        case CMD_LI:
               release_all();
               listSlots();
            break;
        case CMD_NE:
               if (DebugOutput==DEBUG_FULLOUTPUT)  {
                 Serial.print("load next slot");
                 reportSlotParameters=REPORT_ONE_SLOT;
               }
               release_all();
               readFromEEPROM(0);
               reportSlotParameters=REPORT_NONE;
               break;
        case CMD_DE:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("delete slots"); 
               release_all();
               // deleteIRCommand(0);      // removed to keep IR commands !
               deleteSlots();
            break;
        case CMD_NC:
            break;

        case CMD_MM:
               settings.stickMode=par1;
               if (DebugOutput==DEBUG_FULLOUTPUT)
               {  
                 if (settings.stickMode==STICKMODE_MOUSE)
                   Serial.println("mouse function activated");
                 else if(settings.stickMode>=STICKMODE_JOYSTICK_XY) 
                   Serial.println("joystick function activated");
                 else Serial.println("alternative functions activated");
               }
            break;
        case CMD_SW:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("switch mouse / alternative function");
               initBlink(6,15);
               if (settings.stickMode==STICKMODE_ALTERNATIVE)  settings.stickMode=STICKMODE_MOUSE;
               else settings.stickMode=STICKMODE_ALTERNATIVE; 
            break;
        case CMD_SR:
              reportRawValues=1;
            break;
        case CMD_ER:
              reportRawValues=0;
            break;
        case CMD_CA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("start calibration");
               initBlink(10,20);
               calib_now=100;
               makeTone(TONE_CALIB,0);
            break;
        case CMD_AX:
               settings.ax=par1;
            break;
        case CMD_AY:
               settings.ay=par1;
            break;
        case CMD_DX:
               settings.dx=par1;
            break;
        case CMD_DY:
               settings.dy=par1;
            break;
        case CMD_MS:
               settings.ms=par1;
            break;
        case CMD_AC:
               settings.ac=par1;
            break;
        case CMD_MA:
               {
                 char current[MAX_KEYSTRING_LEN], *cmd_copy_ptr, backslash;
                 uint8_t len;
                 if (DebugOutput==DEBUG_FULLOUTPUT)  
                 {  Serial.print("execute macro:"); Serial.println(keystring); }

                 // do the macro stuff: feed single commands to parser, seperator: ';'
                 cmd_copy_ptr=keystring;
                 while (*cmd_copy_ptr)
                 {
                    len=0;backslash=0;
                    while ((*cmd_copy_ptr) && ((*cmd_copy_ptr != ';') || backslash) && (len<MAX_KEYSTRING_LEN-1))
                    {
                       if ((*cmd_copy_ptr == '\\') && (!backslash))   // check for escape character
                         backslash=1; 
                       else  {
                        current[len++] = *cmd_copy_ptr;
                        backslash=0;
                      }
                      cmd_copy_ptr++;
                    }
                    current[len]=0; 
                    parseCommand(current);
                    if (*cmd_copy_ptr) cmd_copy_ptr++;
                 }
               }
               break;
        case CMD_WA:
                delay(par1);
               break;
        case CMD_TS:
               settings.ts=par1;
            break;
        case CMD_TP:
               settings.tp=par1;
            break;
        case CMD_SP:
               settings.sp=par1;
            break;
        case CMD_SS:
               settings.ss=par1;
            break;
        case CMD_GU:
               settings.gu=par1;
            break;
        case CMD_GD:
               settings.gd=par1;
            break;
        case CMD_GL:
               settings.gl=par1;
            break;
        case CMD_GR:
               settings.gr=par1;
            break;
        case CMD_RO:
               settings.ro=par1;
            break;

        case CMD_IR:
    				if (DebugOutput==DEBUG_FULLOUTPUT) Serial.println("record IR command");
    				record_IR_command(keystring);
            break;
        case CMD_IP:
    				if (DebugOutput==DEBUG_FULLOUTPUT) Serial.println("play IR command");
    				play_IR_command(keystring);
            break;
        case CMD_IL:
    				list_IR_commands();
            break;
        case CMD_IC:
    				delete_IR_command(keystring);
            break;
        case CMD_IT:
            set_IR_timeout(par1);
            break;

        case CMD_E2:
      			DebugOutput=DEBUG_FULLOUTPUT; 
	      		eepromDebugLevel = EEPROM_FULL_DEBUG;
            Serial.println("extended debug echo on"); 
      			break;
        case CMD_E1:
            DebugOutput=DEBUG_FULLOUTPUT; 
            eepromDebugLevel = EEPROM_BASIC_DEBUG;
            Serial.println("echo on"); 
            break;
        case CMD_E0:
            DebugOutput=DEBUG_NOOUTPUT; 
            eepromDebugLevel = EEPROM_NO_DEBUG;
            break;
     }
}

