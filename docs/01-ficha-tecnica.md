# 01. Ficha Técnica

> Robot bípedo Otto DIY basado en Arduino Nano.

| Parámetro | Especificación |
| --- | --- |
| **Nombre del Proyecto** | Otto DIY Biped Robot |
| **Microcontrolador** | Arduino Nano (ATmega328P, 16 MHz, 5V) |
| **Placa de Expansión** | Arduino Nano Shield / Sensor Shield v1.0 |
| **Actuadores** | 4 x Miniservos SG90 (Torque: 1.6 kg·cm @ 4.8V, Ángulo: 180°) |
| **Sensores** | Sensor ultrasónico HC-SR04 (Rango: 2 cm a 400 cm) |
| **Audio** | Zumbador piezoeléctrico pasivo/activo (5V) |
| **Alimentación Recomendada** | Según la placa Arduino correspondiente: 5V por USB / pin 5V, 7–12V por pin Vin (ver datasheet de tu variante) |
| **Consumo Promedio** | ~300 mA (reposo) / hasta 1.5 A (pico con 4 servos en movimiento) |
| **Conectividad** | USB Mini-B (programación/alimentación) |
| **Entorno de Programación** | Arduino IDE (C++) / Otto Blockly |

> **Nota:** Nano clásico ATmega328P: `Operating 5V`, `Input 7–12V` (ver [store.arduino.cc](https://store.arduino.cc/products/arduino-nano)). Clones CH340: 7–9V recomendado. Nano Every: 7–21V. La Shield hereda estos límites.

---

[← Índice](../README.md) | [Docs](./index.md) | [Siguiente: Planos STL →](./02-planos-stl.md)
