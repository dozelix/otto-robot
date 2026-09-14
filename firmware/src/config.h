#pragma once
// Config única C++ y referencia para el port Rust (firmware-rust/src/config.rs).
// Cambia SIM_SIN_SERVOS a 0 cuando lleguen los 4x SG90.

// 1 = sin servos: no hace attach, solo testea HC-SR04 + buzzer + log [SIM]
// 0 = con servos: usa OttoDIYLib + ESP32Servo
#define SIM_SIN_SERVOS 1

// Servos SG90 (reservados, no conectar hasta Fase 2 hardware)
#define SERVO_CI 32  // Cadera izquierda
#define SERVO_CD 33  // Cadera derecha
#define SERVO_TI 25  // Tobillo izquierdo
#define SERVO_TD 26  // Tobillo derecho

// Test con 1 solo SG90: ahora en D2 (GPIO2) según tu cableado.
// Ojo: GPIO2 lleva el LED onboard y es strapping (debe estar LOW/float en boot).
// Si la subida falla, desconecta la señal naranja durante upload o mueve a 32.
#define PIN_SERVO_TEST 2

// Sensor HC-SR04 (Echo con divisor 1k/2k a GND, nunca 5V directo al GPIO)
#define PIN_TRIG 18
#define PIN_ECHO 19

// Buzzer pasivo en GPIO PWM seguro
#define PIN_BUZZER 27

// Trims cuando lleguen servos (ajustar si camina chueco, ver docs/10)
#define TRIM_CI 0
#define TRIM_CD 0
#define TRIM_TI 0
#define TRIM_TD 0

// Comportamiento autónomo
#define UMBRAL_OBSTACULO_CM 20
#define TIEMPO_PASO_MS 1000
#define PERIODO_LECTURA_MS 200
#define FILTRO_MUESTRAS 5
