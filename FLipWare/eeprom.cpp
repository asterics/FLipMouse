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
uint8_t saveToEEPROM(char const * slotname)
{
  
  //check if a slot with this name is available
  int8_t nr = slotnameToNumber(slotname);
  
  //if not
  if (nr == -1) {
    //get last slot index number, add 1 and store.
      saveToEEPROMSlotNumber(getLastSlotIndex()+1, slotname);
  } else {
    //overwrite existing one
    #ifdef DEBUG_OUTPUT_MEMORY
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
void saveToEEPROMSlotNumber(int8_t nr, char const * slotname)
{
  (void)slotname; //TODO: this implementation currently uses the slotname from slotSettings.
  
  /** save this slot **/
  char path[32];
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  sprintf(path,"/%03d/%02d",revision,nr);
  File f = LittleFS.open(path,"w");

  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("Start new slot: ");
    Serial.println(path);
  #endif
  
  if (!f) {
    Serial.println("file open failed");
    return;
  }

  //just use the reporting function to put all our settings into the file
  printCurrentSlot(&f);
  
  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("Slotsize:");
    Serial.println(f.size());
  #endif
  
  //finished
  f.close();
}

/**
   slotnameToNumber(char * slotname)
   Determines the slot number for a given slot name
 * */
int8_t slotnameToNumber(char const * slotname)
{
  /** open current directory */
  char path[32];
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  sprintf(path,"/%03d",revision);
  Dir dir = LittleFS.openDir(path);
  
  /** find a slot for given name */
  uint8_t slotnumber = 0;
  while (dir.next()) {
    if(dir.fileSize()) {
      File f = dir.openFile("r");
      String name = f.readStringUntil('\n');
      name.trim();
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
uint8_t readFromEEPROM(char const * slotname)
{
  if (*slotname == 0)
  {
    //either load next slot or first slot.
    #ifdef DEBUG_OUTPUT_MEMORY
			Serial.print("Load next:");
			if(actSlot == getLastSlotIndex()) Serial.println("0");
			else Serial.println(actSlot + 1);
		#endif
    if(actSlot == getLastSlotIndex()) return readFromEEPROMSlotNumber(0, true);
    else return readFromEEPROMSlotNumber(actSlot + 1, true);
  }

  int8_t nr = slotnameToNumber(slotname);
  #ifdef DEBUG_OUTPUT_MEMORY
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
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  sprintf(path,"/%03d/%02d",revision,nr);
  
  if(!LittleFS.exists(path))
  {
    #ifdef DEBUG_OUTPUT_MEMORY
      Serial.print(nr);
      Serial.println(" Slot not found!");
    #endif
    return 0;
  }
  
  File f = LittleFS.open(path,"r");
  
  if(!f) return 0;

  // load name & store to slotSettings
  String name = f.readStringUntil('\n');
  name.trim();
  strncpy(slotSettings.slotName,name.c_str(),MAX_NAME_LEN);
  
  
  // read line by line & feed into parser
  String line = "";
  do{
		//check for remaining byte size, otherwise readStringUntil hangs until timeout
		if((f.position()+5) > f.size()) break;
    line = f.readStringUntil('\n');
    line.trim();
    //check again length, we need at least 5 bytes: "AT ..", e.g. "AT NC"
    if(line.length() < 5) break;
    line = line.substring(3);
    //after substringing, we need at least 2 bytes: "..", e.g. "NC"
    if(line.length() < 2) break;
    #ifdef DEBUG_OUTPUT_MEMORY
      Serial.print("Sending to parser: ");
      Serial.println(line);
    #endif
    parseCommand((char*)line.c_str());
  } while(line.length()>0);
  
  //finished
  f.close();

  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("read slotname "); Serial.println(slotSettings.slotName);
  #endif

  //now next slot is active
  actSlot = nr;

  if (playTone) makeTone(TONE_CHANGESLOT, actSlot);
  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("actSlot: "); Serial.println(actSlot);
  #endif

  return(1);
}


//
//  Infrared Code Storage
//

/**
   Determines the irSlot number for a given ir Name
 * */
int8_t slotnameIRToNumber(char const * irName)
{
  /** open current directory */
  char path[32];
  sprintf(path,"/ir");
  Dir dir = LittleFS.openDir(path);
  
  /** find a slot for given name */
  uint8_t slotnumber = 0;
  while (dir.next()) {
    if(dir.fileSize()) {
      File f = dir.openFile("r");
      String name = f.readString();
      f.close();
      
      if(name.equals(irName)) return slotnumber; 
      else slotnumber++;
    }
  }
  //not found
  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("No IR slotName found for ");
    Serial.println(irName);
  #endif
  return -1;
}



/**
   This function deletes one IR command
   If the "name" parameter is set to \0, all IR commands will be deleted.
   returns 1 if successful, 0 otherwise

 * */
uint8_t deleteIRCommand(char const * name)
{
  char path[32];
    
  if (!strlen(name)) {
    #ifdef DEBUG_OUTPUT_MEMORY
        Serial.println("Deleting all IR slots");
    #endif
    
    //open ir dir
    Dir dir = LittleFS.openDir("/ir");
    //and iterate over files to remove them.
    while(dir.next()) {
      LittleFS.remove(String("/ir/") + String(dir.fileName()));
    }
    return (1);
  }
  
  int8_t nr = slotnameIRToNumber(name);
  if (nr < 0) return (0);  // slot name does not exist
  
  //get current number of IR cmds
  int lastSlot= getLastIRIndex();
  
  #ifdef DEBUG_OUTPUT_MEMORY
      Serial.print("Deleting slot ");
      Serial.print(name);
      Serial.print("@");
      Serial.println(nr);   
  #endif
  
  //delete file
  sprintf(path,"/ir/%02d",nr);
  LittleFS.remove(path);

  
  if (nr != lastSlot) {
    // we must move files
    char pathTo[32];
    for (int i=nr; i<lastSlot;i++) {
      sprintf(path,"/ir/%02d",i+1);
      sprintf(pathTo,"/ir/%02d",i);
      LittleFS.rename(path,pathTo);
    }
  }
  
  return(1);
}

/**
   Save one IR command to the EEPROM. If the name is already assigned,
   it will be overwritten, otherwise a new slot will be used.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROM(char * name, uint16_t *timings, uint16_t cntEdges)
{
  int8_t nr = slotnameIRToNumber(name);
  uint16_t irSlot=0;
  if (nr == -1)  {
    // a new IR slot, use next free number
    irSlot = getLastIRIndex() + 1;

    if (irSlot < MAX_IRCOMMANDS_IN_EERPOM) {
      #ifdef DEBUG_OUTPUT_MEMORY
        Serial.print("New IR command @");
        Serial.println(irSlot);
      #endif
      saveIRToEEPROMSlotNumber(irSlot, name, timings, cntEdges);
    } else Serial.println ("IR memory full, code not saved.");
  }
  else {
    #ifdef DEBUG_OUTPUT_MEMORY
      Serial.print("Overwrite IR Slot ");
      Serial.print(name);
      Serial.print(" at position ");
      Serial.println(nr);
    #endif
    saveIRToEEPROMSlotNumber(nr, name, timings, cntEdges);
  }
}


/**
   Save one IR command to the EEPROM. The slot is determined by the slot number.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint16_t *timings, uint16_t cntEdges)
{
  #ifdef DEBUG_OUTPUT_MEMORY
    //determine the size of this slot
    size_t size = sizeof (irCommand) + cntEdges * 2;
    Serial.print("IR slot size:");
    Serial.println(size);
  #endif
  
  /** save this slot **/
  char path[32];
  sprintf(path,"/ir/%02d",nr);
  File f = LittleFS.open(path,"w");

  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("Start new slot: ");
    Serial.println(path);
  #endif
  
  if (!f) {
    Serial.println("file open failed");
    return;
  }
  
  //prepare header
  strncpy(irCommand.irName, name, MAX_NAME_LEN);
  irCommand.edges = cntEdges;
  
  // save the command header (name / edgecount
  f.write((uint8_t *)&irCommand, sizeof(irCommandHeader));
  
  // write IR data
  f.write((uint8_t *)timings, cntEdges * 2);
  
  //finished
  f.close();
}

/**
   Replay one IR command from the EEPROM.
   The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name, uint16_t *timings, uint16_t maxEdges)
{
  int8_t nr = slotnameIRToNumber(name);

  if (nr < 0) {
    #ifdef DEBUG_OUTPUT_FULL
      Serial.print("Could not find IR command ");
      Serial.println(name);
    #endif
    return 0;
  }
  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("Load IR command by nr: ");
    Serial.print(nr);
  #endif
  
  char path[32];
  sprintf(path,"/ir/%02d",nr);
  
  if(!LittleFS.exists(path))
  {
    #ifdef DEBUG_OUTPUT_MEMORY
      Serial.print(nr);
      Serial.println(" IRcmd not found!");
    #endif
    return 0;
  }
  
  File f = LittleFS.open(path,"r");
  
  if(!f) return 0;

  // load the general ircmd struct, containing the name
  f.readBytes((char *)&irCommand, sizeof(irCommandHeader));
  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("read slotname "); Serial.println(irCommand.irName);
    Serial.print("edge count "); Serial.println(irCommand.edges);
  #endif
  
  if(irCommand.edges > maxEdges) {
    f.close();
    return 0;
  }
  
  f.readBytes((char *)timings, irCommand.edges * 2);
  f.close();

  //return the count of available edges (used by the IR parser)
  return irCommand.edges;
}


/**
   Check if storage is valid, initialize if not.
 * */
void initStorage()
{
  //uint16_t versionID = 0; //TODO: maybe implement the version ID to initialize/overwrite slots?

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
    
    //create IR folder
    LittleFS.mkdir("/ir");
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
uint8_t deleteSlot(char const * name)
{
  char path[32];
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  
  if (!strlen(name)) {
    #ifdef DEBUG_OUTPUT_MEMORY
        Serial.println("Deleting all slots");
    #endif
  
    //open current dir
    sprintf(path,"/%03d/",revision);
    Dir dir = LittleFS.openDir(path);
    //and iterate over files to remove them.
    while(dir.next()) {
      LittleFS.remove(String(path) + String(dir.fileName()));
    }
    return (1);
  }

  #ifdef DEBUG_OUTPUT_MEMORY
    Serial.print("Deleting slot ");
    Serial.println(name);      
  #endif
  
  int8_t nr = slotnameToNumber(name);
  if (nr < 0) return (0);  // slot name does not exist
  
  //get currently valid last slot
  int lastSlot = getLastSlotIndex();
  
  //delete file
  sprintf(path,"/%03d/%02d",revision,nr);
  LittleFS.remove(path);

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
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  for(int8_t i = 0; i<MAX_SLOTS_IN_EERPOM; i++)
  {
    sprintf(path,"/%03d/%02d",revision,i);
    File f = LittleFS.open(path,"r");
    if(f)
    {
      //read slotname (until new line, trim away CR)
      String slotname = f.readStringUntil('\n');
      slotname.trim();
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
  char path[32];
  for(int8_t i = 0; i<MAX_IRCOMMANDS_IN_EERPOM; i++)
  {
    sprintf(path,"/ir/%02d",i);
    File f = LittleFS.open(path,"r");
    if(f)
    {
      //read slotname
      String slotname = f.readString();
      f.close();
      Serial.print("IRCommand"); Serial.print(i); Serial.print(":");
      Serial.println(slotname);
    } else return;
  }
}

/**
   get the index of the last slot which holds data  
 * */
int8_t getLastSlotIndex(void) {
  char path[32];
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  for(int8_t i = 0; i<MAX_SLOTS_IN_EERPOM; i++)
  {
    sprintf(path,"/%03d/%02d",revision,i);
    if(!LittleFS.exists(path)) return i-1;
  }
  return -1;
}

/**
   get the index of the last IR command  
 * */
int8_t getLastIRIndex(void) {
  char path[32];
  for(int8_t i = 0; i<MAX_IRCOMMANDS_IN_EERPOM; i++)
  {
    sprintf(path,"/ir/%02d",i);
    if(!LittleFS.exists(path)) return i-1;
  }
  return -1;
}



/**
   print all slot slotSettings and button mode to serial 
 * */
void printAllSlots(void) {
  char path[32];
  uint8_t revision = getSettingsRevision(); //determine current settings revision number
  
  for(uint8_t i = 0; i<MAX_SLOTS_IN_EERPOM; i++)
  {
	  sprintf(path,"/%03d/%02d",revision,i);
	  File f = LittleFS.open(path,"r");
	  if(f)
	  {
		  String slot = f.readString();
		  Serial.print("Slot"); Serial.print(":");
		  Serial.print(slot);
	  } else break;
	}
	Serial.println("END");
}

/**
 * If settings are under version control (TBD!), this function returns the current
 * revision (-> foldername in FS) to read from / write to.
 */
uint8_t getSettingsRevision(void)
{
	//TODO: implement revisions (or use the first folder name for something different)
	return 1;
}
