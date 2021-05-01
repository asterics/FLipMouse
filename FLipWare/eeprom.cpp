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
   -) implement the defragmentation for slots and IR commands
   -) implement a "memory full" command
   -) implement overwriting of existing slots/ir commands, if the name already exists
 * */

#include "eeprom.h"

/** Device address of the EEPROM **/
#define deviceaddress 0x50
#define EEPROM_PAGESIZE 64
#define EEPROM_PAGEWRITE
#define I2C_MAX_TRANSFER 30

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

// EEPROM page write - write multiple bytes to I2C EEPROM
// thanks to CuckTodd:
// https://forum.arduino.cc/t/faster-way-to-write-to-external-eeprom/429356/5

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
    if (!eeprom_waitReady()) return;
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

void moveSlotsInEEPROM(int nr, int bytesToMove) {
  uint8_t pagebuf[I2C_MAX_TRANSFER], b = 0;
  uint16_t targetAdr;

  if (bytesToMove > 0) {
    #ifdef DEBUG_OUTPUT_BASIC    
      Serial.print("old slot size is bigger: decrease addresses of higher slots by ");
      Serial.print(bytesToMove); Serial.println(" bytes ");
    #endif
    for (int i = nr + 1; i < EEPROM_COUNT_SLOTS; i++) {
      if (header.startSlotAddress[i] != 0) {
        targetAdr = header.startSlotAddress[i] - bytesToMove;
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
  int8_t nr = slotnameToNumber(slotname);
  uint16_t addr;
  //a new slot, search for the first adress which is "0"
  if (nr == -1)
  {
    for (uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
    {
      if (header.startSlotAddress[i] == 0)
      {
        #ifdef DEBUG_OUTPUT_BASIC
          Serial.print("New slot @");
          Serial.println(i);
        #endif
        return saveToEEPROMSlotNumber(i, slotname);
      } else {
        addr = header.startSlotAddress[i];
        if (readEEPROM(addr++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF))
        {
          if (readEEPROM(addr++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8)
          {
            #ifdef DEBUG_OUTPUT_BASIC
              Serial.print("Overwrite slot @");
              Serial.println(i);
            #endif
            return saveToEEPROMSlotNumber(i, slotname);
          }
        }
      }
    }
  }
  else {
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Would like to overwrite Slot ");
      Serial.print(slotname);
      Serial.print(" at position ");
      Serial.println(nr);
    #endif
    return saveToEEPROMSlotNumber(nr, slotname);
  }
}

/**
   Store current slot data to the EEPROM.
   The slot is identified by the slot number. If the nr parameter is -1,
   a new slot will be created (at the first possible position)
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
  while (*(slotname + addr) != '\0') addr++;
  size += addr;
  size++; //\0

  addr = 0;
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    size += sizeof(slotButtonSettings);
    size++; //\0
    addr = 0;
    while (*(keystringButtons[i] + addr) != '\0') addr++;
    size += addr;
    size++;
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Slot size:");
    Serial.println(size);
  #endif

  //if slot is not empty: overwrite (eventually move other slots)
  if (header.startSlotAddress[nr] != 0)
  {
    int old_size = header.endSlotAddress[nr] - header.startSlotAddress[nr];
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print(", old size:");
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

  //write both bytes of the magicnumber, indicating a valid slot
  writeEEPROM(addr++, (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF));
  writeEEPROM(addr++, (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8);

  //save the general settings to the global settings struct
  p = (uint8_t *)&settings;
  #ifndef EEPROM_PAGEWRITE
    for (uint16_t t = 0; t < sizeof(slotGeneralSettings); t++)
      writeEEPROM(addr++, *(p + t));
  #else
    writeEEPROMBin(addr, p, sizeof(slotGeneralSettings));
    addr += sizeof(slotGeneralSettings);
  #endif

  //write '\0' seperator
  writeEEPROM(addr++, '\0');

  //write the slotname
  addr2 = 0;
  #ifndef EEPROM_PAGEWRITE
    do {
      writeEEPROM(addr++, *(slotname + addr2));
      addr2++;
    } while (*(slotname + addr2 - 1) != '\0');
  #else
    writeEEPROMBin(addr, slotname, strlen(slotname));
    addr += strlen(slotname);
    writeEEPROM(addr++, '\0');
  #endif

  //load all button settings
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    //load struct slotButtonSettings
    p = (uint8_t *)&buttons[i];
    #ifndef EEPROM_PAGEWRITE
      for (uint16_t t = 0; t < sizeof(slotButtonSettings); t++)
        writeEEPROM(addr++, *(p + t));
    #else
      writeEEPROMBin(addr, p, sizeof(slotButtonSettings));
      addr += sizeof(slotButtonSettings);
    #endif

    //write '\0' seperator
    writeEEPROM(addr++, 0);

    //add additional payload
    p = (uint8_t *)keystringButtons[i];
    #ifndef EEPROM_PAGEWRITE
      for (uint16_t t = 0; t < MAX_KEYSTRING_LEN; t++) {
        writeEEPROM(addr++, *(p + t));
        if (*(p + t) == 0) break;
      }
    #else
      writeEEPROMBin(addr, p, strlen(p));
      addr += strlen(p);
      writeEEPROM(addr++, '\0');
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

  //store the storageheader permanently to the EEPROM
  #ifndef EEPROM_PAGEWRITE
    for (uint16_t i = 0; i < sizeof(storageHeader); i++) {
      writeEEPROM(i, *((uint8_t*)&header + i));
    }
  #else
    writeEEPROMBin(0, (uint8_t*)&header, sizeof(storageHeader));
  #endif
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
  for (uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
  {
    //load the base address for the current slot
    address = header.startSlotAddress[i];

    //a zero address is not valid, no slot there...
    if (address == 0) continue;

    //check both bytes of the magicnumber, indicating a valid slot
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) continue;
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8) continue;

    //skip the general settings, we only need the slot names
    address += sizeof(slotGeneralSettings);

    //skip '\0'
    address++;

    //reset the match flag
    matches = 1;

    //compare the slotname
    for (unsigned int t = 0; t < MAX_NAME_LEN; t++)
    {
      //compare byte for byte
      if (*(slotname + t) != readEEPROM(address + t))
      {
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
    if (matches)
    {
      return i;

    }
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
  for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
  {
    //load the base address for the current slot
    address = header.startIRAddress[i];

    //a zero address is not valid, no slot there...
    if (address == 0) continue;

    //check both bytes of the magicnumber, indicating a valid slot
    if (readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF)) continue;
    if (readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00) >> 8) continue;

    //reset the match flag
    matches = 1;

    //compare the slotname
    for (unsigned int t = 0; t < MAX_NAME_LEN; t++)
    {
      //compare byte for byte
      if (*(slotname + t) != readEEPROM(address - t))
      {
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
    if (matches)
    {
      #ifdef DEBUG_OUTPUT_BASIC
        Serial.print("Found IR command ");
        Serial.print(slotname);
        Serial.print("@");
        Serial.println(i);
      #endif
      return i;

    }
  }
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("No slot found for ");
    Serial.println(slotname);
  #endif
  return -1;
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
   If the search flag is set, this function loads the next possible slot, if
   the current one is not valid
 * */
void readFromEEPROMSlotNumber(uint8_t nr, bool search)
{
  readFromEEPROMSlotNumber(nr, search, true);
}

/**
   Read one slot data from the EEPROM to the global variables
   The slot is identified by the slot number.
   If the search flag is set, this function loads the next possible slot, if
   the current one is not valid
   if the playTone flag is set, a tone according to the current slot number will be played
 * */
void readFromEEPROMSlotNumber(uint8_t nr, bool search, bool playTone)
{
  uint8_t* p = (uint8_t *)&settings;
  uint16_t address;
  uint8_t x;

  //fence the slot number, avoiding out of array index problems
  if (nr >= EEPROM_COUNT_SLOTS) nr = 0;

  if (search)
  {
    //check for a valid slot. If the current nr contains no slot, try
    //to load next one, until the end is reached.
    for (x = nr; x < EEPROM_COUNT_SLOTS; x++)
    {
      address = header.startSlotAddress[x];

      #ifdef DEBUG_OUTPUT_BASIC
        Serial.print("Load slot by nr");
        Serial.println(x);
      #endif

      //check both bytes of the magicnumber, indicating a valid slot
      if (readEEPROM(address++) == (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF))
      {
        if (readEEPROM(address++) == (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8)
        {
          //load the address again, recheck after this loop
          address = header.startSlotAddress[x];
          //set the current slot nr
          actSlot = x;
          break;
        }
      }

    }

    //try one more time if this is a valid slot.
    //If no, we should point at the end of all slots and we try it one
    //more time with slot 0
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF) ||
        readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8)
    {
      address = header.startSlotAddress[0];
      if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) return;
      if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8) return;
      actSlot = 0;
    }
  } else {
    address = header.startSlotAddress[nr];
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) return;
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8) return;
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Magic bytes OK, loading data @");
    Serial.println(address);
  #endif

  //save the general settings to the global settings struct
  for (uint16_t t = 0; t < sizeof(slotGeneralSettings); t++) *p++ = readEEPROM(address++);

  //skip '\0' seperator
  address++;

  //read the slotname
  p = (uint8_t *)&slotName;
  do {
    *p++ = readEEPROM(address++);
  } while (*(p - 1) != '\0');

  #ifdef DEBUG_OUTPUT_FULL
    Serial.print("found slotname "); Serial.println(slotName);
  #endif

  //load all button settings
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    //load struct slotButtonSettings
    p = (uint8_t *)&buttons[i];
    for (uint16_t t = 0; t < sizeof(slotButtonSettings); t++) *(p + t) = readEEPROM(address++);

    //skip '\0' seperator
    if (readEEPROM(address++) != '\0') {
      Serial.print("Error: seperator problem");
      Serial.println(readEEPROM(address - 1));
    }

    //check for additional payload
    if (readEEPROM(address) != '\0') {
      //load additional payload
      p = (uint8_t *)workingmem;
      for (uint16_t t = 0; t < MAX_KEYSTRING_LEN; t++)
      {
        *(p + t) = readEEPROM(address++);
        if (*(p + t) == 0) break;
      }
      storeKeystringButton(i, (char *) workingmem);
    } else {
      //jump to next slot
      address++;
    }
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
   This function deletes all slots by deleting the magic number (reset to 0xFF)
 * */
void deleteSlots()
{
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.println("Deleting all slots");
  #endif
  for (uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
  {
    writeEEPROM(header.startSlotAddress[i], 0x00);
    writeEEPROM(header.startSlotAddress[i] + 1, 0x00);
    header.startSlotAddress[i] = 0;
  }
  //store the storageheader permanently to the EEPROM
  for (uint16_t i = 0; i < sizeof(storageHeader); i++)
    writeEEPROM(i, *((uint8_t*)&header + i));
}

/**
   This function deletes one IR command by deleting the magic number (reset to 0xFF)
   If the "name" parameter is set to \0, all IR commands will be deleted.
   ATTENTION: if you call this command with \0 as parameter, all slots
   will be deleted, BUT: the changes of the storage header is not
   permanently stored to the EEPROM (save write access), so use it with
   another command that writes it (e.g. deleteSlots())!
 * */
void deleteIRCommand(char * name)
{
  uint8_t nr;
  if (name[0] != 0)
  {
    nr = slotnameIRToNumber(name);
    #ifdef DEBUG_OUTPUT_BASIC
      Serial.print("Deleting slot ");
      Serial.print(name);
      Serial.print("@");
      Serial.println(nr);
    #endif
    if (nr >= 0)
    {
      if (header.startIRAddress[nr] > sizeof(storageHeader))
      {
        writeEEPROM(header.startIRAddress[nr], 0xFF);
        writeEEPROM(header.startIRAddress[nr] - 1, 0xFF);
      }
    }
  } else {
    Serial.println("Deleting all IR slots");
    for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
    {
      if (header.startIRAddress[i] > sizeof(storageHeader))
      {
        writeEEPROM(header.startIRAddress[i], 0xFF);
        writeEEPROM(header.startIRAddress[i] - 1, 0xFF);
      }
      header.startIRAddress[i] = 0;
    }
  }
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
  uint16_t addr;
  //a new slot, search for the first adress which is "0"
  if (nr == -1)
  {
    for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
    {
      if (header.startIRAddress[i] == 0)
      {
        #ifdef DEBUG_OUTPUT_BASIC
          Serial.print("New IR command @");
          Serial.println(i);
        #endif
        return saveIRToEEPROMSlotNumber(i, name, timings, cntEdges);
      } else {
        addr = header.startIRAddress[i];
        if (readEEPROM(addr--) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF))
        {
          if (readEEPROM(addr--) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00) >> 8)
          {
            #ifdef DEBUG_OUTPUT_BASIC
              Serial.print("New IR command @");
              Serial.println(i);
            #endif
            return saveIRToEEPROMSlotNumber(i, name, timings, cntEdges);
          }
        }
      }
    }
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
  uint16_t size = 2; //magic number
  uint16_t addr = 0; //address pointer 1
  uint16_t addr2 = 0; //address pointer 2

  //determine the size of this slot, maybe we need to defrag the EEPROM
  while (*(name + addr) != '\0') addr++;
  addr++; //'\0'
  size++; //count edges field
  size += cntEdges * 2;


  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("IR slot size:");
    Serial.println(size);
  #endif

  //if this is the last slot, don't move other slots
  //TBD: defragmentation

  /** save this slot **/
  //first slot, start right the top address (maximum EEPROM address)
  if (nr == 0) header.startIRAddress[nr] = EEPROM_MAX_ADDRESS;

  //load the start address
  addr = header.startIRAddress[nr];

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Start new slot addr@");
    Serial.println(addr);
  #endif

  //write both bytes of the magicnumber, indicating a valid slot
  writeEEPROM(addr--, (EEPROM_MAGIC_NUMBER_IR & 0x00FF));
  writeEEPROM(addr--, (EEPROM_MAGIC_NUMBER_IR & 0xFF00) >> 8);

  //write the slotname
  addr2 = 0;
  do {
    writeEEPROM(addr--, *(name + addr2));
    addr2++;
  } while (*(name + addr2 - 1) != '\0');

  //write count of edges
  writeEEPROM(addr--, cntEdges);

  //write all edges
  for (uint8_t i = 0; i < cntEdges; i++)
  {
    writeEEPROM(addr--, (uint8_t) ((*(timings + i)) & 0xff));
    writeEEPROM(addr--, (uint8_t) ((*(timings + i)) >> 8));
  }

  /** update the header table */
  //is this the last slot? If yes, just store the end of settings memory
  if (nr == (EEPROM_COUNT_IRCOMMAND - 1))
  {
    //TODO...
  } else {
    //if this is not the last slot, resave all following start adresses
    header.startIRAddress[nr + 1] = addr;
    for (uint8_t i = (nr + 1); i < EEPROM_COUNT_SLOTS; i++)
    {
      //TODO!!!!
      //header.startSlotAddress[i] = ;
    }
    //header.endSlotAddress = ;
  }


  //store the storageheader permanently to the EEPROM
  for (uint16_t i = 0; i < sizeof(storageHeader); i++)
  {
    writeEEPROM(i, *((uint8_t*)&header + i));
  }
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
  for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++)
  {
    //load the base address for the current slot
    address = header.startIRAddress[i];
    //check both bytes of the magicnumber, indicating a valid slot
    if (readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF)) continue;
    if (readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00) >> 8) continue;

    //print out the slot name
    Serial.print("IRCommand"); Serial.print(i); Serial.print(":");
    while ((b = readEEPROM(address--)) && b != 0) Serial.write(b); // print slot name
    Serial.println();
  }
}

