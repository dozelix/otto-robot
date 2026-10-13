// Otto DIY — Firmware base SDD (ESP32-WROOM, PlatformIO, C++).
// Base: movimiento (walk/turn/dance/home) + proximidad HC-SR04.
// Extras (módulos): WiFi único 2.4 GHz para modos exposición/manual.
//
// Envs:
//   otto_esp32      -> base sin red (ENABLE_WIFI_MODULE=0)
//   otto_esp32_wifi -> base + panel WiFi (ENABLE_WIFI_MODULE=1)
//   test_1sg90      -> 1 servo + sonar (mesa)
//
// Serial 115200:
//   exposition: "dist: X cm" + [Otto]/[SIM]
//   manual: usa el panel WiFi o comandos serie (w/a/s/d/h/b/m/e).

#include <Arduino.h>
#include "config.h"
#include "core/autonomy.h"
#include "core/modes.h"
#include "modules/net_wifi.h"

static void printAyuda() {
  Serial.println("Serie: w=adelante s=atrás a=izq d=der h=home b=baile");
  Serial.println("       m=manual e=exposición ?=esta ayuda");
}

static void handleSerial() {
  while (Serial.available()) {
    String c = Serial.readStringUntil('\n');
    c.trim();
    if (c.length() == 0) continue;
    char k = c.charAt(0);
    // setMode() ya resetea el watchdog internamente.
    if (k == 'm' || k == 'M') setMode(OttoMode::MANUAL);
    else if (k == 'e' || k == 'E') setMode(OttoMode::EXPOSITION);
    else if (k == '?') printAyuda();
    else if (getMode() != OttoMode::MANUAL) {
      Serial.println("pasa a manual primero (m) para comandos serie");
    } else if (k == 'w') manualWalk(2, 1000, 1);
    else if (k == 's') manualWalk(2, 1000, -1);
    else if (k == 'a') manualTurn(2, 1000, -1);
    else if (k == 'd') manualTurn(2, 1000, 1);
    else if (k == 'h') manualHome();
    else if (k == 'b') manualDance();
    else Serial.println("usa w/a/s/d/h/b/m/e/?");
    // Nota: manual*() y setMode() resetean el watchdog solo si el
    // comando es válido; no se notifica en rechazos para que el
    // watchdog siga protegiendo ante comandos inválidos.
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n[Otto-ESP32] Firmware base SDD v0.1");
  Serial.printf("[Otto] SIM=%d WIFI_MODULE=%d\n", SIM_SIN_SERVOS, ENABLE_WIFI_MODULE);
  autonomyInit();
  wifiBegin();  // no-op si ENABLE_WIFI_MODULE=0
  if (wifiConnected()) Serial.printf("[Otto] Panel: http://%s/\n", wifiIP());
  printAyuda();
}

void loop() {
  handleSerial();
  wifiHandle();
  autonomyTick();
}
