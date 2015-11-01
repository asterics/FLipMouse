
 /* 
 
   Supported AT-commands:  
   (sent via serial interface, 115200 baud, using spaces between parameters.  Enter (<cr>, ASCII-code 0x0d) finishes a command)
   
          AT                returns "OK"
          AT ID             returns identification string (e.g. "FLipMouse V2.0")
          AT BM <uint>      puts button into programming mode (e.g. "AT BM 2" -> next AT-command defines the new function for button 2)
                            for the FLipmouse, there are 11 buttons available (3 physical buttons, 8 virtual functions): 

                              1: internal button1 / Special UP
                              2: external button2 / Special LEFT 
                              3: external button3 / Special RIGHT 
                              4: alternative UP 
                              5: alternative DOWN 
                              6: alternative LEFT 
                              7: alternative RIGHT
                              8: SIP (pressure lower than sip threshold)
                              9: Special SIP
                              10: PUFF (pressure bigger than puff threshold)
                              11: Special PUFF

    USB HID commands:
      
          AT CL             click left mouse button  
          AT CR             click right mouse button  
          AT CM             click middle mouse button  
          AT CD             click double with left mouse button

          AT PL             press/hold the left mouse button  
          AT PR             press/hold the right mouse button
          AT PM             press/hold the middle mouse button 
  
          AT RL             release the left mouse button  
          AT RR             release the right mouse button
          AT RM             release the middle mouse button 
          
          AT WU             move mouse wheel up  
          AT WD             move mouse wheel down  
          AT WS <uint>      set mouse wheel stepsize (e.g. "AT WS 3" sets the wheel stepsize to 3 rows)
   
          AT MX <int>       move mouse in x direction (e.g. "AT MX 4" moves cursor 4 pixels to the right)  
          AT MY <int>       move mouse in y direction (e.g. "AT MY -10" moves cursor 10 pixels up)  

          AT KW <string>    keyboard write string (e.g." AT KW Hello!" writes "Hello!")    
          AT KP <string>    key press: press/hold keys identifier 
                            (e.g. "AT KP KEY_UP" presses the "Cursor-Up" key, "AT KP KEY_CTRL KEY_ALT KEY_DELETE" presses all three keys)
                            for a list of supported key idientifier strings see below ! 
                            
          AT KR <string>    key release: releases all keys identified in the string    
          AT RA             release all: releases all currently pressed keys and buttons    
          
    Housekeeping commands:

          AT SA <string>  save settings and current button modes to next free eeprom slot under given name (e.g. AT SAVE mouse1)
          AT LO <string>  load button modes from eeprom slot (e.g. AT LOAD mouse1 -> loads profile named "mouse1")
          AT LA           load all slots (displays names and settings of all stored slots) 
          AT LI           list all saved mode names 
          AT NE           next mode will be loaded (wrap around after last slot)
          AT DE           delete EEPROM content (delete all stored slots)
          AT NC           no command (idle operation)
          
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
          AT TS <uint>    treshold for sip action  (0-512)
          AT TP <uint>    treshold for puff action (512-1023)
          AT SM <uint>    treshold for special mode (512-1023)
          AT HM <uint>    treshold for hold mode (0-512)
          AT GU <uint>    gain for up sensor (0-100)
          AT GD <uint>    gain for down sensor (0-100)
          AT GL <uint>    gain for left sensor (0-100)
          AT GR <uint>    gain for right sensor (0-100)
  
    Infrared-specific commands:

          AT IR <string>  record new infrared code and store it under given name (e.g. "AT IR vol_up")
          AT IP <string>  play  infrared code with given name (e.g. "AT IP vol_up")
          AT IC <string>  clear infrared code with given name (e.g. "AT ID vol_up")
          AT IL           lists all stored infrared command names

   supported key identifiers for key press command (AT KP):
 
    KEY_A   KEY_B   KEY_C   KEY_D    KEY_E   KEY_F   KEY_G   KEY_H   KEY_I   KEY_J    KEY_K    KEY_L
    KEY_M   KEY_N   KEY_O   KEY_P    KEY_Q   KEY_R   KEY_S   KEY_T   KEY_U   KEY_V    KEY_W    KEY_X 
    KEY_Y   KEY_Z   KEY_1   KEY_2    KEY_3   KEY_4   KEY_5   KEY_6   KEY_7   KEY_8    KEY_9    KEY_0
    KEY_F1  KEY_F2  KEY_F3  KEY_F4   KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  KEY_F11  KEY_F12	
    
    KEY_RIGHT   KEY_LEFT       KEY_DOWN        KEY_UP      KEY_ENTER    KEY_ESC   KEY_BACKSPACE   KEY_TAB	
    KEY_HOME    KEY_PAGE_UP    KEY_PAGE_DOWN   KEY_DELETE  KEY_INSERT   KEY_END	  KEY_NUM_LOCK    KEY_SCROLL_LOCK
    KEY_SPACE   KEY_CAPS_LOCK  KEY_PAUSE       KEY_SHIFT   KEY_CTRL     KEY_ALT   KEY_RIGHT_ALT   KEY_GUI 
    KEY_RIGHT_GUI
    
*/

#ifndef _COMMANDS_H_
#define _COMMANDS_H_


// command identifiers

enum atCommands {
  CMD_ID, CMD_BM, CMD_CL, CMD_CR, CMD_CM, CMD_CD, CMD_PL, CMD_PR, CMD_PM, CMD_RL, CMD_RR, CMD_RM,
  CMD_WU, CMD_WD, CMD_WS, CMD_MX, CMD_MY, CMD_KW, CMD_KP, CMD_KR, CMD_RA, CMD_SA, CMD_LO, CMD_LA,
  CMD_LI, CMD_NE, CMD_DE, CMD_NC, CMD_MM, CMD_SW, CMD_SR, CMD_ER, CMD_CA, CMD_AX, CMD_AY, CMD_DX,
  CMD_DY, CMD_TS, CMD_TP, CMD_SM, CMD_HM, CMD_GU, CMD_GD, CMD_GL, CMD_GR, CMD_IR, CMD_IP, CMD_IC,
  CMD_IL,
  NUM_COMMANDS
};



#endif