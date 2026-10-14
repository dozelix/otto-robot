// Runner Unity para test_core (placa ESP32 y PC native).
// En Arduino usa setup()/loop(); en native usa main().
// Agrega aquí el RUN_TEST de cada void test_*() nuevo.

#include <unity.h>

// --- test_validate.cpp ---
void test_steps_bounds(void);
void test_period_bounds(void);
void test_dir_and_beep(void);
void test_state_decisions(void);
void test_watchdog_overflow(void);
void test_clamp(void);

// --- test_sonar_filter.cpp ---
void test_timeout_es_invalido(void);
void test_valor_central(void);
void test_bordes_de_rango(void);
void test_mediana_rechaza_outliers(void);

// --- test_modes.cpp ---
void test_mode_values(void);
void test_mode_names(void);
void test_mode_from_string(void);

// --- test_gait_params.cpp ---
void test_walk_fases(void);
void test_turn_fases(void);
void test_amplitudes_y_rango(void);

// --- test_config.cpp ---
void test_config_servos(void);
void test_config_sensores(void);
void test_config_comportamiento(void);
void test_config_sin_flag_wifi(void);

// --- test_net_wifi_core.cpp ---
void test_wifi_api_en_core(void);

void setUp(void) {}
void tearDown(void) {}

static void runAllTests(void) {
  RUN_TEST(test_steps_bounds);
  RUN_TEST(test_period_bounds);
  RUN_TEST(test_dir_and_beep);
  RUN_TEST(test_state_decisions);
  RUN_TEST(test_watchdog_overflow);
  RUN_TEST(test_clamp);
  RUN_TEST(test_timeout_es_invalido);
  RUN_TEST(test_valor_central);
  RUN_TEST(test_bordes_de_rango);
  RUN_TEST(test_mediana_rechaza_outliers);
  RUN_TEST(test_mode_values);
  RUN_TEST(test_mode_names);
  RUN_TEST(test_mode_from_string);
  RUN_TEST(test_walk_fases);
  RUN_TEST(test_turn_fases);
  RUN_TEST(test_amplitudes_y_rango);
  RUN_TEST(test_config_servos);
  RUN_TEST(test_config_sensores);
  RUN_TEST(test_config_comportamiento);
  RUN_TEST(test_config_sin_flag_wifi);
  RUN_TEST(test_wifi_api_en_core);
}

#ifdef ARDUINO
#include <Arduino.h>
void setup() {
  UNITY_BEGIN();
  runAllTests();
  UNITY_END();
}
void loop() {}
#else
int main(void) {
  UNITY_BEGIN();
  runAllTests();
  return UNITY_END();
}
#endif
