#include "common.h"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 *mpu = new Adafruit_MPU6050();

static sensors_event_t cached_accel;
static sensors_event_t cached_gyro;
static uint32_t cached_sample_ms = 0;
static bool cached_sample_valid = false;

static void mpu_refresh_sample(void)
{
  uint32_t now = lv_tick_get();
  if (cached_sample_valid && cached_sample_ms == now) {
    return;
  }

  sensors_event_t temp;
  mpu->getEvent(&cached_accel, &cached_gyro, &temp);
  cached_sample_ms = now;
  cached_sample_valid = true;
}

static void mpu_accel_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  LV_UNUSED(indev);

  mpu_refresh_sample();

  data->point.x = (int16_t)(cached_accel.acceleration.x * 1000.0f);
  data->point.y = (int16_t)(cached_accel.acceleration.y * 1000.0f);
  data->key = (int32_t)(cached_accel.acceleration.z * 1000.0f);
  data->state = LV_INDEV_STATE_PRESSED;
}

static void mpu_gyro_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
  LV_UNUSED(indev);

  mpu_refresh_sample();

  data->point.x = (int16_t)(cached_gyro.gyro.x * 1000.0f);
  data->point.y = (int16_t)(cached_gyro.gyro.y * 1000.0f);
  data->key = (int32_t)(cached_gyro.gyro.z * 1000.0f);
  data->state = LV_INDEV_STATE_PRESSED;
}

bool hal_mpu6050_init() {
   // Try to initialize!
  if (!mpu->begin()) {
    // Serial.println("Failed to find MPU6050 chip");
    DBLOG("MPU6050 check failed!");
    return false; // MPU6050 not OK
  } else {
    DBLOG("MPU6050 check OK!");
  }

  cached_sample_ms = 0;
  cached_sample_valid = false;

  //setupt motion detection
  mpu->setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu->setMotionDetectionThreshold(1);
  mpu->setMotionDetectionDuration(20);
  mpu->setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu->setInterruptPinPolarity(true);
  mpu->setMotionInterrupt(true);

  lv_indev_t *indev_acc = lv_indev_create();
  lv_indev_enable(indev_acc, false);
  lv_indev_set_driver_data(indev_acc, mpu);
  lv_indev_set_type(indev_acc, MY_INDEV_TYPE_ACCEL);
  lv_indev_set_mode(indev_acc, LV_INDEV_MODE_EVENT);
  lv_indev_set_read_cb(indev_acc, mpu_accel_read_cb);

  lv_indev_t *indev_gyro = lv_indev_create();
  lv_indev_enable(indev_gyro, false);
  lv_indev_set_driver_data(indev_gyro, mpu);
  lv_indev_set_type(indev_gyro, MY_INDEV_TYPE_GYRO);
  lv_indev_set_mode(indev_gyro, LV_INDEV_MODE_EVENT);
  lv_indev_set_read_cb(indev_gyro, mpu_gyro_read_cb);

  return true;
}
