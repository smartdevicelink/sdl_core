#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/plugin_manager.h"

using namespace functional_modules;

class MockGenericModule : public GenericModule {
  public:
    MockGenericModule(ModuleID module_id): GenericModule(module_id) {}
    MOCK_CONST_METHOD0(GetPluginInfo, PluginInfo());
    MOCK_METHOD1(ChangeModuleState, void(ModuleState));
    MOCK_METHOD1(ProcessMessage, ProcessResult(const Json::Value& msg));
    MOCK_METHOD1(RemoveAppExtensions, void(void));
};

EXPORT_FUNCTION(MockGenericModule)

TEST(plugin_manager, loadModule) {
  MockGenericModule module(19);

}