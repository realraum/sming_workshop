/**
 * Hello World in sming. Let the led blink!
 *
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>


#define LED_PIN 2 // LED, TXD1

// forward declaration
void blink();


Timer timer; // timer for blinking
bool state; // state of led

/**
 * Hello World in sming. Let the led blink!
 *
 */

// this will be called by the the framework after booting
void init() {
  pinMode(LED_PIN, OUTPUT);
  timer.initializeMs(1000, blink); //initialize timer; call the function "blink" every second
  timer.start(); // start the timer
}

/**
 * sets the led with the content of the state variable and invert the state variable
 */
void blink() {
  digitalWrite(LED_PIN, (uint8_t) state);
  state = !state;
}