/**
   Replay one IR command from the EEPROM.
   The slot is identified by the slot number
 * */
uint16_t readIRFromEEPROMSlotNumber(uint8_t slotNr, uint16_t *timings, uint8_t maxEdges)
{
  uint8_t* p;
  uint8_t slotEdges = 0;
  uint16_t address = header.startIRAddress[slotNr];

  if (slotNr > EEPROM_COUNT_IRCOMMAND) return 0;
  #ifdef DEBUG_OUTPUT_BASIC
    Serial.println("Load IR command by nr");
  #endif

  //check both bytes of the magicnumber, indicating a valid slot
  if (readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0x00FF)) return 0;
  if (readEEPROM(address--) != (EEPROM_MAGIC_NUMBER_IR & 0xFF00) >> 8) return 0;

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.print("Magic bytes OK, loading IR data @");
    Serial.println(address);
  #endif

  while (readEEPROM(address--)) ;   // skip IR name
  slotEdges = readEEPROM(address--);

  //load all IR command edges
  for (uint8_t i = 0; i < maxEdges; i++)
  {
    if (i == slotEdges) break;
    *(timings + i) = (uint16_t) readEEPROM(address--);
    *(timings + i) +=  ((uint16_t)readEEPROM(address--)) << 8;
  }

  //return the count of available edges (used by the IR parser)
  return slotEdges;
}

