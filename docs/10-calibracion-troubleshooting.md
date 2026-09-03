# 10. Calibración y Troubleshooting

Guía de ajuste de servos y solución de problemas mecánicos/de movimiento.

> Nota: se excluye el tema de alimentación/baterías según alcance actual del proyecto.

## Prerrequisito

* Haber ejecutado el [Código de Calibración](./07-codigo-calibracion.md) (servos a 90°) antes del montaje definitivo.

## Trimming (si camina chueco)

Si el robot camina chueco o no pisa en plano:

1. Volver a subir el sketch de 90°.
2. Aflojar el tornillo del horn del servo desviado, reposicionar 1 diente y reapretar.
3. Si la librería lo soporta, aplicar offset por software (`trim`) y probar `Otto.home()` y `Otto.walk(4, 1000, 1)`.
4. Repetir hasta que en reposo los pies apoyen planos y las caderas queden simétricas.

## Tabla de problemas

| Problema | Causa probable | Solución |
| --- | --- | --- |
| Camina chueco / gira solo | Horn desplazado, trim sin ajustar | Recalibrar a 90°, reposicionar horn, ajustar trim |
| Pies no apoyan planos | Tobillo mal centrado | Repetir [calibración](./07-codigo-calibracion.md), recolocar horn de tobillo |
| Vibra / zumba en reposo | Horn forzado contra tope | Reposicionar horn 1-2 dientes, verificar tornillería M2 |
| No responde un servo | Cable S/V/G invertido o suelto | Revisar [Guía de Conexiones](./05-guia-conexiones.md), fila V = rojo |
| HC-SR04 mide mal | Sensor suelto en la cabeza | Reasentar sensor, verificar Trig D8 / Echo D9 |

## Referencias

* [Código de Calibración](./07-codigo-calibracion.md)
* [Guía de Conexiones](./05-guia-conexiones.md)
* [Manual de Ensamblaje](./09-manual-ensamblaje.md)

---

[← Manual de Ensamblaje](./09-manual-ensamblaje.md) | [Índice](../README.md) | [Docs](./index.md)
