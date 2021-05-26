/*
     FLipWare - AsTeRICS Foundation
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
#include "eeprom.h"

uint8_t actButton = 0;

extern void parseCommand (char * cmdstr);

const char ERRORMESSAGE_NOT_FOUND[] = "E: not found";

const struct atCommandType atCommands[] PROGMEM = {
  {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
  {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
  {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
  {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
  {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING}, {"KP"  , PARTYPE_STRING}, {"KR"  , PARTYPE_STRING},
  {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING}, {"LO"  , PARTYPE_STRING}, {"LA"  , PARTYPE_NONE },
  {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_STRING }, {"RS"  , PARTYPE_NONE },
  {"NC"  , PARTYPE_NONE },  {"MM"  , PARTYPE_UINT },
  {"SW"  , PARTYPE_NONE },  {"SR"  , PARTYPE_NONE }, {"ER"  , PARTYPE_NONE }, {"CA"  , PARTYPE_NONE },
  {"AX"  , PARTYPE_UINT },  {"AY"  , PARTYPE_UINT }, {"DX"  , PARTYPE_UINT }, {"DY"  , PARTYPE_UINT },
  {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"SP"  , PARTYPE_UINT }, {"SS"  , PARTYPE_UINT },
  {"GV"  , PARTYPE_UINT },  {"RV"  , PARTYPE_UINT }, {"GH"  , PARTYPE_UINT }, {"RH"  , PARTYPE_UINT },
  {"IR"  , PARTYPE_STRING}, {"IP"  , PARTYPE_STRING}, {"IC"  , PARTYPE_STRING}, {"IL"  , PARTYPE_NONE },
  {"JX"  , PARTYPE_INT  },  {"JY"  , PARTYPE_INT  }, {"JZ"  , PARTYPE_INT  },
  {"JT"  , PARTYPE_INT  },  {"JS"  , PARTYPE_INT  }, {"JP"  , PARTYPE_INT  }, {"JR"  , PARTYPE_INT  },
  {"JH"  , PARTYPE_INT  },  {"IT"  , PARTYPE_UINT  }, {"KH"  , PARTYPE_STRING}, {"MS"  , PARTYPE_UINT },
  {"AC"  , PARTYPE_UINT },  {"MA"  , PARTYPE_STRING}, {"WA"  , PARTYPE_UINT  }, {"RO"  , PARTYPE_UINT },
  {"IW"  , PARTYPE_NONE },  {"BT"  , PARTYPE_UINT }, {"HL"  , PARTYPE_NONE }, {"HR"  , PARTYPE_NONE },
  {"HM"  , PARTYPE_NONE },  {"TL"  , PARTYPE_NONE }, {"TR"  , PARTYPE_NONE }, {"TM"  , PARTYPE_NONE },
  {"KT"  , PARTYPE_STRING }, {"IH"  , PARTYPE_STRING }, {"IS"  , PARTYPE_NONE }, 
};

void printCurrentSlot()
{
  Serial.print("Slot:");  Serial.println(settings.slotName);
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
  Serial.print("AT GV "); Serial.println(settings.gv);
  Serial.print("AT RV "); Serial.println(settings.rv);
  Serial.print("AT GH "); Serial.println(settings.gh);
  Serial.print("AT RH "); Serial.println(settings.rh);
  Serial.print("AT RO "); Serial.println(settings.ro);
  Serial.print("AT BT "); Serial.println(settings.bt);

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    Serial.print("AT BM ");
    if (i < 9) Serial.print("0"); // leading zero for button numbers !
    Serial.println(i + 1);
    Serial.print("AT ");
    int actCmd = buttons[i].mode;
    char cmdStr[4];
        
    strcpy_FM(cmdStr, (uint_farptr_t_FM)atCommands[actCmd].atCmd);
    Serial.print(cmdStr);
    switch (pgm_read_byte_near(&(atCommands[actCmd].partype)))
    {
      case PARTYPE_UINT:
      case PARTYPE_INT:  Serial.print(" "); Serial.print(buttons[i].value); break;
      case PARTYPE_STRING: Serial.print(" "); Serial.print(buttonKeystrings[i]); break;
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
#ifdef DEBUG_OUTPUT_FULL
    Serial.print("got new mode for button "); Serial.print(actButton); Serial.print(":");
    Serial.print(cmd); Serial.print(","); Serial.print(par1); Serial.print(","); Serial.println(keystring);
#endif
    buttons[actButton - 1].mode = cmd;
    buttons[actButton - 1].value = par1;
    setButtonKeystring(actButton - 1, keystring);
    actButton = 0;
    return;  // do not actually execute the command (just store it)
  }

  switch (cmd) {
    case CMD_ID:
      Serial.println(VERSION_STRING);
      break;
    case CMD_BM:
      release_all();
#ifdef DEBUG_OUTPUT_FULL
      Serial.print("set mode for button "); Serial.println(par1);
#endif
      if ((par1 > 0) && (par1 <= NUMBER_OF_BUTTONS))
        actButton = par1;
      else  Serial.println("?");
      break;

    case CMD_CL:
      mousePress(MOUSE_LEFT);
      delay(DEFAULT_CLICK_TIME);
      mouseRelease(MOUSE_LEFT);
      break;
    case CMD_CR:
      mousePress(MOUSE_RIGHT);
      delay(DEFAULT_CLICK_TIME);
      mouseRelease(MOUSE_RIGHT);
      break;
    case CMD_CD:
      mousePress(MOUSE_LEFT);
      delay(DEFAULT_CLICK_TIME);
      mouseRelease(MOUSE_LEFT);
      delay(DEFAULT_CLICK_TIME);
      mousePress(MOUSE_LEFT);
      delay(DEFAULT_CLICK_TIME);
      mouseRelease(MOUSE_LEFT);
      break;
    case CMD_CM:
      mousePress(MOUSE_MIDDLE);
      delay(DEFAULT_CLICK_TIME);
      mouseRelease(MOUSE_MIDDLE);
      break;
    case CMD_PL:  // for compatibility to v2.5 and below
    case CMD_HL:
      mousePress(MOUSE_LEFT);
      break;
    case CMD_PR:  // for compatibility to v2.5 and below
    case CMD_HR:
      mousePress(MOUSE_RIGHT);
      break;
    case CMD_PM:  // for compatibility to v2.5 and below
    case CMD_HM:
      mousePress(MOUSE_MIDDLE);
      break;
    case CMD_TL:
      mouseToggle(MOUSE_LEFT);
      break;
    case CMD_TR:
      mouseToggle(MOUSE_RIGHT);
      break;
    case CMD_TM:
      mouseToggle(MOUSE_MIDDLE);
      break;
    case CMD_RL:
      mouseRelease(MOUSE_LEFT);
      break;
    case CMD_RR:
      mouseRelease(MOUSE_RIGHT);
      break;
    case CMD_RM:
      mouseRelease(MOUSE_MIDDLE);
      break;
    case CMD_WU:
      mouseScroll(-settings.ws);
      break;
    case CMD_WD:
      mouseScroll(settings.ws);
      break;
    case CMD_WS:
      settings.ws = par1;
      break;
    case CMD_MX:
      if (periodicMouseMovement) moveX = par1;
      else mouseMove(par1, 0);
      break;
    case CMD_MY:
      if (periodicMouseMovement) moveY = par1;
      else mouseMove(0, par1);
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
      Joystick.button(par1, 1);
      break;
    case CMD_JR:
      Joystick.button(par1, 0);
      break;
    case CMD_JH:
      Joystick.hat(par1);
      break;

    case CMD_KW:
      if (keystring) keyboardPrint(keystring);
      break;
    case CMD_KP:
      if (keystring) pressKeys(keystring);
      break;
    case CMD_KH:
      if (keystring) holdKeys(keystring);
      break;
    case CMD_KT:
      if (keystring) toggleKeys(keystring);
      break;
    case CMD_KR:
      if (keystring) releaseKeys(keystring);
      break;
    case CMD_RA:
      release_all();
      break;

    case CMD_SA:
      release_all();
      if (keystring) {
        if ((strlen(keystring) > 0) && (strlen(keystring) < MAX_NAME_LEN-1)) {
          strcpy (settings.slotName, keystring);  // store current slot name
          saveToEEPROM(keystring);
        }
        makeTone(TONE_INDICATE_PUFF, 0);
        Serial.println("OK");
      }
      break;
    case CMD_LO:
      if (keystring) {
        release_all();
        if (readFromEEPROM(keystring)) Serial.println("OK");
        else Serial.println(ERRORMESSAGE_NOT_FOUND);
      }
      break;
    case CMD_LA:
      release_all();
      printAllSlots();
      break;
    case CMD_LI:
      release_all();
      listSlots();
      Serial.println("OK");  // send AT command acknowledge
      break;
    case CMD_NE:
#ifdef DEBUG_OUTPUT_FULL
      Serial.print("load next slot");
#endif
      release_all();
      if (!readFromEEPROM("")) Serial.println(ERRORMESSAGE_NOT_FOUND);
      break;
    case CMD_DE:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("delete slots");
#endif
      release_all();
      if (deleteSlot(keystring))  Serial.println("OK");    // send AT command acknowledge      
      else Serial.println(ERRORMESSAGE_NOT_FOUND);
      break;
    case CMD_RS:
      deleteSlot(""); // delete all slots
      memcpy(&settings,&defaultSettings,sizeof(struct slotGeneralSettings)); //load default values from flash
      initButtons(); //reset buttons
      saveToEEPROM(settings.slotName); //save default slot to default name
      readFromEEPROM(""); //load this slot
      Serial.println("OK");    // send AT command acknowledge
      break;
    case CMD_NC:
      break;

    case CMD_MM:
      settings.stickMode = par1;
#ifdef DEBUG_OUTPUT_FULL
      if (settings.stickMode == STICKMODE_MOUSE)
        Serial.println("mouse function activated");
      else if (settings.stickMode >= STICKMODE_JOYSTICK_XY)
        Serial.println("joystick function activated");
      else Serial.println("alternative functions activated");
#endif
      break;
    case CMD_SW:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("switch mouse / alternative function");
#endif
      initBlink(6, 15);
      if (settings.stickMode == STICKMODE_ALTERNATIVE)  settings.stickMode = STICKMODE_MOUSE;
      else settings.stickMode = STICKMODE_ALTERNATIVE;
      break;
    case CMD_SR:
      reportRawValues = 1;
      break;
    case CMD_ER:
      reportRawValues = 0;
      break;
    case CMD_CA:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("start calibration");
#endif
      initBlink(10, 20);
      calib_now = 100;
      makeTone(TONE_CALIB, 0);
      break;
    case CMD_AX:
      settings.ax = par1;
      break;
    case CMD_AY:
      settings.ay = par1;
      break;
    case CMD_DX:
      settings.dx = par1;
      break;
    case CMD_DY:
      settings.dy = par1;
      break;
    case CMD_MS:
      settings.ms = par1;
      break;
    case CMD_AC:
      settings.ac = par1;
      break;
    case CMD_MA:
      {
        char current[MAX_KEYSTRING_LEN];  // TBD: save memory here via improved command extraction ...
        char *cmd_copy_ptr, backslash;
        uint8_t len;
#ifdef DEBUG_OUTPUT_FULL
        Serial.print("execute macro:"); Serial.println(keystring);
#endif

        // do the macro stuff: feed single commands to parser, seperator: ';'
        cmd_copy_ptr = keystring;
        while (*cmd_copy_ptr)
        {
          len = 0; backslash = 0;
          while ((*cmd_copy_ptr) && ((*cmd_copy_ptr != ';') || backslash) && (len < MAX_KEYSTRING_LEN - 1))
          {
            if ((*cmd_copy_ptr == '\\') && (!backslash))   // check for escape character
              backslash = 1;
            else  {
              current[len++] = *cmd_copy_ptr;
              backslash = 0;
            }
            cmd_copy_ptr++;
          }
          current[len] = 0;
          parseCommand(current);
          if (*cmd_copy_ptr) cmd_copy_ptr++;
        }
      }
      break;
    case CMD_WA:
      delay(par1);
      break;
    case CMD_TS:
      settings.ts = par1;
      break;
    case CMD_TP:
      settings.tp = par1;
      break;
    case CMD_SP:
      settings.sp = par1;
      break;
    case CMD_SS:
      settings.ss = par1;
      break;
    case CMD_GV:
      settings.gv = par1;
      break;
    case CMD_RV:
      settings.rv = par1;
      break;
    case CMD_GH:
      settings.gh = par1;
      break;
    case CMD_RH:
      settings.rh = par1;
      break;
    case CMD_RO:
      settings.ro = par1;
      break;
    case CMD_BT:
      settings.bt = par1;
      break;

    case CMD_IR:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("record IR command");
#endif
      if (keystring) {
        if ((strlen(keystring) > 0) && (strlen(keystring) < MAX_NAME_LEN-1))
          record_IR_command(keystring);
      }
      break;
    case CMD_IP:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("play IR command");
#endif
      if (keystring)
        play_IR_command(keystring);
      break;
    case CMD_IH:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("hold IR command");
#endif
      if (keystring) {
        if ((strlen(keystring) > 0) && (strlen(keystring) < MAX_NAME_LEN))
          hold_IR_command(keystring);
      }
      break;
    case CMD_IS:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("stop IR command");
#endif
      stop_IR_command();
      break;
    case CMD_IL:
      list_IR_commands();
      Serial.println("OK");  // send AT command acknowledge
      break;
    case CMD_IC:
      if (keystring) {
        if (delete_IR_command(keystring)) Serial.println("OK");  // send AT command acknowledge
        else Serial.println(ERRORMESSAGE_NOT_FOUND);
      }
      break;
    case CMD_IT:
      set_IR_timeout(par1);
      break;
    case CMD_IW:
      wipe_IR_commands();
      Serial.println("OK");  // send AT command acknowledge
      break;
  }
}
