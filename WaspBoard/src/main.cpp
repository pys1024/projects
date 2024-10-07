#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "config.h"

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

WiFiClient net;
MQTTClient client;
String state = STATE_DEFAULT;
String rgb = "255,255,255";
String brightness = "255";
int32_t last_touch_val = 0;
bool state_changed = false;
uint32_t connect_cnt = 0;

uint8_t fw_update = 0;
uint64_t ota_progress_millis = 0;
AsyncWebServer server(80);

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void start_ota() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();
}

void connect(uint8_t retry_times) {
  uint8_t cnt = 0;

  Serial.print("\nchecking wifi...");
  cnt = 0;
  while (WiFi.status() != WL_CONNECTED && cnt < retry_times) {
    Serial.print(".");
    delay(1000);
    cnt++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("\nfailed to connect to WIFI");
    return;
  }

  Serial.print("\nconnecting...");
  cnt = 0;
  while (!client.connected() && !client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS) && cnt < retry_times) {
    Serial.print(".");
    delay(10);
    cnt++;
  }
  if (!client.connected()) {
    Serial.print("\nfailed to connect to MQTT server");
    return;
  }

  Serial.println("\nconnected!");

  client.subscribe(TOPIC_COMMAND);
  client.subscribe(TOPIC_RGB_COMMAND);
  client.subscribe(TOPIC_BRIGHT_COMMAND);
  client.subscribe(TOPIC_DEV_UPDATE);

  // state = STATE_DEFAULT;
  state_changed = true;

  client.publish(TOPIC_DEV_IP, WiFi.localIP().toString(), true, 1);
  client.publish(TOPIC_AVAILABLE, AVAILABLE, true, 1);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("\nincoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  if (topic == TOPIC_DEV_UPDATE) {
    fw_update = 1;
  } else if (topic == TOPIC_COMMAND) {
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

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(MQTT_HOST, MQTT_PORT, net);
  client.setWill(TOPIC_AVAILABLE, UNAVAILABLE, true, 1);
  client.onMessage(messageReceived);

  delay(5000);

  connect(CLIENT_CONNECT_MAX_RETRY_TIMES);
}

void loop() {
  ElegantOTA.loop();
  if (fw_update) {
    if (fw_update == 1) {
      start_ota();
      fw_update++;
    }
    return;
  }

  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  connect_cnt++;
  if (connect_cnt > 6000) { // 60s
    connect_cnt = 0;
    if (!client.connected()) {
      connect(1);
    }
  }

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
      analogWrite(LED_R, rgb.substring(0, i0).toInt());
      analogWrite(LED_G, rgb.substring(i0 + 1, i1).toInt());
      analogWrite(LED_B, rgb.substring(i1 + 1).toInt());
      // analogWrite(LAMP_Y, brightness.toInt());
    } else {
      digitalWrite(LED_R, GPIO_OFF);
      digitalWrite(LED_G, GPIO_OFF);
      digitalWrite(LED_B, GPIO_OFF);
      // digitalWrite(LAMP_Y, GPIO_OFF);
    }

    if (client.connected()) {
      client.publish(TOPIC_STATE, state, true, 1);
      client.publish(TOPIC_RGB_STATE, rgb, true, 1);
      client.publish(TOPIC_BRIGHT_STATE, brightness, true, 1);
    }

    state_changed = false;
  }
}

