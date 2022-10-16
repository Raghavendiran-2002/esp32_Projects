#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22 

const char *SSID = "IOT";
const char *PWD = "123456789";

DHT dht(DHTPIN, DHTTYPE);

// Web server running on port 80
WebServer server(80);

StaticJsonDocument<60> jsonDocument;
char buffer[60];

float temperature = 20.13;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
//     if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {  
//    Serial.println("STA Failed to configure");
//  }
  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup_routing() {     
  server.on("/temperature", getTemperature);      
       
  // start server    
  server.begin();    
}

void create_json(char *tag, float value, char *unit) {  
  jsonDocument.clear();  
//  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
//  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}

void getTemperature() {
  float t = dht.readTemperature();
  Serial.println("Get temperature");
  create_json("temperature", t, "°C");
  server.send(200, "application/json", buffer);
}
 

void setup() {     
  Serial.begin(9600);    
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  connectToWiFi();  
 

  dht.begin();         
  setup_routing();     
  // Initialize Neopixel     
    
}    
       
void loop() {    
  server.handleClient();     
}
