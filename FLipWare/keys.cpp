/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: keys.cpp - implementation of the keyboard handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "keys.h"


/**
   forward declarations of module-internal functions
*/
int pressed_keys[KEYPRESS_BUFFERSIZE];
uint8_t in_keybuffer(int key);
void remove_from_keybuffer(int key);
void add_to_keybuffer(int key);
void performKeyActions(char* text, uint8_t keyAction);
char kbdLayout[6] = "en_US";
const uint8_t *kbdLayoutArray = KeyboardLayout_en_US;

/**
   @name printKeyboardLayout
   @brief Prints out the currently used keyboard layout (e.g. "en_US\n")
   @return none
*/
void printKeyboardLayout()
{
	Serial.println(kbdLayout);
}

/**
   @name setKeyboardLayout
   @brief Updates the currently used keyboard layout.
   @param name Name of the new keyboard layout (e.g. "en_US" or "de_DE")
   @return 1 on success, 0 if layout is not found.
   @note Currently supported: de_DE, en_US, es_ES, fr_FR, it_IT, sv_SE, da_DK
*/
int8_t setKeyboardLayout(char *name)
{
	const uint8_t *newLayout = 0;
	
	if(strncmp(name, "de_DE",5) == 0) newLayout = KeyboardLayout_de_DE;
	if(strncmp(name, "en_US",5) == 0) newLayout = KeyboardLayout_en_US;
	if(strncmp(name, "es_ES",5) == 0) newLayout = KeyboardLayout_es_ES;
	if(strncmp(name, "fr_FR",5) == 0) newLayout = KeyboardLayout_fr_FR;
	if(strncmp(name, "it_IT",5) == 0) newLayout = KeyboardLayout_it_IT;
	if(strncmp(name, "sv_SE",5) == 0) newLayout = KeyboardLayout_sv_SE;
	if(strncmp(name, "da_DK",5) == 0) newLayout = KeyboardLayout_da_DK;
	
	if(newLayout)
	{
		Keyboard.begin(newLayout);
    kbdLayoutArray = newLayout;
		#ifdef DEBUG_OUTPUT_FULL
			Serial.print("Found new layout pointer for ");
			Serial.print(name);
			Serial.println(", setting in Keyboard.begin");
		#endif
    strncpy(kbdLayout,name,5); //save locally
		return 1;
	} else return 0;
}

/**
   @name getKeyboardLayout
   @brief Used to get the pointer to the current keyboard layout
   @return Pointer to keyboard layout array
*/
const uint8_t *getKeyboardLayout()
{
  return kbdLayoutArray;
}

/**
   @name updateKey
   @brief performs a new press/release action for a given keycode
   @param key the keycode
   @param keyAction action to be performed
   @return none
*/
void updateKey(int key, uint8_t keyAction)
{
  switch (keyAction)  {
    case KEY_PRESS:
    #ifdef DEBUG_OUTPUT_KEYS
      Serial.print("P+");
    #endif
    case KEY_HOLD:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.println("H");
      #endif
      add_to_keybuffer(key);
      keyboardPress(key);       // press/hold keys individually
      break;

    case KEY_RELEASE:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.println("R");
      #endif
      remove_from_keybuffer(key);
      keyboardRelease(key);       // release keys individually
      break;

    case KEY_TOGGLE:
      #ifdef DEBUG_OUTPUT_KEYS
        Serial.print("T-");
      #endif
      if (in_keybuffer(key))  {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.println("R");
        #endif
        remove_from_keybuffer(key);
        keyboardRelease(key);
      } else {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.println("P");
        #endif
        add_to_keybuffer (key);
        keyboardPress(key);
      }
      break;
  }
  //need to delay to avoid missing keyboard actions
  delay(10);
}

void pressKeys (char * text)
{
  performKeyActions(text, KEY_PRESS);
  performKeyActions(text, KEY_RELEASE);
}

void holdKeys (char * text)
{
  performKeyActions(text, KEY_HOLD);
}

void releaseKeys (char * text)
{
  performKeyActions(text, KEY_RELEASE);
}

void toggleKeys (char * text)
{
  performKeyActions(text, KEY_TOGGLE);
}

