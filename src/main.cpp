#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "config.h"

const char* SSID = MY_WIFI_SSID;
const char* PASSWORD = MY_WIFI_PASSWORD;

AsyncWebServer server(80);

void setup() {
  // Serial ---------------------------------------------------------------
  Serial.begin(115200);

  // FS -------------------------------------------------------------------
  if(!LittleFS.begin()){
    Serial.println("LITTLEFS Mount Failed");
  }
  
  // WiFi -----------------------------------------------------------------
  Serial.print("Connecting to \"" + String(SSID) + "\" : ");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.hostname("esp-srv1");
  Serial.print(WiFi.localIP());
  Serial.println("");

  // Server ---------------------------------------------------------------
  server.on("/b1", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument doc(1024);
    doc["foo"] = "bar";

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/b2", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"heap\": " + String(ESP.getFreeHeap()) + "}";
    request->send(200, "text/plain", json);
  });

  server.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  
  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    }else{
      request->send(404);
    }
  });

  server.begin();
}

void loop() {}