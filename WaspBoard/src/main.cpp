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
  pinMode(LED_B, OUTPUT);
  pinMode(TOUCH_PAD, INPUT_PULLUP);

  digitalWrite(LED_R, GPIO_DEFAULT);
  digitalWrite(LED_G, GPIO_DEFAULT);
  digitalWrite(LED_B, GPIO_DEFAULT);
}

void loop() {
  static uint8_t led = LED_R;
  static uint8_t bright = 0;

  analogWrite(led, bright);
  delay(1000);
  bright = (bright + 1) % 255;

  if (digitalRead(TOUCH_PAD)) {
    delay(5);
    if (digitalRead(TOUCH_PAD)) {
      digitalWrite(LED_R, GPIO_DEFAULT);
      digitalWrite(LED_G, GPIO_DEFAULT);
      digitalWrite(LED_B, GPIO_DEFAULT);
      switch (led) {
        case LED_R:
        led = LED_G;
        break;
        case LED_G:
        led = LED_B;
        break;
        case LED_B:
        default:
        led = LED_R;
        break;
      }
      while (digitalRead(TOUCH_PAD));
    }

  }

  // digitalWrite(LED_G, GPIO_ON);
  // delay(500);
  // digitalWrite(LED_G, GPIO_OFF);
  // delay(500);

  // digitalWrite(LED_B, GPIO_ON);
  // delay(500);
  // digitalWrite(LED_B, GPIO_OFF);
  // delay(500);
}

