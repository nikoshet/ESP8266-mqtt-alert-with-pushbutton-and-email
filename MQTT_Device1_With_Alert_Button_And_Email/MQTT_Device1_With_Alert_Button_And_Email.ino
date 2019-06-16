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

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/button_pressed/with/key/YOUR_KEY";

// Maker Webhooks IFTTT
const char* IFTTTserver = "maker.ifttt.com";
// MQTT broker
const char* MQTTserver = "broker.hivemq.com";
const char* MQTTtopic = "YOUR_TOPIC/button/status";
const char* MQTTmessage = "button_pressed";
const boolean retained = true;
WiFiManager wifiManager;
WiFiClient client;
PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  while (! Serial);
  initWifi();
  makeIFTTTRequest();
  publishMQTTmessage();
  Serial.println("Going back to deep sleep..");
  // Deep sleep mode until RESET pin is connected to a LOW signal (pushbutton is pressed)
  ESP.deepSleep(0);
}

void loop() {
  // sleeping so wont get here
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


// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("\nConnecting to ");
  Serial.print(IFTTTserver);

  int retries = 5;
  while (!!!client.connect(IFTTTserver, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect\n");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);
  client.print(String("GET ") + resource +
               " HTTP/1.1\r\n" +
               "Host: " + IFTTTserver + "\r\n" +
               "Connection: close\r\n\r\n");

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response\n");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}

// Publish an MQTT message to HiveMQ broker
void publishMQTTmessage(){
  mqttClient.setServer(MQTTserver, 1883);
  
  while (!mqttClient.connected()) {
    Serial.print("\nAttempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("clientName1")) {
      Serial.println("connected");
      Serial.println("Publishing a message..");
      // Once connected, publish a message..
      if (mqttClient.publish(MQTTtopic,MQTTmessage,retained)){
        Serial.println("Publish ok");     
      }
      else {
        Serial.println("Publish failed");
      }
      Serial.println("closing connection");
      mqttClient.disconnect();
      break;
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds\n");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
