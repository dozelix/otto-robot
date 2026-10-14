# 05. Guía de Conexiones (ESP32-WROOM)

> Placa objetivo: **ESP32-WROOM** (`esp32dev`, 3.3 V lógicos).
> Referencia histórica Nano al final. Esquema visual (Nano):
> [`hardware/esquemas/esquema-conexiones.svg`](../hardware/esquemas/esquema-conexiones.svg).

---

## 1. Orden de conexión (qué módulo va primero)

Conectar en este orden. La regla de oro: **primero señal y lógica,
la potencia de servos al final**.

| # | Módulo | Qué se conecta | Env / verificación |
| --- | --- | --- | --- |
| 1 | **ESP32 solo** | USB al PC. Nada más. | Monitor 115200: el firmware arranca (`[Otto-ESP32]`) |
| 2 | **HC-SR04** | VCC→5V/VIN, GND→GND, TRIG→GPIO18, ECHO→GPIO19 **con divisor** | `dist: X cm` estable al acercar la mano |
| 3 | **Buzzer** | (+)→GPIO27, (−)→GND | `beep` al cambiar de modo / evitar obstáculo |
| 4 | **1 servo (banco)** | Solo señal naranja→GPIO2; rojo/marrón **aún sin fuente** | `manual_1sg90`: centra 90° 2 s |
| 5 | **Fuente externa 5 V 2 A** | Rojo servos→5V, marrón→GND **común con ESP32**, interruptor en (+) | Sin jitter ni resets al mover |
| 6 | **4 servos** | Señales→GPIO32/33/25/26 (ver §2), potencia a la fuente | `calibracion_90`: los 4 a 90° |
| 7 | **Montaje mecánico** | Recién con servos a 90° (ver [Ensamblaje](./09-manual-ensamblaje.md)) | Horns atornillados a 90° |
| 8 | **WiFi/panel** | `wifi_secrets.h` con red 2.4 GHz | `http://<ip>/` desde el teléfono |

> ¿Por qué este orden? Cada etapa valida la anterior con lo mínimo
> energizado. Si algo falla, sabes exactamente qué módulo lo causó.

---

## 2. Servomotores (4 x SG90)

Colores del cable: **naranja = señal (S)**, **rojo = 5 V (V)**,
**marrón = GND (G)**.

| Servo | Función | Señal ESP32 | Potencia |
| --- | --- | --- | --- |
| Cadera izquierda (CI) | `SERVO_CI` | **GPIO32** | Fuente 5 V / GND común |
| Cadera derecha (CD) | `SERVO_CD` | **GPIO33** | Fuente 5 V / GND común |
| Tobillo izquierdo (TI) | `SERVO_TI` | **GPIO25** | Fuente 5 V / GND común |
| Tobillo derecho (TD) | `SERVO_TD` | **GPIO26** | Fuente 5 V / GND común |

Reglas:

* La **señal** (3.3 V del ESP32) basta para comandar el SG90; la **potencia**
  sale siempre de la fuente externa, **nunca** del pin 3V3 ni del USB.
* **GND común obligatorio**: GND de la fuente unido al GND del ESP32.
  Sin esto hay jitter y resets.
* Señal PWM 50 Hz, pulsos 500–2400 µs (lo maneja `ESP32Servo`).
* Banco de 1 servo: señal en **GPIO2** (`PIN_SERVO_TEST`). Ojo: GPIO2 lleva
  el LED onboard y es pin de arranque — si falla la subida, desconecta el
  cable naranja durante el upload o mueve el test a GPIO32.
* **Centrado a 90° antes de atornillar** (`env:calibracion_90`). Los trims
  (`TRIM_CI/CD/TI/TD` en `config.h`) corrigen si camina chueco.

---

## 3. Sensor ultrasónico (HC-SR04)

| Pin HC-SR04 | Conexión ESP32 |
| --- | --- |
| **VCC** | **5V / VIN** (con 3V3 mide mal) |
| **TRIG** | **GPIO18** (salida 3V3, OK directo) |
| **ECHO** | **GPIO19 con divisor** (el Echo devuelve 5 V: directo quema el GPIO) |
| **GND** | **GND** (común) |

Divisor obligatorio para instalación fija:

```text
ECHO HC-SR04 (5V) ---[1k]---+--- GPIO19 (3.3V)
                             |
                            [2k]
                             |
                            GND
```

`Vgpio = 5V · 2k/(1k+2k) = 3.3V`. Alternativas: 1k+1k = 2.5 V (funciona),
10k+20k = 3.3 V. Solo para pruebas cortas puede ir directo (no fijo).

---

## 4. Zumbador (buzzer pasivo, opcional)

| Pin zumbador | Conexión ESP32 |
| --- | --- |
| **Positivo (+)** | **GPIO27** |
| **Negativo (−)** | **GND** |

Opcional: sin buzzer el robot anda igual (servos + sonar). En código es
`modules/buzzer.*`, registrado vía `core/audio/sound_hook.h`.

---

## 5. Alimentación: baterías o cable

| Fuente | Sirve para | No sirve para |
| --- | --- | --- |
| **USB del PC** (5 V / 500 mA) | Lógica, programar, monitor serial | Mover servos (picos de hasta 1.5 A → resets) |
| **Baterías / fuente 5 V 2 A** | Servos + operación autónoma del robot | — |

Reglas:

1. **Programar por USB con la fuente externa apagada** (o servos desconectados).
2. **Operar con fuente externa encendida**; el USB puede quedar para el monitor.
3. El interruptor va en serie con el **(+)** de la batería/fuente:
   `(+) ──o interruptor o──→ 5V servos + VIN ESP32` · `(−) ──→ GND común`.
4. Orden de encendido: USB primero (lógica) → subir firmware → encender fuente.
5. Si el ESP32 se resetea al moverse: falta corriente o falta GND común.

---

## 6. Verificación previa al primer encendido

1. **90°:** servos centrados con `calibracion_90` antes del montaje mecánico.
2. **Polaridad:** rojo a 5V, marrón a GND, naranja a señal. Nada invertido.
3. **Divisor Echo** colocado (1k/2k).
4. **GND común** fuente ↔ ESP32 verificado con multímetro (continuidad).
5. **Interruptor** en (+) y en OFF antes de conectar la batería.

---

## Apéndice: referencia Nano (histórica)

Placa original: Arduino Nano + Expansion Board (filas S/V/G por pin).
Servos D2–D5, Trig D8, Echo D9, buzzer D13. El firmware actual es ESP32
y no usa esta placa; se conserva solo como referencia del proyecto original.

---

[← Ficha Descriptiva](./04-ficha-descriptiva.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Librerías →](./06-librerias.md)
