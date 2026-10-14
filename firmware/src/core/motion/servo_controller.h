#pragma once
// Controlador de 4x SG90 sobre ESP32Servo.
// Si SIM_SIN_SERVOS=1 no hace attach (modo mesa sin servos).

#include <Arduino.h>

class ServoController {
 public:
  bool begin();
  void home();
  void stand();
  void writeAll(float ci, float cd, float ti, float td);
  void detachAll();
  bool attached() const { return isAttached; }

 private:
  bool isAttached = false;
  void applyTrimmed(int idx, float angle);
};
