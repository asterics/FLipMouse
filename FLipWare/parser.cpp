/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: parser.cpp - implementation of the parser for serial commands and parameters

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/  


#include "FlipWare.h"
#include "parser.h"

uint8_t readstate = 0;

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern void init_CIM_frame (void);
extern void parse_CIM_protocol(int actbyte);
extern uint8_t CimParserActive;

/**
   @name get_uint
   @brief converts a string into an unsigned integer (max 16 bit!)
   @param str pointer to string
   @param result pointer to 16 bit integer where result shall be stored
   @return true if conversion valid, false if wrong characters detected
*/
uint8_t get_uint(char * str, int16_t * result)
{
  int num = 0;
  if ((str == 0) || (*str == 0)) return (0);
  while (*str)
  {
    if ((*str >= '0') && (*str <= '9'))
      num = num * 10 + (*str - '0');
    else return (0);
    str ++;
  }
  *result = num;
  return (1);
}

/**
   @name get_int
   @brief converts a string into an signed integer (max 16 bit!)
   @param str pointer to string
   @param result pointer to 16 bit integer where result shall be stored
   @return true if conversion valid, false if wrong characters detected
*/
uint8_t get_int(char * str, int16_t * result)
{
  int16_t num, fact;
  if (str == 0) return (0);
  if (*str == '-') {
    fact = -1;
    str++;
  } else fact = 1;
  if (!get_uint(str, &num)) return (0);
  *result = num * fact;
  // Serial.println(*result);
  return (1);
}

/**
   @name strup
   @brief converts a string into upper case characters
   @param str pointer to string
   @return none
*/
void strup (char * str)   // convert to upper case letters
{
  if (!str) return;
  while (*str)
  {
    if ((*str >= 'a') && (*str <= 'z')) *str = *str - 'a' + 'A';
    str++;
  }
}

void parseCommand (char * cmdstr)
{
  int8_t cmd = -1;
  int16_t num = 0;

  cmdstr[strlen(cmdstr)+1]=0;  // to prevent exceeing the actual commandstring (when emptry string parameters are passed!)
#ifdef DEBUG_OUTPUT_FULL
  Serial.print("parseCommand:"); Serial.println(cmdstr);
#endif
  char * actpos = strtok(cmdstr, " ");  // see a nice explaination of strtok here:  http://www.reddit.com/r/arduino/comments/2h9l1l/using_the_strtok_function/

  if (actpos)
  {
#ifdef DEBUG_OUTPUT_FULL
    Serial.print("actpos:"); Serial.println(actpos);
#endif
    int i;
    strup(actpos);

    for (i = 0; (i < NUM_COMMANDS) && (cmd == -1); i++)
    {
      if (!strcmp_FM(actpos, (uint_farptr_t_FM)atCommands[i].atCmd))  {
        // Serial.print ("partype="); Serial.println (pgm_read_byte_near(&(atCommands[i].partype)));
        switch (pgm_read_byte_near(&(atCommands[i].partype)))
        {
          case PARTYPE_UINT: actpos = strtok(NULL, " ");  if (get_uint(actpos, &num)) cmd = i ; break;
          case PARTYPE_INT:  actpos = strtok(NULL, " ");  if (get_int(actpos, &num)) cmd = i ; break;
          case PARTYPE_STRING: actpos += 3; cmd = i; break;
          default: cmd = i; actpos = 0; break;
        }
      }
    }
  }

  if (cmd > -1) {
    //Serial.print("cmd:");Serial.print(cmd);Serial.print("numpar:");
    //Serial.print(num);Serial.print("stringpar:");Serial.println(actpos);
    performCommand(cmd, num, actpos, 0);
  }
  else Serial.println("???");       // command not recognized!
}



void parseByte (int newByte)  // parse an incoming commandbyte from serial interface, perform command if valid
{
  static uint8_t cmdlen = 0;

  if (CimParserActive)
    parse_CIM_protocol(newByte);   // handle AsTeRICS CIM protocol messages !
  else
  {
    switch (readstate) {
      case 0:
        if ((newByte == 'A') || (newByte == 'a')) readstate++;
        if (newByte == '@') {
          readstate++;  // switch to AsTeRICS CIM protocol parser
          CimParserActive = 1;
        }
        break;
      case 1:
        if ((newByte == 'T') || (newByte == 't')) readstate++; else readstate = 0;
        break;
      case 2:
        if ((newByte == '\r') || (newByte == '\n')) // AT reply: "OK"
        {
          Serial.println("OK");
          readstate = 0;
        }
        else if (newByte == ' ') {
          cmdlen = 0;
          readstate++;
        }
        else goto err;
        break;
      case 3:
        if ((newByte == '\r') || (newByte == '\n') || (cmdlen >= WORKINGMEM_SIZE - 5))
        { workingmem[cmdlen] = 0;  parseCommand((char *)workingmem);
          readstate = 0;
        }
        else workingmem[cmdlen++] = newByte;
        break;
default: err: Serial.println("?"); readstate = 0;
    }
  }
}
