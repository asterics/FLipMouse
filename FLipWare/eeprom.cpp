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

#include "eeprom.h"

/** Device address of the EEPROM **/
#define deviceaddress 0x50

/**
 * Local copy of all slot and ir command start adresses
 * */
struct storageHeader slotAdresses;

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
 
    return rdata;
  #else
    return(EEPROM.read(eeaddress));
  #endif
}



void saveToEEPROM(char * slotname)
{
} 
/*
   char act_slotname[MAX_SLOTNAME_LEN];
   int address = 0;
   int tmpStartAddress=0;
   uint8_t found=0;
   uint8_t * p;

   if (!slotname) address=EmptySlotAddress;
   else
   {
     if (strlen(slotname) >= MAX_SLOTNAME_LEN) slotname[MAX_SLOTNAME_LEN-1]=0;
     
     while ((readEEPROM(address)==SLOT_VALID) && (!found) && ((address+1) < EmptySlotAddress))  // indicates valid eeprom content !
     {
       tmpStartAddress=address;
       address++;

       uint8_t i=0;
       while ((act_slotname[i++]=readEEPROM(address++)) != 0) ;      

       // Serial.print("peeking slot "); Serial.print(act_slotname); Serial.print(" @ "); Serial.println(tmpStartAddress);
       address=tmpStartAddress;
       if (!strcmp(act_slotname, slotname)) found=1; 
       else  address+= (sizeof(settingsType)+NUMBER_OF_BUTTONS*sizeof(buttonType)+1);      
     }
   }

   if (DebugOutput==DEBUG_FULLOUTPUT)  {
     Serial.print("Writing slot "); if (slotname) Serial.print(slotname);
     Serial.print(" starting from EEPROM address "); Serial.println(address);
   }
   
   // start with new slot 
   writeEEPROM(address++,SLOT_VALID);  

   // update slotname
   if (!slotname) settings.slotname[0]=0;
   else strcpy(settings.slotname,slotname);
         
   // write general settings 
   p = (uint8_t*) &settings;
   for (unsigned int t=0;t<sizeof(settingsType);t++)
      writeEEPROM(address++,*p++);

   // write all buttons
   p = (uint8_t*) buttons;
   for (unsigned int i=0;i<NUMBER_OF_BUTTONS*sizeof(buttonType);i++)
        writeEEPROM(address++,*p++);

   if (EmptySlotAddress<=address) {
     writeEEPROM(address,0);
     EmptySlotAddress=address;
  
  }
}*/

void readFromEEPROM(char * slotname)
{
	uint16_t address; //current EEPROM address
	uint8_t matches = 1;

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

		//compare the slotname
		for (unsigned int t=0;t<MAX_SLOTNAME_LEN;t++)
		{
			//compare byte for byte
			if(*(slotname + t) != readEEPROM(address + t))
			{
				//if one byte doesn't match, reset match flag and break the for loop
				matches = 0;
				break;
			}
			//if the end of the string is reached, break the for loop
			if(*(slotname + t) == '\0') break;
		}
		
		//if it matches, load all remaining settings & buttons
		if(matches)
		{
			//call the method which loads the data
			readFromEEPROMSlotNumber(i);
			return;
		}
	}
}

/**
 * Read one slot data from the EEPROM to the global variables
 * The slot is identified by the slot number
 * ATTENTION: if this method is not called from another function (e.g. readFromEEPROM()),
 * it is necessary to preload the start adresses via bootstrapSlotAddresses()!!!
 * */
void readFromEEPROMSlotNumber(uint8_t nr)
{
	uint8_t* p = (uint8_t *)&settings;
	uint16_t address = slotAdresses.startSlotAddress[nr];
	
	//check both bytes of the magicnumber, indicating a valid slot
	if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) Serial.println(F("Error: no slot found at given slotnr"));
	if(readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00)>>8) Serial.println(F("Error: no slot found at given slotnr"));
	
	//save the general settings to the global settings struct
	for (unsigned int t=0;t<sizeof(slotGeneralSettings);t++) *p++=readEEPROM(address++);
	
	//skip '\0' seperator
	address++;
	
	//skip the slotname
	while(readEEPROM(address++) != '\0');
	
	//load all button settings
	for(uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
	{
		//TODO: TBD....
	}
}
	
	
	
	
/*
	
	
	
   char act_slotname[MAX_SLOTNAME_LEN];
   int address=0;
   int tmpSlotAddress=0;
   int tmpStartAddress=0;
   //uint8_t done;
   uint8_t numSlots=0;
   uint8_t* p;
   
   
   while (readEEPROM(address)==SLOT_VALID)  // indicates valid eeprom content !
   {
      uint8_t found=0;
     
      if ((!slotname) && (address==nextSlotAddress)) found=1;
      address++;

      tmpStartAddress=address;
      uint8_t i=0;
      while ((act_slotname[i++]=readEEPROM(address++)) != 0) ; 
 
      if (DebugOutput==DEBUG_FULLOUTPUT)  
      {  Serial.print("found slotname "); Serial.println(act_slotname); }
     
      if (slotname)  {
        if (!strcmp(act_slotname, slotname)) found=1;  
      }
      
      address=tmpStartAddress;
      if ((found) || (reportSlotParameters==REPORT_ALL_SLOTS))  {      
           
        p = (uint8_t*) &settings;
        for (unsigned int t=0;t<sizeof(settingsType);t++)
            *p++=readEEPROM(address++);
        
        p = (uint8_t*) buttons;
        for (unsigned int i=0;i<NUMBER_OF_BUTTONS*sizeof(buttonType);i++) 
           *p++=readEEPROM(address++);
        
         if (reportSlotParameters!=REPORT_NONE)  
           printCurrentSlot();

        actSlot=numSlots+1; 
        tmpSlotAddress=address;
      } 
      else address += (sizeof(settingsType) + NUMBER_OF_BUTTONS * sizeof(buttonType));   // skip this slot  
      numSlots++;
   }
   
   makeTone(TONE_CHANGESLOT,actSlot);

   
   if (DebugOutput==DEBUG_FULLOUTPUT)  
   {
       Serial.print(numSlots); Serial.print(" slots were found in EEPROM, occupying ");
       Serial.print(address); Serial.println(" bytes.");
   }
   if (reportSlotParameters) 
       Serial.println("END");   // important: end marker for slot parameter list (command "load all" - AT LA)

}*/

/**
 * This function deletes all slots by deleting the magic number (reset to 0xFF)
 * */
void deleteSlots()
{
   for(uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++) 
   {
	   writeEEPROM(slotAdresses.startSlotAddress[i],0xFF);
	   writeEEPROM(slotAdresses.startSlotAddress[i]+1,0xFF);
   }
}

/**
 * This function deletes all IR commands by deleting the magic number (reset to 0xFF)
 * */
void deleteIRCommand()
{
	for(uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++) 
   {
	   writeEEPROM(slotAdresses.startIRAddress[i],0xFF);
	   writeEEPROM(slotAdresses.startIRAddress[i]+1,0xFF);
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
		*((uint8_t *)&slotAdresses + 1) = readEEPROM(i);
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
		
		//print out the slot name
		Serial.print("Slot"); Serial.print(i); Serial.print(":"); 
		while ((b=readEEPROM(address++)) != 0) Serial.write(b); // print slot name 
		Serial.println();
	}
}




