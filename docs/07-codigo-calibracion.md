# 07. Código de Calibración

Este código permite calibrar todos los servomotores en su posición natural en 90°.

```cpp
#include <Servo.h>

// Definición de objetos para los 4 servomotores
Servo servoCaderaIzq;  // Pin 2
Servo servoCaderaDer;  // Pin 3
Servo servoTobilloIzq; // Pin 4
Servo servoTobilloDer; // Pin 5

void setup() {
  // Asignación de pines según la guía de conexiones
  servoCaderaIzq.attach(2);
  servoCaderaDer.attach(3);
  servoTobilloIzq.attach(4);
  servoTobilloDer.attach(5);

  // Mover todos los servos a la posición central (90 grados)
  servoCaderaIzq.write(90);
  servoCaderaDer.write(90);
  servoTobilloIzq.write(90);
  servoTobilloDer.write(90);
}

void loop() {
  // Mantiene los servos bloqueados firmemente en 90 grados
  delay(1000);
}
```

> **Uso:** sube este sketch antes del montaje mecánico definitivo. Con los servos a 90°, coloca los brazos/horns y las piernas/pies. Ver [Guía de Conexiones](./05-guia-conexiones.md) y [Calibración y Troubleshooting](./10-calibracion-troubleshooting.md).

---

[← Librerías](./06-librerias.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Firmware Autónomo →](./08-firmware-autonomo.md)