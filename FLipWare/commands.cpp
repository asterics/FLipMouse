
#include "FlipWare.h"

uint8_t actButton=0;


const struct atCommandType atCommands[] PROGMEM = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE }, 
    {"E1"  , PARTYPE_NONE },  {"E0"  , PARTYPE_NONE }, {"MM"  , PARTYPE_UINT }, {"SW"  , PARTYPE_NONE }, 
    {"SR"  , PARTYPE_NONE },  {"ER"  , PARTYPE_NONE }, {"CA"  , PARTYPE_NONE }, {"AX"  , PARTYPE_UINT }, 
    {"AY"  , PARTYPE_UINT },  {"DX"  , PARTYPE_UINT }, {"DY"  , PARTYPE_UINT }, {"TS"  , PARTYPE_UINT }, 
    {"TP"  , PARTYPE_UINT },  {"SM"  , PARTYPE_UINT }, {"HM"  , PARTYPE_UINT }, {"GU"  , PARTYPE_UINT }, 
    {"GD"  , PARTYPE_UINT },  {"GL"  , PARTYPE_UINT }, {"GR"  , PARTYPE_UINT }, {"IR"  , PARTYPE_STRING},
    {"IP"  , PARTYPE_STRING}, {"IC"  , PARTYPE_STRING},{"IL"  , PARTYPE_NONE }
};

void initButtons() {
     buttons[0].mode=CMD_NE;  // default function for first button: switch to next slot
     buttons[1].mode=CMD_KP; strcpy(buttons[1].keystring,"KEY_ESC ");;
     buttons[2].mode=CMD_NC;  // no command
     buttons[3].mode=CMD_KP; strcpy(buttons[3].keystring,"KEY_UP ");
     buttons[4].mode=CMD_KP; strcpy(buttons[4].keystring,"KEY_DOWN ");
     buttons[5].mode=CMD_KP; strcpy(buttons[5].keystring,"KEY_LEFT ");
     buttons[6].mode=CMD_KP; strcpy(buttons[6].keystring,"KEY_RIGHT ");
     buttons[7].mode=CMD_PL;   // press left mouse button
     buttons[8].mode=CMD_NC;   // no command 
     buttons[9].mode=CMD_CR;   // click right                        
     buttons[10].mode=CMD_CA;  // calibrate      
     buttons[11].mode=CMD_NC;  // no command      
}


