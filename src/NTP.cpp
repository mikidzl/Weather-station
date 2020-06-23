#include "NTP.h"

NTP::NTP()
{
}

void NTP::begin()
{
    UDP.begin(NTP_DEFAULT_LOCAL_PORT);
    do
    {
        isUpdated = updateTime();
    } while (!isUpdated);
}

void NTP::sendNTPpacket()
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    UDP.beginPacket(poolServerName, 123); //NTP requests are to port 123
    UDP.write(packetBuffer, NTP_PACKET_SIZE);
    UDP.endPacket();
}

bool NTP::updateTime()
{
    Serial.println("NTP request");
    sendNTPpacket();

    // Wait till data is there or timeout...
    byte timeout = 0;
    int cb = 0;
    do
    {
        delay(10);
        cb = UDP.parsePacket();
        if (timeout > 100)
            return false; // timeout after 1000 ms
        timeout++;
    } while (cb == 0);

    lastUpdate = millis() - (10 * (timeout + 1)); // Account for delay in reading the time

    UDP.read(this->packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(this->packetBuffer[40], this->packetBuffer[41]);
    unsigned long lowWord = word(this->packetBuffer[42], this->packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    epochTime = secsSince1900 - SEVENZYYEARS;

    Serial.println("Got time");

    return true;
}

unsigned long NTP::getTime()
{
    if (millis() - lastUpdate >= 1800000)
    {
        isUpdated = updateTime();
    }
    return epochTime + (millis() - lastUpdate) / 1000;
}