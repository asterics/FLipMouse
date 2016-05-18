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
 * TODO:
 * -) implement the defragmentation for slots and IR commands
 * -) implement a "memory full" command
 * -) activate methods for IR commands, will be done after the merge to master
 * -) implement overwriting of existing slots/ir commands, if the name already exists
 * */

#include "eeprom.h"

/** Device address of the EEPROM **/
#define deviceaddress 0x50

/**
 * Local copy of all slot and ir command start adresses
 * */
struct storageHeader slotAdresses;
uint8_t eepromDebugLevel = EEPROM_NO_DEBUG;
uint8_t currentSlot;

/**
 * Write one byte to the EEPROM
 * */
void writeEEPROM(unsigned int eeaddress, byte data ) 
{
  #ifdef TEENSY_LC
    Wire.beginTransmission(deviceaddress);
    Wire.send((int)(eeaddress >> 8));   // MSB
    Wire.send((int)(eeaddress & 0xFF)); // LSB
    Wire.send(data);
    Wire.endTransmission();
    delay(5);
    
    if(eepromDebugLevel == EEPROM_FULL_DEBUG)
	{
		Serial.print(F("EEPROM w@"));
		Serial.print(eeaddress);
		Serial.print(": ");
		Serial.println(data);
	}
  #else
    EEPROM.write(eeaddress, data);
  #endif
}
 
/**
 * Read one byte from the EEPROM
 * TODO: add another read function for more than one byte
 * */
