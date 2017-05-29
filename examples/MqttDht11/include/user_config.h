#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif
#ifndef WIFI_SSID
  #define WIFI_SSID "PleaseEnterSSID"
  #define WIFI_PWD "PleaseEnterPass"
#endif

// ... and/or MQTT username and password
#ifndef MQTT_USERNAME
  #define MQTT_USERNAME ""
  #define MQTT_PWD ""
#endif

// ... and/or MQTT host and port
#ifndef MQTT_HOST
  #define MQTT_HOST "192.168.0.5"
  #ifndef ENABLE_SSL
    #define MQTT_PORT 1883
  #else
    #define MQTT_PORT 8883
  #endif
#endif

// UART config
#define SERIAL_BAUD_RATE 115200

// ESP SDK config
#define LWIP_OPEN_SRC
#define USE_US_TIMER

// Default types
#define __CORRECT_ISO_CPP_STDLIB_H_PROTO
#include <limits.h>
#include <stdint.h>

// Override c_types.h include and remove buggy espconn
#define _C_TYPES_H_
#define _NO_ESPCON_

// Updated, compatible version of c_types.h
// Just removed types declared in <stdint.h>
#include <espinc/c_types_compatible.h>

// System API declarations
#include <esp_systemapi.h>

// C++ Support
#include <esp_cplusplus.h>
// Extended string conversion for compatibility
#include <stringconversion.h>
// Network base API
#include <espinc/lwip_includes.h>

// Beta boards
#define BOARD_ESP01

#ifdef __cplusplus
}
#endif

#endif
