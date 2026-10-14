"""Contrato de firmware/src/config.h: pines únicos y umbrales sanos."""
import os
import unittest

from helpers import SRC, read, parse_int_defines


class TestConfig(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.raw = read(os.path.join(SRC, "config.h"))
        cls.cfg = parse_int_defines(cls.raw)

    def test_flags_base(self):
        self.assertEqual(self.cfg["SIM_SIN_SERVOS"], 1)
        # WiFi siempre presente en core: sin flag condicional.
        self.assertNotIn("ENABLE_WIFI_MODULE", self.raw)

    def test_pines_unicos(self):
        pins = {k: self.cfg[k] for k in
                ("SERVO_CI", "SERVO_CD", "SERVO_TI", "SERVO_TD",
                 "PIN_TRIG", "PIN_ECHO", "PIN_BUZZER", "PIN_SERVO_TEST")}
        self.assertEqual(len(set(pins.values())), len(pins),
                         "pines duplicados: %r" % (pins,))

    def test_pines_servos_principales(self):
        self.assertEqual(
            {self.cfg["SERVO_CI"], self.cfg["SERVO_CD"],
             self.cfg["SERVO_TI"], self.cfg["SERVO_TD"]},
            {32, 33, 25, 26})
        # El GPIO2 (strapping + LED) queda solo para el test de 1 servo.
        self.assertEqual(self.cfg["PIN_SERVO_TEST"], 2)
        for p in ("SERVO_CI", "SERVO_CD", "SERVO_TI", "SERVO_TD"):
            self.assertNotEqual(self.cfg[p], 2)

    def test_pines_sensor_buzzer(self):
        self.assertEqual(self.cfg["PIN_TRIG"], 18)
        self.assertEqual(self.cfg["PIN_ECHO"], 19)
        self.assertEqual(self.cfg["PIN_BUZZER"], 27)

    def test_umbrales_autonomia(self):
        self.assertEqual(self.cfg["UMBRAL_OBSTACULO_CM"], 20)
        self.assertTrue(5 <= self.cfg["UMBRAL_OBSTACULO_CM"] <= 100)
        self.assertEqual(self.cfg["TIEMPO_PASO_MS"], 1000)
        self.assertTrue(self.cfg["PERIODO_LECTURA_MS"] > 0)
        self.assertEqual(self.cfg["FILTRO_MUESTRAS"], 5)  # mediana de 5
        self.assertTrue(self.cfg["MAX_LECTURAS_INVALIDAS"] > 0)

    def test_limites_manuales(self):
        self.assertEqual(self.cfg["MANUAL_MAX_STEPS"], 4)
        self.assertTrue(1 <= self.cfg["MANUAL_MAX_STEPS"] <= 8)
        self.assertEqual(self.cfg["MANUAL_MIN_T_MS"], 500)
        self.assertEqual(self.cfg["MANUAL_MAX_T_MS"], 2000)
        self.assertLess(self.cfg["MANUAL_MIN_T_MS"], self.cfg["MANUAL_MAX_T_MS"])
        self.assertEqual(self.cfg["MANUAL_WATCHDOG_MS"], 30000)
        self.assertEqual(self.cfg["DEFAULT_MODE_EXPOSITION"], 1)


if __name__ == "__main__":
    unittest.main()
