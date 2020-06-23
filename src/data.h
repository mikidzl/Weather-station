#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct SensorData
{
    float temperature;
    double pressure;
    double pressureAsl;
    float humidity;
    float dewPoint;
};

struct da{
	
	float windSpeed;
	int windDirection;
	float batteryVolatage;
	unsigned long epochTime;
};

class data
{
private:
    /* data */
public:
    data(/* args */);
    SensorData sensorData;
    void show();
    void encodeData(String &dataString, unsigned long epochTime);
};



#endif