#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <OneWire.h> //for Thermister
#include "pitches.h"


#define ANALOG_INPUT  2 // Will be used for voltmeter
#define ANALOG_OUTPUT 5
#define ONE_WIRE_BUS 2  // Data wire is plugged into pin 2 on the Arduino 
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.


OneWire  ds(2);  //temp sensor to pin 2


//aqui definimos variables para el teclado
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float R1 = 100000.0; // resistance of R1 (100K) -see text!
float R2 = 33000.0; // resistance of R2 (33K) - see text!
float temperatureLimit = 10.0;

void setup() {
  Serial.begin(9600);
  pinMode(ANALOG_INPUT, INPUT);
  lcd.begin(16, 2);
  sensors.begin();
}
void loop() {
  float voltage =  getVoltage();
  float temperature = getTemperature();
  attachKeyboard();

  if (voltage < 4.0) {
    turnFridgeOff();
    turnAlarmOn();

  } else {
    Serial.print ("tem is  ");
    Serial.print (temperature);
    if (temperature > temperatureLimit) {             // here is the value we would like to set as a variable
      turnFridgeOn();
    } else {
      turnFridgeOff();
    }
  }

}


void turnFridgeOn() {
  digitalWrite (13, HIGH); //Relay to pin 13
}

void turnFridgeOff() {
  Serial.println ("/n turning fridge off ");
  digitalWrite (13, LOW);
}

float getVoltage() {

  int value = 0;
  float vout = 0.0;
  float currentVoltage = 0.0;

  // read the value at analog input
  value = analogRead(ANALOG_INPUT);
  vout = (value * 5.0) / 1024.0; // see text
  currentVoltage = vout / (R2 / (R1 + R2));

  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(  String.format("%.02f", currentVoltage));

  delay(500);

  Serial.print("INPUT V= ");
  Serial.println(currentVoltage);

  delay (500);
  return currentVoltage;
}

void turnAlarmOn() {
  // notes in the melody:
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.://
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

float getTemperature() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  /********************************************************************/
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");
  /********************************************************************/
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(sensors.getTempCByIndex(0));
  Serial.print("T: ");
  Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?
  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire
  return sensors.getTempCByIndex(0);

}

// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
  */
  return btnNONE;  // when all others fail, return this...
}

void attachKeyboard() {

  lcd.setCursor(9, 0);           // move to the begining of the second line

  lcd.print("c: ");
  lcd.print(temperatureLimit);
  
  lcd_key = read_LCD_buttons();  // read the buttons

  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
      {
        break;
      }
    case btnLEFT:
      {
        break;
      }
    case btnUP:
      {
        temperatureLimit += 1;
        break;
      }
    case btnDOWN:
      {
        temperatureLimit -= 1;
        break;
      }
    case btnSELECT:
      {
        break;
      }
    case btnNONE:
      {
        break;
      }
  }
}





