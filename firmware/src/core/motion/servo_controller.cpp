#include "servo_controller.h"
#include <ESP32Servo.h>
#include "../../config.h"

static Servo sCI, sCD, sTI, sTD;

bool ServoController::begin() {
#if SIM_SIN_SERVOS
  Serial.println("[Servo] SIM_SIN_SERVOS=1: sin attach (mesa sin servos)");
  isAttached = false;
  return true;
#else
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  sCI.setPeriodHertz(50);
  sCD.setPeriodHertz(50);
  sTI.setPeriodHertz(50);
  sTD.setPeriodHertz(50);
  sCI.attach(SERVO_CI, 500, 2400);
  sCD.attach(SERVO_CD, 500, 2400);
  sTI.attach(SERVO_TI, 500, 2400);
  sTD.attach(SERVO_TD, 500, 2400);
  isAttached = true;
  Serial.println("[Servo] 4x SG90 attached (32,33,25,26) @50Hz 500-2400us");
  home();
  return true;
#endif
}

void ServoController::applyTrimmed(int idx, float angle) {
  int a = (int)constrain(angle, 0.0f, 180.0f);
#if !SIM_SIN_SERVOS
  switch (idx) {
    case 0: sCI.write(a); break;
    case 1: sCD.write(a); break;
    case 2: sTI.write(a); break;
    case 3: sTD.write(a); break;
  }
#else
  (void)idx; (void)a;
#endif
}

void ServoController::writeAll(float ci, float cd, float ti, float td) {
  applyTrimmed(0, ci + TRIM_CI);
  applyTrimmed(1, cd + TRIM_CD);
  applyTrimmed(2, ti + TRIM_TI);
  applyTrimmed(3, td + TRIM_TD);
}

void ServoController::home() {
#if SIM_SIN_SERVOS
  Serial.println("[SIM] home(90,90,90,90)");
#else
  writeAll(90, 90, 90, 90);
  Serial.println("[Servo] home OK");
#endif
}

void ServoController::stand() {
  // Postura de reposo = home. Se separa por si se quiere ergonomía distinta.
  home();
}

void ServoController::detachAll() {
#if !SIM_SIN_SERVOS
  sCI.detach(); sCD.detach(); sTI.detach(); sTD.detach();
#endif
  isAttached = false;
}
