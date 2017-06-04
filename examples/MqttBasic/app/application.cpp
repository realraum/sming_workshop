/**
 * Basic MQTT sample
 *
 * Subscribe to to topic "workshop/topic" and publish "hey!" every  4 seconds
 *
 */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#define LED_PIN 2 // LED, TXD1

void onMessageReceived(String topic, String message);

void startMqttClient();

void sendMessage();

Timer timer;
bool state = true;
MqttClient *mqtt;


void connectOk() {
  startMqttClient();
}


// subscribe to a topic (at startMqttClient) to enable the callback function
void onMessageReceived(String topic, String message) {
  digitalWrite(LED_PIN, state);
  state = !state;
}

void startMqttClient() {
  mqtt->connect("esp8266", "", "", FALSE);
  //mqtt->subscribe("workshop/topic");
}

void sendMessage() {
  if (mqtt->getConnectionState() != eTCS_Connected) {
    startMqttClient(); // Auto reconnect
  }
  mqtt->publish("workshop/topic", "hey!");
}

void init() {

  WifiStation.config(WIFI_SSID, WIFI_PWD);

  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(true); // Enables/disables WiFi client mode

  pinMode(LED_PIN, OUTPUT);

  mqtt = new MqttClient(MQTT_HOST, MQTT_PORT, onMessageReceived);


  timer.initializeMs(4000, sendMessage).start();
}

