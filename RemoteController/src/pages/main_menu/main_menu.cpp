#include "common.h"

#include "main_menu.h"
#include "dev_check.h"
#include "mpu_test.h"

static void menu_item_event_cb(lv_event_t *e)
{
  const char *name = (const char *)lv_event_get_user_data(e);
  lv_obj_t *next = NULL;

  if (strcmp(name, "dev") == 0) {
    next = dev_check();
  } else if (strcmp(name, "mpu") == 0) {
    next = mpu_test();
  }

  if (next) {
    lv_screen_load_anim(next, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 260, 0, true);
  }
}

static lv_obj_t *create_menu_card(lv_obj_t *parent, const char *title, const char *subtitle, const char *key)
{
  lv_obj_t *card = lv_button_create(parent);
  lv_obj_set_size(card, LCD_WIDTH - 40, 76);
  lv_obj_set_style_radius(card, 12, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, lv_color_hex(0x11405A), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(card, lv_color_hex(0x156581), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, lv_color_hex(0x2BD3C6), LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 14, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(card, lv_color_hex(0x071D2B), LV_PART_MAIN);
  lv_obj_set_style_pad_all(card, 10, LV_PART_MAIN);

  lv_obj_add_event_cb(card, menu_item_event_cb, LV_EVENT_CLICKED, (void *)key);

  lv_obj_t *label_title = lv_label_create(card);
  lv_label_set_text(label_title, title);
  lv_obj_set_style_text_color(label_title, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(label_title, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *label_sub = lv_label_create(card);
  lv_label_set_text(label_sub, subtitle);
  lv_obj_set_style_text_color(label_sub, lv_color_hex(0xB8ECFF), LV_PART_MAIN);
  lv_obj_set_style_text_opa(label_sub, LV_OPA_70, LV_PART_MAIN);
  lv_obj_align(label_sub, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  lv_obj_t *arrow = lv_label_create(card);
  lv_label_set_text(arrow, ">>");
  lv_obj_set_style_text_color(arrow, lv_color_hex(0x8FFAFF), LV_PART_MAIN);
  lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -2, 0);

  return card;
}

lv_obj_t *main_menu_screen(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x041621), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x0A2F44), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);

  lv_obj_t *title = lv_label_create(screen);
  lv_label_set_text(title, "REMOTE CONTROLLER");
  lv_obj_set_style_text_color(title, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 18);

  lv_obj_t *subtitle = lv_label_create(screen);
  lv_label_set_text(subtitle, "Select test menu");
  lv_obj_set_style_text_color(subtitle, lv_color_hex(0x9FDDF6), LV_PART_MAIN);
  lv_obj_set_style_text_opa(subtitle, LV_OPA_70, LV_PART_MAIN);
  lv_obj_align_to(subtitle, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);

  lv_obj_t *card_dev = create_menu_card(screen, "Device Check", "Buttons / Joystick / RF", "dev");
  lv_obj_align(card_dev, LV_ALIGN_TOP_MID, 0, 72);

  lv_obj_t *card_mpu = create_menu_card(screen, "MPU6050 Test", "Accelerometer + Gyroscope", "mpu");
  lv_obj_align(card_mpu, LV_ALIGN_TOP_MID, 0, 160);

  lv_obj_t *hint = lv_label_create(screen);
  lv_label_set_text(hint, "Tap card to enter");
  lv_obj_set_style_text_color(hint, lv_color_hex(0x7FD7E7), LV_PART_MAIN);
  lv_obj_set_style_text_opa(hint, LV_OPA_60, LV_PART_MAIN);
  lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -10);

  return screen;
}
