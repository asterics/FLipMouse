/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
 
/**
 * General EEPROM memory layout:
 * 
 * 0x0000-0x008F	struct storageHeader		
 * 0x0090-0xXXXX	setting slots:
 * 
 * Magic Number: 0xAA55
 * struct slotGeneralSettings
 * '\0'
 * char * slotName ('\0')
 * //////////////////////////
 * struct slotButtonSettings
 * '\0'
 * uint8_t * buttonParameter
 * '\0'
 * ////// repeated for NUMBER_OF_BUTTONS
 * 
 * 
 * 0xXXXX-0x8000	IR slot (EEPROM max address in this case for 256kbit):
 * 
 * Magic Number: 0xBB66
 * char * slotName ('\0')
 * uint8_t countEdges
 * uint8_t timings[]
 * '\0'
 * 
 * 
 * */
 
#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "FlipWare.h"
#include <Wire.h>
#include <EEPROM.h>

#define EEPROM_COUNT_SLOTS 4
#define EEPROM_COUNT_IRCOMMAND 64
#define EEPROM_MAX_ADDRESS 0x7FFF


#define EEPROM_MAGIC_NUMBER_SLOT 	0xAA55
#define EEPROM_MAGIC_NUMBER_IR		0xBB66


#define EEPROM_FULL_DEBUG 2
#define EEPROM_BASIC_DEBUG 1
#define EEPROM_NO_DEBUG 0

extern uint8_t eepromDebugLevel;

/**
 * Describing the header structure of memory, starting at 0x00.
 * Each slot is started at a fixed address in the EEPROM memory.
 * To reduce the read requests to find slot data, these start addresses
 * are initialized once (it is updated, if something is changed).
 * In adddition, infrared commands are available, which are stored at
 * the end of the memory.
 * 
 * */
struct storageHeader {
	uint16_t	startSlotAddress[EEPROM_COUNT_SLOTS];
	uint16_t	startIRAddress[EEPROM_COUNT_IRCOMMAND];
	uint16_t	endSlotAddress;
};



struct slotGeneralSettings {
	uint8_t  mouseOn;// mouse or alternative functions 
	uint8_t  ax;     // acceleration x
	uint8_t  ay;     // acceleration y
	int16_t  dx;     // deadzone x
	int16_t  dy;     // deadzone y
	uint16_t ts;     // threshold sip
	uint16_t tp;     // threshold puff 
	uint8_t  ws;     // wheel stepsize  
	uint16_t sm;     // threshold special mode 
	uint16_t hm;     // threshold hold mode 
	uint8_t  gu;     // gain up 
	uint8_t  gd;     // gain down 
	uint8_t  gl;     // gain left 
	uint8_t  gr;     // gain right 
	int16_t  cx;     // calib x
	int16_t  cy;     // calib y
};

struct slotButtonSettings {
	uint16_t mode;
	uint16_t value;
};


/**
 * Load the EEPROM header, which contains all start addresses of the
 * slots and IR commands
 * */
void bootstrapSlotAddresses();


/**
 * This function deletes one IR command by deleting the magic number (reset to 0xFF)
 * If the "name" parameter is set to \0, all IR commands will be deleted
 * */
void deleteIRCommand(char * name);

/**
 * Save one IR command to the EEPROM. If the name is already assigned,
 * it will be overwritten, otherwise a new slot will be used.
 * There is an array of timings between the edges, the length
 * is provided by cntEdges.
 * The name is also provided as parameter
 * */ 
void saveIRToEEPROM(char * name,uint8_t *timings,uint8_t cntEdges);


/**
 * Save one IR command to the EEPROM. The slot is determined by the slot number.
 * There is an array of timings between the edges, the length
 * is provided by cntEdges.
 * The name is also provided as parameter
 * */ 
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint8_t *timings,uint8_t cntEdges);

/**
 * Print out all slotnames to the serial interface
 * Used by the PC-GUI to load all available slot names
 * */
void listIRCommands();

/**
 * Replay one IR command from the EEPROM.
 * The slot is identified by the slot number
 * ATTENTION: if this method is not called from another function (e.g. readIRFromEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!!!
 * */
uint16_t readIRFromEEPROMSlotNumber(uint8_t slotNr,uint8_t *timings,uint8_t maxEdges);

/**
 * Replay one IR command from the EEPROM.
 * The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name,uint8_t *timings,uint8_t maxEdges);

/**
 * This function deletes all slots by deleting the magic number (reset to 0xFF)
 * */
void deleteSlots();


/**
 * Print out all slotnames to the serial interface
 * Used by the PC-GUI to load all available slot names
 * */
void listSlots();


/**
 * Read one slot data from the EEPROM to the global variables
 * The slot is identified by the slot number.
 * If the search flag is set, this function loads the next possible slot, if
 * the current one is not valid
 * ATTENTION: if this method is not called from another function (e.g. readFromEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!
 * */
void readFromEEPROMSlotNumber(uint8_t nr, bool search);

/**
 * Read one slot data from the EEPROM to the global variables
 * The slot is identified by the slot name
 * */
void readFromEEPROM(char * slotname);


/**
 * Matcher function to determine the slot number for a given slot name.
 * Currently the GUI is handling all slot assignments via the name.
 * For future implementations, it may base on numbers.
 * To make the development easy, the parsing of slotnames to slot numbers
 * is done in this method (used by read and save functions)
 * */
int8_t slotnameToNumber(char * slotname);

/**
 * Store current slot data to the EEPROM.
 * The slot is identified by the slot number. If the nr parameter is -1,
 * a new slot will be created (at the first possible position)
 * ATTENTION: if this method is not called from another function (e.g. saveToEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!
 * */
void saveToEEPROMSlotNumber(int8_t nr, char * slotname);

#endif