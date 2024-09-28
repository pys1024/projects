# Allows PlatformIO to upload directly to AsyncElegantOTA
#
# To use:
# - copy this script into the same folder as your platformio.ini
# - set the following for your project in platformio.ini:
#
# extra_scripts = platformio_upload.py
# upload_protocol = custom
# upload_url = <your upload URL>
# 
# An example of an upload URL:
# upload_URL = http://192.168.1.123/update

import requests
import hashlib
import time
from paho.mqtt import client as mqtt
Import('env')

MQTT_SERVER = '192.168.0.111'
MQTT_PORT = 1883
MQTT_USER = 'mqtt'
MQTT_PASSWD = 'mqtt102426'

try:
    from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
    from tqdm import tqdm
except ImportError:
    env.Execute("$PYTHONEXE -m pip install requests_toolbelt")
    env.Execute("$PYTHONEXE -m pip install tqdm")
    from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
    from tqdm import tqdm

dev_ip = None
def get_url(dev_id):
    topic_ip = f'homeassistant/{dev_id}/ip'
    topic_upgrade = f'homeassistant/{dev_id}/upgrade'

    def on_msg(client, userdata, msg):
        print('ON_MSG <= ' + msg.topic + ': ' + str(msg.payload))
        global dev_ip
        if (msg.topic == topic_ip):
            dev_ip = str(msg.payload.decode())
            client.publish(topic_upgrade, 'go', 1)
            time.sleep(2)
            client.disconnect()

    def on_connect(client, userdata, flags, rc):
        print('MQTT server connected with result code: ' + str(rc))
        client.subscribe(topic_ip)

    if dev_id:
        print(f'MQTT server: {MQTT_SERVER}:{MQTT_PORT}')
        client = mqtt.Client('mqtt.test')
        client.username_pw_set(MQTT_USER, MQTT_PASSWD)
        client.on_connect = on_connect
        client.on_message = on_msg
        client.connect(MQTT_SERVER, MQTT_PORT, 60)
        client.loop_forever()

    print(f'Get device IP: {dev_ip}')
    return dev_ip

def on_upload(source, target, env):
    firmware_path = str(source[0])
    upload_dev = env.GetProjectOption('upload_dev', None)
    upload_ip = get_url(upload_dev)
    if upload_ip:
        upload_url = f'http://{upload_ip}/update'
    else:
        upload_url = env.GetProjectOption('upload_url')

    print(f'Upload URL: {upload_url}')
    with open(firmware_path, 'rb') as firmware:
        md5 = hashlib.md5(firmware.read()).hexdigest()
        firmware.seek(0)
        encoder = MultipartEncoder(fields={
            'MD5': md5,
            'firmware': ('firmware', firmware, 'application/octet-stream')}
        )

        bar = tqdm(desc='Upload Progress',
              total=encoder.len,
              dynamic_ncols=True,
              unit='B',
              unit_scale=True,
              unit_divisor=1024
              )

        monitor = MultipartEncoderMonitor(encoder, lambda monitor:
                                          bar.update(monitor.bytes_read - bar.n))

        response = requests.post(upload_url, data=monitor,
                                 headers={'Content-Type': monitor.content_type})
        bar.close()
        print(response,response.text)

env.Replace(UPLOADCMD=on_upload)
