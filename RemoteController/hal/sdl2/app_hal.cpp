#include "common.h"

#include <unistd.h>
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "drivers/sdl/lv_sdl_mouse.h"
#include "drivers/sdl/lv_sdl_mousewheel.h"
#include "drivers/sdl/lv_sdl_keyboard.h"
#include "beeper.h"


static lv_display_t *lvDisplay;
static lv_indev_t *lvMouse;
static lv_indev_t *lvMouseWheel;
static lv_indev_t *lvKeyboard;


#if LV_USE_LOG != 0
static void lv_log_print_g_cb(lv_log_level_t level, const char * buf)
{
  LV_UNUSED(level);
  LV_UNUSED(buf);
}
#endif


void hal_setup(void)
{
    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(SDL_GetTicks);

    // Workaround for sdl2 `-m32` crash
    // https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
    #ifndef WIN32
        setenv("DBUS_FATAL_WARNINGS", "0", 1);
    #endif

    #if LV_USE_LOG != 0
    lv_log_register_print_cb(lv_log_print_g_cb);
    #endif

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    lvDisplay = lv_sdl_window_create(LCD_WIDTH, LCD_HEIGHT);
    lvMouse = lv_sdl_mouse_create();
    lvMouseWheel = lv_sdl_mousewheel_create();
    lvKeyboard = lv_sdl_keyboard_create();
}

void hal_loop(void)
{
    lv_timer_handler(); // Update the UI-
    // SDL_Delay(5);
}

hal_status_t hal_init_device(uint8_t dev_id, char *msg, uint32_t msg_size)
{
    switch (dev_id) {
        case 0:
            snprintf(msg, msg_size, "SDL2 Display check OK!");
            return HAL_OK;
        case 1:
            snprintf(msg, msg_size, "SDL2 Mouse check OK!");
            return HAL_OK;
        case 2:
            snprintf(msg, msg_size, "SDL2 Mousewheel check OK!");
            return HAL_OK;
        case 3:
            snprintf(msg, msg_size, "SDL2 Keyboard check OK!");
            return HAL_OK;
        default:
            snprintf(msg, msg_size, "No more devices");
            return HAL_NO_MORE_DEVICES;
    }
}

void hal_beep()
{
    Beeper::open();
    Beeper::setVolume(10.0);
    Beeper::setFrequency(1000);

    Beeper::play();
    SDL_Delay(100);
    Beeper::stop();
}
