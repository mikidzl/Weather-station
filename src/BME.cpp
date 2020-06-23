#include "BME.h"

BME::BME(float altitude)
{
    height = altitude;
}

bool BME::start()
{
    Wire.begin();
    Wire.setClock(400000);

    sensor.setI2CAddress(0x76);
    if (sensor.beginI2C())
        connected = true;
    else
    {
        connected = false;
    }

    sensor.setMode(MODE_FORCED);

    return connected;
}

float BME::calculateASL(SensorData &sensorData)
{
    return sensorData.pressure + 11.3 * height;
}

void BME::measure(SensorData &sensorData)
{
    sensor.setMode(MODE_FORCED);

    sensorData.temperature = sensor.readTempC();
    sensorData.pressure = sensor.readFloatPressure();
    sensorData.pressureAsl = 0;
    sensorData.humidity = sensor.readFloatHumidity();
    delay(50);

    for (int i = 0; i < oversampling - 1; i++)
    {
        sensor.setMode(MODE_FORCED);
        sensorData.temperature += sensor.readTempC();
        sensorData.pressure += sensor.readFloatPressure();
        sensorData.humidity += sensor.readFloatHumidity();
        delay(10);
    }

    sensorData.temperature = sensorData.temperature / float(oversampling);
    sensorData.pressure = sensorData.pressure / float(oversampling);
    sensorData.pressureAsl = calculateASL(sensorData);
    sensorData.humidity = sensorData.humidity / float(oversampling);
}