/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: commands.h - implementation of the AT-commands, header file


   Supported AT-commands:
   (sent via serial interface, 115200 baud, using spaces between parameters.  Enter (<cr>, ASCII-code 0x0d) finishes a command)

          AT                returns "OK"
          AT ID             returns identification string (e.g. "FLipMouse V2.0")
          AT BM <uint>      puts button into programming mode (e.g. "AT BM 2" -> next AT-command defines the new function for button 2)
                            for the FLipmouse, there are 11 buttons available (3 physical buttons, 8 virtual functions):

                              1: internal button1 / Strong Puff + UP
                              2: external button2 / Strong Puff + LEFT
                              3: external button3 / Strong Puff + RIGHT
                              4: alternative UP
                              5: alternative DOWN
                              6: alternative LEFT
                              7: alternative RIGHT
                              8: Sip (pressure lower than sip threshold)
                              9: Strong Sip
                              10: Puff (pressure bigger than puff threshold)
                              11: StrongSip + Up
                              12: StrongSip + Down
                              13: StrongSip + Left
                              14: StrongSip + Right
                              15: StrongPuff + Up
                              16: StrongPuff + Down
                              17: StrongPuff + Left
                              18: StrongPuff + Right

    USB HID commands:

          AT CL             click left mouse button
          AT CR             click right mouse button
          AT CM             click middle mouse button
          AT CD             click double with left mouse button

          AT HL             hold the left mouse button (automatic release when user action is done)
          AT HR             hold the right mouse button (automatic release when user action is done)
          AT HM             hold the middle mouse button (automatic release when user action is done)

          AT RL             release the left mouse button
          AT RR             release the right mouse button
          AT RM             release the middle mouse button

          AT TL             toggle the left mouse button
          AT TR             toggle the right mouse button
          AT TM             toggle the middle mouse button

          AT WU             move mouse wheel up
          AT WD             move mouse wheel down
          AT WS <uint>      set mouse wheel stepsize (e.g. "AT WS 3" sets the wheel stepsize to 3 rows)

          AT MX <int>       move mouse in x direction (e.g. "AT MX 4" moves cursor 4 pixels to the right)
          AT MY <int>       move mouse in y direction (e.g. "AT MY -10" moves cursor 10 pixels up)
          AT RO <uint>      rotate stick orientation (e.g. "AT RO 180" flips x any y movements)

          AT JX <int>       set joystick x axis (e.g. "AT JX 512" sets the x-axis to middle position)
          AT JY <int>       set joystick y axis (e.g. "AT JY 1023" sets the y-axis to full up position)
          AT JZ <int>       set joystick z axis (e.g. "AT JZ 0" sets the z-axis to lowest position)
          AT JT <int>       set joystick z turn axis (e.g. "AT JT 512" sets the rotation to middle position)
          AT JS <int>       set joystick slider position (e.g. "AT JS 512" sets the slider to middle position)
          AT JP <int>       press joystick button (e.g. "AT JP 1" presses joystick button 1)
          AT JR <int>       release joystick button (e.g. "AT JR 2" releases joystick button 2)
          AT JH <int>       set joystick hat position (e.g. "AT JH 45" sets joystick hat to 45 degrees)
                            possible values are: 0, 45, 90, 135, 180, 225, 270, 315 and -1 to set center position)

          AT KW <string>    keyboard write string (e.g." AT KW Hello!" writes "Hello!")
          AT KP <string>    key press: press keys once (automatic release after all keys were pressed)
                            (e.g. "AT KP KEY_UP" presses the "Cursor-Up" key, "AT KP KEY_CTRL KEY_ALT KEY_DELETE" presses all three keys)
          AT KH <string>    key hold: hold keys (automatic release when user action is done)
                            (e.g. "AT KH KEY_A" presses the "A" key until  "AT KR KEY_A" is sent)
          AT KT <string>    key toggle: "sticky" hold keys (key will be pressed until "AT KT" command is sent again or a "AT KR" command is sent)
                            in contrast to "AT KH" a finished user action does not release the keys
                            (e.g. "AT KT KEY_A" presses the "A" key until  "AT KT KEY_A" is sent again.)

                            for a list of supported key idientifier strings see below !

          AT KR <string>    key release: releases all keys identified in the string
          AT RA             release all: releases all currently pressed keys and buttons

    Housekeeping commands:

          AT SA <string>  save settings and current button modes to next free eeprom slot under given name (e.g. "AT SA mouse1")
          AT LO <string>  load button modes from eeprom slot (e.g. AT LOAD mouse1 -> loads profile named "mouse1")
          AT LA           load all slots (displays names and settings of all stored slots)
          AT LI           list all saved mode names
          AT NE           next mode will be loaded (wrap around after last slot)
          AT DE           delete EEPROM content (delete all stored slots)
          AT NC           no command (idle operation)
          AT BT <uint>    set bluetooth mode, 1=USB only, 2=BT only, 3=both(default)
                          (e.g. AT BT 2 -> send HID commands only via BT if BT-daughter board is available)

    FLipMouse-specific settings and commands:

          AT MM <uint>    mouse mode: cursor on (uint==1) or alternative functions on (uint==0)
          AT SW           switch between mouse cursor and alternative functions
          AT SR           start reporting raw values (5 sensor values, starting with "VALUES:")
          AT ER           end reporting raw values
          AT CA           calibration of zeropoint
          AT AX <uint>    acceleration x-axis  (0-100)
          AT AY <uint>    acceleration y-axis  (0-100)
          AT DX <uint>    deadzone x-axis  (0-1000)
          AT DY <uint>    deadzone y-axis  (0-1000)
          AT MS <uint>    maximum speed  (0-100)
          AT AC <uint>    acceleration time (0-100)
          AT MA <string>  execute a command macro containing multiple commands (separated by semicolon)
                          example: "AT MA MX 100;MY 100;CL;"  use backslash to mask semicolon: "AT MA KW \;;CL;" writes a semicolon and then clicks left
          AT WA <uint>    wait (given in milliseconds, useful for macro commands)

          AT TS <uint>    treshold for sip action  (0-512)
          AT TP <uint>    treshold for puff action (512-1023)
          AT SP <uint>    treshold for strong puff (512-1023)
          AT SS <uint>    treshold for strong sip (0-512)
          AT GV <uint>    gain vertical drift compensation (0-100)
          AT RV <uint>    range vertical drift compensation (0-100)
          AT GH <uint>    gain horizontal drift compensation (0-100)
          AT RH <uint>    range horizontal drift compensation (0-100)

    Infrared-specific commands:

          AT IR <string>  record new infrared code and store it under given name (e.g. "AT IR vol_up")
          AT IP <string>  play infrared code with given name (e.g. "AT IP vol_up")
          AT IH <string>  play and hold the infrared code with given name
          AT IS           stop currently playing infrared code
          AT IC <string>  clear infrared code with given name (e.g. "AT IC vol_up")
          AT IW           wipe infrared memory (clear all codes)
          AT IL           lists all stored infrared command names
          AT IT <uint>    set code timeout value for IR Recording (e.g. "AT IT 10" sets 10 milliseconds timeout)

   supported key identifiers for key commands (AT KP/KH/KR/KT):

    KEY_A   KEY_B   KEY_C   KEY_D    KEY_E   KEY_F   KEY_G   KEY_H   KEY_I   KEY_J    KEY_K    KEY_L
    KEY_M   KEY_N   KEY_O   KEY_P    KEY_Q   KEY_R   KEY_S   KEY_T   KEY_U   KEY_V    KEY_W    KEY_X
    KEY_Y   KEY_Z   KEY_1   KEY_2    KEY_3   KEY_4   KEY_5   KEY_6   KEY_7   KEY_8    KEY_9    KEY_0
    KEY_F1  KEY_F2  KEY_F3  KEY_F4   KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  KEY_F11  KEY_F12

    KEY_RIGHT   KEY_LEFT       KEY_DOWN        KEY_UP          KEY_ENTER    KEY_ESC   KEY_BACKSPACE   KEY_TAB
    KEY_HOME    KEY_PAGE_UP    KEY_PAGE_DOWN   KEY_DELETE      KEY_INSERT   KEY_END   KEY_NUM_LOCK    KEY_SCROLL_LOCK
    KEY_SPACE   KEY_CAPS_LOCK  KEY_PAUSE       
    KEY_SHIFT   KEY_CTRL       KEY_ALT         KEY_RIGHT_ALT   KEY_GUI      KEY_RIGHT_GUI
    KEY_SLASH   KEY_BACKSLASH  KEY_LEFT_BRACE  KEY_RIGHT_BRACE KEY_QUOTE    KEY_TILDE    
    KEY_MINUS   KEY_SEMICOLON  KEY_EQUAL       KEY_COMMA       KEY_PERIOD   KEY_MENU
    
    KEYPAD_1        KEYPAD_2       KEYPAD_3        KEYPAD_4         KEYPAD_5        KEYPAD_6   
    KEYPAD_7        KEYPAD_8       KEYPAD_9        KEYPAD_0         KEYPAD_SLASH    KEYPAD_MINUS    KEYPAD_PLUS
    KEYPAD_ENTER    KEYPAD_PERIOD  KEYPAD_ASTERIX


   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _COMMANDS_H_
