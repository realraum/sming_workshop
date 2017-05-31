/**
 * basic template for CLion IDE
 * open CMakeLists.txt in CLion and use terminal to run make or use Makefile support plugin
 * https://plugins.jetbrains.com/plugin/9333-makefile-support
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#define LED_PIN 2 // GPIO2

Timer timer;
bool state = true;

void GDB_IRAM_ATTR showSystemInfo();
void GDB_IRAM_ATTR blink();

void GDB_IRAM_ATTR init() {
  System.setCpuFrequency(eCF_80MHz); //CpuFrequency: eCF_80MHz (default) or eCF_160MHz
  system_set_os_print(0); // Zero to disable, 1 to enable. Disable SDK output to serial
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.systemDebugOutput(true); // Enables/disables framework debugging.

  WifiStation.config(WIFI_SSID, WIFI_PWD);

  WifiAccessPoint.enable(false); // Enables/disables WiFi Access point mode
  WifiStation.enable(true); // Enables/disables WiFi client mode

  showSystemInfo();

  pinMode(LED_PIN, OUTPUT);
  timer.initializeMs(500, blink).start();
}

void GDB_IRAM_ATTR showSystemInfo() {
  Serial.printf("\r\nsming version: %s\r\n", SMING_VERSION);
  Serial.printf("sdk: v%s\r\n", system_get_sdk_version());
  Serial.printf("free heap: %d\r\n", system_get_free_heap_size());
  Serial.printf("cpu frequency: %d MHz\r\n", system_get_cpu_freq());
  Serial.printf("system chip id: 0x%x\r\n", system_get_chip_id());
  Serial.printf("spi flash id: 0x%x\r\n", spi_flash_get_id());
  Serial.printf("boot mode: %d\r\n", system_get_boot_mode());
  Serial.printf("adc: %d\r\n", system_get_vdd33());
  Serial.printf("%s\r\n\n", WifiStation.getSSID().c_str());
}

void GDB_IRAM_ATTR blink() {
  digitalWrite(LED_PIN, (uint8_t) state);
  state = !state;
}

