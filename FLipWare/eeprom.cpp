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

/** Device address of the EEPROM **/
#define deviceaddress 0x50
#define EEPROM_PAGESIZE 64
#define I2C_WRITE_BUFFER_LEN 16  // 64   Note: smaller i2c buffer size in Wire Library than in i2c_t3! -> adjust?
#define I2C_READ_BUFFER_LEN  16

/**
   Local copy of all slot and ir command start adresses
 * */
struct storageHeader header;
struct irCommandHeader irCommand;

/**
   check if EEPROM is ready
 * */
uint8_t eeprom_waitReady() {
  uint32_t timeout;
  //wait for the EEPROM device to complete a prior write, or 10ms
  timeout = millis();
  bool ready = false;
  while (!ready && (millis() - timeout < 10)) {
    Wire.beginTransmission((uint8_t)deviceaddress);
    ready = (Wire.endTransmission(true) == 0); // wait for device to become ready!
  }
  if (!ready) { // chip either does not exist, is hung, or died
    Serial.print(deviceaddress, HEX);
    Serial.println(F(" chip timeout"));
    return 0;
  }
  return (1);
}


/**
   Write one byte to the EEPROM
 * */
void writeEEPROM(unsigned int eeaddress, byte data )
{
  if (!eeprom_waitReady()) return;
  Wire.beginTransmission(deviceaddress);
  Wire.send((int)(eeaddress >> 8));   // MSB
  Wire.send((int)(eeaddress & 0xFF)); // LSB
  Wire.send(data);
  Wire.endTransmission();

  #ifdef DEBUG_OUTPUT_FULL
    Serial.print("EEPROM w@");
    Serial.print(eeaddress);
    Serial.print(": ");
    Serial.println(data);
  #endif
}

/**
  EEPROM page write - write multiple bytes to I2C EEPROM
  thanks to CuckTodd:
  https://forum.arduino.cc/t/faster-way-to-write-to-external-eeprom/429356/5
 * */
bool writeEEPROMBin(uint16_t adr, char data[], const uint16_t len) {
  #ifdef DEBUG_OUTPUT_FULL
    Serial.print("writeBin(0x");
    Serial.print(deviceaddress, HEX);
    Serial.print(",0x");
    Serial.print(adr, HEX);
    Serial.print(",len=");
    Serial.println(len, DEC);
  #endif
  uint16_t bk = len;
  bool abort = false;
  uint8_t i;
  uint16_t j = 0;
  uint32_t timeout;
  uint16_t mask = EEPROM_PAGESIZE - 1;
  while ((bk > 0) && !abort) {
    i = I2C_WRITE_BUFFER_LEN; // maximum data bytes that Wire.h can send in one transaction
    if (i > bk) i = bk; // data block is bigger than Wire.h can handle in one transaction
    if (((adr) & ~mask) != ((((adr) + i) - 1) & ~mask)) { // over page! block would wrap around page
      i = (((adr) | mask) - (adr)) + 1; // shrink the block until it stops at the end of the current page
    }
    //wait for the EEPROM device to complete a prior write, or 10ms
    if (!eeprom_waitReady()) return (false);
    // start sending this current block
    Wire.beginTransmission((uint8_t)deviceaddress);
    Wire.write((uint8_t)highByte(adr));
    Wire.write((uint8_t)lowByte(adr));
    bk = bk - i;
    adr = (adr) + i;
    while (i > 0) {
      // Serial.print(" ");
      // Serial.print((uint8_t)data[j],HEX);
      Wire.write((uint8_t)data[j++]);
      i--;
    }

    uint8_t err = Wire.endTransmission();
    if (err != 0) {
      Serial.print(F("write Failure="));
      Serial.println(err, DEC);
      abort = true;
      break;
    }
  }
  return !abort;
}

/**
   Read one byte from the EEPROM
 * */
