#include "sound_hook.h"

static SoundBeginFn g_begin = nullptr;
static SoundBeepFn g_beep = nullptr;
static SoundBeepDoubleFn g_beepDouble = nullptr;

void setSoundHooks(SoundBeginFn beginFn, SoundBeepFn beepFn,
                   SoundBeepDoubleFn beepDoubleFn) {
  g_begin = beginFn;
  g_beep = beepFn;
  g_beepDouble = beepDoubleFn;
}

void soundBegin() {
  if (g_begin) g_begin();
}

void soundBeep(int freq, int ms) {
  if (g_beep) g_beep(freq, ms);
}

void soundBeepDouble(int freq, int ms, int gap) {
  if (g_beepDouble) {
    g_beepDouble(freq, ms, gap);
    return;
  }
  // Fallback sin beepDouble registrado: dos beeps simples.
  if (g_beep) {
    g_beep(freq, ms);
    g_beep(freq, ms);
  }
}
