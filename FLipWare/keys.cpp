/*
     FLipWare - AsTeRICS Foundation 2017
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

#define KEY_ADD     0
#define KEY_HOLD    1
#define KEY_RELEASE 2

#define KEYPRESS_BUFFERSIZE 8

int pressed_keys[KEYPRESS_BUFFERSIZE];

uint8_t in_keybuffer(int key);
void remove_from_keybuffer(int key);
void add_to_keybuffer(int key);
void setKeyValues(char* text, uint8_t keyAction);



void updateKey(int key, uint8_t keyAction)
{
   switch (keyAction)  {
     case KEY_ADD:
        keyboardPress(key);      // press keys individually
        break;
     case KEY_RELEASE:
        keyboardRelease(key);   // release keys individually
        break;
        
     case KEY_HOLD:
        if (in_keybuffer(key))  {
            remove_from_keybuffer(key);
            keyboardRelease(key);   // release keys individually
        } else {
            add_to_keybuffer (key);
            keyboardPress(key);     // press keys individually  
        }        
     }  
}

void pressKeys (char * text)
{
   setKeyValues(text,KEY_ADD);
}

void holdKeys (char * text)
{
   setKeyValues(text,KEY_HOLD);
}

void releaseKeys (char * text)
{
   setKeyValues(text,KEY_RELEASE);
}

void release_all_keys()
{
   keyboardReleaseAll();
   for (int i=0;i<KEYPRESS_BUFFERSIZE;i++)
      pressed_keys[i]=0;
}


void add_to_keybuffer(int key)
{
    for (int i=0;i<KEYPRESS_BUFFERSIZE;i++)
    {
       if (pressed_keys[i]==0)
           pressed_keys[i]=key;
       if (pressed_keys[i]==key) i=KEYPRESS_BUFFERSIZE;
    }
}

void remove_from_keybuffer(int key)
{
    for (int i=0;i<KEYPRESS_BUFFERSIZE;i++)
    {
       if (pressed_keys[i]==key)
       {
           while (i<KEYPRESS_BUFFERSIZE-1)
           {
              pressed_keys[i]=pressed_keys[i+1];
              i++;
           }
           pressed_keys[KEYPRESS_BUFFERSIZE-1]=0;
           i=KEYPRESS_BUFFERSIZE;   
       }
    }
}

uint8_t in_keybuffer(int key)
{
    for (int i=0;i<KEYPRESS_BUFFERSIZE;i++)
    {
       if (pressed_keys[i]==key)
         return(1);
    }
    return(0);
}


// press, release or hold all supported keys 
// text is a string which contains the key identifiers eg. "KEY_CTRL KEY_C" for Ctrl-C
void setKeyValues(char* text,  uint8_t keyAction)
{
  char tmptxt[80];   // for parsing keystrings
  char * acttoken;
  //int modifiers=0;

  //Serial.print("setKeyValues: "); Serial.println(text);

  strcpy(tmptxt, text); 
  acttoken = strtok(tmptxt," ");
  
  while (acttoken)
  {
    if (!strcmp(acttoken,"KEY_SHIFT"))  updateKey(KEY_LEFT_SHIFT, keyAction);
    if (!strcmp(acttoken,"KEY_CTRL"))  updateKey(KEY_LEFT_CTRL, keyAction);
    if (!strcmp(acttoken,"KEY_ALT"))  updateKey(KEY_LEFT_ALT, keyAction);
    if (!strcmp(acttoken,"KEY_RIGHT_ALT"))  updateKey(KEY_RIGHT_ALT, keyAction);
    if (!strcmp(acttoken,"KEY_GUI"))  updateKey(KEY_LEFT_GUI, keyAction);
    if (!strcmp(acttoken,"KEY_RIGHT_GUI"))  updateKey(KEY_RIGHT_GUI, keyAction);
    if (!strcmp(acttoken,"KEY_UP")) updateKey(KEY_UP, keyAction);
    if (!strcmp(acttoken,"KEY_DOWN")) updateKey(KEY_DOWN, keyAction);
    if (!strcmp(acttoken,"KEY_LEFT")) updateKey(KEY_LEFT, keyAction);
    if (!strcmp(acttoken,"KEY_RIGHT")) updateKey(KEY_RIGHT, keyAction);
    if (!strcmp(acttoken,"KEY_ENTER")) updateKey(KEY_ENTER, keyAction);
    if (!strcmp(acttoken,"KEY_SPACE")) updateKey(KEY_SPACE, keyAction);
    if (!strcmp(acttoken,"KEY_ESC")) updateKey(KEY_ESC, keyAction);
    if (!strcmp(acttoken,"KEY_BACKSPACE")) updateKey(KEY_BACKSPACE, keyAction);
    if (!strcmp(acttoken,"KEY_TAB")) updateKey(KEY_TAB, keyAction);
    if (!strcmp(acttoken,"KEY_CAPS_LOCK")) updateKey(KEY_CAPS_LOCK, keyAction);
    if (!strcmp(acttoken,"KEY_F1")) updateKey(KEY_F1, keyAction);
    if (!strcmp(acttoken,"KEY_F2")) updateKey(KEY_F2, keyAction);
    if (!strcmp(acttoken,"KEY_F3")) updateKey(KEY_F3, keyAction);
    if (!strcmp(acttoken,"KEY_F4")) updateKey(KEY_F4, keyAction);
    if (!strcmp(acttoken,"KEY_F5")) updateKey(KEY_F5, keyAction);
    if (!strcmp(acttoken,"KEY_F6")) updateKey(KEY_F6, keyAction);
    if (!strcmp(acttoken,"KEY_F7")) updateKey(KEY_F7, keyAction);
    if (!strcmp(acttoken,"KEY_F8")) updateKey(KEY_F8, keyAction);
    if (!strcmp(acttoken,"KEY_F9")) updateKey(KEY_F9, keyAction);
    if (!strcmp(acttoken,"KEY_F10")) updateKey(KEY_F10, keyAction);
    if (!strcmp(acttoken,"KEY_F11")) updateKey(KEY_F11, keyAction);
    if (!strcmp(acttoken,"KEY_F12")) updateKey(KEY_F12, keyAction);
    if (!strcmp(acttoken,"KEY_F13")) updateKey(KEY_F13, keyAction);
    if (!strcmp(acttoken,"KEY_F14")) updateKey(KEY_F14, keyAction);
    if (!strcmp(acttoken,"KEY_F15")) updateKey(KEY_F15, keyAction);
    if (!strcmp(acttoken,"KEY_F16")) updateKey(KEY_F16, keyAction);
    if (!strcmp(acttoken,"KEY_F17")) updateKey(KEY_F17, keyAction);
    if (!strcmp(acttoken,"KEY_F18")) updateKey(KEY_F18, keyAction);
    if (!strcmp(acttoken,"KEY_F19")) updateKey(KEY_F19, keyAction);
    if (!strcmp(acttoken,"KEY_F20")) updateKey(KEY_F20, keyAction);
    if (!strcmp(acttoken,"KEY_F21")) updateKey(KEY_F21, keyAction);
    if (!strcmp(acttoken,"KEY_F22")) updateKey(KEY_F22, keyAction);
    if (!strcmp(acttoken,"KEY_F23")) updateKey(KEY_F23, keyAction);
    if (!strcmp(acttoken,"KEY_F24")) updateKey(KEY_F24, keyAction);
    if (!strcmp(acttoken,"KEY_INSERT")) updateKey(KEY_INSERT, keyAction);
    if (!strcmp(acttoken,"KEY_HOME")) updateKey(KEY_HOME, keyAction);
    if (!strcmp(acttoken,"KEY_PAGE_UP")) updateKey(KEY_PAGE_UP, keyAction);
    if (!strcmp(acttoken,"KEY_DELETE")) updateKey(KEY_DELETE, keyAction);
    if (!strcmp(acttoken,"KEY_END")) updateKey(KEY_END, keyAction);
    if (!strcmp(acttoken,"KEY_PAGE_DOWN")) updateKey(KEY_PAGE_DOWN, keyAction);

    if (!strcmp(acttoken,"KEY_A")) updateKey(KEY_A, keyAction);
    if (!strcmp(acttoken,"KEY_B")) updateKey(KEY_B, keyAction);
    if (!strcmp(acttoken,"KEY_C")) updateKey(KEY_C, keyAction);
    if (!strcmp(acttoken,"KEY_D")) updateKey(KEY_D, keyAction);
    if (!strcmp(acttoken,"KEY_E")) updateKey(KEY_E, keyAction);
    if (!strcmp(acttoken,"KEY_F")) updateKey(KEY_F, keyAction);
    if (!strcmp(acttoken,"KEY_G")) updateKey(KEY_G, keyAction);
    if (!strcmp(acttoken,"KEY_H")) updateKey(KEY_H, keyAction);
    if (!strcmp(acttoken,"KEY_I")) updateKey(KEY_I, keyAction);
    if (!strcmp(acttoken,"KEY_J")) updateKey(KEY_J, keyAction);
    if (!strcmp(acttoken,"KEY_K")) updateKey(KEY_K, keyAction);
    if (!strcmp(acttoken,"KEY_L")) updateKey(KEY_L, keyAction);
    if (!strcmp(acttoken,"KEY_M")) updateKey(KEY_M, keyAction);
    if (!strcmp(acttoken,"KEY_N")) updateKey(KEY_N, keyAction);
    if (!strcmp(acttoken,"KEY_O")) updateKey(KEY_O, keyAction);
    if (!strcmp(acttoken,"KEY_P")) updateKey(KEY_P, keyAction);
    if (!strcmp(acttoken,"KEY_Q")) updateKey(KEY_Q, keyAction);
    if (!strcmp(acttoken,"KEY_R")) updateKey(KEY_R, keyAction);
    if (!strcmp(acttoken,"KEY_S")) updateKey(KEY_S, keyAction);
    if (!strcmp(acttoken,"KEY_T")) updateKey(KEY_T, keyAction);
    if (!strcmp(acttoken,"KEY_U")) updateKey(KEY_U, keyAction);
    if (!strcmp(acttoken,"KEY_V")) updateKey(KEY_V, keyAction);
    if (!strcmp(acttoken,"KEY_W")) updateKey(KEY_W, keyAction);
    if (!strcmp(acttoken,"KEY_X")) updateKey(KEY_X, keyAction);
    if (!strcmp(acttoken,"KEY_Y")) updateKey(KEY_Y, keyAction);
    if (!strcmp(acttoken,"KEY_Z")) updateKey(KEY_Z, keyAction);
    if (!strcmp(acttoken,"KEY_1")) updateKey(KEY_1, keyAction);
    if (!strcmp(acttoken,"KEY_2")) updateKey(KEY_2, keyAction);
    if (!strcmp(acttoken,"KEY_3")) updateKey(KEY_3, keyAction);
    if (!strcmp(acttoken,"KEY_4")) updateKey(KEY_4, keyAction);
    if (!strcmp(acttoken,"KEY_5")) updateKey(KEY_5, keyAction);
    if (!strcmp(acttoken,"KEY_6")) updateKey(KEY_6, keyAction);
    if (!strcmp(acttoken,"KEY_7")) updateKey(KEY_7, keyAction);
    if (!strcmp(acttoken,"KEY_8")) updateKey(KEY_8, keyAction);
    if (!strcmp(acttoken,"KEY_9")) updateKey(KEY_9, keyAction);
    if (!strcmp(acttoken,"KEY_0")) updateKey(KEY_0, keyAction);

    
    if (!strcmp(acttoken,"KEY_PAUSE")) updateKey(KEY_PAUSE, keyAction);
    if (!strcmp(acttoken,"KEY_SCROLL_LOCK")) updateKey(KEY_SCROLL_LOCK, keyAction);
    if (!strcmp(acttoken,"KEY_NUM_LOCK")) updateKey(KEY_NUM_LOCK, keyAction);
    if (!strcmp(acttoken,"KEY_PRINTSCREEN")) updateKey(KEY_PRINTSCREEN, keyAction);

    if (!strcmp(acttoken,"KEY_SEMICOLON")) updateKey(KEY_SEMICOLON, keyAction);
    if (!strcmp(acttoken,"KEY_COMMA")) updateKey(KEY_COMMA, keyAction);
    if (!strcmp(acttoken,"KEY_PERIOD")) updateKey(KEY_PERIOD, keyAction);
    if (!strcmp(acttoken,"KEY_MINUS")) updateKey(KEY_MINUS, keyAction);
    if (!strcmp(acttoken,"KEY_EQUAL")) updateKey(KEY_EQUAL, keyAction);
    if (!strcmp(acttoken,"KEY_SLASH")) updateKey(KEY_SLASH, keyAction);
    if (!strcmp(acttoken,"KEY_BACKSLASH")) updateKey(KEY_BACKSLASH, keyAction);
    if (!strcmp(acttoken,"KEY_LEFT_BRACE")) updateKey(KEY_LEFT_BRACE, keyAction);
    if (!strcmp(acttoken,"KEY_RIGHT_BRACE")) updateKey(KEY_RIGHT_BRACE, keyAction);
    if (!strcmp(acttoken,"KEY_QUOTE")) updateKey(KEY_QUOTE, keyAction);
    if (!strcmp(acttoken,"KEY_TILDE")) updateKey(KEY_TILDE, keyAction);
    if (!strcmp(acttoken,"KEY_MENU")) updateKey(KEY_MENU, keyAction);

    if (!strcmp(acttoken,"KEYPAD_SLASH")) updateKey(KEYPAD_SLASH, keyAction);
    if (!strcmp(acttoken,"KEYPAD_ASTERIX")) updateKey(KEYPAD_ASTERIX, keyAction);
    if (!strcmp(acttoken,"KEYPAD_MINUS")) updateKey(KEYPAD_MINUS, keyAction);
    if (!strcmp(acttoken,"KEYPAD_PLUS")) updateKey(KEYPAD_PLUS, keyAction);
    if (!strcmp(acttoken,"KEYPAD_ENTER")) updateKey(KEYPAD_ENTER, keyAction);
    if (!strcmp(acttoken,"KEYPAD_1")) updateKey(KEYPAD_1, keyAction);
    if (!strcmp(acttoken,"KEYPAD_2")) updateKey(KEYPAD_2, keyAction);
    if (!strcmp(acttoken,"KEYPAD_3")) updateKey(KEYPAD_3, keyAction);
    if (!strcmp(acttoken,"KEYPAD_4")) updateKey(KEYPAD_4, keyAction);
    if (!strcmp(acttoken,"KEYPAD_5")) updateKey(KEYPAD_5, keyAction);
    if (!strcmp(acttoken,"KEYPAD_6")) updateKey(KEYPAD_6, keyAction);
    if (!strcmp(acttoken,"KEYPAD_7")) updateKey(KEYPAD_7, keyAction);
    if (!strcmp(acttoken,"KEYPAD_8")) updateKey(KEYPAD_8, keyAction);
    if (!strcmp(acttoken,"KEYPAD_9")) updateKey(KEYPAD_9, keyAction);
    if (!strcmp(acttoken,"KEYPAD_0")) updateKey(KEYPAD_0, keyAction);
    if (!strcmp(acttoken,"KEYPAD_PERIOD")) updateKey(KEYPAD_PERIOD, keyAction);
    
    acttoken = strtok(NULL," ");
  }
}


