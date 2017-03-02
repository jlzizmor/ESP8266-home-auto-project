#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Bounce2.h>
#include <EEPROM.h>

#define LIVING_ROOM_LAMPS // Controlled object

#define PWM_PIN 2
#define ALTERNATE_BUTTON 0
#define EEPROM_ADDR 0
#define PWM_DELAY_TIME 100


#define PWM_ON 100
#define PWM_OFF 0
#define PWM_NEUTRAL 50

#define ON 1
#define OFF 0

const char* all_topic = "\lights\all";
char* room_topic = "";
char* topic = "";
bool serial = false;

const char* ssid = "FiOS-S07RJ";
const char* password = "airs03erik4644mute";
const char* mqtt_server = "YourMQTTBroker'sIP";

#ifdef LIVING_ROOM
topic = "\lights\living_room\lamps";
room_topic = "\lights\living_room\all";
serial = true;
#endif

#ifdef LIVING_ROOM_DOOR
topic = "\lights\living_room\door";
room_topic = "\lights\living_room\all";
serial = true;
#endif

#ifdef LIVING_ROOM_FIXTURE
topic = "\lights\living_room\fixture";
room_topic = "\lights\living_room\all";
serial = true;
#endif

#ifdef KITCHEN_MAIN
#undef PWM_ON
#undef PWM_OFF
#undef PWM_NEUTRAL

#define PWM_ON 100
#define PWM_OFF 0
#define PWM_NEUTRAL 50

topic = "\lights\kitchen\main";
room_topic = "\lights\kitchen\all";
serial = false;
#endif

#ifdef BEDROOM_MAIN
#undef PWM_ON
#undef PWM_OFF
#undef PWM_NEUTRAL


#define PWM_ON 100
#define PWM_OFF 0
#define PWM_NEUTRAL 50


topic = "\lights\bedroom\main";
room_topic = "\lights\bedroom\all";
serial = false;
#endif

#ifdef BEDROOM_CLOSET
#undef PWM_ON
#undef PWM_OFF
#undef PWM_NEUTRAL


#define PWM_ON 100
#define PWM_OFF 0
#define PWM_NEUTRAL 50


topic = "\lights\bedroom\closet";
room_topic = "\lights\bedroom\all";
serial = false;
#endif

#ifdef BATHROOM_MAIN
topic = "\lights\bathroom\main";
room_topic = "\lights\bathroom\all";
serial = true;
#endif

#ifdef BATHROOM_FAN
topic = "\lights\bathroom\fan";
room_topic = "\lights\bathroom\all";
serial = true;
#endif

#ifdef BATHROOM_LAUNDRY
topic = "\lights\bathroom\laundry";
room_topic = "\lights\bathroom\all";
serial = true;
#endif

WiFiClient espClient;
PubSubClient client(espClient);
Bounce debouncer = Bounce();

void extButton();
void setOn(String sw);
void setOff(String sw);
void setup_wifi();
void reconnect();
String reverseString(String input);
String getLowestTopic(char* inputTopic);
void callback(char* feedTopic, byte* payload, unsigned int length);
void setNeutral();

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    extButton();
    for (int i = 0; i < 500; i++) {
      extButton();
      delay(1);
    }
    Serial.print(".");
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

String reverseString(String input) {
  String output = "";

  for (int j = input.length() - 1; j >= 0; j--) {
    output += input.charAt(j);
  }
  return output;
}

String getLowestTopic(char* inputTopic) {
  String input = String(inputTopic);
  String lowest = "";

  for (int i = input.length() - 1; i >= 0; i--) {
    if (input.charAt(i) != '\\') {
      lowest += input.charAt(i);
    }
    else {
      break;
    }
  }

  return reverseString(lowest);
}

void callback(char* feedTopic, byte* payload, unsigned int length) {
  if (strcmp(feedTopic, all_topic) || strcmp(feedTopic, room_topic)) {
    if (payload[0] == ON) {
      setOn("all");
    }
    else {
      setOff("all");
    }
  }
  else if (strcmp(feedTopic, topic)) {
    if (payload[0] == ON) {
      setOn(getLowestTopic(feedTopic));
    }
    else {
      setOff(getLowestTopic(feedTopic));
    }
  }
}

void extButton() {
  if (!(serial)) {
    if (debouncer.fell()) {
      if (EEPROM.read(EEPROM_ADDR) == 1) {
        setOn("");
      }
      else {
        setOff("");
      }
    }
  }
}

void setOn(String sw) {
  if (!(serial)) {
    analogWrite(PWM_PIN, PWM_ON);
    EEPROM.write(EEPROM_ADDR, 1);
    EEPROM.commit();
    delay(PWM_DELAY_TIME);
    setNeutral();
  }
  else {
    Serial.print(sw + "," + "on");
  }
}

void setOff(String sw) {
  if (!(serial)) {
    analogWrite(PWM_PIN, PWM_OFF);
    EEPROM.write(EEPROM_ADDR, 0);
    EEPROM.commit();
    delay(PWM_DELAY_TIME);
    setNeutral();
  }
  else {
    Serial.print(sw + "," + "off");
  }
}

void setNeutral() {
  if (!(serial)) {
    analogWrite(PWM_PIN, PWM_NEUTRAL);
    delay(PWM_DELAY_TIME);
  }
}

void setup() {
  pinMode(ALTERNATE_BUTTON, INPUT_PULLUP);
  debouncer.attach(ALTERNATE_BUTTON);
  debouncer.interval(50);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  EEPROM.begin(512);
  Serial.begin(115200);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  extButton();
}
