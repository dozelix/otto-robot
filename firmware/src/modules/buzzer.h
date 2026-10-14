#pragma once
// Buzzer pasivo en PIN_BUZZER (GPIO27). En ESP32 usa tone().
// MODULO OPCIONAL: el robot base solo usa servos + proximidad.
// Se registra en core/audio/sound_hook.h via setSoundHooks(). core/ nunca
// incluye este header directamente.

void buzzerBegin();
void buzzBeep(int freq = 2000, int ms = 120);
void buzzBeepDouble(int freq = 2500, int ms = 150, int gap = 200);

// Compat: autonomia/tools viejos llamaban beep()/beepDouble().
inline void beep(int freq = 2000, int ms = 120) { buzzBeep(freq, ms); }
inline void beepDouble(int freq = 2500, int ms = 150, int gap = 200) {
  buzzBeepDouble(freq, ms, gap);
}
