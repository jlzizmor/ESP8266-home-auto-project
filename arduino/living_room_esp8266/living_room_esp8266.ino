/*
 * Script for the ESP8266 attached to the living room switches.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* topic = "\lights\living_room";

const char* ssid = "FiOS-S07RJ";
const char* password = "airs03erik4644mute";
const char* mqtt_server = "192.168.1.151";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outTopic, "Sonoff1 booted");
      // ... and resubscribe
      //client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

char* byteAtoCharA(byte* payload, unsigned int length) {
  char output[length];

  for (int i = 0; i < length; i++) {
    output[i] = (char)payload[i];
  }

  return output;
}

void callback(char* feedTopic, byte* payload, unsigned int length) {
  if (strcmp(feedTopic, topic)) {
    Serial.print(byteAtoCharA(payload, length));
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.begin(115200);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
