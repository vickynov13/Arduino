
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial
#include "ArduinoJson.h"

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;


void setup() {

    USE_SERIAL.begin(115200);

      WiFi.mode(WIFI_STA);
      WiFi.begin("VIGNESH_H", "9865467820");
      USE_SERIAL.print("Connecting to WiFi ..");
      while (WiFi.status() != WL_CONNECTED) {
        USE_SERIAL.print('.');
        delay(1000);
      }
      USE_SERIAL.print(WiFi.localIP());
  
  ledcAttachPin(12, ledChannel);
  ledcSetup(ledChannel, freq, resolution);

}

void loop() {
    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        http.begin("http://chuchuchika.servehttp.com:5000/status"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                //USE_SERIAL.println(payload);
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
                int speed = doc["speed"]; // 0
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
                  USE_SERIAL.println(speed);
                  ledcWrite(0,255);
                  delay(500);
                }
                USE_SERIAL.println(speed);
                ledcWrite(0,speed);
                
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    delay(5000);
}
