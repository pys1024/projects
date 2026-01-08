#include "common.h"
#include <ESP_Knob.h>

// Versatile_RotaryEncoder *encoder = new Versatile_RotaryEncoder(EC2_CLK_PIN, EC2_DT_PIN, 0);
ESP_Knob *knob1 = new ESP_Knob(EC1_CLK_PIN, EC1_DT_PIN);
ESP_Knob *knob2 = new ESP_Knob(EC2_CLK_PIN, EC2_DT_PIN);
char str[128];

#if USE_KNOB_CALLBACK
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
#endif

static void encoder_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  ESP_Knob *knob = (ESP_Knob *)lv_indev_get_driver_data(indev);
  int16_t count = knob->getCountValue();
  if (count != 0) {
    knob->clearCountValue();
  }

  data->enc_diff = count;
  data->state = (count != 0) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

bool hal_encoder_init(void)
{
  knob1->begin();
  knob2->begin();

#if USE_KNOB_CALLBACK
  knob1->attachLeftEventCallback(onKnobLeftEventCallback);
  knob1->attachRightEventCallback(onKnobRightEventCallback);
  knob1->attachHighLimitEventCallback(onKnobHighLimitEventCallback);
  knob1->attachLowLimitEventCallback(onKnobLowLimitEventCallback);
  knob1->attachZeroEventCallback(onKnobZeroEventCallback);

  knob2->attachLeftEventCallback(onKnobLeftEventCallback);
  knob2->attachRightEventCallback(onKnobRightEventCallback);
  knob2->attachHighLimitEventCallback(onKnobHighLimitEventCallback);
  knob2->attachLowLimitEventCallback(onKnobLowLimitEventCallback);
  knob2->attachZeroEventCallback(onKnobZeroEventCallback);
#endif

  lv_indev_t *indev1 = lv_indev_create();
  lv_indev_enable(indev1, false);
  lv_indev_set_driver_data(indev1, knob1);
  lv_indev_set_type(indev1, MY_INDEV_TYPE_ENCODER1);
  lv_indev_set_mode(indev1, LV_INDEV_MODE_EVENT); // event driven
  lv_indev_set_read_cb(indev1, encoder_read_cb);

  lv_indev_t *indev2 = lv_indev_create();
  lv_indev_enable(indev2, false);
  lv_indev_set_driver_data(indev2, knob2);
  lv_indev_set_type(indev2, MY_INDEV_TYPE_ENCODER2);
  lv_indev_set_mode(indev2, LV_INDEV_MODE_EVENT); // event driven
  lv_indev_set_read_cb(indev2, encoder_read_cb);

  return true;
}