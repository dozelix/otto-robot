# 08. Firmware Autónomo (base SDD + modos)

> **Estado: En progreso (base funcional).**
> Código: `firmware/src/` (ESP32-WROOM, PlatformIO, C++). Ver [Librerías](./06-librerias.md) y [Arquitectura](./12-arquitectura-modulos.md).

## 1. Alcance de la base

Incluye (sin red):

1. Init hardware: 4x SG90 (o `[SIM]` sin servos), buzzer GPIO27, HC-SR04 (TRIG 18 / ECHO 19 con divisor).
2. Sensor: mediana de 5, timeout 30 ms, rango 2–400 cm, `-1` = inválido.
3. Actuación: `home()`, `stand()`, `walk(steps,T,dir)`, `turn(steps,T,dir)`, `danceBasic()`.
4. Autonomía + modos `EXPOSITION/MANUAL` con watchdog.
5. Logs Serial 115200 (`[Otto]`, `[SIM]`, `dist: X cm`).

Excluye (módulos extra): WiFi/OTA, melodías avanzadas, LEDs/accesorios, telemetría, port Rust.

## 2. Máquina de estados

```text
INIT -> HOME -> EXPOSITION:
  dist < 20 cm      -> AVOID (beep + turn 2 pasos) -> HOME
  dist >= 20 cm     -> WALK 2 pasos (cada 3er ciclo: danceBasic)
  dist inválida x5  -> SAFE_HOME + beep lento

MANUAL:
  IDLE_MANUAL -> espera comandos (walk/turn/home/dance/beep)
  sin comandos 30 s -> vuelve a EXPOSITION (watchdog)
```

Defaults (`config.h`): `UMBRAL=20 cm`, `T=1000 ms`, `PERIODO=200 ms`, `FILTRO=5`.

## 3. Modos (cambiables por WiFi extra o Serial)

| Modo | Comportamiento | Comandos |
| --- | --- | --- |
| `EXPOSITION` (defecto) | Baila en bucle + evita obstáculos con HC-SR04 | Solo `setMode(manual)` |
| `MANUAL` | Autonomía apagada, ejecuta lo pedido | `walk/back/turnL/turnR/home/dance/beep` |

API (`core/autonomy.h`):

```cpp
void setMode(OttoMode m);
OttoMode getMode();
bool manualWalk(int steps, int T, int dir);  // dir +1/-1, steps 1..4, T 500..2000
bool manualTurn(int steps, int T, int dir);
bool manualHome();
bool manualDance();
```

Serial (115200): `w/s/a/d/h/b` = mover en manual, `m/e` = manual/exposición, `?` = ayuda.

## 4. Módulo WiFi extra (`ENABLE_WIFI_MODULE=1`)

Panel en `http://<ip>/` + API:

* `GET /status` → `{"mode":"exposition","dist":34.5,"sim":1,"ip":"..."}`.
* `GET /mode?m=exposition|manual` → cambia de modo.
* `GET /cmd?a=walk&steps=2&T=1000` (`walk|back|turnL|turnR|home|dance|beep`).
  En `exposition` devuelve `409` (pasa a manual primero).

Si WiFi falla, la base sigue local en exposición.

## 5. Envs y puesta en marcha

```text
env:otto_esp32      base sin red (desarrollo diario)
env:otto_esp32_wifi base + panel (edita src/wifi_secrets.h, red 2.4 GHz)
env:test_1sg90      1 servo + sonar (mesa)
```

1. Sube `test_1sg90`, verifica `dist` + centro 90°.
2. Sube `otto_esp32` (`SIM=1`): acerca la mano <20 cm → `[SIM] giro + beep`.
3. Con 4x SG90 + fuente 5V 2A (GND común): pon `SIM=0`, recalibra a 90°, prueba `walk`.
4. Para modos por red: sube `otto_esp32_wifi`, abre `http://<ip>/`.

## Referencias

* [Librerías y Entorno](./06-librerias.md) — deps y política WiFi único.
* [Arquitectura y Módulos](./12-arquitectura-modulos.md) — base vs extras + flags.
* [Guía de Conexiones](./05-guia-conexiones.md) — Nano; ESP32 en [Migración ESP32](./11-migracion-esp32.md).
* [Calibración y Troubleshooting](./10-calibracion-troubleshooting.md) — trims.

---

[← Código de Calibración](./07-codigo-calibracion.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Manual de Ensamblaje →](./09-manual-ensamblaje.md)
