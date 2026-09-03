# Esquema de conexiones Otto DIY

> No se requiere PCB propia: se usa Arduino Nano + Nano Shield comercial. Este esquema es documentativo.

Ver detalle pin a pin en [`docs/05-guia-conexiones.md`](../../docs/05-guia-conexiones.md) y diagrama en [`esquema-conexiones.svg`](./esquema-conexiones.svg).

## Redes

| Red | Origen (Shield) | Destino | Cable |
| --- | --- | --- | --- |
| Cadera izq. | D2 S/V/G | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Cadera der. | D3 S/V/G | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Tobillo izq. | D4 S/V/G | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Tobillo der. | D5 S/V/G | SG90 | Naranja (S) / Rojo (V) / Marrón (G) |
| Trig | D8 (S) | HC-SR04 Trig | Jumper hembra-hembra |
| Echo | D9 (S) | HC-SR04 Echo | Jumper hembra-hembra |
| HC-SR04 VCC/GND | V / G libres | HC-SR04 VCC/GND | Jumper hembra-hembra |
| Buzzer + / - | D13 (S) / G | Buzzer pasivo 5V | Directo |
| Alimentación + | Vía interruptor → Vin/VCC | Shield | Según Nano: 7–12V Vin (ver Ficha Técnica) |
| Alimentación - | Directo → GND | Shield | — |

## Alimentación e interruptor

```text
       [ Interruptor ]
                    /
 (+) Fuente ------o   o-------------------> Vin / VCC Shield
 (-) Fuente ------------------------------> GND / G Shield
```

* Fuente compatible con Vin 7–12V de la Nano (ver [Ficha Técnica](../../docs/01-ficha-tecnica.md)).
* Al programar por USB, abrir el interruptor (apagar batería externa).
* Rojo siempre a V, marrón/negro a G, señal a S.

## Archivos

| Archivo | Descripción |
| --- | --- |
| `esquema-conexiones.md` | Este índice (texto) |
| `esquema-conexiones.svg` | Diagrama visual para el informe |
