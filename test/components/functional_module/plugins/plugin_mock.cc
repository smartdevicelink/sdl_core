#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/generic_module.h"

using namespace functional_modules;

class MockGenericModule : public GenericModule,
  public utils::Singleton<MockGenericModule> {
  public:
    MockGenericModule(ModuleID module_id = 19): GenericModule(module_id) {}
    MOCK_CONST_METHOD0(GetPluginInfo, PluginInfo());
    MOCK_METHOD1(ProcessMessage, ProcessResult(application_manager::MessagePtr));
    MOCK_METHOD0(RemoveAppExtensions, void(void));
  private:
    FRIEND_BASE_SINGLETON_CLASS(MockGenericModule);
};

EXPORT_FUNCTION(MockGenericModule)