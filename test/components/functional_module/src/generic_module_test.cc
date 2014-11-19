#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/generic_module.h"
#include "functional_module/module_observer.h"

using namespace functional_modules;

class MockGenericModule : public GenericModule {
  public:
    MockGenericModule(ModuleID module_id): GenericModule(module_id) {}
    ProcessResult ProcessMessage(application_manager::MessagePtr msg) {
      if (msg->function_id() == 100) return PROCESSED;
      else return CANNOT_PROCESS;
    }
    PluginInfo GetPluginInfo() const {
      PluginInfo info;
      info.name = "MockGenericModule";
      info.version = 1;
      info.mobile_function_list.push_back(
        static_cast<MobileFunctionID>(100));
      return info;
    }
    MOCK_METHOD1(ChangeModuleState, void(ModuleState));
    MOCK_METHOD0(RemoveAppExtensions, void(void));
    MOCK_METHOD1(RemoveAppExtension, void(uint32_t));
};

TEST(generic_module, create) {
  MockGenericModule module(17);
  ASSERT_TRUE(module.GetModuleID() == 17);
}

TEST(generic_module, getPluginInfo) {
  MockGenericModule module(18);
  PluginInfo info = module.GetPluginInfo();
  EXPECT_EQ("MockGenericModule", info.name);
  EXPECT_EQ(1, info.version);
  EXPECT_EQ(1, info.mobile_function_list.size());

  application_manager::MessagePtr message = 
  new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
  message->set_function_id(100);
  ASSERT_TRUE(PROCESSED == module.ProcessMessage(message));
  message->set_function_id(101);
  ASSERT_TRUE(CANNOT_PROCESS == module.ProcessMessage(message));
}

/////////////////////////////////////////////////

class ErrorGenericModule : public GenericModule {
  public:
    ErrorGenericModule(ModuleID module_id): GenericModule(module_id) {}
    ProcessResult ProcessMessage(application_manager::MessagePtr msg) {
      NotifyObservers(ModuleObserver::FS_FAILURE);
      return ProcessResult::FAILED;
    }
    MOCK_CONST_METHOD0(GetPluginInfo, PluginInfo());
    MOCK_METHOD1(ChangeModuleState, void(ModuleState));
    MOCK_METHOD0(RemoveAppExtensions, void());
    MOCK_METHOD1(RemoveAppExtension, void(uint32_t));
};

class MockModuleObserver : public ModuleObserver {
  public:
    MOCK_METHOD2(OnError, void(ModuleObserver::Errors, ModuleID));
};

TEST(generic_module, notify_observer) {
  ErrorGenericModule module(3);
  utils::SharedPtr<MockModuleObserver> p_observer1 = new MockModuleObserver;
  module.AddObserver(p_observer1);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE, 3))
  .Times(1)
  ;
  application_manager::MessagePtr message = 
  new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
  message->set_function_id(-1);

  ProcessResult res = module.ProcessMessage(message);
  EXPECT_TRUE(res == ProcessResult::FAILED);
  module.RemoveObserver(p_observer1);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE, 3))
  .Times(0);
  module.ProcessMessage(message);
  utils::SharedPtr<MockModuleObserver> p_observer2 = new MockModuleObserver;
  module.AddObserver(p_observer1);
  module.AddObserver(p_observer2);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);
  EXPECT_CALL(*p_observer2, OnError(MockModuleObserver::FS_FAILURE, 3)).Times(1);
  module.ProcessMessage(message);
  for (size_t i = 0; i < 2; ++i) {
    module.RemoveObserver(p_observer2);
    EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);
    EXPECT_CALL(*p_observer2, OnError(MockModuleObserver::FS_FAILURE, 3)).Times(0);
    module.ProcessMessage(message);
  }
  module.RemoveObserver(p_observer1);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE, 3))
  .Times(0);
  module.ProcessMessage(message);

}

/////////////////////////////////////////////////

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}