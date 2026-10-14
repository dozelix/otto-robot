# firmware/ — Contrato de carpetas (C++ único, PlatformIO desde la raíz)

Proyecto PlatformIO en la **raíz del repo** (`platformio.ini`):
`pio run -e otto_esp32` / `pio test -e native` / `pio test -e test_core_esp32`.
Todo el código es C++ (nada de Rust hasta que el usuario lo pida).

## `src/` — solo firmware de producción
- `main.cpp`, `config.h`, `wifi_secrets.h`: entrada, pines/umbrales, credenciales.
- Nada `test_*`, nada `.example` aquí.

## `src/core/` — base siempre presente (servos + proximidad + WiFi)
- `autonomy.*`: máquina EXPOSITION/MANUAL, watchdog, acciones manuales.
- `gait.*` + `gait_params.h`: marcha senoidal (parámetros puros).
- `servo_controller.*`: 4x SG90 sobre ESP32Servo.
- `sonar.*` + `sonar_filter.h`: HC-SR04 con mediana de 5 (puro).
- `modes.h`, `validate.h`: modos y reglas de seguridad (puros).
- `net_wifi.*`: WiFi STA 2.4 GHz + dashboard, **siempre on**, BT deshabilitado.
- `sound_hook.*`: abstracción de sonido (no-op si no hay buzzer).
- Regla: `core/` **nunca** incluye de `modules/`.

## `src/modules/` — opcionales (dependen de core/, nunca al revés)
- `buzzer.*`: beep pasivo GPIO27, se registra vía `setSoundHooks()`.

## `src/tools/` — firmwares manuales de banco (no son tests)
- `manual_1sg90.cpp` (env `manual_1sg90`): 1 servo + sonar en mesa.
- `calibracion_90.cpp` (env `calibracion_90`): 4 servos a 90°.

## `test/` — ÚNICO lugar con tests automatizados (Unity, C++)
- `test/test_core/`: `test_validate/sanar_filter/gait_params/modes/config/net_wifi_core`
  + `test_main.cpp` (runner `setup()/loop()` en placa, `main()` en PC).
- En placa: `pio test -e test_core_esp32`. En PC: `pio test -e native`.
