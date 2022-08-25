# FLipMouse/Pad Arduino Initialisation

**Fabrication Note v1.0, AsTeRICS Foundation**

![FLipMouseLogo](./img/flipmouseLogo.png)

# Scope

FLipMouse and FLipPad use from version 3 onward an __Arduino Nano RP2040 Connect__ platform.

The big advantage over the TeensyLC from version 2, this controller has much more RAM & ROM and already contains the ESP32, which is used for the Bluetooth connectivity.

To fully use the update procedures from the WebGUI, the Arduino needs to be programmed initially:

* Upload a sketch to the RP2040 to act as an ESP32 programmer for _esp-idf_
* Upload the ESP32 bootloader/update code
* Upload FLipMouse/FLipPad firmware
* Upload the Bluetooth firmware

# Preparation

## Material

| Nr.  | Description                        | Source                                                       | Image                                                        |
| ---- | ---------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 1    | Arduino Nano RP2040 Connect        | https://store.arduino.cc/products/arduino-nano-rp2040-connect | ![A new Arduino Nano RP2040 connect](./img/rp2040_unmarked.png) |

__Note: Material will be referenced in square brackets: []__

## Tools

| Nr.  | Description                       | Source                                                       |
| ---- | --------------------------------- | ------------------------------------------------------------ |
| 1    | idf.py                            | Install esp-idf according to: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/ |
| 2    | Arduino                           | Install from: https://www.arduino.cc/en/software             |
| 3    | Arduino RP2040 core               | Install from (or with instructions from): https://github.com/earlephilhower/arduino-pico |
| 4    | ESP32 bootloader code             | Download or clone from: https://github.com/asterics/esp32_addon_bootloader/ |
| 5    | FLipMouse/FLipPad firmware        | Download or clone from either: https://github.com/asterics/flippad or: https://github.com/asterics/flipmouse |
| 6    | Permanent marker (red/blue/green) | DYI store, paper store (possibly any store)                  |

__Note: Tools will be referenced in curly brackets: {}__

<div style="page-break-after: always; break-after: page;"></div>
# Procedure

__Install flasher sketch:__

1. Open the file __esp32_addon_bootloader.ino__ from the code _{4}_
1. Connect the Arduino Nano RP2040 Connect _[1]_, press the white button nearby the USB plug while inserting the USB cable.
1. Select the correct __board__ via `Tools -> Board -> (Raspberry Pi RP2040) -> Arduino Nano RP2040 Connect` and select the correct __port__ via `Tools -> Port`. 
1. Upload the sketch to the board



__Upload the ESP32 code:__

1. Connect __D2/GPIO25__ with GND to put the ESP32 into download mode
2. Flash the firmware _{4}_ by calling following command from the _esp32_addon_bootloader_ folder: `idf.py -b 115200 -p /dev/ttyUSB0 build flash`
3. Verify that everything worked without an error, the red LED should light up after a few seconds.
4. To ensure this step is done before any Arduino is shipped, __mark the white sticker on the Arduino with a permanent marker__ *{6}*.



__Upload the RP2040 firmware - variant 1:__

1. Open the FLipWare sketch from _{5}_ (subfolder FLipWare) with the Arduino IDE

2. Select the correct flash layout in Arduino via `Tools -> Flash Size -> 16MB (Sketch: 15MB, FS: 1MB)`

3. Upload the sketch to the board


__Upload the RP2040 firmware - variant 2 (TODO: implement!):__

1. Open https://flippad.asterics.eu or https://flipmouse.asterics.eu
2. Press __Device Initialisation__ and follow the instructions.



__Upload the ESP32 firmware:__

1. Open https://flippad.asterics.eu or https://flipmouse.asterics.eu
2. Connect to the FLipMouse/FLipPad
3. Open the Tab __General__
4. Press __Overwrite Bluetooth firmware__ and follow the instructions
5. The __blue LED should blink after the procedure is finished__



# Testing

Not available, if procedure is followed, the software is flashed correctly:

* __Steady red__ LED for a flashed bootloader code which is ready for an update
* __Blue blinking__ LED for a flashed Bluetooth firmware

## Documentation

For each produced batch, fill out one document __template_arduino_init_production.ots__ and save it as: `arduino_init_<date>.ods`(e.g.: arduino_init_20221118.ods)
