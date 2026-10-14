#pragma once
// HC-SR04 con filtro de mediana (N=FILTRO_MUESTRAS). -1 = fuera de rango.
// Requiere divisor en ECHO (1k/2k a GND). Nunca 5V directo al GPIO.

void sonarBegin();
float sonarReadCm();
long sonarPulseUs();
