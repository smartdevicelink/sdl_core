#include "gtest/gtest.h"
#include "application_manager/mock_application.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "application_manager/application_manager.h"

namespace test {
namespace components {
namespace rc_rpc_plugin {

using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
using ::testing::NiceMock;
using ::rc_rpc_plugin::RCAppExtension;

const std::string climate = "CLIMATE";
const std::string radio = "RADIO";

class RCAppExtensionTest : public ::testing::Test {
 public:
  RCAppExtensionTest() : rc_ext(1) {}

 protected:
  virtual void SetUp() OVERRIDE {}
  RCAppExtension rc_ext;
};

TEST_F(RCAppExtensionTest, IsSubscribed_SUCCESS) {
  rc_ext.SubscribeToInteriorVehicleData(climate);
  bool ret = rc_ext.IsSubscibedToInteriorVehicleData(climate);
  EXPECT_TRUE(ret);
  ret = rc_ext.IsSubscibedToInteriorVehicleData(radio);
  EXPECT_FALSE(ret);
}

TEST_F(RCAppExtensionTest, UnsubscribeFromAllData_SUCCESS) {
  rc_ext.SubscribeToInteriorVehicleData(climate);
  rc_ext.SubscribeToInteriorVehicleData(radio);
  rc_ext.UnsubscribeFromInteriorVehicleData();
  bool ret = rc_ext.IsSubscibedToInteriorVehicleData(climate);
  EXPECT_FALSE(ret);
  ret = rc_ext.IsSubscibedToInteriorVehicleData(radio);
  EXPECT_FALSE(ret);
}

TEST_F(RCAppExtensionTest, UnsubscribeFromClimate_SUCCESS) {
  rc_ext.SubscribeToInteriorVehicleData(climate);
  rc_ext.SubscribeToInteriorVehicleData(radio);
  rc_ext.UnsubscribeFromInteriorVehicleData(climate);
  bool ret = rc_ext.IsSubscibedToInteriorVehicleData(climate);
  EXPECT_FALSE(ret);
  ret = rc_ext.IsSubscibedToInteriorVehicleData(radio);
  EXPECT_TRUE(ret);
}
}
}
}