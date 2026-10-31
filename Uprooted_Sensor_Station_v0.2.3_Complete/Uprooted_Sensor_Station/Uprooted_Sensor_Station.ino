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
  // TODO 1: Use station.setupPT100() and print success / error.
  // Hint: if (station.setupPT100()) Serial.println("PT100 ready");
  // If it fails, the board still works as an exit node for other sensors.
}

void setupLoRa() {
  // TODO 2: Start LoRa with station.setupLoRa() and print result.
}

void setupAP() {
  // Students supply the HTML in website.h, not a station name.
  // TODO 3: Start the AP with your HTML page from website.h.
  // Hint: station.setupAP(WIFI_PASSWORD, STUDENT_HTML);
  // Then print station.wifiName() and station.wifiIP().
}

void readTemp() {
  station.readTemp(); // advances the nonblocking MAX31865 measurement
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint < 2000) return;
  lastPrint = millis();
  // TODO 4: Get Reading r = station.local();
  // If r.valid, print r.temperatureC; otherwise print r.fault.
}

void broadcastTemp() {
  unsigned long now = millis();
  if (now - lastSent < BROADCAST_MS) return;
  if (now - lastAttempt < RETRY_MS) return;
  lastAttempt = now;
  // The library prevents overlapping radio transmissions and enforces cooldown.
  // TODO 5: Only update lastSent when station.broadcastTemp() succeeds.
  // With no local MAX31865, there is no local reading to broadcast;
  // incoming LoRa packets are still received and relayed by station.update().
  // Hint: if (station.broadcastTemp()) lastSent = now;
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
