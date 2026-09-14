#![no_std]
#![no_main]

use esp_backtrace as _;
use esp_hal::{clock::CpuClock, delay::Delay, main};
use esp_println::println;

mod buzzer;
mod config;
mod gait;
mod oscillator;
mod sonar;

use config::*;

esp_bootloader_esp_idf::esp_app_desc!();

#[main]
fn main() -> ! {
    let cfg = esp_hal::Config::default().with_cpu_clock(CpuClock::max());
    let _p = esp_hal::init(cfg);
    let delay = Delay::new();

    println!("[Otto-ESP32] Fase 2 Rust (port de firmware/src/main.cpp)");

    if SIM_SIN_SERVOS {
        println!("[SIM_SIN_SERVOS=true] Sin SG90: sonar real + log [SIM]");
        // TODO: init TRIG/ECHO/BUZZER con esp-hal GPIO + LEDC.
        // Referencia C++: leerDistanciaCM() con mediana de 5 + beep si < UMBRAL.
        loop {
            // TODO: let d = sonar::leer_distancia_cm(...);
            let d: f32 = -1.0; // placeholder hasta cablear sonar.rs
            if d < 0.0 {
                println!("dist: fuera de rango");
            } else {
                println!("dist: {} cm", d as u32);
            }
            if d > 0.0 && d < UMBRAL_OBSTACULO_CM {
                println!("[SIM] obstaculo -> [SIM] turn + beep");
            } else if d > 0.0 {
                println!("[SIM] libre -> [SIM] walk(4)");
            }
            // Ejemplo uso gait portado (cuando haya servos):
            let _w = gait::walk_params(1);
            let _home = gait::home_angles();
            delay.delay_millis(PERIODO_LECTURA_MS as u32);
        }
    } else {
        // TODO CON servos: crea 4x esp_hal_servo::Servo (CI,CD,TI,TD),
        // home() a 90+TRIM, luego loop walk/turn según sonar igual que C++.
        // Ver config::duty_from_angle + oscillator::Oscillator para el refresh.
        println!("Modo CON servos: implementar attach + home + gait loop");
        loop {
            delay.delay_millis(1000);
        }
    }
}
