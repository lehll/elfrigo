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


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float R1 = 100000.0; // resistance of R1 (100K) -see text!
float R2 = 33000.0; // resistance of R2 (33K) - see text!


void setup() {
  Serial.begin(9600);
  pinMode(ANALOG_INPUT, INPUT);
  lcd.begin(16, 2);
  sensors.begin(); 
}
void loop() {
  float voltage =  getVoltage();
  float temperature = getTemperature();

  if (voltage < 4.0) {
    turnFridgeOff();
    turnAlarmOn();
    
  } else {
    Serial.print ("tem is  ");
    Serial.print (temperature);
    if (temperature > 10){              // here is the value we would like to set as a variable
      turnFridgeOn();
      } else{
        turnFridgeOff();
        }
    }

}


void turnFridgeOn(){
  digitalWrite (13, HIGH); //Relay to pin 13
}

void turnFridgeOff(){
  Serial.println ("/n turning fridge off ");
  digitalWrite (13,LOW);
  
  
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
  lcd.print("Battery= ");
  lcd.print(currentVoltage);

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


  
