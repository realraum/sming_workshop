/**
 * Simple weather station with MQTT and DHT11 sensor
 * @see user_config.h for WIFI and MQTT setting
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/DHT/DHT.h>

#define LED_PIN 2 // LED
#define DHT_PIN 5 // D1


void onMessageReceived(String topic, String message);
void connectMqttClient();
void initializeMqtt(String ssid, uint8_t ssid_len, uint8_t *bssid, uint8_t channel);
void showSystemInfo();
void publishData();


Timer timer;
bool state = true;
float temperature, humidity;
MqttClient *mqtt;
String mqttClientId;
DHT dht(DHT_PIN);



void init() {

  pinMode(LED_PIN, OUTPUT);

  System.setCpuFrequency(eCF_80MHz); //CpuFrequency: eCF_80MHz (default) or eCF_160MHz
  system_set_os_print(0); // Zero to disable, 1 to enable. Disable SDK output to serial
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.systemDebugOutput(false); // Enables/disables framework debugging.

  WifiStation.config(WIFI_SSID, WIFI_PWD);
  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(true); // Enables/disables WiFi client mode

  mqttClientId = "esp8266_" + String(system_get_chip_id());

  showSystemInfo();


  //disable watchdog
  WDT.enable(false);
  //wait for sensor startup
  delay(2000);
  dht.begin();
  // enable watchdog
  WDT.enable(true);


  WifiEvents.onStationConnect(initializeMqtt);

}

// initialize mqtt client and register a periodic timer for sending messages
void initializeMqtt(String ssid, uint8_t ssid_len, uint8_t *bssid, uint8_t channel){
  mqtt = new MqttClient(MQTT_HOST, MQTT_PORT, onMessageReceived);
  timer.initializeMs(4000, publishData).start();
}

// connect to the  mqtt broker
void connectMqttClient() {
  Serial.printf("\r\n\nconnect to: %s\r\n\n", MQTT_HOST);
  mqtt->connect(mqttClientId, "", "", FALSE);
}

// read the DHT11 sensor values and publish it in the topics "sensor001/temperature" and "sensor001/humidity"
// with retain flag to preserve the last sensor readings
void publishData() {

  digitalWrite(LED_PIN, (uint8_t) state);
  state = !state;

  temperature = dht.readTemperature(false);
  humidity = dht.readHumidity();

  Serial.printf("\r\n\nTemperature: %f\r\n", temperature);
  Serial.printf("Humidity: %f\r\n", humidity);

  if (mqtt->getConnectionState() != eTCS_Connected) {
    connectMqttClient(); // Auto reconnect
  }

  Serial.println("Let's publish message now!");
  mqtt->publish("sensor001/temperature", String(temperature), true); // publish with retain flag
  mqtt->publish("sensor001/humidity", String(humidity), true);
}


// callback for received messages; not used in this example
void onMessageReceived(String topic, String message) {
}


// prints some basic information about the system
void showSystemInfo() {
  Serial.printf("\r\nsming version: %s\r\n", SMING_VERSION);
  Serial.printf("sdk: v%s\r\n", system_get_sdk_version());
  Serial.printf("free heap: %d\r\n", system_get_free_heap_size());
  Serial.printf("cpu frequency: %d MHz\r\n", system_get_cpu_freq());
  Serial.printf("system chip id: 0x%x\r\n", system_get_chip_id());
  Serial.printf("spi flash id: 0x%x\r\n", spi_flash_get_id());
  Serial.printf("boot mode: %d\r\n", system_get_boot_mode());
  Serial.printf("adc: %d\r\n", system_get_vdd33());
  Serial.printf("wifi ssid: %s\r\n", WifiStation.getSSID().c_str());
  Serial.printf("mqtt client id: %s\r\n", mqttClientId.c_str());
}

