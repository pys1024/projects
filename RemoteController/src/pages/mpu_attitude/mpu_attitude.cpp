#include "common.h"

#include "app_navigation.h"
#include "mpu_attitude.h"

#define DIAL_TICK_COUNT        12
#define DIAL_NUMBER_COUNT      6
#define ATT_TOP_HIT_H          36
#define DIAL_CARD_W            74
#define DIAL_CARD_H            104
#define DIAL_FACE_SIZE         56
#define DIAL_CENTER            (DIAL_FACE_SIZE / 2)
#define DIAL_NEEDLE_FRONT_LEN  20
#define DIAL_NEEDLE_BACK_LEN   9
#define DIAL_TICK_OUTER_RADIUS 25
#define DIAL_TICK_MAJOR_INNER  19
#define DIAL_TICK_MINOR_INNER  22
#define DIAL_NUMBER_RADIUS     16

typedef enum {
  ATT_THEME_TECH = 0,
  ATT_THEME_MECH = 1,
} att_theme_t;

typedef struct {
  lv_obj_t *card;
  lv_obj_t *title;
  lv_obj_t *face;
  lv_obj_t *value;
  lv_obj_t *needle_front;
  lv_obj_t *needle_back;
  lv_obj_t *hub;
  lv_obj_t *ticks[DIAL_TICK_COUNT];
  lv_obj_t *numbers[DIAL_NUMBER_COUNT];
  lv_point_precise_t tick_pts[DIAL_TICK_COUNT][2];
  const char *unit;
  uint8_t decimals;
  float min;
  float max;
  bool wrap_pointer;
  float val;
} dial_t;

static lv_timer_t *att_timer = NULL;
static dial_t dials[6] = {0};
static float att_deg[3] = {0};
static float vel_mps[3] = {0};
static float pos_cm[3] = {0};
static float accel_bias[3] = {0};
static uint16_t accel_bias_n = 0;
static uint32_t last_ms = 0;
static att_theme_t att_theme = ATT_THEME_TECH;
static lv_obj_t *att_screen_obj = NULL;
static lv_obj_t *att_top_obj = NULL;
static lv_obj_t *att_title_obj = NULL;
static lv_obj_t *att_theme_tag = NULL;

static void apply_theme(void);

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

static void add_att_bg_fx(lv_obj_t *screen)
{
  static const lv_point_precise_t fx_line1[] = {{14, 232}, {84, 162}};
  static const lv_point_precise_t fx_line2[] = {{226, 86}, {166, 146}};

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

static void att_delete_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (att_timer) {
    lv_timer_delete(att_timer);
    att_timer = NULL;
  }
  att_screen_obj = NULL;
  att_top_obj = NULL;
  att_title_obj = NULL;
  att_theme_tag = NULL;
}

static float wrap_deg(float v)
{
  while (v > 180.0f) v -= 360.0f;
  while (v < -180.0f) v += 360.0f;
  return v;
}

static float deg_to_rad(float v)
{
  return v * 3.1415926f / 180.0f;
}

static void calc_gravity_body(float roll_deg, float pitch_deg, float out[3])
{
  const float g = 9.80665f;
  float roll = deg_to_rad(roll_deg);
  float pitch = deg_to_rad(pitch_deg);

  out[0] = -sinf(pitch) * g;
  out[1] = sinf(roll) * cosf(pitch) * g;
  out[2] = cosf(roll) * cosf(pitch) * g;
}

static lv_color_t theme_screen_top(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x08131F) : lv_color_hex(0x17120D);
}

static lv_color_t theme_screen_bottom(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x123149) : lv_color_hex(0x2B2117);
}

static lv_color_t theme_top_bg(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x14354E) : lv_color_hex(0x3A2C1E);
}

static lv_color_t theme_top_bg_grad(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x1E4E6E) : lv_color_hex(0x5B4631);
}

static lv_color_t theme_top_border(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x6FD5FF) : lv_color_hex(0xD6B98E);
}

static lv_color_t theme_card_bg(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x112A3D) : lv_color_hex(0x2A2118);
}

static lv_color_t theme_card_bg_grad(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x1A405B) : lv_color_hex(0x3D3023);
}

static lv_color_t theme_face_bg(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x0D2332) : lv_color_hex(0x17120D);
}

static lv_color_t theme_face_bg_grad(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0x16374E) : lv_color_hex(0x2A2118);
}

