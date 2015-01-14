#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/plugin_manager.h"
#include "utils/file_system.h"
#include <dlfcn.h>

using namespace functional_modules;

TEST(plugin_manager, loadModule) {
  std::vector<std::string> plugin_files = file_system::ListFiles(
  "./plugins/");
  size_t plugins = 0;
  printf("Number of files: %d\n", plugin_files.size());
  for(size_t i = 0; i < plugin_files.size(); ++i) {
    size_t pos = plugin_files[i].find_last_of(".");
    if (std::string::npos != pos) {
      if (plugin_files[i].substr(pos+1).compare("so") != 0)
        continue; 
    } else {
      continue;
    }
    printf("Working with plugin %s\n", plugin_files[i].c_str());
    void* generic_plugin_dll = dlopen(plugin_files[i].c_str(), RTLD_LAZY);
    ASSERT_TRUE(generic_plugin_dll != 0);
    typedef GenericModule* (*Create)();
    Create create_manager = reinterpret_cast<Create>(dlsym(generic_plugin_dll, "Create"));
    char* error_string = dlerror();
    ASSERT_TRUE(error_string == NULL);
    GenericModule* module = create_manager();
    ASSERT_TRUE(module);
    ASSERT_TRUE(module->GetModuleID() == 19);
    dlclose(generic_plugin_dll);
    ++plugins;
  }
  PluginManager* manager = PluginManager::instance();
  ASSERT_EQ(plugins, manager->LoadPlugins("./plugins/"));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}