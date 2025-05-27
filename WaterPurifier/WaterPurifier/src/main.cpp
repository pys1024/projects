#include <Arduino.h>
#include <Wire.h>
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
#include <ArduinoJson.h>
#include <Ticker.h>

#define WORK_MODE_SHUT 0
#define WORK_MODE_PUMP 1
#define WORK_MODE_IDLE 2
#define WORK_MODE_FILTER 3
#define WORK_MODE_WASH 4

#define GPIO_2_ADDR(port, bit) ((port << 3) | bit)
#define PORT_2_ADDR(port) ((port + 8) << 3)

/**
 * SW1: P0.1
 * SW2: P0.2
 * SW3: P0.3
 * 
 * V1: P1.6
 * V2: P1.5
 * V3: P1.4
 * V4: P1.3
 * V5: P1.2
 * V6: P1.1
 * 
 * PUMP: P3.6
 * BEEP: P2.5
 * 
 */
#define VALVE1 GPIO_2_ADDR(1, 6)
#define VALVE2 GPIO_2_ADDR(1, 5)
#define VALVE3 GPIO_2_ADDR(1, 4)
#define VALVE4 GPIO_2_ADDR(1, 3)
#define VALVE5 GPIO_2_ADDR(1, 2)
#define VALVE6 GPIO_2_ADDR(1, 1)

#define PUMP GPIO_2_ADDR(3, 6)
#define BEEP GPIO_2_ADDR(2, 5)


static const uint8_t TDS_CMD_GET_DATA_CH1[] = {0x55, 0x07, 0x05, 0x01, 0x00, 0x00, 0x00, 0x62};
static const uint8_t TDS_CMD_GET_DATA_CH2[] = {0x55, 0x07, 0x05, 0x02, 0x00, 0x00, 0x00, 0x63};
static char TDS_RSP_BUF[11];

WiFiClient net;
MQTTClient client;
uint32_t loop_cnt = 0;

uint8_t fw_update = 0;
uint64_t ota_progress_millis = 0;
AsyncWebServer server(80);

uint8_t tds_temp_flag = 0;
uint16_t tds_ch1 = 0;
uint16_t tds_ch2 = 0;
uint16_t temp_ch1 = 0;
uint16_t temp_ch2 = 0;
uint8_t work_mode = WORK_MODE_SHUT;

void task_pump();
void task_upload_info();
Ticker ticker1(task_pump, 10, 0, MILLIS);
Ticker ticker2(task_upload_info, 1000, 0, MILLIS);

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
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

#if DEBUG_MODE
  Serial.print("\nchecking wifi...");
#endif
  cnt = 0;
  while (WiFi.status() != WL_CONNECTED && cnt < retry_times) {
#if DEBUG_MODE
    Serial.print(".");
#endif
    delay(1000);
    cnt++;
  }
  if (WiFi.status() != WL_CONNECTED) {
#if DEBUG_MODE
    Serial.print("\nfailed to connect to WIFI");
#endif
    return;
  }

#if DEBUG_MODE
  Serial.print("\nconnecting to MQTT server...");
#endif
  cnt = 0;
  while (!client.connected() && !client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS) && cnt < retry_times) {
#if DEBUG_MODE
    Serial.print(".");
#endif
    delay(10);
    cnt++;
  }
  if (!client.connected()) {
#if DEBUG_MODE
    Serial.print("\nfailed to connect to MQTT server");
#endif
    return;
  }

#if DEBUG_MODE
  Serial.println("\nMQTT server connected!");
#endif

  client.subscribe(TOPIC_DEV_UPDATE);

  client.publish(TOPIC_DEV_IP, WiFi.localIP().toString(), true, 1);
  client.publish(TOPIC_AVAILABLE, AVAILABLE, true, 1);
  client.publish(TOPIC_WATER_DISC, PAYLOAD_WATER_DISC, true, 1);
}

void messageReceived(String &topic, String &payload) {
#if DEBUG_MODE
  Serial.println("\nincoming: " + topic + " - " + payload);
#endif

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  if (topic == TOPIC_DEV_UPDATE) {
    fw_update = 1;
  }
}

uint8_t gpio_ex_set(uint8_t addr, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  return Wire.endTransmission();
}

void switch_work_mode(uint8_t mode) {
  if (work_mode != mode) {
    work_mode = mode;

    switch (mode) {
    case WORK_MODE_SHUT:
      gpio_ex_set(PUMP, 0);

      gpio_ex_set(VALVE1, 0);
      gpio_ex_set(VALVE2, 0);
      gpio_ex_set(VALVE3, 0);
      gpio_ex_set(VALVE4, 0);
      gpio_ex_set(VALVE5, 0);
      gpio_ex_set(VALVE6, 0);
    break;

    case WORK_MODE_IDLE:
      gpio_ex_set(PUMP, 0);

      gpio_ex_set(VALVE1, 0);
      gpio_ex_set(VALVE2, 0);
      gpio_ex_set(VALVE3, 0);
      gpio_ex_set(VALVE4, 0);
      gpio_ex_set(VALVE5, 0);
      gpio_ex_set(VALVE6, 1);
      break;

    case WORK_MODE_PUMP:
      gpio_ex_set(PUMP, 1);

      gpio_ex_set(VALVE1, 1);
      gpio_ex_set(VALVE2, 0);
      gpio_ex_set(VALVE3, 1);
      gpio_ex_set(VALVE4, 0);
      gpio_ex_set(VALVE5, 1);
      gpio_ex_set(VALVE6, 1);
      break;

    case WORK_MODE_FILTER:
      gpio_ex_set(PUMP, 1);

      gpio_ex_set(VALVE1, 1);
      gpio_ex_set(VALVE2, 1);
      gpio_ex_set(VALVE3, 0);
      gpio_ex_set(VALVE4, 0);
      gpio_ex_set(VALVE5, 1);
      gpio_ex_set(VALVE6, 1);
      break;

    case WORK_MODE_WASH:
      gpio_ex_set(PUMP, 1);

      gpio_ex_set(VALVE1, 1);
      gpio_ex_set(VALVE2, 1);
      gpio_ex_set(VALVE3, 0);
      gpio_ex_set(VALVE4, 1);
      gpio_ex_set(VALVE5, 1);
      gpio_ex_set(VALVE6, 1);
      break;
    }
  }
}

