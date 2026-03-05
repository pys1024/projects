#include "common.h"
#include "dev_check.h"
#include "lv_tools.h"
#include "main_menu.h"

#define ENABLE_JOYSTICK_TRAJECTORY 1
#define TRAJECTORY_MAX_POINTS    1000

#define CO_PREFIX "dev_"
#define CO_NAME(key) CO_PREFIX #key

#define SWITCH_WIDTH  (14)
#define SWITCH_HEIGHT (28)

#define SWITCH_OFFSET_X (104)
#define SWITCH_OFFSET_Y (102)
#define SWITCH_GAP_X    (8)

#define ARC_WIDTH  (36)
#define ARC_HEIGHT (36)

#define KEY_WIDTH    (22)
#define KEY_HEIGHT   (22)
#define KEY_OFFSET_X (72)
#define KEY_OFFSET_Y (78)
#define KEY_GAP_X    (30)
#define KEY_GAP_Y    (30)

#define JOYSTICK_BASE_RADIUS  (34)
#define JOYSTICK_STICK_RADIUS (8)
#define JOYSTICK_BOUNDARY ((int32_t)(JOYSTICK_BASE_RADIUS - (JOYSTICK_STICK_RADIUS * 1.0)))

#define JOYSTICK_OFFSET_X (60)
#define JOYSTICK_OFFSET_Y (-4)

#define SCREEN_PAD        (8)
#define TOP_BAR_HEIGHT    (26)
#define MIDLINE_Y_OFFSET  (20)

#define BAT_BODY_W        (28)
#define BAT_BODY_H        (14)
#define BAT_FILL_MAX_W    (22)

#define METEOR_COUNT      (6)

#define COLOR_BG_TOP      lv_color_hex(0x041621)
#define COLOR_BG_BOTTOM   lv_color_hex(0x0B293A)
#define COLOR_ACCENT      lv_color_hex(0x20C997)
#define COLOR_ACCENT_SOFT lv_color_hex(0x2A9D8F)
#define COLOR_DANGER      lv_color_hex(0xE63946)

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

typedef struct {
  lv_obj_t *line_core;
  lv_obj_t *line_glow;
  lv_obj_t *head;
  lv_point_precise_t pts[2];
  int16_t x;
  int16_t y;
  int8_t vx;
  int8_t vy;
  uint8_t len;
  uint8_t life;
  bool active;
} meteor_t;

static meteor_t meteors[METEOR_COUNT] = {0};

