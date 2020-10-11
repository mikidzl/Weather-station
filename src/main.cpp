#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <WebSocketsClient.h>

#include <FS.h>

#include "BME.h"
#include "NTP.h"

BME bme(55);

data handleData;

String output;

unsigned long epoch = 0;
unsigned long sleepInterval = 180e6;
unsigned long sleepTime = 0;
unsigned long measureTime = 0;

bool connected = false;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

void setup()
{
	Serial.begin(115200);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	delay(3);

	bme.start();

	delay(20);
	bme.measure(handleData.sensorData);
	measureTime = micros();

	webSocket.begin("192.168.0.122", 81, "/");
	

	// event handler
	webSocket.onEvent(webSocketEvent);

	while(epoch == 0)
	{
		webSocket.loop();
		delay(10);

		if(millis() >= 50000)				//wait 50sec for answer from server
			break;
	}


	epoch = epoch - (micros() - measureTime)/u_long(1000000);			//taking into account that measuring was earlier

	handleData.encodeData(output, epoch);
	webSocket.sendTXT(output);
	output.clear();

	webSocket.disconnect();

	if(epoch != 0)
	{	
		SPIFFS.begin();
		File memory = SPIFFS.open("memory.txt", "r");
		String line;
		cout(memory, line);
		memory.read();

	}
	
	Serial.println(micros());
	if((sleepInterval - micros()) > 60e6)
		sleepTime = sleepInterval - micros();
	else
	{
		sleepTime = 60e6;
	}
	
	Serial.printf("Going to sleep for %d microseconds \n", sleepTime);

	ESP.deepSleep(sleepTime);
}

void loop()
{
	
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

	switch (type)
	{
	case WStype_DISCONNECTED:
		Serial.printf("[WSc] Disconnected!\n");
		break;
	case WStype_CONNECTED:
	{
		Serial.printf("[WSc] Connected to url: %s\n", payload);

		// send message to server when Connected
		webSocket.sendTXT("Connected");
	}
	break;
	case WStype_TEXT:
		Serial.printf("[WSc] get text: %s\n", payload);

		epoch = strtoul((const char *) &payload[0], NULL, 10);
		// send message to server
		// webSocket.sendTXT("message here");
		break;
	case WStype_BIN:
		Serial.printf("[WSc] get binary length: %u\n", length);
		hexdump(payload, length);

		// send data to server
		// webSocket.sendBIN(payload, length);
		break;
	case WStype_PING:
		// pong will be send automatically
		Serial.printf("[WSc] get ping\n");
		break;
	case WStype_PONG:
		// answer to a ping we send
		Serial.printf("[WSc] get pong\n");
		break;
	}
}