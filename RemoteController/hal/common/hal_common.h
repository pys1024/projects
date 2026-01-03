#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    HAL_OK,
    HAL_ERROR,
    HAL_NO_MORE_DEVICES
} hal_status_t;

hal_status_t hal_init_device(uint8_t dev_id, char *msg, uint32_t msg_size);
void hal_beep();

#ifdef __cplusplus
} /*extern "C"*/
#endif