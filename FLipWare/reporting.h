/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: reporting.h - implementation of serial reports

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _REPORTING_H_
#define _REPORTING_H_

/**
   constant definitions for reporting modes
*/
#define REPORT_NONE  0
#define REPORT_ALL_SLOTS 1

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern uint8_t reportRawValues;

/** 
 * @brief Print current to given stream
 * @param S Stream to send the AT commands to; in our case Serial or a File
 */
void printCurrentSlot(Stream *S);

/**
   @name reportValues
   @brief prints the current live movement data and button values to the serial interface
   @return none
*/
void reportValues();

#endif