byte readEEPROM(unsigned int eeaddress ) 
{

  #ifdef TEENSY_LC
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.send((int)(eeaddress >> 8));   // MSB
    Wire.send((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
 
    Wire.requestFrom(deviceaddress,1);
 
    if (Wire.available()) rdata = Wire.receive();
	if(eepromDebugLevel == EEPROM_FULL_DEBUG)
	{
		Serial.print(F("EEPROM r@"));
		Serial.print(eeaddress);
		Serial.print(": ");
		Serial.println(rdata);
	}
    return rdata;
  #else
    return(EEPROM.read(eeaddress));
  #endif
}


/**
 * Save the current slot by the given slotname.
 * If there is no slot existing with this name,
 * it will be saved to a new slot.
 * */
void saveToEEPROM(char * slotname)
{
	int8_t nr = slotnameToNumber(slotname);
	uint16_t addr;
	//a new slot, search for the first adress which is "0"
	if(nr == -1)
	{
		for(uint8_t i = 0; i<EEPROM_COUNT_SLOTS; i++)
		{
			if(slotAdresses.startSlotAddress[i] == 0) 
			{
				if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
				{
					Serial.print(F("New slot @"));
					Serial.println(i);
				}
				return saveToEEPROMSlotNumber(i, slotname);
			} else {
				addr = slotAdresses.startSlotAddress[i];
				if(readEEPROM(addr++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF))
				{
					if(readEEPROM(addr++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8)
					{
						if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
						{
							Serial.print(F("Overwrite slot @"));
							Serial.println(i);
						}
						return saveToEEPROMSlotNumber(i, slotname);
					}
				}
			}
		}
	}
}

/**
 * Store current slot data to the EEPROM.
 * The slot is identified by the slot number. If the nr parameter is -1,
 * a new slot will be created (at the first possible position)
 * ATTENTION: if this method is not called from another function (e.g. saveToEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!
 * */
void saveToEEPROMSlotNumber(int8_t nr, char * slotname)
{
	uint16_t size = 2; //magic number
	uint16_t addr = 0; //address pointer 1
	uint16_t addr2 = 0; //address pointer 2
	uint8_t* p = (uint8_t *)&settings; //pointer to the global settings
	
	//determine the size of this slot, maybe we need to defrag the EEPROM
	size += sizeof(slotGeneralSettings);
	size++; //\0
	while(*(slotname + addr) != '\0') addr++;
	size += addr;
	size++; //\0
	
	addr = 0;
	for(uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		size += sizeof(slotButtonSettings);
		size++; //\0
		addr = 0;
		while(*(keystringButton[i] + addr) != '\0') addr++;
		size+=addr;
	}
	
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print(F("Slot size:"));
		Serial.println(size);
	}
	
	//if this is the last slot, don't move other slots
	//TBD: defragmentation, currently not necessary (all slots are deleted before saving)
	/*if(nr < (EEPROM_COUNT_SLOTS-1)) 
	{
		//do we need to defrag?
		if((slotAdresses.startSlotAddress[nr+1]-slotAdresses.startSlotAddress[nr]) != size)
		{
			#ifdef EEPROM_FULL_DEBUG
				Serial.println(F("Size != free space, defrag"));
			#endif
			//2 possibilities:
			//-) if the size of the new slot is bigger, resave from the back to the start
			//-) if the size is smaller, resave from the beginning to the back
			//TODO: nur slots mit adresse != 0 machen...
			
			if(size > (slotAdresses.startSlotAddress[nr+1]-slotAdresses.startSlotAddress[nr]))
			{
				//load the current last address
				addr = slotAdresses.endSlotAddress;
				//add the additional required size (required size minus the size of the current slot)
				addr += size - (slotAdresses.startSlotAddress[nr+1]-slotAdresses.startSlotAddress[nr]);
				
				//resave all following slots, starting from the back of the memory
				for(uint16_t i = slotAdresses.endSlotAddress; i >= slotAdresses.startSlotAddress[nr+1]; i--)
				{
					writeEEPROM(addr,readEEPROM(i));
					addr--;
				}
			} else {
				//load the address of this slot and add the size
				addr = slotAdresses.startSlotAddress[nr] + size;
				//resave all following slots, starting from the back of the memory
				for(uint16_t i = slotAdresses.startSlotAddress[nr+1]; i >= slotAdresses.endSlotAddress; i++)
				{
					writeEEPROM(addr,readEEPROM(i));
					addr++;
				}
			}
			
		}
	}*/
	
	/** save this slot **/
	//first slot, start right after the storage header
	if(nr == 0) 
	{
		slotAdresses.startSlotAddress[nr] = sizeof(storageHeader);
	}
	
	//load the start address
	addr = slotAdresses.startSlotAddress[nr];
	
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print(F("Start new slot @"));
		Serial.println(addr);
	}
	
	//write both bytes of the magicnumber, indicating a valid slot
	writeEEPROM(addr++,(EEPROM_MAGIC_NUMBER_SLOT & 0x00FF));
	writeEEPROM(addr++,(EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8);
	
	//save the general settings to the global settings struct
	p = (uint8_t *)&settings;
	for (uint16_t t=0;t<sizeof(slotGeneralSettings);t++) writeEEPROM(addr++, *(p+t));
	
	//write '\0' seperator
	writeEEPROM(addr++, '\0');
	
	//write the slotname
	addr2 = 0;
	do {
		writeEEPROM(addr++,*(slotname + addr2));
		addr2++;
	} while(*(slotname + addr2 -1) != '\0');
	
	//load all button settings
	for(uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		//load struct slotButtonSettings
		p = (uint8_t *)&buttons[i];
		for (uint16_t t=0;t<sizeof(slotButtonSettings);t++) writeEEPROM(addr++, *(p+t));
		
		//write '\0' seperator
		writeEEPROM(addr++,0);
		
		//add additional payload
		p = (uint8_t *)keystringButton[i];
		for(uint16_t t = 0; t<MAX_KEYSTRING_LEN; t++)
		{
			writeEEPROM(addr++,*(p+t));
			if(*(p+t) == 0) break;
		}
		
	}
	
	/** update the slotAdresses table */
	//is this the last slot? If yes, just store the end of settings memory
	if(nr == (EEPROM_COUNT_SLOTS - 1)) 
	{
		//slotAdresses.endSlotAddress = slotAdresses.startSlotAddress[nr] + size;
		slotAdresses.endSlotAddress = addr;
	} else {
		//if this is not the last slot, resave all following start adresses
		slotAdresses.startSlotAddress[nr+1] = addr++;
		for(uint8_t i = (nr+1); i<EEPROM_COUNT_SLOTS; i++)
		{
			//TODO!!!!
			//slotAdresses.startSlotAddress[i] = ;
		}
		//slotAdresses.endSlotAddress = ;
	}
	
	
	//store the storageheader permanently to the EEPROM
	//bootstrap is not necessary, already updated previously
	for(uint16_t i = 0; i<sizeof(storageHeader); i++)
	{
		writeEEPROM(i, *((uint8_t*)&slotAdresses+i));
	}
}

/**
 * Matcher function to determine the slot number for a given slot name.
 * Currently the GUI is handling all slot assignments via the name.
 * For future implementations, it may base on numbers.
 * To make the development easy, the parsing of slotnames to slot numbers
 * is done in this method (used by read and save functions)
 * */
int8_t slotnameToNumber(char * slotname)
{
	uint16_t address; //current EEPROM address
	uint8_t matches;

	//pre-load all starting adresses from the memory
	bootstrapSlotAddresses();
	
	//iterate all possible slots
	for(uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
	{
		//load the base address for the current slot
		address = slotAdresses.startSlotAddress[i];
		
		//a zero address is not valid, no slot there...
		if(address == 0) continue;
		
		//check both bytes of the magicnumber, indicating a valid slot
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) continue;
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8) continue;
		
		//skip the general settings, we only need the slot names
		address += sizeof(slotGeneralSettings);
		
		//skip '\0'
		address++;
		
		//reset the match flag
		matches = 1;

		//compare the slotname
		for (unsigned int t=0;t<MAX_SLOTNAME_LEN;t++)
		{
			//compare byte for byte
			if(*(slotname + t) != readEEPROM(address + t))
			{
				//if one byte doesn't match, reset match flag and break the for loop
				matches = 0;
				if(eepromDebugLevel == EEPROM_FULL_DEBUG)
				{
					Serial.print("Names don't match @");
					Serial.print(t);
					Serial.print(" ");
					Serial.print(*(slotname+t));
					Serial.print("!=");
					Serial.println(readEEPROM(address + t));
				}
				break;
			}
			//if the end of the string is reached, break the for loop
			if(*(slotname + t) == '\0') break;
		}
		
		//if it matches, return the number
		if(matches)
		{
			return i;
		
		}
	}
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG) 
	{
		Serial.print("No slot found for ");
		Serial.println(slotname);
	}
	return -1;
}

