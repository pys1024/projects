#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool hal_mcp23017_init(void);
void mcpDigitalWrite(uint8_t pin, uint8_t val);
uint8_t mcpDigitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif