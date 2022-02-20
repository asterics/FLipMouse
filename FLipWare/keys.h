/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: keys.h - implementation of the keyboard handling

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _KEYS_H_
#define _KEYS_H_


void pressKeys(char* text); // presses individual keys
void holdKeys(char* text);  // holds individual keys
void toggleKeys(char* text);  // toggles individual keys
void releaseKeys(char* text);  // releases individual keys
void release_all_keys();       // releases all previously pressed keys
void release_all();            // releases all previously pressed keys and stops mouse actions


#endif
