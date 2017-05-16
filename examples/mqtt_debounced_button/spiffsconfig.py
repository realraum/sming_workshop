#!/usr/bin/python3

from ipaddress import ip_address
import os
import struct

def writeConfig(ip,nm,gw,wifi_ssid,wifi_pass,mqtt_broker,mqtt_clientid,mqtt_user,mqtt_pass,authtoken,dhcp=True,mqtt_port=1883,update_interval=7000,debounce_interval=3500):
    NET_SETTINGS_FILE = "net.conf"
    WIFISSID_SETTINGS_FILE = "wifi.ssid.conf"
    WIFIPASS_SETTINGS_FILE = "wifi.pass.conf"
    MQTTCLIENT_SETTINGS_FILE = "mqtt.clientid.conf"
    MQTTUSER_SETTINGS_FILE = "mqtt.user.conf"
    MQTTPASS_SETTINGS_FILE = "mqtt.pass.conf"
    MQTTBROKER_SETTINGS_FILE = "mqttbroker.conf"
    AUTHTOKEN_SETTINGS_FILE = "authtoken.conf"
    USEDHCP_SETTINGS_FILE = "dhcp.flag"
    UPDATE_INTERVAL_SETTINGS_FILE = "updateinterval.conf"
    DEBOUNCE_INTERVAL_SETTINGS_FILE = "debounceinterval.conf"
    DIR="./files/"
    with open(os.path.join(DIR, NET_SETTINGS_FILE),"wb") as fh:
        fh.write(struct.pack(">III", int(ip_address(ip)), int(ip_address(nm)), int(ip_address(gw))))
        fh.write(struct.pack("<I",  int(mqtt_port)))
    with open(os.path.join(DIR, UPDATE_INTERVAL_SETTINGS_FILE),"wb") as fh:
        fh.write(struct.pack("<I",  int(update_interval)))
    with open(os.path.join(DIR, DEBOUNCE_INTERVAL_SETTINGS_FILE),"wb") as fh:
        fh.write(struct.pack("<I",  int(debounce_interval)))
    with open(os.path.join(DIR, WIFISSID_SETTINGS_FILE),"wb") as fh:
        fh.write(wifi_ssid)
    with open(os.path.join(DIR, WIFIPASS_SETTINGS_FILE),"wb") as fh:
        fh.write(wifi_pass)
    with open(os.path.join(DIR, MQTTBROKER_SETTINGS_FILE),"wb") as fh:
        fh.write(mqtt_broker)
    with open(os.path.join(DIR, MQTTCLIENT_SETTINGS_FILE),"wb") as fh:
        fh.write(mqtt_clientid)
    with open(os.path.join(DIR, MQTTUSER_SETTINGS_FILE),"wb") as fh:
        fh.write(mqtt_user)
    with open(os.path.join(DIR, MQTTPASS_SETTINGS_FILE),"wb") as fh:
        fh.write(mqtt_pass)
    with open(os.path.join(DIR, AUTHTOKEN_SETTINGS_FILE),"wb") as fh:
        fh.write(authtoken)
    if dhcp:
        with open(os.path.join(DIR, USEDHCP_SETTINGS_FILE),"wb") as fh:
            fh.write(b"true")
    else:
        try:
            os.unlink(os.path.join(DIR, USEDHCP_SETTINGS_FILE))
        except:
            pass

## Example Use
# writeConfig(
#     ip="",
#     nm="",
#     gw="",
#     dhcp=True,
#     wifi_ssid=b"",
#     wifi_pass=b"",
#     mqtt_port=1883,
#     mqtt_broker=b"",
#     mqtt_clientid=b"",
#     mqtt_user=b"",
#     mqtt_pass=b"",
#     authtoken=b"")
