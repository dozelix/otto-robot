#include "buzzer.h"
#include <Arduino.h>
#include "../config.h"

void buzzerBegin() {
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}

void beep(int freq, int ms) {
#if defined(ESP32)
  tone(PIN_BUZZER, freq, ms);
  delay(ms + 20);
  noTone(PIN_BUZZER);
#else
  digitalWrite(PIN_BUZZER, HIGH);
  delay(ms);
  digitalWrite(PIN_BUZZER, LOW);
#endif
}

void beepDouble(int freq, int ms, int gap) {
  beep(freq, ms);
  delay(gap);
  beep(freq, ms);
}
