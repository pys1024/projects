#include "common.h"

#include "app_navigation.h"
#include "main_menu.h"

#define MENU_BG_TOP      lv_color_hex(0x08131F)
#define MENU_BG_BOTTOM   lv_color_hex(0x123149)
#define MENU_PANEL_TOP   lv_color_hex(0x14354E)
#define MENU_PANEL_BTM   lv_color_hex(0x1E4E6E)
#define MENU_CARD_TOP    lv_color_hex(0x112A3D)
#define MENU_CARD_BTM    lv_color_hex(0x1A405B)
#define MENU_BORDER      lv_color_hex(0x6FD5FF)
#define MENU_TEXT_MAIN   lv_color_hex(0xEAF8FF)
#define MENU_TEXT_SOFT   lv_color_hex(0xBCE8FF)
#define MENU_TEXT_DIM    lv_color_hex(0x8EC9E6)
#define MENU_ARROW       lv_color_hex(0x82DEFF)
#define MENU_HOT         lv_color_hex(0x58C7F2)

static void anim_set_border_opa_cb(void *obj, int32_t value)
{
  lv_obj_set_style_border_opa((lv_obj_t *)obj, (lv_opa_t)value, LV_PART_MAIN);
}

static void anim_set_text_opa_cb(void *obj, int32_t value)
{
  lv_obj_set_style_text_opa((lv_obj_t *)obj, (lv_opa_t)value, LV_PART_MAIN);
}

static void play_panel_pulse(lv_obj_t *obj, uint32_t delay)
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_exec_cb(&a, anim_set_border_opa_cb);
  lv_anim_set_values(&a, LV_OPA_50, LV_OPA_100);
  lv_anim_set_time(&a, 1400);
  lv_anim_set_delay(&a, delay);
  lv_anim_set_playback_time(&a, 1400);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
  lv_anim_start(&a);
}

static void play_label_pulse(lv_obj_t *obj, uint32_t delay)
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_exec_cb(&a, anim_set_text_opa_cb);
  lv_anim_set_values(&a, LV_OPA_50, LV_OPA_100);
  lv_anim_set_time(&a, 1200);
  lv_anim_set_delay(&a, delay);
  lv_anim_set_playback_time(&a, 1200);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
  lv_anim_start(&a);
}

