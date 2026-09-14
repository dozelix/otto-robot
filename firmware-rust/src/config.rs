// Mismo pinout que firmware/src/config.h — cambiar aquí y en C++ a la vez.
// 1 = sin SG90 (solo sonar + log [SIM]). 0 = con servos.
pub const SIM_SIN_SERVOS: bool = true;

pub const SERVO_CI: u8 = 32;
pub const SERVO_CD: u8 = 33;
pub const SERVO_TI: u8 = 25;
pub const SERVO_TD: u8 = 26;

pub const PIN_TRIG: u8 = 18;
pub const PIN_ECHO: u8 = 19; // con divisor 1k/2k, nunca 5V directo
pub const PIN_BUZZER: u8 = 27;

pub const TRIM: [i8; 4] = [0, 0, 0, 0];

pub const UMBRAL_OBSTACULO_CM: f32 = 20.0;
pub const TIEMPO_PASO_MS: u64 = 1000;
pub const PERIODO_LECTURA_MS: u64 = 200;

// LEDC 50Hz 12-bit (SG90): 0°=102 / 90°=307 / 180°=512
pub const SERVO_FREQ_HZ: u32 = 50;
pub const DUTY_MIN: u32 = 102;
pub const DUTY_MAX: u32 = 512;
pub fn duty_from_angle(deg: u32) -> u16 {
    let gap = DUTY_MAX - DUTY_MIN;
    (DUTY_MIN + (deg * gap) / 180) as u16
}
