# ATtiny Weather Station
Small and accurate device to collect environmental temperature and relative humidity which is sent through RF 433/315mhz. Transmission uses the protocol of [Alecto WS-1700](http://www.alecto.nl/weerstation-1700) weather stations, since that is recognized by a great domotica solution: [Pilight](http://www.pilight.org). Compared to [my other weather station](http://www.github.com/gregnau/arduino_weather_station) running on Arduino Nano (328P) it is a power saving device: consuming only a few microamps!
Therefore it can run off a single 9v battery for months, maybe years. Everything is detailed below to build yourself one, but in case you have questions or issues, i'm glad to answer/help. The project is still not complete, watch out for any mistakes! 

## Parts-list
 - [**Atmel ATtiny85 microchip**](http://www.atmel.com/images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf) (tested to work with)
 - [**DHT22/AM2302 sensor**](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)
 - [**RF 433 transmitter module (3.3v)**](http://www.dx.com/nl/p/mini-3-3v-rf-transmitter-receiver-module-433mhz-wireless-link-kit-for-arduino-3-3v-mcu-414004#.VrDg8Hko-Uk)
 - [**battery holder 3xAA or AAA (4.5v)**]()
 - [**Tactile SPST switch**](http://www.ebay.com/sch/i.html?_from=R40&_trksid=p2050601.m570.l1313.TR10.TRC2.A0.H0.Xspst+switch+smd.TRS0&_nkw=spst+switch+smd&_sacat=0) (momentary pushbutton)
 - [**few different resistors**](http://www.ebay.com/sch/i.html?_from=R40&_trksid=p2050601.m570.l1313.TR12.TRC2.A0.H0.Xresistor+pack.TRS0&_nkw=resistor+pack&_sacat=0) (see the [Schematic](#schematic))
 - [**a 3mm led**](http://www.ebay.com/sch/i.html?_from=R40&_trksid=p2050601.m570.l1313.TR12.TRC2.A0.H0.Xled+3mm.TRS0&_nkw=led+3mm&_sacat=0)
 - [**some wires**](http://www.ebay.com/sch/i.html?_from=R40&_trksid=p2050601.m570.l1313.TR12.TRC2.A0.H0.Xjumper+wire+arduino.TRS0&_nkw=jumper+wire+arduino&_sacat=0)

Most of these are easily available anywhere online, but some of the links above are pointing directly to products available to buy online.

## Wiring
Please take the time to study properly the wiring below. Also pay attention for the connection points, some places just cable crossings but not connected! If something still not clear you find a [schematic](#schematic) further down.

![ATtiny Weather Station](/wiring.jpg?raw=true "Wiring")

*The length of the antenna is very important for a decent range on the radio. 1/4 wave antenna lengths are 433MHz - 17.28 cm, 315MHz - 23.81 cm. Hint: it can be a piece of wire, not necessary to search an antenna like on the picture above.

## Schematic
Hopefully this clarifies the above picture!

![ATtiny Weather Station](/schematics.jpg?raw=true "Schematics")

## Configuration
There is a settings block on top of the main code file.
It's possible to adjust the signal and transmission properties here.

	// SETTINGS
	#define INTVAL 60  // frequency of sending the data in seconds.
	#define CHAN 1  // set channel bit (0=CH1,1=CH2,2=CH3, )
	#define REP 7  // signal repeats (default=7x)
	#define ID 1318  // device id (1280-1535) [when disabled, random id on start]
Pretty much self-explanatory, though it is not necessary to change any of these for basic operation.

Since version v3 there is a calibration function for the battery. Not calibrated readings of the VCC are very inaccurate, that's why it is recommended to do. Measure the voltage on the VCC pin and multiply the value with 100, then uncomment this line in the code with that value.

	#define VCC1 424  // Battery calibration value

## Compile
Lately i've switched to [Arduino IDE 1.6.5](http://arduino.cc) on every of my computer, so i would suggest to use the same version.
Then get the [ATtiny support](http://code.google.com/archive/p/arduino-tiny/) for the IDE. This one is tested to compile fine.
Installation goes by the instructions in the archive, but there is additional step before it's going to compile. Navigate into the freshly created `~/Arduino/hardware/tiny/avr/` and create a folder named `libraries`. From your Arduino installation folder `../Arduino/hardware/arduino/avr/libraries` copy the `EEPROM` directory to the `~/Arduino/hardware/tiny/avr/libraries`.
The path structure have changed in the IDE 1.6+ and maybe there is also needed to edit the compiler path in the Tiny's `platform.txt`.
Something like this: `compiler.path={runtime.ide.path}/hardware/tools/avr/bin/`

To compile there is [DHTlib v0.1.13 (by Rob Tillaart)](http://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib) needed. It is possible to work with newer versions, but this one is tested to work with.

![Build | Passing](https://img.shields.io/badge/build-passing-brightgreen.svg)

## Troubleshooting
Most common issue is with the timing, caused by the internal oscillator of the ATtinys. These devices stated to be calibrated, but it is advised to do it again yourself. This can be done easily with [TinyTuner](http://ernstc.dk/arduino/tinytuner.html) and a [USB-TTL cable](http://www.ebay.com/sch/i.html?_from=R40&_trksid=p2050601.m570.l1313.TR0.TRC0.H0.TRS0&_nkw=usb+ttl&_sacat=0). For more information please check out the links.

Serial terminal have been removed, because it were causing more trouble than anything. Besides it was just garbage many time, printing unreadable characters. Instead there is the `ledBlink(n)` function, where "`n`" is times the led will blink.


## Reference
More details on the protocol can be found in the [Pilight Wiki - Alecto WS1700 section](http://wiki.pilight.org/doku.php/alecto_ws1700_v7_0#protocol) and at the forum thread from the wiki page.