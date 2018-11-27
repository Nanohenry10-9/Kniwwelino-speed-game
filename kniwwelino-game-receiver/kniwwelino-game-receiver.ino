#include <ESP8266WiFi.h>
#include <Kniwwelino.h>
extern "C" {
#include <espnow.h>
}

bool r = 0;
char msg = 0;

#define WIFI_CHANNEL 4

uint8_t master1[] = {0x2C, 0x3A, 0xE8, 0x41, 0x26, 0x00};
uint8_t master2[] = {0x2C, 0x3A, 0xE8, 0x41, 0x36, 0xEF};

long countdown = 0;
uint8_t t = 0;

void setup() {
  Kniwwelino.begin();
  Serial.begin(115200);
  Serial.println("Starting...");
  if (esp_now_init() != 0) {
    Serial.println("esp_now error");
    Kniwwelino.MATRIXwriteAndWait("ERROR");
    while (1);
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  //esp_now_add_peer(master1, ESP_NOW_ROLE_CONTROLLER, WIFI_CHANNEL, NULL, 0);
  esp_now_add_peer(master2, ESP_NOW_ROLE_CONTROLLER, WIFI_CHANNEL, NULL, 0);
  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {
    Serial.print("Received");
    r = 1;
    msg = data[0];
  });
  Serial.println("Done");
  Kniwwelino.MATRIXwriteAndWait("Ready");
}

void loop() {
  if (countdown == 0) {
    countdown = millis();
  }
  if (millis() - countdown >= 5000 && t == 0) {
    t = 1;
    Kniwwelino.MATRIXdrawIcon("B1010101110111110111010101");
  }
  if (t == 0) {
    Kniwwelino.MATRIXwrite(String(5 - ((millis() - countdown) / 1000)));
  }
  if (t == 1) {
    Kniwwelino.RGBsetColor(0, 0, 255 * ((millis() / 100) % 2 == 0));
  } else {
    Kniwwelino.RGBsetColor(0, 0, 0);
  }
  if (t == 2 && (Kniwwelino.BUTTONAclicked() || Kniwwelino.BUTTONBclicked())) {
    t = 0;
    countdown = millis();
  }
  if (r) {
    r = 0;
    if (msg == 1 && t == 1) {
      Kniwwelino.MATRIXwriteAndWait("Player 1 wins!");
      t = 2;
    } else if (msg == 2 && t == 1) {
      Kniwwelino.MATRIXwriteAndWait("Player 2 wins!");
      t = 2;
    }
  }
  delay(1);
}
