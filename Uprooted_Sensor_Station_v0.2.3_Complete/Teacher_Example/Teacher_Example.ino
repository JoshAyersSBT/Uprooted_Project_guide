#include <Arduino.h>
#include <HeltecPT100Kit.h>
#include "website.h"              // Student-authored website HTML

using HeltecPT100Kit::Station;
using HeltecPT100Kit::Reading;

Station station;
const char* WIFI_PASSWORD = "Classroom2026"; // change before using
const unsigned long BROADCAST_MS = 12000;
const unsigned long RETRY_MS = 1000;
unsigned long lastSent = 0, lastAttempt = 0;

void setupPT100() {
  if (station.setupPT100()) Serial.println("PT100 ready");
  else Serial.println("MAX31865 not found: operating as LoRa/Wi-Fi exit node");
}

void setupLoRa() {
  if (station.setupLoRa()) Serial.println("LoRa ready");
  else Serial.println("LoRa setup failed");
}

void setupAP() {
  // Students supply the HTML in website.h, not a station name.
  if (station.setupAP(WIFI_PASSWORD, STUDENT_HTML)) {
    Serial.print("WiFi: "); Serial.println(station.wifiName());
    Serial.print("Website: http://"); Serial.println(station.wifiIP());
  } else {
    Serial.println("AP/website setup failed");
  }
}

void readTemp() {
  station.readTemp(); // advances the nonblocking MAX31865 measurement
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint < 2000) return;
  lastPrint = millis();
  Reading r = station.local();
  if (r.valid) {
    Serial.print("Temp: "); Serial.print(r.temperatureC, 1);
    Serial.println(" C");
  } else {
    Serial.print("Sensor fault / waiting: 0x");
    Serial.println(r.fault, HEX);
  }
}

void broadcastTemp() {
  unsigned long now = millis();
  if (now - lastSent < BROADCAST_MS) return;
  if (now - lastAttempt < RETRY_MS) return;
  lastAttempt = now;
  // The library prevents overlapping radio transmissions and enforces cooldown.
  if (station.broadcastTemp()) lastSent = now;
}

void setup() {
  station.beginBoard(); // Uprooted name is automatic; each radio has its own ID
  setupPT100();
  setupLoRa();
  setupAP();
}

void loop() {
  station.update();  // OLED, HTTP, incoming LoRa and relay
  readTemp();
  broadcastTemp();  // 12-second nonblocking broadcast pace
}
