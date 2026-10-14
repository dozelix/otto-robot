#pragma once
// Marcha senoidal (gait) propia para ESP32, sin OttoDIYLib.
// Parámetros puros en gait_params.h (única fuente de verdad).
// Orden servos: [CI, CD, TI, TD] = [caderaIzq, caderaDer, tobilloIzq, tobilloDer].

#include "gait_params.h"

class ServoController;

// Ejecuta la marcha de forma bloqueante (steps ciclos de T_ms).
void gaitExecute(ServoController& sc, const GaitParams& p, int steps, int T_ms);

// Rutina de baile corta para modo exposición (usa walk/turn + giros).
void danceBasic(ServoController& sc);
