/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: eeprom.h - implementation of the EEPROM storage management, header file

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

/**
   General EEPROM memory layout:
   (0x7FFF is the max address in case of a 256kbit EEPROM)

   0x0000-0x006D	struct storageHeader
   0x006E-0xXXXX	setting slots

   layout of one slot:
     struct slotGeneralSettings
     struct slotButtonSettings[NUMBER_OF_BUTTONS]
     char buttonParameter[len][NUMBER_OF_BUTTONS] ('\0' terminated strings)

   0x7FFE-0xXXXX	IR code storage 
   Note that the IR slots start at the top address (0x7FFE, top->down)

   layout of one IR command slot:
     char irSlotName[MAX_NAME_LEN]  ('\0' terminated string)
     uint8_t countEdges     
     uint16_t timings[countEdges]

   0x7FFF  Magic Byte to indicate valid EEPROM content   

 * */

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "FlipWare.h"
#include "i2c_t3.h"
#include <EEPROM.h>

#define EEPROM_COUNT_SLOTS 7
#define EEPROM_COUNT_IRCOMMAND 20

#define EEPROM_MAX_ADDRESS 0x7FFF
#define EEPROM_MAGIC_NUMBER 	0x29
#define MAX_NAME_LEN  15 

/**
   header structure, starts at adress 0x00 in EEPROM,
   contains start and end adresses of configuration slots
   and IR commands (the IR commands are stored top-down)

 * */
struct storageHeader {
  uint16_t	startSlotAddress[EEPROM_COUNT_SLOTS];
  uint16_t  endSlotAddress[EEPROM_COUNT_SLOTS];
  uint16_t	startIRAddress[EEPROM_COUNT_IRCOMMAND];
  uint16_t  endIRAddress[EEPROM_COUNT_IRCOMMAND];
  uint16_t  versionID;
};

struct irCommandHeader {
  char irName[MAX_NAME_LEN];
  uint8_t edges;
};

/**
   Load the EEPROM header, which contains all start addresses of the
   slots and IR commands
 * */
void bootstrapSlotAddresses();


/**
   This function deletes one IR command by deleting the magic number (reset to 0xFF)
   If the "name" parameter is set to \0, all IR commands will be deleted
 * */
void deleteIRCommand(char * name);

/**
   Save one IR command to the EEPROM. If the name is already assigned,
   it will be overwritten, otherwise a new slot will be used.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROM(char * name, uint16_t *timings, uint8_t cntEdges);


/**
   Save one IR command to the EEPROM. The slot is determined by the slot number.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint16_t *timings, uint8_t cntEdges);

/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listIRCommands();

/**
   Replay one IR command from the EEPROM.
   The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name, uint16_t *timings, uint8_t maxEdges);

/**
   This function deletes the slot from EEPROM.
   if an empty string is given as parameter, all slots are deleted!
 * */
void deleteSlot(char * name);

/**
   get the first free address for slot data  
 * */
uint16_t getFreeSlotAddress(void);

/**
   get the index of the lat slot which holds data  
 * */
int8_t getLastSlotIndex(void);

/**
   Print out all slot data  to the serial interface
 * */
void printAllSlots(void);

/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listSlots();

/**
   Read one slot data from the EEPROM to the global variables
   The slot is identified by the slot number.
   if the playTone flag is set, a tone according to the current slot number will be played
 * */
void readFromEEPROMSlotNumber(uint8_t nr,  bool playTone);

/**
   Read one slot data from the EEPROM to the global variables
   The slot is identified by the slot name
 * */
void readFromEEPROM(char * slotname);


/**
   Determines the slot number for a given slot name.
 * */
int8_t slotnameToNumber(char * slotname);

/**
   Store current slot data to the EEPROM.
   The slot is identified by the slot number. If the nr parameter is -1,
   a new slot will be created (at the first possible position)
   ATTENTION: if this method is not called from another function (e.g. saveToEEPROM()),
   it is necessary to preload the start adresses via bootstrapSlotAddresses()!
 * */
void saveToEEPROMSlotNumber(int8_t nr, char * slotname);

#endif
