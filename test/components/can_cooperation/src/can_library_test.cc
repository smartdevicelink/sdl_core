#include <dlfcn.h>
#include "gtest/gtest.h"
#include "can_cooperation/can_module.h"

using can_cooperation::CANModule;
using functional_modules::PluginInfo;

namespace test {
namespace components {
namespace can_cooperation {

::testing::AssertionResult IsError(void* error) {
  if (error) {
    return ::testing::AssertionSuccess() << static_cast<const char*>(error);
  } else {
    return ::testing::AssertionFailure() << error;
  }
}

TEST(CanLibraryTest, Load) {
  const std::string kLibraryPath = "libCANCooperation.so";

  void* handle = dlopen(kLibraryPath.c_str(), RTLD_LAZY);
  EXPECT_FALSE(IsError(dlerror()));
  ASSERT_TRUE(handle != NULL);

  const std::string kSymbol = "Create";
  void* symbol = dlsym(handle, kSymbol.c_str());
  EXPECT_FALSE(IsError(dlerror()));
  ASSERT_TRUE(symbol != NULL);

  typedef CANModule* (*Create)();
  Create create_manager = reinterpret_cast<Create>(symbol);
  CANModule* module = create_manager();
  ASSERT_TRUE(module != NULL);

  PluginInfo plugin = module->GetPluginInfo();
  EXPECT_EQ(plugin.name, "ReverseSDLPlugin");
  EXPECT_EQ(plugin.version, 1);

  CANModule::destroy();
  int ret = dlclose(handle);
  EXPECT_FALSE(ret);
  EXPECT_FALSE(IsError(dlerror()));
}

}  // namespace can_cooperation
}  // namespace components
}  // namespace test
