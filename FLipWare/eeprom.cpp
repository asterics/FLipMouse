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
   -) implement delete for individual slots
   -) implement a "memory full" command
 * */

#include "eeprom.h"

/** Device address of the EEPROM **/
#define deviceaddress 0x50
#define EEPROM_PAGESIZE 64
#define EEPROM_PAGEWRITE
#define I2C_MAX_TRANSFER 64

/**
   Local copy of all slot and ir command start adresses
 * */
struct storageHeader header;
uint8_t currentSlot;

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
  // delay(5);
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
    i = I2C_MAX_TRANSFER; // maximum data bytes that Wire.h can send in one transaction
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
   store the storageheader permanently to the EEPROM
 * */
void storeHeader(void) {
  #ifndef EEPROM_PAGEWRITE
    for (uint16_t i = 0; i < sizeof(storageHeader); i++)
      writeEEPROM(i, *((uint8_t*)&header + i));
  #else
    writeEEPROMBin(0, (uint8_t*)&header, sizeof(storageHeader));
  #endif
}

/**
   Read one byte from the EEPROM
   TODO: add another read function for more than one byte
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
   move multiple slots in EEPROM 
   nr: the first slot which should be moved 
   bytesToMove: postive (increase slot size) or negative (shrink slot size)
 * */
void moveSlotsInEEPROM(int nr, int bytesToMove) {
  uint8_t pagebuf[I2C_MAX_TRANSFER];
  uint16_t targetAdr, b;

  if (bytesToMove > 0) {
    #ifdef DEBUG_OUTPUT_BASIC    
      Serial.print("old slot size is bigger: decrease addresses of higher slots by ");
      Serial.print(bytesToMove); Serial.println(" bytes ");
    #endif
    for (int i = nr + 1; i < EEPROM_COUNT_SLOTS; i++) {
      if (header.startSlotAddress[i] != 0) {
        targetAdr = header.startSlotAddress[i] - bytesToMove;
        b=0;
        #ifdef DEBUG_OUTPUT_BASIC
          Serial.print("move slot "); Serial.println(i);
        #endif
        for (int t = header.startSlotAddress[i]; t < header.endSlotAddress[i]; t++) {
          pagebuf[b++] = readEEPROM(t);
          if (b == I2C_MAX_TRANSFER) {
            writeEEPROMBin(targetAdr, pagebuf, I2C_MAX_TRANSFER);
            targetAdr += I2C_MAX_TRANSFER;
            b = 0;
          }
        }
        if (b) writeEEPROMBin(targetAdr, pagebuf, b);
        header.startSlotAddress[i] -= bytesToMove;
        header.endSlotAddress[i] -= bytesToMove;
      }
    }
  }
  else {
    bytesToMove = -bytesToMove;
    #ifdef DEBUG_OUTPUT_BASIC    
      Serial.print("new slot size is bigger: increase addresses of higher slots by ");
      Serial.print(bytesToMove); Serial.println(" bytes ");
    #endif
    for (int i = EEPROM_COUNT_SLOTS - 1; i > nr; i--) {
      if (header.startSlotAddress[i] != 0) {

        // for (int t = header.endSlotAddress[i] - 1; t >= header.startSlotAddress[i]; t--)
        //   writeEEPROM(t + bytesToMove, readEEPROM(t));

        targetAdr = header.endSlotAddress[i] + bytesToMove;
        b = I2C_MAX_TRANSFER;
        #ifdef DEBUG_OUTPUT_BASIC
          Serial.print("move slot "); Serial.println(i);
        #endif
        for (int t = header.endSlotAddress[i] - 1; t >= header.startSlotAddress[i]; t--) {
          targetAdr--; b--;
          pagebuf[b] = readEEPROM(t);
          if (b == 0) {
            writeEEPROMBin(targetAdr, pagebuf, I2C_MAX_TRANSFER);
            b = I2C_MAX_TRANSFER;
          }
        }
        if (b < I2C_MAX_TRANSFER) writeEEPROMBin(targetAdr, pagebuf + b, I2C_MAX_TRANSFER - b);

        header.startSlotAddress[i] += bytesToMove;
        header.endSlotAddress[i] += bytesToMove;
      }
    }
  }
}



/**
   Save the current slot by the given slotname.
   If there is no slot existing with this name,
   it will be saved to a new slot.
 * */
