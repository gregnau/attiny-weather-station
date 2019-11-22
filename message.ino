// Function used to set constant message bits at start
void initMessage() {
  #if not defined (ID)
    randomSeed(analogRead(A3)); // generate random ID on every restart...
    const int ID = random(1280,1535); // ...if there is no defined ID
  #endif
  
  // write to message array
  convertBit(ID, 0);
  convertBit(CHAN-1, 14);
}

// helper function to update message data
void updateMessage(byte mode) {
  checkBattery();
  message[13] = mode;  // 0:auto, 1:forced
  updateDHT();
}

// Convert data values into bits and fill to message array
void convertBit(int decValue, byte binIndex) {
  int data_length;

  switch (binIndex) {
    case 0:  // ID
      data_length = 12;
      break;
    case 12:  // BAT
      data_length = 1;
      break;
    case 13:  //TXMOD
      data_length = 1;
      break;
    case 14:  // CH
      data_length = 2;
      break;
    case 16:  // TEMP
      data_length = 12;
      break;
    case 28:  // HUM
      data_length = 8;
      break;
  }

  for (data_length -= 1; data_length>=0; data_length--) {
    int binValue = decValue >> data_length;
      if (binValue & 1) {
        message[binIndex] = 1;
        binIndex += 1;
      }
      else {
        message[binIndex] = 0;
        binIndex += 1;
      }
  }
}

// read and convert TEMP and HUM to bits and fill to message array
void updateDHT() {
  int tempValue,humValue;
  int chk = DHT.read22(0);
  
  while (chk != DHTLIB_OK) {
    delay(2200);
    chk = DHT.read22(0);
  }
  tempValue = DHT.temperature * 10;
  humValue = DHT.humidity;

  // minus grades
  if (tempValue < 0) tempValue += 1023;

  // convert values into message array
  convertBit(tempValue, 16);
  convertBit(humValue, 28);
}

// function to measure voltage
int readVcc() {
  ADMUX = _BV(MUX3) | _BV(MUX2);  // set reference to vcc and measurement to int. 1.1v reference.
  delay(2); // wait for vref to settle
  ADCSRA |= _BV(ADSC); // start conversion
  while (bit_is_set(ADCSRA,ADSC)); // wait here while measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long vcc_reading = (high<<8) | low;
  int result = round(scale_constant / vcc_reading); // calculate VCC (in mV)
  return result;
}

void checkBattery() {
  int voltage = readVcc();
  
  if (voltage < 370) message[12] = 0;  // low
  else message[12] = 1;  // charged
}

// Improve accuracy of VCC readings with calibration
void calibration() {  //... if VCC1 is available
  #if defined (VCC1)
    unsigned int readings = 0;
    
    for (byte i=0;i<10;i++) {  // get 10 initial readings
      readings += readVcc();
      delay(25);  // gives more stable results
    }

    int vcc2 = readings / 10;  // calculate the average

    // ###### ###### could check here those readings later...
    int refvolt = 110L*VCC1/vcc2;
    scale_constant = refvolt * 1024L;
  #endif
}
