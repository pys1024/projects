#include "common.h"
#include "joystick_and_bat.h"

static uint16_t static_lx_val = 0;
static uint16_t static_ly_val = 0;
static uint16_t static_rx_val = 0;
static uint16_t static_ry_val = 0;
uint16_t s_rx_vals[10] = {0};

static void joystick1_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  uint16_t lx = analogRead(LX_PIN);
  uint16_t ly = analogRead(LY_PIN);

  // Map the analog values to joystick coordinates
  int16_t x = lx >= static_lx_val ? my_map(lx, static_lx_val, 4095, 0, 100) : my_map(lx, 0, static_lx_val, -100, 0);
  int16_t y = ly >= static_ly_val ? my_map(ly, static_ly_val, 4095, 0, 100) : my_map(ly, 0, static_ly_val, -100, 0);

  // Set the data structure
  data->point.x = x;
  data->point.y = y;
  data->key = lx;
  data->btn_id = ly;
  data->state = (abs(x) > 10 || abs(y) > 10) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void joystick2_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  uint16_t rx = analogRead(RX_PIN);
  uint16_t ry = analogRead(RY_PIN);

  // Map the analog values to joystick coordinates
  int16_t x = rx >= static_rx_val ? my_map(rx, static_rx_val, 4095, 0, 100) : my_map(rx, 0, static_rx_val, -100, 0);
  int16_t y = ry >= static_ry_val ? my_map(ry, static_ry_val, 4095, 0, 100) : my_map(ry, 0, static_ry_val, -100, 0);

  // Set the data structure
  data->point.x = x;
  data->point.y = y;
  data->key = rx; // real value
  data->btn_id = ry; // real value
  data->timestamp = 100; // max value
  data->state = (abs(x) > 10 || abs(y) > 10) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void bat_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  uint32_t bat_val = analogReadMilliVolts(BAT_DET) * 2;
  data->key = bat_val;
  data->state = (bat_val > 3700) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

// Modify static values to be the median of 10 readings taken every 1ms
static uint16_t readMedianValue(int pin) {
  uint16_t readings[10];
  for (int i = 0; i < 10; ++i) {
    readings[i] = analogRead(pin);
    delay(1); // 1ms delay between readings
  }
  std::sort(readings, readings + 10);
  return readings[4]; // Return the median value
}

bool hal_joystick_and_bat_init(void)
{
  pinMode(BAT_DET, INPUT); // Set BAT_DET as input

  pinMode(LX_PIN, INPUT); // Set LX_PIN as input
  pinMode(LY_PIN, INPUT); // Set LY_PIN as input
  pinMode(RX_PIN, INPUT); // Set RX_PIN as input
  pinMode(RY_PIN, INPUT); // Set RY_PIN as input

  analogSetPinAttenuation(LX_PIN, ADC_11db); // Set LX_PIN attenuation to 11db
  analogSetPinAttenuation(LY_PIN, ADC_11db); // Set LY_PIN attenuation to 11db
  analogSetPinAttenuation(RX_PIN, ADC_11db); // Set RX_PIN attenuation to 11db
  analogSetPinAttenuation(RY_PIN, ADC_11db); // Set RY_PIN attenuation to 11db
  analogSetPinAttenuation(BAT_DET, ADC_11db); // Set BAT_DET attenuation to 11db

  // Set the initial values of the joystick coordinates
  static_lx_val = readMedianValue(LX_PIN);
  static_ly_val = readMedianValue(LY_PIN);
  static_rx_val = readMedianValue(RX_PIN);
  static_ry_val = readMedianValue(RY_PIN);

  lv_indev_t *indev1 = lv_indev_create();
  lv_indev_enable(indev1, false);
  lv_indev_set_driver_data(indev1, NULL);
  lv_indev_set_type(indev1, MY_INDEV_TYPE_JOYSTICK1);
  lv_indev_set_mode(indev1, LV_INDEV_MODE_EVENT); // event driven
  lv_indev_set_read_cb(indev1, joystick1_read_cb);

  lv_indev_t *indev2 = lv_indev_create();
  lv_indev_enable(indev2, false);
  lv_indev_set_driver_data(indev2, NULL);
  lv_indev_set_type(indev2, MY_INDEV_TYPE_JOYSTICK2);
  lv_indev_set_mode(indev2, LV_INDEV_MODE_EVENT); // event driven
  lv_indev_set_read_cb(indev2, joystick2_read_cb);

  lv_indev_t *indev3 = lv_indev_create();
  lv_indev_enable(indev3, false);
  lv_indev_set_driver_data(indev3, NULL);
  lv_indev_set_type(indev3, MY_INDEV_TYPE_BATTERY);
  lv_indev_set_mode(indev3, LV_INDEV_MODE_EVENT); // event driven
  lv_indev_set_read_cb(indev3, bat_read_cb);

  return true;
}