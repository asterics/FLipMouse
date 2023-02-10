/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

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
#include "display.h"
#include "infrared.h"
#include "gpio.h"
#include "tone.h"
#include "modes.h"
#include "keys.h"
#include "parser.h"
#include "reporting.h"
#include "sensors.h"
#include "utils.h"
#include <hardware/watchdog.h>

/**
   atCommands this is the array containing all supported AT commands,
   it consists of the AT command identifier (e.g. "ID" for command "AT ID")
   and the identifier for the parameter data type of this command (e.g. PARTYPE_STRING)
*/
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
  {"KT"  , PARTYPE_STRING }, {"IH"  , PARTYPE_STRING }, {"IS"  , PARTYPE_NONE }, {"UG", PARTYPE_NONE },
  {"BC"  , PARTYPE_STRING}, {"KL"  , PARTYPE_STRING }, {"BR"  , PARTYPE_UINT }, {"RE"  , PARTYPE_NONE },
  {"SB"  , PARTYPE_UINT },  {"SC"  , PARTYPE_STRING },
};

/**
   error messages for handling EEPROM problems
*/
const char ERRORMESSAGE_NOT_FOUND[] = "E: not found";
const char ERRORMESSAGE_EEPROM_FULL[] = "E: eeprom full";


/**
   @name performCommand (called from parser.cpp)
   @brief performs a particular action/AT command
   @param cmd AT command identifier
   @param par1 numeric parameter for the command
   @param keystring string parameter for the command
   @param periodicMouseMovement if true, mouse will continue moving after action, otherwise only one movement
   @return none
*/
void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement)
{
  static uint8_t actButton = 0;   // to remember the button number for the button-mode-assignment AT command "AT BM"
  
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
      Serial.print(moduleName); Serial.print(" ");
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
      mouseScroll(-slotSettings.ws);
      break;
    case CMD_WD:
      mouseScroll(slotSettings.ws);
      break;
    case CMD_WS:
      slotSettings.ws = par1;
      break;
    case CMD_MX:
      if (periodicMouseMovement) sensorData.autoMoveX = par1;
      else mouseMove(par1, 0);
      break;
    case CMD_MY:
      if (periodicMouseMovement) sensorData.autoMoveY = par1;
      else mouseMove(0, par1);
      break;
    case CMD_JX:
      joystickAxis(par1,-1,0);
      break;
    case CMD_JY:
      joystickAxis(-1,par1,0);
      break;
    case CMD_JZ:
      joystickAxis(par1,-1,1);
      break;
    case CMD_JT:
      joystickAxis(-1,par1,0);
      break;
    case CMD_JS:
      joystickAxis(par1,-1,0);
      break;
    case CMD_JP:
      joystickButton(par1, 1);
      break;
    case CMD_JR:
      joystickButton(par1, 0);
      break;
    case CMD_JH:
      joystickHat(par1);
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
    case CMD_KL:
      //change keyboard layout.
      if(strnlen(keystring,5) == 5) {
        if(setKeyboardLayout(keystring)) {
          strncpy(slotSettings.kbdLayout, keystring, 5);
        } else Serial.println("NOK: supported layouts: de_DE, en_US, es_ES, fr_FR, it_IT, sv_SE, da_DK");
      } else { 
        printKeyboardLayout(); 
      }
      break;
    case CMD_RA:
      release_all();
      break;

    case CMD_SA:
      release_all();
      if (keystring) {
        if ((strlen(keystring) > 0) && (strlen(keystring) < MAX_NAME_LEN-1)) {
          strcpy (slotSettings.slotName, keystring);  // store current slot name
          if (saveToEEPROM(keystring)) Serial.println("OK");
          else Serial.println(ERRORMESSAGE_EEPROM_FULL);
        }
        makeTone(TONE_INDICATE_PUFF, 0);
        
      }
      break;
    case CMD_LO:
      if (keystring) {
        release_all();
        if (readFromEEPROM(keystring)) Serial.println("OK");
        else Serial.println(ERRORMESSAGE_NOT_FOUND);
        displayUpdate();
        setKeyboardLayout(slotSettings.kbdLayout);
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
      displayUpdate();
      setKeyboardLayout(slotSettings.kbdLayout);
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
      memcpy(&slotSettings,&defaultSlotSettings,sizeof(struct SlotSettings)); //load default values from flash
      initButtons(); //reset buttons
      saveToEEPROM(slotSettings.slotName); //save default slot to default name
      readFromEEPROM(""); //load this slot
      setKeyboardLayout(slotSettings.kbdLayout);
      Serial.println("OK");    // send AT command acknowledge
      break;
    case CMD_RE:
      watchdog_reboot(0, 0, 10);
      while (1) { continue; }     
      break;
    case CMD_NC:
      break;

    case CMD_MM:
      slotSettings.stickMode = par1;
      displayUpdate();
      
#ifdef DEBUG_OUTPUT_FULL
      if (slotSettings.stickMode == STICKMODE_MOUSE)
        Serial.println("mouse function activated");
      else if (slotSettings.stickMode >= STICKMODE_JOYSTICK_XY)
        Serial.println("joystick function activated");
      else Serial.println("alternative functions activated");
#endif
      break;
    case CMD_SW:
#ifdef DEBUG_OUTPUT_FULL
      Serial.println("switch mouse / alternative function");
#endif
      initBlink(6, 15);
      if (slotSettings.stickMode == STICKMODE_ALTERNATIVE)  slotSettings.stickMode = STICKMODE_MOUSE;
      else slotSettings.stickMode = STICKMODE_ALTERNATIVE;
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
      sensorValues.calib_now = CALIBRATION_PERIOD;
      makeTone(TONE_CALIB, 0);
      break;
    case CMD_AX:
      slotSettings.ax = par1;
      break;
    case CMD_AY:
      slotSettings.ay = par1;
      break;
    case CMD_DX:
      slotSettings.dx = par1;
      break;
    case CMD_DY:
      slotSettings.dy = par1;
      break;
    case CMD_MS:
      slotSettings.ms = par1;
      break;
    case CMD_AC:
      slotSettings.ac = par1;
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
      slotSettings.ts = par1;
      break;
    case CMD_TP:
      slotSettings.tp = par1;
      break;
    case CMD_SP:
      slotSettings.sp = par1;
      break;
    case CMD_SS:
      slotSettings.ss = par1;
      break;
    case CMD_GV:
      slotSettings.gv = par1;
      break;
    case CMD_RV:
      slotSettings.rv = par1;
      break;
    case CMD_GH:
      slotSettings.gh = par1;
      break;
    case CMD_RH:
      slotSettings.rh = par1;
      break;
    case CMD_SB:
      if ((par1 < SENSORBOARD_REPORT_X) && (slotSettings.sb != par1)) {
        slotSettings.sb = par1;
        sensorValues.calib_now = CALIBRATION_PERIOD;  // initiate calibration for new sensorboard profile!
        initBlink(10, 20);
        makeTone(TONE_CALIB, 0);
        rp2040.fifo.push_nb(par1); // tell the other core to switch sensorboard profile
      }
      else  rp2040.fifo.push_nb(par1);  // tell the other core to apply sensorboard reporting settings
      break;
    case CMD_SC:
#ifdef DEBUG_OUTPUT_FULL
       Serial.print ("slot color: ");Serial.println (keystring);
       Serial.println((uint32_t)strtol(keystring, NULL, 0));
#endif
      slotSettings.sc = (uint32_t)strtol(keystring, NULL, 0);
      break;

    case CMD_RO:
      slotSettings.ro = par1;
      displayUpdate();
      break;
    case CMD_BT:
      slotSettings.bt = par1;
      displayUpdate();
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
    case CMD_BC:
      if (isBluetoothAvailable()) {
        
        Serial_AUX.write(keystring);
        Serial_AUX.write('\n'); //terminate command
        
        //byte bf[]= {0xfd,0,3,0,5,0,0,0,0};
        //Serial_AUX.write(bf, 9); //terminate command

        digitalWrite (6,!digitalRead (6));
      }
      break;
    case CMD_UG:
      //we set this flag here, flushing & disabling serial port is done in loop()
      addonUpgrade = BTMODULE_UPGRADE_START;
      Serial.println("Starting upgrade for BT addon!");
      // Command for upgrade sent to ESP - triggering reset into factory reset mode
      Serial_AUX.println("$UG");
      // delaying to ensure that UART command is sent and received
      delay(500);
      break;  
    case CMD_BR:
      resetBTModule (par1);
      break;
  }
}
