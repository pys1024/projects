#ifndef __CONFIG_H__
#define __CONFIG_H__

/* GPIO configuration */
#define LED_R        4
#define LED_G        5
#define LAMP_R       0
#define LAMP_G       3
#define LAMP_B       2
#define LAMP_Y       1
#define POWER_0      12
#define POWER_1      13
#define TOUCH_PAD    14

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

#define DEV_NAME             "bedside_lamp"
#define TOPIC_STATE          "/mqtt/light/state/" DEV_NAME
#define TOPIC_COMMAND        "/mqtt/light/command/" DEV_NAME
#define TOPIC_AVAILABLE      "/mqtt/light/available/" DEV_NAME
#define TOPIC_RGB_STATE      "/mqtt/light/rgb/state/" DEV_NAME
#define TOPIC_RGB_COMMAND    "/mqtt/light/rgb/command/" DEV_NAME
#define TOPIC_BRIGHT_STATE   "/mqtt/light/bright/state/" DEV_NAME
#define TOPIC_BRIGHT_COMMAND "/mqtt/light/bright/command/" DEV_NAME


// your AP/router SSID to config your device networking
#define WIFI_SSID            "Aliu-2.4g"
// your AP/router password
#define WIFI_PASS            "Aliu102426"
// the listening port of your MQTT server or MQTT broker
#define MQTT_PORT            1883
// the IP address or domain name of your MQTT server or MQTT broker
#define MQTT_HOST            "192.168.0.101"
// the ID of yourself, client would use this ID register itself to MQTT server
#define MQTT_CLIENT_ID       "ESP Device" DEV_NAME
// your MQTT login name, if MQTT server allow anonymous login,any string is OK,
// otherwise, please input valid login name which you had registered
#define MQTT_USER            "homeassistant"
// you MQTT login password, same as above
#define MQTT_PASS            "homeassistant"


#endif
