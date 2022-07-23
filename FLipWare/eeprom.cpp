/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: eeprom.cpp - implementation of the EEPROM storage management

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


/**
   TODO:
   -) implement overwrite/move for IR commands
   -) implement a "memory full" command
 * */

#include "eeprom.h"
#include "reporting.h"
#include "tone.h"

#include <FS.h>
#include <LittleFS.h>

/**
   Local copy of all slot and ir command start adresses
 * */
struct storageHeader header;
struct irCommandHeader irCommand;

/**
   Save the current slot by the given slotname.
   If there is no slot existing with this name,
   it will be saved to a new slot.
   returns 1 if successful, 0 if maximum slot count is reached
 * */
uint8_t saveToEEPROM(char * slotname)
{
  uint32_t timestamp = millis();
  
  //check if a slot with this name is available
  int8_t nr = slotnameToNumber(slotname);
  
  //if not
  if (nr == -1) {
    //get last slot index number, add 1 and store.
      saveToEEPROMSlotNumber(getLastSlotIndex()+1, slotname);
  } else {
    //overwrite existing one
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Overwrite Slot ");
      Serial.print(slotname);
      Serial.print(", slot index= ");
      Serial.println(nr);
    #endif
    saveToEEPROMSlotNumber(nr, slotname);
  }
  return 1;
}

/**
   Store current slot data to the EEPROM.
   nr: the slot number.
   If the start address of the given slot is != 0, the slot will be 
   overwritten (other slot data will be moved if necessary).
   If the start address of the slot is 0 (first free slot), 
   a new slot will be created, starting at the end address of 
   the previous slot. 
 * */
void saveToEEPROMSlotNumber(int8_t nr, char * slotname)
{
  size_t size = 0;
  
  // determine the size of this slot
  size += sizeof(SlotSettings);
  size += sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS;
  size += slotSettings.keystringBufferLen;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Slotsize:");
    Serial.println(size);
  #endif

  /** save this slot **/
  char path[32];
  uint8_t revision = 1; //TODO: determine current settings revision number
  sprintf(path,"/%03d/%02d",revision,nr);
  File f = LittleFS.open(path,"w");

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Start new slot: ");
    Serial.println(path);
  #endif
  
  if (!f) {
    Serial.println("file open failed");
    return;
  }

  // save the name
  f.print(slotname);
  f.write('\0');
  // save the general slotSettings to the global slotSettings struct
  f.write((uint8_t *)&slotSettings, sizeof(SlotSettings));
  // write all button slotSettings
  f.write((char*)buttons, sizeof(slotButtonSettings) * NUMBER_OF_BUTTONS);
  // write all keystrings
  f.write(keystringBuffer, slotSettings.keystringBufferLen);
  
  //finished
  f.close();
}

/**
   slotnameToNumber(char * slotname)
   Determines the slot number for a given slot name
 * */
int8_t slotnameToNumber(char * slotname)
{
  /** open current directory */
  char path[32];
  uint8_t revision = 1; //TODO: determine current settings revision number
  sprintf(path,"/%03d",revision);
  Dir dir = LittleFS.openDir(path);
  
  /** find a slot for given name */
  uint8_t slotnumber = 0;
  while (dir.next()) {
    if(dir.fileSize()) {
      File f = dir.openFile("r");
      String name = f.readString();
      f.close();
      
      if(name.equals(slotname)) return slotnumber; 
      else slotnumber++;
    }
  }
  //not found
  return -1;
}


/**
   Load one slot by the given slotname
   If there is no slotname given: load next slot...
   returns 1 if successful, 0 otherwise
 * */
uint8_t readFromEEPROM(char * slotname)
{
  if (*slotname == 0) return readFromEEPROMSlotNumber(actSlot + 1, true);

  int8_t nr = slotnameToNumber(slotname);
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Load slot ");
    Serial.print(slotname);
    Serial.print("@");
    Serial.println(nr);
  #endif
  //call the method which loads the data
  if (nr >= 0) return readFromEEPROMSlotNumber(nr, true);
  else return readFromEEPROMSlotNumber(0, true);
  
}

