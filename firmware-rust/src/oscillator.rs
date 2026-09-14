// Port de Oscillator.h (OttoDIYLib) a Rust no_std.
// pos = O + A * sin(2*pi*t/T + phase). Sin floats pesados: usa libm o tabla.
// Esta versión usa f32 simple; en no_std real añade `libm` al Cargo si el target lo pide.
use core::f32::consts::PI;

pub struct Oscillator {
    pub a: f32,     // amplitud grados
    pub o: f32,     // offset grados
    pub t: u32,     // periodo ms
    pub phase: f32, // fase radianes
}

impl Oscillator {
    pub fn new(a: f32, o: f32, t: u32, phase: f32) -> Self {
        Self { a, o, t, phase }
    }
    // t_ms: tiempo desde inicio del ciclo
    pub fn pos(&self, t_ms: u32) -> f32 {
        self.o + self.a * ((2.0 * PI * (t_ms as f32) / (self.t as f32)) + self.phase).sin()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn centro_en_90_con_offset() {
        let o = Oscillator::new(30.0, 90.0, 1000, 0.0);
        let p = o.pos(0);
        assert!((p - 90.0).abs() < 0.01);
    }
}
