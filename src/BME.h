#ifndef BME_H
#define BME_H

#include <Arduino.h>

#include <Wire.h>
#include <SparkFunBME280.h>

#include "data.h"

class BME
{
private:
    BME280 sensor;
    float calculateASL(SensorData &sensorData);
    float height;
    int oversampling = 1;           // number of measurements taken into account for averging

public:
    BME(float altitude);
    bool connected;
    void printBme();
    float temperature();
    bool start();
    void measure(SensorData &sensorData);
};

#endif