#include "common.h"

#include "mpu_test.h"
#include "main_menu.h"

#define MPU_BG_TOP      lv_color_hex(0x08131F)
#define MPU_BG_BOTTOM   lv_color_hex(0x123149)
#define MPU_TOP_BAR     lv_color_hex(0x14354E)
#define MPU_TOP_GRAD    lv_color_hex(0x1E4E6E)
#define MPU_CARD_TOP    lv_color_hex(0x112A3D)
#define MPU_CARD_BTM    lv_color_hex(0x1A405B)
#define MPU_BORDER      lv_color_hex(0x6FD5FF)
#define MPU_TEXT_MAIN   lv_color_hex(0xEAF8FF)
#define MPU_TEXT_SOFT   lv_color_hex(0xBCE8FF)
#define MPU_BAR_BG      lv_color_hex(0x12283A)
#define MPU_BAR_FILL    lv_color_hex(0x64C9F4)

static lv_timer_t *mpu_timer = NULL;

static void anim_set_text_opa_cb(void *obj, int32_t value)
{
  lv_obj_set_style_text_opa((lv_obj_t *)obj, (lv_opa_t)value, LV_PART_MAIN);
}

static void play_label_pulse(lv_obj_t *obj, uint32_t delay)
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_exec_cb(&a, anim_set_text_opa_cb);
  lv_anim_set_values(&a, LV_OPA_50, LV_OPA_100);
  lv_anim_set_time(&a, 1300);
  lv_anim_set_delay(&a, delay);
  lv_anim_set_playback_time(&a, 1300);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
  lv_anim_start(&a);
}

