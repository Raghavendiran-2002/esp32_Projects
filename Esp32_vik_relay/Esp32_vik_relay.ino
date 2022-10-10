#include <ArduinoJson.h>

#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>


#define WIFISSID "Raghavendiran22"
#define PWD "poiuytrewq"

const char *mqtt_server = "192.168.1.2"; //for iPhone wired
//const char *mqtt_server = "172.20.10.6" //for iPhone wireless lan
const char *device_id = "esp32";
const int mqttPort = 1883;
const char* mqttUser = "raghav";
const char* mqttPassword = "4104";

WiFiClient espClient;
PubSubClient client(espClient);

char message_buff[100];

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  char message[length]; 
  int i;
  for (i = 0; i<length; i++) {
    message[i] = ((char)payload[i]);
    }
  message[length] = '\0';  
  
  DynamicJsonDocument doc(128); //32 bytes calculated for worst case without any overhead
                                 //for particular use case using ArduinoJson Assistant
  deserializeJson(doc, message);
  const char* id = doc["id"];
  const char* device_status  = doc["status"];
  if (strcmp(doc["id"], "esp1/relay1") == 0){
    Serial.println("relay1");
    Serial.print(device_status);
//    DynamicJsonDocument responseDoc(1024);
//    responseDoc["id"] = id;
//    responseDoc["status"] = device_status;
//    char responseJson[128];
//    serializeJson(responseDoc, responseJson);
//    strcat(topic, "/response");
//    client.publish(topic,responseJson);
//    if (strcmp(device_status, "ON") == 0){
//      digitalWrite(2, LOW);
//    }
//    else {
//      digitalWrite(2, HIGH);
//    }
  }
  else if (strcmp(id, "esp1/relay2") == 0) {
    Serial.println("relay2");
    Serial.print(device_status);
    }
  else if (strcmp(id, "esp1/relay3") == 0) {
    Serial.println("relay3");
    Serial.print(device_status);
    }
  else if (strcmp(id, "esp1/relay4") == 0) {
    Serial.println("relay4");
    Serial.print(device_status);
    }
  else {
    Serial.println("Requested relay doesn't exist!");
    }   
  

//  if (strcmp(led_control, "esp/led_control") == 0)
//  { 
//    if (msgString == "1"){
//      digitalWrite(2, LOW); // PIN HIGH will switch OFF the relay
//    }
//    if (msgString == "0"){
//      digitalWrite(2, HIGH); // PIN LOW will switch ON the relay
//    }
//  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(device_id, mqttUser, mqttPassword))
    { 
    Serial.println("connected");
    client.publish("esp/welcome", "hello from esp"); 
    client.subscribe("esp1");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  //pinMode (2, OUTPUT);
  WiFi.begin(WIFISSID, PWD);
 
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);
  
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
