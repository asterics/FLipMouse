/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: infrared.h - implementation of the infrared record/replay functions 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _INFRARED_H_
#define _INFRARED_H_



/**
   physical pin definitions
*/
#define IR_LED_PIN     D16    //  IR-Led output pin
#define IR_SENSOR_PIN  D14    //  input pin of the TSOP IR receiver


// name of the idle code command (played after other ir commands if it exists)
#define IDLESEQUENCE_NAME "idle"
#define IDLESEQUENCE_REPEAT 1

#define IR_EDGE_REC_MAX 512


/**
   @name initIR
   @brief initializes IR gpio pins and carrier frequency
   @return none
*/
void initIR();

/**
   @name record_IR_command
   @brief record an IR command with given name and store edge timing to EEPROM
          prints error messages if timeout or data structure full
   @param name name of the IR command to be recorded
   @return none
*/
void record_IR_command(char * name);

/**
   @name play_IR_command
   @brief play an IR command with given name from EEPROM 
          code will be played once
   @param name name of the IR command to be played
   @return none
*/
void play_IR_command(char * name);

/**
   @name hold_IR_command
   @brief play an IR command with given name from EEPROM 
          code will be played repeatedly (until repeatCounter==0)
   @param name name of the IR command to be played
   @return none
*/
void hold_IR_command(char * name);

/**
   @name stop_IR_command
   @brief stop currently playing IR command by setting repeatCounter=0
   @return none
*/
void stop_IR_command();

/**
   @name list_IR_command
   @brief prints all IR command names stored in EEPROM to serial interface
   @return none
*/
void list_IR_commands();

/**
   @name delete_IR_command
   @brief deletes IR command of given names from EEPROM
          If you want to clear all IR commands, use '\0' as name
   @param name name of the IR command to be deleted
   @return 1 if successful, 0 otherwise
*/
uint8_t delete_IR_command(char * name);

/**
   @name set_IR_timeout
   @brief sets the timeout for IR recording 
   @param ms timeout value in milliseconds
   @return none
*/
void set_IR_timeout(uint16_t ms);

/**
   @name wipe_IR_commands
   @brief deletes all IR commands from EEPROM
   @return none
*/
void wipe_IR_commands();

#endif
