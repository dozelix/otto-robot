// Unity: contrato de src/config.h (pines y umbrales del robot).
// El robot base solo usa servos + proximidad; el buzzer es opcional
// (sigue definido PIN_BUZZER para modules/buzzer).
#include <unity.h>
#include "../../src/config.h"

void test_config_servos(void) {
  TEST_ASSERT_EQUAL_INT(32, SERVO_CI);
  TEST_ASSERT_EQUAL_INT(33, SERVO_CD);
  TEST_ASSERT_EQUAL_INT(25, SERVO_TI);
  TEST_ASSERT_EQUAL_INT(26, SERVO_TD);
  // Pines únicos entre sí.
  TEST_ASSERT_TRUE(SERVO_CI != SERVO_CD && SERVO_CI != SERVO_TI &&
                   SERVO_CI != SERVO_TD && SERVO_CD != SERVO_TI &&
                   SERVO_CD != SERVO_TD && SERVO_TI != SERVO_TD);
}

void test_config_sensores(void) {
  TEST_ASSERT_EQUAL_INT(18, PIN_TRIG);
  TEST_ASSERT_EQUAL_INT(19, PIN_ECHO);
  TEST_ASSERT_EQUAL_INT(27, PIN_BUZZER);
  TEST_ASSERT_TRUE(PIN_TRIG != PIN_ECHO);
}

void test_config_comportamiento(void) {
  TEST_ASSERT_EQUAL_INT(20, UMBRAL_OBSTACULO_CM);
  TEST_ASSERT_EQUAL_INT(1000, TIEMPO_PASO_MS);
  TEST_ASSERT_EQUAL_INT(200, PERIODO_LECTURA_MS);
  TEST_ASSERT_EQUAL_INT(5, FILTRO_MUESTRAS);
  TEST_ASSERT_EQUAL_INT(5, MAX_LECTURAS_INVALIDAS);
  TEST_ASSERT_EQUAL_INT(4, MANUAL_MAX_STEPS);
  TEST_ASSERT_EQUAL_INT(500, MANUAL_MIN_T_MS);
  TEST_ASSERT_EQUAL_INT(2000, MANUAL_MAX_T_MS);
}

// Política: WiFi siempre en core → prohibido el flag viejo.
void test_config_sin_flag_wifi(void) {
#ifdef ENABLE_WIFI_MODULE
  TEST_FAIL_MESSAGE("ENABLE_WIFI_MODULE no debe existir: WiFi siempre on");
#else
  TEST_PASS();
#endif
}
