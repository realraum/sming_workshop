/**
 * Basic MQTT sample
 *
 * Subscribe to to topic "workshop/topic" and publish "hey!" every  4 seconds
 *
 */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#define LED_PIN 2 // LED, TXD1



// forward declarations
void onMessageReceived(String topic, String message);
void connectMqttClient();
void sendMessage();
void initializeMqtt(String ssid, uint8_t ssid_len, uint8_t *bssid, uint8_t channel);


Timer timer;
bool state = true;
MqttClient *mqtt = nullptr;
String mqttClientId;

void init() {
  pinMode(LED_PIN, OUTPUT);

  WifiStation.config(WIFI_SSID, WIFI_PWD);
  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(true); // Enables/disables WiFi client mode

  mqttClientId = "esp8266_" + String(system_get_chip_id());

  WifiEvents.onStationConnect(initializeMqtt);
}


// initialize the mqtt client and register callback function for received messages.
// also register a periodic timer for sending messages
void initializeMqtt(String ssid, uint8_t ssid_len, uint8_t *bssid, uint8_t channel){
  mqtt = new MqttClient(MQTT_HOST, MQTT_PORT, onMessageReceived);
  connectMqttClient();
  timer.initializeMs(4000, sendMessage).start();
}

// connect to the  mqtt broker and subscribe to the "workshop/toggleLed" topic
void connectMqttClient() {
  if (nullptr == mqtt)
    return;
  mqtt->connect(mqttClientId, "", "", FALSE);
  mqtt->subscribe("workshop/toggleLed");
}

// when receive a message on topic ""workshop/toggleLed" toggle the led
void onMessageReceived(String topic, String message) {
  digitalWrite(LED_PIN, (uint8_t) state);
  state = !state;
}

// send message "hey!" to the topic "workshop/topic"
void sendMessage() {
  if (nullptr == mqtt)
    return;
  if (mqtt->getConnectionState() != eTCS_Connected) {
    connectMqttClient(); // Auto reconnect
  }
  mqtt->publish("workshop/topic", "hey!");
}
