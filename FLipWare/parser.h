/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: parser.h - implementation of the parser for serial commands and parameters

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _PARSER_H_
#define _PARSER_H_

/**
   constant definitions of identifiers for supported parameter types
*/
#define PARTYPE_NONE  0
#define PARTYPE_UINT  1
#define PARTYPE_INT   2
#define PARTYPE_STRING  3

/**
   @name parseByte
   @brief parses AT command input from serial interface
   @param newByte incoming serial character, to be fed into parsing state machine
   @return none
*/
void parseByte (int newByte);


/**
   @name parseCommand
   @brief parses a detected AT command
   @param cmdstr pointer to a string which contains the AT command identifier and parameter
   @return none
*/
void parseCommand (char * cmdstr);

#endif
