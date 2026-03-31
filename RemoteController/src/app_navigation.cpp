#include "common.h"

#include "app_navigation.h"
#include "dev_check.h"
#include "main_menu.h"
#include "mpu_attitude.h"
#include "mpu_test.h"

static std::vector<app_page_id_t> nav_history;
static bool nav_has_current_page = false;
static app_page_id_t nav_current_page = APP_PAGE_MAIN_MENU;

static lv_obj_t *app_create_page(app_page_id_t page_id)
{
  switch (page_id) {
    case APP_PAGE_MAIN_MENU:
      return main_menu_screen();
    case APP_PAGE_DEV_CHECK:
      return dev_check();
    case APP_PAGE_MPU_TEST:
      return mpu_test();
    case APP_PAGE_MPU_ATTITUDE:
      return mpu_attitude();
    default:
      return NULL;
  }
}

bool app_nav_open_root(app_page_id_t page_id, lv_screen_load_anim_t anim_type,
                       uint32_t duration, uint32_t delay, bool auto_del)
{
  lv_obj_t *screen = app_create_page(page_id);
  if (!screen) {
    return false;
  }

  nav_history.clear();
  nav_has_current_page = true;
  nav_current_page = page_id;
  lv_screen_load_anim(screen, anim_type, duration, delay, auto_del);
  return true;
}

bool app_nav_open(app_page_id_t page_id, lv_screen_load_anim_t anim_type,
                  uint32_t duration, uint32_t delay, bool auto_del)
{
  lv_obj_t *screen = app_create_page(page_id);
  if (!screen) {
    return false;
  }

  if (nav_has_current_page) {
    nav_history.push_back(nav_current_page);
  }

  nav_has_current_page = true;
  nav_current_page = page_id;
  lv_screen_load_anim(screen, anim_type, duration, delay, auto_del);
  return true;
}

bool app_nav_back(lv_screen_load_anim_t anim_type, uint32_t duration,
                  uint32_t delay, bool auto_del)
{
  if (nav_history.empty()) {
    return false;
  }

  app_page_id_t prev_page = nav_history.back();
  lv_obj_t *screen = app_create_page(prev_page);
  if (!screen) {
    return false;
  }

  nav_history.pop_back();
  nav_has_current_page = true;
  nav_current_page = prev_page;
  lv_screen_load_anim(screen, anim_type, duration, delay, auto_del);
  return true;
}

bool app_nav_can_go_back(void)
{
  return !nav_history.empty();
}
