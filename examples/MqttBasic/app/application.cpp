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
void startMqttClient();
void sendMessage();
void startSystem();


Timer timer;
bool state = true;
MqttClient *mqtt;


void init() {

  WifiStation.config(WIFI_SSID, WIFI_PWD);
  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(true); // Enables/disables WiFi client mode

  pinMode(LED_PIN, OUTPUT);

  System.onReady(startSystem);
}

// start the mqtt client and register callback function for received messages.
// also register a periodic timer for sending messages
void startSystem(){
  mqtt = new MqttClient(MQTT_HOST, MQTT_PORT, onMessageReceived);
  startMqttClient();
  timer.initializeMs(4000, sendMessage).start();
}

// initialize mqtt client
void startMqttClient() {
  mqtt->connect("esp8266_" + system_get_chip_id(), "", "", FALSE);
  mqtt->subscribe("workshop/toggleLed");
}

// when receive a message on topic ""workshop/toggleLed" toggle the led
void onMessageReceived(String topic, String message) {
  digitalWrite(LED_PIN, (uint8_t) state);
  state = !state;
}

// send message "hey!" to the topic "workshop/topic"
void sendMessage() {
  if (mqtt->getConnectionState() != eTCS_Connected) {
    startMqttClient(); // Auto reconnect
  }
  mqtt->publish("workshop/topic", "hey!");
}
