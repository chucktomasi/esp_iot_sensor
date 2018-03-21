
void startServer() {
  server.on("/", homeScreen);
  server.on("/config", handleConfig);
  server.on("/reset", handleReset);
  server.on("/restart", handleRestart);
  server.on("/identify", handleIdentify);
// server.on("/led_on", handleLEDOn);
// server.on("/led_off", handleLEDOff);
  server.on("/test_image", handleTestImage);
  server.on("/clear_display", handleClearDisplay);
  server.begin();
  debugln("HTTP server started");
}

/*   
 * Config page @ http://192.168.4.1 in a web browser
 */
void homeScreen() {

  String url = buildURL();
  displayOLEDState();

  IPAddress ip;
  ip = WiFi.localIP();

  debug("homeScreen(): netname=");
  debugln(netname);

  String response = "<!doctype html><html><head>";
  response += "<title>IoT Properties - ";
  response += netname;
  response += " - ";
  if (WiFi.status() != WL_CONNECTED) {
    response += "AP mode";
  } else {
    response += "Std mode";
  }
  response += "</title>";
  response += "</head><body>";

// Display the current state table
  response += "<h1>IoT Properties - ";
  response += netname;
  response += "</h1>";

    response += "<p><table>";
    response += "  <tr>";
    response += "    <td>";
    response += "       Version:";
    response += "    </td>";
    response += "    <td>";
    response += "       " + String(VERSION);
    response += "    </td>";
    response += "  </tr>";
    response += "  <tr>";
    response += "    <td>";
    response += "       Web addr:";
    response += "    </td>";
    response += "    <td>";
    response += "       <a href=\"" + url + "\">" + url + "</a>";
    response += "    </td>";
    response += "  <tr>";
    response += "    <td>";
    response += "       IP Address:";
    response += "    </td>";
    response += "    <td>";
    response += "       " + ip.toString();
    response += "    </td>";
    response += "  </tr>";
    response += "  </tr>";
    response += "  <tr>";
    response += "    <td>";
    response += "       OLED mode:";
    response += "    </td>";
    response += "    <td>";
    response += "       " + stateToString(oledState);
    response += "    </td>";
    response += "  </tr>";
    response += "</table>";
    response += "<hr />";

    
  if (WiFi.status() != WL_CONNECTED) {
    response += "<h1>AP Mode</h1>";
  }

 
  // Display the WiFi config table

  response += "<h2>Configure</h2>";
  response += "<form method=\"POST\" action=\"/config\">";
  response += "<table>";
  response += "<tr><td>Network ssid</th><td><input type=\"text\" name=\"ssid\" value=\"" + String(ssid) + "\"/></td></tr>";
  response += "<tr><td>Password</th><td><input type=\"password\" name=\"password\" value=\"" + String(password) + "\"/></td></tr>";
  /*
  response += "<tr><td>endpoint</th><td><input type=\"text\" name=\"endpoint\" value=\"" + String(endpoint) + "\" size=\"128\"/></td></tr>";
  response += "<tr><td>endpoint_auth</th><td><input type=\"text\" name=\"endpoint_auth\" value=\"\" size=\"64\"/></td></tr>";
  response += "<tr><td>endpoint_fingerprint</th><td><input type=\"text\" name=\"endpoint_fingerprint\" value=\"" + String(endpoint_fingerprint) + "\" size=\"64\"/></td></tr>";
  */
  response += "</table>";
  response += "<input type=\"submit\" value=\"Configure\"/>";
  response += "</form>";

// Display the test actions
  if (WiFi.status() == WL_CONNECTED) {
    response += "<hr />";
    response += "<h2>Test</h2>";
    response += "<p><table cellpadding=\"20\" border=\"1px\">";
    response += "  <tr>";
    response += "    <td>";
    response += "       <a href=\"/test_image\">Display Test Image</a>";
    response += "    </td>";
    response += "    <td>";
    response += "       <a href=\"/clear_display\">Clear Display</a>";
    response += "    </td>";
    response += "  </tr>";
    response += "  <tr>";
    response += "    <td>";
    response += "      <a href=\"/identify\">Identify this device</a>";
    response += "    </td>";
    response += "  </tr>";
    response += "  <tr>";
    response += "    <td>";
    response += "      <a href=\"/restart\">Restart</a>";
    response += "    </td>";
    response += "    <td>";
    response += "      <a href=\"/reset\">Reset</a>";
    response += "    </td>";
    response += "  </tr>";
    response += "</table>";
    response += "</p>";
  }
  response += "</body></html>";

  server.send(200, "text/html", response);
  debugln("Home screen refreshed");
}