/**
 * Matcher function to determine the slot number for a given slot name (IR command)
 * Currently the GUI is handling all slot assignments via the name.
 * For future implementations, it may base on numbers.
 * To make the development easy, the parsing of slotnames to slot numbers
 * is done in this method (used by read and save functions)
 * */
int8_t slotnameIRToNumber(char * slotname)
{
	uint16_t address; //current EEPROM address
	uint8_t matches;

	//pre-load all starting adresses from the memory
	bootstrapSlotAddresses();
	
	//iterate all possible slots
	for(uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
	{
		//load the base address for the current slot
		address = slotAdresses.startIRAddress[i];
		
		//a zero address is not valid, no slot there...
		if(address == 0) continue;
		
		//check both bytes of the magicnumber, indicating a valid slot
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF)) continue;
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00)>>8) continue;
		
		//reset the match flag
		matches = 1;

		//compare the slotname
		for (unsigned int t=0;t<MAX_SLOTNAME_LEN;t++)
		{
			//compare byte for byte
			if(*(slotname + t) != readEEPROM(address + t))
			{
				//if one byte doesn't match, reset match flag and break the for loop
				matches = 0;
				if(eepromDebugLevel == EEPROM_FULL_DEBUG)
				{
					Serial.print("Names don't match @");
					Serial.print(t);
					Serial.print(" ");
					Serial.print(*(slotname+t));
					Serial.print("!=");
					Serial.println(readEEPROM(address + t));
				}
				break;
			}
			//if the end of the string is reached, break the for loop
			if(*(slotname + t) == '\0') break;
		}
		
		//if it matches, return the number
		if(matches)
		{
			if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
			{
				Serial.print("Found IR command ");
				Serial.print(slotname);
				Serial.print("@");
				Serial.println(i);
			}
			return i;
		
		}
	}
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG) 
	{
		Serial.print("No slot found for ");
		Serial.println(slotname);
	}
	return -1;
}


