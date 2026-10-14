#pragma once
// Otto DIY — Config base SDD (ESP32-WROOM, PlatformIO, C++).
// Placa objetivo: ESP32-WROOM (esp32dev, Arduino framework).
// Base: servos + proximidad + WiFi + dashboard, siempre presentes.
// Política de red: SOLO WiFi 2.4 GHz. Bluetooth siempre deshabilitado.

#include <stdint.h>

// ---------------------------------------------------------------- Sim
// 1 = sin servos: no hace attach, solo HC-SR04 + log [SIM].
// 0 = con servos: attach 4x SG90 + gait real.
// Cambia en platformio.ini con -DSIM_SIN_SERVOS=0 o edita aquí.
#ifndef SIM_SIN_SERVOS
#define SIM_SIN_SERVOS 1
#endif

// ---------------------------------------------------------------- Servos SG90
#define SERVO_CI 32  // Cadera izquierda
#define SERVO_CD 33  // Cadera derecha
#define SERVO_TI 25  // Tobillo izquierdo
#define SERVO_TD 26  // Tobillo derecho

// Test 1 servo (env:manual_1sg90). GPIO2 = LED onboard + strapping:
// si falla el upload, desconecta S naranja durante la subida o mueve a 32.
#define PIN_SERVO_TEST 2

// ---------------------------------------------------------------- Sensores
// HC-SR04: VCC->5V/VIN, GND->GND común, TRIG->GPIO18, ECHO->GPIO19 con divisor.
#define PIN_TRIG 18
#define PIN_ECHO 19

// Buzzer pasivo en GPIO PWM seguro
#define PIN_BUZZER 27

// Trims (ajustar si camina chueco, ver docs/10)
#define TRIM_CI 0
#define TRIM_CD 0
#define TRIM_TI 0
#define TRIM_TD 0

// ---------------------------------------------------------------- Comportamiento base
#define UMBRAL_OBSTACULO_CM 20
#define TIEMPO_PASO_MS 1000
#define PERIODO_LECTURA_MS 200
#define FILTRO_MUESTRAS 5
#define MAX_LECTURAS_INVALIDAS 5

// Límites de seguridad para comandos manuales (evitan sobrecarga de servos)
#define MANUAL_MAX_STEPS 4
#define MANUAL_MIN_T_MS 500
#define MANUAL_MAX_T_MS 2000

// Watchdog manual: si no hay comandos en este tiempo, vuelve a EXPOSITION.
#define MANUAL_WATCHDOG_MS 30000UL

// Modo por defecto al arrancar (siempre con WiFi + dashboard)
#define DEFAULT_MODE_EXPOSITION 1  // 1 = EXPOSITION, 0 = MANUAL
