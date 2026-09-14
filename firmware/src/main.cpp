#include <Arduino.h>
#include "config.h"

#if !SIM_SIN_SERVOS
#include <Otto.h>
Otto Otto;
#endif

long leerEchoUS() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  // 30ms timeout ~= 5m, fuera de rango HC-SR04 (400cm)
  return pulseIn(PIN_ECHO, HIGH, 30000);
}

float leerDistanciaCM() {
  long muestras[FILTRO_MUESTRAS];
  for (int i = 0; i < FILTRO_MUESTRAS; i++) {
    long us = leerEchoUS();
    float cm = (us == 0) ? -1 : (us / 2.0f) / 29.1f;
    muestras[i] = (long)(cm * 10); // 1 decimal como entero
    delay(10);
  }
  // mediana simple (burbuja, N=5)
  for (int i = 0; i < FILTRO_MUESTRAS - 1; i++)
    for (int j = i + 1; j < FILTRO_MUESTRAS; j++)
      if (muestras[j] < muestras[i]) {
        long t = muestras[i]; muestras[i] = muestras[j]; muestras[j] = t;
      }
  long med = muestras[FILTRO_MUESTRAS / 2];
  if (med < 20 || med > 4000) return -1; // <2cm o >400cm = fuera de rango
  return med / 10.0f;
}

void beep(int freq = 2000, int ms = 120) {
#if defined(ESP32)
  tone(PIN_BUZZER, freq, ms);
#else
  digitalWrite(PIN_BUZZER, HIGH);
  delay(ms);
  digitalWrite(PIN_BUZZER, LOW);
#endif
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  delay(500);

  Serial.println("\n[Otto-ESP32] Fase 1 C++");
#if SIM_SIN_SERVOS
  Serial.println("[SIM_SIN_SERVOS=1] Sin SG90: solo HC-SR04 + buzzer + log [SIM]");
  Serial.println("Acerca la mano: <20cm debe pitar y loguear [SIM] giro");
#else
  Serial.println("Modo CON servos: Otto.init + home");
  Otto.init(SERVO_CI, SERVO_CD, SERVO_TI, SERVO_TD, true, PIN_BUZZER, PIN_TRIG, PIN_ECHO);
  Otto.setTrims(TRIM_CI, TRIM_CD, TRIM_TI, TRIM_TD);
  Otto.home();
  Otto.sing(S_connection);
  Serial.println("[Otto] home OK, entrando a loop autonomo");
#endif
}

void loop() {
  float d = leerDistanciaCM();
  if (d < 0) {
    Serial.println("dist: fuera de rango");
  } else {
    Serial.printf("dist: %.1f cm\n", d);
  }

#if SIM_SIN_SERVOS
  // Test hoy: sensor real + actuadores simulados
  if (d > 0 && d < UMBRAL_OBSTACULO_CM) {
    Serial.println("[SIM] obstaculo -> [SIM] turn + beep");
    beep(2500, 150);
    delay(200);
    beep(2500, 150);
  } else if (d > 0) {
    Serial.println("[SIM] libre -> [SIM] walk(4)");
  }
  delay(PERIODO_LECTURA_MS);
#else
  // Firmware inicial real (cuando SIM=0 y hay 4x SG90 + fuente 5V 2A GND comun)
  if (d > 0 && d < UMBRAL_OBSTACULO_CM) {
    Otto.sing(S_surprise);
    Otto.turn(2, TIEMPO_PASO_MS, 1);
    Otto.home();
  } else if (d > 0) {
    Otto.walk(4, TIEMPO_PASO_MS, 1);
    Otto.home();
  } else {
    Otto.home();
    delay(PERIODO_LECTURA_MS);
  }
#endif
}
