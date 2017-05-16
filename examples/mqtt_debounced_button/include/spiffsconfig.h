#ifndef INCLUDE_DEFAULTCONFIG_H_
#define INCLUDE_DEFAULTCONFIG_H_
#include "pinconfig.h"

class SpiffsConfigStorage
{
public:
	IPAddress ip = IPAddress(192, 168, 127, 242);
	IPAddress netmask = IPAddress(255,255,255,0);
	IPAddress gw = IPAddress(192, 168, 127, 254);
	String wifi_ssid="realraum";
	String wifi_pass="";
	String mqtt_broker="mqtt.realraum.at";
	String mqtt_clientid="buttontest";
	String mqtt_user;
	String mqtt_pass;
	bool enabledhcp=true;
	uint16_t mqtt_port=1883;  //8883 for ssl
	uint32_t publish_interval=8000;
	uint32_t debounce_interval=300;
	String authtoken;

	void load();
	void save();
	bool exist();

	String getMQTTTopic(String topic3)
	{
		return JSON_TOPIC1+mqtt_clientid+topic3;
	}
};

extern SpiffsConfigStorage SpiffsConfig;

#endif /* INCLUDE_DEFAULTCONFIG_H_ */
