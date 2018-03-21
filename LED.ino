/***
 * Set the built-in LED state to ON/OFF
 ***/
void LEDSetState(int state) {
  digitalWrite(LED, state);
  ledState = state;
}

/***
 * Give a 1/10th second flash on the LED
 ***/
void LEDQuickFlash() {
  LEDSetState(ON);
  delay(100);
  LEDSetState(OFF);
}

/***
 * Flash the LED fast for 3 seconds indicating it is ready
 ***/
void LEDShowReadyState() {
  
  for (int i = 0; i < 30; i++) {
    ledState = (ledState == ON) ? OFF : ON;
    LEDSetState(ledState);
    delay(100);
  }
}

/***
 * Flash the 1/2s on, 1/2 off pulse to identify the device
 ***/
void LEDIdentify() {
  for (int i = 0; i < 5; i++) {
    LEDSetState(ON);
    delay(500);
    debug(".");
    LEDSetState(OFF);
    delay(500);
  }
}

