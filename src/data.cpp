#include "data.h"

data::data()
{
}


void data::encodeData(String &dataString, unsigned long epochTime)
{
    StaticJsonDocument<200> doc;

    JsonObject weather = doc.createNestedObject("weather");
    weather["temperature"] = sensorData.temperature;
    weather["pressure"] = sensorData.pressure;
    weather["pressureASL"] = sensorData.pressureAsl;
    weather["humidity"] = sensorData.humidity;

    doc["epochTime"] = epochTime;

    doc["battery"] = "good";

    serializeJson(doc,dataString);
}