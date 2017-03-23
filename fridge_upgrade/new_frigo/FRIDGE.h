#ifndef FRIDGE_H
#define FRIDGE_H

#include <Arduino.h>



class FRIDGE {
    //properties
  private:
    float currentVoltage;
    float currentTemp;
    //methods or functions
  public:
    FRIDGE();
    ~FRIDGE();
    //    on/off fridge
    void turnOnFridge();
    void turnOffFridge();
    //    temperature
    float getCurrentTemp();
    void setCurrentTemp(int ct);
    float readCurrentTemp();
    float getCurrentVoltage(float v, float r1, float r2);
    int readCurrentTemp();
    //    voltage
    int getCurrentVoltage(float vPow, float r1, float r2);
    void setCurrentVoltage(int cv);
    void emitSound();
};

#endif
