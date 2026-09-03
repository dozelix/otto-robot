# 09. Manual de Ensamblaje Mecánico

> **Estado: Pendiente.** Solo texto (sin fotos por ahora).

Secuencia de montaje físico de las partes impresas y colocación de cables dentro del cuerpo.

## Prerrequisitos

* Piezas impresas: ver [Planos STL](./02-planos-stl.md).
* Tornillería y electrónica: ver [Lista de Materiales](./03-lista-materiales-bom.md).
* Servos centrados a 90°: ver [Código de Calibración](./07-codigo-calibracion.md).
* Conexiones: ver [Guía de Conexiones](./05-guia-conexiones.md).

## Herramientas

* Destornillador pequeño de cruz.
* Pinzas para pasar cables.

## Pasos (texto)

1. **Centrar servos:** subir el sketch de calibración y dejar los 4 servos a 90° antes de atornillar horns.
2. **Caderas:** fijar 2 servos al cuerpo (`Otto_Body`) con tornillos M2 x 6/8 mm.
3. **Piernas:** pasar cables por las piernas (`Otto_Leg_L/R`) y fijar horns al eje.
4. **Pies:** montar servos de tobillo en los pies (`Otto_Foot_L/R`) con tornillos M2 x 4 mm.
5. **Electrónica:** fijar la Nano Shield al chasis con M2 x 5 mm, conectar servos D2-D5, HC-SR04 D8-D9 y buzzer D13.
6. **Cabeza:** colocar el HC-SR04 en los ojos, pasar cables y cerrar la cabeza (`Otto_Head`).
7. **Verificación:** encender con interruptor apagado al programar por USB, revisar polaridad S/V/G.

<!-- TODO: detallar orden de inserción, paso de cables y cierre de cabeza -->

---

[← Firmware Autónomo](./08-firmware-autonomo.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Calibración y Troubleshooting →](./10-calibracion-troubleshooting.md)
