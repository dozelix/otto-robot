# 09. Manual de Ensamblaje Mecánico

> Solo texto (sin fotos por ahora). Sigue las etapas en orden: cada una
> termina con una verificación antes de pasar a la siguiente.

## Prerrequisitos

* Piezas impresas: ver [Planos STL](./02-planos-stl.md).
* Tornillería y electrónica: ver [Lista de Materiales](./03-lista-materiales-bom.md).
* Orden de conexiones: ver [Guía de Conexiones](./05-guia-conexiones.md).
* Herramientas: destornillador cruz pequeño, pinzas para cables.

## Etapa 0 — Piezas y banco electrónico (sin montar nada)

1. Verifica las piezas impresas (cuerpo, cabeza, 2 piernas, 2 pies): sin
   hilos sueltos ni restos en los huecos de servos y cables.
2. Conecta solo ESP32 por USB y el HC-SR04 (etapas 1–2 de [Conexiones](./05-guia-conexiones.md)).
3. ✅ Verificación: `dist: X cm` estable en el monitor al acercar la mano.

## Etapa 1 — Un servo en banco

1. Conecta solo la señal (naranja) de 1 SG90 a GPIO2, sin fuente aún.
2. Sube `env:manual_1sg90`: centra a 90° 2 s (momento de presentar el horn).
3. ✅ Verificación: el servo obedece `0..180/c/s` por serial y el sonar sigue midiendo.

## Etapa 2 — Fuente externa y GND común

1. Con la fuente **apagada**, cablea rojo→5V, marrón→GND, une GND fuente↔ESP32.
2. Interruptor en serie con el (+) de la fuente.
3. Enciende la fuente y repite el barrido del servo.
4. ✅ Verificación: movimiento firme, sin jitter ni resets del ESP32.

## Etapa 3 — Calibración a 90° (antes de atornillar nada)

1. Conecta las 4 señales: CI→32, CD→33, TI→25, TD→26 (potencia a la fuente).
2. Sube `env:calibracion_90`: los 4 servos quedan fijos a 90°.
3. ✅ Verificación: los 4 ejes inmóviles a 90°. **No apagues**: monta sobre esta posición.

## Etapa 4 — Caderas al cuerpo

1. Fija los 2 servos de cadera al cuerpo con M2 x 6/8 mm (eje hacia afuera).
2. Presenta los horns a 90° y atorníllalos al eje (M2 x 4 mm al eje).
3. ✅ Verificación: ambas caderas a 90°, horns simétricos.

## Etapa 5 — Piernas y paso de cables

1. Pasa los cables de tobillo por el hueco interior de cada pierna (pinzas).
2. Une pierna al horn de cadera con sus tornillos.
3. ✅ Verificación: cables sin tensión ni pellizcos al mover la pierna a mano.

## Etapa 6 — Pies y tobillos

1. Monta cada servo de tobillo dentro de su pie (M2 x 6/8 mm).
2. Horn a 90° y pie atornillado al horn.
3. Apoya el robot: debe pararse solo en `home()` (90° en todo).
4. ✅ Verificación: pies planos y simétricos sobre la mesa.

## Etapa 7 — Electrónica en el chasis

1. Fija el ESP32 al cuerpo (M2 x 5 mm o cinta doble faz).
2. Conecta señales 32/33/25/26, sonar 18/19, buzzer 27, según [Conexiones](./05-guia-conexiones.md).
3. Ordena cables dentro del cuerpo, sin tocar los horns.
4. ✅ Verificación: con fuente encendida, `home()` deja al robot parado y estable.

## Etapa 8 — Cabeza y sensor

1. Coloca el HC-SR04 en los recortes de los ojos (cableado hacia atrás).
2. Pasa TRIG/ECHO/VCC/GND al interior y cierra la cabeza.
3. ✅ Verificación: `dist` responde tapando/alejando la mano frente a los ojos.

## Etapa 9 — Primera marcha y panel

1. Sube `env:otto_esp32` (`SIM=0` con servos reales) y enciende la fuente.
2. En `exposition` debe caminar y girar ante obstáculos < 20 cm.
3. Configura `wifi_secrets.h`, abre `http://<ip>/` desde el teléfono y prueba el modo manual.
4. ✅ Verificación: camina recto (si no, ajusta trims: ver [Troubleshooting](./10-calibracion-troubleshooting.md)).

---

[← Firmware Autónomo](./08-firmware-autonomo.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Calibración y Troubleshooting →](./10-calibracion-troubleshooting.md)
