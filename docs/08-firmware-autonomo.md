# 08. Firmware Autónomo

> **Estado: Pendiente.**

Código definitivo para que Otto camine, emita sonidos y esquive obstáculos de forma automática usando el sensor ultrasónico.

## Qué se necesita definir

1. **Sketch principal:** programa autónomo basado en `OttoDIYLib` (ver [Librerías](./06-librerias.md)).
2. **Comportamiento:** evitar obstáculos con HC-SR04, bailes, sonidos y postura inicial (`Otto.home()`).
3. **Pines:** reutilizar el mapeo de [Guía de Conexiones](./05-guia-conexiones.md) (servos D2-D5, Trig D8, Echo D9, buzzer D13) y el centrado previo de [Código de Calibración](./07-codigo-calibracion.md).

## Referencias

* [Librerías y Entorno](./06-librerias.md) — ejemplo mínimo de `Otto.init()` y `Otto.walk()`.
* [Guía de Conexiones](./05-guia-conexiones.md) — mapeo de pines.
* [Calibración y Troubleshooting](./10-calibracion-troubleshooting.md) — ajuste si camina chueco.

<!-- TODO: añadir el sketch autónomo cuando esté disponible -->

---

[← Código de Calibración](./07-codigo-calibracion.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Manual de Ensamblaje →](./09-manual-ensamblaje.md)
