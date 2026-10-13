# Documentación Otto DIY

Índice de la documentación modular del robot Otto DIY (ESP32-WROOM, base SDD).

## Estado del proyecto

| N.º | Sección | Estado |
| --- | --- | --- |
| 01 | [Ficha Técnica](./01-ficha-tecnica.md) | Completado |
| 02 | [Planos STL](./02-planos-stl.md) | Completado (body V9 69×69×39 mm, 2026-10-13) |
| 03 | [Lista de Materiales (BOM)](./03-lista-materiales-bom.md) | Completado |
| 04 | [Ficha Descriptiva](./04-ficha-descriptiva.md) | Completado |
| 05 | [Guía de Conexiones](./05-guia-conexiones.md) | Completado (Nano; ESP32 en 11) |
| 06 | [Librerías y Entorno](./06-librerias.md) | Completado (PlatformIO + WiFi único) |
| 07 | [Código de Calibración](./07-codigo-calibracion.md) | Completado |
| 08 | [Firmware Autónomo](./08-firmware-autonomo.md) | En progreso (base + modos) |
| 09 | [Manual de Ensamblaje](./09-manual-ensamblaje.md) | Pendiente |
| 10 | [Calibración y Troubleshooting](./10-calibracion-troubleshooting.md) | Pendiente |
| 11 | [Migración ESP32](./11-migracion-esp32.md) | Completado |
| 12 | [Arquitectura y Módulos](./12-arquitectura-modulos.md) | Completado |

## Flujo de lectura recomendado

1. [Ficha Técnica](./01-ficha-tecnica.md) → [Planos](./02-planos-stl.md) → [Materiales](./03-lista-materiales-bom.md)
2. [Guía de Conexiones](./05-guia-conexiones.md) → [Librerías](./06-librerias.md) → [Calibración](./07-codigo-calibracion.md)
3. [Ensamblaje](./09-manual-ensamblaje.md) → [Firmware](./08-firmware-autonomo.md) → [Troubleshooting](./10-calibracion-troubleshooting.md)

## Hardware en el repo

| Carpeta | Contenido |
| --- | --- |
| [`../hardware/stl/`](../hardware/stl/FUENTE.md) | 4 STL oficiales `OttoDIY_*_V9` + trazabilidad |
| [`../hardware/esquemas/`](../hardware/esquemas/esquema-conexiones.md) | Esquema documentativo `.md` + [`.svg`](../hardware/esquemas/esquema-conexiones.svg) (sin PCB propia) |

[← Volver al README](../README.md)
