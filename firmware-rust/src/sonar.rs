// HC-SR04 en Rust: TRIG 10us + medir ECHO con timeout.
// Esqueleto: completa con esp-hal GPIO + esp-hal::delay::Delay + instant::Instant.
// Fórmula igual que C++: cm = (us/2)/29.1, mediana de 5, -1 = fuera de rango.

pub fn us_to_cm(us: u32) -> f32 {
    (us as f32 / 2.0) / 29.1
}

pub fn mediana5(mut m: [f32; 5]) -> f32 {
    m.sort_by(|a, b| a.partial_cmp(b).unwrap());
    m[2]
}

// TODO port: fn leer_distancia_cm(trig, echo, delay) -> f32
//   trig.set_low(); delay.delay_micros(5);
//   trig.set_high(); delay.delay_micros(10); trig.set_low();
//   medir pulso HIGH con timeout 30ms (ver esp-hal gpio + Instant).
