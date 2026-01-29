#include "common.h"
#include "dev_check.h"
#include "lv_tools.h"

#define ENABLE_JOYSTICK_TRAJECTORY 1
#define TRAJECTORY_MAX_POINTS    100

#define CO_PREFIX "dev_"
#define CO_NAME(key) CO_PREFIX #key

#define SWITCH_WIDTH  (20)
#define SWITCH_HEIGHT (40)

#define SWITCH_OFFSET_X (100)
#define SWITCH_OFFSET_Y (110)
#define SWITCH_GAP_X    (8)

#define ARC_WIDTH  (40)
#define ARC_HEIGHT (40)

#define KEY_WIDTH    (25)
#define KEY_HEIGHT   (25)
#define KEY_OFFSET_X  (70)
#define KEY_OFFSET_Y  (90)
#define KEY_GAP_X     (30)
#define KEY_GAP_Y     (30)

#define JOYSTICK_BASE_RADIUS  (50)
#define JOYSTICK_STICK_RADIUS (8)
#define JOYSTICK_BOUNDARY (JOYSTICK_BASE_RADIUS - (JOYSTICK_STICK_RADIUS * 1.0))

#define JOYSTICK_OFFSET_X (70)
#define JOYSTICK_OFFSET_Y (-15)

#define UPDATE_KEY_STATE(KEY, BIT)                 \
  obj = lv_obj_find_by_name(screen, CO_NAME(KEY)); \
  if (obj)                                         \
  {                                                \
    if (data.key & (1 << BIT))                     \
    {                                              \
      lv_obj_add_state(obj, LV_STATE_PRESSED);     \
    }                                              \
    else                                           \
    {                                              \
      lv_obj_remove_state(obj, LV_STATE_PRESSED);  \
    }                                              \
  }

#define UPDATE_SWITCH_STATE(KEY, BIT)              \
  obj = lv_obj_find_by_name(screen, CO_NAME(KEY)); \
  if (obj)                                         \
  {                                                \
    if (data.key & (1 << BIT))                     \
    {                                              \
      lv_obj_add_state(obj, LV_STATE_CHECKED);     \
    }                                              \
    else                                           \
    {                                              \
      lv_obj_remove_state(obj, LV_STATE_CHECKED);  \
    }                                              \
  }

static lv_timer_t *timer = NULL;

