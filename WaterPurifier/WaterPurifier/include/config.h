#ifndef __CONFIG_H__
#define __CONFIG_H__

// #define DEBUG_MODE 1

/* GPIO configuration */
#define FLOW_DETECT    12

/* MQTT configuration */
#define AVAILABLE            "online"
#define UNAVAILABLE          "offline"

#define MSG_ON               "\xbb\x01\xff"
#define MSG_OFF              "\xbb\x02\xff"

#define DEV_NAME             "WaterPurifier"

#define TOPIC_STATE          "/mqtt/sensor/state/" DEV_NAME

#define KEY_TDS1              "tds1"
#define KEY_TDS2              "tds2"
#define KEY_TEMP1             "temp1"
#define KEY_TEMP2             "temp2"
#define KEY_RSSI              "rssi"

#define TOPIC_DEV_IP         "/mqtt/dev/ip/" DEV_NAME
#define TOPIC_DEV_UPDATE     "/mqtt/dev/update/" DEV_NAME
#define TOPIC_AVAILABLE      "/mqtt/sensor/available/" DEV_NAME

#define TOPIC_WATER_DISC     "homeassistant/device/" DEV_NAME "/config"
#define PAYLOAD_WATER_DISC "{\
\"dev\": {\
    \"ids\": \"20250504\",\
    \"name\": \"Penn\",\
    \"mf\": \"Penn\",\
    \"sw\": \"1.0\",\
    \"sn\": \"20250504\"\
},\
\"o\": {\
    \"name\": \"Penn\",\
    \"sw\": \"1.0\",\
    \"url\": \"https://pys1024.top\"\
},\
\"cmps\": {\
    \"component1\": {\
        \"p\": \"sensor\",\
        \"device_class\": \"volatile_organic_compounds_parts\",\
        \"unique_id\": \"" DEV_NAME "_tds1\",\
        \"name\": \"TDS1\",\
        \"suggested_display_precision\": \"1\",\
        \"unit_of_measurement\": \"ppm\",\
        \"value_template\": \"{{ value_json.tds1 }}\",\
        \"retain\": true\
    },\
    \"component2\": {\
        \"p\": \"sensor\",\
        \"device_class\": \"volatile_organic_compounds_parts\",\
        \"unique_id\": \"" DEV_NAME "_tds2\",\
        \"name\": \"TDS2\",\
        \"suggested_display_precision\": \"1\",\
        \"unit_of_measurement\": \"ppm\",\
        \"value_template\": \"{{ value_json.tds2 }}\",\
        \"retain\": true\
    },\
    \"component3\": {\
        \"p\": \"sensor\",\
        \"device_class\": \"temperature\",\
        \"unique_id\": \"" DEV_NAME "_temp1\",\
        \"name\": \"TEMP1\",\
        \"suggested_display_precision\": \"1\",\
        \"unit_of_measurement\": \"°C\",\
        \"value_template\": \"{{ value_json.temp1 }}\",\
        \"retain\": true\
    },\
    \"component4\": {\
        \"p\": \"sensor\",\
        \"device_class\": \"temperature\",\
        \"unique_id\": \"" DEV_NAME "_temp2\",\
        \"name\": \"TEMP2\",\
        \"suggested_display_precision\": \"1\",\
        \"unit_of_measurement\": \"°C\",\
        \"value_template\": \"{{ value_json.temp2 }}\",\
        \"retain\": true\
    },\
    \"component5\": {\
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
