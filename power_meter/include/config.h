#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <Arduino.h>


#define DEV_ID               "pwr_meter"
// your AP/router SSID to config your device networking
#define WIFI_SSID            "Aliu-2.4g"
// your AP/router password
#define WIFI_PASS            "Aliu102426"
// the listening port of your MQTT server or MQTT broker
#define MQTT_PORT            1883
// the IP address or domain name of your MQTT server or MQTT broker
#define MQTT_HOST            "192.168.0.111"
// the ID of yourself, client would use this ID register itself to MQTT server
#define MQTT_CLIENT_ID       "ESP Device" DEV_ID
// your MQTT login name, if MQTT server allow anonymous login,any string is OK,
// otherwise, please input valid login name which you had registered
#define MQTT_USER            "mqtt"
// you MQTT login password, same as above
#define MQTT_PASS            "mqtt102426"


/* GPIO configuration */
#define PIN_USER     4
#define PIN_POWER    5
#define PIN_CLK      1
#define PIN_DIN      3
#define PIN_DC       12
#define PIN_CE       14
#define PIN_RST      16
#define PIN_BL       0

#define BL_ON        false
#define BL_OFF       true

#define GPIO_ON                LOW
#define GPIO_OFF               HIGH
#define GPIO_DEFAULT           GPIO_OFF

#define IM1281B_REG_START      0x48
#define IM1281B_REG_NUM        8

/* MQTT configuration */
#define STATE_ON               "ON"
#define STATE_OFF              "OFF"
#define AVAILABLE              "online"
#define UNAVAILABLE            "offline"

#define STATE_DEFAULT          GPIO_DEFAULT == GPIO_ON ? STATE_ON : STATE_OFF

#define TOPIC_IP               "homeassistant/" DEV_ID "/ip"
#define TOPIC_UPGRADE          "homeassistant/" DEV_ID "/upgrade"
#define TOPIC_AVAILABLE        "homeassistant/" DEV_ID "/available"

// power supply switch
#define TOPIC_PWR_STATE        "homeassistant/switch/" DEV_ID "/state"
#define TOPIC_PWR_CMD          "homeassistant/switch/" DEV_ID "/set"
#define TOPIC_PWR_DISC         "homeassistant/switch/" DEV_ID "/config"
#define PAYLOAD_PWR_DISC       "{\
\"name\": \"Power Supply\",\
\"state_topic\": \"" TOPIC_PWR_STATE "\",\
\"command_topic\": \"" TOPIC_PWR_CMD "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"

// sensor 1
#define TOPIC_VOL_STATE         "homeassistant/sensor/" DEV_ID "_vol/state"
#define TOPIC_VOL_DISC          "homeassistant/sensor/" DEV_ID "_vol/config"
#define PAYLOAD_VOL_DISC        "{\
\"name\": \"Voltage\",\
\"device_class\": \"voltage\",\
\"unit_of_measurement\": \"V\",\
\"state_topic\": \"" TOPIC_VOL_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"

// sensor 2
#define TOPIC_CUR_STATE         "homeassistant/sensor/" DEV_ID "_cur/state"
#define TOPIC_CUR_DISC          "homeassistant/sensor/" DEV_ID "_cur/config"
#define PAYLOAD_CUR_DISC        "{\
\"name\": \"Current\",\
\"device_class\": \"current\",\
\"unit_of_measurement\": \"A\",\
\"state_topic\": \"" TOPIC_CUR_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"

// sensor 3
#define TOPIC_POWER_STATE       "homeassistant/sensor/" DEV_ID "_power/state"
#define TOPIC_POWER_DISC        "homeassistant/sensor/" DEV_ID "_power/config"
#define PAYLOAD_POWER_DISC      "{\
\"name\": \"Power\",\
\"device_class\": \"power\",\
\"unit_of_measurement\": \"W\",\
\"state_topic\": \"" TOPIC_POWER_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"

// sensor 4
#define TOPIC_ENERGY_STATE       "homeassistant/sensor/" DEV_ID "_energy/state"
#define TOPIC_ENERGY_DISC        "homeassistant/sensor/" DEV_ID "_energy/config"
#define PAYLOAD_ENERGY_DISC      "{\
\"name\": \"Energy\",\
\"device_class\": \"energy\",\
\"unit_of_measurement\": \"KWh\",\
\"state_topic\": \"" TOPIC_ENERGY_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"


// sensor 5
#define TOPIC_FACTOR_STATE       "homeassistant/sensor/" DEV_ID "_factor/state"
#define TOPIC_FACTOR_DISC        "homeassistant/sensor/" DEV_ID "_factor/config"
#define PAYLOAD_FACTOR_DISC      "{\
\"name\": \"Power factor\",\
\"device_class\": \"power_factor\",\
\"state_topic\": \"" TOPIC_FACTOR_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"

// sensor 6
#define TOPIC_TEMP_STATE       "homeassistant/sensor/" DEV_ID "_temp/state"
#define TOPIC_TEMP_DISC        "homeassistant/sensor/" DEV_ID "_temp/config"
#define PAYLOAD_TEMP_DISC      "{\
\"name\": \"Meter temp\",\
\"device_class\": \"temperature\",\
\"unit_of_measurement\": \"°C\",\
\"state_topic\": \"" TOPIC_TEMP_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"

// sensor 7
#define TOPIC_FREQ_STATE       "homeassistant/sensor/" DEV_ID "_freq/state"
#define TOPIC_FREQ_DISC        "homeassistant/sensor/" DEV_ID "_freq/config"
#define PAYLOAD_FREQ_DISC      "{\
\"name\": \"Frequency\",\
\"device_class\": \"frequency\",\
\"unit_of_measurement\": \"Hz\",\
\"state_topic\": \"" TOPIC_FREQ_STATE "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\"\
}"


#endif
