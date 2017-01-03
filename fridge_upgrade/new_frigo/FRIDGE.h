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
    void turnOnFridge();
    void turnOffFridge();
    float getCurrentTemp();
    void setCurrentTemp(int ct);
    int readCurrentTemp();
    int getCurrentVoltage();
    void setCurrentVoltage(int cv);
};

#endif
