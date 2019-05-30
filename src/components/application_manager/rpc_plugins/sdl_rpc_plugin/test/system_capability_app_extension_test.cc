#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "application_manager/application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_rpc_plugin.h"
#include "gtest/gtest.h"
#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

namespace test {
namespace components {
namespace sdl_rpc_plugin {

using ::application_manager::plugin_manager::MockRPCPlugin;
using ::sdl_rpc_plugin::SDLRPCPlugin;
using ::sdl_rpc_plugin::SystemCapabilityAppExtension;
using test::components::application_manager_test::MockApplication;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

typedef mobile_apis::SystemCapabilityType::eType SystemCapabilityType;

const SystemCapabilityType app_services = SystemCapabilityType::APP_SERVICES;
const SystemCapabilityType nav = SystemCapabilityType::NAVIGATION;

class SystemCapabilityAppExtensionTest : public ::testing::Test {
 public:
  SystemCapabilityAppExtensionTest() : sys_cap_ext(as_plugin_, mock_app_) {}

 protected:
  virtual void SetUp() OVERRIDE {}
  SDLRPCPlugin as_plugin_;
  MockApplication mock_app_;
  SystemCapabilityAppExtension sys_cap_ext;
};

TEST_F(SystemCapabilityAppExtensionTest, Subscribe_SUCCESS) {
  bool ret = sys_cap_ext.SubscribeTo(app_services);
  EXPECT_TRUE(ret);
}

TEST_F(SystemCapabilityAppExtensionTest, DoubleSubscribe_FAILURE) {
  bool ret = sys_cap_ext.SubscribeTo(app_services);
  EXPECT_TRUE(ret);
  ret = sys_cap_ext.SubscribeTo(app_services);
  EXPECT_FALSE(ret);
}

TEST_F(SystemCapabilityAppExtensionTest, IsSubscribed_SUCCESS) {
  bool ret = sys_cap_ext.SubscribeTo(app_services);
  EXPECT_TRUE(ret);
  ret = sys_cap_ext.IsSubscribedTo(app_services);
  EXPECT_TRUE(ret);
  ret = sys_cap_ext.IsSubscribedTo(nav);
  EXPECT_FALSE(ret);
}

TEST_F(SystemCapabilityAppExtensionTest, UnsubscribeFromAllCaps_SUCCESS) {
  bool ret = sys_cap_ext.SubscribeTo(app_services);
  EXPECT_TRUE(ret);
  ret = sys_cap_ext.SubscribeTo(nav);
  EXPECT_TRUE(ret);
  sys_cap_ext.UnsubscribeFromAll();
  ret = sys_cap_ext.IsSubscribedTo(app_services);
  EXPECT_FALSE(ret);
  ret = sys_cap_ext.IsSubscribedTo(nav);
  EXPECT_FALSE(ret);
}

TEST_F(SystemCapabilityAppExtensionTest, UnsubscribeFromAppService_SUCCESS) {
  bool ret = sys_cap_ext.SubscribeTo(app_services);
  EXPECT_TRUE(ret);
  ret = sys_cap_ext.SubscribeTo(nav);
  EXPECT_TRUE(ret);
  sys_cap_ext.UnsubscribeFrom(app_services);
  ret = sys_cap_ext.IsSubscribedTo(app_services);
  EXPECT_FALSE(ret);
  ret = sys_cap_ext.IsSubscribedTo(nav);
  EXPECT_TRUE(ret);
}
}  // namespace sdl_rpc_plugin
}  // namespace components
}  // namespace test