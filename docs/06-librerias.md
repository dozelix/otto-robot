# 06. Librerías y Entorno

> Placa objetivo: **ESP32-WROOM** (`esp32dev`, Arduino framework) con **PlatformIO**.
> Referencia Nano/Arduino IDE al final (solo histórica).

---

## 1. Base ESP32 (PlatformIO, C++)

### Librería base (única dependencia)

| Librería | Uso | Declaración PIO |
| --- | --- | --- |
| `madhephaestus/ESP32Servo` | 4x SG90 a 50 Hz (500–2400 µs), timers dedicados | `lib_deps = madhephaestus/ESP32Servo` |

No se usa `OttoDIYLib` en ESP32: es AVR-céntrica y rompe la compilación.
La marcha (`walk/turn/dance`) es propia en `firmware/src/core/gait.*`
(parámetros compatibles con `firmware-rust/src/gait.rs`).

El resto (sonar, buzzer, modos, autonomía) es código propio sin deps:

* `core/sonar.*` — HC-SR04 con mediana de 5, timeout 30 ms, rango 2–400 cm.
* `core/buzzer.*` — `tone()` en GPIO27.
* `core/autonomy.*` — modos `EXPOSITION/MANUAL` + watchdog 30 s.
* `core/servo_controller.*` — attach/home/trims.

### Envs PlatformIO (`firmware/platformio.ini`)

```ini
[env:otto_esp32]        # base sin red (SIM_SIN_SERVOS=1 por defecto)
lib_deps = madhephaestus/ESP32Servo
build_flags = -DSIM_SIN_SERVOS=1 -DENABLE_WIFI_MODULE=0

[env:otto_esp32_wifi]   # base + panel WiFi (edita src/wifi_secrets.h)
lib_deps = madhephaestus/ESP32Servo
build_flags = -DSIM_SIN_SERVOS=1 -DENABLE_WIFI_MODULE=1

[env:test_1sg90]        # 1 servo + sonar (mesa)
```

Compilar/subir (VSCode + extensión PlatformIO):

1. Abre la carpeta `firmware/`.
2. Abajo a la derecha elige env (`otto_esp32`, `otto_esp32_wifi` o `test_1sg90`).
3. `Build` → `Upload` → `Monitor` (115200).
4. Cuando lleguen los 4x SG90: cambia `SIM_SIN_SERVOS` a `0` (en `platformio.ini` o `config.h`).

## 2. Módulos extra (solo si `ENABLE_WIFI_MODULE=1`)

Usan librerías **incluidas en el framework Arduino-ESP32** (sin instalar nada):

* `WiFi` (solo modo `WIFI_STA`, 2.4 GHz) + `WebServer` (`GET / /status /mode /cmd`).
* `Update` (solo para OTA futura).

Opcional: `ArduinoJson` si se define protocolo JSON (hoy se usa JSON manual).

### Política de red: WiFi único

* **Solo WiFi 2.4 GHz. Bluetooth/BLE siempre deshabilitado** (comparten banda y tumban el sistema).
* El módulo llama `esp_bt_controller_mem_release(ESP_BT_MODE_BTDM)` (+ `btStop()` si aplica) antes de `WiFi.begin()`.
* Si WiFi falla o el módulo está en `0`, la base sigue en `EXPOSITION` local (degradación elegante).
* Credenciales en `firmware/src/wifi_secrets.h` (red única, no dual).

## 3. Referencia Nano (Arduino IDE, histórica)

Para Arduino Nano clásico (sin WiFi) la comunidad usa:

* `OttoDIYLib` (incluye `Oscillator.h`, `US.h`, requiere `Servo.h`).
* Instalación: IDE → Herramientas → Administrar bibliotecas → buscar **OttoDIY** → Instalar todo.
* Pines Nano: servos D2–D5, Trig D8, Echo D9, buzzer D13 (ver [Guía de Conexiones](./05-guia-conexiones.md)).

Ejemplo mínimo Nano (no ESP32):

```cpp
#include <Otto.h>
Otto Otto;
#define SERVO_CI 2
#define SERVO_CD 3
#define SERVO_TI 4
#define SERVO_TD 5
#define BUZZER   13
#define TRIG     8
#define ECHO     9
void setup() {
  Otto.init(SERVO_CI, SERVO_CD, SERVO_TI, SERVO_TD, true, BUZZER, TRIG, ECHO);
  Otto.home();
}
void loop() {
  Otto.walk(4, 1000, 1);
  delay(1000);
}
```

---

[← Guía de Conexiones](./05-guia-conexiones.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Código de Calibración →](./07-codigo-calibracion.md)
