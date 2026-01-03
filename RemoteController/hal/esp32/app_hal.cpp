#include "common.h"

#include "app_hal.h"

hal_status_t hal_init_device(uint8_t dev_id, char *msg, uint32_t msg_size)
{
  switch (dev_id) {
    case 0:
      if (hal_mcp23017_init()) {
        snprintf(msg, msg_size, "MCP23017 check OK!");
        return HAL_OK;
      }
      snprintf(msg, msg_size, "MCP23017 check failed!");
      return HAL_ERROR;

    case 1:
      if (hal_mpu6050_init()) {
        snprintf(msg, msg_size, "MPU6050 check OK!");
        return HAL_OK;
      }
      snprintf(msg, msg_size, "MPU6050 check failed!");
      return HAL_ERROR;

    case 2:
      if (hal_encoder_init()) {
        snprintf(msg, msg_size, "Encoder check OK!");
        return HAL_OK;
      }
      snprintf(msg, msg_size, "Encoder check failed!");
      return HAL_ERROR;

    case 3:
      if (hal_nrf24_init()) {
        snprintf(msg, msg_size, "nRF24 check OK!");
        return HAL_OK;
      }
      snprintf(msg, msg_size, "nRF24 check failed!");
      return HAL_ERROR;

    case 4:
      if (lv_port_indev_init()) {
        snprintf(msg, msg_size, "Touchpad check OK!");
        return HAL_OK;
      }
      snprintf(msg, msg_size, "Touchpad check failed!");
      return HAL_ERROR;

    default:
      return HAL_NO_MORE_DEVICES;
  }
}

void hal_setup()
{
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

  if (!lv_port_disp_init()) { // Initialize the LVGL display port
    while (1) {
      delay(1000); // Delay for 1 second
      // Loop indefinitely if the display port initialization fails
      DBLOG("LVGL display port initialization failed!");
      hal_beep(); // Beep to indicate the failure
    }
  }

  DBLOG("Setup completed.");
}

void hal_loop()
{
  lv_port_disp_loop(); // Call the LVGL display loop function

  if (mcpDigitalRead(KEY1_PIN) == LOW) {
    hal_beep();
  }

  if (mcpDigitalRead(KEY3_PIN) == LOW) {
    digitalWrite(POWER_EN_PIN, LOW); // Turn off the device
    // while (1);
  }

#if 0
  if (millis() % 100 == 0) {
    Serial.printf("LX: %04d, LY: %04d, RX: %04d, RY: %04d, Battery: %04dmV\n",
      analogRead(LX_PIN), analogRead(LY_PIN),
      analogRead(RX_PIN), analogRead(RY_PIN),
      analogReadMilliVolts(BAT_DET)*2);
  }

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
#endif
}