#define _COMMANDS_H_


// command identifiers

enum atCommands {
  CMD_ID, CMD_BM, CMD_CL, CMD_CR, CMD_CM, CMD_CD, CMD_PL, CMD_PR, CMD_PM, CMD_RL, CMD_RR, CMD_RM,
  CMD_WU, CMD_WD, CMD_WS, CMD_MX, CMD_MY, CMD_KW, CMD_KP, CMD_KR, CMD_RA, CMD_SA, CMD_LO, CMD_LA,
  CMD_LI, CMD_NE, CMD_DE, CMD_NC, CMD_MM, CMD_SW, CMD_SR, CMD_ER, CMD_CA, CMD_AX,
  CMD_AY, CMD_DX, CMD_DY, CMD_TS, CMD_TP, CMD_SP, CMD_SS, CMD_GV, CMD_RV, CMD_GH, CMD_RH, CMD_IR,
  CMD_IP, CMD_IC, CMD_IL, CMD_JX, CMD_JY, CMD_JZ, CMD_JT, CMD_JS, CMD_JP, CMD_JR, CMD_JH,
  CMD_IT, CMD_KH, CMD_MS, CMD_AC, CMD_MA, CMD_WA, CMD_RO, CMD_IW, CMD_BT, CMD_HL, CMD_HR, CMD_HM,
  CMD_TL, CMD_TR, CMD_TM, CMD_KT, CMD_IH, CMD_IS,
  NUM_COMMANDS
};

#endif
