#define NETWORK_ATTEMPTS 20

void initWifi() {
  // turn off AP (if it's still on)
  WiFi.softAPdisconnect(true);
  // switch to pure station
  WiFi.mode(WIFI_STA);

}

// set pin modes
void setPinModes() {
  
  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
 
}

// build device/AP name
void buildDeviceName() {

  String s1 = WiFi.macAddress();
  String s2 = netnamePrefix + s1.substring(8);
  s2.replace(":","");
  netname = (char *)malloc(s2.length()+1);
  s2.toCharArray(netname,s2.length()+1);
  // strcpy(netname, s.c_str());
  displayText(netname);
  
  debug("Device network name: ");
  debugln(netname);

}

// Return a URL
String buildURL() {
  
  String url = "http://" + String(netname) + ".local/";
  
  return url;
}

void attemptWifi() {

  WiFi.begin(ssid, password);
  int retry = 0;
  debugln("Connecting Wifi");
  displayText("Wifi....");
  while (WiFi.status() != WL_CONNECTED && retry < NETWORK_ATTEMPTS) {
    LEDSetState(ledState);
    debug(".");
    ledState = (ledState == ON) ? OFF : ON;
    retry++;
    delay(1000);
  }
  LEDSetState(OFF);

}

void showWifiSuccess() {
    // connected to ssid, boot normally
    
    debugln("");
    debugln("WiFi connected");
    debug("IP address: ");
    
    debugln(WiFi.localIP());
    displayText("OK");

    debug("Rapid LED blink (3s)...");
    // blink LED rapidly for 3 sec to confirm connection
    LEDShowReadyState();
    debugln("Done.");    
}

void registerMDNS() {
  
    if(MDNS.begin(netname,WiFi.localIP(),120)) {
      debug("mDNS responder started: ");
      debug(netname);
      debugln(".local");

      MDNS.addService("http", "tcp", 80);

      MDNS.update();
      displayText("HTTP....OK");
    } else {
      debugln("Error starting mDNS");
      displayText("mDNS error\n"); 
    }

}

void doAP() {
  clearDisplay();
  displayText("AP mode\n");
  displayText("Network=\n");
  displayText(netname);
  displayText("\n");
  displayText("IP=192.168.4.1");
  LEDSetState(ON);

  debugln("");
  debugln("Configuring access point...");

  // turn on AP mode
  WiFi.mode(WIFI_AP);

  WiFi.softAP(netname);

  IPAddress myIP = WiFi.softAPIP();
  delay(1000);
  debug("AP IP address: ");
  debugln(myIP);

  // fire up web server
  startServer();
}

void displayIPAddress() {

  IPAddress ip;
  ip = WiFi.localIP();

  displayText(ip.toString());
}

