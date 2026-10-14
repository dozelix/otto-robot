// Unity: WiFi + dashboard viven en core/ y siempre presentes.
// Solo incluye el header (declaraciones, sin Arduino) para que corra
// igual en placa (test_core_esp32) y en PC (native).
#include <unity.h>
#include "../../src/core/net/net_wifi.h"

void test_wifi_api_en_core(void) {
  // Si compila, el header está en core/. Verificamos la API completa.
  TEST_ASSERT_TRUE((void*)wifiBegin != nullptr);
  TEST_ASSERT_TRUE((void*)wifiHandle != nullptr);
  TEST_ASSERT_TRUE((void*)wifiConnected != nullptr);
  TEST_ASSERT_TRUE((void*)wifiIP != nullptr);
}