/**
   Read one slot data from the EEPROM to the global variables
   The slot is identified by the slot number.
   if the playTone flag is set, a tone according to the current slot number will be played
   returns 1 if successful, 0 otherwise   
 * */
uint8_t readFromEEPROMSlotNumber(uint8_t nr, bool playTone)
{
  /** open & read this slot **/
  char path[32];
  uint8_t revision = 1; //TODO: determine current settings revision number
  sprintf(path,"/%03d/%02d",revision,nr);
  
  if(!LittleFS.exists(path))
  {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print(nr);
      Serial.println(" Slot not found!");
    #endif
    return 0;
  }
  
  File f = LittleFS.open(path,"r");
  
  if(!f) return 0;
  
  //read slotname
  String slotname = f.readString();
  // load the general slotSettings struct
  f.readBytes((char *)&slotSettings, sizeof(SlotSettings));
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("read slotname "); Serial.println(slotSettings.slotName);
  #endif

  // load all button slotSettings
  f.readBytes((char *)buttons, sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS);

  // load keystring buffer
  f.readBytes(keystringBuffer, slotSettings.keystringBufferLen);
  initButtonKeystrings();

  if (reportSlotParameters != REPORT_NONE)
    printCurrentSlot();

  if (playTone) makeTone(TONE_CHANGESLOT, actSlot);
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("actSlot: "); Serial.println(actSlot);
  #endif

  if (reportSlotParameters)
    Serial.println("END");   // important: end marker for slot parameter list (command "load all" - AT LA)

  return(1);
}


//
//  Infrared Code Storage
//

/**
   Determines the irSlot number for a given ir Name
 * */
int8_t slotnameIRToNumber(char * irName)
{
  #if 0
  
  uint16_t address; //current EEPROM address

  //iterate all possible slots
  for (uint8_t i = 0; (i < MAX_IRCOMMANDS_IN_EERPOM) && header.startIRAddress[i]; i++)  {
    //load the base address for the current slot
    address = header.startIRAddress[i] - sizeof (irCommand);
    readEEPROMBin((uint8_t *)&irCommand, address, sizeof (irCommand));
    //compare the slotname
    if (!strcmp(irCommand.irName, irName))
      return i;  // IR slotname found!
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("No IR slotName found for ");
    Serial.println(irName);
  #endif
  #endif
  return -1;
  
  
}



/**
   This function deletes one IR command
   If the "name" parameter is set to \0, all IR commands will be deleted.
   returns 1 if successful, 0 otherwise

 * */
uint8_t deleteIRCommand(char * name)
{
  //TODO!
  Serial.println("IR deleting not implemented...");
  return 0;
  
  #if 0
  int8_t nr;
  if (strlen(name)) {
    nr = slotnameIRToNumber(name);
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Deleting slot ");
      Serial.print(name);
      Serial.print("@");
      Serial.println(nr);
    #endif
    if (nr >= 0) {
      // TBD: move data of other ir Slots !
      for (uint8_t i = nr; i < MAX_IRCOMMANDS_IN_EERPOM-1; i++) {
        header.startIRAddress[i] = header.startIRAddress[i+1];
        header.endIRAddress[i] = header.endIRAddress[i+1];
      }
    } else return(0);
  } else {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.println("Deleting all IR slots");
    #endif
    for (uint8_t i = 0; i < MAX_IRCOMMANDS_IN_EERPOM; i++) {
      header.startIRAddress[i] = 0;
      header.endIRAddress[i] = 0;
    }
  }
  storeHeader();
    
  #endif
  return(1);
}

/**
   Save one IR command to the EEPROM. If the name is already assigned,
   it will be overwritten, otherwise a new slot will be used.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROM(char * name, unsigned int *timings, size_t cntEdges)
{
  /*int8_t nr = slotnameIRToNumber(name);
  uint16_t irSlot=0;
  if (nr == -1)  {
    // a new IR slot, search for the first adress which is "0"
    while  ((irSlot < MAX_IRCOMMANDS_IN_EERPOM) && header.startIRAddress[irSlot])
      irSlot++;

    if (irSlot < MAX_IRCOMMANDS_IN_EERPOM) {
      #ifdef DEBUG_OUTPUT_BASIC
        Serial.print("New IR command @");
        Serial.println(irSlot);
      #endif
      saveIRToEEPROMSlotNumber(irSlot, name, timings, cntEdges);
    } else Serial.println ("IR memory full, code not saved.");
  }
  else {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Overwrite IR Slot ");
      Serial.print(name);
      Serial.print(" at position ");
      Serial.println(nr);
    #endif
    saveIRToEEPROMSlotNumber(nr, name, timings, cntEdges);
  }*/
}


