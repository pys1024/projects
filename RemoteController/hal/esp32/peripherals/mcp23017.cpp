#include "common.h"
#include <Adafruit_MCP23X17.h>

static Adafruit_MCP23X17 *mcp = new Adafruit_MCP23X17();

static void mcp_read_cb(lv_indev_t * indev, lv_indev_data_t * data)
{
  // Adafruit_MCP23X17 *mcp = (Adafruit_MCP23X17 *)lv_indev_get_driver_data(indev);
  static uint32_t last_key_val = 0;
  uint32_t key_val = 0;

  key_val |= mcp->digitalRead(KEY1_PIN) ? 0 : (1 << 0);
  key_val |= mcp->digitalRead(KEY2_PIN) ? 0 : (1 << 1);
  key_val |= mcp->digitalRead(KEY3_PIN) ? 0 : (1 << 2);
  key_val |= digitalRead(KEY4_PIN) ? 0 : (1 << 3); // from esp32 gpio
  key_val |= mcp->digitalRead(KEY5_PIN) ? 0 : (1 << 4);
  key_val |= mcp->digitalRead(KEY6_PIN) ? 0 : (1 << 5);
  key_val |= mcp->digitalRead(KEY7_PIN) ? 0 : (1 << 6);
  key_val |= mcp->digitalRead(KEY8_PIN) ? 0 : (1 << 7);
  key_val |= mcp->digitalRead(EC1_SW_PIN) ? 0 : (1 << 8);
  key_val |= mcp->digitalRead(EC2_SW_PIN) ? 0 : (1 << 9);
  key_val |= mcp->digitalRead(RC1_SW_PIN) ? 0 : (1 << 10);
  key_val |= mcp->digitalRead(RC2_SW_PIN) ? 0 : (1 << 11);
  key_val |= mcp->digitalRead(SW1_PIN) ? 0 : (1 << 12);
  key_val |= mcp->digitalRead(SW2_PIN) ? 0 : (1 << 13);
  key_val |= mcp->digitalRead(SW3_PIN) ? 0 : (1 << 14);
  key_val |= mcp->digitalRead(SW4_PIN) ? 0 : (1 << 15);

  key_val = key_val ^ 0xF100; // invert key value

  data->key = key_val;
  data->state = (key_val != last_key_val) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
  last_key_val = key_val;
}

bool hal_mcp23017_init()
{
  // Wire.setPins(MCP_I2C_SDA_PIN, MCP_I2C_SCK_PIN);
  Wire.begin(MCP_I2C_SDA_PIN, MCP_I2C_SCK_PIN, I2C_SPEED);
  if (!mcp->begin_I2C()) {
    DBLOG("MCP23017 check failed!");
    return false; // MCP23017 not OK
  } else {
    DBLOG("MCP23017 check OK!");
  }

  mcp->pinMode(BEEP_PIN, OUTPUT);
  mcp->pinMode(KEY1_PIN, INPUT);
  mcp->pinMode(KEY2_PIN, INPUT);
  mcp->pinMode(KEY3_PIN, INPUT);
  mcp->pinMode(KEY5_PIN, INPUT);
  mcp->pinMode(KEY6_PIN, INPUT);
  mcp->pinMode(KEY7_PIN, INPUT);
  mcp->pinMode(KEY8_PIN, INPUT);
  mcp->pinMode(EC1_SW_PIN, INPUT);
  mcp->pinMode(EC2_SW_PIN, INPUT);
  mcp->pinMode(RC1_SW_PIN, INPUT);
  mcp->pinMode(RC2_SW_PIN, INPUT);
  mcp->pinMode(SW1_PIN, INPUT);
  mcp->pinMode(SW2_PIN, INPUT);
  mcp->pinMode(SW3_PIN, INPUT);
  mcp->pinMode(SW4_PIN, INPUT);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_enable(indev, false);
  lv_indev_set_driver_data(indev, mcp);
  lv_indev_set_type(indev, MY_INDEV_TYPE_BUTTON);
  lv_indev_set_mode(indev, LV_INDEV_MODE_EVENT); // event driven
  lv_indev_set_read_cb(indev, mcp_read_cb);

  return true;
}

void mcpDigitalWrite(uint8_t pin, uint8_t val)
{
  mcp->digitalWrite(pin, val);
}

uint8_t mcpDigitalRead(uint8_t pin)
{
  return mcp->digitalRead(pin);
}

void hal_beep()
{
  mcp->digitalWrite(BEEP_PIN, HIGH);
  delay(100);
  mcp->digitalWrite(BEEP_PIN, LOW);
}