/**
   Replay one IR command from the EEPROM.
   The slot is identified by the slot name
 * */
uint16_t readIRFromEEPROM(char * name, uint16_t *timings, uint8_t maxEdges)
{
  int8_t nr = slotnameIRToNumber(name);

  if (nr >= 0)
  {
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

  for (uint16_t i = 0; i < sizeof(storageHeader); i++)
  {
    *((uint8_t *)&header + i) = readEEPROM(i);
  }

  for (uint8_t i = 0; i < strlen(VERSION_STRING); i++)
    versionID += VERSION_STRING[i];     // create version ID from version string

  // check if we need to initialize EEPROM (at first start or when firmware version changed )
  if ((readEEPROM(header.startSlotAddress[0]) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) ||
      (readEEPROM(header.startSlotAddress[0] + 1) != ((EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8)) ||
      (versionID != header.versionID))
  {
    makeTone(TONE_CHANGESLOT, 4);
    Serial.println("Initializing EEPROM!");
    deleteSlots();                        // delete all slots
    deleteIRCommand(0);                   // initialize IR command address table
    header.versionID = versionID;         // store version ID
    saveToEEPROMSlotNumber(0, "mouse");   // save default settings to first slot
  }

  #ifdef DEBUG_OUTPUT_BASIC
    Serial.println("EEPROM slot address struct (bootstrap):");
    for (uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(header.startSlotAddress[i]);
      Serial.print("  MB:");
      Serial.print(readEEPROM(header.startSlotAddress[i]));
      Serial.print("/");
      Serial.println(readEEPROM(header.startSlotAddress[i] + 1));
    }
    for (uint8_t i = 0; i < EEPROM_COUNT_IRCOMMAND; i++) {
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
  for (uint8_t i = 0; i < EEPROM_COUNT_SLOTS; i++)
  {
    //load the base address for the current slot
    address = header.startSlotAddress[i];
    //check both bytes of the magicnumber, indicating a valid slot
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0x00FF)) continue;
    if (readEEPROM(address++) != (EEPROM_MAGIC_NUMBER_SLOT & 0xFF00) >> 8) continue;

    //skip the general settings, we only need the slot names
    address += sizeof(slotGeneralSettings);

    //skip '\0' seperator
    address++;

    //print out the slot name
    Serial.print("Slot"); Serial.print(i); Serial.print(":");
    while ((b = readEEPROM(address++)) && b != 0) Serial.write(b); // print slot name
    Serial.println();
  }
}
