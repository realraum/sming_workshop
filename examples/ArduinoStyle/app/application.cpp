/**
 * Arduino style does not work in sming
 * the example crashes with "Software Watchdog Reset"
 *
 * @see: SmingTemplate for the right way
 *
 */
#include <SmingCore/SmingCore.h>

#define LED_PIN 2 // LED, TXD1

bool state = true;

void loop();

void init() {
  pinMode(LED_PIN, OUTPUT);
  loop();
}


void loop() {
  while (true) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
}
