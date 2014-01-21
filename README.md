PongSat
=======

PongSat in a project originally designed for [TinyDuino][] in order to fit whole electronic part in ping pong ball size sphere(40mm diameter) to be able to store at least 4h of data on LiPo battery. You can run it on any Arduino-based boards after small changes in pin configuration.

PongSat uses [Barometric Pressure Sensor - MPL115A1][sensor] to measure pressure and temperature. The measurements are saved on SD-card in DATALOG.TXT file. 

Settings are stored in CONFIG.TXT file on SD-card. This mechanism allows us to configure device's behaviour in very convenient way. 

###Hardware used in original project:
* [Barometric Pressure Sensor - MPL115A1 Breakout][sensor]
* [TinyDuino Processor Board](http://tiny-circuits.com/shop/tinyduino-processor-board/)
* [TinyShield USB & ICP](http://tiny-circuits.com/shop/usb_icp_tinyshield/)
* [Polymer Lithium Ion Battery - 110mAh](https://www.sparkfun.com/products/731)
* [TinyShield microSD](http://tiny-circuits.com/shop/tinyshield-microsd/)
* [TinyShield Proto Board 1](http://tiny-circuits.com/shop/tinyshield-proto-board-1/)
* [TinyLily Push-button 3mm](http://tiny-circuits.com/shop/tinylily-switch-3mm/)
* [TinyLily LED 1206](http://tiny-circuits.com/shop/tinylily-led-120/)

This is a site containing very useful info about LiPo batteries - [link](http://www.rchelicopterfun.com/rc-lipo-batteries.html) Remember not to over-discharge the battery it's voltage shouldn't fall under 3V. 

Push-button is used to force saving of all data and stopping measurement.

The diode communicates the state of the device:
* every measurement diode do short blink,
* when the data are saved to SD-card diode blinks twice,
* 4 slow blink - break -...  at the begging means that Arduino cannot initiate SD-card
* after pressing push-button diode blinks constantly. 


###Hardware connections:

[Barometric Pressure Sensor - MPL115A1][sensor] hardware pin-configuration:

	SDN ------------------- D9
	CSN ------------------- D3 
	SDO ------------------- D12 *
	SDI ------------------- D11 *
	SCK ------------------- D13 *
	GND ------------------- GND
	VDD ------------------- VCC 
*SPI - you cannot change them

Pin D9 has additional diode attached. 
Push-button is connected to pin D2 with [pull-up resistor](https://en.wikipedia.org/wiki/Pull-up_resistor). The pin can be changed, but you will need to ensure that the new one can be used as interrupt.

###Configuration
The parameters of measurement can be set in CONFIG.TXT file. Settings include:
* Approximate period between measurements specified in seconds - e.g. <b>mP = 5</b>
* whether measure pressure [0 or 1] - <b>iPM = 1</b>
* whether measure temperature [0 or 1] - <b>iTM = 1</b>
* how often write the results to SD card [0-100, 0-every measurement, 100- every 100 measurement] - <b>SDWP = 5</b>
* datalog comment included at the beginning of every set of measurements (keep it short, long strings may overload arduino's memory) - <b>dC = "14-01-18, 17:06, Krakow"</b>

Each key must be typed exactly like in examples above (case sensitive) and these are all currently parsed settings (mP, iPM, iTM, SDWP, dC). Each of them must be followed by "=". Each ones' value should follow specified convention:
* iPM, iTM - boolean value 0 or 1,
* mP, SDWP - integer; mP range = [0 - 37767], SDWP range = [0-100],
* dC - string within quotation marks e.g. "keep it short"

//The CONFIG.TXT file may include comments preceded by shlash like this line

Writing results to SD-file is energy consuming, so you can specify SDWP - the amount of measurements stored in Arduino memory before writing them to DATALOG.TXT file (not more than 100). The data stored in Arduino memory will be lost after power down or reset, but data in SD-card will be safe in such cases. The push-button has interrupt attached - it can force during Arduino sleeping time saving all remaining data to SD-card and stops measurement. 

The time between each measurement (mP) also can be set (up to maximum ATMega integer range - 37767). Actual time may float - it depends on battery voltage. Between measurement Arduino is in sleep mode in order to save energy. 


###Data interpretation

Data saved in DATALOG.TXT need to be processed. The [MPL115A1][sensor] sensor returns two integer values representing pressure and temperature measurements. You should use following formula in order to convert this values. See [MMPL115A1 documentation](https://www.sparkfun.com/datasheets/Sensors/Pressure/MPL115A1.pdf)

#####Pressure
Sensor range is is 50 to 115kPa and its output value varies from 0 to 1023. So to calculate pressure in kPa use following equation:
* pressure_kPa = (65.0/1024.0)*pressure_int + 50

#####Temperature
Temperature resolution is -5.35 counts/Celcius and 472 ADC value is 25 Celcius, so:
* temperature_celcius = 25 + (temperature_int - 472) / -5.35


###Used open-source code:
Part of this project was make on the basis of the following work: 
* https://github.com/JurgenG/ArduinoSettings
* http://robofun.ro/docs/ArdMPL115A1.pde
* [Arduino DataLogger Example](http://arduino.cc/en/Tutorial/YunDatalogger)
* [watch dog1](http://donalmorrissey.blogspot.com/2010/04/sleeping-arduino-part-5-wake-up-via.html)
* [watch dog2](http://citizen-sensing.org/2013/07/arduino-watchdog/)


[sensor]: https://www.sparkfun.com/products/9721
[TinyDuino]: https://tiny-circuits.com/