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
     struct SlotSettings
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
#include "parser.h"

#define MAX_SLOTS_IN_EERPOM 10
#define MAX_IRCOMMANDS_IN_EERPOM 20

#define EEPROM_MAX_ADDRESS 0x7FFF
#define EEPROM_MAGIC_NUMBER 	0x3b
#define MAX_NAME_LEN  15

/**
   header structure, starts at adress 0x00 in EEPROM,
   contains start and end adresses of configuration slots
   and IR commands (the IR commands are stored top-down)

 * */
struct storageHeader {
  uint16_t	startSlotAddress[MAX_SLOTS_IN_EERPOM];
  uint16_t  endSlotAddress[MAX_SLOTS_IN_EERPOM];
  uint16_t	startIRAddress[MAX_IRCOMMANDS_IN_EERPOM];
  uint16_t  endIRAddress[MAX_IRCOMMANDS_IN_EERPOM];
  uint16_t  versionID;
};

struct irCommandHeader {
  char irName[MAX_NAME_LEN];
  uint16_t edges;
};

/**
   Check if storage is valid, initialize if not.
 * */
void initStorage();


/**
   This function deletes one IR command by deleting the magic number (reset to 0xFF)
   If the "name" parameter is set to \0, all IR commands will be deleted
   returns 1 if successful, 0 otherwise
 * */
uint8_t deleteIRCommand(char const * name);

/**
   Save one IR command to the EEPROM. If the name is already assigned,
   it will be overwritten, otherwise a new slot will be used.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROM(char * name, uint16_t *timings, uint16_t cntEdges);


/**
   Save one IR command to the EEPROM. The slot is determined by the slot number.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint16_t *timings, uint16_t cntEdges);

/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listIRCommands();

/**
   Replay one IR command from the EEPROM.
   The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name, uint16_t *timings, uint16_t maxEdges);

/**
   This function deletes the slot from EEPROM.
   if an empty string is given as parameter, all slots are deleted!
   returns 1 if successful, 0 otherwise
 * */
uint8_t deleteSlot(char const * name);

/**
   get the first free address for slot data  
 * */
uint16_t getFreeSlotAddress(void);

/**
   get the index of the last slot which holds data  
 * */
int8_t getLastSlotIndex(void);

/**
   get the index of the last available IR command
 * */
int8_t getLastIRIndex(void);

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
   returns 1 if successful, 0 otherwise
 * */
uint8_t readFromEEPROMSlotNumber(uint8_t nr,  bool playTone);

/**
   Read one slot data from the EEPROM to the global variables
   The slot is identified by the slot name
   returns 1 if successful, 0 otherwise
 * */
uint8_t readFromEEPROM(char const * slotname);


/**
   Determines the slot number for a given slot name.
 * */
int8_t slotnameToNumber(char const * slotname);

/**
   Save the current slot by the given slotname.
   If there is no slot existing with this name,
   it will be saved to a new slot.
   returns 1 if successful, 0 if max slot count is reached

 * */
uint8_t saveToEEPROM(char const * slotname);

/**
   Store current slot data to the EEPROM.
   The slot is identified by the slot number. If the nr parameter is -1,
   a new slot will be created (at the first possible position)
 * */
void saveToEEPROMSlotNumber(int8_t nr, char const * slotname);

/**
 * If settings are under version control (TBD!), this function returns the current
 * revision (-> foldername in FS) to read from / write to.
 */
uint8_t getSettingsRevision(void);

#endif
