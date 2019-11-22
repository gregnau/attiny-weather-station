void sleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode to full power down, most power-saving
  power_adc_disable();  // Turn off the ADC while asleep.
  sleep_enable();  // Enable sleep and enter sleep mode.
  sleep_mode();  // System sleeps here
  // CPU is now asleep and program execution completely halts!
  // Once awake, execution will resume at this point.
  sleep_disable();    // When awake, disable sleep mode...
  power_all_enable();  // ...and turn on all devices.
}

// set wdt timer to run an interrupt waking the attiny every 8 sec
void setup_watchdog() {
  noInterrupts();  // timing critical part, disable interrupts
  MCUSR &= ~(1<<WDRF);  // set the watchdog reset bit in the MCU status register to 0
  WDTCR |= (1<<WDCE) | (1<<WDE);  // set WDCE and WDE bits in the watchdog control register
  WDTCR = (1<<WDP0) | (1<<WDP3);  // set watchdog clock prescaler bits to a value of 8 seconds
  WDTCR |= (1<<WDIE);  // enable watchdog as interrupt only (no reset)
  interrupts();  // enable interrupts again.
}
