/**
 * Simple weather station with MQTT and DHT11 sensor
 * @see user_config.h for WIFI and MQTT setting
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/DHT/DHT.h>
#include <system/include/esp_systemapi.h>
#include "../../../../../esp8266/esp-open-sdk/sdk/include/user_interface.h"

#define LED_PIN 2 // LED
#define DHT_PIN 5 // D1

void onMessageReceived(String topic, String message);
void startMqttClient();
void showSystemInfo();


Timer timer;
bool state = true;
float temperature, humidity;
MqttClient *mqtt;

DHT dht(DHT_PIN);



void publishData() {

  digitalWrite(LED_PIN, (uint8_t) state);
  state = !state;

  temperature = dht.readTemperature(false);
  humidity = dht.readHumidity();

  Serial.printf("\r\n\nTemperature: %f\r\n", temperature);
  Serial.printf("Humidity: %f\r\n", humidity);

  if (mqtt->getConnectionState() != eTCS_Connected) {
    startMqttClient(); // Auto reconnect
  }
  Serial.println("Let's publish message now!");
  mqtt->publish("sensor001/temperature", String(temperature));
  mqtt->publish("sensor001/humidity", String(humidity));
}



void onMessageReceived(String topic, String message) {
  Serial.print(topic);
  Serial.print(":\r\n\t"); // Pretify alignment for printing
  Serial.println(message);
}


void startMqttClient() {
  mqtt->connect("esp8266", MQTT_USERNAME, MQTT_PWD, (boolean) true);
  Serial.print("\rConnected to MQTT server\n");
}

void init() {

  System.setCpuFrequency(eCF_80MHz); //CpuFrequency: eCF_80MHz (default) or eCF_160MHz
  system_set_os_print(0); // Zero to disable, 1 to enable. Disable SDK output to serial
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.systemDebugOutput(false); // Enables/disables framework debugging.

  WifiStation.config(WIFI_SSID, WIFI_PWD);

  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(true); // Enables/disables WiFi client mode

  showSystemInfo();

  pinMode(LED_PIN, OUTPUT);

  //disable watchdog
  WDT.enable(false);

  //wait for sensor startup
  delay(2000);
  dht.begin();
  WDT.enable(true);

  mqtt = new MqttClient(MQTT_HOST, MQTT_PORT, onMessageReceived);

  timer.initializeMs(4000, publishData).start();

}

void showSystemInfo() {
  Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
  Serial.printf("Free Heap: %d\r\n", system_get_free_heap_size());
  Serial.printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
  Serial.printf("System Chip ID: 0x%x\r\n", system_get_chip_id());
  Serial.printf("SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
  Serial.printf("ADC: %f\r\n", system_get_vdd33());
}
