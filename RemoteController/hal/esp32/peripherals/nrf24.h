#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool hal_nrf24_init(void);
bool hal_nrf24_send_cmd(int8_t cmd0, int8_t cmd1, int8_t cmd2, int8_t cmd3);

#ifdef __cplusplus
}
#endif