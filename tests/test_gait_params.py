"""Parámetros de marcha: valores de gait_params.h + cotas de seguridad."""
import os
import re
import unittest

from helpers import (AUTONOMY_CPP, GAIT_CPP, GAIT_H, GAIT_PARAMS_H,
                     REPO_ROOT, VALIDATE_H, read)


def walk_params(d):
    p = -90.0 if d >= 0 else 90.0
    return {"a": [30.0, 30.0, 20.0, 20.0],
            "o": [0.0, 0.0, 4.0, -4.0],
            "ph": [0.0, 0.0, p, p]}


def turn_params(d):
    p = 90.0 if d >= 0 else -90.0
    return {"a": [30.0, 30.0, 20.0, 20.0],
            "o": [0.0, 0.0, 4.0, -4.0],
            "ph": [0.0, 180.0, p, p + 180.0]}


def angle_range(g):
    lo = min(90.0 + o - a for o, a in zip(g["o"], g["a"]))
    hi = max(90.0 + o + a for o, a in zip(g["o"], g["a"]))
    return lo, hi


class TestGaitParams(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.h = read(GAIT_PARAMS_H)
        cls.gait_h = read(GAIT_H)
        cls.gait_cpp = read(GAIT_CPP)

    def test_fuente_unica(self):
        self.assertIn("GaitParams", self.h)
        self.assertIn("walkParams", self.h)
        self.assertIn("turnParams", self.h)
        self.assertNotIn("Arduino.h", self.h)
        self.assertIn("gait_params.h", self.gait_h)
        self.assertNotIn("GaitParams walkParams", self.gait_cpp)
        self.assertNotIn("GaitParams turnParams", self.gait_cpp)

    def test_valores_en_fuente(self):
        self.assertIn("30.0f, 30.0f, 20.0f, 20.0f", self.h)
        self.assertIn("0.0f, 0.0f, 4.0f, -4.0f", self.h)
        self.assertTrue(re.search(r"dir >= 0.*-90\.0f", self.h, re.S))

    def test_walk_fases(self):
        self.assertEqual(walk_params(1)["ph"], [0.0, 0.0, -90.0, -90.0])
        self.assertEqual(walk_params(-1)["ph"], [0.0, 0.0, 90.0, 90.0])

    def test_turn_fases(self):
        self.assertEqual(turn_params(1)["ph"], [0.0, 180.0, 90.0, 270.0])
        self.assertEqual(turn_params(-1)["ph"], [0.0, 180.0, -90.0, 90.0])

    def test_simetria(self):
        w = walk_params(1)
        self.assertEqual(w["a"][0], w["a"][1])      # caderas iguales
        self.assertEqual(w["o"][2], -w["o"][3])      # tobillos espejo

    def test_angulos_dentro_de_rango(self):
        for g in (walk_params(1), walk_params(-1),
                  turn_params(1), turn_params(-1)):
            lo, hi = angle_range(g)
            self.assertGreaterEqual(lo, 0.0, g)
            self.assertLessEqual(hi, 180.0, g)
            for a in g["a"]:
                self.assertLessEqual(a, 30.0)

    def test_paridad_con_rust(self):
        rust = read(os.path.join(REPO_ROOT, "firmware-rust", "src", "gait.rs"))
        for token in ("30.0, 30.0, 20.0, 20.0", "4.0, -4.0",
                      "90.0, 90.0, 90.0", "-90.0"):
            self.assertIn(token, rust, token)


if __name__ == "__main__":
    unittest.main()