void release_all_keys()
{
  keyboardReleaseAll();
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++)
    pressed_keys[i] = 0;
}


void release_all()  // releases all previously pressed keys and stop mouse actions
{
  release_all_keys();
  mouseRelease(MOUSE_LEFT);
  mouseRelease(MOUSE_MIDDLE);
  mouseRelease(MOUSE_RIGHT);
  sensorData.autoMoveX = 0;
  sensorData.autoMoveY = 0;
}

/**
   @name add_to_keybuffer
   @brief adds a keycode to buffer of pressed keys
   @param key the keycode
   @return none
*/
void add_to_keybuffer(int key)
{
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == 0)
      pressed_keys[i] = key;
    if (pressed_keys[i] == key) i = KEYPRESS_BUFFERSIZE;
  }
}

/**
   @name remove_from_keybuffer
   @brief removes a keycode from buffer of pressed keys
   @param key the keycode
   @return none
*/
void remove_from_keybuffer(int key)
{
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == key)
    {
      while (i < KEYPRESS_BUFFERSIZE - 1)
      {
        pressed_keys[i] = pressed_keys[i + 1];
        i++;
      }
      pressed_keys[KEYPRESS_BUFFERSIZE - 1] = 0;
      i = KEYPRESS_BUFFERSIZE;
    }
  }
}

/**
   @name in_keybuffer
   @brief returns true if a given keycode is in the buffer of pressed keys
   @param key the keycode
   @return true if key is actually pressed, otherwise false
*/
uint8_t in_keybuffer(int key)
{
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == key)
      return (1);
  }
  return (0);
}

/**
   keymap_struct 
   maps a keycode to a key-identifier string
*/
struct keymap_struct {
  char const *token;
  int key;
};

/**
   keymap1
   keycode/key-identifier mapping for key-identifiers with prefix "KEY_"
*/
const keymap_struct keymap1 [] = {
  {"SHIFT", KEY_LEFT_SHIFT},
  {"CTRL", KEY_LEFT_CTRL},
  {"ALT", KEY_LEFT_ALT},
  {"RIGHT_ALT", KEY_RIGHT_ALT},
  {"GUI", KEY_LEFT_GUI},
  {"RIGHT_GUI", KEY_RIGHT_GUI},
  {"UP", KEY_UP_ARROW},
  {"DOWN", KEY_DOWN_ARROW},
  {"LEFT", KEY_LEFT_ARROW},
  {"RIGHT", KEY_RIGHT_ARROW},
  {"ENTER", KEY_RETURN},
  {"SPACE", ' '},
  {"ESC", KEY_ESC},
  {"BACKSPACE", KEY_BACKSPACE},
  {"TAB", KEY_TAB},
  {"CAPS_LOCK", KEY_CAPS_LOCK},
  {"F1", KEY_F1},
  {"F2", KEY_F2},
  {"F3", KEY_F3},
  {"F4", KEY_F4},
  {"F5", KEY_F5},
  {"F6", KEY_F6},
  {"F7", KEY_F7},
  {"F8", KEY_F8},
  {"F9", KEY_F9},
  {"F10", KEY_F10},
  {"F11", KEY_F11},
  {"F12", KEY_F12},
  {"F13", KEY_F13},
  {"F14", KEY_F14},
  {"F15", KEY_F15},
  {"F16", KEY_F16},
  {"F17", KEY_F17},
  {"F18", KEY_F18},
  {"F19", KEY_F19},
  {"F20", KEY_F20},
  {"F21", KEY_F21},
  {"F22", KEY_F22},
  {"F23", KEY_F23},
  {"F24", KEY_F24},
  {"INSERT", KEY_INSERT},
  {"HOME", KEY_HOME},
  {"PAGE_UP", KEY_PAGE_UP},
  {"DELETE", KEY_DELETE},
  {"END", KEY_END},
  {"PAGE_DOWN", KEY_PAGE_DOWN},
  {"PAUSE", KEY_PAUSE},
  {"SCROLL_LOCK", KEY_SCROLL_LOCK},
  {"NUM_LOCK", KEY_NUM_LOCK},
  {"PRINTSCREEN", KEY_PRINT_SCREEN},
  {"SEMICOLON", ';'},
  {"COMMA", ','},
  {"PERIOD", ','},
  {"MINUS", '-'},
  {"EQUAL", '='},
  {"SLASH", '/'},
  {"BACKSLASH", '\\'},
  {"LEFT_BRACE", '('},
  {"RIGHT_BRACE", ')'},
  {"QUOTE", '"'},
  {"TILDE", '~'},
  {"MENU", KEY_MENU}
};

