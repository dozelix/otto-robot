"""Modos EXPOSITION/MANUAL: contrato de modes.h + spec ejecutable."""
import re
import unittest

from helpers import AUTONOMY_H, MODES_H, read


def mode_from_string(s):
    """Espejo de modeFromString() en core/modes.h."""
    if s is None or len(s) == 0:
        return "EXPOSITION"
    if s[0] in ("m", "M") or s[0] == "1":
        return "MANUAL"
    return "EXPOSITION"


def mode_name(m):
    return "manual" if m == "MANUAL" else "exposition"


class TestModes(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.h = read(MODES_H)
        cls.autonomy_h = read(AUTONOMY_H)

    def test_header_puro_sin_arduino(self):
        self.assertNotIn("Arduino.h", self.h)

    def test_enum_valores(self):
        self.assertIn("enum class OttoMode", self.h)
        self.assertTrue(re.search(r"EXPOSITION\s*=\s*0", self.h))
        self.assertTrue(re.search(r"MANUAL\s*=\s*1", self.h))

    def test_mode_name(self):
        self.assertIn('"manual"', self.h)
        self.assertIn('"exposition"', self.h)
        self.assertEqual(mode_name("MANUAL"), "manual")
        self.assertEqual(mode_name("EXPOSITION"), "exposition")

    def test_mode_from_string_tabla(self):
        for s in ("manual", "m", "M", "1"):
            self.assertEqual(mode_from_string(s), "MANUAL", s)
        for s in ("exposition", "e", "E", "x", "", None):
            self.assertEqual(mode_from_string(s), "EXPOSITION", repr(s))

    def test_api_autonomy_declarada(self):
        for fn in ("setMode", "getMode", "isAutonomyEnabled",
                   "notifyManualCommand", "lastDistanceCm",
                   "manualWalk", "manualTurn", "manualHome",
                   "manualDance", "manualBeep"):
            self.assertIn(fn, self.autonomy_h, fn)


if __name__ == "__main__":
    unittest.main()
