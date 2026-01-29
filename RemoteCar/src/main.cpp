#include <Arduino.h>
#include "RF24.h"
#include "SPI.h"

#include "config.h"

// SPIClass *nrf_spi = new SPIClass(HSPI); // Use FSPI/HSPI/VSPI for NRF24L01+
SPIClass *nrf_spi = &SPI; // Use default SPI for NRF24L01+
RF24 *radio = new RF24(NRF_CE_PIN, NRF_CSN_PIN, SPI_SPEED);
const uint8_t address[] = "12345";
int8_t cmd_code[4] = {0, 0, 0, 0};

bool hal_nrf24_init()
{
  // initialize the transceiver on the SPI bus
  nrf_spi->begin(NRF_SCK_PIN, NRF_MISO_PIN, NRF_MOSI_PIN, NRF_CSN_PIN);
  nrf_spi->setFrequency(SPI_SPEED);

  if (!radio->begin(nrf_spi) || !radio->isChipConnected()) {
    DBLOG("NRF24L01+ check failed!");
    return false; // NRF24L01+ not OK
  } else {
    DBLOG("NRF24L01+ check OK!");
  }

  return true;
}

void setup()
{
  pinMode(PWR_EN, OUTPUT);
  pinMode(BAT1_PIN, INPUT);
  pinMode(BAT2_PIN, INPUT);

  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT);
  pinMode(M4_IN2, OUTPUT);

  digitalWrite(PWR_EN, HIGH); // Enable power to motor driver

  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);
  digitalWrite(M3_IN1, LOW);
  digitalWrite(M3_IN2, LOW);
  digitalWrite(M4_IN1, LOW);
  digitalWrite(M4_IN2, LOW);

#if DEBUG_ENABLE
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setDebugOutput(true);
  while(!Serial); // Wait for serial port to be available
  DBLOG("Remote Car Starting...");
#endif

  if (!hal_nrf24_init()) {
    DBLOG("NRF24L01+ initialization failed!");
  }

  radio->setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio->setPayloadSize(sizeof(cmd_code));  // float datatype occupies 4 bytes
  radio->setAddressWidth(sizeof(address) - 1);  // 5 bytes address width

  // set the TX address of the RX node for use on the TX pipe (pipe 0)
  radio->stopListening(address);  // put radio in TX mode

  // set the RX address of the TX node into a RX pipe
  radio->openReadingPipe(1, address);  // using pipe 1

  // additional setup specific to the node's RX role
  radio->startListening();  // put radio in RX mode

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data
}

void write_cmd(int8_t cmd, uint8_t pin1, uint8_t pin2)
{
  cmd *= 10;
  cmd = constrain(cmd, -255, 255);

  if (cmd > 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else if (cmd < 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
}

void loop()
{
  uint8_t pipe = 0;

  if (radio->available(&pipe)) {              // is there a payload? get the pipe number that received it
    uint8_t bytes = radio->getPayloadSize();  // get the size of the payload
    if (bytes >= sizeof(cmd_code)) {
      radio->read(&cmd_code, sizeof(cmd_code));             // fetch payload from FIFO

      DBLOG("Received %d bytes on pipe %d: %d, %d, %d, %d", bytes, pipe,
            cmd_code[0], cmd_code[1], cmd_code[2], cmd_code[3]);

      write_cmd(cmd_code[0], M1_IN1, M1_IN2);
      write_cmd(cmd_code[1], M2_IN1, M2_IN2);
      write_cmd(cmd_code[2], M3_IN1, M3_IN2);
      write_cmd(cmd_code[3], M4_IN1, M4_IN2);
    }
  }

}
