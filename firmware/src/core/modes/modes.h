#pragma once
// Modos de operación (core, sin red).
// EXPOSITION: baila en bucle + sensor de proximidad (autonomía).
// MANUAL: autonomía apagada, solo comandos explícitos (vía Serial o WiFi extra).

#include <stdint.h>

enum class OttoMode : uint8_t {
  EXPOSITION = 0,
  MANUAL = 1
};

inline const char* modeName(OttoMode m) {
  return (m == OttoMode::MANUAL) ? "manual" : "exposition";
}

inline OttoMode modeFromString(const char* s) {
  if (s == nullptr) return OttoMode::EXPOSITION;
  // acepta: manual / m / 1
  if (s[0] == 'm' || s[0] == 'M' || s[0] == '1') return OttoMode::MANUAL;
  return OttoMode::EXPOSITION;
}