/**
 * Load one slot by the given slotname
 * If there is no slotname give, load next slot...
 * */
void readFromEEPROM(char * slotname)
{
	if(*slotname == 0) return readFromEEPROMSlotNumber(actSlot + 1, true);
	
	int8_t nr = slotnameToNumber(slotname);
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print(F("Load slot "));
		Serial.print(slotname);
		Serial.print("@");
		Serial.println(nr);
	}
	//call the method which loads the data
	if(nr >= 0) readFromEEPROMSlotNumber(nr,true);
	else readFromEEPROMSlotNumber(0,true);
}

/**
 * Read one slot data from the EEPROM to the global variables
 * The slot is identified by the slot number.
 * If the search flag is set, this function loads the next possible slot, if
 * the current one is not valid
 * ATTENTION: if this method is not called from another function (e.g. readFromEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!
 * */
void readFromEEPROMSlotNumber(uint8_t nr, bool search)
{
	uint8_t* p = (uint8_t *)&settings;
	uint16_t address;
	uint8_t x;
	
	//fence the slot number, avoiding out of array index problems
	if(nr >= EEPROM_COUNT_SLOTS) nr = 0;
	
	if(search)
	{
		//check for a valid slot. If the current nr contains no slot, try
		//to load next one, until the end is reached.
		for(x = nr; x < EEPROM_COUNT_SLOTS; x++)
		{
			address = slotAdresses.startSlotAddress[x];
		
			if(eepromDebugLevel == EEPROM_BASIC_DEBUG) 
			{
				Serial.print("Load slot by nr");
				Serial.println(x);
			}
		
			//check both bytes of the magicnumber, indicating a valid slot
			if(readEEPROM(address++) == (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF))
			{
				if(readEEPROM(address++) == (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8)
				{
					//load the address again, recheck after this loop
					address = slotAdresses.startSlotAddress[x];
					//set the current slot nr
					actSlot = x;
					break;
				}
			}
			
		}
		
		//try one more time if this is a valid slot.
		//If no, we should point at the end of all slots and we try it one
		//more time with slot 0
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF) ||
		readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8)
		{
			address = slotAdresses.startSlotAddress[0];
			if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) return;
			if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8) return;
			actSlot = 0;
		}
	} else {
		address = slotAdresses.startSlotAddress[nr];
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) return;
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8) return;
	}
	
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print("Magic bytes OK, loading data @");
		Serial.println(address);
	}
	
	//save the general settings to the global settings struct
	for (uint16_t t=0;t<sizeof(slotGeneralSettings);t++) *p++=readEEPROM(address++);
	
	//skip '\0' seperator
	address++;
	
	//read the slotname
	p = (uint8_t *)&slotName;
	do {
		*p++ = readEEPROM(address++);
	} while(*(p-1) != '\0');
	
	if (DebugOutput==DEBUG_FULLOUTPUT) {  Serial.print("found slotname "); Serial.println(slotName); }

	//load all button settings
	for(uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		//load struct slotButtonSettings
		p = (uint8_t *)&buttons[i];
		for (uint16_t t=0;t<sizeof(slotButtonSettings);t++) *(p+t)=readEEPROM(address++);
		
		//skip '\0' seperator
		if(readEEPROM(address++) != '\0') { Serial.print(F("Error: seperator problem")); Serial.println(readEEPROM(address-1)); }
		
		//check for additional payload
		if(readEEPROM(address) != '\0') {
			//load additional payload
			p = (uint8_t *) &keystringButton[i];
			for(uint16_t t = 0; t<sizeof(keystringButton[i]);t++)
			{
				*(p+t) = readEEPROM(address++);
				if(*(p+t) == 0) break;
			}
		} else {
			//jump to next slot
			address++;
		}
	}
	
	if (reportSlotParameters!=REPORT_NONE)  
           printCurrentSlot();
           
	makeTone(TONE_CHANGESLOT,actSlot);
	if(eepromDebugLevel >= EEPROM_BASIC_DEBUG) { Serial.print("actSlot: "); Serial.println(actSlot); }

	if (reportSlotParameters) 
		Serial.println("END");   // important: end marker for slot parameter list (command "load all" - AT LA)
}

