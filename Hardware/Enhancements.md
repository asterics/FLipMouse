# Possible Enhancements for future revisions

* Use different resistor networks, they are too easy to be swapped
* Remove I2C header, unused and can lead to short-circuits on a daughterboard
* Place I2C pins on AUX header and remove SPI
* Change IR LED (and possibly the FET too) to a side-viewer LED (e.g., LTE-302)
* Use RGB LED instead of 3 single LEDs (to be discussed)
* Route I2C pins to pressure sensor, for possible update to a different sensor (cheaper!)
* Change jack plugs to THT types, easier to solder.
* Re-route the program button. Mostly unused and could be more useful on a GPIO (e.g. for enabling the webinterface from BLE addon)
* Overthink FSR soldering, many FSRs were destroyed by soldering...