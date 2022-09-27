/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: reporting.h - implementation of serial reports

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/  


#include "FlipWare.h"
#include "parser.h"
#include "reporting.h"

/**
  static variables for report management
*/
uint8_t reportRawValues = 0;

/** 
 * @brief Print current to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printCurrentSlot(Stream *S)
{
  S->println(slotSettings.slotName);
  S->print("AT AX "); S->println(slotSettings.ax);
  S->print("AT AY "); S->println(slotSettings.ay);
  S->print("AT DX "); S->println(slotSettings.dx);
  S->print("AT DY "); S->println(slotSettings.dy);
  S->print("AT MS "); S->println(slotSettings.ms);
  S->print("AT AC "); S->println(slotSettings.ac);
  S->print("AT TS "); S->println(slotSettings.ts);
  S->print("AT TP "); S->println(slotSettings.tp);
  S->print("AT WS "); S->println(slotSettings.ws);
  S->print("AT SP "); S->println(slotSettings.sp);
  S->print("AT SS "); S->println(slotSettings.ss);
  S->print("AT MM "); S->println(slotSettings.stickMode);
  S->print("AT GV "); S->println(slotSettings.gv);
  S->print("AT RV "); S->println(slotSettings.rv);
  S->print("AT GH "); S->println(slotSettings.gh);
  S->print("AT RH "); S->println(slotSettings.rh);
  S->print("AT RO "); S->println(slotSettings.ro);
  S->print("AT BT "); S->println(slotSettings.bt);
  S->print("AT KL "); S->println(slotSettings.kbdLayout);

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    S->print("AT BM ");
    if (i < 9) S->print("0"); // leading zero for button numbers !
    S->println(i + 1);
    int actCmd = buttons[i].mode;
    char cmdStr[4];

    if ((actCmd <0 ) || (actCmd>=NUM_COMMANDS)) {
      S->print("E: buttonmode =");
      S->println(actCmd);
      actCmd=CMD_NC;
    }
    
    S->print("AT ");
    strcpy_FM(cmdStr, (uint_farptr_t_FM)atCommands[actCmd].atCmd);
    S->print(cmdStr);
    switch (pgm_read_byte_near(&(atCommands[actCmd].partype)))
    {
      case PARTYPE_UINT:
      case PARTYPE_INT:  S->print(" "); S->print(buttons[i].value); break;
      case PARTYPE_STRING: S->print(" "); S->print(buttonKeystrings[i]); break;
    }
    S->println("");
  }
}

void reportValues()
{
  static uint8_t valueReportCount = 0;
  if (!reportRawValues)   return;

  if (valueReportCount++ > 10) {      // report raw values approx. every 50ms !
    Serial.print("VALUES:"); Serial.print(sensorData.pressure); Serial.print(",");
    Serial.print(sensorData.up); Serial.print(","); Serial.print(sensorData.down); Serial.print(",");
    Serial.print(sensorData.left); Serial.print(","); Serial.print(sensorData.right); Serial.print(",");
    Serial.print(sensorData.xRaw); Serial.print(","); Serial.print(sensorData.yRaw); Serial.print(",");
    for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
      if (buttonStates & (1 << i)) Serial.print("1");
      else Serial.print("0");
    }
    Serial.print(",");
    Serial.print(actSlot);
    Serial.print(",");
    Serial.print(sensorData.xDriftComp);
    Serial.print(",");
    Serial.print(sensorData.yDriftComp);
    Serial.println("");
    valueReportCount = 0;
  }
}
