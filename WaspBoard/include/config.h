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

#define STATE_DEFAULT        GPIO_DEFAULT == GPIO_ON ? STATE_ON : STATE_OFF

#define DEV_NAME             "waspboard"
#define TOPIC_STATE          "/mqtt/light/state/" DEV_NAME
#define TOPIC_COMMAND        "/mqtt/light/command/" DEV_NAME
#define TOPIC_AVAILABLE      "/mqtt/light/available/" DEV_NAME
#define TOPIC_RGB_STATE      "/mqtt/light/rgb/state/" DEV_NAME
#define TOPIC_RGB_COMMAND    "/mqtt/light/rgb/command/" DEV_NAME
#define TOPIC_BRIGHT_STATE   "/mqtt/light/bright/state/" DEV_NAME
#define TOPIC_BRIGHT_COMMAND "/mqtt/light/bright/command/" DEV_NAME

#define TOPIC_DEV_IP         "/mqtt/light/ip/" DEV_NAME
#define TOPIC_DEV_UPDATE     "/mqtt/light/update/" DEV_NAME

#define CLIENT_CONNECT_MAX_RETRY_TIMES 3

// your AP/router SSID to config your device networking
#define WIFI_SSID            "滨江2602"
// your AP/router password
#define WIFI_PASS            "Aliu102426"
// the listening port of your MQTT server or MQTT broker
#define MQTT_PORT            1883
// the IP address or domain name of your MQTT server or MQTT broker
#define MQTT_HOST            "192.168.110.57"
// the ID of yourself, client would use this ID register itself to MQTT server
#define MQTT_CLIENT_ID       "ESP Device" DEV_NAME
// your MQTT login name, if MQTT server allow anonymous login,any string is OK,
// otherwise, please input valid login name which you had registered
#define MQTT_USER            "homeassistant"
// you MQTT login password, same as above
#define MQTT_PASS            "homeassistant"


#endif
