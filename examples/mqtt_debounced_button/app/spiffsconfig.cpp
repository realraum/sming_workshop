#include <SmingCore/SmingCore.h>
#include "spiffsconfig.h"

SpiffsConfigStorage SpiffsConfig;


const String DEFAULTLIGHT_SETTINGS_FILE = "defaultlight.conf";
const String NET_SETTINGS_FILE = "net.conf";
const String WIFISSID_SETTINGS_FILE = "wifi.ssid.conf";
const String WIFIPASS_SETTINGS_FILE = "wifi.pass.conf";
const String MQTTCLIENT_SETTINGS_FILE = "mqtt.clientid.conf";
const String MQTTUSER_SETTINGS_FILE = "mqtt.user.conf";
const String MQTTPASS_SETTINGS_FILE = "mqtt.pass.conf";
const String MQTTBROKER_SETTINGS_FILE = "mqttbroker.conf";
const String AUTHTOKEN_SETTINGS_FILE = "authtoken.conf";
const String UPDATE_INTERVAL_SETTINGS_FILE = "updateinterval.conf";
const String DEBOUNCE_INTERVAL_SETTINGS_FILE = "debounceinterval.conf";
const String USEDHCP_SETTINGS_FILE = "dhcp.flag";



void SpiffsConfigStorage::load()
{
	if (exist())
	{
		uint32_t netsettings[4];
		file_t f = fileOpen(NET_SETTINGS_FILE, eFO_ReadOnly);
		fileRead(f, (void*) netsettings, 4*sizeof(uint32_t));
		fileClose(f);
		ip = IPAddress(netsettings[0]);
		netmask = IPAddress(netsettings[1]);
		gw = IPAddress(netsettings[2]);
		mqtt_port = (uint16_t)(netsettings[3]);
		wifi_ssid = fileGetContent(WIFISSID_SETTINGS_FILE);
		wifi_pass = fileGetContent(WIFIPASS_SETTINGS_FILE);
		mqtt_broker = fileGetContent(MQTTBROKER_SETTINGS_FILE);
		mqtt_clientid = fileGetContent(MQTTCLIENT_SETTINGS_FILE);
		mqtt_user = fileGetContent(MQTTUSER_SETTINGS_FILE);
		mqtt_pass = fileGetContent(MQTTPASS_SETTINGS_FILE);
		authtoken = fileGetContent(AUTHTOKEN_SETTINGS_FILE);
		enabledhcp = fileExist(USEDHCP_SETTINGS_FILE);
		f = fileOpen(UPDATE_INTERVAL_SETTINGS_FILE, eFO_ReadOnly);
		fileRead(f, (void*) &publish_interval, sizeof(uint32_t));
		fileClose(f);
		f = fileOpen(DEBOUNCE_INTERVAL_SETTINGS_FILE, eFO_ReadOnly);
		fileRead(f, (void*) &debounce_interval, sizeof(uint32_t));
		fileClose(f);
	}
}

void SpiffsConfigStorage::save()
{
	uint32_t netsettings[4] = {ip,netmask,gw, (uint32_t)mqtt_port};
	file_t f = fileOpen(NET_SETTINGS_FILE, eFO_WriteOnly | eFO_CreateNewAlways);
	fileWrite(f, (void*) netsettings, 4*sizeof(uint32_t));
	fileClose(f);
	fileSetContent(WIFISSID_SETTINGS_FILE, wifi_ssid);
	fileSetContent(WIFIPASS_SETTINGS_FILE, wifi_pass);
	fileSetContent(MQTTBROKER_SETTINGS_FILE, mqtt_broker);
	fileSetContent(MQTTCLIENT_SETTINGS_FILE, mqtt_clientid);
	fileSetContent(MQTTUSER_SETTINGS_FILE, mqtt_user);
	fileSetContent(MQTTPASS_SETTINGS_FILE, mqtt_pass);
	fileSetContent(AUTHTOKEN_SETTINGS_FILE, authtoken);
	if (enabledhcp)
		fileSetContent(USEDHCP_SETTINGS_FILE, "true");
	else
		fileDelete(USEDHCP_SETTINGS_FILE);
	f = fileOpen(UPDATE_INTERVAL_SETTINGS_FILE, eFO_WriteOnly | eFO_CreateNewAlways);
	fileWrite(f, (void*) &publish_interval, sizeof(uint32_t));
	fileClose(f);
	f = fileOpen(DEBOUNCE_INTERVAL_SETTINGS_FILE, eFO_WriteOnly | eFO_CreateNewAlways);
	fileWrite(f, (void*) &debounce_interval, sizeof(uint32_t));
	fileClose(f);
}

bool SpiffsConfigStorage::exist() { return fileExist(NET_SETTINGS_FILE); }