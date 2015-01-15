#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <dlfcn.h>
#include "functional_module/generic_module.h"

using namespace functional_modules;

int LoadPluginLibrary(const std::string& path, void*& generic_plugin_dll, GenericModule*& module) {
  generic_plugin_dll = dlopen(path.c_str(), RTLD_LAZY);
  if (NULL == generic_plugin_dll) {
    char* failed_to_open = dlerror();
    printf("Failed to open dll %s with error %s \n", path.c_str(),
      failed_to_open);
    return -1;
  }
  typedef GenericModule* (*Create)();
  Create create_manager = reinterpret_cast<Create>(dlsym(generic_plugin_dll, "Create"));
  char* error_string = dlerror();
  if (NULL != error_string) {
    printf("Failed to export dll's %s symbols: %s \n", path.c_str(), error_string);
    dlclose(generic_plugin_dll);
    return -1;
  }
  module = create_manager();
  if (!module) {
    printf("Failed to create plugin main class %s \n", path.c_str());
    dlclose(generic_plugin_dll);
    return -1; 
  }
  return 0;
}

void UnloadPluginLibrary(void* dll) {
  dlclose(dll);
}

TEST(can_library_test, load) {
  const std::string library_path = "libCANCooperation.so";
  void* dll = NULL;
  GenericModule* module = NULL;
  ASSERT_EQ(0, LoadPluginLibrary(library_path, dll, module));
  ASSERT_FALSE(NULL == dll);
  ASSERT_TRUE(module);
  // destroy module
  UnloadPluginLibrary(dll);
}