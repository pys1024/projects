#include "reg52.h"
#include "i2c.h"

sbit SDA = P2^1;
sbit SCL = P2^0;

void i2c_init() {
    SDA = 1;  // Set SDA to high level
    SCL = 1;  // Set SCL to high level
}

// I2C slave start function
uint8_t i2c_slave_start(uint8_t *addr) {
    uint8_t ack = 0;

    // Wait for start signal
    while (SDA == 1 && SCL == 1);
    DELAY_5US();

    // Detect start signal
    if (SDA == 0 && SCL == 1) {
        DELAY_5US();
        while (SCL == 1);  // Wait for master to pull SCL low
        ack = 1;      // Indicates a master request

        // Read the address sent by the master
        *addr = i2c_slave_read();  // Store the received address in the pointer
    }

    return ack;
}

void i2c_slave_stop() {
    SDA = 1;
    SCL = 1;

    while (SDA != 1 || SCL != 1);  // Wait for stop signal
}

// I2C slave write data function with ACK handling
uint8_t i2c_slave_write(uint8_t data0) {
    uint8_t i = 0;

    do {
        SDA = (data0 & 0x80) ? 1 : 0;  // Send the most significant bit
        data0 <<= 1;  // Shift left by one bit
        DELAY_5US();
        while (SCL == 0);  // Wait for master to pull SCL high
        DELAY_5US();
        while (SCL == 1);  // Wait for master to pull SCL low
    } while (++i < 8);

    // Wait for ACK from the master
    SDA = 1;  // Release SDA
    DELAY_5US();
    while (SCL == 0);  // Wait for master to pull SCL high

    // Check if master sent ACK (SDA should be low)
    DELAY_5US();
    if (SDA) {
        // No ACK received, handle error if necessary
        i = 0;
    } else {
        i = 1;  // ACK received
    }

    while (SCL == 1);  // Wait for master to pull SCL low

    return i;
}

// I2C slave read data function with ACK handling
uint8_t i2c_slave_read() {
    uint8_t i = 0;
    uint8_t data0 = 0;

    for (i = 0; i < 8; i++) {
        DELAY_5US();
        while (SCL == 0);  // Wait for master to pull SCL high
        DELAY_5US();
        data0 <<= 1;
        if (SDA) {
            data0 |= 0x01;  // Read data bit when SCL is high
        }
        while (SCL == 1);  // Wait for master to pull SCL low
    }

    // Send ACK signal to the master
    SDA = 0;  // Pull SDA low to indicate ACK
    DELAY_5US();
    while (SCL == 0);  // Wait for master to pull SCL high
    DELAY_5US();
    while (SCL == 1);  // Wait for master to pull SCL low
    SDA = 1;  // Release SDA

    return data0;
}
