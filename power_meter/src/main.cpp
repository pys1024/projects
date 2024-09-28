#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "config.h"
#include <ModbusMaster.h>
#include <Nokia_LCD.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>


// instantiate ModbusMaster object
ModbusMaster node;
WiFiClient net;
MQTTClient client(1024);
Nokia_LCD lcd(PIN_CLK /* CLK */, PIN_DIN /* DIN */, PIN_DC /* DC */, PIN_CE /* CE */, PIN_RST /* RST */, PIN_BL /* BL */);
AsyncWebServer server(80);

int fw_update = 0;
String state = STATE_DEFAULT;
bool state_changed = false;
int last_input_val;

void connect() {
  Serial1.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial1.print(".");
    delay(1000);
  }

  Serial1.print("\nconnecting...");
  while (!client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) {
    Serial1.print(".");
    delay(1000);
  }

  Serial1.println("\nconnected!");

  client.subscribe(TOPIC_UPGRADE);
  client.subscribe(TOPIC_PWR_CMD);

  state = STATE_DEFAULT;
  state_changed = true;

  client.publish(TOPIC_IP, WiFi.localIP().toString(), true, 0);
  client.publish(TOPIC_PWR_DISC, PAYLOAD_PWR_DISC, true, 0);
  client.publish(TOPIC_VOL_DISC, PAYLOAD_VOL_DISC, true, 0);
  client.publish(TOPIC_CUR_DISC, PAYLOAD_CUR_DISC, true, 0);
  client.publish(TOPIC_POWER_DISC, PAYLOAD_POWER_DISC, true, 0);
  client.publish(TOPIC_ENERGY_DISC, PAYLOAD_ENERGY_DISC, true, 0);
  client.publish(TOPIC_FACTOR_DISC, PAYLOAD_FACTOR_DISC, true, 0);
  client.publish(TOPIC_TEMP_DISC, PAYLOAD_TEMP_DISC, true, 0);
  client.publish(TOPIC_FREQ_DISC, PAYLOAD_FREQ_DISC, true, 0);
  client.publish(TOPIC_AVAILABLE, AVAILABLE, true, 0);
}

void messageReceived(String &topic, String &payload) {
  Serial1.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  if (topic == TOPIC_UPGRADE) {
    fw_update = 1;
  } else if (topic == TOPIC_PWR_CMD) {
    state = payload;
  }
  state_changed = true;
}

void setup() {
  // init GPIO
  pinMode(PIN_POWER, OUTPUT);
  pinMode(PIN_POWER, OUTPUT_OPEN_DRAIN);
  pinMode(PIN_USER, INPUT);
  pinMode(PIN_USER, INPUT_PULLUP);

  digitalWrite(PIN_POWER, GPIO_DEFAULT);

  Serial.begin(4800);
  Serial.swap();
  Serial1.begin(115200);
  node.begin(0x01, Serial);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(MQTT_HOST, MQTT_PORT, net);
  client.setWill(TOPIC_AVAILABLE, UNAVAILABLE, true, 0);
  client.onMessage(messageReceived);

  connect();

  // Initialize the screen
  lcd.begin();
  pinMode(PIN_BL, OUTPUT_OPEN_DRAIN);
  // Turn on the backlight
  lcd.setBacklight(BL_OFF);
  // Set the contrast
  lcd.setContrast(0x40);  // Good values are usualy between 40 and 60
  lcd.sendCommand(0x21);  // Tell LCD extended commands follow
  lcd.sendCommand(0xC0);  // Set LCD Vop (Contrast)
  lcd.sendCommand(0x06);  // Set Temp coefficent
  lcd.sendCommand(0x13);  // LCD bias mode 1:48 (try 0x13)
  // We must send 0x20 before modifying the display control mode
  lcd.sendCommand(0x20);
  lcd.sendCommand(0x0C);  // Set display control, normal mode.
  // Clear the screen by filling it with white pixels
  lcd.clear(false);

}

void loop() {
  static uint32_t meter_counter = 0, bl_counter = 0;
  uint8_t result, idx;
  String data;

  if (fw_update) {
    if (fw_update == 1) {
      AsyncElegantOTA.begin(&server); // Start AsyncElegantOTA
      server.begin();
      Serial1.println("HTTP server started");
      fw_update++;
    }
    return;
  }

  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  int val = digitalRead(PIN_USER);
  if (!state_changed && last_input_val && !val) {
    state_changed = true;
    state = state == STATE_ON ? STATE_OFF : STATE_ON;
  }
  last_input_val = val;

  if (state_changed) {
    if (state == STATE_ON) {
      digitalWrite(PIN_POWER, GPIO_ON);
    } else {
      digitalWrite(PIN_POWER, GPIO_OFF);
    }
    client.publish(TOPIC_PWR_STATE, state, true, 0);

    state_changed = false;

    bl_counter = 1;
    lcd.setBacklight(BL_ON);
  }

  if (bl_counter > 0 && bl_counter++ > 3000) {
    bl_counter = 0;
    lcd.setBacklight(BL_OFF);
  }

  if (meter_counter++ > 500) {
    meter_counter = 0;
    result = node.readHoldingRegisters(IM1281B_REG_START, IM1281B_REG_NUM);
    Serial1.println("Reading data from IM1281B...");
    if (result == node.ku8MBSuccess) {
      Serial1.println("Read data from IM1281B successfully!");
      lcd.clear();
      idx = 0;
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 1000 / 10.0); // voltage: 0.0001V
      Serial1.println("Voltage: " + data);
      lcd.println(("U: " + data + "V").c_str());
      client.publish(TOPIC_VOL_STATE, data, true, 0);
      idx += 2;
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 1000 / 10.0); // current: 0.0001A
      Serial1.println("Current: " + data);
      lcd.println(("I: " + data + "A").c_str());
      client.publish(TOPIC_CUR_STATE, data, true, 0);
      idx += 2;
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 1000 / 10.0); // power: 0.0001W
      Serial1.println("power: " + data);
      lcd.println(("P: " + data + "W").c_str());
      client.publish(TOPIC_POWER_STATE, data, true, 0);
      idx += 2;
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 1000 / 10.0); // energy: 0.0001KWh
      Serial1.println("energy: " + data);
      lcd.println(("E: " + data + "KWh").c_str());
      client.publish(TOPIC_ENERGY_STATE, data, true, 0);
      idx += 2;
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 1000.0); // power factor: 0.001
      Serial1.println("power factor: " + data);
      lcd.println(("M: " + data).c_str());
      client.publish(TOPIC_FACTOR_STATE, data, true, 0);
      idx += 4;                                                                               // skip co2
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 100.0); // temp: 0.01C
      Serial1.println("temp: " + data);
      client.publish(TOPIC_TEMP_STATE, data, true, 0);
      idx += 2;
      data = String(((node.getResponseBuffer(idx) << 16) | node.getResponseBuffer(idx + 1)) / 100.0); // frequency: 0.01Hz
      Serial1.println("freq: " + data);
      lcd.println(("F: " + data + "Hz").c_str());
      client.publish(TOPIC_FREQ_STATE, data, true, 0);
    }
  }

}
