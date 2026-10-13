# 11. Migración ESP32-WROOM (tu cableado actual)

> Nano (docs/05) usa D2-D5/D8-D9/D13 a 5V. En ESP32-WROOM todo es 3.3V y cambia el pinout.

## Tu repaso actual

| Elemento | Cable | ESP32 | Nota |
|---|---|---|---|
| SG90 señal | Naranja (S) | **D2 / GPIO2** | `PIN_SERVO_TEST 2`. Lleva LED onboard; si falla subida, quita S para subir o mueve a GPIO32 |
| SG90 power | Rojo (V) | **5V fuente externa** | Nunca al 3V3. Portapilas / 5V 2A |
| SG90 masa | Marrón (G) | **GND ESP32 + GND fuente (común)** | Obligatorio, si no hay jitter/resets |
| HC-SR04 VCC | VCC | **5V / VIN** | Con 3V3 falla |
| HC-SR04 GND | GND | **GND** | Común |
| HC-SR04 TRIG | TRIG | **GPIO18** | Salida 3V3 OK |
| HC-SR04 ECHO | ECHO | **GPIO19 con divisor** | Echo devuelve 5V, directo quema GPIO |
| Buzzer | + / - | **GPIO27 / GND** | Reservado, aún no cableado |

## Divisor Echo (obligatorio para dejarlo fijo)

```text
ECHO HC-SR04 (5V) ---[1k]---+--- GPIO19 (3.3V)
                             |
                            [2k]
                             |
                            GND
```

`Vgpio = 5V * 2k/(1k+2k) = 3.3V`. Alternativas con lo que tengas:
1k+1k = 2.5V (funciona), 10k+20k = 3.3V (vale), solo 1k en serie = apaño mínimo.

Sin divisor puedes probar 10-20s para ver números, no lo dejes fijo.

## Orden de encendido seguro

1. USB al PC (lógica) con fuente externa apagada.
2. Sube `env:test_1sg90`, abre Monitor 115200.
3. Enciende fuente externa del servo.
4. Debe centrar a 90° 2s, luego `dist + nudge 75/105°`.
5. Para subir de nuevo, apaga fuente externa o quita S si usas D2.

## WiFi único (sin Bluetooth)

* Base (`env:otto_esp32`) compila sin red. Extra (`env:otto_esp32_wifi`) usa **solo WiFi STA 2.4 GHz** (una red en `src/wifi_secrets.h`).
* **BT/BLE siempre deshabilitado** (`esp_bt_controller_mem_release` + `btStop()` si aplica): WiFi+BT comparten 2.4 GHz y tumban el sistema.
* Si WiFi falla, la base sigue en `EXPOSITION` local. Ver [Librerías](./06-librerias.md) y [Arquitectura](./12-arquitectura-modulos.md).