void handleReset() {

  String url = buildURL();

  String response = "{\n";
  response +=       "\t\"status\":\"OK\",\n";
  response +=       "\t\"home\":\"" + url + "\",\n";
  response +=       "\t\"endpoint\":\"" + url + "reset\"\n";
  response +=       "}\n";
  server.send(200,"application/json",response);

  debugln("Resetting...");
  displayText("Resetting\n");

  delay(1000);
  ESP.reset();
}

void handleRestart() {

  String url = buildURL();

  String response = "{\n";
  response +=       "\t\"status\":\"OK\",\n";
  response +=       "\t\"home\":\"" + url + "\",\n";
  response +=       "\t\"endpoint\":\"" + url + "restart\"\n";
  response +=       "}\n";
  server.send(200,"application/json",response);
  
  debugln("Restarting...");
  displayText("Restarting\n");
  delay(1000);
  ESP.restart();
//  homeScreen();
}

void handleIdentify() {
  // suspend sensor handling by flagging sensor as un-initialized
  // save previous state
  int sensor_state = sensor_initialized;
  sensor_initialized = 0;

  String url = buildURL();

  String response = "{\n";
  response +=       "\t\"status\":\"OK\",\n";
  response +=       "\t\"home\":\"" + url + "\",\n";
  response +=       "\t\"endpoint\":\"" + url + "identify\"\n";
  response +=       "}\n";
  server.send(200,"application/json", response);

  debug("Identifying...");
  LEDIdentify();
  debugln("Done!");

  // restore previous state
  sensor_initialized = sensor_state;
  // homeScreen();
}

void handleConfig() {
  
  String url = buildURL();

  String response = "{\"status\":\"OK\"";

  for (int i = 0; i < server.args(); i++) {
    response += ",\"" + server.argName(i) + "\":\"" + server.arg(i) + "\"";
    if (server.argName(i) == "ssid") {
      debug("Found ssid: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(ssid,server.arg(i).length() + 1);
    }
    if (server.argName(i) == "password") {
      debug("Found password: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(password,server.arg(i).length() + 1);
    }
    /*
    if(server.argName(i)=="endpoint") {
      debug("Found endpoint: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(endpoint,server.arg(i).length()+1);
    }
    if(server.argName(i)=="endpoint_auth") {
      debug("Found endpoint_auth: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(endpoint_auth,server.arg(i).length()+1);
    }
    if(server.argName(i)=="endpoint_fingerprint") {
      debug("Found endpoint_fingerprint: ");
      debugln(server.arg(i));
      server.arg(i).toCharArray(endpoint_fingerprint,server.arg(i).length()+1);
    }
    */
  } 

  // store configured values to EEPROM
  saveConfig();

  // Send response back to the requester (browser or REST API)
  response += ",\"endpoint\":\"" + url + "config\"}";
  server.send(200,"application/json", response);
  delay(1000);
  ESP.restart();
}

void handleTestImage() {

  String url = buildURL();
  String response = "{\n";
  response +=       "\t\"status\":\"OK\",\n";
  response +=       "\t\"home\":\"" + url + "\",\n";
  response +=       "\t\"endpoint\":\"" + url + "test_image\"\n";
  response +=       "}\n";

  clearDisplay();
  displayImage();
  
  server.send(200,"application/json", response);
}

void handleClearDisplay() {

  String url = buildURL();
  String response = "{\n";
  response +=       "\t\"status\":\"OK\",\n";
  response +=       "\t\"home\":\"" + url + "\",\n";
  response +=       "\t\"endpoint\":\"" + url + "clear_display\"\n";
  response +=       "}\n";

  clearDisplay();

  server.send(200,"application/json", response);
}

