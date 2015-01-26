#include "gtest/gtest.h"
#include "can_cooperation/can_app_extension.h"

using can_cooperation::CANAppExtension;

namespace test {
namespace components {
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

}  // namespace can_cooperation
}  // namespace components
}  // namespace test
