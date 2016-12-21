
#include <OneWire.h> //for Thermister
#include <SPI.h>
#include <LiquidCrystal.h>


//initailizes display
LiquidCrystal display(8, 9, 4, 5, 6, 7);

//--------------------------------------- ---------------------
float vPow = 5.0; //voltmeter variables
float r1 = 100000;
float r2 = 10000;


//---------------YOU CAN CHANGE : ON ---------------------------
float Tmidl = 4;  //ideal T° : 4°C
float diffMinMax = 1; //Tmin=Tmoy-diff/2  Tmax=Tmoy+diff/2
float diffLowHigh = 2; //TminLow=TminHigh+diff

//Tension test for batterie :
float U0Lim = 11.4;
//---------------YOU CAN CHANGE : OFF-----------------------


//Variables
//const int U1=1; //to modify with the good input
OneWire  ds(2);  // on pin 2 (a 4.7K resistor is necessary)
const int U0pin = 1;
const int Tpin = 8;
const int compres = 3; //... output
const int pinPlus = 4;
const int pinLess = 5;
const int pinBuzz = 2;
boolean Ttest = false;
float U0, Tmin, Tmax, T, v;
//int U1State = 0;
int Plus = 0, Less = 0;


//-------FUNCTIONS--------------
//------------TO READ THE VOLTAGE----------
float readVolt (int pin, float r1, float r2) {
  float v = (analogRead(pin) * vPow) / 1024.0;
  float v2 = v / (r2 / (r1 + r2));
  return v2;
}
//------------TO DEFINE THE SETTINGS TEMP MAX / MIN
float controler(float Tmidl, int pinPlus, int pinLess, float impl) {
  //We dont know what ze did
  if (!pinPlus) {
    Tmidl = Tmidl + impl;
  }
  if (pinLess) {
    Tmidl = Tmidl - impl;
  }
  return Tmidl;
}
//-----------TO CONTROLE THE RELAY DEPENDING ON THE TEMP
boolean compressor(float T, boolean Ttest, int pin, float Tmin, float Tmax, float U0, float U0Lim) {
  if (Ttest) { // if True : we have been under Tmin : Temp in fridge incresing
    if (T > Tmax) {
      Ttest = false;
    }
  }
  else { //if False : not been under Tmin yet;
    if (T < Tmin) {
      Ttest = true;
      digitalWrite(pin, LOW); //here pin=compres
    } else {
      if (U0 > U0Lim) {
        digitalWrite(pin, HIGH);
      }
      //      else {
      //        tone(pinBuzz, 440, 2000);
      //        }
    }
  }
  return Ttest;
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
//-------------TO READ THE TEMP------------------------------------
float Temperature(void) {
  byte i;
  byte present = 0;
  byte type_s;
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
//--------------------------------PRINT ON LCD TEMP AND V---------
void printVT(float v2, float temp, float Tmidl) {
  /*display.setTextSize(1);
    display.setTextColor(WHITE);*/
  display.setCursor(0, 0);
  display.print(v2, 3);
  display.println(" V");
  display.print(temp, 1);
  display.println(" *C");
  display.print("Tmidl =");
  display.print(Tmidl, 1);
  display.println(" *C");
  display.display();
  //display.clearDisplay();
  //delay(1000);
}
//----------END FUNCTIONS


//---------SETUP------------------
void setup() {
  //pinMode (U1, INPUT);
  pinMode (pinPlus, INPUT);
  pinMode (pinLess, INPUT);
  pinMode (compres, OUTPUT);
  digitalWrite(compres, LOW);

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initializes display gives it address, adafruit logo
  display.display();
  delay(2000);
  // Clear the buffer.
  //display.clearDisplay();
}



//--------------------------------LOOP------------------------------
void loop() {
    Serial.print(" ###### Into Loop ###### ");


  Plus = digitalRead(pinPlus);
  Less = digitalRead(pinLess);
  Tmidl = controler(Tmidl, Plus, Less, 0.1);

  //U1State = digitalRead(U1);
  U0 = readVolt (U0pin, r1, r2);
  T = Temperature();

//  //First test : to know if we use High or Low settings
//  if (U0 >= U0Lim) { //(U1State == HIGH or U0>U0Lim)
//    Tmin = Tmidl - diffMinMax / 2; //min --- Tmidl --- max
//    Tmax = Tmidl + diffMinMax / 2;
//  } else {
//    Tmin = Tmidl - diffMinMax / 2 + diffLowHigh; // tmidl -- tmidlLOW
//    Tmax = Tmidl + diffMinMax / 2 + diffLowHigh; // min --- tmidlLOW --- max
//  }

  //Second Test : to know if we are at the end of a Temperature Loop
//  Ttest = compressor (T, Ttest, compres, Tmin, Tmax, U0, U0Lim);
  superCompressor(T, compres, Tmin, Tmax, U0, U0Lim);
  printVT(U0, T, Tmidl);
}
