#pragma once


#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
  #define DBLOG(fmt, ...)  Serial.printf((fmt "\n"), ##__VA_ARGS__)
#else
  #define DBLOG(fmt, ...)
#endif

#define SERIAL_BAUDRATE 115200
#define SPI_SPEED 10000000
#define I2C_SPEED 400000

#define PWR_EN 15

#define M1_IN1 12
#define M1_IN2 13
#define M2_IN1 5
#define M2_IN2 18
#define M3_IN1 16
#define M3_IN2 4
#define M4_IN1 21
#define M4_IN2 19

#define NRF_CSN_PIN        32 // GPIO pin for NRF24L01 CSN
#define NRF_CE_PIN         27 // GPIO pin for NRF24L01 CE
#define NRF_MOSI_PIN       33 // GPIO pin for NRF24L01 MOSI
#define NRF_MISO_PIN       25 // GPIO pin for NRF24L01 MISO
#define NRF_SCK_PIN        26 // GPIO pin for NRF24L01 SCK

#define MPU_SDA_PIN 23
#define MPU_SCL_PIN 22

#define BAT1_PIN 34
#define BAT2_PIN 35
