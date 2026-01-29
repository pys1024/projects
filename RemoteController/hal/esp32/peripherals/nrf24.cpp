#include "common.h"
#include "RF24.h"

SPIClass *nrf_spi = new SPIClass(HSPI); // Use FSPI/HSPI/VSPI for NRF24L01+
// SPIClass *nrf_spi = &SPI; // Use default SPI for NRF24L01+
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

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio->setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio->setPayloadSize(sizeof(cmd_code));  // float datatype occupies 4 bytes
  radio->setAddressWidth(sizeof(address) - 1);  // 5 bytes address width

  // set the TX address of the RX node for use on the TX pipe (pipe 0)
  radio->stopListening(address);  // put radio in TX mode

  // set the RX address of the TX node into a RX pipe
  radio->openReadingPipe(1, address);  // using pipe 1

  return true;
}

bool hal_nrf24_send_cmd(int8_t cmd0, int8_t cmd1, int8_t cmd2, int8_t cmd3)
{
  cmd_code[0] = cmd0;
  cmd_code[1] = cmd1;
  cmd_code[2] = cmd2;
  cmd_code[3] = cmd3;

  radio->stopListening(); // Ensure we are in TX mode
  bool report = radio->write(&cmd_code, sizeof(cmd_code));  // transmit & save the report

  if (report) {
    return true;
  }

  return false;
}