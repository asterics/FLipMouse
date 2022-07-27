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
uint8_t reportSlotParameters = REPORT_NONE;
uint8_t reportRawValues = 0;

void printCurrentSlot()
{
  Serial.print("Slot:");  Serial.println(slotSettings.slotName);
  Serial.print("AT AX "); Serial.println(slotSettings.ax);
  Serial.print("AT AY "); Serial.println(slotSettings.ay);
  Serial.print("AT DX "); Serial.println(slotSettings.dx);
  Serial.print("AT DY "); Serial.println(slotSettings.dy);
  Serial.print("AT MS "); Serial.println(slotSettings.ms);
  Serial.print("AT AC "); Serial.println(slotSettings.ac);
  Serial.print("AT TS "); Serial.println(slotSettings.ts);
  Serial.print("AT TP "); Serial.println(slotSettings.tp);
  Serial.print("AT WS "); Serial.println(slotSettings.ws);
  Serial.print("AT SP "); Serial.println(slotSettings.sp);
  Serial.print("AT SS "); Serial.println(slotSettings.ss);
  Serial.print("AT MM "); Serial.println(slotSettings.stickMode);
  Serial.print("AT GV "); Serial.println(slotSettings.gv);
  Serial.print("AT RV "); Serial.println(slotSettings.rv);
  Serial.print("AT GH "); Serial.println(slotSettings.gh);
  Serial.print("AT RH "); Serial.println(slotSettings.rh);
  Serial.print("AT RO "); Serial.println(slotSettings.ro);
  Serial.print("AT BT "); Serial.println(slotSettings.bt);
  Serial.print("AT KL "); Serial.println(slotSettings.kbdLayout);

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    Serial.print("AT BM ");
    if (i < 9) Serial.print("0"); // leading zero for button numbers !
    Serial.println(i + 1);
    int actCmd = buttons[i].mode;
    char cmdStr[4];

    if ((actCmd <0 ) || (actCmd>=NUM_COMMANDS)) {
      Serial.print("E: buttonmode =");
      Serial.println(actCmd);
      actCmd=CMD_NC;
    }
    
    Serial.print("AT ");
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
