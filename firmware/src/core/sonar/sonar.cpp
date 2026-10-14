#include "sonar.h"
#include "sonar_filter.h"
#include <Arduino.h>
#include "../../config.h"

#if FILTRO_MUESTRAS != 5
#error "sonarFilterCm() es mediana de 5: ajusta FILTRO_MUESTRAS o el filtro"
#endif

void sonarBegin() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  delay(50);
}

long sonarPulseUs() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  // 30 ms timeout ~= 5 m (fuera de rango HC-SR04 400 cm)
  return pulseIn(PIN_ECHO, HIGH, 30000);
}

float sonarReadCm() {
  long us[FILTRO_MUESTRAS];
  for (int i = 0; i < FILTRO_MUESTRAS; i++) {
    us[i] = sonarPulseUs();
    delay(10);
  }
  return sonarFilterCm(us);
}
