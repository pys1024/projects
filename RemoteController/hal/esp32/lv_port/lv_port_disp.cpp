#include "common.h"
#include <Arduino_GFX_Library.h>

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include <examples/lv_examples.h>
#include <demos/lv_demos.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */

// #include <examples/lv_examples.h>
// #include <demos/lv_demos.h>

// #define DIRECT_RENDER_MODE // Uncomment to enable full frame buffer

// #define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define GFX_BL LCD_BLK_PIN

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *tft = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
// Arduino_DataBus *bus = create_default_Arduino_DataBus();
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC_PIN /* DC */, LCD_CS_PIN /* CS */, LCD_SCK_PIN /* SCK */, LCD_MOSI_PIN /* MOSI */, -1 /* MISO */, FSPI /* HW SPI */);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
// Arduino_GFX *tft = new Arduino_ILI9341(bus, DF_GFX_RST, 0 /* rotation */, false /* IPS */);
Arduino_GFX *tft = new Arduino_ST7789(
    /* data bus instance */ bus,
    /* reset pin       */ LCD_RST_PIN,
    /* rotation        */ 0,
    /* IPS             */ true,
    /* width           */ LCD_WIDTH,
    /* height          */ LCD_HEIGHT,
    /* col offset1     */ 0,
    /* row offset1     */ 20,
    /* col offset2     */ 0,
    /* row offset2     */ 0
);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
lv_display_t *disp;
lv_color_t *disp_draw_buf;

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  LV_UNUSED(buf);
  // Serial.println(buf);
  // Serial.flush();
}
#endif

uint32_t millis_cb(void)
{
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
#ifndef DIRECT_RENDER_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  // lv_draw_sw_rgb565_swap(px_map, w * h);
  tft->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif // #ifndef DIRECT_RENDER_MODE

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

bool lv_port_disp_init(void)
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  // Init Display
  if (!tft->begin())
  {
    DBLOG("TFT check failed!");
    return false; // TFT not OK
  } else {
    DBLOG("TFT check OK!");
  }

  tft->fillScreen(RGB565_BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenWidth = tft->width();
  screenHeight = tft->height();

#ifdef DIRECT_RENDER_MODE
  bufSize = screenWidth * screenHeight;
#else
  bufSize = screenWidth * 40;
#endif

#ifdef ESP32
#if defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
  disp_draw_buf = (lv_color_t *)tft->getFramebuffer();
#else  // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
  {
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
#endif // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
#else  // !ESP32
  DBLOG("LVGL disp_draw_buf heap_caps_malloc failed! malloc again...");
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif // !ESP32
  if (!disp_draw_buf)
  {
    DBLOG("LVGL disp_draw_buf allocate failed!");
    return false;
  }
  else
  {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_RENDER_MODE
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    /* Option 1: Create a simple label
     * ---------------------
     */
    // lv_obj_t *label = lv_label_create(lv_scr_act());
    // lv_label_set_text(label, "Hello Arduino, I'm LVGL!(V" GFX_STR(LVGL_VERSION_MAJOR) "." GFX_STR(LVGL_VERSION_MINOR) "." GFX_STR(LVGL_VERSION_PATCH) ")");
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /* Option 2: Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------
     */
    // lv_example_btn_1();
    // lv_example_scroll_1();

    /* Option 3: Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS
     * -------------------------------------------------------------------------------------------
     */
    // lv_demo_widgets();
    // lv_demo_benchmark();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    // lv_demo_stress();
  }

  DBLOG("lv_port_disp_init done");
  return true;
}

void lv_port_disp_loop(void)
{
  lv_task_handler(); /* let the GUI do its work */

#ifdef DIRECT_RENDER_MODE
#if defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)
  tft->flush();
#else  // !(defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
  tft->draw16bitRGBBitmap(0, 0, (uint16_t *)disp_draw_buf, screenWidth, screenHeight);
#endif // !(defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
#else  // !DIRECT_RENDER_MODE
#ifdef CANVAS
  tft->flush();
#endif
#endif // !DIRECT_RENDER_MODE

  // delay(5);
}