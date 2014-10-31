#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/plugin_manager.h"
#include <dlfcn.h>

using namespace functional_modules;

TEST(plugin_manager, loadModule) {
  void* generic_plugin_dll = dlopen("libPluginMock.so", RTLD_LAZY);
  ASSERT_TRUE(generic_plugin_dll != 0);
  typedef GenericModule* (*Create)();
  Create create_manager = reinterpret_cast<Create>(dlsym(generic_plugin_dll, "Create"));
  char* error_string = dlerror();
  ASSERT_TRUE(error_string == NULL);
  GenericModule* module = create_manager();
  ASSERT_TRUE(module);
  ASSERT_TRUE(module->GetModuleID() == 19);
  dlclose(generic_plugin_dll);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}