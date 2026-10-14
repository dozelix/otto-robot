// Unity: filtro de core/sonar_filter.h (pio test -e test_core_esp32 / -e native).
#include <unity.h>
#include "../../src/core/sonar/sonar_filter.h"

void test_timeout_es_invalido(void) {
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, sonarCmFromUs(0));
  long all[] = {0, 0, 0, 0, 0};
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, sonarFilterCm(all));
}

void test_valor_central(void) {
  long s[] = {600, 600, 600, 600, 600};
  TEST_ASSERT_FLOAT_WITHIN(0.06f, 10.3f, sonarFilterCm(s));
}

void test_bordes_de_rango(void) {
  long lo[] = {116, 116, 116, 116, 116};
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, sonarFilterCm(lo));
  long ok[] = {118, 118, 118, 118, 118};
  TEST_ASSERT_FLOAT_WITHIN(0.06f, 2.0f, sonarFilterCm(ok));
  long hi[] = {23280, 23280, 23280, 23280, 23280};
  float r = sonarFilterCm(hi);
  TEST_ASSERT_TRUE(r >= 399.0f && r <= 400.0f);
  long over[] = {23400, 23400, 23400, 23400, 23400};
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, sonarFilterCm(over));
}

void test_mediana_rechaza_outliers(void) {
  long a[] = {600, 600, 600, 600, 23400};
  TEST_ASSERT_FLOAT_WITHIN(0.06f, 10.3f, sonarFilterCm(a));
  long b[] = {0, 600, 600, 600, 600};
  TEST_ASSERT_FLOAT_WITHIN(0.06f, 10.3f, sonarFilterCm(b));
}
