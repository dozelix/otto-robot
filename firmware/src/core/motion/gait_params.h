#pragma once
// Parámetros de marcha puros (sin Arduino). Única fuente de verdad.
// Valores compatibles con firmware-rust/src/gait.rs.
// Orden servos: [CI, CD, TI, TD] = [caderaIzq, caderaDer, tobilloIzq, tobilloDer].

struct GaitParams {
  float a[4];          // amplitud por servo (grados)
  float o[4];          // offset respecto a 90° (grados)
  float phase_deg[4];  // fase (grados)
};

// dir: +1 adelante, -1 atrás. Caderas en fase, pies en fase, 90° cadera-pie.
inline GaitParams walkParams(int dir) {
  float p = (dir >= 0) ? -90.0f : 90.0f;
  GaitParams g = {
    {30.0f, 30.0f, 20.0f, 20.0f},
    {0.0f, 0.0f, 4.0f, -4.0f},
    {0.0f, 0.0f, p, p}
  };
  return g;
}

// dir: +1 derecha, -1 izquierda. Fases opuestas entre lados.
inline GaitParams turnParams(int dir) {
  float p = (dir >= 0) ? 90.0f : -90.0f;
  GaitParams g = {
    {30.0f, 30.0f, 20.0f, 20.0f},
    {0.0f, 0.0f, 4.0f, -4.0f},
    {0.0f, 180.0f, p, p + 180.0f}
  };
}
