#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WebServer.h>
#include "ota.h"

WebServer server(80);
TaskHandle_t Task;
const char* host = "esp32";
bool ledOn = false;
const char* ssid = "yourNetworkName";
const char* password = "yourNetworkPassword";

void handleBlinks(void * pvParameters) {
  while(true) {
    ledOn = true;
    delay(1000);
    ledOn = false;
    delay(1000);
  }
}

void setup() {
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(ssid, password);
  delay(100);
  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 20) {
    count++;
    delay(500);
  }
  while(WiFi.status() != WL_CONNECTED);
  delay(100);
  #include "otaroute.h"
  pinMode(LED_BUILTIN, OUTPUT);
  xTaskCreatePinnedToCore(
    handleBlinks,
    "handleBlinks",
    10000,
    NULL,
    1,
    &Task,
    (xPortGetCoreID() == 1) ? 0 : 1);
}
void loop() {
  int tt = millis();
  digitalWrite(LED_BUILTIN, ledOn);
  while (millis() - tt < 10) {
    server.handleClient();
  }
}