/**
   Save one IR command to the EEPROM. The slot is determined by the slot number.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint16_t *timings, uint8_t cntEdges)
{
	Serial.println("###TBD###: save slot IR nr (load a specific file? Or access somthing in a structured or binary file)");
	#if 0
  uint16_t size;
  uint16_t addr = 0; 

  //determine the size of this slot
  size = sizeof (irCommand) + cntEdges * 2;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("IR slot size:");
    Serial.println(size);
  #endif

  //if IR slot is not empty: overwrite (eventually move other IR slots)
  if (header.startIRAddress[nr] != 0)  {
    int old_size = header.startIRAddress[nr] - header.endIRAddress[nr];
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("old IR slotsize:");
      Serial.println(old_size);
    #endif
    if (old_size != size) {
      int bytesToMove = old_size - size;
      // TBD:  moveIRSlotsInEEPROM(nr, bytesToMove);
    }
  }

  /** save this irSlot **/
  //if  this is the first IR slot, start right at the top address (maximum EEPROM address minus 1)
  if (nr == 0) addr = EEPROM_MAX_ADDRESS-1;
  else addr = header.endIRAddress[nr-1];  // else start at end address of last IR slot

  header.startIRAddress[nr]=addr;
  
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Start new irSlot addr@");
    Serial.println(addr);
  #endif

  strcpy(irCommand.irName, name);
  irCommand.edges=cntEdges;

  // write name and edgecount
  addr-=sizeof(irCommand);
  writeEEPROMBin(addr, (uint8_t *)&irCommand, sizeof(irCommand));

  // write all edges  
  addr-=cntEdges*2;
  writeEEPROMBin(addr, (uint8_t *)timings, cntEdges*2);

  /** update the header table */
  header.endIRAddress[nr] = addr;
  storeHeader();
  #endif
}

/**
   Load one IR command from the EEPROM.
   identified by the command name
 * */
size_t readIRFromEEPROM(char * name, uint16_t *timings, size_t maxEdges)
{
  //TODO!
  Serial.println("IR saving not implemented...");
  return 0;
    
  #if 0
  int8_t nr = slotnameIRToNumber(name);

  if (nr < 0) {
    #ifdef DEBUG_OUTPUT_FULL
      Serial.print("Could not find IR command ");
      Serial.println(name);
    #endif
    return 0;
  }

  uint16_t address = header.startIRAddress[nr];

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Load IR command by nr: ");
    Serial.print(nr);
    Serial.print(" from address: ");
    Serial.println(address);
  #endif
 
  address-=sizeof(irCommand)+irCommand.edges*2 ;   // set address to ir data start!
  readEEPROMBin((uint8_t *) timings, address, irCommand.edges*2);

  //return the count of available edges (used by the IR parser)
  return irCommand.edges;
    
  #endif
}


/**
   Load the EEPROM header, which contains all start addresses of the
   slots and IR commands
 * */
