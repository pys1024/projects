#pragma once

#include <stdint.h>

#include "lvgl.h"

typedef enum {
  APP_PAGE_MAIN_MENU = 0,
  APP_PAGE_DEV_CHECK,
  APP_PAGE_MPU_TEST,
  APP_PAGE_MPU_ATTITUDE,
} app_page_id_t;

bool app_nav_open_root(app_page_id_t page_id, lv_screen_load_anim_t anim_type,
                       uint32_t duration, uint32_t delay, bool auto_del);
bool app_nav_open(app_page_id_t page_id, lv_screen_load_anim_t anim_type,
                  uint32_t duration, uint32_t delay, bool auto_del);
bool app_nav_back(lv_screen_load_anim_t anim_type, uint32_t duration,
                  uint32_t delay, bool auto_del);
bool app_nav_can_go_back(void);
