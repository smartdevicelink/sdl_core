#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "can_cooperation/can_module.h"
#include "functional_module/module_observer.h"

using namespace functional_modules;
using namespace can_cooperation;

TEST(can_module, create) {
  CANModule* module = CANModule::instance();
  ASSERT_TRUE(module->GetModuleID() == 153);
}

TEST(can_module, notify_observer) {
  CANModule& module = *CANModule::instance();
  application_manager::MessagePtr message = 
  new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
  /*message->set_function_id(MobileFunctionID::TUNE_RADIO);
  ProcessResult res = module.ProcessMessage(message);
  EXPECT_TRUE(res == ProcessResult::PROCESSED);*/
  message->set_function_id(-1);
  ProcessResult res = module.ProcessMessage(message);
  EXPECT_TRUE(res == ProcessResult::CANNOT_PROCESS);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}