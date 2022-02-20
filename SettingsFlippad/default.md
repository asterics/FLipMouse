## Basic information
This configuration is the default configuration of the FlipPad.
It offers 4 modes (slots) for different mouse-pointer or keyboard actions.
The modes can be changed by finger gestures (double tap + movement) or by pressing Button1/Button3.

## Slot1 - "StickMode" (double tap + up)
- controls mouse cursor via USB interface
- uses joystick mode (return finger to first touch point to stop moving)

## Slot2 - "PadMode"  (double tap + right)
- controls mouse cursor via USB interface
- uses pad mode (finger movement = cursor movement)

## Slot3 - "Phone" (double tap + down)
- controls mouse cursor via BT interface 
- uses pad mode (finger movement = cursor movement)
- a tap + down gesture creates the keyboard shortcut KEY_GUI + KEY_BACKSPACE which acctivates the Home/Back button in many Android phones
- note that the BT-module must be installed and paired with a BT-capable device

## Slot4 - "GameKey" (double tap + left)
- finger movement creates cursor keys (up / down / left / right) via USB interface
- tap creates space key
- uses pad mode (last finger movement determines key)


## for all slots:
- Button 1 (internal button), Button3 = load next slot
- Button 2, Sip = hold left mouse button (in Slot4: Space Key)
- Puff = click right mouse button (in Slot4: Enter Key)


## Please note:
The position correction which is applied after taps and finger gestures only works correctly if the feature "enhance mouse pointer precision" is deactivated, 
please check the mouse pointer settings of your operating system. Also the overall mouse pointer speed may be adjusted there.