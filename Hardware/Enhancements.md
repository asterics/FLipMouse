# Possible Enhancements for future revisions

* Use different resistor networks, they are too easy to be swapped
* Remove I2C header, unused and can lead to short-circuits on a daughterboard
* Place I2C pins on AUX header and remove SPI
* Change IR LED to a side-viewer LED (e.g., LTE-302)
* Change IR-FET from BS170 (3V Vgs is around 200mA, near the requirements) to e.g., ZVN4210 (3V Vgs -> ~500mA); but D/S are switched.
* Use RGB LED instead of 3 single LEDs (to be discussed)
* Route I2C pins to pressure sensor, for possible update to a different sensor (cheaper!)
* Place a second pressure sensor footprint for DPS310 (2€ for single pieces!). An adapter for the tube is already available here: https://byteswap.systems:666/git/AT_devices/pressure_sensor
* Change jack plugs to THT types, easier to solder.
* Re-route the program button. Mostly unused and could be more useful on a GPIO (e.g. for enabling the webinterface from BLE addon)
* Overthink FSR soldering, many FSRs were destroyed by soldering...