static lv_color_t theme_title_color(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0xD9F5FF) : lv_color_hex(0xF6E7CD);
}

static lv_color_t theme_value_color(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? lv_color_hex(0xF2FBFF) : lv_color_hex(0xFFEED1);
}

static const char *theme_name(att_theme_t theme)
{
  return (theme == ATT_THEME_TECH) ? "TECH" : "MECH";
}

static lv_color_t dial_ring_color(att_theme_t theme, uint8_t idx)
{
  if (theme == ATT_THEME_TECH) {
    switch (idx) {
      case 0: return lv_color_hex(0x58D5FF);
      case 1: return lv_color_hex(0x6EE7A8);
      case 2: return lv_color_hex(0xFFC36E);
      case 3: return lv_color_hex(0x7EA6FF);
      case 4: return lv_color_hex(0x9CE86D);
      default: return lv_color_hex(0xFF9B7A);
    }
  }

  switch (idx) {
    case 0: return lv_color_hex(0xD8C38F);
    case 1: return lv_color_hex(0xCFAF76);
    case 2: return lv_color_hex(0xE0A66A);
    case 3: return lv_color_hex(0xA7B0B8);
    case 4: return lv_color_hex(0xC7D0B8);
    default: return lv_color_hex(0xD59D84);
  }
}

static lv_color_t dial_needle_color(att_theme_t theme, uint8_t idx)
{
  if (theme == ATT_THEME_TECH) {
    switch (idx) {
      case 0: return lv_color_hex(0x82E3FF);
      case 1: return lv_color_hex(0xA0F4C3);
      case 2: return lv_color_hex(0xFFD89B);
      case 3: return lv_color_hex(0xB4C8FF);
      case 4: return lv_color_hex(0xBDF49A);
      default: return lv_color_hex(0xFFC2AF);
    }
  }

  switch (idx) {
    case 0: return lv_color_hex(0xFFE4A6);
    case 1: return lv_color_hex(0xFFD38A);
    case 2: return lv_color_hex(0xFFBE7A);
    case 3: return lv_color_hex(0xE1E8EF);
    case 4: return lv_color_hex(0xE2F1C2);
    default: return lv_color_hex(0xFFC9B0);
  }
}

static void format_value(char *buf, size_t size, float value, const char *unit, uint8_t decimals)
{
  if (decimals == 0) {
    int32_t iv = value >= 0.0f ? (int32_t)(value + 0.5f) : (int32_t)(value - 0.5f);
    snprintf(buf, size, "%d%s", (int)iv, unit);
    return;
  }

  int32_t tenths = value >= 0.0f ? (int32_t)(value * 10.0f + 0.5f) : (int32_t)(value * 10.0f - 0.5f);
  int32_t abs_tenths = tenths >= 0 ? tenths : -tenths;
  snprintf(buf, size, "%s%d.%d%s", tenths < 0 ? "-" : "", abs_tenths / 10, abs_tenths % 10, unit);
}

static float wrap_360(float v)
{
  while (v < 0.0f) v += 360.0f;
  while (v >= 360.0f) v -= 360.0f;
  return v;
}

static void dial_set(dial_t *d, float pointer_value, float display_value)
{
  d->val = display_value;

  float span = d->max - d->min;
  if (span <= 0.001f) {
    return;
  }

  float v = pointer_value;
  if (d->wrap_pointer) {
    while (v < d->min) v += span;
    while (v >= d->max) v -= span;
  } else {
    if (v < d->min) v = d->min;
    if (v > d->max) v = d->max;
  }

  float t = (v - d->min) / span;
  int32_t rot10 = (int32_t)((-90.0f + t * 360.0f) * 10.0f);
  while (rot10 > 1800) rot10 -= 3600;
  while (rot10 < -1800) rot10 += 3600;

  if (d->needle_front) {
    lv_obj_set_style_transform_rotation(d->needle_front, rot10, LV_PART_MAIN);
  }
  if (d->needle_back) {
    lv_obj_set_style_transform_rotation(d->needle_back, rot10, LV_PART_MAIN);
  }

  char value_text[16];
  format_value(value_text, sizeof(value_text), display_value, d->unit, d->decimals);
  if (d->value) {
    lv_label_set_text(d->value, value_text);
  }
}

