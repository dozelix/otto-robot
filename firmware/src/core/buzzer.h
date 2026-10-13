#pragma once
// Buzzer pasivo en PIN_BUZZER (GPIO27). En ESP32 usa tone().

void buzzerBegin();
void beep(int freq = 2000, int ms = 120);
void beepDouble(int freq = 2500, int ms = 150, int gap = 200);
