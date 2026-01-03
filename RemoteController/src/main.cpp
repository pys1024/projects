#include "common.h"

#include "dev_check.h"
#include "hal_check.h"

void setup() {
  hal_setup();

  hal_check();
  hal_beep();

  lv_obj_t *scr = dev_check();

  lv_screen_load_anim(scr, LV_SCREEN_LOAD_ANIM_FADE_ON, 500, 0, true);
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