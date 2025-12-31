#pragma once


// GPIO Configuration
#define POWER_EN_PIN       12 // GPIO pin to enable power
#define BAT_DET            1  // GPIO pin for battery detection
#define KEYX_PIN           41 // GPIO pin for key X

#define LX_PIN             5 // GPIO pin for joystick left X-axis
#define LY_PIN             4 // GPIO pin for joystick left Y-axis
#define RX_PIN             9 // GPIO pin for joystick right X-axis
#define RY_PIN             10 // GPIO pin for joystick right Y-axis

#define MCP_I2C_SDA_PIN    13 // GPIO pin for MCP23X17 SDA
#define MCP_I2C_SCK_PIN    14 // GPIO pin for MCP23X17 SCK

#define EC1_DT_PIN         21  // GPIO pin for Encoder 1 DT
#define EC1_CLK_PIN        47  // GPIO pin for Encoder 1 CLK
#define EC2_DT_PIN         48  // GPIO pin for Encoder 2 DT
#define EC2_CLK_PIN        35  // GPIO pin for Encoder 2 CLK

#define NRF_CSN_PIN        40 // GPIO pin for NRF24L01 CSN
#define NRF_CE_PIN         36 // GPIO pin for NRF24L01 CE
#define NRF_MOSI_PIN       39 // GPIO pin for NRF24L01 MOSI
#define NRF_MISO_PIN       38 // GPIO pin for NRF24L01 MISO
#define NRF_SCK_PIN        37 // GPIO pin for NRF24L01 SCK

#define LCD_BLK_PIN        11 // GPIO pin for LCD backlight
#define LCD_DC_PIN         8  // GPIO pin for LCD DC
#define LCD_CS_PIN         18 // GPIO pin for LCD CS
#define LCD_SCK_PIN        17 // GPIO pin for LCD SCK
#define LCD_MOSI_PIN       16 // GPIO pin for LCD MOSI
#define LCD_RST_PIN        15 // GPIO pin for LCD Reset

#define TP_SCK_PIN         7  // GPIO pin for touchpanel SCK
#define TP_SDA_PIN         6  // GPIO pin for touchpanel SDA
#define TP_INT_PIN         2  // GPIO pin for touchpanel INT
#define TP_RST_PIN         42 // GPIO pin for touchpanel RST

// MCP23X17 Configuration
#define SW1_PIN         2 // GPIO pin for a switch input
#define SW2_PIN         1 // GPIO pin for a switch input
#define SW3_PIN         0 // GPIO pin for a switch input

#define SW5_PIN         15 // GPIO pin for a switch input
#define SW6_PIN         14 // GPIO pin for a switch input
#define SW7_PIN         12 // GPIO pin for a switch input
#define SW8_PIN         13 // GPIO pin for a switch input

#define BEEP_PIN        5 // GPIO pin for beep sound
