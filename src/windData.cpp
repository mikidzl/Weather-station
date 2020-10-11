#include "windData.h"

windData ::windData(int vpin)
{
    velocityPIN = vpin;
}

void windData::measureRPM()
{
    pinMode(velocityPIN, INPUT);
    digitalRead(velocityPIN);
    int impulse = 0;
    unsigned long periodTime = 0;

    unsigned long timer = millis();

    while(!velocityPIN)
    {
        digitalRead(velocityPIN);
        if(millis() - timer >= waitTime)
        {
            frequency = 0;
            return;
        }
    }

    timer = millis();

    while (timer + samplingTime >= millis())
    {   
        digitalRead(velocityPIN);
        if(velocityPIN)
        {   
            periodTime += millis() - timer;
            impulse++;
        }
        while (digitalRead(velocityPIN)){}   
    }

    frequency = (impulse-1)/periodTime/2;
    
}

void windData::convertToVelocity()
{
    k=1;
    velocity = frequency*k;
}