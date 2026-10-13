# 12. Arquitectura y Módulos (SDD)

> Base mínima estable + extras opcionales por flags. La base **nunca depende de red**.

## 1. Árbol (`firmware/src/`)

```text
src/
├── config.h               # pines, trims, umbrales, SIM, ENABLE_WIFI_MODULE
├── wifi_secrets.h         # SSID/PASS 2.4 GHz (solo extra WiFi)
├── main.cpp               # setup: autonomyInit + wifiBegin; loop: serial + wifi + tick
├── test_1sg90.cpp         # env de mesa (1 servo + sonar)
├── core/                  # BASE (siempre compila)
│   ├── modes.h            # OttoMode {EXPOSITION, MANUAL}
│   ├── servo_controller.* # 4x SG90 / [SIM]
│   ├── sonar.*            # mediana 5, -1 inválido
│   ├── buzzer.*           # beep/tone
│   ├── gait.*             # walk/turn/danceBasic senoidal
│   └── autonomy.*         # setMode + tick + manual* + watchdog
└── modules/               # EXTRAS (solo si flag=1)
    └── net_wifi.*         # WiFi STA 2.4 GHz + WebServer / /status /mode /cmd
    # futuros: ota.*, songs.*, leds.*
```

## 2. Flags (`platformio.ini` / `config.h`)

| Flag | 0 (base) | 1 (extra) |
| --- | --- | --- |
| `SIM_SIN_SERVOS` | Servos reales | Simula actuadores, sensor real |
| `ENABLE_WIFI_MODULE` | Sin `WiFi/WebServer` | Panel + API de modos |

Envs: `otto_esp32` (0), `otto_esp32_wifi` (1), `test_1sg90` (mesa).

## 3. Contrato base (`core/autonomy.h`)

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

* La base no incluye `<WiFi.h>` ni `<WebServer.h>` (solo el módulo).
* Los extras llaman a la base; nunca al revés.
* Si el extra falla, la base sigue en `EXPOSITION` local.
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

* Lógica pura testeable en `core/validate.h`, `sonar_filter.h`, `gait_params.h`, `modes.h` (sin Arduino).
* Suite host: `python tests/run_tests.py` (49 tests, stdlib). Ver [`tests/README.md`](../tests/README.md).
* Unity en placa: `pio test -e test_core_esp32` (casos en `firmware/test/test_core/`).
* Regla: todo módulo nuevo trae sus reglas puras + casos en ambas capas.

---

[← Migración ESP32](./11-migracion-esp32.md) | [Índice](../README.md) | [Docs](./index.md)
