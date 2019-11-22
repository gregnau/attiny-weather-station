// replay the value b on tx-pin
void sendBit(byte b) {
  if (b == 0) {               // LOW
    PORTB = 1 << PB1;
    delayMicroseconds(SHORT);
    PORTB = 0 << PB1;
  }
  else if (b == 1) {          // HIGH
    PORTB = 1 << PB1;
    delayMicroseconds(LONG);
    PORTB = 0 << PB1;
  }
  delayMicroseconds(SEP);
}

// SYNC signal
void sendSync() {
  PORTB = 1 << PB1;
  delayMicroseconds(SYNC);
  PORTB = 0 << PB1;
}

// send the message by bits
void sendMessage(byte repeats) {
  noInterrupts();

  for (repeats; repeats > 0; repeats--) {
    // begin with a separator
    PORTB = 0 << PB1;
    delayMicroseconds(SEP);
    
    for (byte i = 0; i < 36; i++) {
      sendBit(message[i]);
    }
    sendSync();    // send the SYNC signal at the end
  }

  interrupts();
  ledBlink(2);
}
