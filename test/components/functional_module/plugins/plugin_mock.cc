#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/generic_module.h"

using namespace functional_modules;

class MockGenericModule : public GenericModule,
  public utils::Singleton<MockGenericModule> {
  public:
    MockGenericModule(ModuleID module_id = 19): GenericModule(module_id) {}
    PluginInfo GetPluginInfo() const {
      PluginInfo info;
      info.name = "MockGenericModule";
      info.version = 1;
      info.mobile_function_list.push_back(
        static_cast<MobileFunctionID>(101));
      return info;
    }
    MOCK_METHOD1(ProcessMessage, ProcessResult(application_manager::MessagePtr));
    MOCK_METHOD0(RemoveAppExtensions, void(void));
    MOCK_METHOD1(RemoveAppExtension, void(uint32_t));
  private:
    FRIEND_BASE_SINGLETON_CLASS(MockGenericModule);
};

EXPORT_FUNCTION(MockGenericModule);