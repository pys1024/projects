#ifndef APP_HAL_H
#define APP_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "encoder.h"
#include "mcp23017.h"
#include "mpu6050.h"
#include "nrf24.h"

/**
 * This function runs once and typically includes:
 * - Setting up display drivers.
 * - Configuring LVGL display and input devices
 */
void hal_setup(void);

/**
 * This function is continuously executed and typically includes:
 * - Updating LVGL's internal state & UI.
 */
void hal_loop(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*APP_HAL_H*/
