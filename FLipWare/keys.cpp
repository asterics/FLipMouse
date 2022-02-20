/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: keys.cpp - implementation of the keyboard handling

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/



#include "FlipWare.h"

#define KEY_PRESS    0
#define KEY_HOLD     1
#define KEY_RELEASE  2
#define KEY_TOGGLE   3

#define KEYPRESS_BUFFERSIZE 8

int pressed_keys[KEYPRESS_BUFFERSIZE];

uint8_t in_keybuffer(int key);
void remove_from_keybuffer(int key);
void add_to_keybuffer(int key);
void setKeyValues(char* text, uint8_t keyAction);



void updateKey(int key, uint8_t keyAction)
{
  switch (keyAction)  {
    case KEY_PRESS:
    case KEY_HOLD:
      add_to_keybuffer(key);
      keyboardPress(key);       // press/hold keys individually
      break;

    case KEY_RELEASE:
      remove_from_keybuffer(key);
      keyboardRelease(key);       // release keys individually
      break;

    case KEY_TOGGLE:
      if (in_keybuffer(key))  {
        remove_from_keybuffer(key);
        keyboardRelease(key);
      } else {
        add_to_keybuffer (key);
        keyboardPress(key);
      }
      break;
  }
}

void pressKeys (char * text)
{
  setKeyValues(text, KEY_PRESS);
  setKeyValues(text, KEY_RELEASE);
}

void holdKeys (char * text)
{
  setKeyValues(text, KEY_HOLD);
}

void releaseKeys (char * text)
{
  setKeyValues(text, KEY_RELEASE);
}

void toggleKeys (char * text)
{
  setKeyValues(text, KEY_TOGGLE);
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


void add_to_keybuffer(int key)
{
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == 0)
      pressed_keys[i] = key;
    if (pressed_keys[i] == key) i = KEYPRESS_BUFFERSIZE;
  }
}

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

uint8_t in_keybuffer(int key)
{
  for (int i = 0; i < KEYPRESS_BUFFERSIZE; i++)
  {
    if (pressed_keys[i] == key)
      return (1);
  }
  return (0);
}

struct keymap_struct {
  char *token;
  int key;
};

const keymap_struct keymap1 [] = {      // keymap for prefix KEY_

  {"SHIFT", KEY_LEFT_SHIFT},
  {"CTRL", KEY_LEFT_CTRL},
  {"ALT", KEY_LEFT_ALT},
  {"RIGHT_ALT", KEY_RIGHT_ALT},
  {"GUI", KEY_LEFT_GUI},
  {"RIGHT_GUI", KEY_RIGHT_GUI},
  {"UP", KEY_UP},
  {"DOWN", KEY_DOWN},
  {"LEFT", KEY_LEFT},
  {"RIGHT", KEY_RIGHT},
  {"ENTER", KEY_ENTER},
  {"SPACE", KEY_SPACE},
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
  {"A", KEY_A},
  {"B", KEY_B},
  {"C", KEY_C},
  {"D", KEY_D},
  {"E", KEY_E},
  {"F", KEY_F},
  {"G", KEY_G},
  {"H", KEY_H},
  {"I", KEY_I},
  {"J", KEY_J},
  {"K", KEY_K},
  {"L", KEY_L},
  {"M", KEY_M},
  {"N", KEY_N},
  {"O", KEY_O},
  {"P", KEY_P},
  {"Q", KEY_Q},
  {"R", KEY_R},
  {"S", KEY_S},
  {"T", KEY_T},
  {"U", KEY_U},
  {"V", KEY_V},
  {"W", KEY_W},
  {"X", KEY_X},
  {"Y", KEY_Y},
  {"Z", KEY_Z},
  {"1", KEY_1},
  {"2", KEY_2},
  {"3", KEY_3},
  {"4", KEY_4},
  {"5", KEY_5},
  {"6", KEY_6},
  {"7", KEY_7},
  {"8", KEY_8},
  {"9", KEY_9},
  {"0", KEY_0},
  {"PAUSE", KEY_PAUSE},
  {"SCROLL_LOCK", KEY_SCROLL_LOCK},
  {"NUM_LOCK", KEY_NUM_LOCK},
  {"PRINTSCREEN", KEY_PRINTSCREEN},
  {"SEMICOLON", KEY_SEMICOLON},
  {"COMMA", KEY_COMMA},
  {"PERIOD", KEY_PERIOD},
  {"MINUS", KEY_MINUS},
  {"EQUAL", KEY_EQUAL},
  {"SLASH", KEY_SLASH},
  {"BACKSLASH", KEY_BACKSLASH},
  {"LEFT_BRACE", KEY_LEFT_BRACE},
  {"RIGHT_BRACE", KEY_RIGHT_BRACE},
  {"QUOTE", KEY_QUOTE},
  {"TILDE", KEY_TILDE},
  {"MENU", KEY_MENU}
};

const keymap_struct keymap2 [] = {       // keymap for prefix KEYPAD_
  {"SLASH", KEYPAD_SLASH},
  {"ASTERIX", KEYPAD_ASTERIX},
  {"MINUS", KEYPAD_MINUS},
  {"PLUS", KEYPAD_PLUS},
  {"ENTER", KEYPAD_ENTER},
  {"1", KEYPAD_1},
  {"2", KEYPAD_2},
  {"3", KEYPAD_3},
  {"4", KEYPAD_4},
  {"5", KEYPAD_5},
  {"6", KEYPAD_6},
  {"7", KEYPAD_7},
  {"8", KEYPAD_8},
  {"9", KEYPAD_9},
  {"0", KEYPAD_0},
  {"PERIOD", KEYPAD_PERIOD}
};

#define KEYMAP1_ELEMENTS (sizeof keymap1 / sizeof keymap1[0])
#define KEYMAP2_ELEMENTS (sizeof keymap2 / sizeof keymap2[0])

// press, release or hold all supported keys
// text is a string which contains the key identifiers eg. "KEY_CTRL KEY_C" for Ctrl-C
void setKeyValues(char* text,  uint8_t keyAction)
{
  char * tmptxt = (char *) malloc( sizeof(char) * ( strlen(text) + 2 ) ); // for parsing keystrings
  char * acttoken;

  strcpy(tmptxt, text);
  if (tmptxt[strlen(tmptxt) - 1] != ' ') strcat(tmptxt, " ");

  acttoken = strtok(tmptxt, " ");
  while (acttoken)
  {
    if (!strncmp(acttoken, "KEY_", 4)) {
      acttoken += 4;
      for (int i = 0; i < KEYMAP1_ELEMENTS; i++) {
        // Serial.print("scanning for ");  Serial.println(keymap1[i].token);
        if (!strcmp(acttoken, keymap1[i].token)) {
          // Serial.println("found!");
          updateKey(keymap1[i].key, keyAction);
          break;
        }
      }
    }

    if (!strncmp(acttoken, "KEYPAD_", 7)) {
      acttoken += 7;
      for (int i = 0; i < KEYMAP2_ELEMENTS; i++) {
        if (!strcmp(acttoken, keymap2[i].token))
          updateKey(keymap2[i].key, keyAction);
      }
    }
    acttoken = strtok(NULL, " ");
  }
  free(tmptxt);
}
