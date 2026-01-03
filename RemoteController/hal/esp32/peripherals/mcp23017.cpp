#include "common.h"
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 *mcp = new Adafruit_MCP23X17();

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