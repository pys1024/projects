#include "common.h"
#include "dev_check.h"

#define SWITCH_WIDTH  (20)
#define SWITCH_HEIGHT (40)

#define SPINNER_WIDTH  (40)
#define SPINNER_HEIGHT (40)

#define KEY_WIDTH    (20)
#define KEY_HEIGHT   (20)
#define KEY_OFFSET_X  (70)
#define KEY_OFFSET_Y  (90)
#define KEY_GAP_X     (20)
#define KEY_GAP_Y     (20)

lv_obj_t *dev_check(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);

  lv_obj_t *sw1 = lv_switch_create(screen);
  lv_obj_t *sw2 = lv_switch_create(screen);
  lv_obj_t *sw3 = lv_switch_create(screen);
  lv_obj_t *sw4 = lv_switch_create(screen);
  lv_obj_set_size(sw1, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_obj_set_size(sw2, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_obj_set_size(sw3, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_obj_set_size(sw4, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_switch_set_orientation(sw1, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(sw2, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(sw3, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(sw4, LV_SWITCH_ORIENTATION_VERTICAL);

  lv_obj_t *spinner1 = lv_spinner_create(screen);
  lv_obj_t *spinner2 = lv_spinner_create(screen);
  lv_obj_set_size(spinner1, SPINNER_WIDTH, SPINNER_HEIGHT);
  lv_obj_set_size(spinner2, SPINNER_WIDTH, SPINNER_HEIGHT);

  lv_obj_t *key1 = lv_button_create(screen);
  lv_obj_t *key2 = lv_button_create(screen);
  lv_obj_t *key3 = lv_button_create(screen);
  lv_obj_t *key4 = lv_button_create(screen);
  lv_obj_t *key5 = lv_button_create(screen);
  lv_obj_t *key6 = lv_button_create(screen);
  lv_obj_t *key7 = lv_button_create(screen);
  lv_obj_t *key8 = lv_button_create(screen);
  lv_obj_set_size(key1, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key2, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key3, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key4, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key5, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key6, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key7, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key8, KEY_WIDTH, KEY_HEIGHT);

  lv_obj_align(sw1, LV_ALIGN_CENTER, -100, -110);
  lv_obj_align(sw2, LV_ALIGN_CENTER, -75, -110);
  lv_obj_align(sw3, LV_ALIGN_CENTER, 75, -110);
  lv_obj_align(sw4, LV_ALIGN_CENTER, 100, -110);

  lv_obj_align(spinner1, LV_ALIGN_CENTER, -40, -110);
  lv_obj_align(spinner2, LV_ALIGN_CENTER, 40, -110);

  lv_obj_align(key1, LV_ALIGN_CENTER, -KEY_OFFSET_X, KEY_OFFSET_Y - KEY_GAP_Y);
  lv_obj_align(key2, LV_ALIGN_CENTER, -KEY_OFFSET_X - KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key3, LV_ALIGN_CENTER, -KEY_OFFSET_X + KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key4, LV_ALIGN_CENTER, -KEY_OFFSET_X, KEY_OFFSET_Y + KEY_GAP_Y);

  lv_obj_align(key5, LV_ALIGN_CENTER, KEY_OFFSET_X, KEY_OFFSET_Y - KEY_GAP_Y);
  lv_obj_align(key6, LV_ALIGN_CENTER, KEY_OFFSET_X - KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key7, LV_ALIGN_CENTER, KEY_OFFSET_X + KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key8, LV_ALIGN_CENTER, KEY_OFFSET_X, KEY_OFFSET_Y + KEY_GAP_Y);

  return screen;
}