#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "can_cooperation/can_module.h"
#include "functional_module/module_observer.h"

using namespace functional_modules;
using namespace can_cooperation;

TEST(can_module, create) {
  CANModule module;
  ASSERT_TRUE(module.GetModuleID() == 153);
}

TEST(can_module, notify_observer) {
  CANModule module;
  Json::Value val;
  ProcessResult res = module.ProcessMessage(val);
  EXPECT_TRUE(res == ProcessResult::CANNOT_PROCESS);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}