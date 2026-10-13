#include "gait.h"
#include "servo_controller.h"
#include <Arduino.h>
#include "../config.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static float deg2rad(float d) { return d * (float)M_PI / 180.0f; }

void gaitExecute(ServoController& sc, const GaitParams& p, int steps, int T_ms) {
  if (steps < 1) steps = 1;
  if (T_ms < 400) T_ms = 400;
  if (T_ms > 3000) T_ms = 3000;

#if SIM_SIN_SERVOS
  Serial.printf("[SIM] gait steps=%d T=%d a=[%.0f,%.0f,%.0f,%.0f]\n",
                steps, T_ms, p.a[0], p.a[1], p.a[2], p.a[3]);
  delay((unsigned long)steps * (unsigned long)T_ms / 2);  // simula tiempo
  return;
#endif

  const int slice_ms = 20;  // resolución 50 Hz como el servo
  for (int s = 0; s < steps; s++) {
    unsigned long t0 = millis();
    while (millis() - t0 < (unsigned long)T_ms) {
      float t = (float)(millis() - t0) / (float)T_ms;  // 0..1
      float ang[4];
      for (int i = 0; i < 4; i++) {
        float ph = deg2rad(p.phase_deg[i]);
        ang[i] = 90.0f + p.o[i] + p.a[i] * sin(2.0f * (float)M_PI * t + ph);
      }
      sc.writeAll(ang[0], ang[1], ang[2], ang[3]);
      delay(slice_ms);
    }
  }
  sc.home();
}

void danceBasic(ServoController& sc) {
#if SIM_SIN_SERVOS
  Serial.println("[SIM] danceBasic: giro izq + giro der + shake");
  delay(1200);
  return;
#endif
  Serial.println("[Otto] danceBasic");
  gaitExecute(sc, turnParams(-1), 2, 800);
  gaitExecute(sc, turnParams(1), 2, 800);
  // Shake: oscilación rápida de caderas
  for (int k = 0; k < 2; k++) {
    GaitParams wiggle = {
      {20.0f, 20.0f, 10.0f, 10.0f},
      {0.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 180.0f, 0.0f, 180.0f}
    };
    gaitExecute(sc, wiggle, 1, 600);
  }
  sc.home();
}