static void add_mpu_bg_fx(lv_obj_t *screen)
{
  static const lv_point_precise_t fx_line1[] = {{12, 228}, {92, 148}};
  static const lv_point_precise_t fx_line2[] = {{226, 98}, {150, 174}};

  lv_obj_t *l1 = lv_line_create(screen);
  lv_line_set_points(l1, fx_line1, 2);
  lv_obj_set_style_line_color(l1, lv_color_hex(0x5DBEE8), LV_PART_MAIN);
  lv_obj_set_style_line_width(l1, 2, LV_PART_MAIN);
  lv_obj_set_style_line_opa(l1, LV_OPA_30, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(l1, true, LV_PART_MAIN);
  lv_obj_remove_flag(l1, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_move_background(l1);

  lv_obj_t *l2 = lv_line_create(screen);
  lv_line_set_points(l2, fx_line2, 2);
  lv_obj_set_style_line_color(l2, lv_color_hex(0x5DBEE8), LV_PART_MAIN);
  lv_obj_set_style_line_width(l2, 2, LV_PART_MAIN);
  lv_obj_set_style_line_opa(l2, LV_OPA_30, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(l2, true, LV_PART_MAIN);
  lv_obj_remove_flag(l2, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_move_background(l2);
}

static void format_milli(char *buf, size_t size, int32_t v)
{
  int32_t abs_v = v >= 0 ? v : -v;
  snprintf(buf, size, "%c%d.%03d", v < 0 ? '-' : '+', abs_v / 1000, abs_v % 1000);
}

static void back_event_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  lv_obj_t *menu = main_menu_screen();
  lv_screen_load_anim(menu, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 220, 0, true);
}

static void mpu_screen_delete_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (mpu_timer) {
    lv_timer_delete(mpu_timer);
    mpu_timer = NULL;
  }
}

static void mpu_timer_cb(lv_timer_t *timer)
{
  lv_obj_t *screen = (lv_obj_t *)lv_timer_get_user_data(timer);
  lv_indev_data_t data;

  lv_indev_t *accel = find_indev_by_type(MY_INDEV_TYPE_ACCEL);
  lv_indev_t *gyro = find_indev_by_type(MY_INDEV_TYPE_GYRO);

  if (accel) {
    lv_indev_get_read_cb(accel)(accel, &data);
    int32_t ax = data.point.x;
    int32_t ay = data.point.y;
    int32_t az = data.key;

    lv_obj_t *label = lv_obj_find_by_name(screen, "mpu_accel");
    if (label) {
      char sx[16], sy[16], sz[16], line[80];
      format_milli(sx, sizeof(sx), ax);
      format_milli(sy, sizeof(sy), ay);
      format_milli(sz, sizeof(sz), az);
      snprintf(line, sizeof(line), "X  %s\nY  %s\nZ  %s", sx, sy, sz);
      lv_label_set_text(label, line);
    }

    lv_obj_t *bar = lv_obj_find_by_name(screen, "mpu_accel_bar");
    if (bar) {
      int32_t val = (int32_t)my_map(ax, -10000, 10000, 0, 100);
      val = val < 0 ? 0 : val > 100 ? 100 : val;
      lv_bar_set_value(bar, val, LV_ANIM_ON);
    }
  }

  if (gyro) {
    lv_indev_get_read_cb(gyro)(gyro, &data);
    int32_t gx = data.point.x;
    int32_t gy = data.point.y;
    int32_t gz = data.key;

    lv_obj_t *label = lv_obj_find_by_name(screen, "mpu_gyro");
    if (label) {
      char sx[16], sy[16], sz[16], line[80];
      format_milli(sx, sizeof(sx), gx);
      format_milli(sy, sizeof(sy), gy);
      format_milli(sz, sizeof(sz), gz);
      snprintf(line, sizeof(line), "X  %s\nY  %s\nZ  %s", sx, sy, sz);
      lv_label_set_text(label, line);
    }

    lv_obj_t *bar = lv_obj_find_by_name(screen, "mpu_gyro_bar");
    if (bar) {
      int32_t val = (int32_t)my_map(gx, -3000, 3000, 0, 100);
      val = val < 0 ? 0 : val > 100 ? 100 : val;
      lv_bar_set_value(bar, val, LV_ANIM_ON);
    }
  }
}

static lv_obj_t *create_sensor_card(lv_obj_t *parent, const char *title, const char *name_label, const char *name_bar)
{
  lv_obj_t *card = lv_obj_create(parent);
  lv_obj_set_size(card, LCD_WIDTH - 24, 98);
  lv_obj_set_style_radius(card, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, MPU_CARD_TOP, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(card, MPU_CARD_BTM, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, MPU_BORDER, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(card, 8, LV_PART_MAIN);
  lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *accent = lv_obj_create(card);
  lv_obj_set_size(accent, 3, 68);
  lv_obj_align(accent, LV_ALIGN_LEFT_MID, -7, 0);
  lv_obj_set_style_radius(accent, 2, LV_PART_MAIN);
  lv_obj_set_style_border_width(accent, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(accent, MPU_BAR_FILL, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(accent, LV_OPA_70, LV_PART_MAIN);
  lv_obj_set_style_pad_all(accent, 0, LV_PART_MAIN);
  lv_obj_remove_flag(accent, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_t *t = lv_label_create(card);
  lv_label_set_text(t, title);
  lv_obj_set_style_text_font(t, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_color(t, MPU_TEXT_SOFT, LV_PART_MAIN);
  lv_obj_align(t, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *label = lv_label_create(card);
  lv_obj_set_name(label, name_label);
  lv_label_set_text(label, "X  +0.000\nY  +0.000\nZ  +0.000");
  lv_obj_set_width(label, 106);
  lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_set_style_text_color(label, MPU_TEXT_MAIN, LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 10);

  lv_obj_t *bar = lv_bar_create(card);
  lv_obj_set_name(bar, name_bar);
  lv_obj_set_size(bar, 80, 12);
  lv_bar_set_range(bar, 0, 100);
  lv_bar_set_value(bar, 50, LV_ANIM_OFF);
  lv_obj_set_style_radius(bar, 6, LV_PART_MAIN);
  lv_obj_set_style_bg_color(bar, MPU_BAR_BG, LV_PART_MAIN);
  lv_obj_set_style_bg_color(bar, MPU_BAR_FILL, LV_PART_INDICATOR);
  lv_obj_align(bar, LV_ALIGN_RIGHT_MID, -2, 10);

  return card;
}

lv_obj_t *mpu_test(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, MPU_BG_TOP, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(screen, MPU_BG_BOTTOM, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(screen, mpu_screen_delete_cb, LV_EVENT_DELETE, NULL);
  add_mpu_bg_fx(screen);

  lv_obj_t *top = lv_obj_create(screen);
  lv_obj_set_size(top, LCD_WIDTH - 12, 30);
  lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 6);
  lv_obj_set_style_radius(top, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(top, MPU_TOP_BAR, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(top, MPU_TOP_GRAD, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(top, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_border_color(top, MPU_BORDER, LV_PART_MAIN);
  lv_obj_set_style_border_width(top, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(top, 6, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(top, 0, LV_PART_MAIN);

  lv_obj_t *title = lv_label_create(top);
  lv_label_set_text(title, "MPU6050 TEST");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_set_style_text_color(title, MPU_TEXT_MAIN, LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t *tag = lv_label_create(top);
  lv_label_set_text(tag, "LIVE");
  lv_obj_set_style_text_font(tag, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_color(tag, MPU_BORDER, LV_PART_MAIN);
  lv_obj_set_style_text_opa(tag, LV_OPA_90, LV_PART_MAIN);
  lv_obj_align(tag, LV_ALIGN_RIGHT_MID, -8, 0);
  play_label_pulse(tag, 240);

  lv_obj_t *top_hit = lv_button_create(screen);
  lv_obj_set_size(top_hit, LCD_WIDTH - 12, 30);
  lv_obj_align(top_hit, LV_ALIGN_TOP_MID, 0, 6);
  lv_obj_set_style_bg_opa(top_hit, LV_OPA_0, LV_PART_MAIN);
  lv_obj_set_style_border_width(top_hit, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(top_hit, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(top_hit, 10, LV_PART_MAIN);
  lv_obj_add_event_cb(top_hit, back_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_move_foreground(top_hit);

  lv_obj_t *accel_card = create_sensor_card(screen, "Accelerometer (m/s^2)", "mpu_accel", "mpu_accel_bar");
  lv_obj_align(accel_card, LV_ALIGN_TOP_MID, 0, 52);

  lv_obj_t *gyro_card = create_sensor_card(screen, "Gyroscope (rad/s)", "mpu_gyro", "mpu_gyro_bar");
  lv_obj_align(gyro_card, LV_ALIGN_TOP_MID, 0, 156);

  mpu_timer = lv_timer_create(mpu_timer_cb, 100, screen);

  return screen;
}
