#pragma once
// Máquina de estados base + modos EXPOSITION/MANUAL.
// core/net/net_wifi (siempre presente) solo llama a setMode()/manual*().
// Sonido opcional vía core/audio/sound_hook.h (modules/buzzer se registra).

#include "../modes/modes.h"

void autonomyInit();
void autonomyTick();

// Modos
void setMode(OttoMode m);
OttoMode getMode();
bool isAutonomyEnabled();

// Llamar en cada comando manual válido (resetea watchdog).
void notifyManualCommand();

// Última distancia válida (-1 = inválida). Para /status y logs.
float lastDistanceCm();

// Acciones manuales (solo se ejecutan en MODE_MANUAL, si no devuelven false).
// Validan límites de seguridad (MANUAL_MAX_STEPS, T 500-2000 ms).
bool manualWalk(int steps, int T_ms, int dir);
bool manualTurn(int steps, int T_ms, int dir);
bool manualHome();
bool manualDance();
bool manualBeep(int freq, int ms);
