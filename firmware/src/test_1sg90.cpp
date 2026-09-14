// Test con 1 solo SG90 + HC-SR04 en ESP32-WROOM.
// Env PlatformIO: test_1sg90 (ver platformio.ini, usa src_filter).
//
// CABLEADO SEGURO (importante):
//   Marrón (GND servo) -> GND ESP32 Y GND fuente externa (GND común obligatorio)
//   Rojo (V servo)     -> 5V fuente externa (portapilas / 5V 2A). NUNCA al 3V3 del ESP32.
//   Naranja (S servo)  -> PIN_SERVO_TEST (D2 ahora, ver config.h)
//   HC-SR04: VCC->5V USB/VIN, TRIG->GPIO18, ECHO->GPIO19 con divisor 1k/2k, GND->GND
//   Si ECHO está sin divisor (tu estado actual): test cortito OK, no lo dejes fijo.
//   Ver docs/11-migracion-esp32.md para el divisor.
//   USB del PC solo para lógica. Si el servo tiembla/resetea la placa = falta corriente.
//
// USO (Monitor 115200):
//   - Al arrancar centra a 90° 2s (momento para montar el horn).
//   - Luego barre 60-120-90-0-180-90 en loop con pausa y muestra distancia.
//   - Serial interactivo: envía 0..180 + Enter para ir a ese ángulo,
//     'c' = centro 90, 's' = barrido demo una vez.
#include <Arduino.h>
#include <ESP32Servo.h>
#include "config.h"

Servo sTest;

float leerDistanciaFast() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long us = pulseIn(PIN_ECHO, HIGH, 30000);
  if (us == 0) return -1;
  float cm = (us / 2.0f) / 29.1f;
  if (cm < 2 || cm > 400) return -1;
  return cm;
}

void irA(int ang, int espera = 600) {
  ang = constrain(ang, 0, 180);
  sTest.write(ang);
  Serial.printf("servo -> %d\n", ang);
  delay(espera);
}

void barridoDemo() {
  Serial.println("[1SG90] barrido demo");
  irA(90, 800);
  irA(60, 600);
  irA(120, 600);
  irA(90, 600);
  irA(0, 800);
  irA(180, 800);
  irA(90, 800);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  delay(300);

  ESP32PWM::allocateTimer(0);
  sTest.setPeriodHertz(50);
  sTest.attach(PIN_SERVO_TEST, 500, 2400);

  Serial.printf("\n[1SG90] Test un servo en GPIO%d + HC-SR04\n", PIN_SERVO_TEST);
  Serial.println("[1SG90] Centrando a 90 por 2s: monta el horn ahora");
  sTest.write(90);
  delay(2000);
}

void loop() {
  // Comandos serie sin bloquear mucho
  while (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "c") {
      irA(90);
    } else if (cmd == "s") {
      barridoDemo();
    } else {
      int ang = cmd.toInt();
      if (ang >= 0 && ang <= 180 && cmd.length() > 0) irA(ang);
      else Serial.println("usa 0..180, c=90, s=barrido");
    }
  }

  float d = leerDistanciaFast();
  if (d < 0) Serial.println("dist: fuera de rango | servo listo (0..180/c/s)");
  else Serial.printf("dist: %.1f cm | servo listo (0..180/c/s)\n", d);

  // Movimiento suave de fondo para ver que sigue vivo si no escribes nada:
  // comenta este bloque si solo quieres control manual.
  static unsigned long t0 = 0;
  static int fase = 0;
  if (millis() - t0 > 4000) {
    t0 = millis();
    fase = (fase + 1) % 2;
    sTest.write(fase ? 75 : 105); // oscila ±15° sobre 90, seguro para mesa
    Serial.printf("servo nudge -> %d (anti-bloqueo)\n", fase ? 75 : 105);
  }

  delay(500);
}
