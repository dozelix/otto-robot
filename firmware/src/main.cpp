// Otto DIY — Firmware base SDD (ESP32-WROOM, PlatformIO, C++).
// Base: servos + proximidad HC-SR04 + WiFi + dashboard (siempre).
// Buzzer: módulo opcional (modules/), registrado vía sound_hook.
//
// Envs (platformio.ini en raíz del repo):
//   otto_esp32      -> firmware producción
//   manual_1sg90    -> banco 1 servo + sonar (mesa)
//   calibracion_90  -> lleva 4 servos a 90°
//   test_core_esp32 -> Unity en placa
//   native          -> Unity en PC
//
// Serial 115200:
//   exposition: "dist: X cm" + [Otto]/[SIM]
//   manual: usa el panel WiFi o comandos serie (w/a/s/d/h/b/m/e).

#include <Arduino.h>
#include "config.h"
#include "core/autonomy/autonomy.h"
#include "core/modes/modes.h"
#include "core/net/net_wifi.h"
#include "core/audio/sound_hook.h"
#include "modules/buzzer.h"

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
  Serial.printf("[Otto] SIM=%d WIFI=always-on\n", SIM_SIN_SERVOS);
  setSoundHooks(buzzerBegin, buzzBeep, buzzBeepDouble);
  autonomyInit();
  wifiBegin();
  if (wifiConnected()) Serial.printf("[Otto] Panel: http://%s/\n", wifiIP());
  printAyuda();
}

void loop() {
  handleSerial();
  wifiHandle();
  autonomyTick();
}
