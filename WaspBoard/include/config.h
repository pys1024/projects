#ifndef __CONFIG_H__
#define __CONFIG_H__

/* GPIO configuration */
#define LED_R        12
#define LED_G        16
#define LED_B        14
#define TOUCH_PAD    4

#define GPIO_ON      HIGH
#define GPIO_OFF     LOW
#define GPIO_DEFAULT GPIO_OFF

/* MQTT configuration */
#define STATE_ON             "ON"
#define STATE_OFF            "OFF"
#define AVAILABLE            "online"
#define UNAVAILABLE          "offline"

#define MSG_ON               "\xbb\x01\xff"
#define MSG_OFF              "\xbb\x02\xff"

#define STATE_DEFAULT        (GPIO_DEFAULT == GPIO_ON ? STATE_ON : STATE_OFF)

#define DEV_NAME             "waspboard"

#define TOPIC_STATE          "/mqtt/light/state/" DEV_NAME
#define TOPIC_COMMAND        "/mqtt/light/command/" DEV_NAME

#define KEY_STATE             "state"
#define KEY_COLOR_MODE        "color_mode"
#define KEY_COLOR             "color"
#define KEY_BRIGHTNESS        "brightness"
#define KEY_COLOR_TEMP        "color_temp"
#define KEY_EFFECT            "effect"
#define KEY_RSSI              "rssi"

#define TOPIC_DEV_IP         "/mqtt/dev/ip/" DEV_NAME
#define TOPIC_DEV_UPDATE     "/mqtt/dev/update/" DEV_NAME
#define TOPIC_AVAILABLE      "/mqtt/light/available/" DEV_NAME

#define TOPIC_LIGHT_DISC     "homeassistant/device/" DEV_NAME "/config"
#define PAYLOAD_LIGHT_DISC   "{\
\"dev\": {\
    \"ids\": \"102426\",\
    \"name\": \"Penn\",\
    \"mf\": \"Penn\",\
    \"sw\": \"1.0\",\
    \"sn\": \"1234567890\"\
},\
\"o\": {\
    \"name\": \"Penn\",\
    \"sw\": \"1.0\",\
    \"url\": \"https://pys1024.top\"\
},\
\"cmps\": {\
    \"component\": {\
        \"p\": \"light\",\
        \"unique_id\": \"" DEV_NAME "12138\",\
        \"name\": \"Wasp Light\",\
        \"schema\": \"json\",\
        \"brightness\": false,\
        \"supported_color_modes\": [\"rgb\"],\
        \"retain\": true\
    },\
    \"component2\": {\
        \"p\": \"sensor\",\
        \"device_class\": \"signal_strength\",\
        \"unique_id\": \"" DEV_NAME "_rssi\",\
        \"name\": \"RSSI\",\
        \"suggested_display_precision\": \"0\",\
        \"unit_of_measurement\": \"dBm\",\
        \"value_template\": \"{{ value_json.rssi }}\",\
        \"retain\": true\
    }\
},\
\"state_topic\": \"" TOPIC_STATE "\",\
\"command_topic\": \"" TOPIC_COMMAND "\",\
\"availability_topic\": \"" TOPIC_AVAILABLE "\",\
\"qos\": 1\
}"


#define CLIENT_CONNECT_MAX_RETRY_TIMES 3

// your AP/router SSID to config your device networking
#define WIFI_SSID            "滨江2602"
// your AP/router password
#define WIFI_PASS            "Aliu102426"
// the listening port of your MQTT server or MQTT broker
#define MQTT_PORT            1883
// the IP address or domain name of your MQTT server or MQTT broker
// #define MQTT_HOST            "192.168.110.57" // dev pc ip
#define MQTT_HOST            "192.168.110.50" // homeassistant
// the ID of yourself, client would use this ID register itself to MQTT server
#define MQTT_CLIENT_ID       "ESP Device" DEV_NAME
// your MQTT login name, if MQTT server allow anonymous login,any string is OK,
// otherwise, please input valid login name which you had registered
#define MQTT_USER            "homeassistant"
// you MQTT login password, same as above
#define MQTT_PASS            "homeassistant"


#endif
