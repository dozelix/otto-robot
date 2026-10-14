# 08. Firmware Autónomo (base SDD + modos)

> **Estado: En progreso (base funcional).**
> Código: `firmware/src/` (ESP32-WROOM, PlatformIO, C++). Ver [Librerías](./06-librerias.md) y [Arquitectura](./12-arquitectura-modulos.md).

## 1. Alcance de la base

Incluye:

1. Init hardware: 4x SG90 (o `[SIM]` sin servos), HC-SR04 (TRIG 18 / ECHO 19 con divisor), buzzer opcional GPIO27.
2. Sensor: mediana de 5, timeout 30 ms, rango 2–400 cm, `-1` = inválido.
3. Actuación: `home()`, `stand()`, `walk(steps,T,dir)`, `turn(steps,T,dir)`, `danceBasic()`.
4. Autonomía + modos `EXPOSITION/MANUAL` con watchdog.
5. WiFi + dashboard mobile-first (red única 2.4 GHz, sin BT).
6. Logs Serial 115200 (`[Otto]`, `[SIM]`, `dist: X cm`).

Excluye: OTA, melodías avanzadas, LEDs/accesorios, telemetría, port Rust.

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

## 3. Modos (cambiables por panel WiFi o Serial)

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

## 4. WiFi + dashboard (`core/net`, siempre presente)

Red única 2.4 GHz en `firmware/src/wifi_secrets.h`, BT/BLE deshabilitado.
Si WiFi no conecta, la base sigue local en exposición.

### 4.1 Panel mobile-first (`GET /`)

El robot se maneja desde el teléfono. Specs de la página (servida inline
desde la ESP32, ~2.5 KB, sin dependencias externas):

* Viewport `width=device-width, initial-scale=1, viewport-fit=cover`.
* `touch-action: manipulation` (sin retardo táctil ni zoom por doble-tap).
* Layout de una columna, ancho máx. 520 px.
* Targets táctiles: botones ≥ 54 px, D-pad en grilla 3×3 de 60 px (▲◀■▶▼).
* Selector de modo segmentado con estado activo; badge de modo + distancia
  en grande; pad deshabilitado fuera de `manual` (avisa antes de pedir `/cmd`).
* Polling de `/status` cada 2 s; sin conexión muestra aviso, no se cuelga.

### 4.2 API

* `GET /status` → `200 application/json`
  `{"mode":"exposition","dist":34.5,"sim":1,"ip":"192.168.1.50"}`.
  `dist` en cm (`-1` = inválida), `sim` = 1 sin servos.
* `GET /mode?m=exposition|manual` → `200` + status. Sin `m` → `400`.
  Acepta `m/M/1` = manual, resto = exposition.
* `GET /cmd?a=walk&steps=2&T=1000` (`walk|back|turnL|turnR|home|dance|beep`)
  → `200` + status. En `exposition` → `409`. Acción inválida o fuera de
  límites (`steps` 1–4, `T` 500–2000 ms, beep 100–5000 Hz/20–1000 ms) → `400`.

## 5. Envs y puesta en marcha

```text
env:otto_esp32       producción (WiFi + panel siempre; SIM=1 sin servos)
env:manual_1sg90     banco: 1 servo + sonar (mesa)
env:calibracion_90   banco: 4 servos a 90° (montaje mecánico)
env:test_core_esp32  Unity en placa · env:native Unity en PC
```

1. Sube `manual_1sg90`, verifica `dist` + centro 90°.
2. Sube `otto_esp32` (`SIM=1`): acerca la mano <20 cm → `[SIM] giro + beep`.
3. Con 4x SG90 + fuente 5V 2A (GND común): pon `SIM=0`, recalibra a 90°, prueba `walk`.
4. Con WiFi conectado (edita `src/wifi_secrets.h`, red 2.4 GHz): abre `http://<ip>/` desde el teléfono.

## Referencias

* [Librerías y Entorno](./06-librerias.md) — deps y política WiFi único.
* [Arquitectura y Módulos](./12-arquitectura-modulos.md) — árbol por directorios y reglas.
* [Guía de Conexiones](./05-guia-conexiones.md) — Nano; ESP32 en [Migración ESP32](./11-migracion-esp32.md).
* [Calibración y Troubleshooting](./10-calibracion-troubleshooting.md) — trims.

---

[← Código de Calibración](./07-codigo-calibracion.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Manual de Ensamblaje →](./09-manual-ensamblaje.md)
