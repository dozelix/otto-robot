// Buzzer pasivo en GPIO27 vía LEDC (segunda unidad, no la de servos).
// Esqueleto: configura LEDC a 2kHz y haz tone(ms) como en C++ tone(pin,freq,ms).

pub const BEEP_FREQ_HZ: u32 = 2000;

// TODO port: struct Buzzer { channel } con
//   fn beep(&mut self, freq_hz: u32, ms: u64)
//   fn beep_obstaculo(&mut self) { beep(2500,150); delay(200); beep(2500,150); }
