#include <SmingCore/SmingCore.h>
#include <spiffsconfig.h>
#include "pinconfig.h"
#include "mqtt.h"
#include "application.h"

//////////////////////////////////
/////// MQTT Stuff ///////////////
//////////////////////////////////

MqttClient *mqtt = nullptr;


// Check for MQTT Disconnection
void checkMQTTDisconnect(TcpClient& client, bool flag){

	// Called whenever MQTT connection is failed.
	if (flag == true)
	{
		Serial.println("MQTT Broker Disconnected!!");
	}
	else
	{
		Serial.println("MQTT Broker Unreachable!!");
	}
}

// void onMessageDelivered(uint16_t msgId, int type) {
	//Serial.printf("Message with id %d and QoS %d was delivered successfully.", msgId, (type==MQTT_MSG_PUBREC? 2: 1));
// }

// Callback for messages, arrived from MQTT server
void onMessageReceived(String topic, String message)
{
	Serial.print(topic);
	Serial.print(":\r\n\t"); // Pretify alignment for printing
	Serial.println(message);
}

// Run MQTT client, connect to server, subscribe topics
void startMqttClient()
{
	// if mqtt not initialized yet, do so (does DNS lookup, so network needed!!)
	if (nullptr == mqtt)
		mqtt = new MqttClient(SpiffsConfig.mqtt_broker, SpiffsConfig.mqtt_port, onMessageReceived);

/*	if(!mqtt->setWill("last/will","The connection from this device is lost:(", 1, true)) {
		debugf("Unable to set the last will and testament. Most probably there is not enough memory on the device.");
	}
*/
	debugf(">%s<",SpiffsConfig.mqtt_broker.c_str());
	debugf(">%d<",SpiffsConfig.mqtt_port);
	debugf(">%s<",SpiffsConfig.mqtt_clientid.c_str());
	debugf(">%s<",SpiffsConfig.mqtt_user.c_str());
	debugf(">%s<",SpiffsConfig.mqtt_pass.c_str());

	//SSL or not
	bool usessl=false;
#ifdef ENABLE_SSL
	usessl=true;
	mqtt->addSslOptions(SSL_SERVER_VERIFY_LATER);

	mqtt->setSslClientKeyCert(default_private_key, default_private_key_len,
							  default_certificate, default_certificate_len, NULL, true);
#endif

	// Assign a disconnect callback function
	// mqtt->subscribe(SpiffsConfig.getMQTTTopic(JSON_TOPIC3_LIGHT,true));
	debugf("connecting to to MQTT broker");
	WDT.enable(false);	//disable watchdog
	mqtt->connect(SpiffsConfig.mqtt_clientid, SpiffsConfig.mqtt_user, SpiffsConfig.mqtt_pass, usessl);
	mqtt->setKeepAlive(42);
	mqtt->setPingRepeatTime(21);
	mqtt->setCompleteDelegate(checkMQTTDisconnect);
	debugf("connected to MQTT broker");
	WDT.enable(true);	//reenable watchdog
}

void stopMqttClient()
{
	if (nullptr == mqtt)
		return;
	// mqtt->unsubscribe(SpiffsConfig.getMQTTTopic(JSON_TOPIC3_LIGHT,true));
	mqtt->setKeepAlive(0);
	mqtt->setPingRepeatTime(0);
}
