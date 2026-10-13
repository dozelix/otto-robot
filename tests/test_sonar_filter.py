"""Filtro sonar: spec ejecutable del algoritmo + verifica el refactor.

Nota float32: el firmware trunca en float32 y aquí se calcula en float64.
Los asserts usan márgenes (no igualdades exactas en bordes).
"""
import os
import unittest

from helpers import CORE, read


def cm_from_us(us):
    if us == 0:
        return -1.0
    return (us / 2.0) / 29.1


def scaled(cm):
    return int(cm * 10.0)  # trunca como (long) en C++


def median5(s):
    return sorted(s)[2]


def filter_cm(samples):
    m = [scaled(cm_from_us(us)) for us in samples]
    med = median5(m)
    if not (20 <= med <= 4000):
        return -1.0
    return med / 10.0


class TestSonarFilter(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.h = read(os.path.join(CORE, "sonar_filter.h"))
        cls.cpp = read(os.path.join(CORE, "sonar.cpp"))

    def test_header_puro_y_api(self):
        self.assertNotIn("Arduino.h", self.h)
        for fn in ("sonarCmFromUs", "sonarScaled", "sonarCmValid",
                   "sonarMedian5", "sonarFilterCm"):
            self.assertIn(fn, self.h, fn)

    def test_sonar_cpp_usa_filtro(self):
        self.assertIn("sonar_filter.h", self.cpp)
        self.assertIn("sonarFilterCm", self.cpp)
        self.assertIn("pulseIn", self.cpp)  # capa hardware intacta

    def test_timeout(self):
        self.assertEqual(cm_from_us(0), -1.0)
        self.assertEqual(filter_cm([0, 0, 0, 0, 0]), -1.0)

    def test_valor_central(self):
        self.assertAlmostEqual(filter_cm([600] * 5), 10.3, delta=0.06)

    def test_borde_inferior(self):
        self.assertEqual(filter_cm([116] * 5), -1.0)      # ~1.9 cm
        self.assertAlmostEqual(filter_cm([118] * 5), 2.0, delta=0.06)

    def test_borde_superior(self):
        r = filter_cm([23280] * 5)
        self.assertTrue(399.0 <= r <= 400.0, r)            # ~400 cm válido
        self.assertEqual(filter_cm([23400] * 5), -1.0)    # fuera de rango

    def test_mediana_rechaza_outlier(self):
        self.assertAlmostEqual(
            filter_cm([600, 600, 600, 600, 23400]), 10.3, delta=0.06)
        self.assertAlmostEqual(
            filter_cm([0, 600, 600, 600, 600]), 10.3, delta=0.06)


if __name__ == "__main__":
    unittest.main()
