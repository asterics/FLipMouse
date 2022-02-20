/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: parser.h - implementation of the parser for serial commands and parameters

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _PARSER_H_
#define _PARSER_H_

#define PARTYPE_NONE  0
#define PARTYPE_UINT  1
#define PARTYPE_INT   2
#define PARTYPE_STRING  3

#define REPORT_NONE  0
#define REPORT_ALL_SLOTS 1

extern uint8_t reportSlotParameters;
extern uint8_t reportRawValues;

void parseByte (int newByte);
void parseCommand (char * cmdstr);

void printCurrentSlot();
void reportValues();

#endif
