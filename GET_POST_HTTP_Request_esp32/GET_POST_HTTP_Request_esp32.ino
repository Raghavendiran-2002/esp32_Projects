#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22 

const char *SSID = "Raghavendiran";
const char *PWD = "apple@5g";

DHT dht(DHTPIN, DHTTYPE);

// Web server running on port 80
WebServer server(80);

StaticJsonDocument<60> jsonDocument;
char buffer[60];

float temperature = 20.13;


void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
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

void create_json(char *tag, float Temp,float Humi, char *unit) {  
  jsonDocument.clear();  
//  jsonDocument["type"] = tag;
  jsonDocument["Temp"] = Temp;
  jsonDocument["Humi"] = Humi;
//  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}

void getTemperature() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.println("Temp : ");
  Serial.println(h);
  create_json("temperature", t,h, "°C");
  server.send(200, "application/json", buffer);
}
 

void setup() {     
  Serial.begin(9600);    
     
     
  connectToWiFi();
  dht.begin();         
  setup_routing();     
  // Initialize Neopixel     
    
}    
       
void loop() {    
  server.handleClient();     
}
