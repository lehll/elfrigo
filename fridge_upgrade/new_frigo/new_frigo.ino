
#include <OneWire.h> //for Thermister
//#include <SPI.h>
#include <LiquidCrystal.h>
#include "FRIDGE.h"


//inicializa display
LiquidCrystal display(8, 9, 4, 5, 6, 7);
FRIDGE fridge;

//-----------------Constantes de voltimetro ---------------------
float vPow = 5.0; //voltmeter variables
float r1 = 100000;
float r2 = 10000;

//-----------------Constantes de temperatura y baterias ---------------------------
//Tension test for batterie :
float minBattery = 11.4;

//Variables de los pines
//OneWire  ds(2);  // on pin 2 (a 4.7K resistor is necessary)
const int voltmeterPin = 1;
const int relayPin = 3; //... output
const int tempSensorPin = 2;
const int buzzPin = 11; //pending
const int led = 12;
float currentBattery, currentTemp;
const int Tmin = 4;
const int Tmax = 10;
//int U1State = 0;
int Plus = 0, Less = 0;


void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps:

  // put your setup code here, to run once:
  display.begin(16, 2);
  // Print a message to the LCD.
  //display.print("Adventures of the valparaiso!");
}

void loop() {

  currentTemp = fridge.readCurrentTemp();
  currentBattery = fridge.getCurrentVoltage(vPow, r1, r2);

  if (currentBattery > 13)
    delay(30000);

  if (currentTemp > Tmax) {
    if (currentBattery > minBattery) {
      fridge.turnOnFridge();
    } else {
      fridge.emitSound();
      fridge.turnOffFridge();
    }
  } else {
    fridge.turnOffFridge();
  }
  // put your main code here, to run repeatedly:
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  display.setCursor(0, 0);
  //display.scrollDisplayLeft();
  // print the number of seconds since reset:
  // display.print();
  //display.print(fridge.readCurrentTemp());
  display.print(fridge.readCurrentTemp(), 2);
  display.print(" *C");
  display.setCursor(0, 1);
  display.print(fridge.getCurrentVoltage(vPow, r1, r2), 2);
  display.print(" V");
}


