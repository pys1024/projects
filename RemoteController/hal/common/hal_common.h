#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MY_INDEV_TYPE_JOYSTICK1 ((lv_indev_type_t)0x10)
#define MY_INDEV_TYPE_JOYSTICK2 ((lv_indev_type_t)0x11)
#define MY_INDEV_TYPE_BATTERY ((lv_indev_type_t)0x12)
#define MY_INDEV_TYPE_BUTTON ((lv_indev_type_t)0x13)
#define MY_INDEV_TYPE_ENCODER1 ((lv_indev_type_t)0x14)
#define MY_INDEV_TYPE_ENCODER2 ((lv_indev_type_t)0x15)
#define MY_INDEV_TYPE_ACCEL ((lv_indev_type_t)0x16)
#define MY_INDEV_TYPE_GYRO ((lv_indev_type_t)0x17)

typedef enum {
    HAL_OK,
    HAL_ERROR,
    HAL_NO_MORE_DEVICES
} hal_status_t;

hal_status_t hal_init_device(uint8_t dev_id, char *msg, uint32_t msg_size);
void hal_beep();
bool hal_nrf24_send_cmd(int8_t cmd0, int8_t cmd1, int8_t cmd2, int8_t cmd3);

#ifdef __cplusplus
} /*extern "C"*/
#endif
