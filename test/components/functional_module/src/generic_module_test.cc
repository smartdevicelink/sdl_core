#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/generic_module.h"
#include "functional_module/module_observer.h"

using namespace functional_modules;

class MockGenericModule : public GenericModule {
  public:
    MockGenericModule(ModuleID module_id): GenericModule(module_id) {}
    MOCK_METHOD1(ProcessMessage, ProcessResult(const Json::Value& msg));
};

class ErrorGenericModule : public GenericModule {
  public:
    ErrorGenericModule(ModuleID module_id): GenericModule(module_id) {}
    ProcessResult ProcessMessage(const Json::Value& msg) {
      NotifyObservers(ModuleObserver::FS_FAILURE);
      return ProcessResult::FAILED;
    }
};

class MockModuleObserver : public ModuleObserver {
  public:
    MOCK_METHOD1(OnError, void(ModuleObserver::Errors));
};

TEST(generic_module, create) {
  MockGenericModule module(17);
  ASSERT_TRUE(module.GetModuleID() == 17);
}

TEST(generic_module, notify_observer) {
  ErrorGenericModule module(3);
  utils::SharedPtr<MockModuleObserver> p_observer1 = new MockModuleObserver;
  module.AddObserver(p_observer1);
  Json::Value val;
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE))
  .Times(1)
  ;

  ProcessResult res = module.ProcessMessage(val);
  EXPECT_TRUE(res == ProcessResult::FAILED);
  module.RemoveObserver(p_observer1);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE))
  .Times(0);
  module.ProcessMessage(val);
  utils::SharedPtr<MockModuleObserver> p_observer2 = new MockModuleObserver;
  module.AddObserver(p_observer1);
  module.AddObserver(p_observer2);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE)).Times(1);
  EXPECT_CALL(*p_observer2, OnError(MockModuleObserver::FS_FAILURE)).Times(1);
  module.ProcessMessage(val);
  for (size_t i = 0; i < 2; ++i) {
    module.RemoveObserver(p_observer2);
    EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE)).Times(1);
    EXPECT_CALL(*p_observer2, OnError(MockModuleObserver::FS_FAILURE)).Times(0);
    module.ProcessMessage(val);
  }
  module.RemoveObserver(p_observer1);
  EXPECT_CALL(*p_observer1, OnError(ModuleObserver::FS_FAILURE))
  .Times(0);
  module.ProcessMessage(val);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}