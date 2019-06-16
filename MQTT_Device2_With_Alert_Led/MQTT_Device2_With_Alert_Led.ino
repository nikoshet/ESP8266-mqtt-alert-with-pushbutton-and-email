// #############################
// Author: nikoshet
// Copyright: Copyright 2019, esp8266-mqtt-alert-with-pushbutton-and-email
// License: GNU GPLv3
// Mmaintainer: nikoshet
// ## Email: nnikitarrr@gmail.com
// #############################

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#define LED 2 //LED to turn ON if button is pressed

// MQTT broker
const char* MQTTserver = "broker.hivemq.com";
const char* MQTTtopic = "YOUR_TOPIC/button/status";
const char* MQTTmessage = "button_pressed";
const char* MQTTmessageACK = ""; //zero-byte payload to delete retained message
const boolean retained = true;
WiFiManager wifiManager;
WiFiClient client;
PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  while (! Serial);
  // initialize GPIO 2 as an output
  pinMode(LED, OUTPUT);
  
  initWifi();
  subscribeToMQTTtopic();
  //CheckForMQTTmessage();
  Serial.println("Going back to deep sleep for 10 seconds..");
  // Deep sleep mode
  //ESP.deepSleep(3e6);
}

void loop() {
  //Serial.println("in loop");
  
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  delay(10000);
  ////ESP.deepSleep(10e6);
  //WiFi.forceSleepBegin();
  //WiFi.forceSleepWake();
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();
  
  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("AutoConnectAP");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  //Now connect via laptop/smartphone to the AutoConnectAP access point,type the following IP address: 192.168.4.1
  //and configure your ESP’s WiFi credentials
}

// Subscribe to MQTT topic on HiveMQ broker
void subscribeToMQTTtopic(){
  mqttClient.setServer(MQTTserver, 1883);
  mqttClient.setCallback(callback);

  reconnect();
}

// Try to reconnect to MQTT broker
void reconnect(){
  while (!mqttClient.connected()) {
    Serial.print("\nAttempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("clientName2")) {
      Serial.println("connected");
      Serial.println("Subscribing to topic..");
      // Once connected,resubscribe to topic..
      if (mqttClient.subscribe(MQTTtopic,1)){
        Serial.println("Successfully subscribed");
      }
      else{
        Serial.println("Subscribe failed");
      }
      //Serial.println("closing connection");
      //mqttClient.disconnect();
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds\n");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Callback function to handle received message
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic [");
  Serial.print(topic);
  Serial.print("] \n");

  // Alert via LED button
  if(CheckForCorrectMQTTmessage(payload)==true){
    Serial.print("[");
    for (int i=0;i<length;i++) {
      Serial.print((char)payload[i]);
    }
    Serial.print("] \n");
    // Sending ACK message by publishing zero-byte payload to delete retained message
    if (mqttClient.publish(MQTTtopic,MQTTmessageACK,retained)){
      Serial.println("Publish ACK ok");     
    }
    else {
      Serial.println("Publish ACK failed");
    }
    // Turn LED On/Off 
    for (int i=0 ; i<5 ; i++){
      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("Turning Led ON");
      delay(2000);
      digitalWrite(LED, LOW);   // turn the LED off (LOW is the voltage level)
      Serial.println("Turning Led OFF\n");
      delay(1000);
    }
    Serial.println();
  }
  //Serial.println("closing connection");
  //mqttClient.disconnect();
}

// Check the MQTT message on subscribed topic
boolean CheckForCorrectMQTTmessage(byte* payload){
  Serial.print("Message arrived: ");
  if (strcmp((char *)payload, MQTTmessage) == 0 ) {
    Serial.println("Correct message arrived");
  return true;
  }
  else{
    Serial.println("Wrong message arrived");
    return false;
  }
}