static lv_obj_t *create_joystick(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
  static lv_style_t style_stick_pressed;
  lv_style_init(&style_stick_pressed);
  lv_style_set_bg_color(&style_stick_pressed, lv_palette_darken(LV_PALETTE_RED, 2));

  lv_obj_t *base = lv_obj_create(parent);
  lv_obj_set_size(base, JOYSTICK_BASE_RADIUS * 2, JOYSTICK_BASE_RADIUS * 2);
  lv_obj_align(base, LV_ALIGN_CENTER, x, y);
  lv_obj_set_style_radius(base, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(base, lv_color_make(20, 20, 20), LV_PART_MAIN);
  lv_obj_set_style_border_width(base, 1, LV_PART_MAIN);
  lv_obj_set_style_pad_all(base, 0, LV_PART_MAIN);
  lv_obj_remove_flag(base, LV_OBJ_FLAG_SCROLLABLE);

  // Add horizontal line
  lv_obj_t *h_line = lv_line_create(base);
  static const lv_point_precise_t h_points[] = {{0, JOYSTICK_BASE_RADIUS}, {JOYSTICK_BASE_RADIUS*2, JOYSTICK_BASE_RADIUS}};
  lv_line_set_points(h_line, h_points, 2);
  lv_obj_set_style_line_color(h_line, lv_color_make(0, 100, 0), LV_PART_MAIN);
  lv_obj_set_style_line_width(h_line, 1, LV_PART_MAIN);
  lv_obj_set_style_line_dash_gap(h_line, 5, LV_PART_MAIN);
  lv_obj_set_style_line_dash_width(h_line, 5, LV_PART_MAIN);
  lv_obj_set_style_bg_color(h_line, lv_color_make(0, 50, 0), LV_PART_MAIN);

  // Add vertical line
  lv_obj_t *v_line = lv_line_create(base);
  static const lv_point_precise_t v_points[] = {{JOYSTICK_BASE_RADIUS, 0}, {JOYSTICK_BASE_RADIUS, JOYSTICK_BASE_RADIUS*2}};
  lv_line_set_points(v_line, v_points, 2);
  lv_obj_set_style_line_color(v_line, lv_color_make(0, 100, 0), LV_PART_MAIN);
  lv_obj_set_style_line_width(v_line, 1, LV_PART_MAIN);
  lv_obj_set_style_line_dash_gap(v_line, 5, LV_PART_MAIN);
  lv_obj_set_style_line_dash_width(v_line, 5, LV_PART_MAIN);

  lv_obj_t *stick = lv_button_create(base);
  lv_obj_set_name(stick, "stick");
  lv_obj_set_size(stick, JOYSTICK_STICK_RADIUS * 2, JOYSTICK_STICK_RADIUS * 2);
  lv_obj_center(stick);
  lv_obj_set_style_radius(stick, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(stick, lv_color_make(50, 100, 200), LV_PART_MAIN);
  lv_obj_set_style_border_width(stick, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(stick, 0, LV_PART_MAIN);
  lv_obj_add_style(stick, &style_stick_pressed, LV_STATE_PRESSED);

  lv_obj_t *label = lv_label_create(base);
  lv_label_set_text(label, "0,0");
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_center(label);

#if ENABLE_JOYSTICK_TRAJECTORY
  lv_obj_t *trajectory = lv_line_create(base);
  lv_obj_set_style_line_color(trajectory, lv_color_make(255, 0, 0), LV_PART_MAIN);
  lv_obj_set_style_line_width(trajectory, 2, LV_PART_MAIN);
  lv_obj_set_name(trajectory, "trajectory");
#endif

  return base;
}

static void event_handler(lv_event_t *e)
{
  lv_obj_t *screen = (lv_obj_t *)lv_event_get_target(e);
  // lv_event_code_t code = lv_event_get_code(e);
  uint32_t key = lv_indev_get_key(lv_indev_get_act());
  lv_obj_t *obj = NULL;
  char co_name[20];
  uint8_t opcode = 0;

  switch (key) {
    case '5' ... '8':
      key = key - 4;
      opcode = 2; // switch off
    case '1' ... '4':
      opcode = opcode ? opcode : 1; // switch on
    case 'w':
    case 'a':
    case 's':
    case 'd':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
      snprintf(co_name, sizeof(co_name), "%s%c", CO_PREFIX, (char)key);
      obj = lv_obj_find_by_name(screen, co_name);
      break;
    default:
      return;
  }

  if (obj == NULL) {
    LV_LOG_USER("Device not found");
    return;
  }
  // LV_LOG_INFO("Button event code: %d", code);
  // LV_LOG_INFO("Key code: %c", key);

  if (opcode == 0) { // toggle
    if (lv_obj_has_state(obj, LV_STATE_PRESSED)) {
      lv_obj_remove_state(obj, LV_STATE_PRESSED);
    } else {
      lv_obj_add_state(obj, LV_STATE_PRESSED);
    }
  } else if (opcode == 1) { // switch on
    lv_obj_add_state(obj, LV_STATE_CHECKED);
  } else if (opcode == 2) { // switch off
    lv_obj_remove_state(obj, LV_STATE_CHECKED);
  }
}

static void value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * arc = lv_event_get_target_obj(e);
    lv_obj_t * label = (lv_obj_t *)lv_event_get_user_data(e);

    lv_label_set_text_fmt(label, "%" LV_PRId32 "%%", lv_arc_get_value(arc));

    /*Rotate the label to the current position of the arc*/
    // lv_arc_rotate_obj_to_angle(arc, label, 25);
}

static void timer_cb(lv_timer_t *timer)
{
  lv_obj_t *screen = (lv_obj_t *)lv_timer_get_user_data(timer);

  lv_indev_t *indev_btn = find_indev_by_type(MY_INDEV_TYPE_BUTTON);
  lv_indev_t *indev_encoder1 = find_indev_by_type(MY_INDEV_TYPE_ENCODER1);
  lv_indev_t *indev_encoder2 = find_indev_by_type(MY_INDEV_TYPE_ENCODER2);
  lv_indev_t *indev_joystick1 = find_indev_by_type(MY_INDEV_TYPE_JOYSTICK1);
  lv_indev_t *indev_joystick2 = find_indev_by_type(MY_INDEV_TYPE_JOYSTICK2);
  lv_indev_t *indev_battery = find_indev_by_type(MY_INDEV_TYPE_BATTERY);

  lv_obj_t *obj = NULL;
  lv_obj_t *child = NULL;
  lv_indev_data_t data;

  if (indev_btn) {
    lv_indev_get_read_cb(indev_btn)(indev_btn, &data);

    if (data.state == LV_INDEV_STATE_PRESSED) {
      UPDATE_KEY_STATE(w, 0);
      UPDATE_KEY_STATE(a, 1);
      UPDATE_KEY_STATE(d, 2);
      UPDATE_KEY_STATE(s, 3);
      UPDATE_KEY_STATE(i, 4);
      UPDATE_KEY_STATE(j, 5);
      UPDATE_KEY_STATE(l, 6);
      UPDATE_KEY_STATE(k, 7);

      UPDATE_SWITCH_STATE(1, 12);
      UPDATE_SWITCH_STATE(2, 13);
      UPDATE_SWITCH_STATE(3, 14);
      UPDATE_SWITCH_STATE(4, 15);

      if (data.key & (1 << 4)) { // i
        hal_nrf24_send_cmd(1, 1, 1, 1);
      } else if (data.key & (1 << 7)) { // k
        hal_nrf24_send_cmd(-1, -1, -1, -1);
      } else if (data.key & (1 << 5)) { // j
        hal_nrf24_send_cmd(-1, 1, 1, -1);
      } else if (data.key & (1 << 6)) { // l
        hal_nrf24_send_cmd(1, -1, -1, 1);
      } else if (data.key & (1 << 1)) { // a
        hal_nrf24_send_cmd(-1, 1, -1, 1);
      } else if (data.key & (1 << 2)) { // d
        hal_nrf24_send_cmd(1, -1, 1, -1);
      } else if (data.key & (1 << 3)) { // s
        hal_nrf24_send_cmd((data.key & (1 << 12)) ? 1 : -1,
                          (data.key & (1 << 13)) ? 1 : -1,
                          (data.key & (1 << 14)) ? 1 : -1,
                          (data.key & (1 << 15)) ? 1 : -1);
      } else {
        hal_nrf24_send_cmd(0, 0, 0, 0);
      }

      obj = lv_obj_find_by_name(screen, CO_NAME(j1)); // joystick
      if (obj) {
        obj = lv_obj_find_by_name(obj, "stick"); // stick
        if (obj) {
          if (data.key & (1 << 10)) {
            lv_obj_add_state(obj, LV_STATE_PRESSED);
          } else {
            lv_obj_remove_state(obj, LV_STATE_PRESSED);
          }
        }
      }

      obj = lv_obj_find_by_name(screen, CO_NAME(j2)); // joystick
      if (obj) {
        obj = lv_obj_find_by_name(obj, "stick"); // stick
        if (obj) {
          if (data.key & (1 << 11)) {
            lv_obj_add_state(obj, LV_STATE_PRESSED);
          } else {
            lv_obj_remove_state(obj, LV_STATE_PRESSED);
          }
        }
      }

      obj = lv_obj_find_by_name(screen, CO_NAME(z)); // arc
      if (obj) {
        obj = lv_obj_get_child_by_type(obj, 0, &lv_label_class); // label
        if (obj) {
          if (data.key & (1 << 8)) {
            lv_obj_set_style_text_color(obj, lv_color_make(200, 0, 0), 0);
          } else {
            lv_obj_set_style_text_color(obj, lv_color_white(), 0);
          }
        }
      }

      obj = lv_obj_find_by_name(screen, CO_NAME(x)); // arc
      if (obj) {
        obj = lv_obj_get_child_by_type(obj, 0, &lv_label_class); // label
        if (obj) {
          if (data.key & (1 << 9)) {
            lv_obj_set_style_text_color(obj, lv_color_make(200, 0, 0), 0);
          } else {
            lv_obj_set_style_text_color(obj, lv_color_white(), 0);
          }
        }
      }
    }
  }

  if (indev_encoder1) {
    lv_indev_get_read_cb(indev_encoder1)(indev_encoder1, &data);

    if (data.state == LV_INDEV_STATE_PRESSED) {
      obj = lv_obj_find_by_name(screen, CO_NAME(z));
      if (obj) {
        int32_t value = lv_arc_get_value(obj) + data.enc_diff;
        value = value < 0 ? 0 : value > 360 ? 360 : value;
        lv_arc_set_value(obj, value);
        lv_obj_send_event(obj, LV_EVENT_VALUE_CHANGED, NULL);
      }
    }
  }

  if (indev_encoder2) {
    lv_indev_get_read_cb(indev_encoder2)(indev_encoder2, &data);

    if (data.state == LV_INDEV_STATE_PRESSED) {
      obj = lv_obj_find_by_name(screen, CO_NAME(x));
      if (obj) {
        int32_t value = lv_arc_get_value(obj) + data.enc_diff;
        value = value < 0 ? 0 : value > 360 ? 360 : value;
        lv_arc_set_value(obj, value);
        lv_obj_send_event(obj, LV_EVENT_VALUE_CHANGED, NULL);
      }
    }
  }

  if (indev_joystick1) {
    lv_indev_get_read_cb(indev_joystick1)(indev_joystick1, &data);

    obj = lv_obj_find_by_name(screen, CO_NAME(j1)); // joystick
    if (obj) {
      int32_t x = data.point.x;
      int32_t y = data.point.y;

      child = lv_obj_get_child_by_type(obj, 0, &lv_label_class); // label
      if (child) {
        lv_label_set_text_fmt(child, "%d,%d", x, y);
      }

      child = lv_obj_find_by_name(obj, "stick"); // stick
      if (child) {
        if (x > 10 || x < -10 || y > 10 || y < -10) {
          // hal_nrf24_send_cmd(data.point.x, data.point.y, 0, 0);
        }

        x = my_map(x, -100, 100, -JOYSTICK_BOUNDARY, JOYSTICK_BOUNDARY);
        y = -my_map(y, -100, 100, -JOYSTICK_BOUNDARY, JOYSTICK_BOUNDARY);

        float distance_from_center = sqrt(x * x + y * y);
        if (distance_from_center < JOYSTICK_BOUNDARY) {
          lv_obj_set_pos(child, x, y);
        }
      }

#if ENABLE_JOYSTICK_TRAJECTORY
      child = lv_obj_find_by_name(obj, "trajectory"); // trajectory
      if (child) {
        // Draw trajectory within the base circle
        static lv_point_precise_t points[TRAJECTORY_MAX_POINTS] = {0};
        static uint16_t point_idx = 0;

        points[point_idx].x = x / 2;
        points[point_idx].y = y / 2;
        point_idx = (point_idx + 1) % TRAJECTORY_MAX_POINTS;

        lv_line_set_points(child, points, TRAJECTORY_MAX_POINTS);
      }
#endif
    }
  }

  if (indev_joystick2) {
    lv_indev_get_read_cb(indev_joystick2)(indev_joystick2, &data);

    obj = lv_obj_find_by_name(screen, CO_NAME(j2)); // joystick
    if (obj) {
      child = lv_obj_get_child_by_type(obj, 0, &lv_label_class); // label
      obj = lv_obj_find_by_name(obj, "stick"); // stick
      if (obj) {
        int32_t x = my_map(data.point.x, -100, 100, -JOYSTICK_BOUNDARY, JOYSTICK_BOUNDARY);
        int32_t y = -my_map(data.point.y, -100, 100, -JOYSTICK_BOUNDARY, JOYSTICK_BOUNDARY);

        float distance_from_center = sqrt(x * x + y * y);
        if (distance_from_center < JOYSTICK_BOUNDARY) {
          lv_obj_set_pos(obj, x, y);
        }

        if (child) {
          lv_label_set_text_fmt(child, "%d,%d", data.point.x, data.point.y);
        }
      }
    }
  }

  if (indev_battery) {
    lv_indev_get_read_cb(indev_battery)(indev_battery, &data);

    obj = lv_obj_find_by_name(screen, CO_NAME(bat)); // battery label
    if (obj) {
      lv_label_set_text_fmt(obj, "%d.%03dV", data.key / 1000, data.key % 1000);
    }
  }

}

lv_obj_t *dev_check(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);

  lv_obj_t *sw1 = lv_switch_create(screen);
  lv_obj_t *sw2 = lv_switch_create(screen);
  lv_obj_t *sw3 = lv_switch_create(screen);
  lv_obj_t *sw4 = lv_switch_create(screen);
  lv_obj_set_name(sw1, CO_NAME(1));
  lv_obj_set_name(sw2, CO_NAME(2));
  lv_obj_set_name(sw3, CO_NAME(3));
  lv_obj_set_name(sw4, CO_NAME(4));
  lv_obj_set_size(sw1, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_obj_set_size(sw2, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_obj_set_size(sw3, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_obj_set_size(sw4, SWITCH_WIDTH, SWITCH_HEIGHT);
  lv_switch_set_orientation(sw1, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(sw2, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(sw3, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(sw4, LV_SWITCH_ORIENTATION_VERTICAL);

  lv_obj_t *arc1 = lv_arc_create(screen);
  lv_obj_t *arc2 = lv_arc_create(screen);
  lv_obj_set_name(arc1, CO_NAME(z));
  lv_obj_set_name(arc2, CO_NAME(x));
  lv_obj_set_size(arc1, ARC_WIDTH, ARC_HEIGHT);
  lv_obj_set_size(arc2, ARC_WIDTH, ARC_HEIGHT);
  lv_arc_set_rotation(arc1, 270);
  lv_arc_set_rotation(arc2, 270);
  lv_arc_set_bg_angles(arc1, 0, 360);
  lv_arc_set_bg_angles(arc2, 0, 360);
  lv_obj_remove_style(arc1, NULL, LV_PART_KNOB);
  lv_obj_remove_style(arc2, NULL, LV_PART_KNOB);
  lv_obj_set_style_arc_width(arc1, 5, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc2, 5, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc1, 5, LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(arc2, 5, LV_PART_INDICATOR);

  lv_obj_t *label1 = lv_label_create(arc1);
  lv_obj_t *label2 = lv_label_create(arc2);
  lv_obj_set_style_text_color(label1, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_color(label2, lv_color_white(), LV_PART_MAIN);
  lv_obj_center(label1);
  lv_obj_center(label2);
  lv_obj_add_event_cb(arc1, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label1);
  lv_obj_add_event_cb(arc2, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label2);
  lv_arc_set_value(arc1, 50);
  lv_arc_set_value(arc2, 50);
  /*Manually update the label for the first time*/
  lv_obj_send_event(arc1, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_send_event(arc2, LV_EVENT_VALUE_CHANGED, NULL);

  static lv_style_t style_btn_pressed;
  lv_style_init(&style_btn_pressed);
  lv_style_set_bg_color(&style_btn_pressed, lv_palette_darken(LV_PALETTE_RED, 2));

  lv_obj_t *key1 = lv_button_create(screen);
  lv_obj_t *key2 = lv_button_create(screen);
  lv_obj_t *key3 = lv_button_create(screen);
  lv_obj_t *key4 = lv_button_create(screen);
  lv_obj_t *key5 = lv_button_create(screen);
  lv_obj_t *key6 = lv_button_create(screen);
  lv_obj_t *key7 = lv_button_create(screen);
  lv_obj_t *key8 = lv_button_create(screen);
  lv_obj_set_name(key1, CO_NAME(w));
  lv_obj_set_name(key2, CO_NAME(a));
  lv_obj_set_name(key3, CO_NAME(d));
  lv_obj_set_name(key4, CO_NAME(s));
  lv_obj_set_name(key5, CO_NAME(i));
  lv_obj_set_name(key6, CO_NAME(j));
  lv_obj_set_name(key7, CO_NAME(l));
  lv_obj_set_name(key8, CO_NAME(k));
  lv_obj_set_size(key1, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key2, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key3, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key4, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key5, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key6, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key7, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_size(key8, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_add_style(key1, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key2, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key3, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key4, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key5, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key6, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key7, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_style(key8, &style_btn_pressed, LV_STATE_PRESSED);

  lv_obj_align(sw1, LV_ALIGN_CENTER, -SWITCH_OFFSET_X, -SWITCH_OFFSET_Y);
  lv_obj_align(sw2, LV_ALIGN_CENTER, -SWITCH_OFFSET_X + SWITCH_WIDTH + SWITCH_GAP_X, -SWITCH_OFFSET_Y);
  lv_obj_align(sw3, LV_ALIGN_CENTER, SWITCH_OFFSET_X - SWITCH_WIDTH - SWITCH_GAP_X, -SWITCH_OFFSET_Y);
  lv_obj_align(sw4, LV_ALIGN_CENTER, SWITCH_OFFSET_X, -SWITCH_OFFSET_Y);

  lv_obj_align(arc1, LV_ALIGN_CENTER, -SWITCH_OFFSET_X + SWITCH_WIDTH*1.5 + SWITCH_GAP_X*2 + ARC_WIDTH*0.5, -SWITCH_OFFSET_Y);
  lv_obj_align(arc2, LV_ALIGN_CENTER, SWITCH_OFFSET_X - SWITCH_WIDTH*1.5 - SWITCH_GAP_X*2 - ARC_WIDTH*0.5, -SWITCH_OFFSET_Y);

  lv_obj_align(key1, LV_ALIGN_CENTER, -KEY_OFFSET_X, KEY_OFFSET_Y - KEY_GAP_Y);
  lv_obj_align(key2, LV_ALIGN_CENTER, -KEY_OFFSET_X - KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key3, LV_ALIGN_CENTER, -KEY_OFFSET_X + KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key4, LV_ALIGN_CENTER, -KEY_OFFSET_X, KEY_OFFSET_Y + KEY_GAP_Y);

  lv_obj_align(key5, LV_ALIGN_CENTER, KEY_OFFSET_X, KEY_OFFSET_Y - KEY_GAP_Y);
  lv_obj_align(key6, LV_ALIGN_CENTER, KEY_OFFSET_X - KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key7, LV_ALIGN_CENTER, KEY_OFFSET_X + KEY_GAP_X, KEY_OFFSET_Y);
  lv_obj_align(key8, LV_ALIGN_CENTER, KEY_OFFSET_X, KEY_OFFSET_Y + KEY_GAP_Y);

  lv_obj_t *joystick1 = create_joystick(screen, -JOYSTICK_OFFSET_X, JOYSTICK_OFFSET_Y);
  lv_obj_t *joystick2 = create_joystick(screen, JOYSTICK_OFFSET_X, JOYSTICK_OFFSET_Y);
  lv_obj_set_name(joystick1, CO_NAME(j1));
  lv_obj_set_name(joystick2, CO_NAME(j2));

  lv_obj_t *label_bat = lv_label_create(screen);
  lv_obj_set_name(label_bat, CO_NAME(bat));
  lv_label_set_text(label_bat, "4.200V");
  lv_obj_align(label_bat, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_text_color(label_bat, lv_color_white(), LV_PART_MAIN);


  /* --------------------------------------------------------------------------------------------------------------- */
  lv_indev_t *keypad = find_indev_by_type(LV_INDEV_TYPE_KEYPAD);
  if (keypad) {
    lv_group_t *group1 = lv_group_create();
    lv_indev_set_group(keypad, group1);
    lv_group_add_obj(group1, screen);
    lv_group_focus_obj(screen);
    lv_group_focus_freeze(group1, true);
    lv_obj_add_event_cb(screen, event_handler, LV_EVENT_KEY, NULL);
  } else {
    LV_LOG_USER("No keypad found");
  }

  lv_indev_t *encoder = find_indev_by_type(LV_INDEV_TYPE_ENCODER);
  if (encoder) {
    lv_group_t *group2 = lv_group_create();
    lv_indev_set_group(encoder, group2);
    lv_group_add_obj(group2, arc1);
    lv_group_add_obj(group2, arc2);
  } else {
    LV_LOG_USER("No encoder found");
  }

  timer = lv_timer_create(timer_cb, 100, screen);

  return screen;
}