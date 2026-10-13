"""Reglas de validate.h: spec ejecutable + verifica que autonomy las usa."""
import os
import unittest

from helpers import CORE, read

MASK32 = 0xFFFFFFFF


# --- Espejos de core/validate.h (mantener sincronizados) ---
def is_valid_steps(s, mx):
    return 1 <= s <= mx


def is_valid_period(t, lo, hi):
    return lo <= t <= hi


def is_valid_dir(d):
    return d in (1, -1)


def is_valid_beep_freq(f):
    return 100 <= f <= 5000


def is_valid_beep_ms(ms):
    return 20 <= ms <= 1000


def should_avoid(dist, umbral):
    return dist >= 0.0 and dist < umbral


def should_safe_home(n, mx):
    return n >= mx


def should_watchdog(now, last, timeout):
    return ((now - last) & MASK32) > timeout


def clamp_angle(a):
    return max(0.0, min(180.0, a))


class TestValidate(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.h = read(os.path.join(CORE, "validate.h"))
        cls.cpp = read(os.path.join(CORE, "autonomy.cpp"))

    def test_header_puro(self):
        self.assertNotIn("Arduino.h", self.h)
        for fn in ("isValidSteps", "isValidPeriodMs", "isValidDir",
                   "isValidBeepFreq", "isValidBeepMs", "isValidManual",
                   "shouldAvoid", "shouldSafeHome", "shouldWatchdogRevert",
                   "clampAngle"):
            self.assertIn(fn, self.h, fn)

    def test_autonomy_usa_helpers(self):
        for fn in ("isValidSteps", "isValidPeriodMs",
                   "isValidBeepFreq", "isValidBeepMs",
                   "shouldAvoid", "shouldSafeHome", "shouldWatchdogRevert"):
            self.assertIn(fn, self.cpp, fn)

    def test_steps(self):
        self.assertFalse(is_valid_steps(0, 4))
        self.assertTrue(is_valid_steps(1, 4))
        self.assertTrue(is_valid_steps(4, 4))
        self.assertFalse(is_valid_steps(5, 4))

    def test_periodo(self):
        self.assertFalse(is_valid_period(499, 500, 2000))
        self.assertTrue(is_valid_period(500, 500, 2000))
        self.assertTrue(is_valid_period(2000, 500, 2000))
        self.assertFalse(is_valid_period(2001, 500, 2000))

    def test_dir(self):
        self.assertTrue(is_valid_dir(1))
        self.assertTrue(is_valid_dir(-1))
        self.assertFalse(is_valid_dir(0))
        self.assertFalse(is_valid_dir(2))

    def test_beep(self):
        self.assertFalse(is_valid_beep_freq(99))
        self.assertTrue(is_valid_beep_freq(100))
        self.assertTrue(is_valid_beep_freq(5000))
        self.assertFalse(is_valid_beep_freq(5001))
        self.assertFalse(is_valid_beep_ms(19))
        self.assertTrue(is_valid_beep_ms(20))
        self.assertTrue(is_valid_beep_ms(1000))
        self.assertFalse(is_valid_beep_ms(1001))

    def test_should_avoid(self):
        self.assertFalse(should_avoid(-1.0, 20))   # inválida no es obstáculo
        self.assertTrue(should_avoid(0.0, 20))
        self.assertTrue(should_avoid(19.9, 20))
        self.assertFalse(should_avoid(20.0, 20))   # borde: libre
        self.assertFalse(should_avoid(100.0, 20))

    def test_safe_home(self):
        self.assertFalse(should_safe_home(4, 5))
        self.assertTrue(should_safe_home(5, 5))
        self.assertTrue(should_safe_home(6, 5))

    def test_watchdog(self):
        self.assertFalse(should_watchdog(10000, 0, 30000))
        self.assertFalse(should_watchdog(30000, 0, 30000))  # estricto >
        self.assertTrue(should_watchdog(30001, 0, 30000))

    def test_watchdog_overflow_millis(self):
        last = 0xFFFFFF00
        self.assertFalse(should_watchdog(0x00000100, last, 30000))  # 512 ms
        self.assertTrue(should_watchdog((last + 40000) & MASK32, last, 30000))

    def test_clamp(self):
        self.assertEqual(clamp_angle(-10.0), 0.0)
        self.assertEqual(clamp_angle(0.0), 0.0)
        self.assertEqual(clamp_angle(90.0), 90.0)
        self.assertEqual(clamp_angle(180.0), 180.0)
        self.assertEqual(clamp_angle(200.0), 180.0)


if __name__ == "__main__":
    unittest.main()
