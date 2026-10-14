#include "autonomy.h"
#include "../motion/servo_controller.h"
#include "../sonar/sonar.h"
#include "../audio/sound_hook.h"
#include "../motion/gait.h"
#include "../validate/validate.h"
#include <Arduino.h>
#include "../../config.h"

static ServoController sc;
static OttoMode currentMode =
#if DEFAULT_MODE_EXPOSITION
  OttoMode::EXPOSITION;
#else
  OttoMode::MANUAL;
#endif
static unsigned long lastManualMs = 0;
static float lastDist = -1.0f;
static int invalidCount = 0;
static int expoCycle = 0;

void autonomyInit() {
  sonarBegin();
  soundBegin();
  sc.begin();
  lastManualMs = millis();
  invalidCount = 0;
  Serial.printf("[Otto] autonomyInit mode=%s SIM=%d\n", modeName(currentMode), SIM_SIN_SERVOS);
#if SIM_SIN_SERVOS
  Serial.println("[SIM] Sin SG90: acerca la mano <20cm -> [SIM] giro + beep");
#else
  sc.home();
  soundBeep(880, 120);
#endif
}

void setMode(OttoMode m) {
  if (m == currentMode) return;
  currentMode = m;
  lastManualMs = millis();
  Serial.printf("[Otto] mode -> %s\n", modeName(currentMode));
  sc.home();
  soundBeep(m == OttoMode::MANUAL ? 1200 : 880, 150);
}

OttoMode getMode() { return currentMode; }
bool isAutonomyEnabled() { return currentMode == OttoMode::EXPOSITION; }
void notifyManualCommand() { lastManualMs = millis(); }
float lastDistanceCm() { return lastDist; }

static bool checkManual(int steps, int T_ms) {
  return isValidSteps(steps, MANUAL_MAX_STEPS) &&
         isValidPeriodMs(T_ms, MANUAL_MIN_T_MS, MANUAL_MAX_T_MS);
}

bool manualWalk(int steps, int T_ms, int dir) {
  if (currentMode != OttoMode::MANUAL) return false;
  if (!checkManual(steps, T_ms)) return false;
  if (dir != 1 && dir != -1) return false;
  notifyManualCommand();
  Serial.printf("[Manual] walk steps=%d T=%d dir=%d\n", steps, T_ms, dir);
  gaitExecute(sc, walkParams(dir), steps, T_ms);
  return true;
}

bool manualTurn(int steps, int T_ms, int dir) {
  if (currentMode != OttoMode::MANUAL) return false;
  if (!checkManual(steps, T_ms)) return false;
  if (dir != 1 && dir != -1) return false;
  notifyManualCommand();
  Serial.printf("[Manual] turn steps=%d T=%d dir=%d\n", steps, T_ms, dir);
  gaitExecute(sc, turnParams(dir), steps, T_ms);
  return true;
}

bool manualHome() {
  if (currentMode != OttoMode::MANUAL) return false;
  notifyManualCommand();
  Serial.println("[Manual] home");
  sc.home();
  return true;
}

bool manualDance() {
  if (currentMode != OttoMode::MANUAL) return false;
  notifyManualCommand();
  Serial.println("[Manual] danceBasic");
  danceBasic(sc);
  return true;
}

bool manualBeep(int freq, int ms) {
  if (currentMode != OttoMode::MANUAL) return false;
  if (!isValidBeepFreq(freq)) return false;
  if (!isValidBeepMs(ms)) return false;
  notifyManualCommand();
  soundBeep(freq, ms);
  return true;
}

void autonomyTick() {
  // ---- Modo manual: solo watchdog ----
  if (currentMode == OttoMode::MANUAL) {
    if (shouldWatchdogRevert(millis(), lastManualMs, MANUAL_WATCHDOG_MS)) {
      Serial.println("[Manual] watchdog sin comandos -> vuelvo a EXPOSITION");
      setMode(OttoMode::EXPOSITION);
    }
    delay(50);
    return;
  }

  // ---- Modo exposición: baila + proximidad ----
  float d = sonarReadCm();
  lastDist = d;

  if (d < 0) {
    Serial.println("dist: fuera de rango");
    invalidCount++;
    if (shouldSafeHome(invalidCount, MAX_LECTURAS_INVALIDAS)) {
      Serial.println("[Otto] sensor inválido xN -> SAFE_HOME + beep lento");
      sc.home();
      soundBeep(440, 300);
      invalidCount = 0;
    }
    delay(PERIODO_LECTURA_MS);
    return;
  }

  invalidCount = 0;
  Serial.printf("dist: %.1f cm\n", d);

  if (shouldAvoid(d, UMBRAL_OBSTACULO_CM)) {
    Serial.println("[Otto] obstáculo -> turn + beep");
#if SIM_SIN_SERVOS
    Serial.println("[SIM] obstáculo -> [SIM] turn + beep");
    soundBeepDouble();
#else
    soundBeep(1200, 150);
    gaitExecute(sc, turnParams(1), 2, TIEMPO_PASO_MS);
#endif
  } else {
    expoCycle++;
    if (expoCycle % 3 == 0) {
#if SIM_SIN_SERVOS
      Serial.println("[SIM] libre -> [SIM] danceBasic");
      delay(600);
      soundBeep(880, 100);
#else
      Serial.println("[Otto] libre -> danceBasic");
      danceBasic(sc);
#endif
    } else {
#if SIM_SIN_SERVOS
      Serial.println("[SIM] libre -> [SIM] walk(2)");
      delay(400);
#else
      Serial.println("[Otto] libre -> walk(2)");
      gaitExecute(sc, walkParams(1), 2, TIEMPO_PASO_MS);
#endif
    }
  }
  delay(PERIODO_LECTURA_MS);
}
