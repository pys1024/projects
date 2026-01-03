#include "common.h"
#include "RF24.h"

SPIClass *nrf_spi = new SPIClass(HSPI); // Use FSPI/HSPI/VSPI for NRF24L01+
// SPIClass *nrf_spi = &SPI; // Use default SPI for NRF24L01+
RF24 *radio = new RF24(NRF_CE_PIN, NRF_CSN_PIN, SPI_SPEED);

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
