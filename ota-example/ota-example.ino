WebServer server(80);
const char* host = "esp32";
bool ledOn = false;
void setup() {
  #include "otaroute.h"
}
void loop() {
  int tt = millis();
  
  while (millis() - tt < 40) {
    server.handleClient();
  }
}