void printCurrentSlot()
{
        Serial.print("Slot:");  Serial.println(settings.slotname);
        Serial.print("AT AX "); Serial.println(settings.ax); 
        Serial.print("AT AY "); Serial.println(settings.ay);
        Serial.print("AT DX "); Serial.println(settings.dx);
        Serial.print("AT DY "); Serial.println(settings.dy);
        Serial.print("AT TS "); Serial.println(settings.ts);
        Serial.print("AT TP "); Serial.println(settings.tp);
        Serial.print("AT WS "); Serial.println(settings.ws);
        Serial.print("AT SM "); Serial.println(settings.sm);
        Serial.print("AT HM "); Serial.println(settings.hm);
        Serial.print("AT MM "); Serial.println(settings.mouseOn);
        Serial.print("AT GU "); Serial.println(settings.gu);
        Serial.print("AT GD "); Serial.println(settings.gd);
        Serial.print("AT GL "); Serial.println(settings.gl);
        Serial.print("AT GR "); Serial.println(settings.gr);
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
               case PARTYPE_STRING: Serial.print(" ");Serial.print(buttons[i].keystring); break;
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
        if (keystring==0) buttons[actButton-1].keystring[0]=0;
        else strcpy(buttons[actButton-1].keystring,keystring);
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
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click left");
               leftMouseButton=1;  leftClickRunning=DEFAULT_CLICK_TIME;
               break;
        case CMD_CR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click right");
               rightMouseButton=1; rightClickRunning=DEFAULT_CLICK_TIME;
               break;
        case CMD_CD:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click double");
               leftMouseButton=1;  doubleClickRunning=DEFAULT_CLICK_TIME*DOUBLECLICK_MULTIPLIER;
               break;
        case CMD_CM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("click middle");
               middleMouseButton=1; middleClickRunning=DEFAULT_CLICK_TIME;
              break;
        case CMD_PL:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("press left");
               leftMouseButton=1; 
               break;
        case CMD_PR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("press right");
               rightMouseButton=1; 
               break;
        case CMD_PM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("press middle");
               middleMouseButton=1; 
               break;
        case CMD_RL:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("release left");
               leftMouseButton=0;
               break; 
        case CMD_RR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("release right");
               rightMouseButton=0;
               break; 
        case CMD_RM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("release middle");
               middleMouseButton=0;
               break; 
        case CMD_WU:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("wheel up");
                 Mouse.scroll(-settings.ws); 
            break;
        case CMD_WD:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("wheel down");
                 Mouse.scroll(settings.ws); 
            break;
        case CMD_WS:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("wheel step");
               settings.ws=par1;
            break;
        case CMD_MX:
               if (DebugOutput==DEBUG_FULLOUTPUT) 
               {  Serial.print("mouse move x "); Serial.println(par1); }
               Mouse.move(par1, 0);
               if (periodicMouseMovement) moveX=par1;
            break;
        case CMD_MY:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("mouse move y "); Serial.println(par1); }
               Mouse.move(0, par1);
               if (periodicMouseMovement) moveY=par1;
            break;
        case CMD_KW:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("keyboard write: "); Serial.println(keystring); }
               writeKeystring=keystring;
               break;
        case CMD_KP:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("key press: "); Serial.println(keystring); }
               if (keystring[strlen(keystring)-1] != ' ') strcat(keystring," ");
               setKeyValues(keystring);
               break;
        case CMD_KR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("key release: ");  Serial.println(keystring); }
               strcat(keystring," ");
               releaseKeys(keystring);             
               break;
        case CMD_RA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.print("release all");
               release_all();             
               break;
              
        case CMD_SA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("save slot ");  Serial.println(keystring); }
               release_all();
               saveToEEPROM(keystring); 
            break;
        case CMD_LO:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
               {  Serial.print("load slot: "); Serial.println(keystring); }
               if (keystring) {
                 release_all();
                 reportSlotParameters=REPORT_ONE_SLOT;
                 readFromEEPROM(keystring);
                 reportSlotParameters=REPORT_NONE;
                 if ((settings.gu!=50)||(settings.gd!=50)||(settings.gl!=50)||(settings.gr!=50)) // TBD: improve ! 
                 { cx=settings.cx; cy=settings.cy; }   // update calibration settings if gain settings are not default 
               }
            break;
        case CMD_LA:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("laod all slots");
               release_all();
               reportSlotParameters=REPORT_ALL_SLOTS;
               readFromEEPROM(keystring);
               reportSlotParameters=REPORT_NONE;
               readFromEEPROM(0);
            break;
        case CMD_LI:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("list slots: ");
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
               deleteSlots(); 
            break;
        case CMD_NC:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("no command"); 
            break;
    
        case CMD_MM:
               settings.mouseOn=par1;
               if (DebugOutput==DEBUG_FULLOUTPUT)
               {  
                 if (settings.mouseOn)
                   Serial.println("mouse function on");
                   else Serial.println("alternative functions on");
               }
            break;
        case CMD_SW:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("switch mouse / alternative function");
               initBlink(6,15);
               if (settings.mouseOn==0)  settings.mouseOn=1;
               else settings.mouseOn=0;
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
                 Serial.print("set deadzone x");
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
        case CMD_SM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("set special threshold");
               settings.sm=par1;
            break;
        case CMD_HM:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("set hold threshold");
               settings.hm=par1;
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
        case CMD_IR:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("record IR command");
				
				record_IR_command();
                // TBD
            break;
        case CMD_IP:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("play IR command");
				 
				 play_IR_command();
                // TBD
            break;
        case CMD_IL:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("list IR commands");
                // TBD
            break;
        case CMD_IC:
               if (DebugOutput==DEBUG_FULLOUTPUT)  
                 Serial.println("delete IR command");
                // TBD
            break;
        case CMD_E1:
               DebugOutput=DEBUG_FULLOUTPUT; 
               Serial.println("echo on"); 
            break;
        case CMD_E0:
               DebugOutput=DEBUG_NOOUTPUT; 
            break;
     }
}




