#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "can_cooperation/can_app_extension.h"

using namespace can_cooperation;

TEST(can_app_extension, create) {
  CANAppExtenstion extension(7);
  ASSERT_TRUE(extension.uid() == 7);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}