/**
 * Hello World!
 *
 * Basic Blink example with Sming
 *
 */

#include "include/user_config.h"
#include <SmingCore/SmingCore.h>

#define LED_PIN 2 // GPIO2

Timer timer;
auto state = true;

void blink() {
  digitalWrite(LED_PIN, (uint8_t) state);

  state = !state;
}

void showSystemInfo() {
  Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
  Serial.printf("Free Heap: %d\r\n", system_get_free_heap_size());
  Serial.printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
  Serial.printf("System Chip ID: 0x%x\r\n", system_get_chip_id());
  Serial.printf("SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
  Serial.printf("ADC: %f\r\n", (double) system_get_vdd33());
}


void init() {
  System.setCpuFrequency(eCF_80MHz); //CpuFrequency: eCF_80MHz (default) or eCF_160MHz
  system_set_os_print(0); // Zero to disable, 1 to enable. Disable SDK output to serial
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.systemDebugOutput(false); // Enables/disables framework debugging.

  WifiStation.config(WIFI_SSID, WIFI_PWD);

  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(false); // Enables/disables WiFi client mode

  showSystemInfo();

  pinMode(LED_PIN, OUTPUT);
  timer.initializeMs(500, blink).start();
}
