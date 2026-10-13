#pragma once
// Filtro sonar puro (sin Arduino): conversión us->cm, mediana de 5 y validez.
// Mismo algoritmo que usaba sonar.cpp (burbuja N=5, 1 decimal como entero).
// Testeable en host: tests/test_sonar_filter.py + firmware/test/test_core/.

inline float sonarCmFromUs(long us) {
  if (us == 0) return -1.0f;  // timeout de pulseIn
  return ((us / 2.0f) / 29.1f);
}

// 1 decimal como entero (10.3 cm -> 103). Trunca como el firmware original.
inline long sonarScaled(float cm) {
  return (long)(cm * 10.0f);
}

// Rango HC-SR04: 2..400 cm expresado en décimas.
inline bool sonarCmValid(long scaledTenths) {
  return scaledTenths >= 20 && scaledTenths <= 4000;
}

inline long sonarMedian5(long s[5]) {
  for (int i = 0; i < 4; i++)
    for (int j = i + 1; j < 5; j++)
      if (s[j] < s[i]) {
        long t = s[i]; s[i] = s[j]; s[j] = t;
      }
  return s[2];
}

// Entrada: 5 lecturas crudas en µs (0 = timeout). Salida: cm o -1.
inline float sonarFilterCm(long usSamples[5]) {
  long m[5];
  for (int i = 0; i < 5; i++) m[i] = sonarScaled(sonarCmFromUs(usSamples[i]));
  long med = sonarMedian5(m);
  if (!sonarCmValid(med)) return -1.0f;
  return med / 10.0f;
}
