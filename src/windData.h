#ifndef WINDDATA_H
#define WINDDATA_H

#include <Arduino.h>

class windData
{
private:
    int velocityPIN;

    float velocity;
    float frequency;
    unsigned long samplingTime = 2000;
    unsigned long waitTime = 1000;
    int Azimuth;
    String Direction;
    void measureRPM();
    void convertToVelocity();
public:
    windData(int vpin);
    void measureWind();

    ~windData();
};



#endif 