/**
 * This function deletes all slots by deleting the magic number (reset to 0xFF)
 * */
void deleteSlots()
{
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG) Serial.println("Deleting all slots");
	for(uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++) 
	{
		if(slotAdresses.startSlotAddress[i] > sizeof(storageHeader))
		{
			writeEEPROM(slotAdresses.startSlotAddress[i],0x00);
			writeEEPROM(slotAdresses.startSlotAddress[i]+1,0x00);
		}
		slotAdresses.startSlotAddress[i] = 0;
	}
	//store the storageheader permanently to the EEPROM
	//bootstrap is not necessary, already updated previously
	for(uint16_t i = 0; i<sizeof(storageHeader); i++)
	{
		writeEEPROM(i, *((uint8_t*)&slotAdresses+i));
	}
}

/**
 * This function deletes one IR command by deleting the magic number (reset to 0xFF)
 * If the "name" parameter is set to \0, all IR commands will be deleted
 * */
void deleteIRCommand(char * name)
{
	uint8_t nr;
	if(name[0] != 0)
	{
		nr = slotnameIRToNumber(name);
		if(eepromDebugLevel==EEPROM_BASIC_DEBUG)
		{
			Serial.print("Deleting slot ");
			Serial.print(name);
			Serial.print("@");
			Serial.println(nr);
		}
		if(nr >= 0)
		{
			if(slotAdresses.startIRAddress[nr] > sizeof(storageHeader))
			{
				writeEEPROM(slotAdresses.startIRAddress[nr],0xFF);
				writeEEPROM(slotAdresses.startIRAddress[nr]+1,0xFF);
			}
			slotAdresses.startIRAddress[nr] = 0;
		}
	} else {
		if(eepromDebugLevel==EEPROM_BASIC_DEBUG) Serial.println("Deleting all IR slots");
		for(uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++) 
		{
			if(slotAdresses.startIRAddress[i] > sizeof(storageHeader))
			{
				writeEEPROM(slotAdresses.startIRAddress[i],0xFF);
				writeEEPROM(slotAdresses.startIRAddress[i]+1,0xFF);
			}
			slotAdresses.startIRAddress[i] = 0;
		}
	}
	//store the storageheader permanently to the EEPROM
	//bootstrap is not necessary, already updated previously
	for(uint16_t i = 0; i<sizeof(storageHeader); i++)
	{
		writeEEPROM(i, *((uint8_t*)&slotAdresses+i));
	}
}

/**
 * Save one IR command to the EEPROM. If the name is already assigned,
 * it will be overwritten, otherwise a new slot will be used.
 * There is an array of timings between the edges, the length
 * is provided by cntEdges.
 * The name is also provided as parameter
 * */ 