static void meteor_reset(meteor_t *m)
{
  m->active = false;
  lv_obj_add_flag(m->line_core, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(m->line_glow, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(m->head, LV_OBJ_FLAG_HIDDEN);
}

static void meteor_spawn(meteor_t *m)
{
  m->active = true;
  m->x = (int16_t)(rand() % LCD_WIDTH);
  m->y = (int16_t)(TOP_BAR_HEIGHT + 4 + (rand() % (LCD_HEIGHT / 2)));
  m->vx = (int8_t)(4 + (rand() % 3));
  m->vy = (int8_t)(2 + (rand() % 3));
  m->len = (uint8_t)(8 + (rand() % 8));
  m->life = (uint8_t)(18 + (rand() % 14));

  lv_obj_clear_flag(m->line_core, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(m->line_glow, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(m->head, LV_OBJ_FLAG_HIDDEN);
}

static void update_meteors(void)
{
  for (uint8_t i = 0; i < METEOR_COUNT; i++) {
    meteor_t *m = &meteors[i];

    if (!m->active) {
      if ((rand() % 100) < 14) {
        meteor_spawn(m);
      }
      continue;
    }

    m->x += m->vx;
    m->y += m->vy;

    if (m->life > 0) {
      m->life--;
    }

    if (m->x - m->len > LCD_WIDTH || m->y - m->len > LCD_HEIGHT || m->life == 0) {
      meteor_reset(m);
      continue;
    }

    m->pts[0].x = m->x;
    m->pts[0].y = m->y;
    m->pts[1].x = m->x - m->len;
    m->pts[1].y = m->y - m->len;
    lv_line_set_points(m->line_core, m->pts, 2);
    lv_line_set_points(m->line_glow, m->pts, 2);

    lv_opa_t core_opa = (lv_opa_t)my_map(m->life, 0, 32, LV_OPA_10, LV_OPA_100);
    lv_opa_t glow_opa = (lv_opa_t)my_map(m->life, 0, 32, LV_OPA_10, LV_OPA_40);
    lv_obj_set_style_line_opa(m->line_core, core_opa, LV_PART_MAIN);
    lv_obj_set_style_line_opa(m->line_glow, glow_opa, LV_PART_MAIN);

    lv_obj_set_pos(m->head, m->x - 2, m->y - 2);
    lv_obj_set_style_opa(m->head, core_opa, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(m->head, glow_opa, LV_PART_MAIN);
  }
}

static void dev_check_back_event_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  lv_obj_t *menu = main_menu_screen();
  lv_screen_load_anim(menu, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 220, 0, true);
}

static void dev_check_delete_event_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (timer) {
    lv_timer_delete(timer);
    timer = NULL;
  }
}

static void anim_set_y_cb(void *obj, int32_t v)
{
  lv_obj_set_y((lv_obj_t *)obj, (lv_coord_t)v);
}

static void anim_set_shadow_width_cb(void *obj, int32_t v)
{
  lv_obj_set_style_shadow_width((lv_obj_t *)obj, (int32_t)v, LV_PART_MAIN);
}

static void play_intro_anim(lv_obj_t *obj, int16_t y_offset, uint32_t delay)
{
  lv_coord_t y = lv_obj_get_y(obj);
  lv_obj_set_y(obj, y + y_offset);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_exec_cb(&a, anim_set_y_cb);
  lv_anim_set_values(&a, y + y_offset, y);
  lv_anim_set_time(&a, 360);
  lv_anim_set_delay(&a, delay);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
  lv_anim_start(&a);
}

static void play_glow_anim(lv_obj_t *obj, int32_t min_w, int32_t max_w, uint32_t duration, uint32_t delay)
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_exec_cb(&a, anim_set_shadow_width_cb);
  lv_anim_set_values(&a, min_w, max_w);
  lv_anim_set_time(&a, duration);
  lv_anim_set_delay(&a, delay);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
  lv_anim_set_playback_time(&a, duration);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a);
}

static lv_obj_t *create_key_button(lv_obj_t *parent, const char *name, const char *text)
{
  static lv_style_t style_btn_pressed;
  static bool style_inited = false;
  if (!style_inited) {
    lv_style_init(&style_btn_pressed);
    lv_style_set_bg_color(&style_btn_pressed, COLOR_DANGER);
    lv_style_set_shadow_width(&style_btn_pressed, 12);
    lv_style_set_shadow_color(&style_btn_pressed, COLOR_DANGER);
    style_inited = true;
  }

  lv_obj_t *btn = lv_button_create(parent);
  lv_obj_set_name(btn, name);
  lv_obj_set_size(btn, KEY_WIDTH, KEY_HEIGHT);
  lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0x19455A), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(btn, lv_color_hex(0x1F5B73), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_color(btn, COLOR_ACCENT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_border_width(btn, 1, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(btn, 8, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(btn, lv_color_hex(0x061A26), LV_PART_MAIN);
  lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN);
  lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);

  if (text != NULL && text[0] != '\0') {
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(label);
  }

  return btn;
}

static lv_obj_t *create_joystick(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
  static lv_style_t style_stick_pressed;
  static bool style_inited = false;
  if (!style_inited) {
    lv_style_init(&style_stick_pressed);
    lv_style_set_bg_color(&style_stick_pressed, lv_palette_darken(LV_PALETTE_RED, 2));
    lv_style_set_shadow_width(&style_stick_pressed, 10);
    lv_style_set_shadow_color(&style_stick_pressed, lv_palette_main(LV_PALETTE_RED));
    style_inited = true;
  }

  lv_obj_t *base = lv_obj_create(parent);
  lv_obj_set_size(base, JOYSTICK_BASE_RADIUS * 2, JOYSTICK_BASE_RADIUS * 2);
  lv_obj_set_pos(base, x, y);
  lv_obj_set_style_radius(base, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(base, lv_color_hex(0x0D2331), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(base, lv_color_hex(0x1A3A4D), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(base, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_color(base, COLOR_ACCENT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_border_width(base, 2, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(base, 14, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(base, lv_color_hex(0x061723), LV_PART_MAIN);
  lv_obj_set_style_pad_all(base, 0, LV_PART_MAIN);
  lv_obj_remove_flag(base, LV_OBJ_FLAG_SCROLLABLE);

  // Add horizontal line
  lv_obj_t *h_line = lv_line_create(base);
  static const lv_point_precise_t h_points[] = {{0, JOYSTICK_BASE_RADIUS}, {JOYSTICK_BASE_RADIUS*2, JOYSTICK_BASE_RADIUS}};
  lv_line_set_points(h_line, h_points, 2);
  lv_obj_set_style_line_color(h_line, COLOR_ACCENT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_line_width(h_line, 1, LV_PART_MAIN);
  lv_obj_set_style_line_dash_gap(h_line, 5, LV_PART_MAIN);
  lv_obj_set_style_line_dash_width(h_line, 5, LV_PART_MAIN);

  // Add vertical line
  lv_obj_t *v_line = lv_line_create(base);
  static const lv_point_precise_t v_points[] = {{JOYSTICK_BASE_RADIUS, 0}, {JOYSTICK_BASE_RADIUS, JOYSTICK_BASE_RADIUS*2}};
  lv_line_set_points(v_line, v_points, 2);
  lv_obj_set_style_line_color(v_line, COLOR_ACCENT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_line_width(v_line, 1, LV_PART_MAIN);
  lv_obj_set_style_line_dash_gap(v_line, 5, LV_PART_MAIN);
  lv_obj_set_style_line_dash_width(v_line, 5, LV_PART_MAIN);

  lv_obj_t *stick = lv_button_create(base);
  lv_obj_set_name(stick, "stick");
  lv_obj_set_size(stick, JOYSTICK_STICK_RADIUS * 2, JOYSTICK_STICK_RADIUS * 2);
  lv_obj_center(stick);
  lv_obj_set_style_radius(stick, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(stick, lv_color_hex(0x4CC9F0), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(stick, lv_color_hex(0x3A86FF), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(stick, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(stick, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(stick, 8, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(stick, lv_color_hex(0x3A86FF), LV_PART_MAIN);
  lv_obj_set_style_pad_all(stick, 0, LV_PART_MAIN);
  lv_obj_add_style(stick, &style_stick_pressed, LV_STATE_PRESSED);

  lv_obj_t *label = lv_label_create(base);
  lv_label_set_text(label, "0,0");
  lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_center(label);

#if ENABLE_JOYSTICK_TRAJECTORY
  lv_obj_t *trajectory = lv_line_create(base);
  lv_obj_set_style_line_color(trajectory, lv_color_hex(0xF94144), LV_PART_MAIN);
  lv_obj_set_style_line_opa(trajectory, LV_OPA_70, LV_PART_MAIN);
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

static void joystick_process(const lv_obj_t *obj, const lv_indev_data_t *data, uint8_t joystick_idx)
{
#if ENABLE_JOYSTICK_TRAJECTORY
  static lv_point_precise_t points[2][TRAJECTORY_MAX_POINTS] = {0};
  static uint16_t points_idx[2] = {0};
#endif
  lv_obj_t *child = NULL;

  int32_t x = data->point.x;
  int32_t y = data->point.y;

  LV_UNUSED(joystick_idx);

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
    } else {
      float ratio = JOYSTICK_BOUNDARY / distance_from_center;
      lv_obj_set_pos(child, x * ratio, y * ratio);
    }
  }

#if ENABLE_JOYSTICK_TRAJECTORY
  child = lv_obj_find_by_name(obj, "trajectory"); // trajectory
  if (child) {
    // Draw trajectory within the base circle
    for (uint16_t i = 0; i < TRAJECTORY_MAX_POINTS; i++) {
      if (points[joystick_idx][i].x == 0 && points[joystick_idx][i].y == 0) {
        points[joystick_idx][i].x = JOYSTICK_BASE_RADIUS;
        points[joystick_idx][i].y = JOYSTICK_BASE_RADIUS;
      }
    }

    points[joystick_idx][points_idx[joystick_idx]].x = x + JOYSTICK_BASE_RADIUS;
    points[joystick_idx][points_idx[joystick_idx]].y = y + JOYSTICK_BASE_RADIUS;
    points_idx[joystick_idx] = (points_idx[joystick_idx] + 1) % TRAJECTORY_MAX_POINTS;

    lv_line_set_points(child, points[joystick_idx], TRAJECTORY_MAX_POINTS);
  }
#endif
}

static void timer_cb(lv_timer_t *timer)
{
  lv_obj_t *screen = (lv_obj_t *)lv_timer_get_user_data(timer);
  update_meteors();

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

      if (data.key & (1 << 1)) { // a
        hal_nrf24_send_cmd(-1, 1, -1, 1);
      } else if (data.key & (1 << 2)) { // d
        hal_nrf24_send_cmd(1, -1, 1, -1);
      } else if (data.key & (1 << 3)) { // s
        hal_nrf24_send_cmd((data.key & (1 << 12)) ? 1 : -1,
                          (data.key & (1 << 13)) ? 1 : -1,
                          (data.key & (1 << 14)) ? 1 : -1,
                          (data.key & (1 << 15)) ? 1 : -1);
      } else if (data.key & (1 << 4)) { // i
        hal_nrf24_send_cmd(1, 1, 1, 1);
      } else if (data.key & (1 << 7)) { // k
        hal_nrf24_send_cmd(-1, -1, -1, -1);
      } else if (data.key & (1 << 5)) { // j
        hal_nrf24_send_cmd(-1, 1, 1, -1);
      } else if (data.key & (1 << 6)) { // l
        hal_nrf24_send_cmd(1, -1, -1, 1);
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
      joystick_process(obj, &data, 0);
    }
  }

  if (indev_joystick2) {
    lv_indev_get_read_cb(indev_joystick2)(indev_joystick2, &data);
    obj = lv_obj_find_by_name(screen, CO_NAME(j2)); // joystick
    if (obj) {
      joystick_process(obj, &data, 1);
    }
  }

  if (indev_battery) {
    lv_indev_get_read_cb(indev_battery)(indev_battery, &data);

    int32_t mv = (int32_t)data.key;
    int32_t pct = my_map(mv, 3300, 4200, 0, 100);
    pct = pct < 0 ? 0 : pct > 100 ? 100 : pct;

    obj = lv_obj_find_by_name(screen, CO_NAME(bat_fill));
    if (obj) {
      uint32_t fill_w = (uint32_t)my_map(pct, 0, 100, 2, BAT_FILL_MAX_W);
      lv_obj_set_width(obj, fill_w);

      lv_color_t fill_color = lv_color_hex(0x2ED573);
      if (pct < 20) {
        fill_color = COLOR_DANGER;
      } else if (pct < 50) {
        fill_color = lv_color_hex(0xF4A261);
      }
      lv_obj_set_style_bg_color(obj, fill_color, LV_PART_MAIN);

      if (pct < 20) {
        static bool blink = false;
        blink = !blink;
        lv_obj_set_style_opa(obj, blink ? LV_OPA_40 : LV_OPA_100, LV_PART_MAIN);
      } else {
        lv_obj_set_style_opa(obj, LV_OPA_100, LV_PART_MAIN);
      }
    }

    obj = lv_obj_find_by_name(screen, CO_NAME(bat_text));
    if (obj) {
      lv_label_set_text_fmt(obj, "%d%%", (int)pct);
    }
  }

}

lv_obj_t *dev_check(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  lv_obj_set_size(screen, LCD_WIDTH, LCD_HEIGHT);
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_bg_color(screen, COLOR_BG_TOP, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(screen, COLOR_BG_BOTTOM, LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(screen, SCREEN_PAD, LV_PART_MAIN);
  lv_obj_add_event_cb(screen, dev_check_delete_event_cb, LV_EVENT_DELETE, NULL);

  lv_obj_t *top_bar = lv_obj_create(screen);
  lv_obj_set_size(top_bar, LCD_WIDTH - SCREEN_PAD * 2, TOP_BAR_HEIGHT);
  lv_obj_align(top_bar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(top_bar, lv_color_hex(0x0D3448), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(top_bar, lv_color_hex(0x145571), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(top_bar, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_border_width(top_bar, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(top_bar, COLOR_ACCENT_SOFT, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(top_bar, 8, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(top_bar, lv_color_hex(0x05121B), LV_PART_MAIN);
  lv_obj_set_style_radius(top_bar, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_all(top_bar, 6, LV_PART_MAIN);

  for (uint8_t i = 0; i < METEOR_COUNT; i++) {
    lv_obj_t *line_glow = lv_line_create(screen);
    lv_obj_set_style_line_color(line_glow, lv_color_hex(0x80D8FF), LV_PART_MAIN);
    lv_obj_set_style_line_width(line_glow, 3, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(line_glow, true, LV_PART_MAIN);
    lv_obj_set_style_line_opa(line_glow, LV_OPA_0, LV_PART_MAIN);
    lv_obj_remove_flag(line_glow, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_move_background(line_glow);

    lv_obj_t *line_core = lv_line_create(screen);
    lv_obj_set_style_line_color(line_core, lv_color_hex(0xE8F9FF), LV_PART_MAIN);
    lv_obj_set_style_line_width(line_core, 1, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(line_core, true, LV_PART_MAIN);
    lv_obj_set_style_line_opa(line_core, LV_OPA_0, LV_PART_MAIN);
    lv_obj_remove_flag(line_core, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_move_background(line_core);

    lv_obj_t *head = lv_obj_create(screen);
    lv_obj_set_size(head, 4, 4);
    lv_obj_set_style_radius(head, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_border_width(head, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(head, lv_color_hex(0xE8F9FF), LV_PART_MAIN);
    lv_obj_set_style_shadow_width(head, 8, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(head, lv_color_hex(0x80D8FF), LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(head, LV_OPA_30, LV_PART_MAIN);
    lv_obj_set_style_pad_all(head, 0, LV_PART_MAIN);
    lv_obj_remove_flag(head, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_move_background(head);

    meteors[i].line_core = line_core;
    meteors[i].line_glow = line_glow;
    meteors[i].head = head;
    meteors[i].pts[0].x = 0;
    meteors[i].pts[0].y = 0;
    meteors[i].pts[1].x = 0;
    meteors[i].pts[1].y = 0;
    lv_line_set_points(line_core, meteors[i].pts, 2);
    lv_line_set_points(line_glow, meteors[i].pts, 2);
    meteor_reset(&meteors[i]);
  }

  lv_obj_t *title = lv_label_create(top_bar);
  lv_label_set_text(title, "DEV CHECK");
  lv_obj_set_style_text_color(title, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 30, 0);

  lv_obj_t *back = lv_button_create(top_bar);
  lv_obj_set_size(back, 24, 18);
  lv_obj_align(back, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_set_style_radius(back, 6, LV_PART_MAIN);
  lv_obj_set_style_bg_color(back, lv_color_hex(0x1A627D), LV_PART_MAIN);
  lv_obj_add_event_cb(back, dev_check_back_event_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *back_text = lv_label_create(back);
  lv_label_set_text(back_text, "<");
  lv_obj_set_style_text_color(back_text, lv_color_white(), LV_PART_MAIN);
  lv_obj_center(back_text);

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
  lv_obj_set_style_bg_color(sw1, lv_color_hex(0x184055), LV_PART_MAIN);
  lv_obj_set_style_bg_color(sw2, lv_color_hex(0x184055), LV_PART_MAIN);
  lv_obj_set_style_bg_color(sw3, lv_color_hex(0x184055), LV_PART_MAIN);
  lv_obj_set_style_bg_color(sw4, lv_color_hex(0x184055), LV_PART_MAIN);
  lv_obj_set_style_bg_color(sw1, COLOR_ACCENT, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_bg_color(sw2, COLOR_ACCENT, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_bg_color(sw3, COLOR_ACCENT, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_bg_color(sw4, COLOR_ACCENT, LV_PART_INDICATOR | LV_STATE_CHECKED);

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
  lv_obj_set_style_arc_color(arc1, lv_color_hex(0x1C4254), LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc2, lv_color_hex(0x1C4254), LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc1, lv_color_hex(0xFF7F11), LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc2, lv_color_hex(0xFFB703), LV_PART_INDICATOR);

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
  lv_obj_send_event(arc1, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_send_event(arc2, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_t *key1 = create_key_button(screen, CO_NAME(w), "");
  lv_obj_t *key2 = create_key_button(screen, CO_NAME(a), "");
  lv_obj_t *key3 = create_key_button(screen, CO_NAME(d), "");
  lv_obj_t *key4 = create_key_button(screen, CO_NAME(s), "");
  lv_obj_t *key5 = create_key_button(screen, CO_NAME(i), "");
  lv_obj_t *key6 = create_key_button(screen, CO_NAME(j), "");
  lv_obj_t *key7 = create_key_button(screen, CO_NAME(l), "");
  lv_obj_t *key8 = create_key_button(screen, CO_NAME(k), "");

  lv_obj_set_pos(sw1, 10, 40);
  lv_obj_set_pos(sw2, 28, 40);
  lv_obj_set_pos(sw3, 186, 40);
  lv_obj_set_pos(sw4, 204, 40);

  lv_obj_set_pos(arc1, 58, 36);
  lv_obj_set_pos(arc2, 130, 36);

  lv_obj_t *joystick1 = create_joystick(screen, 18, 96);
  lv_obj_t *joystick2 = create_joystick(screen, 138, 96);
  lv_obj_set_name(joystick1, CO_NAME(j1));
  lv_obj_set_name(joystick2, CO_NAME(j2));

  lv_obj_set_pos(key1, 41, 177);
  lv_obj_set_pos(key2, 15, 203);
  lv_obj_set_pos(key3, 67, 203);
  lv_obj_set_pos(key4, 41, 229);

  lv_obj_set_pos(key5, 161, 177);
  lv_obj_set_pos(key6, 135, 203);
  lv_obj_set_pos(key7, 187, 203);
  lv_obj_set_pos(key8, 161, 229);

  lv_obj_move_foreground(key1);
  lv_obj_move_foreground(key2);
  lv_obj_move_foreground(key3);
  lv_obj_move_foreground(key4);
  lv_obj_move_foreground(key5);
  lv_obj_move_foreground(key6);
  lv_obj_move_foreground(key7);
  lv_obj_move_foreground(key8);

  lv_obj_t *bat_body = lv_obj_create(top_bar);
  lv_obj_set_name(bat_body, CO_NAME(bat));
  lv_obj_set_size(bat_body, BAT_BODY_W, BAT_BODY_H);
  lv_obj_align(bat_body, LV_ALIGN_RIGHT_MID, -8, 0);
  lv_obj_set_style_radius(bat_body, 3, LV_PART_MAIN);
  lv_obj_set_style_border_width(bat_body, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(bat_body, lv_color_hex(0xC4F1FF), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(bat_body, LV_OPA_0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(bat_body, 1, LV_PART_MAIN);

  lv_obj_t *bat_tip = lv_obj_create(top_bar);
  lv_obj_set_size(bat_tip, 3, 8);
  lv_obj_align_to(bat_tip, bat_body, LV_ALIGN_OUT_RIGHT_MID, 1, 0);
  lv_obj_set_style_radius(bat_tip, 1, LV_PART_MAIN);
  lv_obj_set_style_border_width(bat_tip, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(bat_tip, lv_color_hex(0xC4F1FF), LV_PART_MAIN);
  lv_obj_set_style_pad_all(bat_tip, 0, LV_PART_MAIN);

  lv_obj_t *bat_fill = lv_obj_create(bat_body);
  lv_obj_set_name(bat_fill, CO_NAME(bat_fill));
  lv_obj_set_size(bat_fill, BAT_FILL_MAX_W, BAT_BODY_H - 4);
  lv_obj_align(bat_fill, LV_ALIGN_LEFT_MID, 1, 0);
  lv_obj_set_style_radius(bat_fill, 2, LV_PART_MAIN);
  lv_obj_set_style_border_width(bat_fill, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(bat_fill, lv_color_hex(0x2ED573), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(bat_fill, lv_color_hex(0x6DFF9A), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(bat_fill, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_pad_all(bat_fill, 0, LV_PART_MAIN);

  lv_obj_t *bat_text = lv_label_create(top_bar);
  lv_obj_set_name(bat_text, CO_NAME(bat_text));
  lv_label_set_text(bat_text, "100%");
  lv_obj_set_style_text_color(bat_text, lv_color_hex(0x9AF7C2), LV_PART_MAIN);
  lv_obj_set_style_text_opa(bat_text, LV_OPA_90, LV_PART_MAIN);
  lv_obj_align_to(bat_text, bat_body, LV_ALIGN_OUT_LEFT_MID, -5, 0);


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

  play_intro_anim(top_bar, -10, 0);
  play_intro_anim(sw1, -8, 40);
  play_intro_anim(sw2, -8, 60);
  play_intro_anim(sw3, -8, 80);
  play_intro_anim(sw4, -8, 100);
  play_intro_anim(arc1, -8, 90);
  play_intro_anim(arc2, -8, 120);
  play_intro_anim(joystick1, 12, 150);
  play_intro_anim(joystick2, 12, 180);
  play_intro_anim(key1, 16, 220);
  play_intro_anim(key2, 16, 240);
  play_intro_anim(key3, 16, 260);
  play_intro_anim(key4, 16, 280);
  play_intro_anim(key5, 16, 240);
  play_intro_anim(key6, 16, 260);
  play_intro_anim(key7, 16, 280);
  play_intro_anim(key8, 16, 300);

  play_glow_anim(joystick1, 8, 16, 1200, 0);
  play_glow_anim(joystick2, 8, 16, 1200, 180);
  play_glow_anim(arc1, 0, 8, 1400, 200);
  play_glow_anim(arc2, 0, 8, 1400, 420);

  return screen;
}
