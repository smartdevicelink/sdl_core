#include "mock_generic_module.h"

using functional_modules::GenericModule;
using functional_modules::MobileFunctionID;
using functional_modules::PluginInfo;

using ::testing::_;
using ::testing::Return;

MockGenericModule::MockGenericModule() : GenericModule(19) {
  PluginInfo info;
  info.name = "MockGenericModule";
  info.version = 1;
  info.mobile_function_list.push_back(static_cast<MobileFunctionID>(101));
  info.hmi_function_list.push_back("HMI-Func-1");

  EXPECT_CALL(*this, GetPluginInfo()).Times(2).WillRepeatedly(Return(info));
  EXPECT_CALL(*this, set_service(_)).Times(1);
}

extern "C" GenericModule* Create() {
  return new MockGenericModule();
}
