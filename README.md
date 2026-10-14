# Otto DIY Robot

Robot bípedo educativo de código abierto basado en **ESP32-WROOM** (migración desde Arduino Nano), con chasis impreso en 3D, 4 servos SG90, sensor ultrasónico HC-SR04 y buzzer. Proyecto documentado con fines académicos.

Documentación completa en [`docs/index.md`](docs/index.md).

## Tabla de Contenido

### Hardware y diseño

1. [Ficha Técnica](docs/01-ficha-tecnica.md) — Microcontrolador, actuadores, sensores, alimentación y entorno.
2. [Planos STL](docs/02-planos-stl.md) — Body V9 69×69×39 mm (multi-pieza en único archivo) y parámetros recomendados.
3. [Lista de Materiales (BOM)](docs/03-lista-materiales-bom.md) — Electrónica y tornillería.
4. [Ficha Descriptiva](docs/04-ficha-descriptiva.md) — Cómo, cuándo, por qué y dónde del proyecto.
5. [Guía de Conexiones](docs/05-guia-conexiones.md) — Orden de conexión, servos SG90 y alimentación (ESP32).

### Software y puesta en marcha

6. [Librerías y Entorno](docs/06-librerias.md) — PlatformIO + `ESP32Servo`, política WiFi único.
7. [Código de Calibración](docs/07-codigo-calibracion.md) — Centrado de servos a 90°.
8. [Firmware Autónomo](docs/08-firmware-autonomo.md) — Base SDD + modos exposición/manual.
9. [Manual de Ensamblaje](docs/09-manual-ensamblaje.md) — Montaje progresivo en 9 etapas.
10. [Calibración y Troubleshooting](docs/10-calibracion-troubleshooting.md) — *Pendiente:* trimming y solución de problemas.
11. [Migración ESP32](docs/11-migracion-esp32.md) — Cableado actual, divisor Echo, orden de encendido.
12. [Arquitectura y Módulos](docs/12-arquitectura-modulos.md) — Árbol por directorios y reglas.

## Estado del proyecto

| Sección | Estado |
| --- | --- |
| Ficha técnica, planos, BOM, ficha descriptiva | Completado |
| Conexiones, librerías, calibración, migración ESP32, arquitectura | Completado |
| Firmware autónomo (base + modos) | En progreso |
| Ensamblaje | Completado |
| Troubleshooting | Pendiente |

## Inicio rápido (ESP32 + PlatformIO)

1. Revisa la [Ficha Técnica](docs/01-ficha-tecnica.md) y la [Lista de Materiales](docs/03-lista-materiales-bom.md).
2. Conecta según [Migración ESP32](docs/11-migracion-esp32.md) (TRIG 18, ECHO 19 con divisor, buzzer 27, servos 32/33/25/26).
3. Abre la raíz del repo en VSCode + PlatformIO. Envs: `otto_esp32` (producción con panel), `manual_1sg90` / `calibracion_90` (banco).
4. Para el panel: edita `firmware/src/wifi_secrets.h` (red 2.4 GHz única, sin BT), sube `otto_esp32` y abre `http://<ip>/` desde el teléfono.

## Estructura del repositorio

```text
.
├── README.md
├── CONTRIBUTING.md
├── LICENSE
├── hardware/
│   ├── stl/
│   │   ├── OttoDIY_body_V9.stl   # 69x69x39 mm, multi-pieza en único archivo (2026-10-13)
│   │   └── FUENTE.md
│   └── esquemas/
│       ├── esquema-conexiones.md
│       └── esquema-conexiones.svg
├── firmware/
│   ├── test/test_core/           # Unity en placa + PC (headers puros)
│   └── src/
│       ├── config.h / wifi_secrets.h / main.cpp
│       ├── core/                 # un subdirectorio por módulo
│       │   ├── modes/ autonomy/ validate/ sonar/ motion/ net/ audio/
│       ├── modules/              # buzzer (opcional vía sound_hook)
│       └── tools/                # manual_1sg90 + calibracion_90 (banco)
├── platformio.ini                # envs en raíz (src_dir/test_dir); build_src_filter vigente
├── tests/                        # suite Python: python tests/run_tests.py (53 tests)
└── docs/
    ├── index.md
    ├── 01-ficha-tecnica.md
    ├── 02-planos-stl.md
    ├── 03-lista-materiales-bom.md
    ├── 04-ficha-descriptiva.md
    ├── 05-guia-conexiones.md
    ├── 06-librerias.md
    ├── 07-codigo-calibracion.md
    ├── 08-firmware-autonomo.md
    ├── 09-manual-ensamblaje.md
    ├── 10-calibracion-troubleshooting.md
    ├── 11-migracion-esp32.md
    └── 12-arquitectura-modulos.md
```

## Contribución

Ver [CONTRIBUTING.md](CONTRIBUTING.md).

## Créditos

* Proyecto original Otto DIY por Camilo Parra Palacio (2016) — [ottodiy.tech](https://ottodiy.tech).
* Librería: [OttoDIYLib](https://github.com/OttoDIY/OttoDIYLib).
