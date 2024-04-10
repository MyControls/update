/*
 * WiFiEndPoint.cpp
 *
 *  Created on: 26.01.2022
 *      Author: EliteBook HP840
 */

#include "WiFiEndPoint.h"

WiFiEndPoint::WiFiEndPoint() {
	server = new WiFiServer(80);
	sleepTime = 120; // 2 min
	sleep = false;
	vcc= 0;
}

WiFiEndPoint::~WiFiEndPoint() {
	// TODO Auto-generated destructor stub
}

void WiFiEndPoint::enapleSleeping(bool enabled) {
    sleepingEnabled = enabled;
}

int WiFiEndPoint::getPowerPin() {
	return 14;
}

void WiFiEndPoint::powerOn(){
    digitalWrite(getPowerPin(), HIGH);
}
void WiFiEndPoint::powerOff(){
    digitalWrite(getPowerPin(), LOW);
}

int WiFiEndPoint::getSleepTime() {
	return sleepTime;
}

void WiFiEndPoint::setSleepTime(int sleepTime) {
	this->sleepTime = sleepTime;
}


void WiFiEndPoint::setup() {
	sleep = false;
	Serial.begin(115200);
	pinMode(getPowerPin(), OUTPUT);

	Serial.begin(115200);
	Serial.println();

	WiFi.mode(WIFI_OFF);
	WiFi.begin(ssid, password);

	Serial.print("Connecting");
	while (WiFi.status() != 3) {
		delay(500);
		Serial.print(".");
	}
	Serial.println();

	//delay(10000);
	Serial.print("Connected, IP address: ");
	Serial.println(WiFi.localIP());
	server->begin();
	Serial.println("ready");

}

void WiFiEndPoint::doSliping() {
    if (sleepingEnabled && sleep) {
	wifi_station_disconnect();
	wifi_set_opmode (NULL_MODE);
	wifi_set_sleep_type (MODEM_SLEEP_T);
	wifi_fpm_open();
	WiFi.disconnect(true);
	delay(5000);
	Serial.print("Sleeping ");
	Serial.print(sleepTime);
	Serial.print(" seconds");
	Serial.println(" ...");
	ESP.deepSleep(sleepTime * 1e6);
	if (sleepTime > 120) {
	    ESP.restart();
	}
    }
}

void WiFiEndPoint::loop() {
	WiFiClient request = server->available();

	if (request) {
		Serial.println("New request.");
		// load common data
		getCommonData();
		// load sensor data
		loadSensors();
		//
		String currentLine = "";


		while (request.connected()) {
			if (request.available()) {
				char c = request.read();
				Serial.write(c);
				header += c;
				if (c == '\n') {
					if (currentLine.length() == 0) {
						sendResponse(request);
						break;
					} else {
						currentLine = "";
					}
				} else if (c != '\r') {
					currentLine += c;
				}
			}

		}
		header = "";
		request.stop();
		Serial.println("Client disconnected.");
		Serial.println("");
		delay(500);
		sleep = true;
	}
	 //doSliping();
}



void WiFiEndPoint::getCommonData() {
	vcc = ((float)ESP.getVcc())/1024;
	strVcc = "Battery: " + String(vcc, 3);
}





void WiFiEndPoint::sendXML(WiFiClient &request) {
	request.println("HTTP/1.1 200 OK");
	request.println("Content-type:text/xml");
	request.println("Connection: close");
	request.println();
	request.println("<!DOCTYPE xml>");
	generateXML();
	request.println(xmlStr);

}




void WiFiEndPoint::sendHTML(WiFiClient &request) {
	request.println("HTTP/1.1 200 OK");
	request.println("Content-type:text/html");
	request.println("Connection: close");
	request.println();
	request.println("<!DOCTYPE html><html>");
	request.println(
			"<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
	request.println(
			"<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
	request.println(
			".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
	request.println(
			"text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
	request.println(".button2 {background-color: #555555;}</style></head>");
	request.println("<body><h1>ESP8266 Wifi Sensor</h1>");
	//
	insertSensorDataToHtml(request);
	//
	request.println("<p>" + strVcc + "</p>");
	request.println("<p></p>");
	//
	request.println("</body>");
	request.println("</html>");
}



//they need to be worked out because the compiler does not come with it
void WiFiEndPoint::loadSensors() {}
void WiFiEndPoint::generateXML() {}
void WiFiEndPoint::insertSensorDataToHtml(WiFiClient &request) {}
void WiFiEndPoint::sendResponse(WiFiClient &request) {}



