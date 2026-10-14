// Unity: modos de core/modes.h (pio test -e test_core_esp32 / -e native).
#include <unity.h>
#include "../../src/core/modes/modes.h"

void test_mode_values(void) {
  TEST_ASSERT_EQUAL_INT(0, (int)OttoMode::EXPOSITION);
  TEST_ASSERT_EQUAL_INT(1, (int)OttoMode::MANUAL);
}

void test_mode_names(void) {
  TEST_ASSERT_EQUAL_STRING("manual", modeName(OttoMode::MANUAL));
  TEST_ASSERT_EQUAL_STRING("exposition", modeName(OttoMode::EXPOSITION));
}

void test_mode_from_string(void) {
  TEST_ASSERT_TRUE(modeFromString("manual") == OttoMode::MANUAL);
  TEST_ASSERT_TRUE(modeFromString("m") == OttoMode::MANUAL);
  TEST_ASSERT_TRUE(modeFromString("M") == OttoMode::MANUAL);
  TEST_ASSERT_TRUE(modeFromString("1") == OttoMode::MANUAL);
  TEST_ASSERT_TRUE(modeFromString("exposition") == OttoMode::EXPOSITION);
  TEST_ASSERT_TRUE(modeFromString("x") == OttoMode::EXPOSITION);
  TEST_ASSERT_TRUE(modeFromString("") == OttoMode::EXPOSITION);
}
