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

/**
   key action constant definitions
*/
#define KEY_PRESS    0
#define KEY_HOLD     1
#define KEY_RELEASE  2
#define KEY_TOGGLE   3

// size of the buffer for currently pressed keys
#define KEYPRESS_BUFFERSIZE 8

/**
   @name printKeyboardLayout
   @brief Prints out the currently used keyboard layout (e.g. "en_US\n")
   @return none
*/
void printKeyboardLayout();

/**
   @name setKeyboardLayout
   @brief Updates the currently used keyboard layout.
   @param name Name of the new keyboard layout (e.g. "en_US" or "de_DE")
   @return 0 on success, 1 if layout is not found.
   @note Currently supported: de_DE, en_US, es_ES, fr_FR, it_IT, sv_SE, da_DK
*/
int8_t setKeyboardLayout(char *name);

/**
   @name pressKeys
   @brief presses (and releases) individual keys
   @param text string containing one or multiple keycode identifiers of the keys which should be pressed
   @return none
*/
void pressKeys(char* text);

/**
   @name holdKeys
   @brief presses (and holds) individual keys
   @param text string containing one or multiple keycode identifiers of the keys which should be pressed
   @return none
*/
void holdKeys(char* text);

/**
   @name toggleKeys
   @brief toggles the press/release state individual keys
   @param text string containing one or multiple keycode identifiers of the keys which should be toggled
   @return none
*/
void toggleKeys(char* text);

/**
   @name releaseKeys
   @brief releases individual keys
   @param text string containing one or multiple keycode identifiers of the keys which should be released
   @return none
*/
void releaseKeys(char* text);

/**
   @name release_all_keys
   @brief releases all currently pressed keys
   @return none
*/
void release_all_keys();

/**
   @name release_all
   @brief releases all previously pressed keys and stops ongoing mouse actions
   @return none
*/
void release_all();


#endif