void saveIRToEEPROM(char * name,uint8_t *timings,uint8_t cntEdges)
{
	int8_t nr = slotnameIRToNumber(name);
	uint16_t addr;
	//a new slot, search for the first adress which is "0"
	if(nr == -1)
	{
		for(uint8_t i = 0; i<EEPROM_COUNT_IRCOMMAND; i++)
		{
			if(slotAdresses.startIRAddress[i] == 0) 
			{
				if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
				{
					Serial.print(F("New IR command @"));
					Serial.println(i);
				}
				return saveIRToEEPROMSlotNumber(i, name, timings, cntEdges);
			} else {
				addr = slotAdresses.startIRAddress[i];
				if(readEEPROM(addr--) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF))
				{
					if(readEEPROM(addr--) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00)>>8)
					{
						if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
						{
							Serial.print(F("Overwrite IR command @"));
							Serial.println(i);
						}
						return saveIRToEEPROMSlotNumber(i, name, timings, cntEdges);
					}
				}
			}
		}
	}
}


/**
 * Save one IR command to the EEPROM. The slot is determined by the slot number.
 * There is an array of timings between the edges, the length
 * is provided by cntEdges.
 * The name is also provided as parameter
 * */ 
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint8_t *timings,uint8_t cntEdges)
{
	uint16_t size = 2; //magic number
	uint16_t addr = 0; //address pointer 1
	uint16_t addr2 = 0; //address pointer 2
	
	//determine the size of this slot, maybe we need to defrag the EEPROM
	while(*(name + addr) != '\0') addr++;
	size++; //count edges field
	size += cntEdges;
	
	
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print(F("IR slot size:"));
		Serial.println(size);
	}
	
	//if this is the last slot, don't move other slots
	//TBD: defragmentation, currently not necessary (all slots are deleted before saving)
	
	/** save this slot **/
	//first slot, start right after the storage header
	if(nr == 0) slotAdresses.startIRAddress[nr] = EEPROM_MAX_ADDRESS;
	
	//load the start address
	addr = slotAdresses.startSlotAddress[nr];
	
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print(F("Start new slot addr@"));
		Serial.println(addr);
	}
	
	//write both bytes of the magicnumber, indicating a valid slot
	writeEEPROM(addr--,(EEPROM_MAGIC_NUMBER_IR & 0x00FF));
	writeEEPROM(addr--,(EEPROM_MAGIC_NUMBER_IR & 0xFF00)>>8);
	
	//write the slotname
	addr2 = 0;
	do {
		writeEEPROM(addr--,*(name + addr2));
		addr2++;
	} while(*(name + addr2 -1) != '\0');
	
	//write count of edges
	writeEEPROM(addr--,cntEdges);
	
	//write all edges
	for(uint8_t i = 0; i < cntEdges; i++)
	{
		writeEEPROM(addr--,*(timings+i));
	}
	
	/** update the slotAdresses table */
	//is this the last slot? If yes, just store the end of settings memory
	if(nr == (EEPROM_COUNT_IRCOMMAND - 1)) 
	{
		
	} else {
		//if this is not the last slot, resave all following start adresses
		slotAdresses.startIRAddress[nr+1] = addr;
		for(uint8_t i = (nr+1); i<EEPROM_COUNT_SLOTS; i++)
		{
			//TODO!!!!
			//slotAdresses.startSlotAddress[i] = ;
		}
		//slotAdresses.endSlotAddress = ;
	}
	
	
	//store the storageheader permanently to the EEPROM
	//bootstrap is not necessary, already updated previously
	for(uint16_t i = 0; i<sizeof(storageHeader); i++)
	{
		writeEEPROM(i, *((uint8_t*)&slotAdresses+i));
	}	
}

/**
 * Print out all slotnames to the serial interface
 * Used by the PC-GUI to load all available slot names
 * */
void listIRCommands()
{
	uint16_t address; //current EEPROM address
	uint8_t b;

	//pre-load all starting adresses from the memory
	bootstrapSlotAddresses();
	
	//iterate all possible slots
	for(uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
	{
		//load the base address for the current slot
		address = slotAdresses.startIRAddress[i];
		//check both bytes of the magicnumber, indicating a valid slot
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF)) continue;
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00)>>8) continue;

		//print out the slot name
		Serial.print("IRCommand"); Serial.print(i); Serial.print(":");
		while ((b=readEEPROM(address++)) && b != 0) Serial.write(b); // print slot name 
		Serial.println();
	}
}

