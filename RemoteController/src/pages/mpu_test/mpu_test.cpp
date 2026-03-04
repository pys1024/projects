#include "common.h"

#include "mpu_test.h"
#include "main_menu.h"

static lv_timer_t *mpu_timer = NULL;

static void format_milli(char *buf, size_t size, int32_t v)
{
  int32_t abs_v = v >= 0 ? v : -v;
  snprintf(buf, size, "%s%d.%03d", v < 0 ? "-" : "", abs_v / 1000, abs_v % 1000);
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
      snprintf(line, sizeof(line), "A  X:%s\n   Y:%s\n   Z:%s", sx, sy, sz);
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
      snprintf(line, sizeof(line), "G  X:%s\n   Y:%s\n   Z:%s", sx, sy, sz);
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
  lv_obj_set_size(card, LCD_WIDTH - 24, 88);
  lv_obj_set_style_radius(card, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, lv_color_hex(0x0D3146), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(card, lv_color_hex(0x134864), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, lv_color_hex(0x2AD2C3), LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(card, 8, LV_PART_MAIN);
  lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *t = lv_label_create(card);
  lv_label_set_text(t, title);
  lv_obj_set_style_text_color(t, lv_color_hex(0xC9F8FF), LV_PART_MAIN);
  lv_obj_align(t, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *label = lv_label_create(card);
  lv_obj_set_name(label, name_label);
  lv_label_set_text(label, "X:0.00\nY:0.00\nZ:0.00");
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 10);

  lv_obj_t *bar = lv_bar_create(card);
  lv_obj_set_name(bar, name_bar);
  lv_obj_set_size(bar, 72, 12);
  lv_bar_set_range(bar, 0, 100);
  lv_bar_set_value(bar, 50, LV_ANIM_OFF);
  lv_obj_set_style_radius(bar, 6, LV_PART_MAIN);
  lv_obj_set_style_bg_color(bar, lv_color_hex(0x1A3241), LV_PART_MAIN);
  lv_obj_set_style_bg_color(bar, lv_color_hex(0x38D0BA), LV_PART_INDICATOR);
  lv_obj_align(bar, LV_ALIGN_RIGHT_MID, -4, 8);

  return card;
}

lv_obj_t *mpu_test(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x031521), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x09354C), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(screen, mpu_screen_delete_cb, LV_EVENT_DELETE, NULL);

  lv_obj_t *top = lv_obj_create(screen);
  lv_obj_set_size(top, LCD_WIDTH - 12, 30);
  lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 6);
  lv_obj_set_style_radius(top, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(top, lv_color_hex(0x0D3448), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(top, lv_color_hex(0x145571), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(top, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_border_color(top, lv_color_hex(0x2AD2C3), LV_PART_MAIN);
  lv_obj_set_style_border_width(top, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(top, 6, LV_PART_MAIN);

  lv_obj_t *title = lv_label_create(top);
  lv_label_set_text(title, "MPU6050 TEST");
  lv_obj_set_style_text_color(title, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 30, 0);

  lv_obj_t *back = lv_button_create(top);
  lv_obj_set_size(back, 24, 18);
  lv_obj_align(back, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_set_style_radius(back, 6, LV_PART_MAIN);
  lv_obj_set_style_bg_color(back, lv_color_hex(0x1A627D), LV_PART_MAIN);
  lv_obj_add_event_cb(back, back_event_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *back_text = lv_label_create(back);
  lv_label_set_text(back_text, "<");
  lv_obj_set_style_text_color(back_text, lv_color_white(), LV_PART_MAIN);
  lv_obj_center(back_text);

  lv_obj_t *accel_card = create_sensor_card(screen, "Accelerometer (m/s^2)", "mpu_accel", "mpu_accel_bar");
  lv_obj_align(accel_card, LV_ALIGN_TOP_MID, 0, 44);

  lv_obj_t *gyro_card = create_sensor_card(screen, "Gyroscope (rad/s)", "mpu_gyro", "mpu_gyro_bar");
  lv_obj_align(gyro_card, LV_ALIGN_TOP_MID, 0, 140);

  mpu_timer = lv_timer_create(mpu_timer_cb, 100, screen);

  return screen;
}
