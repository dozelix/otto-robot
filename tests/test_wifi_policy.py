"""Política WiFi único: core/net siempre presente, sin BT/BLE/AP."""
import re
import unittest

from helpers import CONFIG_H, NET_WIFI_CPP, NET_WIFI_H, WIFI_SECRETS_H, read


class TestWifiPolicy(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cpp = read(NET_WIFI_CPP)
        cls.h = read(NET_WIFI_H)
        cls.secrets = read(WIFI_SECRETS_H)
        cls.cfg = read(CONFIG_H)

    def test_net_vive_en_core(self):
        self.assertIn("core/net", NET_WIFI_CPP.replace("\\", "/"))
        self.assertIn("SIEMPRE presente", self.h)

    def test_sin_flag_condicional(self):
        self.assertNotIn("ENABLE_WIFI_MODULE", self.cpp)
        self.assertNotIn("ENABLE_WIFI_MODULE", self.cfg)

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

    def test_panel_mobile_first(self):
        # IoT controlado desde el teléfono: viewport completo, sin
        # zoom por doble-tap, targets táctiles >= 48 px, D-pad en grilla.
        for token in ("viewport-fit=cover", "touch-action:manipulation",
                      "min-height:54px", "grid-template-columns",
                      "max-width:520px"):
            self.assertIn(token, self.cpp, token)


if __name__ == "__main__":
    unittest.main()
