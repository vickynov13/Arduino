#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"
#define SERVER "ec2-54-199-245-177.ap-northeast-1.compute.amazonaws.com"
#ifndef STASSID
#define STASSID "VEDHA_4G"
#define STAPSK "8220286261"
#endif

const int hlamp = 14;   //D5
const int elamp = 13;   //D7
const int drive = 4;   //D2

void setup() {
    Serial.begin(115200);
    WiFi.begin(STASSID, STAPSK);
    while (WiFi.status() != WL_CONNECTED) {
        analogWrite(hlamp, 160);
        delay(500);
        Serial.print(".");
        analogWrite(hlamp, 0);
    }
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    analogWrite(hlamp, 160);
    analogWrite(elamp, 160);
    HTTPClient http;
    WiFiClient client;
    int a = 0;
    while(a<=0){
        http.begin(client, "http://" SERVER "/resetall");
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK) {
                a=1;
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    analogWrite(elamp, 0);
}

void loop() { 
    if((WiFi.status() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        http.begin(client, "http://" SERVER "/status");

        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                //Serial.println(payload);
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, payload);
                
                if (error) {
                  Serial.print("deserializeJson() failed: ");
                  Serial.println(error.c_str());
                  return;
                }
                const char* id_ = doc["_id_"]; // "MZHw9O7P60kYzYPIhKvzvuwF3MryRSTT"
                long long ts_ = doc["_ts_"]; // 1683738227728
                int runStatus = doc["runStatus"]; // 0
                int rspeed = doc["speed"]; // 0
                int headLamp = doc["headLamp"]; // 0
                int comp1light = doc["comp1light"]; // 0
                int comp2light = doc["comp2light"]; // 0
                int comp3light = doc["comp3light"]; // 0
                int lastCorssedCheck = doc["lastCorssedCheck"]; // 0
                int lastStoppedCheck = doc["lastStoppedCheck"]; // 0
                int nextCheck = doc["nextCheck"]; // 0
                int nextStop = doc["nextStop"]; // 0
                int totalCheck = doc["totalCheck"]; // 0
                if(runStatus > 1) {
                  analogWrite(drive, 255);
                  delay(500);
                }
                Serial.println(headLamp);
                Serial.println(comp1light);
                Serial.println(runStatus);
                Serial.println(rspeed);
                analogWrite(drive, rspeed);
                analogWrite(hlamp, headLamp);
                analogWrite(elamp, comp1light);
                
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    delay(5000);
}
