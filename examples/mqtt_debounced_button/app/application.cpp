#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <spiffsconfig.h>
#include <pinconfig.h>
#include <SmingCore/Debug.h>
#include <WS2812/WS2812.h>
#include "application.h"
#include "mqtt.h"
#include "button.h"
#ifdef ENABLE_SSL
	#include <ssl/private_key.h>
	#include <ssl/cert.h>
#endif

Timer mainTimer;

DebouncedButton *mybutton_;
//unique_ptr<DebouncedButton> mybutton_ = nullptr;

///////////////////////////////////////
///// WIFI Stuff
///////////////////////////////////////

// Will be called when WiFi station was connected to AP
void wifiConnectOk(IPAddress ip, IPAddress mask, IPAddress gateway)
{
	debugf("WiFi CONNECTED");
	Serial.println(ip.toString());
	//startTelnetServer();
	startMqttClient();
}

// Will be called when WiFi station timeout was reached
void wifiConnectFail(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason)
{
	// The different reason codes can be found in user_interface.h. in your SDK.
	debugf("Disconnected from %s. Reason: %d", ssid.c_str(), reason);
}

void configureWifi()
{
	WifiAccessPoint.enable(false);
	WifiStation.enable(true);
	debugf("config wifi");
	WifiStation.setHostname(SpiffsConfig.mqtt_clientid+".realraum.at");
	WifiStation.config(SpiffsConfig.wifi_ssid, SpiffsConfig.wifi_pass); // Put you SSID and Password here
	WifiStation.enableDHCP(SpiffsConfig.enabledhcp);
	if (!SpiffsConfig.enabledhcp)
		WifiStation.setIP(SpiffsConfig.ip,SpiffsConfig.netmask,SpiffsConfig.gw);
}

//////////////////////////////////////
////// Base System Stuff  ////////////
//////////////////////////////////////

void setupPINs() {
	pinMode(WS2812_PIN, OUTPUT);
}

const uint8_t ws2812_num_settings_ = 4;
const uint8_t ws2812_bufferlen_ = 3;
uint8_t ws2812_cur_settings_ = 0;

char ws2812_col_[ws2812_num_settings_][ws2812_bufferlen_+1] = {"\xF0\x00\x00","\x00\xF0\x00","\x00\x00\xF0","\x10\x00\x10"};

extern MqttClient *mqtt;

void mainLoop() {
	if (mybutton_->wasPressed())
	{
		ws2812_writergb(WS2812_PIN, ws2812_col_[ws2812_cur_settings_], ws2812_bufferlen_);

		if (mqtt)
			mqtt->publish("action/flodder/light", "{\"g\":"+String((uint16_t)ws2812_col_[ws2812_cur_settings_][0]*1)+",\"r\":"+String((uint16_t)ws2812_col_[ws2812_cur_settings_][1]*1)+",\"b\":"+String((uint16_t)ws2812_col_[ws2812_cur_settings_][2]*1)+"}", false);

		ws2812_cur_settings_ = (ws2812_cur_settings_ +1) % ws2812_num_settings_;
	}
}

void spiffs_mount_with_rboot() {
	// Mount file system, in order to work with files
	int slot = rboot_get_current_rom();
#ifndef DISABLE_SPIFFS
	if (slot == 0) {
#ifdef RBOOT_SPIFFS_0
		debugf("trying to mount spiffs at %x, length %d", RBOOT_SPIFFS_0, SPIFF_SIZE);
		spiffs_mount_manual(RBOOT_SPIFFS_0, SPIFF_SIZE);
#else
		debugf("trying to mount spiffs at %x, length %d", 0x100000, SPIFF_SIZE);
		spiffs_mount_manual(0x100000, SPIFF_SIZE);
#endif
	} else {
#ifdef RBOOT_SPIFFS_1
		debugf("trying to mount spiffs at %x, length %d", RBOOT_SPIFFS_1, SPIFF_SIZE);
		spiffs_mount_manual(RBOOT_SPIFFS_1, SPIFF_SIZE);
#else
		debugf("trying to mount spiffs at %x, length %d", 0x300000, SPIFF_SIZE);
		spiffs_mount_manual(0x300000, SPIFF_SIZE);
#endif
	}
#else
	debugf("spiffs disabled");
#endif
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(true); // Allow debug print to serial
	Serial.commandProcessing(true);
	debugf("init()");
	setupPINs();

	spiffs_mount_with_rboot();
	// spiffs_mount();
	SpiffsConfig.load(); //loads netsettings from fs

	commandHandler.registerSystemCommands();
	mybutton_ = new DebouncedButton(BUTTON_PIN, 30, true);
	//mybutton_.reset(new DebouncedButton(BUTTON_PIN, 30, true));
	//this

	configureWifi();
	WifiEvents.onStationGotIP(wifiConnectOk);
	WifiEvents.onStationDisconnect(wifiConnectFail);
	mainTimer.initializeMs(500, mainLoop).start(true);
}
