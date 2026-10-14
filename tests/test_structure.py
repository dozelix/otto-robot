"""Estructura: core/ por directorios, envs PlatformIO y runner Unity."""
import os
import re
import unittest

from helpers import (AUTONOMY_CPP, AUTONOMY_H, CORE, GAIT_CPP, GAIT_H,
                     GAIT_PARAMS_H, MODES_H, MODULES, NET_WIFI_CPP,
                     NET_WIFI_H, PIO_INI, SONAR_CPP, SONAR_FILTER_H,
                     SONAR_H, SRC, TEST_DIR, TOOLS, VALIDATE_H, read)


# Módulo -> archivos esperados dentro de firmware/src/core/<módulo>/.
CORE_TREE = {
    "modes": ["modes.h"],
    "validate": ["validate.h"],
    "sonar": ["sonar.h", "sonar.cpp", "sonar_filter.h"],
    "motion": ["servo_controller.h", "servo_controller.cpp",
               "gait.h", "gait.cpp", "gait_params.h"],
    "autonomy": ["autonomy.h", "autonomy.cpp"],
    "net": ["net_wifi.h", "net_wifi.cpp"],
    "audio": ["sound_hook.h", "sound_hook.cpp"],
}


class TestStructure(unittest.TestCase):
    def test_core_por_directorios(self):
        for mod, files in CORE_TREE.items():
            for f in files:
                self.assertTrue(
                    os.path.isfile(os.path.join(CORE, mod, f)),
                    os.path.join(mod, f))
        # Sin archivos sueltos en core/: todo vive en su subdirectorio.
        sueltos = [f for f in os.listdir(CORE)
                   if os.path.isfile(os.path.join(CORE, f))]
        self.assertEqual(sueltos, [], sueltos)

    def test_top_src(self):
        for f in ("config.h", "wifi_secrets.h", "main.cpp"):
            self.assertTrue(os.path.isfile(os.path.join(SRC, f)), f)
        for f in ("buzzer.h", "buzzer.cpp"):
            self.assertTrue(os.path.isfile(os.path.join(MODULES, f)), f)
        for f in ("manual_1sg90.cpp", "calibracion_90.cpp"):
            self.assertTrue(os.path.isfile(os.path.join(TOOLS, f)), f)

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
        pio = read(PIO_INI)
        for env in ("[env:otto_esp32]", "[env:otto_esp32_wifi]",
                    "[env:manual_1sg90]", "[env:calibracion_90]",
                    "[env:test_core_esp32]", "[env:native]"):
            self.assertIn(env, pio, env)
        self.assertIn("src_dir = firmware/src", pio)
        self.assertIn("test_dir = firmware/test", pio)
        self.assertIn("madhephaestus/ESP32Servo", pio)
        self.assertIn("throwtheswitch/Unity", pio)
        self.assertNotIn("OttoDIYLib", pio)
        self.assertIn("test_build_src = no", pio)
        self.assertIn("test_framework = unity", pio)

    def test_build_src_filter_vigente(self):
        pio = read(PIO_INI)
        self.assertIn("build_src_filter", pio)
        # Sin la clave deprecada: línea que empiece en src_filter (no build_).
        viejas = re.findall(r"(?m)^\s*src_filter\s*=", pio)
        self.assertEqual(viejas, [], viejas)

    def test_build_src_filter_patrones(self):
        pio = read(PIO_INI)
        self.assertIn("+<*> -<tools/>", pio)
        self.assertIn("+<*> -<main.cpp> -<core/> -<modules/>"
                      " -<tools/calibracion_90.cpp>", pio)
        self.assertIn("+<*> -<main.cpp> -<core/> -<modules/>"
                      " -<tools/manual_1sg90.cpp>", pio)

    def test_unity_runner(self):
        self.assertTrue(os.path.isdir(TEST_DIR), TEST_DIR)
        cpps = sorted(f for f in os.listdir(TEST_DIR) if f.endswith(".cpp"))
        self.assertTrue(len(cpps) >= 5, cpps)
        self.assertIn("test_main.cpp", cpps)
        runner = read(os.path.join(TEST_DIR, "test_main.cpp"))
        self.assertIn("RUN_TEST", runner)
        self.assertIn("unity.h", runner)
        for f in cpps:
            if f == "test_main.cpp":
                continue
            body = read(os.path.join(TEST_DIR, f))
            self.assertIn("void test_", body, f)
            self.assertIn("unity.h", body, f)

    def test_rutas_canonicas_existen(self):
        for p in (MODES_H, VALIDATE_H, SONAR_H, SONAR_CPP,
                  SONAR_FILTER_H, GAIT_PARAMS_H, GAIT_H, GAIT_CPP,
                  AUTONOMY_H, AUTONOMY_CPP, NET_WIFI_H, NET_WIFI_CPP):
            self.assertTrue(os.path.isfile(p), p)


if __name__ == "__main__":
    unittest.main()
