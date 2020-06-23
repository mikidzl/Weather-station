#ifndef NTP_H
#define NTP_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define NTP_DEFAULT_LOCAL_PORT 1337

class NTP
{
private:
    WiFiUDP UDP;
    byte packetBuffer[NTP_PACKET_SIZE]; // A buffer to hold incoming and outgoing packets
    const char* poolServerName = "time.nist.gov";
    int ntpInterval = 3600 * 1000; // time between connecting to NTP server in ms
    void sendNTPpacket();
    bool updateTime();
    unsigned long lastUpdate = 0;
    unsigned long epochTime;
    bool isUpdated = false;

public:
    NTP();
    void begin();
    unsigned long getTime();
};

#endif