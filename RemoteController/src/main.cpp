#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#include "common.h"
#include "lv_port/app_hal.h"

Adafruit_MCP23X17 mcp;

void test()
{
   lv_obj_t *screen = lv_screen_active();

   lv_obj_t *obj = lv_obj_create(screen);
   lv_obj_set_size(obj, 240, 280);
   lv_obj_set_pos(obj, 0, 0);

   lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
   lv_obj_t *label = lv_label_create(obj);
   lv_label_set_text(label, "Hello World");

   /*Create style*/
   static lv_style_t style_line;
   lv_style_init(&style_line);
   lv_style_set_line_width(&style_line, 10);
   lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
   lv_style_set_line_rounded(&style_line, true);

   static lv_point_precise_t points[] = { {10, 10}, {10, 90}, {90, 90}, {90, 10} };

   lv_obj_t *line = lv_line_create(screen);
   lv_obj_add_style(line, &style_line, LV_PART_MAIN);
   lv_line_set_points(line, points, sizeof(points) / sizeof(lv_point_precise_t));
   lv_obj_center(line);

   lv_obj_t * bar1 = lv_bar_create(lv_screen_active());
   lv_obj_set_size(bar1, 200, 20);
   lv_obj_center(bar1);
   lv_bar_set_value(bar1, 70, LV_ANIM_OFF);

  //  LV_IMAGE_DECLARE(death_note);
  //  lv_obj_t *img = lv_img_create(lv_screen_active());
  //  lv_image_set_src(img, &death_note);
  //  lv_obj_center(img);

  //  lv_obj_t *img2 = lv_img_create(lv_screen_active());
  //  lv_image_set_src(img2, LV_SYMBOL_AUDIO " Death Note");
  //  lv_obj_center(img2);
  //  lv_obj_align_to(img2, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
 }

void setup() {
  // GPIO Initialization
  pinMode(POWER_EN_PIN, OUTPUT); // Set POWER_EN_PIN as output
  pinMode(LCD_BLK_PIN, OUTPUT); // Set LCD_BLK_PIN as output

  // Power on the device
  digitalWrite(POWER_EN_PIN, HIGH); // Turn on the device

  Wire.setPins(MCP_I2C_SDA_PIN, MCP_I2C_SCK_PIN);

  if (!mcp.begin_I2C()) {
    // Serial.println("Error.");
    while (1);
  }

  lv_init();

  hal_setup();

  // test();

  mcp.pinMode(BEEP_PIN, OUTPUT);
  mcp.digitalWrite(BEEP_PIN, HIGH);
  delay(500);
  mcp.digitalWrite(BEEP_PIN, LOW);

}

void loop() {
  if (mcp.digitalRead(SW1_PIN) == LOW) {
    mcp.digitalWrite(BEEP_PIN, HIGH);
    delay(500);
    mcp.digitalWrite(BEEP_PIN, LOW);
  }

  if (mcp.digitalRead(SW3_PIN) == LOW) {
    digitalWrite(POWER_EN_PIN, LOW); // Turn off the device
    // while (1);
  }

  hal_loop();
}