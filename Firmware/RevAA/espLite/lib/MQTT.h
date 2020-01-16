#pragma once

//-----------------------------------

#define wifiTimeout     5000        // Time to attempt to reconnect to WiFi

//-----------------------------------

int startMQTT(void);

int mqttLoop(void);

int wifiConfig(void);

int mqttCallback(char* topic, byte* payload, unsigned int length);

int mqttReconnect(void);