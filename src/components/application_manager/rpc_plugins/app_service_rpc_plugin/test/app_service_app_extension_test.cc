#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "app_service_rpc_plugin/app_service_rpc_plugin.h"
#include "application_manager/application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_rpc_plugin.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace app_service_rpc_plugin {

using ::app_service_rpc_plugin::AppServiceAppExtension;
using ::app_service_rpc_plugin::AppServiceRpcPlugin;
using ::application_manager::plugin_manager::MockRPCPlugin;
using test::components::application_manager_test::MockApplication;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

const std::string media = "MEDIA";
const std::string nav = "NAVIGATION";
const std::string future = "FUTURESERVICE";

class AppServiceAppExtensionTest : public ::testing::Test {
 public:
  AppServiceAppExtensionTest() : as_ext(as_plugin_, mock_app_) {}

 protected:
  virtual void SetUp() OVERRIDE {}
  AppServiceRpcPlugin as_plugin_;
  MockApplication mock_app_;
  AppServiceAppExtension as_ext;
};

TEST_F(AppServiceAppExtensionTest, Subscribe_SUCCESS) {
  bool ret = as_ext.SubscribeToAppService(media);
  EXPECT_TRUE(ret);
  ret = as_ext.SubscribeToAppService(future);
  EXPECT_TRUE(ret);
}

TEST_F(AppServiceAppExtensionTest, DoubleSubscribe_FAILURE) {
  bool ret = as_ext.SubscribeToAppService(media);
  EXPECT_TRUE(ret);
  ret = as_ext.SubscribeToAppService(media);
  EXPECT_FALSE(ret);
}

TEST_F(AppServiceAppExtensionTest, IsSubscribed_SUCCESS) {
  bool ret = as_ext.SubscribeToAppService(media);
  EXPECT_TRUE(ret);
  ret = as_ext.IsSubscribedToAppService(media);
  EXPECT_TRUE(ret);
  ret = as_ext.IsSubscribedToAppService(nav);
  EXPECT_FALSE(ret);
}

TEST_F(AppServiceAppExtensionTest, UnsubscribeFromAllAppService_SUCCESS) {
  bool ret = as_ext.SubscribeToAppService(media);
  EXPECT_TRUE(ret);
  ret = as_ext.SubscribeToAppService(nav);
  EXPECT_TRUE(ret);
  ret = as_ext.SubscribeToAppService(future);
  EXPECT_TRUE(ret);
  as_ext.UnsubscribeFromAppService();
  ret = as_ext.IsSubscribedToAppService(media);
  EXPECT_FALSE(ret);
  ret = as_ext.IsSubscribedToAppService(nav);
  EXPECT_FALSE(ret);
  ret = as_ext.IsSubscribedToAppService(future);
  EXPECT_FALSE(ret);
}

TEST_F(AppServiceAppExtensionTest, UnsubscribeFromMediaAppService_SUCCESS) {
  bool ret = as_ext.SubscribeToAppService(media);
  EXPECT_TRUE(ret);
  ret = as_ext.SubscribeToAppService(nav);
  EXPECT_TRUE(ret);
  ret = as_ext.SubscribeToAppService(future);
  EXPECT_TRUE(ret);
  as_ext.UnsubscribeFromAppService(media);
  ret = as_ext.IsSubscribedToAppService(media);
  EXPECT_FALSE(ret);
  ret = as_ext.IsSubscribedToAppService(nav);
  EXPECT_TRUE(ret);
  ret = as_ext.IsSubscribedToAppService(future);
  EXPECT_TRUE(ret);
}
}  // namespace app_service_rpc_plugin
}  // namespace components
}  // namespace test