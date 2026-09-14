// Port de Otto::walk/turn/home (Otto.cpp) a parámetros puros.
// Mismos valores que C++ para que el port se comporte igual.

pub struct GaitParams {
    pub a: [f32; 4],
    pub o: [f32; 4],
    pub phase_deg: [f32; 4],
}

// walk(steps, T, dir): caderas en fase, pies en fase, 90° desface cadera-pie
pub fn walk_params(dir: i8) -> GaitParams {
    let p = if dir >= 0 { -90.0 } else { 90.0 };
    GaitParams {
        a: [30.0, 30.0, 20.0, 20.0],
        o: [0.0, 0.0, 4.0, -4.0],
        phase_deg: [0.0, 0.0, p, p],
    }
}

pub fn turn_params(dir: i8) -> GaitParams {
    // Giro: misma amplitud, fases opuestas entre lados (simplificado del original)
    let p = if dir >= 0 { 90.0 } else { -90.0 };
    GaitParams {
        a: [30.0, 30.0, 20.0, 20.0],
        o: [0.0, 0.0, 4.0, -4.0],
        phase_deg: [0.0, 180.0, p, p + 180.0],
    }
}

pub fn home_angles() -> [f32; 4] {
    [90.0, 90.0, 90.0, 90.0]
}
