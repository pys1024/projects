#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
// #include <Versatile_RotaryEncoder.h>
#include <ESP_Knob.h>

#include "common.h"
#include "lv_port/app_hal.h"

Adafruit_MCP23X17 mcp;
// Versatile_RotaryEncoder *encoder = new Versatile_RotaryEncoder(EC2_CLK_PIN, EC2_DT_PIN, 0);
ESP_Knob *knob = new ESP_Knob(EC2_CLK_PIN, EC2_DT_PIN);

lv_obj_t *label;
char str[100] = {0};

void test()
{
  lv_obj_t *screen = lv_screen_active();

  lv_obj_t *obj = lv_obj_create(screen);
  lv_obj_set_size(obj, 240, 280);
  lv_obj_set_pos(obj, 0, 0);

  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
  label = lv_label_create(obj);
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

 void onKnobLeftEventCallback(int count, void *usr_data)
{
  // Serial.printf("Detect left event, count is %d\n", count);
  snprintf(str, sizeof(str), "Left Event: %d", count);
}

void onKnobRightEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect right event, count is %d\n", count);
    snprintf(str, sizeof(str), "Right Event: %d", count);
}

void onKnobHighLimitEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect high limit event, count is %d\n", count);
    snprintf(str, sizeof(str), "High Limit Event: %d", count);
}

void onKnobLowLimitEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect low limit event, count is %d\n", count);
    snprintf(str, sizeof(str), "Low Limit Event: %d", count);
}

void onKnobZeroEventCallback(int count, void *usr_data)
{
    // Serial.printf("Detect zero event, count is %d\n", count);
    snprintf(str, sizeof(str), "Zero Event: %d", count);
}

void setup() {
  // GPIO Initialization
  pinMode(POWER_EN_PIN, OUTPUT); // Set POWER_EN_PIN as output
  pinMode(LCD_BLK_PIN, OUTPUT); // Set LCD_BLK_PIN as output
  pinMode(LX_PIN, INPUT); // Set LX_PIN as input
  pinMode(LY_PIN, INPUT); // Set LY_PIN as input
  pinMode(RX_PIN, INPUT); // Set RX_PIN as input
  pinMode(RY_PIN, INPUT); // Set RY_PIN as input
  pinMode(BAT_DET, INPUT); // Set BAT_DET as input

  analogSetPinAttenuation(LX_PIN, ADC_11db); // Set LX_PIN attenuation to 11db
  analogSetPinAttenuation(LY_PIN, ADC_11db); // Set LY_PIN attenuation to 11db
  analogSetPinAttenuation(RX_PIN, ADC_11db); // Set RX_PIN attenuation to 11db
  analogSetPinAttenuation(RY_PIN, ADC_11db); // Set RY_PIN attenuation to 11db
  analogSetPinAttenuation(BAT_DET, ADC_11db); // Set BAT_DET attenuation to 11db

  digitalWrite(POWER_EN_PIN, HIGH); // Turn on the device

  Serial.begin(115200);
  Serial.println("Remote Controller Starting...");

  Wire.setPins(MCP_I2C_SDA_PIN, MCP_I2C_SCK_PIN);

  if (!mcp.begin_I2C()) {
    // Serial.println("Error.");
    while (1);
  }

  lv_init();

  hal_setup();

  test();

  knob->begin();
  knob->attachLeftEventCallback(onKnobLeftEventCallback);
  knob->attachRightEventCallback(onKnobRightEventCallback);
  knob->attachHighLimitEventCallback(onKnobHighLimitEventCallback);
  knob->attachLowLimitEventCallback(onKnobLowLimitEventCallback);
  knob->attachZeroEventCallback(onKnobZeroEventCallback);

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

  if (strcmp(str, lv_label_get_text(label)) != 0) {
    lv_label_set_text(label, str);
  }

  if (millis() % 100 == 0) {
    Serial.printf("LX: %04d, LY: %04d, RX: %04d, RY: %04d, Battery: %04dmV\n",
      analogRead(LX_PIN), analogRead(LY_PIN),
      analogRead(RX_PIN), analogRead(RY_PIN),
      analogReadMilliVolts(BAT_DET)*2);
  }
}