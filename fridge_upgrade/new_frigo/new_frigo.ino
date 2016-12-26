
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
float Tmidl = 4;  //ideal T° : 4°C
//Tension test for batterie :
float minBatteryLevel = 11.4;

//Variables de los pines
//OneWire  ds(2);  // on pin 2 (a 4.7K resistor is necessary)
const int voltmeterPin = 1;
const int relayPin = 3; //... output
const int tempSensorPin = 2;
const int buzzPin = 11; //pending
const int led = 12;
float currentBattery, Tmin, Tmax, currentTemp;
//int U1State = 0;
int Plus = 0, Less = 0;


void setup() {
    Serial.begin(9600);      // open the serial port at 9600 bps:    

  // put your setup code here, to run once:
  display.begin(16, 2);
  // Print a message to the LCD.
  display.print("Adventures of the valparaisos!");
}

void loop() {
  // put your main code here, to run repeatedly:
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  display.setCursor(0, 1);
  display.scrollDisplayLeft();
  delay(350);


      Serial.println("No more addresses.?n :"+fridge.readCurrentTemp());

  

  
  

}
