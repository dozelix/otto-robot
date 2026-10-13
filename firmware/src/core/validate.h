#pragma once
// Validación pura (sin Arduino, sin config): reglas de seguridad de los
// comandos manuales y decisiones de la máquina de estados.
// Testeable en host: tests/test_validate.py + firmware/test/test_core/.
// autonomy.cpp pasa los umbrales de config.h como parámetros.

inline bool isValidSteps(int steps, int maxSteps) {
  return steps >= 1 && steps <= maxSteps;
}

inline bool isValidPeriodMs(int tMs, int minMs, int maxMs) {
  return tMs >= minMs && tMs <= maxMs;
}

inline bool isValidDir(int dir) {
  return dir == 1 || dir == -1;
}

inline bool isValidBeepFreq(int freq) {
  return freq >= 100 && freq <= 5000;
}

inline bool isValidBeepMs(int ms) {
  return ms >= 20 && ms <= 1000;
}

inline bool isValidManual(int steps, int tMs, int dir,
                          int maxSteps, int minT, int maxT) {
  return isValidSteps(steps, maxSteps) &&
         isValidPeriodMs(tMs, minT, maxT) &&
         isValidDir(dir);
}

// distCm < 0 = lectura inválida (no es obstáculo, se cuenta aparte).
inline bool shouldAvoid(float distCm, float umbralCm) {
  return distCm >= 0.0f && distCm < umbralCm;
}

inline bool shouldSafeHome(int invalidCount, int maxInvalid) {
  return invalidCount >= maxInvalid;
}

// Aritmética unsigned: tolera el overflow de millis() (~49 días).
inline bool shouldWatchdogRevert(unsigned long nowMs, unsigned long lastMs,
                                 unsigned long timeoutMs) {
  return (nowMs - lastMs) > timeoutMs;
}

inline float clampAngle(float a) {
  if (a < 0.0f) return 0.0f;
  if (a > 180.0f) return 180.0f;
  return a;
}
