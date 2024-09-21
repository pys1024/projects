#include <Arduino.h>
#include "config.h"

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

  digitalWrite(led, GPIO_ON);
  delay(500);
  digitalWrite(led, GPIO_OFF);
  delay(500);

  if (digitalRead(TOUCH_PAD)) {
    delay(5);
    if (digitalRead(TOUCH_PAD)) {
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