/**
 * Replay one IR command from the EEPROM.
 * The slot is identified by the slot number
 * ATTENTION: if this method is not called from another function (e.g. readIRFromEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!!!
 * */
uint16_t readIRFromEEPROMSlotNumber(uint8_t slotNr,uint8_t *timings,uint8_t maxEdges)
{
	uint8_t* p;
	uint8_t slotEdges = 0;
	uint16_t address = slotAdresses.startIRAddress[slotNr];
	
	if(slotNr > EEPROM_COUNT_IRCOMMAND) return 0;
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG) Serial.println("Load IR command by nr");
	
	//check both bytes of the magicnumber, indicating a valid slot
	if(readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF)) return 0;
	if(readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00)>>8) return 0;
	
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.print("Magic bytes OK, loading IR data @");
		Serial.println(address);
	}

	//read the slotname
	//TODO: wait for the merge...
	/*p = (uint8_t *)&IRName;
	do {
		*p++ = readEEPROM(address--);
	} while(*(p-1) != '\0');*/
	while(readEEPROM(address--) != 0);
	
	//TODO: also wait for the merge
	//if (DebugOutput==DEBUG_FULLOUTPUT) {  Serial.print("found IR command: "); Serial.println(slotName); }
	
	slotEdges = readEEPROM(address--);

	//load all IR command edges
	for(uint8_t i = 0; i < maxEdges; i++)
	{
		if(i==slotEdges) break;
		*(timings+i) = readEEPROM(address--);
	}
	
	//return the count of available edges (used by the IR parser)
	return slotEdges;
}

/**
 * Replay one IR command from the EEPROM.
 * The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name,uint8_t *timings,uint8_t maxEdges)
{
	int8_t nr = slotnameIRToNumber(name);
	#ifdef EEPROM_FULL_DEBUG
		Serial.print(F("Load IR command "));
		Serial.print(name);
		Serial.print("@");
		Serial.println(nr);
	#endif
	//call the method which loads the data
	if(nr >= 0)
	{
		return readIRFromEEPROMSlotNumber(nr, timings, maxEdges);
	} else {
		return 0;
	}
}


/**
 * Load the EEPROM header, which contains all start addresses of the
 * slots and IR commands
 * */
void bootstrapSlotAddresses()
{
	for(uint16_t i = 0; i < sizeof(storageHeader); i++)
	{
		*((uint8_t *)&slotAdresses + i) = readEEPROM(i);
	}
	if(eepromDebugLevel == EEPROM_BASIC_DEBUG)
	{
		Serial.println(F("EEPROM slot address struct (bootstrap):"));
		for(uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
		{
			Serial.print(i);
			Serial.print(": ");
			Serial.println(slotAdresses.startSlotAddress[i]);
		}
		for(uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
		{
			Serial.print(i);
			Serial.print(": ");
			Serial.println(slotAdresses.startIRAddress[i]);
		}
	}
}

/**
 * Print out all slotnames to the serial interface
 * Used by the PC-GUI to load all available slot names
 * */
void listSlots()
{
	uint16_t address; //current EEPROM address
	uint8_t b;

	//pre-load all starting adresses from the memory
	bootstrapSlotAddresses();
	
	//iterate all possible slots
	for(uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
	{
		//load the base address for the current slot
		address = slotAdresses.startSlotAddress[i];
		//check both bytes of the magicnumber, indicating a valid slot
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) continue;
		if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8) continue;
		
		//skip the general settings, we only need the slot names
		address += sizeof(slotGeneralSettings);
		
		//skip '\0' seperator
		address++;
		
		//print out the slot name
		Serial.print("Slot"); Serial.print(i); Serial.print(":");
		while ((b=readEEPROM(address++)) && b != 0) Serial.write(b); // print slot name 
		Serial.println();
	}
}




