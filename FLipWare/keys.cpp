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

int keyAction=KEY_ADD;
char tmptxt[MAX_KEYSTRING_LEN];   // for parsing keystrings
int pressed_keys[KEYPRESS_BUFFERSIZE];

uint8_t in_keybuffer(int key);
void remove_from_keybuffer(int key);
void add_to_keybuffer(int key);
void setKeyValues(char* text);



void updateKey(int key)
{
   switch (keyAction)  {
     case KEY_ADD:
        // add_to_keybuffer (key);
        Keyboard.press(key);     // press keys individually
        keyboardBTPress(key);   
        break;
     case KEY_RELEASE:
        // remove_from_keybuffer(key);
        Keyboard.release(key);   // release keys individually
        keyboardBTRelease(key);
        break;
        
     case KEY_HOLD:
        if (in_keybuffer(key))  {
            remove_from_keybuffer(key);
            Keyboard.release(key);   // release keys individually
            keyboardBTRelease(key);
        } else {
            add_to_keybuffer (key);
            Keyboard.press(key);     // press keys individually  
            keyboardBTPress(key);
        }        
     }  
}

void pressKeys (char * text)
{
   keyAction=KEY_ADD; 
   setKeyValues(text);
}

void holdKeys (char * text)
{
   keyAction=KEY_HOLD; 
   setKeyValues(text);
}

void releaseKeys (char * text)
{
   keyAction=KEY_RELEASE; 
   setKeyValues(text);
}

