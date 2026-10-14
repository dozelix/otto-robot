#pragma once
// Hook de sonido para desacoplar core/ de modules/.
// core/autonomy llama a soundBegin/soundBeep/soundBeepDouble.
// Si hay buzzer (modules/), main.cpp lo registra con setSoundHooks().
// Sin registro: no-op (robot sin buzzer sigue andando: servos + sonar).

typedef void (*SoundBeginFn)();
typedef void (*SoundBeepFn)(int freq, int ms);
typedef void (*SoundBeepDoubleFn)(int freq, int ms, int gap);

void setSoundHooks(SoundBeginFn beginFn, SoundBeepFn beepFn,
                   SoundBeepDoubleFn beepDoubleFn);

void soundBegin();
void soundBeep(int freq, int ms);
void soundBeepDouble(int freq = 2500, int ms = 150, int gap = 200);
