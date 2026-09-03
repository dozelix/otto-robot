# Otto DIY Robot

Robot bípedo educativo de código abierto basado en **Arduino Nano**, con chasis impreso en 3D, 4 servos SG90, sensor ultrasónico HC-SR04 y buzzer. Proyecto documentado con fines académicos.

Documentación completa en [`docs/index.md`](docs/index.md).

## Tabla de Contenido

### Hardware y diseño

1. [Ficha Técnica](docs/01-ficha-tecnica.md) — Microcontrolador, actuadores, sensores, alimentación y entorno.
2. [Planos STL](docs/02-planos-stl.md) — 5 piezas para impresión 3D y parámetros recomendados.
3. [Lista de Materiales (BOM)](docs/03-lista-materiales-bom.md) — Electrónica y tornillería.
4. [Ficha Descriptiva](docs/04-ficha-descriptiva.md) — Cómo, cuándo, por qué y dónde del proyecto.
5. [Guía de Conexiones](docs/05-guia-conexiones.md) — Mapeo de pines, alimentación e interruptor.

### Software y puesta en marcha

6. [Librerías y Entorno](docs/06-librerias.md) — Instalación de `OttoDIYLib` y ejemplo mínimo.
7. [Código de Calibración](docs/07-codigo-calibracion.md) — Centrado de servos a 90°.
8. [Firmware Autónomo](docs/08-firmware-autonomo.md) — *Pendiente:* comportamiento autónomo.
9. [Manual de Ensamblaje](docs/09-manual-ensamblaje.md) — *Pendiente:* montaje mecánico paso a paso (texto).
10. [Calibración y Troubleshooting](docs/10-calibracion-troubleshooting.md) — *Pendiente:* trimming y solución de problemas.

## Estado del proyecto

| Sección | Estado |
| --- | --- |
| Ficha técnica, planos, BOM, ficha descriptiva | Completado |
| Conexiones, librerías, calibración | Completado |
| Firmware autónomo, ensamblaje, troubleshooting | Pendiente |

## Inicio rápido

1. Revisa la [Ficha Técnica](docs/01-ficha-tecnica.md) y la [Lista de Materiales](docs/03-lista-materiales-bom.md).
2. Conecta según la [Guía de Conexiones](docs/05-guia-conexiones.md).
3. Instala las [Librerías](docs/06-librerias.md) y sube el [Código de Calibración](docs/07-codigo-calibracion.md) para dejar los servos a 90° antes del montaje.

## Estructura del repositorio

```text
.
├── README.md
├── CONTRIBUTING.md
├── LICENSE
├── hardware/
│   ├── stl/
│   │   ├── OttoDIY_head_V9.stl
│   │   ├── OttoDIY_body_V9.stl
│   │   ├── OttoDIY_Leg_V9.stl
│   │   ├── OttoDIY_foot_V9.stl
│   │   └── FUENTE.md
│   └── esquemas/
│       ├── esquema-conexiones.md
│       └── esquema-conexiones.svg
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
    └── 10-calibracion-troubleshooting.md
```

## Contribución

Ver [CONTRIBUTING.md](CONTRIBUTING.md).

## Créditos

* Proyecto original Otto DIY por Camilo Parra Palacio (2016) — [ottodiy.tech](https://ottodiy.tech).
* Librería: [OttoDIYLib](https://github.com/OttoDIY/OttoDIYLib).
