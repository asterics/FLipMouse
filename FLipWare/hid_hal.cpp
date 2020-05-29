/*
	FLipWare - AsTeRICS Foundation
	For more info please visit: http://www.asterics-academy.net

	Module: hid_hal.cpp - covers USB HID and BT HID send routines

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.
  
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; See the GNU General Public License: 
	http://www.gnu.org/licenses/gpl-3.0.en.html
*/ 

#include "hid_hal.h"

void mouseRelease(uint8_t button)
{
  if (settings.bt & 1) 
    Mouse.release(button); 

  if ((settings.bt & 2) && (isBluetoothAvailable()))  
    mouseBTRelease(button);
}

void mousePress(uint8_t button)
{
  if (settings.bt & 1) 
    Mouse.press(button); 

  if ((settings.bt & 2) && (isBluetoothAvailable())) 
    mouseBTPress(button);
}

void mouseToggle(uint8_t button)
{
  if (settings.bt & 1) {
    if (Mouse.isPressed(button))
       Mouse.release(button); else Mouse.press(button);
  }
  
  if ((settings.bt & 2) && (isBluetoothAvailable())) {
    if (isMouseBTPressed(button))
      mouseBTRelease(button); else mouseBTPress(button);
  }
}


void mouseScroll(int8_t steps) 
{
  if (settings.bt & 1) 
    Mouse.scroll(steps);

  if ((settings.bt & 2) && (isBluetoothAvailable())) 
    mouseBT(0,0,steps);
}

void mouseMove(int x, int y) 
{
   while (x<-128) { 
     if (settings.bt & 1) 
		    Mouse.move(-128, 0); 
	   if ((settings.bt & 2) && (isBluetoothAvailable()))  
	      mouseBT(-128,0,0);
	   x+=128; 
	 }
   while (x>127) { 
     if (settings.bt & 1) 
	    	Mouse.move(127, 0); 
	   if ((settings.bt & 2) && (isBluetoothAvailable())) 
	      mouseBT(127,0,0);
		 x-=127; 
	 }
 
   while (y<-128) { 
     if (settings.bt & 1) 
		    Mouse.move(0, -128); 
	   if ((settings.bt & 2) && (isBluetoothAvailable()))  
	      mouseBT(0,-128,0);
	   y+=128; 
	 }
   while (y>127) { 
     if (settings.bt & 1) 
		    Mouse.move(0, 127); 
	   if ((settings.bt & 2) && (isBluetoothAvailable())) 
	      mouseBT(0,127,0);
		 y-=127; 
	 }

   if (settings.bt & 1) 
      Mouse.move(x, y);
   if ((settings.bt & 2) && (isBluetoothAvailable()))
      mouseBT(x,y,0);
}

void keyboardPrint(char * keystring)
{
  if (settings.bt & 1) {
    //Keyboard.print(keystring);  // improved for ISO 8859 compatibility (but: slower ..)
    for (int i=0; i<strlen(keystring); i++)
    {
        Keyboard.press(keystring[i]);
        Keyboard.release(keystring[i]);
    }
  }

  if ((settings.bt & 2) && (isBluetoothAvailable())) 
    keyboardBTPrint(keystring);  // TODO: check ISO8859-compatibility
}

void keyboardPress(int key)
{
  if (settings.bt & 1)
        Keyboard.press(key);
  if ((settings.bt & 2) && (isBluetoothAvailable())) 
        keyboardBTPress(key);   
}

void keyboardRelease(int key)
{
  if (settings.bt & 1)
      Keyboard.release(key);
  if ((settings.bt & 2) && (isBluetoothAvailable())) 
      keyboardBTRelease(key);   
}

void keyboardReleaseAll()
{
  if (settings.bt & 1)
     Keyboard.releaseAll();
  if ((settings.bt & 2) && (isBluetoothAvailable())) 
     keyboardBTReleaseAll();
}


/*
 * name: joystickAxis
 * @param axistype Determine axis for these values: 
 * 			- STICKMODE_JOYSTICK_XY Assign value1 to X, and value2 to Y
 * 			- STICKMODE_JOYSTICK_ZR Assign value1 to Z, and value2 to Zrotate
 * 			- STICKMODE_JOYSTICK_SLIDERS Assign value1 to slider left, and value2 to slider right
 * @param value1 Value 1 for the selected axis
 * @param value2 Value 2 for the selected axis
 * 
 * set a pair of joystick axis to the defined values.
 */
void joystickAxis(uint8_t axistype, int16_t value1, int16_t value2)
{
	switch(axistype)
	{
		case STICKMODE_JOYSTICK_XY:
			if((settings.bt & 1) &&  (value1 != -1)) Joystick.X(value1);
			if((settings.bt & 1) &&  (value2 != -1)) Joystick.Y(value2);
			if((settings.bt & 2) && (isBluetoothAvailable())) joystickBTAxis(axistype,value1,value2);
		break;
		
		case STICKMODE_JOYSTICK_ZR:
			if((settings.bt & 1) &&  (value1 != -1)) Joystick.Z(value1);
			if((settings.bt & 1) &&  (value2 != -1)) Joystick.Zrotate(value2);
			if((settings.bt & 2) && (isBluetoothAvailable())) joystickBTAxis(axistype,value1,value2);
		break;
		
		case STICKMODE_JOYSTICK_SLIDERS:
			if((settings.bt & 1) &&  (value1 != -1)) Joystick.sliderLeft(value1);
			if((settings.bt & 1) &&  (value2 != -1)) Joystick.sliderRight(value2);
			if((settings.bt & 2) && (isBluetoothAvailable())) joystickBTAxis(axistype,value1,value2);
		break;
	}
}

/*
 * name: joystickButtons
 * @param buttonnr Number of button to be pressed/released
 * @param value State of button (false/0 for released, true/1 for pressed)
 * @note Button number is limited to 31! (32buttons)
 * This function sets the state of all buttons for the joystick.
 */
void joystickButtons(uint8_t buttonnr, bool value)
{
	if(settings.bt & 1) Joystick.button(buttonnr,value);
	if(settings.bt & 2) joystickBTButtons(buttonnr,value);
}

/*
 * name: joystickHat
 * @param angle Value of hat, possible values: 0,45,90,135,180,225,270,315,-1
 * 
 * This functions sets the hat position. Center/released position is -1
 */
void joystickHat(int16_t angle)
{
	if(settings.bt & 1) Joystick.hat(angle);
	if(settings.bt & 2) joystickBTHat(angle);
}

