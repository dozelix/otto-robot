// Unity: reglas de core/validate.h (pio test -e test_core_esp32 / -e native).
#include <unity.h>
#include "../../src/core/validate/validate.h"

void test_steps_bounds(void) {
  TEST_ASSERT_FALSE(isValidSteps(0, 4));
  TEST_ASSERT_TRUE(isValidSteps(1, 4));
  TEST_ASSERT_TRUE(isValidSteps(4, 4));
  TEST_ASSERT_FALSE(isValidSteps(5, 4));
}

void test_period_bounds(void) {
  TEST_ASSERT_FALSE(isValidPeriodMs(499, 500, 2000));
  TEST_ASSERT_TRUE(isValidPeriodMs(500, 500, 2000));
  TEST_ASSERT_TRUE(isValidPeriodMs(2000, 500, 2000));
  TEST_ASSERT_FALSE(isValidPeriodMs(2001, 500, 2000));
}

void test_dir_and_beep(void) {
  TEST_ASSERT_TRUE(isValidDir(1));
  TEST_ASSERT_TRUE(isValidDir(-1));
  TEST_ASSERT_FALSE(isValidDir(0));
  TEST_ASSERT_TRUE(isValidBeepFreq(100));
  TEST_ASSERT_TRUE(isValidBeepFreq(5000));
  TEST_ASSERT_FALSE(isValidBeepFreq(99));
  TEST_ASSERT_FALSE(isValidBeepFreq(5001));
  TEST_ASSERT_TRUE(isValidBeepMs(20));
  TEST_ASSERT_TRUE(isValidBeepMs(1000));
  TEST_ASSERT_FALSE(isValidBeepMs(19));
  TEST_ASSERT_FALSE(isValidBeepMs(1001));
  TEST_ASSERT_TRUE(isValidManual(2, 1000, 1, 4, 500, 2000));
  TEST_ASSERT_FALSE(isValidManual(9, 1000, 1, 4, 500, 2000));
  TEST_ASSERT_FALSE(isValidManual(2, 100, 1, 4, 500, 2000));
  TEST_ASSERT_FALSE(isValidManual(2, 1000, 0, 4, 500, 2000));
}

void test_state_decisions(void) {
  TEST_ASSERT_FALSE(shouldAvoid(-1.0f, 20.0f));
  TEST_ASSERT_TRUE(shouldAvoid(19.9f, 20.0f));
  TEST_ASSERT_FALSE(shouldAvoid(20.0f, 20.0f));
  TEST_ASSERT_FALSE(shouldSafeHome(4, 5));
  TEST_ASSERT_TRUE(shouldSafeHome(5, 5));
  TEST_ASSERT_FALSE(shouldWatchdogRevert(30000UL, 0UL, 30000UL));
  TEST_ASSERT_TRUE(shouldWatchdogRevert(30001UL, 0UL, 30000UL));
}

void test_watchdog_overflow(void) {
  unsigned long last = 0xFFFFFF00UL;
  TEST_ASSERT_FALSE(shouldWatchdogRevert(0x00000100UL, last, 30000UL));
  TEST_ASSERT_TRUE(shouldWatchdogRevert(last + 40000UL, last, 30000UL));
}

void test_clamp(void) {
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, clampAngle(-10.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, clampAngle(90.0f));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, clampAngle(200.0f));
}