/**
   keymap2
   keycode/key-identifier mapping for key-identifiers with prefix "KEYPAD_"
*/
const keymap_struct keymap2 [] = {
  {"SLASH", KEY_KP_SLASH},
  {"ASTERIX", KEY_KP_ASTERISK},
  {"MINUS", KEY_KP_MINUS},
  {"PLUS", KEY_KP_PLUS},
  {"ENTER", KEY_KP_ENTER},
  {"1", KEY_KP_1},
  {"2", KEY_KP_2},
  {"3", KEY_KP_3},
  {"4", KEY_KP_4},
  {"5", KEY_KP_5},
  {"6", KEY_KP_6},
  {"7", KEY_KP_7},
  {"8", KEY_KP_8},
  {"9", KEY_KP_9},
  {"0", KEY_KP_0},
  {"PERIOD", KEY_KP_DOT}
};

#define KEYMAP1_ELEMENTS (sizeof keymap1 / sizeof keymap1[0])   // number of key-identifiers with prefix "KEY_"
#define KEYMAP2_ELEMENTS (sizeof keymap2 / sizeof keymap2[0])   // number of key-identifiers with prefix "KEYPAD_"

/**
   @name performKeyActions
   @brief press, release or hold multiple keys
   @param text is a string which contains the key identifiers eg. "KEY_CTRL KEY_C" for Ctrl-C
   @param keyAction the action will shall be performed
   @return none
*/
void performKeyActions(char* text,  uint8_t keyAction)
{
  char * tmptxt = (char *) malloc( sizeof(char) * ( strlen(text) + 2 ) ); // for parsing keystrings
  char * acttoken;
  bool found = false;

  strcpy(tmptxt, text);
  if (tmptxt[strlen(tmptxt) - 1] != ' ') strcat(tmptxt, " ");

  acttoken = strtok(tmptxt, " ");
  while (acttoken)
  {
    if (!strncmp(acttoken, "KEY_", 4)) {
      acttoken += 4;
      found = false;
      
      for (unsigned int i = 0; i < KEYMAP1_ELEMENTS; i++) {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.print("scanning for ");  Serial.println(keymap1[i].token);
        #endif
        
        if (!strcmp(acttoken, keymap1[i].token)) {
          #ifdef DEBUG_OUTPUT_KEYS
            Serial.print("found @"); Serial.print(i); Serial.print(", keycode: "); Serial.println(keymap1[i].key);
          #endif
          
          updateKey(keymap1[i].key, keyAction);
          found = true;
          break;
        }
      }
      //if not found in the array, try if it is 0-9 or A-Z keys
      //we need to split this test, because we need small letters for Keyboard.print.
      if(!found && (acttoken[0] >= '0' && acttoken[0] <= '9'))
      {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.print("found num key: "); Serial.println(acttoken[0]);
        #endif
        
        updateKey(acttoken[0], keyAction);
        found = true;
      }
	    
      if(!found && (acttoken[0] >= 'A' && acttoken[0] <= 'Z'))
      {
        #ifdef DEBUG_OUTPUT_KEYS
          Serial.print("found ascii keys: "); Serial.println(toLowerCase(acttoken[0]));
        #endif
        
        updateKey(toLowerCase(acttoken[0]), keyAction);
        found = true;
      }
    }

    if (!strncmp(acttoken, "KEYPAD_", 7)) {
      acttoken += 7;
      for (unsigned int i = 0; i < KEYMAP2_ELEMENTS; i++) {
        if (!strcmp(acttoken, keymap2[i].token))
          updateKey(keymap2[i].key, keyAction);
      }
    }
    acttoken = strtok(NULL, " ");
  }
  free(tmptxt);
}
