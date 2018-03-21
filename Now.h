#ifdef ARDUINO_ESP8266_NODEMCU
#define LED LED_BUILTIN
#define SENSOR D2
#endif

#ifdef ARDUINO_ESP8266_ESP01
#define LED LED_BUILTIN
#define SENSOR 2
// reusing TX serial pin with LED_BUILTIN, so no serial output
#define NOSERIAL
#endif

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
#define LED LED_BUILTIN
#define SENSOR D0
#endif

/*
 * Next gottcha is on many (most?) ESP8266's the behavior of LED_BUILTIN is
 * reversed, HIGH==off, LOW==on
 * defining ON/OFF lets us keep the same behavior with built-in or LEDs 
 * connected to data pins
*/
#if LED == LED_BUILTIN
#define ON LOW
#define OFF HIGH
#else
#define ON HIGH
#define OFF LOW
#endif

/*
 * If serial is turned off, the debug() macros need to be a noop
 * Gottcha here is if you do something like: 
 * debug("foo"); i++;
 * in which case i++ will not be compiled/executed
*/
#ifdef NOSERIAL
#define debug(x) //(x)
#define debugln(x) //(x)
#else
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#endif