static lv_obj_t *create_dial(lv_obj_t *parent, uint8_t idx, const char *title, const char *unit, uint8_t decimals,
                             float scale_min, float scale_max, bool wrap_pointer, int16_t x, int16_t y, dial_t *d)
{
  lv_color_t ring_color = dial_ring_color(att_theme, idx);
  lv_color_t needle_color = dial_needle_color(att_theme, idx);
  lv_obj_t *needle_back = NULL;
  lv_obj_t *needle_front = NULL;
  lv_obj_t *hub = NULL;

  lv_obj_t *card = lv_obj_create(parent);
  if (!card) {
    return NULL;
  }
  lv_obj_set_size(card, DIAL_CARD_W, DIAL_CARD_H);
  lv_obj_set_pos(card, x, y);
  lv_obj_set_style_radius(card, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, theme_card_bg(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(card, theme_card_bg_grad(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, ring_color, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(card, 4, LV_PART_MAIN);
  lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *t = lv_label_create(card);
  if (t) {
    lv_label_set_text(t, title);
    lv_obj_set_style_text_font(t, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(t, theme_title_color(att_theme), LV_PART_MAIN);
    lv_obj_set_style_text_opa(t, LV_OPA_90, LV_PART_MAIN);
    lv_obj_align(t, LV_ALIGN_TOP_MID, 0, 2);
  }

  lv_obj_t *g = lv_obj_create(card);
  if (g) {
    lv_obj_set_size(g, DIAL_FACE_SIZE, DIAL_FACE_SIZE);
    lv_obj_align(g, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_radius(g, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(g, theme_face_bg(att_theme), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(g, theme_face_bg_grad(att_theme), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(g, LV_GRAD_DIR_VER, LV_PART_MAIN);
    lv_obj_set_style_border_color(g, ring_color, LV_PART_MAIN);
    lv_obj_set_style_border_width(g, 2, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(g, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(g, 0, LV_PART_MAIN);
    lv_obj_remove_flag(g, LV_OBJ_FLAG_SCROLLABLE);

    for (uint8_t i = 0; i < DIAL_TICK_COUNT; i++) {
      bool is_major = ((i % 3) == 0);
      int16_t in_r = is_major ? DIAL_TICK_MAJOR_INNER : DIAL_TICK_MINOR_INNER;
      float ang = deg_to_rad(-90.0f + (360.0f * i) / DIAL_TICK_COUNT);

      d->tick_pts[i][0].x = DIAL_CENTER + (int16_t)(cosf(ang) * in_r);
      d->tick_pts[i][0].y = DIAL_CENTER + (int16_t)(sinf(ang) * in_r);
      d->tick_pts[i][1].x = DIAL_CENTER + (int16_t)(cosf(ang) * DIAL_TICK_OUTER_RADIUS);
      d->tick_pts[i][1].y = DIAL_CENTER + (int16_t)(sinf(ang) * DIAL_TICK_OUTER_RADIUS);

      lv_obj_t *tick = lv_line_create(g);
      if (!tick) {
        d->ticks[i] = NULL;
        continue;
      }
      lv_line_set_points(tick, d->tick_pts[i], 2);
      lv_obj_set_style_line_color(tick, ring_color, LV_PART_MAIN);
      lv_obj_set_style_line_width(tick, is_major ? 2 : 1, LV_PART_MAIN);
      lv_obj_set_style_line_opa(tick, is_major ? LV_OPA_90 : LV_OPA_50, LV_PART_MAIN);
      lv_obj_set_style_line_rounded(tick, true, LV_PART_MAIN);
      d->ticks[i] = tick;
    }

    for (uint8_t i = 0; i < DIAL_NUMBER_COUNT; i++) {
      float span = scale_max - scale_min;
      float mark_value = scale_min + (span * i) / DIAL_NUMBER_COUNT;
      int32_t mark = mark_value >= 0.0f ? (int32_t)(mark_value + 0.5f) : (int32_t)(mark_value - 0.5f);
      char num_text[8];
      snprintf(num_text, sizeof(num_text), "%d", (int)mark);

      float ang = deg_to_rad(-90.0f + (360.0f * i) / DIAL_NUMBER_COUNT);
      int16_t dx = (int16_t)(cosf(ang) * DIAL_NUMBER_RADIUS);
      int16_t dy = (int16_t)(sinf(ang) * DIAL_NUMBER_RADIUS);

      lv_obj_t *num = lv_label_create(g);
      if (!num) {
        d->numbers[i] = NULL;
        continue;
      }
      lv_label_set_text(num, num_text);
      lv_obj_set_style_text_font(num, &lv_font_montserrat_10, LV_PART_MAIN);
      lv_obj_set_style_text_color(num, lv_color_hex(0xDDE8F0), LV_PART_MAIN);
      lv_obj_set_style_text_opa(num, LV_OPA_80, LV_PART_MAIN);
      lv_obj_align(num, LV_ALIGN_CENTER, dx, dy);
      d->numbers[i] = num;
    }

    needle_back = lv_obj_create(g);
    if (needle_back) {
      lv_obj_set_size(needle_back, 4, DIAL_NEEDLE_BACK_LEN);
      lv_obj_align(needle_back, LV_ALIGN_CENTER, 0, DIAL_NEEDLE_BACK_LEN / 2);
      lv_obj_set_style_radius(needle_back, LV_RADIUS_CIRCLE, LV_PART_MAIN);
      lv_obj_set_style_border_width(needle_back, 0, LV_PART_MAIN);
      lv_obj_set_style_bg_color(needle_back, needle_color, LV_PART_MAIN);
      lv_obj_set_style_bg_opa(needle_back, LV_OPA_70, LV_PART_MAIN);
      lv_obj_set_style_shadow_width(needle_back, 0, LV_PART_MAIN);
      lv_obj_set_style_pad_all(needle_back, 0, LV_PART_MAIN);
      lv_obj_set_style_transform_pivot_x(needle_back, 2, LV_PART_MAIN);
      lv_obj_set_style_transform_pivot_y(needle_back, 0, LV_PART_MAIN);
    }

    needle_front = lv_obj_create(g);
    if (needle_front) {
      lv_obj_set_size(needle_front, 2, DIAL_NEEDLE_FRONT_LEN);
      lv_obj_align(needle_front, LV_ALIGN_CENTER, 0, -(DIAL_NEEDLE_FRONT_LEN / 2));
      lv_obj_set_style_radius(needle_front, LV_RADIUS_CIRCLE, LV_PART_MAIN);
      lv_obj_set_style_border_width(needle_front, 0, LV_PART_MAIN);
      lv_obj_set_style_bg_color(needle_front, needle_color, LV_PART_MAIN);
      lv_obj_set_style_bg_opa(needle_front, LV_OPA_100, LV_PART_MAIN);
      lv_obj_set_style_shadow_width(needle_front, 0, LV_PART_MAIN);
      lv_obj_set_style_pad_all(needle_front, 0, LV_PART_MAIN);
      lv_obj_set_style_transform_pivot_x(needle_front, 1, LV_PART_MAIN);
      lv_obj_set_style_transform_pivot_y(needle_front, DIAL_NEEDLE_FRONT_LEN, LV_PART_MAIN);
    }

    hub = lv_obj_create(g);
    if (hub) {
      lv_obj_set_size(hub, 6, 6);
      lv_obj_center(hub);
      lv_obj_set_style_radius(hub, LV_RADIUS_CIRCLE, LV_PART_MAIN);
      lv_obj_set_style_bg_color(hub, needle_color, LV_PART_MAIN);
      lv_obj_set_style_border_width(hub, 0, LV_PART_MAIN);
      lv_obj_set_style_shadow_width(hub, 0, LV_PART_MAIN);
      lv_obj_set_style_pad_all(hub, 0, LV_PART_MAIN);
    }
  }

  lv_obj_t *v = lv_label_create(card);
  if (v) {
    lv_label_set_text(v, "0");
    lv_obj_set_width(v, DIAL_CARD_W - 8);
    lv_label_set_long_mode(v, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(v, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(v, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(v, theme_value_color(att_theme), LV_PART_MAIN);
    lv_obj_set_style_text_opa(v, LV_OPA_90, LV_PART_MAIN);
    lv_obj_align(v, LV_ALIGN_BOTTOM_MID, 0, -6);
  }

  d->card = card;
  d->title = t;
  d->face = g;
  d->unit = unit;
  d->decimals = decimals;
  d->min = scale_min;
  d->max = scale_max;
  d->wrap_pointer = wrap_pointer;
  d->needle_front = needle_front;
  d->needle_back = needle_back;
  d->hub = hub;
  d->value = v;
  if (needle_front) {
    lv_obj_set_style_transform_rotation(needle_front, 0, LV_PART_MAIN);
  }
  if (needle_back) {
    lv_obj_set_style_transform_rotation(needle_back, 0, LV_PART_MAIN);
  }
  return card;
}

static void apply_theme(void)
{
  if (!att_screen_obj || !att_top_obj) {
    return;
  }

  lv_obj_set_style_bg_color(att_screen_obj, theme_screen_top(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(att_screen_obj, theme_screen_bottom(att_theme), LV_PART_MAIN);

  lv_obj_set_style_bg_color(att_top_obj, theme_top_bg(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(att_top_obj, theme_top_bg_grad(att_theme), LV_PART_MAIN);
  lv_obj_set_style_border_color(att_top_obj, theme_top_border(att_theme), LV_PART_MAIN);

  if (att_title_obj) {
    lv_obj_set_style_text_color(att_title_obj, theme_title_color(att_theme), LV_PART_MAIN);
  }
  if (att_theme_tag) {
    lv_label_set_text(att_theme_tag, theme_name(att_theme));
    lv_obj_set_style_text_color(att_theme_tag, theme_top_border(att_theme), LV_PART_MAIN);
    lv_obj_set_style_text_opa(att_theme_tag, LV_OPA_90, LV_PART_MAIN);
  }

  for (uint8_t i = 0; i < 6; i++) {
    dial_t *d = &dials[i];
    lv_color_t ring = dial_ring_color(att_theme, i);
    lv_color_t needle = dial_needle_color(att_theme, i);

    if (d->card) {
      lv_obj_set_style_bg_color(d->card, theme_card_bg(att_theme), LV_PART_MAIN);
      lv_obj_set_style_bg_grad_color(d->card, theme_card_bg_grad(att_theme), LV_PART_MAIN);
      lv_obj_set_style_border_color(d->card, ring, LV_PART_MAIN);
    }
    if (d->title) {
      lv_obj_set_style_text_color(d->title, theme_title_color(att_theme), LV_PART_MAIN);
    }
    if (d->face) {
      lv_obj_set_style_bg_color(d->face, theme_face_bg(att_theme), LV_PART_MAIN);
      lv_obj_set_style_bg_grad_color(d->face, theme_face_bg_grad(att_theme), LV_PART_MAIN);
      lv_obj_set_style_border_color(d->face, ring, LV_PART_MAIN);
    }
    for (uint8_t j = 0; j < DIAL_TICK_COUNT; j++) {
      if (d->ticks[j]) {
        lv_obj_set_style_line_color(d->ticks[j], ring, LV_PART_MAIN);
      }
    }
    for (uint8_t j = 0; j < DIAL_NUMBER_COUNT; j++) {
      if (d->numbers[j]) {
        lv_obj_set_style_text_color(d->numbers[j], theme_title_color(att_theme), LV_PART_MAIN);
        lv_obj_set_style_text_opa(d->numbers[j], LV_OPA_80, LV_PART_MAIN);
      }
    }
    if (d->needle_front) {
      lv_obj_set_style_bg_color(d->needle_front, needle, LV_PART_MAIN);
    }
    if (d->needle_back) {
      lv_obj_set_style_bg_color(d->needle_back, needle, LV_PART_MAIN);
      lv_obj_set_style_bg_opa(d->needle_back, LV_OPA_70, LV_PART_MAIN);
    }
    if (d->hub) {
      lv_obj_set_style_bg_color(d->hub, needle, LV_PART_MAIN);
    }
    if (d->value) {
      lv_obj_set_style_text_color(d->value, theme_value_color(att_theme), LV_PART_MAIN);
    }
  }
}

static void back_to_menu(void)
{
  if (!app_nav_back(LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 220, 0, true)) {
    app_nav_open_root(APP_PAGE_MAIN_MENU, LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 220, 0, true);
  }
}

static void top_hit_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_SHORT_CLICKED) {
    back_to_menu();
    return;
  }

  if (code == LV_EVENT_LONG_PRESSED) {
    att_theme = (att_theme == ATT_THEME_TECH) ? ATT_THEME_MECH : ATT_THEME_TECH;
    apply_theme();
  }
}

static void att_timer_cb(lv_timer_t *timer)
{
  LV_UNUSED(timer);
  lv_indev_t *gyro = find_indev_by_type(MY_INDEV_TYPE_GYRO);
  lv_indev_t *accel = find_indev_by_type(MY_INDEV_TYPE_ACCEL);
  if (!gyro || !accel) {
    return;
  }

  lv_indev_read_cb_t gyro_read = lv_indev_get_read_cb(gyro);
  lv_indev_read_cb_t accel_read = lv_indev_get_read_cb(accel);
  if (!gyro_read || !accel_read) {
    return;
  }

  lv_indev_data_t data_g;
  lv_indev_data_t data_a;
  gyro_read(gyro, &data_g);
  accel_read(accel, &data_a);

  uint32_t now = lv_tick_get();
  if (last_ms == 0) {
    last_ms = now;
    return;
  }
  float dt = (now - last_ms) / 1000.0f;
  last_ms = now;
  if (dt <= 0.0f || dt > 0.5f) {
    return;
  }

  float ax = data_a.point.x / 1000.0f;
  float ay = data_a.point.y / 1000.0f;
  float az = data_a.key / 1000.0f;

  float gx_deg = (data_g.point.x / 1000.0f) * 57.29578f;
  float gy_deg = (data_g.point.y / 1000.0f) * 57.29578f;
  float gz_deg = (data_g.key / 1000.0f) * 57.29578f;

  float roll_acc = atan2f(ay, az) * 57.29578f;
  float pitch_acc = atan2f(-ax, sqrtf(ay * ay + az * az)) * 57.29578f;

  const float acc_blend = 0.02f;
  att_deg[0] = wrap_deg((1.0f - acc_blend) * (att_deg[0] + gx_deg * dt) + acc_blend * roll_acc);
  att_deg[1] = wrap_deg((1.0f - acc_blend) * (att_deg[1] + gy_deg * dt) + acc_blend * pitch_acc);
  att_deg[2] = wrap_deg(att_deg[2] + gz_deg * dt);

  float gravity_bias[3];
  calc_gravity_body(roll_acc, pitch_acc, gravity_bias);

  if (accel_bias_n < 40) {
    float k = 1.0f / (float)(accel_bias_n + 1);
    accel_bias[0] += ((ax - gravity_bias[0]) - accel_bias[0]) * k;
    accel_bias[1] += ((ay - gravity_bias[1]) - accel_bias[1]) * k;
    accel_bias[2] += ((az - gravity_bias[2]) - accel_bias[2]) * k;
    accel_bias_n++;
  }

  float gravity_body[3];
  calc_gravity_body(att_deg[0], att_deg[1], gravity_body);

  float lin_a[3] = {
    ax - gravity_body[0] - accel_bias[0],
    ay - gravity_body[1] - accel_bias[1],
    az - gravity_body[2] - accel_bias[2],
  };

  const float accel_deadband = 0.08f;
  const float still_accel = 0.12f;
  const float still_gyro = 2.0f;
  const float vel_damp = 0.985f;
  const float pos_damp = 0.999f;
  bool stationary = fabsf(gx_deg) < still_gyro && fabsf(gy_deg) < still_gyro && fabsf(gz_deg) < still_gyro;

  for (uint8_t i = 0; i < 3; i++) {
    if (fabsf(lin_a[i]) < accel_deadband) {
      lin_a[i] = 0.0f;
    }
    if (fabsf(lin_a[i]) > still_accel) {
      stationary = false;
    }
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (stationary) {
      vel_mps[i] = 0.0f;
    } else {
      vel_mps[i] = (vel_mps[i] + lin_a[i] * dt) * vel_damp;
      if (fabsf(vel_mps[i]) < 0.002f) {
        vel_mps[i] = 0.0f;
      }
    }

    pos_cm[i] = (pos_cm[i] + vel_mps[i] * dt * 100.0f) * pos_damp;
    if (pos_cm[i] > 120.0f) pos_cm[i] = 120.0f;
    if (pos_cm[i] < -120.0f) pos_cm[i] = -120.0f;
  }

  float roll_disp = wrap_deg(att_deg[0]);
  float pitch_disp = wrap_deg(att_deg[1]);
  float yaw_disp = wrap_360(att_deg[2]);

  dial_set(&dials[0], pos_cm[0], pos_cm[0]);
  dial_set(&dials[1], pos_cm[1], pos_cm[1]);
  dial_set(&dials[2], pos_cm[2], pos_cm[2]);
  dial_set(&dials[3], roll_disp, roll_disp);
  dial_set(&dials[4], pitch_disp, pitch_disp);
  dial_set(&dials[5], yaw_disp, yaw_disp);
}

lv_obj_t *mpu_attitude(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);
  att_screen_obj = screen;
  lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, theme_screen_top(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(screen, theme_screen_bottom(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(screen, att_delete_cb, LV_EVENT_DELETE, NULL);
  add_att_bg_fx(screen);

  lv_obj_t *top = lv_obj_create(screen);
  att_top_obj = top;
  lv_obj_set_size(top, LCD_WIDTH - 12, 34);
  lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 6);
  lv_obj_set_style_radius(top, 10, LV_PART_MAIN);
  lv_obj_set_style_bg_color(top, theme_top_bg(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(top, theme_top_bg_grad(att_theme), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(top, LV_GRAD_DIR_HOR, LV_PART_MAIN);
  lv_obj_set_style_border_color(top, theme_top_border(att_theme), LV_PART_MAIN);
  lv_obj_set_style_border_width(top, 1, LV_PART_MAIN);

  lv_obj_t *title = lv_label_create(top);
  att_title_obj = title;
  lv_label_set_text(title, "MPU ATTITUDE");
  lv_obj_set_style_text_color(title, theme_title_color(att_theme), LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t *theme_tag = lv_label_create(top);
  att_theme_tag = theme_tag;
  lv_label_set_text(theme_tag, theme_name(att_theme));
  lv_obj_set_style_text_color(theme_tag, theme_top_border(att_theme), LV_PART_MAIN);
  lv_obj_set_style_text_opa(theme_tag, LV_OPA_90, LV_PART_MAIN);
  lv_obj_align(theme_tag, LV_ALIGN_RIGHT_MID, -8, 0);

  lv_obj_t *hint = lv_label_create(screen);
  lv_label_set_text(hint, "Tap top: back   Long press: theme");
  lv_obj_set_width(hint, LCD_WIDTH - 24);
  lv_label_set_long_mode(hint, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(hint, lv_color_hex(0xA7D8EE), LV_PART_MAIN);
  lv_obj_set_style_text_font(hint, &lv_font_montserrat_10, LV_PART_MAIN);
  lv_obj_set_style_text_opa(hint, LV_OPA_60, LV_PART_MAIN);
  lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -6);

  lv_obj_t *top_hit = lv_button_create(screen);
  lv_obj_set_size(top_hit, LCD_WIDTH - 12, ATT_TOP_HIT_H);
  lv_obj_align(top_hit, LV_ALIGN_TOP_MID, 0, 6);
  lv_obj_set_style_bg_opa(top_hit, LV_OPA_0, LV_PART_MAIN);
  lv_obj_set_style_border_width(top_hit, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(top_hit, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(top_hit, 10, LV_PART_MAIN);
  lv_obj_remove_flag(top_hit, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(top_hit, top_hit_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
  lv_obj_add_event_cb(top_hit, top_hit_event_cb, LV_EVENT_LONG_PRESSED, NULL);
  lv_obj_move_foreground(top_hit);

  memset(dials, 0, sizeof(dials));

  create_dial(screen, 0, "POS X", "cm", 1, -120.0f, 120.0f, false, 6, 44, &dials[0]);
  create_dial(screen, 1, "POS Y", "cm", 1, -120.0f, 120.0f, false, 83, 44, &dials[1]);
  create_dial(screen, 2, "POS Z", "cm", 1, -120.0f, 120.0f, false, 160, 44, &dials[2]);
  create_dial(screen, 3, "ATT R", "deg", 0, -180.0f, 180.0f, true, 6, 152, &dials[3]);
  create_dial(screen, 4, "ATT P", "deg", 0, -180.0f, 180.0f, true, 83, 152, &dials[4]);
  create_dial(screen, 5, "ATT Y", "deg", 0, 0.0f, 360.0f, true, 160, 152, &dials[5]);

  for (uint8_t i = 0; i < 6; i++) {
    dial_set(&dials[i], 0.0f, 0.0f);
  }
  apply_theme();
  play_panel_pulse(top, 100);
  play_label_pulse(theme_tag, 240);

  for (uint8_t i = 0; i < 3; i++) {
    att_deg[i] = 0.0f;
    vel_mps[i] = 0.0f;
    pos_cm[i] = 0.0f;
    accel_bias[i] = 0.0f;
  }
  accel_bias_n = 0;
  last_ms = 0;

  att_timer = lv_timer_create(att_timer_cb, 60, screen);
  return screen;
}
