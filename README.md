# esp_iot_sensor

Simple ESP8266-based IoT app. Designed as a prototype for ServiceNow IoT lab.

Tested on Wemos D1 mini and OLED shield.

## Details

On boot, reads network SSID/pass from EEPROM and attempts to connect.  If unable to connect, switch to AP mode with iot-<mac_addr> SSID and runs web server with config page @ http://192.168.4.1.  Once page is submitted, SSID/password are stored in EEPROM. If successfully connected to wifi, On board web server continues to run with sample REST endpoints to perform actions (turn on/off display, turn on/off LED, etc.)
