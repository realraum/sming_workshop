/**
 * Arduino style does not work with sming. The example runs into a watchdog reset.
 * WDT.alive() can be used in the loop to prevent the watchdog reset. But only for
 * about 3.2 seconds. system_soft_wdt_stop() disables the software watchdog but after
 * about 6 seconds the hardware watchdog resets the system.
 *
 * @see: SmingStyle for the right way
 *
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>


#define LED_PIN 2 // LED, TXD1

// forward declarations
void setup();
void loop();


// this will be called by the the framework after booting
void init() {
  WifiStation.enable(false);
  setup();
  while (true) {
    loop();
  }
}


// begin of arduino style of programming
void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
//  WDT.alive();
}
