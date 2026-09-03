# 05. Guía de Conexiones

Guía completa de conexiones pin a pin para el robot **Otto DIY estándar** utilizando el **Arduino Nano** sobre su **Nano Expansion Board (Sensor Shield)**.

> Esquema visual: [`hardware/esquemas/esquema-conexiones.svg`](../hardware/esquemas/esquema-conexiones.svg) · detalle en [`hardware/esquemas/esquema-conexiones.md`](../hardware/esquemas/esquema-conexiones.md). No requiere PCB propia.

---

## 1. Mapeo de Pines Principales

En la **Nano Expansion Board**, cada número de pin digital cuenta con tres pines dispuestas verticalmente:

* **S (Signal / Señal):** Pin con el número impreso (generalmente cable amarillo u naranja).
* **V (Voltage / 5V):** Pin de alimentación positiva (cable rojo).
* **G (Ground / Tierra):** Pin de masa/negativo (cable marrón o negro).

---

### A. Servomotores (4 x SG90)

| Elemento | Servo | Pin en Expansion Board | Cable Servo |
| --- | --- | --- | --- |
| **Pierna Izquierda** | Cadera Izquierda | **Pin 2** | Naranja (S) / Rojo (V) / Marrón (G) |
| **Pierna Derecha** | Cadera Derecha | **Pin 3** | Naranja (S) / Rojo (V) / Marrón (G) |
| **Pie Izquierdo** | Tobillo Izquierdo | **Pin 4** | Naranja (S) / Rojo (V) / Marrón (G) |
| **Pie Derecho** | Tobillo Derecho | **Pin 5** | Naranja (S) / Rojo (V) / Marrón (G) |

> **Nota importante:** Asegúrate de orientar el conector del servo correctamente en el conector de 3 pines de la placa: el cable **marrón** debe ir hacia la fila marcada con **G**, el **rojo** a la **V** y el **naranja** a la **S**.

---

### B. Sensor Ultrasónico (HC-SR04)

Conecta el sensor mediante 4 cables Jumper hembra-hembra a los siguientes pines de la placa de expansión:

| Pin HC-SR04 | Conexión en Nano Shield |
| --- | --- |
| **VCC** | Pin **V** (Cualquiera disponible en la placa, por ejemplo junto a Pin 8) |
| **Trig** | Pin **8** (Señal / S) |
| **Echo** | Pin **9** (Señal / S) |
| **GND** | Pin **G** (Cualquiera disponible en la placa) |

---

### C. Zumbador Piezoeléctrico (Buzzer)

| Pin Zumbador | Conexión en Nano Shield |
| --- | --- |
| **Positivo (+)** | Pin **13** (Señal / S) |
| **Negativo (-)** | Pin **G** (Tierra / G) |

---

## 2. Diagrama de Alimentación e Interruptor

Los servomotores consumen bastantes picos de corriente cuando se mueven todos simultáneamente. Por ello, la alimentación de la batería debe ingresar a través de los pines dedicados de la tarjeta o del conector de energía.

```
       [ Interruptor (Toggle Switch) ]
                    /
 (+) Batería ------o   o-------------------> Pin VCC / Vin de la Shield
 (-) Batería ------------------------------> Pin GND / G de la Shield

```

### Instrucciones de la Alimentación:

1. Conecta el cable **negativo (-)** del portabaterías directamente a un pin de tierra (**G**) en la placa de expansión.
2. Interrumpe el cable **positivo (+)** de la batería haciéndolo pasar a través de dos terminales del **interruptor (Toggle Switch)**.
3. El cable saliente del interruptor conéctalo al pin de entrada positiva (**Vin** o **VCC**) de la placa de expansión (o al Jack de alimentación si usas adaptador).

---

## 3. Verificación Previa al Primer Encendido

1. **Centrado previo:** Antes de montar mecánicamente las piernas y pies sobre los ejes de los servos, sube el código de centrado a la placa para dejar todos los servos fijados exactamente a **90°**.
2. **Polaridad:** Revisa que ningún conector del servo esté invertido (el cable rojo **siempre** debe estar alineado con la fila **V**).
3. **USB:** Si vas a programar el Arduino Nano por cable USB conectado a la computadora, es recomendable apagar el interruptor de la batería externa.

---

[← Ficha Descriptiva](./04-ficha-descriptiva.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Librerías →](./06-librerias.md)