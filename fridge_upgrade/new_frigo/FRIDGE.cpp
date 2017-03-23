#include "FRIDGE.h" //include the declaration for this class
#include <OneWire.h> //for Thermister
#include "pitches.h"

OneWire  ds(2);
//<<constructor>> setup the LED, make pin 13 an OUTPUT
FRIDGE::FRIDGE() {
  Serial.begin(9600);
}

//<<destructor>>
FRIDGE::~FRIDGE() {
  /*nothing to destruct*/
}

void FRIDGE::turnOnFridge() {
  Serial.print("Turning on the FRIDGE !!!");
  digitalWrite(3, HIGH);

};
void FRIDGE::turnOffFridge() {
  Serial.print("Turning off the FRIDGE chao bye :)");
  digitalWrite(3, LOW); //here pin=compres
};

float FRIDGE::getCurrentVoltage(float vPow , float r1, float r2) {
  float v = (analogRead(2) * vPow) / 1024.0;
  float v2 = v / (r2 / (r1 + r2));
  Serial.print("\n Voltage is :" );
  Serial.print((float)(v2), 5);
  return v2;
};

void FRIDGE::setCurrentTemp(int ct) {
  currentTemp = ct;
};

int FRIDGE::getCurrentVoltage(float vPow ,float r1, float r2) {
  float v = (analogRead(2) * vPow) / 1024.0;
  float v2 = v / (r2 / (r1 + r2));
  Serial.print("Voltage is :" );

  Serial.print((float)(v2), 1);


  return v2;
};

void FRIDGE::setCurrentVoltage(int cv) {
  currentVoltage = cv;
};
float FRIDGE::readCurrentTemp(void) {
  byte i;
  byte present = 0;
  byte type_s = -1;
  byte data[12];
  byte addr[8];
  float celsius;

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
  }

  Serial.print("ROM =");
  for ( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
  }
  Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      break;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  //delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.println(" Celsius, ");
  return celsius;
}

void FRIDGE::emitSound() {
  // notes in the melody:
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(11, melody[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(11);
  }
}
//New function by Gabo
boolean superCompressor(float T, int pin, float Tmin, float Tmax, float U0, float U0Lim) {
  //  If current Temp is less than constant  'Tmin'
  if (T < Tmin) {
    //if current voltage is more than constant 'U0Lim'
    if (U0 > U0Lim) {
      //turn on frige
      digitalWrite(pin, HIGH);
    } else {
      //turn off
      digitalWrite(pin, LOW); //here pin=compres
      //    and buzz
    }
  }
  else {
    //do not anything
  }
}



