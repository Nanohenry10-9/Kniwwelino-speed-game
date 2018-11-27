#include <ESP8266WiFi.h>
#include <Kniwwelino.h>
extern "C" {
#include <espnow.h>
}

#define WIFI_CHANNEL 4

uint8_t slave[] = {0xEC, 0xFA, 0xBC, 0x07, 0x14, 0xEC};

uint8_t data[] = {(uint8_t)2}; // Change this per board

void setup() {
  Kniwwelino.begin();
  Serial.begin(115200);
  Serial.println("Starting...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != 0) {
    Serial.println("esp_now error");
    Kniwwelino.MATRIXwriteAndWait("ERROR");
    while (1);
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(slave, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);
  Serial.println("Done");
  Kniwwelino.MATRIXwriteAndWait("Ready");
}

void loop() {
  if (Kniwwelino.BUTTONAdown() || Kniwwelino.BUTTONBdown()) {
    Serial.println("Sending...");
    esp_now_send(NULL, data, 1);
    Serial.println("Sent");
    Kniwwelino.RGBsetColor(0, 0, 255);
    delay(10);
    Kniwwelino.RGBsetColor(0, 0, 0);
    while (Kniwwelino.BUTTONAdown() || Kniwwelino.BUTTONBdown()) {
      delay(1);
    }
  }
  delay(1);
}
