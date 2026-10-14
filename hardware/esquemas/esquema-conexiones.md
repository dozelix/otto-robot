# Esquema de conexiones Otto DIY (ESP32)

> No se requiere PCB propia: ESP32-WROOM + fuente externa 5 V 2 A.
> Diagrama en [`esquema-conexiones.svg`](./esquema-conexiones.svg).
> Pin a pin y alimentación en [`docs/05-guia-conexiones.md`](../../docs/05-guia-conexiones.md).

## Redes

| Red | Origen ESP32 | Destino | Cable |
| --- | --- | --- | --- |
| Cadera izq. (CI) | GPIO32 (señal) + fuente 5V/GND | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Cadera der. (CD) | GPIO33 (señal) + fuente 5V/GND | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Tobillo izq. (TI) | GPIO25 (señal) + fuente 5V/GND | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Tobillo der. (TD) | GPIO26 (señal) + fuente 5V/GND | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Trig | GPIO18 | HC-SR04 Trig | Jumper |
| Echo (con divisor 1k/2k) | GPIO19 | HC-SR04 Echo | Jumper + divisor a GND |
| HC-SR04 VCC/GND | 5V / GND | HC-SR04 VCC/GND | Jumper |
| Buzzer + / − | GPIO27 / GND | Buzzer pasivo (opcional) | Directo |
| Potencia + | Vía interruptor → 5V servos + VIN | Fuente 5 V 2 A | (+) con switch en serie |
| Potencia − | Directo → GND común ESP32+fuente | Fuente 5 V 2 A | (−) directo |

## Cómo armarlo (orden de cableado)

1. **ESP32 solo por USB.** Sube el firmware y abre el monitor 115200.
2. **HC-SR04.** VCC→5V, GND→GND, TRIG→GPIO18, ECHO→GPIO19 **con divisor
   1k/2k a GND**. Verifica `dist: X cm` con la mano.
3. **Buzzer (opcional).** (+)→GPIO27, (−)→GND.
4. **Un servo en banco.** Solo naranja→GPIO2, sin fuente. Sube
   `env:manual_1sg90` y verifica 90°.
5. **Fuente externa (apagada).** Rojo servos→5V, marrón→GND, **GND común
   fuente↔ESP32**, interruptor en serie con el (+).
6. **Cuatro servos.** Señales a GPIO32/33/25/26, potencia a la fuente.
   Sube `env:calibracion_90`: todo a 90°. **Recién ahí, montaje mecánico**
   (ver [Manual de Ensamblaje](../../docs/09-manual-ensamblaje.md)).
7. **Ordena cables** dentro del cuerpo sin tocar los horns.
8. **WiFi.** Edita `wifi_secrets.h` (red 2.4 GHz) y abre `http://<ip>/`
   desde el teléfono (ver [Firmware](../../docs/08-firmware-autonomo.md)).

Reglas que no se negocian:

* Rojo siempre a 5V, marrón a GND, naranja a señal. Nada invertido.
* Echo **nunca** directo a 5 V al GPIO (divisor obligatorio fijo).
* GND común fuente↔ESP32 (sin esto: jitter y resets).
* Al programar por USB, fuente externa **apagada**.

## Alimentación e interruptor

```text
        [ Interruptor ]
                     /
  (+) Fuente 5V ----o   o-------------------> 5V servos + VIN ESP32
  (-) Fuente ------------------------------> GND común (fuente + ESP32)
```

* USB del PC = lógica/programación (no mueve servos).
* Fuente 5 V 2 A (baterías o adaptador) = operación del robot.

## Archivos

| Archivo | Descripción |
| --- | --- |
| `esquema-conexiones.md` | Este índice + instrucciones de cableado |
| `esquema-conexiones.svg` | Diagrama visual ESP32 para el informe |
