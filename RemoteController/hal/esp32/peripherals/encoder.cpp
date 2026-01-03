#include "common.h"
#include <ESP_Knob.h>

// Versatile_RotaryEncoder *encoder = new Versatile_RotaryEncoder(EC2_CLK_PIN, EC2_DT_PIN, 0);
ESP_Knob *knob1 = new ESP_Knob(EC1_CLK_PIN, EC1_DT_PIN);
ESP_Knob *knob2 = new ESP_Knob(EC2_CLK_PIN, EC2_DT_PIN);
char str[128];

 void onKnobLeftEventCallback(int count, void *usr_data)
{
  // Serial.printf("Detect left event, count is %d\n", count);
  snprintf(str, sizeof(str), "Left Event: %d", count);
}

void onKnobRightEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect right event, count is %d\n", count);
    snprintf(str, sizeof(str), "Right Event: %d", count);
}

void onKnobHighLimitEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect high limit event, count is %d\n", count);
    snprintf(str, sizeof(str), "High Limit Event: %d", count);
}

void onKnobLowLimitEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect low limit event, count is %d\n", count);
    snprintf(str, sizeof(str), "Low Limit Event: %d", count);
}

void onKnobZeroEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect zero event, count is %d\n", count);
    snprintf(str, sizeof(str), "Zero Event: %d", count);
}

bool hal_encoder_init(void)
{
  knob1->begin();
  knob1->attachLeftEventCallback(onKnobLeftEventCallback);
  knob1->attachRightEventCallback(onKnobRightEventCallback);
  knob1->attachHighLimitEventCallback(onKnobHighLimitEventCallback);
  knob1->attachLowLimitEventCallback(onKnobLowLimitEventCallback);
  knob1->attachZeroEventCallback(onKnobZeroEventCallback);

  knob2->begin();
  knob2->attachLeftEventCallback(onKnobLeftEventCallback);
  knob2->attachRightEventCallback(onKnobRightEventCallback);
  knob2->attachHighLimitEventCallback(onKnobHighLimitEventCallback);
  knob2->attachLowLimitEventCallback(onKnobLowLimitEventCallback);
  knob2->attachZeroEventCallback(onKnobZeroEventCallback);

  return true;
}