void bootstrapSlotAddresses()
{
  uint8_t init_needed = 0;
  uint16_t versionID = 0;

  LittleFS.begin();
  
  // check if we need to initialize FS (/rev.bin not found)
  if (!LittleFS.exists("/rev.bin")) {
    makeTone(TONE_CHANGESLOT, 4);
    Serial.println("Initializing flash!");
    File f = LittleFS.open("/rev.bin", "w");
    if (!f) {
        Serial.println("file open failed");
    } else {
      f.println(VERSION_STRING);
      f.close();
    }
    saveToEEPROMSlotNumber(0, "mouse");   // save default slotSettings to first slot
  }
  
  //if no slot is found, save current startup settings to "mouse"
  if(getLastSlotIndex() < 0) {
    saveToEEPROMSlotNumber(0, "mouse");
  }
}


/**
   This function deletes a slot from EEPROM.
   if an empty string is given as parameter, all slots are deleted!
   returns 1 if successful, 0 otherwise
 * */
uint8_t deleteSlot(char * name)
{
  char path[32];
  uint8_t revision = 1; //TODO: determine current settings revision number
  
  if (!strlen(name)) {
    #ifdef DEBUG_OUTPUT_BASIC
        Serial.println("Deleting all slots");
    #endif
    /** open current directory */
    
    
    sprintf(path,"/%03d",revision);
    LittleFS.remove(path);
    LittleFS.mkdir(path);
    return (1);
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Deleting slot ");
    Serial.println(name);      
  #endif
  
  int8_t nr = slotnameToNumber(name);
  if (nr < 0) return (0);  // slot name does not exist
  
  //delete file
  sprintf(path,"/%03d/%02d",revision,nr);
  LittleFS.remove(path);

  int lastSlot= getLastSlotIndex();
  if (nr != lastSlot) {
    // we must move files
    char pathTo[32];
    for (int i=nr; i<lastSlot;i++) {
      sprintf(path,"/%03d/%02d",revision,i+1);
      sprintf(pathTo,"/%03d/%02d",revision,i);
      LittleFS.rename(path,pathTo);
    }
  }
  return(1);
}


/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listSlots()
{
  char path[32];
  uint8_t revision = 1; //TODO: determine current settings revision number
  for(int8_t i = 0; i<MAX_SLOTS_IN_EERPOM; i++)
  {
    sprintf(path,"/%03d/%02d",revision,i);
    File f = LittleFS.open(path,"r");
    if(f)
    {
      //read slotname
      String slotname = f.readString();
      f.close();
      Serial.print("Slot"); Serial.print(i); Serial.print(":");
      Serial.println(slotname);
    } else return;
  }
}


/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listIRCommands()
{
	Serial.println("####TBD - EEPROM:### IR cmds from file");
  /*uint16_t address; //current EEPROM address
  char tmp[MAX_NAME_LEN];

  //iterate all possible slots
  for (uint8_t i = 0; i < MAX_IRCOMMANDS_IN_EERPOM; i++) {
    //load the base address for the current slot
    address = header.startIRAddress[i];
    if (address) {
      //print out the irSlot name
      Serial.print("IRCommand"); Serial.print(i); Serial.print(":");
      address-=sizeof(irCommand);
      readEEPROMBin(tmp, address, MAX_NAME_LEN);
      Serial.println(tmp);
    }
  }*/
}

/**
   get the index of the last slot which holds data  
 * */
int8_t getLastSlotIndex(void) {
  char path[32];
  uint8_t revision = 1; //TODO: determine current settings revision number
  for(int8_t i = 0; i<MAX_SLOTS_IN_EERPOM; i++)
  {
    sprintf(path,"/%03d/%02d",revision,i);
    if(!LittleFS.exists(path)) return i-1;
  }
  return -1;
}



/**
   print all slot slotSettings and button mode to serial 
 * */
void printAllSlots(void) {
  uint8_t actSlot=0;
  reportSlotParameters = REPORT_ALL_SLOTS;
  for(int8_t i = 0; i<=getLastSlotIndex(); i++) {
    readFromEEPROMSlotNumber(i, false);
  }
  reportSlotParameters = REPORT_NONE;
  if(getLastSlotIndex() >= 0)
    readFromEEPROMSlotNumber(0, true);
  else Serial.println("END");
}
