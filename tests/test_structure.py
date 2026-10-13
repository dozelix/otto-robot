"""Estructura: archivos base, envs PlatformIO y runner Unity en placa."""
import os
import re
import unittest

from helpers import CORE, MODULES, REPO_ROOT, SRC, TEST_DIR, read


CORE_FILES = ("modes.h", "servo_controller.h", "servo_controller.cpp",
              "sonar.h", "sonar.cpp", "sonar_filter.h",
              "buzzer.h", "buzzer.cpp", "gait.h", "gait.cpp",
              "gait_params.h", "autonomy.h", "autonomy.cpp", "validate.h")


class TestStructure(unittest.TestCase):
    def test_core_completo(self):
        for f in CORE_FILES:
            self.assertTrue(os.path.isfile(os.path.join(CORE, f)), f)

    def test_top_src(self):
        for f in ("config.h", "wifi_secrets.h", "main.cpp", "test_1sg90.cpp"):
            self.assertTrue(os.path.isfile(os.path.join(SRC, f)), f)
        self.assertTrue(os.path.isfile(
            os.path.join(MODULES, "net_wifi.h")))
        self.assertTrue(os.path.isfile(
            os.path.join(MODULES, "net_wifi.cpp")))

    def test_base_no_usa_ottodiylib(self):
        for root, _, files in os.walk(SRC):
            for f in files:
                if f.endswith((".h", ".cpp")):
                    body = read(os.path.join(root, f))
                    self.assertNotIn("#include <Otto", body, f)
                    # "sin OttoDIYLib" en comentarios explica que NO se usa.
                    body = body.replace("sin OttoDIYLib", "")
                    self.assertNotIn("OttoDIYLib", body, f)

    def test_platformio_envs(self):
        pio = read(os.path.join(REPO_ROOT, "firmware", "platformio.ini"))
        for env in ("[env:otto_esp32]", "[env:otto_esp32_wifi]",
                    "[env:test_1sg90]", "[env:test_core_esp32]"):
            self.assertIn(env, pio, env)
        self.assertIn("-DENABLE_WIFI_MODULE=0", pio)
        self.assertIn("-DENABLE_WIFI_MODULE=1", pio)
        self.assertIn("madhephaestus/ESP32Servo", pio)
        self.assertNotIn("OttoDIYLib", pio)
        self.assertIn("test_build_src = no", pio)
        self.assertIn("test_framework = unity", pio)

    def test_src_filter(self):
        pio = read(os.path.join(REPO_ROOT, "firmware", "platformio.ini"))
        self.assertIn("+<*> -<test_1sg90.cpp>", pio)
        self.assertIn("+<*> -<main.cpp> -<core/*> -<modules/*>", pio)

    def test_unity_runner_en_placa(self):
        self.assertTrue(os.path.isdir(TEST_DIR), TEST_DIR)
        cpps = [f for f in os.listdir(TEST_DIR) if f.endswith(".cpp")]
        self.assertTrue(len(cpps) >= 4, cpps)
        for f in cpps:
            body = read(os.path.join(TEST_DIR, f))
            self.assertIn("void test_", body, f)
            self.assertIn("unity.h", body, f)


if __name__ == "__main__":
    unittest.main()