void release_all_keys()
{
   Keyboard.releaseAll();
   keyboardBTReleaseAll();
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


// press all supported keys 
// text is a string which contains the key identifiers eg. "KEY_CTRL KEY_C" for Ctrl-C
void setKeyValues(char* text)
{
  char * acttoken;
  //int modifiers=0;

  //Serial.print("setKeyValues: "); Serial.println(text);

  strcpy(tmptxt, text); 
  acttoken = strtok(tmptxt," ");
  
  while (acttoken)
  {
    if (!strcmp(acttoken,"KEY_SHIFT"))  updateKey(KEY_LEFT_SHIFT);
    if (!strcmp(acttoken,"KEY_CTRL"))  updateKey(KEY_LEFT_CTRL);
    if (!strcmp(acttoken,"KEY_ALT"))  updateKey(KEY_LEFT_ALT);
    if (!strcmp(acttoken,"KEY_RIGHT_ALT"))  updateKey(KEY_RIGHT_ALT);
    if (!strcmp(acttoken,"KEY_GUI"))  updateKey(KEY_LEFT_GUI);
    if (!strcmp(acttoken,"KEY_RIGHT_GUI"))  updateKey(KEY_RIGHT_GUI);
    if (!strcmp(acttoken,"KEY_UP")) updateKey(KEY_UP);
    if (!strcmp(acttoken,"KEY_DOWN")) updateKey(KEY_DOWN);
    if (!strcmp(acttoken,"KEY_LEFT")) updateKey(KEY_LEFT);
    if (!strcmp(acttoken,"KEY_RIGHT")) updateKey(KEY_RIGHT);
    if (!strcmp(acttoken,"KEY_ENTER")) updateKey(KEY_ENTER);
    if (!strcmp(acttoken,"KEY_SPACE")) updateKey(KEY_SPACE);
    if (!strcmp(acttoken,"KEY_ESC")) updateKey(KEY_ESC);
    if (!strcmp(acttoken,"KEY_BACKSPACE")) updateKey(KEY_BACKSPACE);
    if (!strcmp(acttoken,"KEY_TAB")) updateKey(KEY_TAB);
    if (!strcmp(acttoken,"KEY_CAPS_LOCK")) updateKey(KEY_CAPS_LOCK);
    if (!strcmp(acttoken,"KEY_F1")) updateKey(KEY_F1);
    if (!strcmp(acttoken,"KEY_F2")) updateKey(KEY_F2);
    if (!strcmp(acttoken,"KEY_F3")) updateKey(KEY_F3);
    if (!strcmp(acttoken,"KEY_F4")) updateKey(KEY_F4);
    if (!strcmp(acttoken,"KEY_F5")) updateKey(KEY_F5);
    if (!strcmp(acttoken,"KEY_F6")) updateKey(KEY_F6);
    if (!strcmp(acttoken,"KEY_F7")) updateKey(KEY_F7);
    if (!strcmp(acttoken,"KEY_F8")) updateKey(KEY_F8);
    if (!strcmp(acttoken,"KEY_F9")) updateKey(KEY_F9);
    if (!strcmp(acttoken,"KEY_F10")) updateKey(KEY_F10);
    if (!strcmp(acttoken,"KEY_F11")) updateKey(KEY_F11);
    if (!strcmp(acttoken,"KEY_F12")) updateKey(KEY_F12);
    if (!strcmp(acttoken,"KEY_INSERT")) updateKey(KEY_INSERT);
    if (!strcmp(acttoken,"KEY_HOME")) updateKey(KEY_HOME);
    if (!strcmp(acttoken,"KEY_PAGE_UP")) updateKey(KEY_PAGE_UP);
    if (!strcmp(acttoken,"KEY_DELETE")) updateKey(KEY_DELETE);
    if (!strcmp(acttoken,"KEY_END")) updateKey(KEY_END);
    if (!strcmp(acttoken,"KEY_PAGE_DOWN")) updateKey(KEY_PAGE_DOWN);

    if (!strcmp(acttoken,"KEY_A")) updateKey(KEY_A);
    if (!strcmp(acttoken,"KEY_B")) updateKey(KEY_B);
    if (!strcmp(acttoken,"KEY_C")) updateKey(KEY_C);
    if (!strcmp(acttoken,"KEY_D")) updateKey(KEY_D);
    if (!strcmp(acttoken,"KEY_E")) updateKey(KEY_E);
    if (!strcmp(acttoken,"KEY_F")) updateKey(KEY_F);
    if (!strcmp(acttoken,"KEY_G")) updateKey(KEY_G);
    if (!strcmp(acttoken,"KEY_H")) updateKey(KEY_H);
    if (!strcmp(acttoken,"KEY_I")) updateKey(KEY_I);
    if (!strcmp(acttoken,"KEY_J")) updateKey(KEY_J);
    if (!strcmp(acttoken,"KEY_K")) updateKey(KEY_K);
    if (!strcmp(acttoken,"KEY_L")) updateKey(KEY_L);
    if (!strcmp(acttoken,"KEY_M")) updateKey(KEY_M);
    if (!strcmp(acttoken,"KEY_N")) updateKey(KEY_N);
    if (!strcmp(acttoken,"KEY_O")) updateKey(KEY_O);
    if (!strcmp(acttoken,"KEY_P")) updateKey(KEY_P);
    if (!strcmp(acttoken,"KEY_Q")) updateKey(KEY_Q);
    if (!strcmp(acttoken,"KEY_R")) updateKey(KEY_R);
    if (!strcmp(acttoken,"KEY_S")) updateKey(KEY_S);
    if (!strcmp(acttoken,"KEY_T")) updateKey(KEY_T);
    if (!strcmp(acttoken,"KEY_U")) updateKey(KEY_U);
    if (!strcmp(acttoken,"KEY_V")) updateKey(KEY_V);
    if (!strcmp(acttoken,"KEY_W")) updateKey(KEY_W);
    if (!strcmp(acttoken,"KEY_X")) updateKey(KEY_X);
    if (!strcmp(acttoken,"KEY_Y")) updateKey(KEY_Y);
    if (!strcmp(acttoken,"KEY_Z")) updateKey(KEY_Z);
    if (!strcmp(acttoken,"KEY_1")) updateKey(KEY_1);
    if (!strcmp(acttoken,"KEY_2")) updateKey(KEY_2);
    if (!strcmp(acttoken,"KEY_3")) updateKey(KEY_3);
    if (!strcmp(acttoken,"KEY_4")) updateKey(KEY_4);
    if (!strcmp(acttoken,"KEY_5")) updateKey(KEY_5);
    if (!strcmp(acttoken,"KEY_6")) updateKey(KEY_6);
    if (!strcmp(acttoken,"KEY_7")) updateKey(KEY_7);
    if (!strcmp(acttoken,"KEY_8")) updateKey(KEY_8);
    if (!strcmp(acttoken,"KEY_9")) updateKey(KEY_9);
    if (!strcmp(acttoken,"KEY_0")) updateKey(KEY_0);
    if (!strcmp(acttoken,"KEY_SEMICOLON")) updateKey(KEY_SEMICOLON);
    
    if (!strcmp(acttoken,"KEY_SCROLL_LOCK")) updateKey(KEY_SCROLL_LOCK);
    if (!strcmp(acttoken,"KEY_PAUSE")) updateKey(KEY_PAUSE);
    if (!strcmp(acttoken,"KEY_NUM_LOCK")) updateKey(KEY_NUM_LOCK);
    if (!strcmp(acttoken,"KEY_PRINTSCREEN")) updateKey(KEY_PRINTSCREEN);

    acttoken = strtok(NULL," ");
  }
}


