#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP_Knob.h>
#include "RF24.h"
#include "SPI.h"

#include "common.h"
#include "lv_port/app_hal.h"

Adafruit_MCP23X17 *mcp = new Adafruit_MCP23X17();
// Versatile_RotaryEncoder *encoder = new Versatile_RotaryEncoder(EC2_CLK_PIN, EC2_DT_PIN, 0);
ESP_Knob *knob = new ESP_Knob(EC2_CLK_PIN, EC2_DT_PIN);
Adafruit_MPU6050 *mpu = new Adafruit_MPU6050();
SPIClass *nrf_spi = new SPIClass(HSPI); // Use FSPI/HSPI/VSPI for NRF24L01+
// SPIClass *nrf_spi = &SPI; // Use default SPI for NRF24L01+
RF24 *radio = new RF24(NRF_CE_PIN, NRF_CSN_PIN, SPI_SPEED);

lv_obj_t *label;
char str[100] = {0};

/* lvgl button callback */
void btn_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* btn = (lv_obj_t *)lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED) {
    static uint8_t cnt = 0;
    cnt++;
    /*Get the first child of the button which is the label and change its text*/
    lv_obj_t* label1 = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label1, "Button %d", cnt);
  }
}

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

  lv_obj_t * btn = lv_button_create(lv_screen_active());
  lv_obj_center(btn);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_t* label1 = lv_label_create(btn);
  lv_label_set_text(label1, "Button");
  lv_obj_center(label1);

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

void beep() {
  mcp->digitalWrite(BEEP_PIN, HIGH);
  delay(100);
  mcp->digitalWrite(BEEP_PIN, LOW);
}

void setup() {
  // GPIO Initialization
  pinMode(POWER_EN_PIN, OUTPUT); // Set POWER_EN_PIN as output
  pinMode(LCD_BLK_PIN, OUTPUT); // Set LCD_BLK_PIN as output

  pinMode(KEY4_PIN, INPUT); // Set KEY4_PIN as input
  pinMode(BAT_DET, INPUT); // Set BAT_DET as input

  pinMode(LX_PIN, INPUT); // Set LX_PIN as input
  pinMode(LY_PIN, INPUT); // Set LY_PIN as input
  pinMode(RX_PIN, INPUT); // Set RX_PIN as input
  pinMode(RY_PIN, INPUT); // Set RY_PIN as input

  analogSetPinAttenuation(LX_PIN, ADC_11db); // Set LX_PIN attenuation to 11db
  analogSetPinAttenuation(LY_PIN, ADC_11db); // Set LY_PIN attenuation to 11db
  analogSetPinAttenuation(RX_PIN, ADC_11db); // Set RX_PIN attenuation to 11db
  analogSetPinAttenuation(RY_PIN, ADC_11db); // Set RY_PIN attenuation to 11db
  analogSetPinAttenuation(BAT_DET, ADC_11db); // Set BAT_DET attenuation to 11db

  digitalWrite(POWER_EN_PIN, HIGH); // Turn on the device

#if DEBUG_ENABLE
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setDebugOutput(true);
  while(!Serial); // Wait for serial port to be available
  DBLOG("Remote Controller Starting...");
#endif

  // Wire.setPins(MCP_I2C_SDA_PIN, MCP_I2C_SCK_PIN);
  Wire.begin(MCP_I2C_SDA_PIN, MCP_I2C_SCK_PIN, I2C_SPEED);
  if (!mcp->begin_I2C()) {
    while (1) {
      delay(10);
      DBLOG("MCP23017 check failed!");
    }
  } else {
    DBLOG("MCP23017 check OK!");
  }

  mcp->pinMode(BEEP_PIN, OUTPUT);
  mcp->pinMode(KEY1_PIN, INPUT);
  mcp->pinMode(KEY2_PIN, INPUT);
  mcp->pinMode(KEY3_PIN, INPUT);
  mcp->pinMode(KEY5_PIN, INPUT);
  mcp->pinMode(KEY6_PIN, INPUT);
  mcp->pinMode(KEY7_PIN, INPUT);
  mcp->pinMode(KEY8_PIN, INPUT);
  mcp->pinMode(EC1_SW_PIN, INPUT);
  mcp->pinMode(EC2_SW_PIN, INPUT);
  mcp->pinMode(RC1_SW_PIN, INPUT);
  mcp->pinMode(RC2_SW_PIN, INPUT);
  mcp->pinMode(SW1_PIN, INPUT);
  mcp->pinMode(SW2_PIN, INPUT);
  mcp->pinMode(SW3_PIN, INPUT);
  mcp->pinMode(SW4_PIN, INPUT);

   // Try to initialize!
  if (!mpu->begin()) {
    // Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
      DBLOG("MPU6050 check failed!");
    }
  } else {
    DBLOG("MPU6050 check OK!");
  }

  //setupt motion detection
  mpu->setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu->setMotionDetectionThreshold(1);
  mpu->setMotionDetectionDuration(20);
  mpu->setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu->setInterruptPinPolarity(true);
  mpu->setMotionInterrupt(true);

  // initialize the transceiver on the SPI bus
  nrf_spi->begin(NRF_SCK_PIN, NRF_MISO_PIN, NRF_MOSI_PIN, NRF_CSN_PIN);
  nrf_spi->setFrequency(SPI_SPEED);
  if (!radio->begin(nrf_spi) || !radio->isChipConnected()) {
    while (1) {
      delay(10);
      DBLOG("NRF24L01+ check failed!");
    }
  } else {
    DBLOG("NRF24L01+ check OK!");
  }

  hal_setup();
  test();

  knob->begin();
  knob->attachLeftEventCallback(onKnobLeftEventCallback);
  knob->attachRightEventCallback(onKnobRightEventCallback);
  knob->attachHighLimitEventCallback(onKnobHighLimitEventCallback);
  knob->attachLowLimitEventCallback(onKnobLowLimitEventCallback);
  knob->attachZeroEventCallback(onKnobZeroEventCallback);

  beep();
  DBLOG("Setup completed.");
}

void loop() {
  if (mcp->digitalRead(KEY1_PIN) == LOW) {
    beep();
  }

  if (mcp->digitalRead(KEY3_PIN) == LOW) {
    digitalWrite(POWER_EN_PIN, LOW); // Turn off the device
    // while (1);
  }

  hal_loop();

  if (strcmp(str, lv_label_get_text(label)) != 0) {
    lv_label_set_text(label, str);
  }

  // if (millis() % 100 == 0) {
  //   Serial.printf("LX: %04d, LY: %04d, RX: %04d, RY: %04d, Battery: %04dmV\n",
  //     analogRead(LX_PIN), analogRead(LY_PIN),
  //     analogRead(RX_PIN), analogRead(RY_PIN),
  //     analogReadMilliVolts(BAT_DET)*2);
  // }

  if(mpu->getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu->getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");
  }
}