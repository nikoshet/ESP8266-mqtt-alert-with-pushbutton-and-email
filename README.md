# ESP8266-mqtt-alert-with-pushbutton-and-email

Alert system using the MQTT protocol, ESP8266-01 modules and the IFTTT web service

## Characteristics of the System
```
•   The first device consists of an ESP8266-01 module as well as a push button 
•   The module is in deepSleep mode,and if the button is pressed, the module initializes, sends an HTTP request 
to the IFTTT web service to send an alert email, as well as publishes an MQTT message to the broker to a specific topic, 
and goes to deepSleep mode for minimum power cumsumption
•   The other device consists of an ESP8266-01 module as well as a LED 
•   The module checks every 10 seconds the same MQTT topic subscribed if there is an alert, and if there is, 
it publishes back a message ACK with zero-byte payload and turns on the LED 5 times
•   The MQTT broker used is the HiveMQ
•   Both devices use the WifiManager.h and PubSubClient.h libraries
•   There is a schematic included for the necessary wiring and hardware needed
```

## FYI

### IFTTT 
[here](https://ifttt.com/discover)

### HiveMQ
[here](https://www.hivemq.com/)

For any feedback or questions do not hesitate to contact me.

--- 
### Licence
Copyright © 2019 Nick Nikitas

HiveMQ is under the Apache License 2.0 that can be found [here](https://github.com/hivemq/hivemq-community-edition/blob/master/LICENSE) ©

Pubsubclient.h and Wifimanager.h libraries are under the MIT License that can be found [here](https://github.com/knolleary/pubsubclient/blob/master/LICENSE.txt) © and [here](https://github.com/tzapu/WiFiManager/blob/master/LICENSE) © 

