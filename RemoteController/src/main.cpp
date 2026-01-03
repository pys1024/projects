#include "common.h"

void hal_check()
{
  char msg[128];
  lv_obj_t *screen = lv_obj_create(lv_screen_active());
  lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);
  lv_obj_center(screen);
  lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_text_color(screen, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);

  // Check all devices, and create a label at each line.
  for (int i = 0; i < 100; i++) {
    hal_status_t ret = hal_init_device(i, msg, sizeof(msg));
    if (ret == HAL_OK) {
      lv_obj_t *label = lv_label_create(screen);
      lv_label_set_text_fmt(label, "%-20s", msg); // Left-aligned text
      lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN); // Set text color to green
    } else if (ret == HAL_ERROR) {
      lv_obj_t *label = lv_label_create(screen);
      lv_label_set_text_fmt(label, "Error: %-20s", msg); // Left-aligned error text
      lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN); // Set text color to red

      while (1) {
        hal_loop();
      }
    } else if (ret == HAL_NO_MORE_DEVICES) {
      break;
    }
  }

 }

void setup() {
  hal_setup();

  hal_check();
  hal_beep();
}

void loop() {
  hal_loop();
}

#ifndef ARDUINO
int main() {
  setup();
  while (1) {
    loop();
  }
}
#endif