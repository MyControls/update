/*
 * WiFiEndPoint.h
 *
 *  Created on: 26.01.2022
 *      Author: EliteBook HP840
 */
#include "Arduino.h"
/* made by Kusnh13
 For refrence watch YouTube video:-https://youtu.be/Hgq2KX5w-_o   */

#include <ESP8266WiFi.h>


#ifndef WIFIENDPOINT_H_
#define WIFIENDPOINT_H_


class WiFiEndPoint {
public:
	WiFiEndPoint();
	virtual ~WiFiEndPoint();

	void setup();
	void loop();
	void powerOn();
	void powerOff();

	virtual void doSliping();
	virtual void enapleSleeping(bool enabled);

    int getSleepTime();
    void setSleepTime(int sleepTime);

protected:
//	char *ssid = "WLAN-3HNSNE";
//	const char *password = "3553887853781456";
	char *ssid = "MyESP32AP";
	const char *password = "";

	WiFiServer *server;
	String header;
	String xmlStr;
	String strVcc;
	float vcc;
	int sleepTime;
	bool sleep;
	bool sleepingEnabled = false;

	virtual int getPowerPin();
	virtual void loadSensors();
	virtual void generateXML();
	virtual void insertSensorDataToHtml(WiFiClient &request);
	virtual void sendResponse(WiFiClient &request);

	void getCommonData();
	void sendXML(WiFiClient &request);
	void sendHTML(WiFiClient &request);



};

#endif /* WIFIENDPOINT_H_ */
