# Tests

Dos capas. La lógica pura vive en headers sin Arduino
(`core/validate/validate.h`, `core/sonar/sonar_filter.h`,
`core/motion/gait_params.h`, `core/modes/modes.h`)
para poder probarse en host y en placa. Las rutas canónicas están en
`tests/helpers.py`: si `core/` se reorganiza, se tocan solo ahí.

## 1. Suite Python (sin toolchain, corre en cualquier PC)

```text
python tests/run_tests.py
```

53 tests, solo stdlib (Python 3.7+). Verifican:

| Archivo | Qué cubre |
| --- | --- |
| `test_config.py` | Pines únicos, servos 32/33/25/26, umbrales y límites (`config.h`) |
| `test_modes.py` | `EXPOSITION=0`/`MANUAL=1`, tabla `modeFromString`, API de `autonomy.h` |
| `test_gait_params.py` | Fases walk/turn, simetría, ángulos en 0–180°, paridad con `firmware-rust` |
| `test_validate.py` | Bordes steps/T/dir/beep, `shouldAvoid/SafeHome/Watchdog` (incl. overflow `millis`) |
| `test_sonar_filter.py` | Conversión us→cm, mediana anti-outliers, bordes 2/400 cm |
| `test_wifi_policy.py` | `core/net` siempre presente, BT deshabilitado, sin BLE/AP, una red, endpoints, gate 409, panel mobile-first |
| `test_structure.py` | Árbol `core/` por directorios, envs PIO, `build_src_filter` vigente, sin `OttoDIYLib`, runner Unity |

Los mirrors Python de `validate`/`sonar_filter`/`gait`/`modes` son la spec
ejecutable: si cambias un umbral en `config.h` o una regla en los headers,
estos tests fallan primero.

## 2. Tests Unity en placa (requiere VSCode + PlatformIO)

```text
pio test -e test_core_esp32
```

Compila solo los headers puros (`test_build_src = no`) y corre Unity en el
ESP32, con resultados por serial 115200. Casos en `firmware/test/test_core/`
(`test_validate/sanar_filter/gait_params/modes.cpp`), en correspondencia 1:1
con la suite Python.

## Nota float32

El firmware trunca en `float` (32 bit) y Python calcula en 64 bit. Los asserts
de distancia usan márgenes (`delta`), no igualdades exactas en bordes.

## Al agregar un módulo

1. Pon sus reglas puras en un header sin Arduino en `core/` o `modules/`.
2. Agrega casos en `tests/test_*.py` y su mirror en `firmware/test/test_core/`.
3. Corre ambas capas antes de commitear.