byte readEEPROM(unsigned int eeaddress )
{
  if (!eeprom_waitReady()) return;
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.send((int)(eeaddress >> 8));   // MSB
  Wire.send((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.receive();
  #ifdef DEBUG_OUTPUT_FULL
    Serial.print("EEPROM r@");
    Serial.print(eeaddress);
    Serial.print(": ");
    Serial.println(rdata);
  #endif
  return rdata;
}

/**
   Read multiple bytes from the EEPROM
 * */
int readEEPROMBin(char * data, unsigned int eeaddress, int length) {
    uint16_t bytes_received = 0;

    while (length>0) {
      if (!eeprom_waitReady()) return(0);
      Wire.beginTransmission(deviceaddress);
      Wire.send((uint8_t)(eeaddress >> 8));
      Wire.send((uint8_t)(eeaddress & 0xff));
      Wire.endTransmission();

      uint16_t bytes_requested = min(length,I2C_READ_BUFFER_LEN);
      Wire.requestFrom(deviceaddress,bytes_requested);
  
      while (Wire.available() && bytes_requested--) {
          *data++ = Wire.receive();
          length--;
          bytes_received++;
          eeaddress++;
      }
    }

    return bytes_received;
}


/**
   move multiple bytes in EEPROM 
   targetAdr: start address for writing bytes 
   sourceAdr: start address for reading bytes 
   count: number of bytes to move
   moving overlapping regions is possible
 * */
void moveEEPROM(uint16_t targetAdr, uint16_t sourceAdr, int count) {
  uint8_t pagebuf[I2C_WRITE_BUFFER_LEN];
  uint16_t b;

  if (sourceAdr > targetAdr) {  // forward copy
    b=0;
    while (count--) {
      pagebuf[b++] = readEEPROM(sourceAdr++);
      if (b == I2C_WRITE_BUFFER_LEN) {
        writeEEPROMBin(targetAdr, pagebuf, I2C_WRITE_BUFFER_LEN);
        targetAdr += I2C_WRITE_BUFFER_LEN;
        b = 0;
      }
    }
    if (b) writeEEPROMBin(targetAdr, pagebuf, b);
  }
  else {  // backward copy
    sourceAdr+=count-1;
    targetAdr+=count;
    b = I2C_WRITE_BUFFER_LEN;
    while (count--) {
      b--;targetAdr--;
      pagebuf[b] = readEEPROM(sourceAdr--);
      if (b == 0) {
        writeEEPROMBin(targetAdr, pagebuf, I2C_WRITE_BUFFER_LEN);
        b = I2C_WRITE_BUFFER_LEN;
      }
    }
    if (b < I2C_WRITE_BUFFER_LEN) writeEEPROMBin(targetAdr, pagebuf + b, I2C_WRITE_BUFFER_LEN - b);
  }
}


/**
   store the storageheader permanently to the EEPROM
 * */
void storeHeader(void) {
  writeEEPROMBin(0, (uint8_t*)&header, sizeof(storageHeader));
}


/**
   Save the current slot by the given slotname.
   If there is no slot existing with this name,
   it will be saved to a new slot.
   returns 1 if successful, 0 if maximum slot count is reached
 * */
uint8_t saveToEEPROM(char * slotname)
{
  uint32_t timestamp = millis();
  int8_t nr = slotnameToNumber(slotname);
  uint16_t slotNum=0;
  uint8_t ret=1;

  if (nr == -1) {
    //a new slot, search for the first adress which is "0"
    while ((slotNum < MAX_SLOTS_IN_EERPOM) && header.startSlotAddress[slotNum])
      slotNum++;
      
    if (slotNum < MAX_SLOTS_IN_EERPOM) {
      #ifdef DEBUG_OUTPUT_BASIC
        Serial.print("New slot @");
        Serial.println(slotNum);
      #endif
      saveToEEPROMSlotNumber(slotNum, slotname);
    } else ret=0;
  }
  else {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Overwrite Slot ");
      Serial.print(slotname);
      Serial.print(", slot index= ");
      Serial.println(nr);
    #endif
    saveToEEPROMSlotNumber(nr, slotname);
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("EEPROM Save took ");
    Serial.print(millis()-timestamp);
    Serial.println(" milliseconds.");
    Serial.print("lastslotIndex = "); Serial.println(getLastSlotIndex()); 
    Serial.print("freeslot = "); Serial.println(getFreeSlotAddress()); 
  #endif
  return(ret);
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
  uint16_t size = 0;
  uint16_t addr = 0; //address pointer
  uint8_t* p = (uint8_t *)&slotSettings; //pointer to the global slotSettings

  // determine the size of this slot
  size += sizeof(SlotSettings);
  size += sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS;
  size += slotSettings.keystringBufferLen;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Slotsize:");
    Serial.println(size);
  #endif

  // if slot is not empty: overwrite (eventually move other slots)
  if ((header.startSlotAddress[nr] != 0) && (nr!=getLastSlotIndex())) {
    int old_size = header.endSlotAddress[nr] - header.startSlotAddress[nr];
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("old Slotsize:");
      Serial.println(old_size);
    #endif
    if (old_size != size) {
      int bytesToMove = size - old_size;
      int sourceAddress=header.startSlotAddress[nr+1];
      
      #ifdef DEBUG_OUTPUT_BASIC    
        if (bytesToMove < 0) {
          Serial.print("old slot size is bigger: decrease addresses of higher slots by ");
          Serial.print(-bytesToMove); Serial.println(" bytes ");
        } else {
          Serial.print("old slot size is smaller: increase addresses of higher slots by ");
          Serial.print(bytesToMove); Serial.println(" bytes ");
        }
        Serial.print("Move slot "); Serial.print(nr+1); 
        Serial.print(" starting ");  Serial.print(sourceAddress); 
        Serial.print(" to "); Serial.print(sourceAddress+bytesToMove); 
        Serial.print(", count= "); Serial.print(getFreeSlotAddress()-sourceAddress); 
        Serial.println(" bytes");
      #endif
      
      //  moveSlotsInEEPROM(nr, bytesToMove);
      moveEEPROM (sourceAddress+bytesToMove, sourceAddress, getFreeSlotAddress()-sourceAddress);

      // update header addresses
      for (int i=nr+1; i<MAX_SLOTS_IN_EERPOM;i++) {
        if (header.startSlotAddress[i]) {
          header.startSlotAddress[i]+=bytesToMove;
          header.endSlotAddress[i]+=bytesToMove;
        }
      }  
    }
  }

  /** save this slot **/
  // if first slot, start right after the storage header
  if (nr == 0)  addr = sizeof(storageHeader);
  else addr = header.endSlotAddress[nr - 1];

  header.startSlotAddress[nr] = addr;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Start new slot @");
    Serial.println(addr);
  #endif

  // save the general slotSettings to the global slotSettings struct
  writeEEPROMBin(addr, (uint8_t *)&slotSettings, sizeof(SlotSettings));
  addr += sizeof(SlotSettings);

  // write all button slotSettings
  writeEEPROMBin(addr, (char*)buttons, sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS);
  addr += sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS; 

  // write all keystrings
  writeEEPROMBin(addr, keystringBuffer, slotSettings.keystringBufferLen);
  addr += slotSettings.keystringBufferLen;

  /** update the header table */
  header.endSlotAddress[nr] = addr;
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("New slot @");
    Serial.print(nr);
    Serial.print(" written, start: ");
    Serial.print(header.startSlotAddress[nr]);
    Serial.print(", end: ");
    Serial.print(header.endSlotAddress[nr]);
    Serial.print(", size: ");
    Serial.println(header.endSlotAddress[nr] - header.startSlotAddress[nr]);
  #endif

  storeHeader();
}

/**
   slotnameToNumber(char * slotname)
   Determines the slot number for a given slot name
 * */
int8_t slotnameToNumber(char * slotname)
{
  uint16_t address; //current EEPROM address
  char tmp[MAX_NAME_LEN];

  //iterate all possible slots
  for (uint8_t i = 0; (i < MAX_SLOTS_IN_EERPOM) && header.startSlotAddress[i]; i++) {
    //load the base address for the current slot
    address = header.startSlotAddress[i];
    //compare the slotname
    readEEPROMBin(tmp, address, MAX_NAME_LEN);
    if (!strncmp(tmp, slotname,MAX_NAME_LEN)) 
      return i;   // name found!
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("No slot found for ");
    Serial.println(slotname);
  #endif
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
  if (nr >= 0) readFromEEPROMSlotNumber(nr, true);
  else readFromEEPROMSlotNumber(0, true);
}

/**
   Read one slot data from the EEPROM to the global variables
   The slot is identified by the slot number.
   if the playTone flag is set, a tone according to the current slot number will be played
   returns 1 if successful, 0 otherwise   
 * */
uint8_t readFromEEPROMSlotNumber(uint8_t nr, bool playTone)
{
  uint8_t* p;
  uint16_t address;

  //fence the slot number, avoiding out of array index problems
  if (nr >= MAX_SLOTS_IN_EERPOM) return;
  if (header.startSlotAddress[nr]==0)  nr=0;    // wrap around at last slot / free slot!
  if (header.startSlotAddress[nr]==0)  {
    // not even the first slot exists -> nothing to load!
    return (0);  
  }

  actSlot=nr;
  address = header.startSlotAddress[nr];

  // load the general slotSettings struct
  readEEPROMBin((uint8_t *)&slotSettings, address, sizeof(SlotSettings));
  address+=sizeof(SlotSettings);
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("read slotname "); Serial.println(slotSettings.slotName);
  #endif

  // load all button slotSettings
  readEEPROMBin((char *)buttons, address, sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS);
  address+=sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS;

  // load keystring buffer
  readEEPROMBin(keystringBuffer, address, slotSettings.keystringBufferLen);
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
  return -1;
}



/**
   This function deletes one IR command
   If the "name" parameter is set to \0, all IR commands will be deleted.
   returns 1 if successful, 0 otherwise

 * */
uint8_t deleteIRCommand(char * name)
{
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
  return(1);
}

/**
   Save one IR command to the EEPROM. If the name is already assigned,
   it will be overwritten, otherwise a new slot will be used.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROM(char * name, uint16_t *timings, uint8_t cntEdges)
{
  int8_t nr = slotnameIRToNumber(name);
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
  }
}


/**
   Save one IR command to the EEPROM. The slot is determined by the slot number.
   There is an array of timings between the edges, the length
   is provided by cntEdges.
   The name is also provided as parameter
 * */
void saveIRToEEPROMSlotNumber(uint8_t nr, char * name, uint16_t *timings, uint8_t cntEdges)
{
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
}

/**
   Load one IR command from the EEPROM.
   identified by the command name
 * */
uint16_t readIRFromEEPROM(char * name, uint16_t *timings, uint8_t maxEdges)
{
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
}


/**
   Load the EEPROM header, which contains all start addresses of the
   slots and IR commands
 * */
void bootstrapSlotAddresses()
{
  uint8_t init_needed = 0;
  uint16_t versionID = 0;

  // check if we need to initialize EEPROM (magic byte not found)
  if (readEEPROM(EEPROM_MAX_ADDRESS) != EEPROM_MAGIC_NUMBER)
    init_needed=1;

  for (uint8_t i = 0; i < strlen(VERSION_STRING); i++)
    versionID += VERSION_STRING[i];     // create version ID from version string

  /*
  // check if firmware version changed:
    if (versionID != header.versionID)
      init_needed=1;
  */
  
  header.versionID = versionID;         // store version ID

  if (init_needed) {
    makeTone(TONE_CHANGESLOT, 4);
    Serial.println("Initializing EEPROM!");
    memset(&header, 0, sizeof(header));     // delete all slots, initialize with 0
    saveToEEPROMSlotNumber(0, "mouse");   // save default slotSettings to first slot
    writeEEPROM(EEPROM_MAX_ADDRESS,EEPROM_MAGIC_NUMBER);  // store magic number!
  }
  else {
    // load header from EEPROM storage!
    readEEPROMBin((uint8_t *)&header,0,sizeof(storageHeader));
    if (!header.startSlotAddress[0])  // in case no slot is available 
      saveToEEPROMSlotNumber(0, "mouse");   // save default slotSettings to first slot
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.println("EEPROM slot address struct:");
    for (uint8_t i = 0; (i < MAX_SLOTS_IN_EERPOM) && header.startSlotAddress[i]; i++) {
      Serial.print(i); Serial.print(": ");
      Serial.println(header.startSlotAddress[i]);
    }
  #endif
  #ifdef DEBUG_OUTPUT_FULL
    Serial.println("EEPROM IR address struct:");
    for (uint8_t i = 0; (i < MAX_IRCOMMANDS_IN_EERPOM) && header.startIRAddress[i]; i++) {
      Serial.print(i); Serial.print(": ");
      Serial.println(header.startIRAddress[i]);
    }
  #endif
}


/**
   This function deletes a slot from EEPROM.
   if an empty string is given as parameter, all slots are deleted!
   returns 1 if successful, 0 otherwise
 * */
uint8_t deleteSlot(char * name)
{
  int size=0;  
  if (!strlen(name)) {
    #ifdef DEBUG_OUTPUT_BASIC
        Serial.println("Deleting all slots");
    #endif
    for (uint8_t i = 0; i < MAX_SLOTS_IN_EERPOM; i++)  {
      header.startSlotAddress[i] = 0;
      header.endSlotAddress[i] = 0;
    }
    storeHeader();
    return (1);
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Deleting slot ");
    Serial.println(name);      
  #endif
  
  int8_t nr = slotnameToNumber(name);
  if (nr < 0) return (0);  // slot name does not exist
  

  int lastSlot= getLastSlotIndex();
  if (nr != lastSlot) {
    // we must move slot data!
    size = header.endSlotAddress[nr] - header.startSlotAddress[nr];
    uint16_t sourceAddress=header.startSlotAddress[nr+1];
    uint16_t count=getFreeSlotAddress()-sourceAddress;
    
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Slotsize:"); Serial.println(size);
      Serial.print("Move slot "); Serial.print(nr+1); 
      Serial.print(" starting ");  Serial.print(sourceAddress); 
      Serial.print(" to "); Serial.print(sourceAddress-size); 
      Serial.print(", count= "); Serial.print(count); 
      Serial.println(" bytes");
    #endif

    // move all following slots
    moveEEPROM (header.startSlotAddress[nr], sourceAddress, count);

    // update header addresses
    for (int i=nr; i<MAX_SLOTS_IN_EERPOM-1;i++) {
      if (header.startSlotAddress[i]) {
        header.startSlotAddress[i]=header.startSlotAddress[i+1]-size;
        header.endSlotAddress[i]=header.endSlotAddress[i+1]-size;
      }
    }
  }
  header.startSlotAddress[lastSlot]=0;
  header.endSlotAddress[lastSlot]=0;
  storeHeader();
  return(1);
}


/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listSlots()
{
  uint16_t address; //current EEPROM address
  char tmp[MAX_NAME_LEN];

  //iterate all possible slots
  for (uint8_t i = 0; (i < MAX_SLOTS_IN_EERPOM) && header.startSlotAddress[i]; i++)  {
    // load the base address for the current slot
    address = header.startSlotAddress[i];
    // print out the slot name (assumed to be located at the beginning of general slotSettings)
    Serial.print("Slot"); Serial.print(i); Serial.print(":");
    readEEPROMBin(tmp, address, MAX_NAME_LEN);
    Serial.println(tmp);    
  }
}


/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listIRCommands()
{
  uint16_t address; //current EEPROM address
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
  }
}

/**
   get the index of the last slot which holds data  
 * */
int8_t getLastSlotIndex(void) {
  int8_t actSlot=0;
  while ((actSlot<MAX_SLOTS_IN_EERPOM) && (header.startSlotAddress[actSlot])) actSlot++;
  return(actSlot-1);
}

/**
   get the first free address for slot data  
 * */
uint16_t getFreeSlotAddress(void) {
  return(header.endSlotAddress[getLastSlotIndex()]);
}



/**
   print all slot slotSettings and button mode to serial 
 * */
void printAllSlots(void) {
  uint8_t actSlot=0;
  reportSlotParameters = REPORT_ALL_SLOTS;
  while (header.startSlotAddress[actSlot]) {
    readFromEEPROMSlotNumber(actSlot, false);
    actSlot++;
  }
  reportSlotParameters = REPORT_NONE;
  if (header.startSlotAddress[0])
    readFromEEPROMSlotNumber(0, true);
  else Serial.println("END"); 
}
