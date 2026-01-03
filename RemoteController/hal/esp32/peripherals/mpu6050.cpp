#include "common.h"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 *mpu = new Adafruit_MPU6050();

bool hal_mpu6050_init() {
   // Try to initialize!
  if (!mpu->begin()) {
    // Serial.println("Failed to find MPU6050 chip");
    DBLOG("MPU6050 check failed!");
    return false; // MPU6050 not OK
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

  return true;
}