void task_upload_info() {
  /* TDS sensor readings */
#if (DEBUG_MODE == 0)
  if (tds_temp_flag == 0) {
    tds_temp_flag = 1;
    Serial.write(&TDS_CMD_GET_DATA_CH1[0], sizeof(TDS_CMD_GET_DATA_CH1));
  } else {
    tds_temp_flag = 0;
    Serial.write(&TDS_CMD_GET_DATA_CH2[0], sizeof(TDS_CMD_GET_DATA_CH2));
  }

  if ((uint32_t)Serial.available() >= sizeof(TDS_RSP_BUF)) {
    Serial.readBytes(&TDS_RSP_BUF[0], sizeof(TDS_RSP_BUF));
    if (TDS_RSP_BUF[0] == 0x55 && TDS_RSP_BUF[1] == 0x0A && TDS_RSP_BUF[2] == 0x85) {
#endif
      uint8_t ch = TDS_RSP_BUF[3];
      uint16_t tds = (TDS_RSP_BUF[4] << 8 | TDS_RSP_BUF[5]);
      uint16_t temp = (TDS_RSP_BUF[6] << 8 | TDS_RSP_BUF[7]);

      if (ch == 1) {
        tds_ch1 = tds;
        temp_ch1 = temp;
      } else if (ch == 2) {
        tds_ch2 = tds;
        temp_ch2 = temp;
      }

      if (client.connected()) {
        JsonDocument doc;
        doc[KEY_TDS1] = tds_ch1/20.0;
        doc[KEY_TDS2] = tds_ch2/20.0;
        doc[KEY_TEMP1] = temp_ch1/10.0;
        doc[KEY_TEMP2] = temp_ch2/10.0;
        doc[KEY_RSSI] = WiFi.RSSI();
  
        // Note: the following publish MUST use QoS 0, otherwise the client may be disconnected.
        client.publish(TOPIC_STATE, doc.as<String>(), true, 0);
      }
    }
#if (DEBUG_MODE == 0)
  }
#endif
}

void task_pump() {
  /**
   * SW1: P0.1
   * SW2: P0.2
   * SW3: P0.3
   * 
   * V1: P1.6
   * V2: P1.5
   * V3: P1.4
   * V4: P1.3
   * V5: P1.2
   * V6: P1.1
   * 
   * PUMP: P3.6
   * BEEP: P2.5
   * 
   */
  /* Water flow control */
  if (Wire.requestFrom(PORT_2_ADDR(0), 1)) {
    int value = Wire.read();
    if (value >= 0) {

      // most high priority
      if (value & (0x01 << 1)) { // SW1: low water pressure reached
        switch_work_mode(WORK_MODE_SHUT);
      } else if (value & (0x01 << 3)) { // SW3: high water pressure reached
        switch_work_mode(WORK_MODE_IDLE);
      } else if (tds_ch1 <= 100) {
        switch_work_mode(WORK_MODE_PUMP);
      } else {
        switch_work_mode(WORK_MODE_FILTER);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  Wire.begin();
  Wire.setClock(5000); // max frequency is 5kHz

  pinMode(FLOW_DETECT, INPUT);

  Serial.println("\nDevice(" DEV_NAME ") is starting...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(MQTT_HOST, MQTT_PORT, net);
  client.setWill(TOPIC_AVAILABLE, UNAVAILABLE, true, 1);
  client.onMessage(messageReceived);

  gpio_ex_set(PUMP, 0);
  gpio_ex_set(BEEP, 0);
  gpio_ex_set(VALVE1, 0);
  gpio_ex_set(VALVE2, 0);
  gpio_ex_set(VALVE3, 0);
  gpio_ex_set(VALVE4, 0);
  gpio_ex_set(VALVE5, 0);
  gpio_ex_set(VALVE6, 0);
  
  connect(CLIENT_CONNECT_MAX_RETRY_TIMES);

  ticker1.start();
  ticker2.start();

#if (DEBUG_MODE == 0)
  Serial.swap();
#endif
}

void loop() {
  ElegantOTA.loop();
  if (fw_update) {
    if (fw_update == 1) {
      Serial.swap();

      if (!client.connected()) {
        connect(10);
      }
      start_ota();
      fw_update++;
    }
    return;
  }

  ticker1.update();
  ticker2.update();
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  loop_cnt++;
  if (loop_cnt >= 6000) { // 60s
    loop_cnt = 0;
    if (!client.connected()) {

      // gpio_ex_set(BEEP, 1);
      // delay(1000);
      // gpio_ex_set(BEEP, 0);
      // turn off pump and try to reconnect

      gpio_ex_set(PUMP, 0);
      connect(1);
    }
  }


}