void saveToEEPROM(char * slotname)
{
  uint32_t timestamp = millis();
  int8_t nr = slotnameToNumber(slotname);
  uint16_t slotNum=0;

  if (nr == -1) {
    //a new slot, search for the first adress which is "0"
    while ((slotNum < EEPROM_COUNT_SLOTS) && header.startSlotAddress[slotNum])
      slotNum++;
      
    if (slotNum < EEPROM_COUNT_SLOTS) {
      #ifdef DEBUG_OUTPUT_BASIC
        Serial.print("New slot @");
        Serial.println(slotNum);
      #endif
      saveToEEPROMSlotNumber(slotNum, slotname);
    } else Serial.print("EEPROM slots full, data not saved.");
  }
  else {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Overwrite Slot ");
      Serial.print(slotname);
      Serial.print(" at position ");
      Serial.println(nr);
    #endif
    saveToEEPROMSlotNumber(nr, slotname);
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("EEPROM Save took ");
    Serial.print(millis()-timestamp);
    Serial.println(" milliseconds.");
  #endif
}

/**
   Store current slot data to the EEPROM.
   The slot is identified by the slot number. If the nr parameter is -1,
   a new slot will be created (at the first possible position)
 * */
void saveToEEPROMSlotNumber(int8_t nr, char * slotname)
{
  uint16_t size = 0;
  uint16_t addr = 0; //address pointer 1
  uint16_t addr2 = 0; //address pointer 2
  uint8_t* p = (uint8_t *)&settings; //pointer to the global settings

  //determine the size of this slot
  size += sizeof(slotGeneralSettings);
  size += (strlen(slotname)+1);  // count slotname lenght (incl. '\0')
  size += sizeof(slotButtonSettings)*NUMBER_OF_BUTTONS;

  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
    size += (strlen(keystringButtons[i])+1);

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Slotsize:");
    Serial.println(size);
  #endif

  //if slot is not empty: overwrite (eventually move other slots)
  if (header.startSlotAddress[nr] != 0) {
    int old_size = header.endSlotAddress[nr] - header.startSlotAddress[nr];
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("old Slotsize:");
      Serial.println(old_size);
    #endif
    if (old_size != size) {
      int bytesToMove = old_size - size;
      moveSlotsInEEPROM(nr, bytesToMove);
    }
  }

  /** save this slot **/
  //first slot, start right after the storage header
  if (nr == 0)
    addr = sizeof(storageHeader);
  else
    addr = header.endSlotAddress[nr - 1];

  header.startSlotAddress[nr] = addr;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Start new slot @");
    Serial.println(addr);
  #endif

  //save the general settings to the global settings struct
  #ifndef EEPROM_PAGEWRITE
    p = (uint8_t *)&settings;
    for (uint16_t t = 0; t < sizeof(slotGeneralSettings); t++)
      writeEEPROM(addr++, *(p + t));
  #else
    writeEEPROMBin(addr, (uint8_t *)&settings, sizeof(slotGeneralSettings));
    addr += sizeof(slotGeneralSettings);
  #endif

  //write the slotname
  #ifndef EEPROM_PAGEWRITE
    addr2 = 0;
    do {
      writeEEPROM(addr++, *(slotname + addr2));
      addr2++;
    } while (*(slotname + addr2 - 1) != '\0');
  #else
    writeEEPROMBin(addr, slotname, strlen(slotname)+1);
    addr += (strlen(slotname)+1);
  #endif

  // write all button settings
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    //load struct slotButtonSettings
    #ifndef EEPROM_PAGEWRITE
      p = (uint8_t *)&buttons[i];
      for (uint16_t t = 0; t < sizeof(slotButtonSettings); t++)
        writeEEPROM(addr++, *(p + t));
    #else
      writeEEPROMBin(addr, (uint8_t *)&buttons[i], sizeof(slotButtonSettings));
      addr += sizeof(slotButtonSettings); 
    #endif

    //add additional payload
    #ifndef EEPROM_PAGEWRITE
      p = (uint8_t *)keystringButtons[i];
      for (uint16_t t = 0; t < MAX_KEYSTRING_LEN; t++) {
        writeEEPROM(addr++, *(p + t));
        if (*(p + t) == 0) break;
      }
    #else
      writeEEPROMBin(addr, keystringButtons[i], strlen(keystringButtons[i])+1);
      addr += strlen(keystringButtons[i])+1;
    #endif
  }

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
   Matcher function to determine the slot number for a given slot name.
   Currently the GUI is handling all slot assignments via the name.
   For future implementations, it may base on numbers.
   To make the development easy, the parsing of slotnames to slot numbers
   is done in this method (used by read and save functions)
 * */
int8_t slotnameToNumber(char * slotname)
{
  uint16_t address; //current EEPROM address
  uint8_t matches;

  //iterate all possible slots
  for (uint8_t i = 0; (i < EEPROM_COUNT_SLOTS) && header.startSlotAddress[i]; i++) {
    //load the base address for the current slot
    address = header.startSlotAddress[i];
    //skip the general settings, we only need the slot names
    address += sizeof(slotGeneralSettings);
    //reset the match flag
    matches = 1;
    //compare the slotname
    for (unsigned int t = 0; t < MAX_NAME_LEN; t++) {
      //compare byte for byte
      if (*(slotname + t) != readEEPROM(address + t)) {
        //if one byte doesn't match, reset match flag and break the for loop
        matches = 0;
        #ifdef DEBUG_OUTPUT_FULL
          Serial.print("Names don't match @");
          Serial.print(t);
          Serial.print(" ");
          Serial.print(*(slotname + t));
          Serial.print("!=");
          Serial.println(readEEPROM(address + t));
        #endif
        break;
      }
      //if the end of the string is reached, break the for loop
      if (*(slotname + t) == '\0') break;
    }

    //if it matches, return the number
    if (matches) return i;
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("No slot found for ");
    Serial.println(slotname);
  #endif
  return -1;
}

/**
   Matcher function to determine the slot number for a given slot name (IR command)
   Currently the GUI is handling all slot assignments via the name.
   For future implementations, it may base on numbers.
   To make the development easy, the parsing of slotnames to slot numbers
   is done in this method (used by read and save functions)
 * */
int8_t slotnameIRToNumber(char * slotname)
{
  uint16_t address; //current EEPROM address
  uint8_t matches;

  //iterate all possible slots
  for (uint8_t i = 0; (i < EEPROM_COUNT_IRCOMMAND) && header.startIRAddress[i]; i++)  {
    //load the base address for the current slot
    address = header.startIRAddress[i];

    //reset the match flag
    matches = 1;

    //compare the slotname
    for (unsigned int t = 0; t < MAX_NAME_LEN; t++) {
      //compare byte for byte
      if (*(slotname + t) != readEEPROM(address - t)) {
        //if one byte doesn't match, reset match flag and break the for loop
        matches = 0;
        #ifdef DEBUG_OUTPUT_FULL
          Serial.print("Names don't match @");
          Serial.print(t);
          Serial.print(" ");
          Serial.print(*(slotname + t));
          Serial.print("!=");
          Serial.println(readEEPROM(address - t));
        #endif
        break;
      }
      //if the end of the string is reached, break the for loop
      if (*(slotname + t) == '\0') break;
    }

    //if it matches, return the number
    if (matches) return i;
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("No slot found for ");
    Serial.println(slotname);
  #endif
  return -1;
}

/**
   print all slot settings and button mode to serial 
 * */
void printAllSlots(void) {
  uint8_t actSlot=0;
  reportSlotParameters = REPORT_ALL_SLOTS;
  while (header.startSlotAddress[actSlot]) {
    readFromEEPROMSlotNumber(actSlot, false);
    actSlot++;
  }
  reportSlotParameters = REPORT_NONE;
  readFromEEPROMSlotNumber(0, true);
}


/**
   Load one slot by the given slotname
   If there is no slotname give, load next slot...
 * */
void readFromEEPROM(char * slotname)
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
 * */
void readFromEEPROMSlotNumber(uint8_t nr, bool playTone)
{
  uint8_t* p = (uint8_t *)&settings;
  uint16_t address;

  //fence the slot number, avoiding out of array index problems
  if (nr >= EEPROM_COUNT_SLOTS) return;
  if (header.startSlotAddress[nr]==0)  nr=0;  // wrap around at last slot / free slot!

  actSlot=nr;
  address = header.startSlotAddress[nr];
   
  //save the general settings to the global settings struct
  for (uint16_t t = 0; t < sizeof(slotGeneralSettings); t++)
    *p++ = readEEPROM(address++);

  //read the slotname
  p = (uint8_t *)&slotName;
  do {
    *p++ = readEEPROM(address++);
  } while (*(p - 1) != '\0');

  #ifdef DEBUG_OUTPUT_FULL
    Serial.print("found slotname "); Serial.println(slotName);
  #endif

  //load all button settings
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    //load struct slotButtonSettings
    p = (uint8_t *)&buttons[i];
    for (uint16_t t = 0; t < sizeof(slotButtonSettings); t++)
      *(p + t) = readEEPROM(address++);

    //load additional payload
    p = (uint8_t *)workingmem;
    uint8_t val;
    do {
      val=readEEPROM(address++);
      *p++ = val; 
    } while (val);
    storeKeystringButton(i, (char *) workingmem);
  }

  if (reportSlotParameters != REPORT_NONE)
    printCurrentSlot();

  if (playTone) makeTone(TONE_CHANGESLOT, actSlot);
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("actSlot: "); Serial.println(actSlot);
  #endif

  if (reportSlotParameters)
    Serial.println("END");   // important: end marker for slot parameter list (command "load all" - AT LA)
}

/**
   This function deletes all configuration slots in EEPROM
 * */
void deleteSlots()
{
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.println("Deleting all slots");
  #endif

  // TBD: delete individual slot / move other slots
  for (uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)  {
    header.startSlotAddress[i] = 0;
    header.endSlotAddress[i] = 0;
  }
  storeHeader();
}

/**
   This function deletes one IR command
   If the "name" parameter is set to \0, all IR commands will be deleted.
 * */
void deleteIRCommand(char * name)
{
  uint8_t nr;
  if (name[0] != 0) {
    nr = slotnameIRToNumber(name);
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Deleting slot ");
      Serial.print(name);
      Serial.print("@");
      Serial.println(nr);
    #endif
    if (nr >= 0) {
      // TBD: move data of other ir Slots !
      for (uint8_t i = nr; i < EEPROM_COUNT_IRCOMMAND-1; i++) {
        header.startIRAddress[i] = header.startIRAddress[i+1];
        header.endIRAddress[i] = header.endIRAddress[i+1];
      }
    }
  } else {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.println("Deleting all IR slots");
    #endif
    for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++) {
      header.startIRAddress[i] = 0;
      header.endIRAddress[i] = 0;
    }
  }
  storeHeader();
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
    while  ((irSlot < EEPROM_COUNT_IRCOMMAND) && header.startIRAddress[irSlot])
      irSlot++;

    if (irSlot < EEPROM_COUNT_IRCOMMAND) {
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
  uint16_t addr = 0; //address pointer 1
  uint16_t addr2 = 0; //address pointer 2

  //determine the size of this slot
  size = strlen(name)+1;
  size++;  // count edges field
  size += cntEdges * 2;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("IR slot size:");
    Serial.println(size);
  #endif

  //if IR slot is not empty: overwrite (eventually move other IR slots)
  if (header.startIRAddress[nr] != 0)
  {
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

  // TBD: improve efficiency / reverse order / page writes?
  //write the slotname
  addr2 = 0;
  do {
    writeEEPROM(addr--, *(name + addr2));
    addr2++;
  } while (*(name + addr2 - 1) != '\0');

  //write count of edges
  writeEEPROM(addr--, cntEdges);

  //write all edges  
  for (uint8_t i = 0; i < cntEdges; i++)  {
    writeEEPROM(addr--, (uint8_t) ((*(timings + i)) & 0xff));
    writeEEPROM(addr--, (uint8_t) ((*(timings + i)) >> 8));
  }

  /** update the header table */
  header.endIRAddress[nr] = addr;
  storeHeader();
}

/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listIRCommands()
{
  uint16_t address; //current EEPROM address
  uint8_t b;

  //iterate all possible slots
  for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++) {
    //load the base address for the current slot
    address = header.startIRAddress[i];
    //print out the slot name
    Serial.print("IRCommand"); Serial.print(i); Serial.print(":");
    while ((b = readEEPROM(address--)) && b != 0) Serial.write(b); // print slot name
    Serial.println();
  }
}

/**
   Read one IR command from the EEPROM.
   The IR command is identified by irNr,
   The timing values are read from EEPROM and stored into the timings array
 * */
uint16_t readIRFromEEPROMSlotNumber(uint8_t irNr, uint16_t *timings, uint8_t maxEdges)
{
  uint8_t* p;
  uint8_t irEdges = 0;
  uint16_t address = header.startIRAddress[irNr];

  if (irNr > EEPROM_COUNT_IRCOMMAND) return 0;
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Load IR command by nr: ");
    Serial.print(irNr);
    Serial.print(" from address: ");
    Serial.println(address);
  #endif

  while (readEEPROM(address--)) ;   // skip IR name
  
  irEdges = readEEPROM(address--);  // read edges count

  //load all IR command edges
  for (uint8_t i = 0; i < maxEdges; i++)
  {
    if (i == irEdges) break;
    *(timings + i) = (uint16_t) readEEPROM(address--);
    *(timings + i) +=  ((uint16_t)readEEPROM(address--)) << 8;
  }

  //return the count of available edges (used by the IR parser)
  return irEdges;
}

/**
   Replay one IR command from the EEPROM.
   The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name, uint16_t *timings, uint8_t maxEdges)
{
  int8_t nr = slotnameIRToNumber(name);

  if (nr >= 0) {
    #ifdef DEBUG_OUTPUT_FULL
      Serial.print("Load IR command ");
      Serial.print(name);
      Serial.print("@");
      Serial.println(nr);
  #endif

    //call the method which loads the data
    return readIRFromEEPROMSlotNumber(nr, timings, maxEdges);
  } else {
    #ifdef DEBUG_OUTPUT_FULL
      Serial.print("Could not Load IR command ");
      Serial.println(name);
    #endif
    return 0;
  }
}


/**
   Load the EEPROM header, which contains all start addresses of the
   slots and IR commands
 * */
void bootstrapSlotAddresses()
{
  uint16_t versionID = 0;
  uint8_t init_needed = 0;

  for (uint16_t i = 0; i < sizeof(storageHeader); i++)
    *((uint8_t *)&header + i) = readEEPROM(i);
 
  for (uint8_t i = 0; i < strlen(VERSION_STRING); i++)
    versionID += VERSION_STRING[i];     // create version ID from version string

  // check if we need to initialize EEPROM 
  // no magic byte found:
  if (readEEPROM(EEPROM_MAX_ADDRESS) != EEPROM_MAGIC_NUMBER)
    init_needed=1;

  // firmware version changed:
  // if (versionID != header.versionID)
  //   init_needed=1;
  
  if (init_needed) {
    makeTone(TONE_CHANGESLOT, 4);
    Serial.println("Initializing EEPROM!");
    memset(&header, 0, sizeof(header));     // delete all slots, initialize with 0
    header.versionID = versionID;         // store version ID
    saveToEEPROMSlotNumber(0, "mouse");   // save default settings to first slot
    writeEEPROM(EEPROM_MAX_ADDRESS,EEPROM_MAGIC_NUMBER);  // store magic number!
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.println("EEPROM slot address struct:");
    for (uint8_t i = 0; (i < EEPROM_COUNT_SLOTS) && header.startSlotAddress[i]; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(header.startSlotAddress[i]);
    }
  #endif
  #ifdef DEBUG_OUTPUT_FULL
    Serial.println("EEPROM IR address struct:");
    for (uint8_t i = 0; (i < EEPROM_COUNT_IRCOMMAND) && header.startIRAddress[i]; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(header.startIRAddress[i]);
    }
  #endif
}

/**
   Print out all slotnames to the serial interface
   Used by the PC-GUI to load all available slot names
 * */
void listSlots()
{
  uint16_t address; //current EEPROM address
  uint8_t b;

  //iterate all possible slots
  for (uint8_t i = 0; (i < EEPROM_COUNT_SLOTS) && header.startSlotAddress[i]; i++)  {
    //load the base address for the current slot
    address = header.startSlotAddress[i];
    //skip the general settings, we only need the slot names
    address += sizeof(slotGeneralSettings);
    //print out the slot name
    Serial.print("Slot"); Serial.print(i); Serial.print(":");
    while ((b = readEEPROM(address++)) && b != 0) Serial.write(b); // print slot name
    Serial.println();
  }
}
