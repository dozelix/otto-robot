# 06. Librerías y Entorno

Para programar todas las funciones de **Otto DIY** (movimientos, bailes, gestos y sonidos) utilizando el código estándar desarrollado por la comunidad, necesitas instalar **2 librerías principales**:

---

## 1. Librerías Necesarias

### 1. OttoDIYLib (Librería Principal)

Es la librería central que abstrae la cinemática del robot. Incluye todas las funciones avanzadas como `walk()`, `turn()`, `bend()`, `shakeLeg()`, `sing()` y `getDistance()`.

* **Librerías internas que incluye o requiere:**
* **`Servo.h`**: Viene integrada por defecto en el entorno de Arduino IDE.
* **`Oscillator.h`**: Controla la generación de ondas senoidales para el movimiento fluido e interconectado de los servomotores.
* **`US.h`**: Maneja la lectura de distancias en centímetros utilizando el sensor ultrasónico HC-SR04.



### 2. US100 (Opcional pero Recomendada)

Maneja sensores de ultrasonido alternativos o calibraciones de temporización específicas en los modelos HC-SR04.

---

## 2. Método de Instalación

Existen dos formas de instalarlas en **Arduino IDE**:

### Opción A: Desde el Gestor de Librerías (Recomendado)

1. Abre **Arduino IDE**.
2. Ve al menú superior: **Herramientas** > **Administrar bibliotecas...** (o presiona `Ctrl + Shift + I`).
3. En el buscador escribe **OttoDIY**.
4. Selecciona la librería **OttoDIYLib** por *Camilo Parra Palacio / Otto DIY*.
5. Haz clic en **Instalar** (si te pregunta si deseas instalar todas las dependencias adicionales, selecciona **Instalar todo / Install all**).

### Opción B: Instalación Manual por Archivo .ZIP

Si descargaste las librerías desde el repositorio de GitHub de Otto DIY ([github.com/OttoDIY/OttoDIYLib](https://github.com/OttoDIY/OttoDIYLib)):

1. En Arduino IDE, ve a **Programa** > **Incluir Librería** > **Añadir biblioteca .ZIP...**
2. Selecciona el archivo `.zip` descargado.

---

## 3. Estructura Básica en Código (Ejemplo de Prueba)

Una vez instaladas las librerías, la estructura mínima para inicializar a Otto en tu *sketch* de Arduino es la siguiente:

```cpp
#include <Otto.h>

Otto Otto; // Instancia del objeto Otto

// Pines asignados
#define SERVO_CI 2 // Cadera Izquierda
#define SERVO_CD 3 // Cadera Derecha
#define SERVO_TI 4 // Tobillo Izquierdo
#define SERVO_TD 5 // Tobillo Derecho
#define BUZZER   13 // Zumbador
#define TRIG     8  // Ultrasónico Trigger
#define ECHO     9  // Ultrasónico Echo

void setup() {
  // Inicialización de servos y accesorios
  Otto.init(SERVO_CI, SERVO_CD, SERVO_TI, SERVO_TD, true, BUZZER, TRIG, ECHO);
  
  // Sonido de bienvenida y postura inicial
  Otto.sing(S_connection);
  Otto.home();
}

void loop() {
  // Caminar 4 pasos hacia adelante
  Otto.walk(4, 1000, 1); // (pasos, T_velocidad, dirección: 1 adelante / -1 atrás)
  
  // Emitir un sonido alegre
  Otto.sing(S_happy);
  delay(1000);
}
```

---

[← Guía de Conexiones](./05-guia-conexiones.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Código de Calibración →](./07-codigo-calibracion.md)