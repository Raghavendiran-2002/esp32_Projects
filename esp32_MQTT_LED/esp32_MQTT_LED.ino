
#include <WiFi.h>
#include <PubSubClient.h>
#include<WiFiClient.h>


const char ssid[60] = "IOT";
const char password[60] = "123456789";

const char* mqtt_server = "192.168.43.42";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



// LED Pin
const int ledPin = 5;

void setup() {
  Serial.begin(115200);
  pinMode (ledPin, OUTPUT);
   
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin("IOT","123456789");


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

 
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
 
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
   
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
     
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
     
      delay(5000);
    }
  }
}
void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  char count[10];
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
   
 
    //dtostrf(i,1,0,count);
    Serial.print("LED ON: ");
    digitalWrite (ledPin, HIGH);
    client.publish("esp32/output","Turning ON");
    delay(500);
   
    digitalWrite (ledPin, LOW);
    delay(500);
   
    Serial.print("LED OFF: ");
    client.publish("esp32/output","Turned OFF");

 
   
   }
 
}
