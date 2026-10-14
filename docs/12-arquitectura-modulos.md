# 12. Arquitectura y Módulos (SDD)

> Base mínima estable + extras opcionales por flags. La base **nunca depende de red**.

## 1. Árbol (`firmware/src/`)

```text
src/
├── config.h               # pines, trims, umbrales, SIM (WiFi siempre en core)
├── wifi_secrets.h         # SSID/PASS 2.4 GHz, red única (sin BT)
├── main.cpp               # setup: autonomyInit + wifiBegin; loop: serial + wifi + tick
├── core/                  # BASE, un subdirectorio por módulo (siempre compila)
│   ├── modes/             # OttoMode {EXPOSITION, MANUAL}
│   ├── validate/          # reglas puras (steps/T/dir, shouldAvoid/Watchdog…)
│   ├── sonar/             # HC-SR04 + filtro mediana 5, -1 inválido
│   ├── motion/            # 4x SG90 + walk/turn/danceBasic senoidal
│   ├── autonomy/          # setMode + tick + manual* + watchdog
│   ├── net/               # WiFi STA 2.4 GHz + dashboard / /status /mode /cmd
│   └── audio/             # sound_hook (desacople core ↔ modules)
├── modules/               # buzzer opcional (se registra vía sound_hook)
└── tools/                 # banco: manual_1sg90 + calibracion_90
```

## 2. Flags y envs (`platformio.ini` en raíz / `config.h`)

| Flag | 0 | 1 (defecto) |
| --- | --- | --- |
| `SIM_SIN_SERVOS` | Servos reales | Simula actuadores, sensor real |

WiFi + dashboard viven en `core/net` **siempre** (sin flag). Envs:
`otto_esp32` (producción), `otto_esp32_wifi` (alias compat.),
`manual_1sg90` y `calibracion_90` (banco, excluyen `core/`+`modules/`),
`test_core_esp32` (Unity en placa) y `native` (Unity en PC).

## 3. Contrato base (`core/autonomy/autonomy.h`)

```cpp
void autonomyInit();
void autonomyTick();
void setMode(OttoMode m);
OttoMode getMode();
float lastDistanceCm();
bool manualWalk(int steps, int T, int dir);
bool manualTurn(int steps, int T, int dir);
bool manualHome();
bool manualDance();
bool manualBeep(int freq, int ms);
```

Reglas:

* Solo `core/net` incluye `<WiFi.h>`/`<WebServer.h>`; el resto de `core/` no conoce la red.
* Si WiFi no conecta, la base sigue en `EXPOSITION` local (degradación elegante).
* El buzzer es opcional: `core/` lo usa vía `audio/sound_hook.h`, `modules/buzzer` se registra en `main.cpp`.
* Comandos manuales validados (`steps 1..4`, `T 500..2000 ms`).

## 4. Red: WiFi único

* Solo `WIFI_STA` 2.4 GHz, una sola red (`wifi_secrets.h`).
* BT/BLE deshabilitados: `esp_bt_controller_mem_release(ESP_BT_MODE_BTDM)` (+ `btStop()` si aplica).
* Sin coexistencia WiFi+BT, sin doble STA/AP.

## 5. Cómo agregar un módulo

1. Crea `modules/mi_modulo.h/.cpp` con `begin()/handle()` + guards `#if ENABLE_MI_MODULO`.
2. Agrega flag en `config.h` + `platformio.ini` (`-DENABLE_MI_MODULO=1` en env extra).
3. Llama al módulo desde `main.cpp` solo bajo `#if`.
4. Documenta endpoints y degradación (qué pasa si falla).

## 6. Pruebas

* Lógica pura testeable en `core/validate/validate.h`, `core/sonar/sonar_filter.h`, `core/motion/gait_params.h`, `core/modes/modes.h` (sin Arduino).
* Suite host: `python tests/run_tests.py` (49 tests, stdlib). Ver [`tests/README.md`](../tests/README.md).
* Unity en placa: `pio test -e test_core_esp32` (casos en `firmware/test/test_core/`).
* Regla: todo módulo nuevo trae sus reglas puras + casos en ambas capas.

---

[← Migración ESP32](./11-migracion-esp32.md) | [Índice](../README.md) | [Docs](./index.md)
