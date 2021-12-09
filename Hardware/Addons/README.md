# FLipmouse Addons

This folder contains any additional piece of software hardware or gadget to increase the usability of the FLipmouse.
Since there is are 2 extension headers on the FLipmouse PCB, it is easy to extend the functionality.

## Glidepoint/Cirque Trackpad adapter

If the force actuated mouthpiece is not sufficient for your use case, it is possible
to combine the FLipMouse PCB with another type of input sensors:

A trackpad manufactured by Glidepoint.

An adapter PCB is available here, where it is possible to attach a different type of sensor.

__Note:__ As of December 2021, there is no stable firmware support. Due to limited flash memory available,
you have to flash a different firmware on your PCB (but the API is compatible with the configuration GUI).


## BT-module

The BT module is an ESP32 based addon board for the FLipMouse v2. It is placed on a separate module on the __AUX__ extension header.
This folder here contains the KiCAD design files for PCB production.
Please refer to [esp32_mouse_keyboard](https://github.com/asterics/esp32_mouse_keyboard) repository for the corresponding source code.

This design was done in KiCAD __v5.1.2__. Please use a version equal or newer.

__Note:__ If you are looking for the first revision of this board, open this [GIT tag](https://github.com/asterics/FLipMouse/tree/PCB_v0.3)
