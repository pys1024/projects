#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "config.h"

WiFiClient net;
MQTTClient client;
String state = STATE_DEFAULT;
String rgb = "0,0,0";
String brightness = "255";
int last_touch_val = 0;
bool state_changed = false;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe(TOPIC_COMMAND);
  client.subscribe(TOPIC_RGB_COMMAND);
  client.subscribe(TOPIC_BRIGHT_COMMAND);

  state = STATE_DEFAULT;
  state_changed = true;

  client.publish(TOPIC_AVAILABLE, AVAILABLE, true, 0);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  if (topic == TOPIC_COMMAND) {
    state = payload;
  } else if (topic == TOPIC_RGB_COMMAND) {
    rgb = payload;
  } else if (topic == TOPIC_BRIGHT_COMMAND) {
    brightness = payload;
  }
  state_changed = true;
}

void setup() {
  // init GPIO
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LAMP_R, OUTPUT);
  pinMode(LAMP_G, OUTPUT);
  pinMode(LAMP_B, OUTPUT);
  pinMode(LAMP_Y, OUTPUT);
  pinMode(POWER_0, INPUT);
  pinMode(POWER_1, INPUT);
  pinMode(TOUCH_PAD, INPUT_PULLUP);

  digitalWrite(LED_G, GPIO_DEFAULT);
  digitalWrite(LED_R, GPIO_DEFAULT);
  digitalWrite(LAMP_R, GPIO_DEFAULT);
  digitalWrite(LAMP_G, GPIO_DEFAULT);
  digitalWrite(LAMP_B, GPIO_DEFAULT);
  digitalWrite(LAMP_Y, GPIO_DEFAULT);

  // Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(MQTT_HOST, MQTT_PORT, net);
  client.setWill(TOPIC_AVAILABLE, UNAVAILABLE, true, 0);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  digitalWrite(LED_G, digitalRead(POWER_0));
  digitalWrite(LED_R, digitalRead(POWER_1));
  int val = digitalRead(TOUCH_PAD);
  if (!state_changed && last_touch_val && !val) {
    state_changed = true;
    state = state == STATE_ON ? STATE_OFF : STATE_ON;
    if (state == STATE_ON) {
      brightness = "255";
    }
  }
  last_touch_val = val;

  if (state_changed) {
    if (state == STATE_ON) {
      int i0 = rgb.indexOf(',');
      int i1 = rgb.lastIndexOf(',');
      analogWrite(LAMP_R, rgb.substring(0, i0).toInt());
      analogWrite(LAMP_G, rgb.substring(i0 + 1, i1).toInt());
      analogWrite(LAMP_B, rgb.substring(i1 + 1).toInt());
      analogWrite(LAMP_Y, brightness.toInt());
    } else {
      digitalWrite(LAMP_R, GPIO_OFF);
      digitalWrite(LAMP_G, GPIO_OFF);
      digitalWrite(LAMP_B, GPIO_OFF);
      digitalWrite(LAMP_Y, GPIO_OFF);
    }
    client.publish(TOPIC_STATE, state, true, 0);
    client.publish(TOPIC_RGB_STATE, rgb, true, 0);
    client.publish(TOPIC_BRIGHT_STATE, brightness, true, 0);

    state_changed = false;
  }
}