static void add_menu_bg_fx(lv_obj_t *screen)
{
  static const lv_point_precise_t fx_line1[] = {{14, 214}, {90, 138}};
  static const lv_point_precise_t fx_line2[] = {{226, 82}, {154, 154}};

  lv_obj_t *l1 = lv_line_create(screen);
  lv_line_set_points(l1, fx_line1, 2);
  lv_obj_set_style_line_color(l1, MENU_HOT, LV_PART_MAIN);
  lv_obj_set_style_line_width(l1, 2, LV_PART_MAIN);
  lv_obj_set_style_line_opa(l1, LV_OPA_30, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(l1, true, LV_PART_MAIN);
  lv_obj_remove_flag(l1, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_move_background(l1);

  lv_obj_t *l2 = lv_line_create(screen);
  lv_line_set_points(l2, fx_line2, 2);
  lv_obj_set_style_line_color(l2, MENU_HOT, LV_PART_MAIN);
  lv_obj_set_style_line_width(l2, 2, LV_PART_MAIN);
  lv_obj_set_style_line_opa(l2, LV_OPA_30, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(l2, true, LV_PART_MAIN);
  lv_obj_remove_flag(l2, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_move_background(l2);
}

static void menu_item_event_cb(lv_event_t *e)
{
  const char *name = (const char *)lv_event_get_user_data(e);
  app_page_id_t next = APP_PAGE_MAIN_MENU;
  bool valid = true;

  if (strcmp(name, "dev") == 0) {
    next = APP_PAGE_DEV_CHECK;
  } else if (strcmp(name, "mpu") == 0) {
    next = APP_PAGE_MPU_TEST;
  } else if (strcmp(name, "att") == 0) {
    next = APP_PAGE_MPU_ATTITUDE;
  } else {
    valid = false;
  }

  if (valid) {
    app_nav_open(next, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 260, 0, true);
  }
}

static lv_obj_t *create_menu_card(lv_obj_t *parent, const char *title, const char *subtitle, const char *key)
{
  static lv_style_t style_pressed;
  static bool style_init = false;
  if (!style_init) {
    lv_style_init(&style_pressed);
    lv_style_set_bg_color(&style_pressed, lv_color_hex(0x1E4A69));
    lv_style_set_border_color(&style_pressed, lv_color_hex(0xA7EAFF));
    lv_style_set_translate_x(&style_pressed, 1);
    lv_style_set_translate_y(&style_pressed, 1);
    style_init = true;
  }

  lv_obj_t *card = lv_button_create(parent);
  lv_obj_set_size(card, LCD_WIDTH - 32, 58);
  lv_obj_set_style_radius(card, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, MENU_CARD_TOP, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(card, MENU_CARD_BTM, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, MENU_BORDER, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(card, 8, LV_PART_MAIN);
  lv_obj_add_style(card, &style_pressed, LV_STATE_PRESSED);

  lv_obj_add_event_cb(card, menu_item_event_cb, LV_EVENT_CLICKED, (void *)key);

  lv_obj_t *accent = lv_obj_create(card);
  lv_obj_set_size(accent, 3, 42);
  lv_obj_align(accent, LV_ALIGN_LEFT_MID, -6, 0);
  lv_obj_set_style_radius(accent, 2, LV_PART_MAIN);
  lv_obj_set_style_border_width(accent, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(accent, MENU_HOT, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(accent, LV_OPA_70, LV_PART_MAIN);
  lv_obj_set_style_pad_all(accent, 0, LV_PART_MAIN);
  lv_obj_remove_flag(accent, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_t *label_title = lv_label_create(card);
  lv_label_set_text(label_title, title);
  lv_obj_set_style_text_color(label_title, MENU_TEXT_MAIN, LV_PART_MAIN);
  lv_obj_set_style_text_font(label_title, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_align(label_title, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *label_sub = lv_label_create(card);
  lv_label_set_text(label_sub, subtitle);
  lv_obj_set_style_text_color(label_sub, MENU_TEXT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_text_font(label_sub, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_opa(label_sub, LV_OPA_80, LV_PART_MAIN);
  lv_obj_align(label_sub, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  lv_obj_t *arrow = lv_label_create(card);
  lv_label_set_text(arrow, ">");
  lv_obj_set_style_text_font(arrow, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_set_style_text_color(arrow, MENU_ARROW, LV_PART_MAIN);
  lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -4, 0);

  return card;
}

lv_obj_t *main_menu_screen(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, MENU_BG_TOP, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(screen, MENU_BG_BOTTOM, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
  add_menu_bg_fx(screen);

  lv_obj_t *top = lv_obj_create(screen);
  lv_obj_set_size(top, LCD_WIDTH - 18, 30);
  lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 8);
  lv_obj_set_style_radius(top, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(top, MENU_PANEL_TOP, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(top, MENU_PANEL_BTM, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(top, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_border_width(top, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(top, MENU_BORDER, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(top, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(top, 6, LV_PART_MAIN);
  lv_obj_remove_flag(top, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *title = lv_label_create(top);
  lv_label_set_text(title, "REMOTE CONTROLLER");
  lv_obj_set_style_text_color(title, MENU_TEXT_MAIN, LV_PART_MAIN);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t *tag = lv_label_create(top);
  lv_label_set_text(tag, "AERO");
  lv_obj_set_style_text_color(tag, MENU_BORDER, LV_PART_MAIN);
  lv_obj_set_style_text_font(tag, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_opa(tag, LV_OPA_90, LV_PART_MAIN);
  lv_obj_align(tag, LV_ALIGN_RIGHT_MID, -6, 0);

  lv_obj_t *subtitle = lv_label_create(screen);
  lv_label_set_text(subtitle, "Select instrument page");
  lv_obj_set_style_text_color(subtitle, MENU_TEXT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_opa(subtitle, LV_OPA_80, LV_PART_MAIN);
  lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 42);

  lv_obj_t *card_dev = create_menu_card(screen, "Device Check", "Buttons / Joystick / RF", "dev");
  lv_obj_align(card_dev, LV_ALIGN_TOP_MID, 0, 64);

  lv_obj_t *card_mpu = create_menu_card(screen, "MPU6050 Test", "Accelerometer + Gyroscope", "mpu");
  lv_obj_align(card_mpu, LV_ALIGN_TOP_MID, 0, 128);

  lv_obj_t *card_att = create_menu_card(screen, "MPU Attitude", "6 dials / integrated angle", "att");
  lv_obj_align(card_att, LV_ALIGN_TOP_MID, 0, 192);

  lv_obj_t *hint = lv_label_create(screen);
  lv_label_set_text(hint, "Tap to enter  |  hold top in attitude to switch theme");
  lv_obj_set_width(hint, LCD_WIDTH - 20);
  lv_label_set_long_mode(hint, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(hint, MENU_TEXT_DIM, LV_PART_MAIN);
  lv_obj_set_style_text_font(hint, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_opa(hint, LV_OPA_80, LV_PART_MAIN);
  lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -6);

  play_panel_pulse(top, 120);
  play_label_pulse(tag, 260);

  return screen;
}
