// Unity: parámetros de core/gait_params.h.
#include <unity.h>
#include "../../src/core/gait_params.h"

void test_walk_fases(void) {
  GaitParams f = walkParams(1);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, f.phase_deg[0]);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, f.phase_deg[1]);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -90.0f, f.phase_deg[2]);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -90.0f, f.phase_deg[3]);
  GaitParams b = walkParams(-1);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, b.phase_deg[2]);
}

void test_turn_fases(void) {
  GaitParams r = turnParams(1);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, r.phase_deg[0]);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, r.phase_deg[1]);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, r.phase_deg[2]);
  GaitParams l = turnParams(-1);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, -90.0f, l.phase_deg[2]);
}

void test_amplitudes_y_rango(void) {
  GaitParams gs[4] = {walkParams(1), walkParams(-1), turnParams(1), turnParams(-1)};
  for (int k = 0; k < 4; k++) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, gs[k].a[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 20.0f, gs[k].a[2]);
    for (int i = 0; i < 4; i++) {
      float lo = 90.0f + gs[k].o[i] - gs[k].a[i];
      float hi = 90.0f + gs[k].o[i] + gs[k].a[i];
      TEST_ASSERT_TRUE(lo >= 0.0f && hi <= 180.0f);
    }
  }
}
