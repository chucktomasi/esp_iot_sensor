// Load runtime parameters from EEPROM
void loadConfig() {
  debugln("Reading parameters from EEPROM...");
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0 + sizeof(ssid), password);
/*
EEPROM.get(0+sizeof(ssid)+sizeof(password), endpoint);
EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(endpoint), endpoint_auth);
EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(endpoint)+sizeof(endpoint_auth), endpoint_fingerprint);
char ok[2+1];
EEPROM.get(0+sizeof(ssid)+sizeof(password)+sizeof(endpoint)+sizeof(endpoint_auth)+sizeof(endpoint_fingerprint), ok);
*/
  char ok[2+1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);

  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
    debugln("  No parameters recovered");
//   endpoint[0] = 0;
//   endpoint_auth[0] = 0;
   
  } else {
    debugln("  Recovered parameters:");
    debug("    WiFi SSID: [");
    debug(ssid);
    debugln("]");
    debug("    Password : [");
    debug(strlen(password) > 0 ? password : "<no password>");
    debugln("]");
//   debugln(endpoint);
//   debugln(strlen(endpoint_auth)>0?"********":"<no auth>");
//   debugln(endpoint_fingerprint);
  }
}

// Store runtime parameters to EEPROM
void saveConfig() {
  debugln("Saving parameters to EEPROM...");
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0 + sizeof(ssid), password);
/* 
EEPROM.put(0+sizeof(ssid)+sizeof(password), endpoint);
EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(endpoint), endpoint_auth);
EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(endpoint)+sizeof(endpoint_auth), endpoint_fingerprint);
char ok[2+1] = "OK";
EEPROM.put(0+sizeof(ssid)+sizeof(password)+sizeof(endpoint)+sizeof(endpoint_auth)+sizeof(endpoint_fingerprint), ok);
  */
  char ok[2+1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);

  EEPROM.commit();
  EEPROM.end();

  debugln("Saved parameters to EEPROM.");
}
