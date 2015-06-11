#include "gtest/gtest.h"
#include "can_cooperation/can_app_extension.h"

namespace can_cooperation {

TEST(CanAppExtensionTest, Create) {
  CANAppExtension extension(7);
  ASSERT_TRUE(extension.uid() == 7);
}

TEST(CanAppExtensionTest, Control) {
  CANAppExtension extension(3);
  ASSERT_EQ(3, extension.uid());
  ASSERT_FALSE(extension.IsControlGiven());
  extension.GiveControl(true);
  ASSERT_TRUE(extension.IsControlGiven());
}

TEST(CanAppExtensionTest, DriverDevice) {
  CANAppExtension extension(5);
  ASSERT_EQ(5, extension.uid());
  ASSERT_FALSE(extension.is_on_driver_device());
  extension.set_is_on_driver_device(true);
  ASSERT_TRUE(extension.is_on_driver_device());
}

}  // namespace can_cooperation
