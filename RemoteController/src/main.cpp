#include "common.h"

#include "app_navigation.h"
#include "hal_check.h"

void setup() {
  hal_setup();

  hal_check();
  hal_beep();

  app_nav_open_root(APP_PAGE_MAIN_MENU, LV_SCREEN_LOAD_ANIM_FADE_ON, 500, 0, true);
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
