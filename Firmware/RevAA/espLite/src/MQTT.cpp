
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "..\lib\config.h"
#include "..\lib\MQTT.h"


WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

int startMQTT(void){
  wifiConfig();
  client.setServer(mqtt_server, 1883);  
  client.setCallback(mqttCallback);
  mqttReconnect();
  
}

int mqttLoop(void){
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
}


int wifiConfig(void) {
  unsigned long timeAtStart = micros();

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while ( (WiFi.status() != WL_CONNECTED) && ( (micros() - timeAtStart) <= wifiTimeout ) ) {
    delay(500);
    Serial.print(".");
  }

  randomSeed( micros() );

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  return 1;
}

int mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

int mqttReconnect(void) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Set Client ID as defined in config.h
    String clientId = CLIENT_ID;
    // Attempt to connect
    if (client.connect( clientId.c_str(),   MQTT_USERNAME, MQTT_PASSWORD ) ) {

      Serial.println("connected");
      // Once connected, subscribe to all topics:
      client.subscribe( topic_relay_in );
      client.subscribe( topic_relay_out );


    } else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);

    }
  }
}
