
// Standard Includes
#include <Arduino.h>
#include <stdint.h>

// Library Includes
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Custom Includes
#include "config.h"
#include "..\lib\MQTT.h"



void setup() {
  
  Serial.begin(9600);
  Serial.println("starting...");
  pinMode(12, OUTPUT);

  startMQTT();


}

void loop() {

  mqttLoop();

}