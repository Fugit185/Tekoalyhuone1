/*
 ESP code for Timmy robot
 https://raw.githubusercontent.com/RuiSantosdotme/Random-Nerd-Tutorials/master/Projects/ESP32-MQTT/ESP32_MQTT_Publish_Subscribe.ino
 https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED 2 //Define blinking LED pin
//const int sensor = 4;

// Update these with values suitable for your network.
const char* ssid = "AI Rekka";
const char* password = "Omnia2019";
const char* mqtt_server = "192.168.1.31";

//const char* ssid = "Kiihdyttamo1";
//const char* password = "kiihdyttamo1";
//const char* mqtt_server = "192.168.8.130";


//Tarvittavat muuttuja
const char* timmyState;
//const char* timmyTila;

int timmyDonePinIn = 4; //pin 2
int timmyPowerOnPinIn = 5; //pin 1
int timmyPinOut = 16; //pin0
//int timmyDonePinIn = 5; //pin 2
//int timmyPowerOnPinIn = 4; //pin 1

//int timmyInPinValue = 0;
const char* timmyPowerState;
const char* timmyDoneState;


//Luo tarvittavat luokat
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //randomSeed(micros());

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

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "timmy") {
    //Serial.print("Changing output to ");
    if(messageTemp == "start"){
      Serial.println("start");
      digitalWrite(timmyPinOut, HIGH);
      Serial.println(digitalRead(timmyPinOut));
      timmyState = "started";
      Serial.println(timmyState);
      client.publish("timmy", timmyState);
      delay (30000);
      digitalWrite(timmyPinOut, LOW);
      Serial.println(digitalRead(timmyPinOut));
    }
    //else if(messageTemp == "off"){
      //Serial.println("off");
      //digitalWrite(timmyPinOut, LOW);
      //timmyState = "Off";
    //}
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
      client.subscribe("timmy");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(timmyPinOut, OUTPUT);
  digitalWrite(timmyPinOut, LOW);
  pinMode(timmyDonePinIn, INPUT);
  digitalWrite(timmyDonePinIn, LOW);
  pinMode(timmyPowerOnPinIn, INPUT);
  digitalWrite(timmyPowerOnPinIn, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED, OUTPUT); // Initialize the LED pin as an output
  //pinMode(sensor, INPUT_PULLUP);
}

void loop() {
  digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
  delay(10); // Wait for a second
  digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
  delay(10); // Wait for two seconds
  //Serial.println("ledin pitäisi syttyä");
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //timmyInPinValue = digitalRead (timmyInPin);
    //if (timmyState == "started"){
      //Serial.println(timmyState);
      //client.publish("timmy", timmyState);
      //delay(2000);
    //}
    if (digitalRead (timmyPowerOnPinIn)) {
      timmyPowerState = "on";
      client.publish("timmy", timmyPowerState);
      Serial.println("ON");
      //Serial.println(digitalRead (timmyPowerOnPinIn));
      delay(2000);
    }
    if (digitalRead (timmyDonePinIn)) {
      Serial.println(timmyDonePinIn);
      timmyDoneState = "done";
      client.publish("timmy", timmyDoneState);
      Serial.println("done");
      Serial.println(digitalRead (timmyDonePinIn));
      delay(2000);
    }
 
}
