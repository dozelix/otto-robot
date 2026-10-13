"""Política WiFi único: sin BT/BLE/AP, una sola red, endpoints y 409."""
import os
import re
import unittest

from helpers import MODULES, SRC, read


class TestWifiPolicy(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cpp = read(os.path.join(MODULES, "net_wifi.cpp"))
        cls.h = read(os.path.join(MODULES, "net_wifi.h"))
        cls.secrets = read(os.path.join(SRC, "wifi_secrets.h"))
        cls.cfg = read(os.path.join(SRC, "config.h"))

    def test_bt_deshabilitado(self):
        self.assertIn("esp_bt_controller_mem_release", self.cpp)
        self.assertIn("btStop", self.cpp)

    def test_sin_ble_ni_ap(self):
        for token in ("BluetoothSerial", "BLEDevice", "BLEServer",
                      "softAP", "WIFI_AP"):
            self.assertNotIn(token, self.cpp, token)

    def test_modo_sta(self):
        self.assertIn("WIFI_STA", self.cpp)

    def test_una_sola_red(self):
        self.assertEqual(len(re.findall(r"#define\s+WIFI_SSID\b", self.secrets)), 1)
        self.assertEqual(len(re.findall(r"#define\s+WIFI_PASS\b", self.secrets)), 1)

    def test_endpoints(self):
        for ep in ('"/"', '"/status"', '"/mode"', '"/cmd"'):
            self.assertIn(ep, self.cpp, ep)
        for a in ("walk", "back", "turnL", "turnR", "home", "dance", "beep"):
            self.assertIn('"%s"' % a, self.cpp, a)

    def test_manual_gated_409(self):
        self.assertIn("send(409", self.cpp)
        self.assertIn("OttoMode::MANUAL", self.cpp)

    def test_base_sin_red_por_defecto(self):
        self.assertIn("ENABLE_WIFI_MODULE", self.cfg)
        self.assertIn("stubs sin dependencias de red", self.cpp)
        self.assertIn("desactivado", self.cpp)


if __name__ == "__main__":
    unittest.main()
