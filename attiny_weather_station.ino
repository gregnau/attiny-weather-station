/* ATtiny Weather Station v3.2
 * with Alecto WS1700 protocol.
 * 
 * Tiny,low power device designed to collect and send
 * environmental temperature and relative humidity.
 * Tested to work nice with ATtiny85, but should be ok
 * on other tiny variants.
 *
 * http://github.com/gregnau/attiny_weather_station
 *
 * GregNau      2016
 */

// SETTINGS
#define INTVAL 60  // frequency of sending the data in seconds.
#define CHAN 1  // set channel bit (0=CH1,1=CH2,2=CH3, )
#define REP 7  // signal repeats (default=7x)
//#define ID 1318  // device id (1280-1535) [when disabled, random id on start]

// sleep function libraries
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

// initialize DHT sensor library
#include <dht.h>
dht DHT;

// timing values of the signal
const unsigned short PULSE_LENGTH = 272;
const unsigned short LONG = PULSE_LENGTH*14;
const unsigned short SHORT = PULSE_LENGTH*7;
const unsigned short SEP = PULSE_LENGTH*2;
const unsigned short SYNC = PULSE_LENGTH*34;

// variables of sleep code
volatile bool watchdogActive = false;
byte sleepCycles = 0;

// buffer to store message ready-to-send by bits
byte message[36];

long scale_constant = 112640L; // default scale_constant (1.1*1024*100)

// define watchdog timer interrupt action
ISR(WDT_vect) {
  watchdogActive = true;  // set flag
}

void setup() {
  //OSCCAL = 0x4E;  // internal oscillator calibration value (important! see manual...)
  
  pinMode(2, OUTPUT);  // STATUS LED
  pinMode(1, OUTPUT);  // RF TRANSMITTER
  pinMode(0, INPUT);  // DHT SENSOR

  calibration(); // calibrate battery readings if VCC1 present

  initMessage();  // set constant message bits (id,ch)
  updateMessage(1);  // refresh message data (bat,txmode,temp,hum)
  sendMessage(REP);  // send the message on rf transmitter for 'REP' times

  setup_watchdog();  // set 8s watchdog timer interrupt
}

void loop() {
  if (watchdogActive) {  // if there was a watchdog wakeup
    watchdogActive = false;  // reset the flag
    sleepCycles += 1;

    if (sleepCycles >= INTVAL/8) {
      updateMessage(0);
      sendMessage(REP);

      sleepCycles = 0;  // reset counter
    }
  }

  sleep();  // go to sleep!
}


// A very simple led blinking function for feedback
void ledBlink(byte n) {
  for (n; n>0; n--) {
    digitalWrite(2, HIGH);
    delay(80);
    digitalWrite(2, LOW);
    delay(80);
  }
}
