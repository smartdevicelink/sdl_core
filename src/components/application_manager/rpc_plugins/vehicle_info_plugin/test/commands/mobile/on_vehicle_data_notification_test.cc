/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <strings.h>

#include "gtest/gtest.h"
#include "mobile/on_vehicle_data_notification.h"

#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/message_helper.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "utils/helpers.h"
#include "vehicle_info_plugin/commands/vi_commands_test.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "vehicle_info_plugin/vehicle_info_plugin.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_vehicle_data_notification {

namespace am = ::application_manager;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

using am::commands::MessageSharedPtr;
using vehicle_info_plugin::commands::OnVehicleDataNotification;

typedef std::shared_ptr<OnVehicleDataNotification> NotificationPtr;
typedef std::shared_ptr<vehicle_info_plugin::VehicleInfoAppExtension>
    VehicleInfoAppExtensionPtr;
typedef DataAccessor<application_manager::ApplicationSet> ApplicationSetDA;

namespace {
const uint32_t kAppId = 1u;
const utils::custom_string::CustomString kAppName("test_app");
}  // namespace

class OnVehicleDataNotificationTest
    : public VICommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnVehicleDataNotificationTest() : mock_app_(CreateMockApp()) {}

 protected:
  void SetUp() OVERRIDE {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(kAppName));

    ON_CALL(mock_message_helper_, PrintSmartObject(_))
        .WillByDefault(Return(false));
  }
  MockAppPtr mock_app_;
};

MATCHER_P(SmartObjectCheck, checker, "") {
  return checker(arg);
}

TEST_F(OnVehicleDataNotificationTest, OnVehicleDataNotification_SUCCESS) {
  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(
      am::strings::gps, mobile_apis::VehicleDataType::VEHICLEDATA_GPS));
  vehicle_data.insert(am::VehicleData::value_type(
      am::strings::speed, mobile_apis::VehicleDataType::VEHICLEDATA_SPEED));
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  application_manager::ApplicationSet apps;
  apps.insert(mock_app_);
  std::shared_ptr<sync_primitives::Lock> apps_lock =
      std::make_shared<sync_primitives::Lock>();
  ApplicationSetDA apps_da(apps, apps_lock);
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da));

  vehicle_info_plugin::VehicleInfoPlugin vi_plugin;
  VehicleInfoAppExtensionPtr vi_app_extention_ptr =
      std::make_shared<vehicle_info_plugin::VehicleInfoAppExtension>(
          vi_plugin, *mock_app_);
  vi_app_extention_ptr->subscribeToVehicleInfo("gps");
  vi_app_extention_ptr->subscribeToVehicleInfo("speed");
  ON_CALL(*mock_app_,
          QueryInterface(vehicle_info_plugin::VehicleInfoAppExtension::
                             VehicleInfoAppExtensionUID))
      .WillByDefault(Return(vi_app_extention_ptr));

  MessageSharedPtr message(CreateMessage(smart_objects::SmartType_Map));
  smart_objects::SmartObject gps_data;
  gps_data[am::strings::longitude_degrees] = 1.0;
  gps_data[am::strings::latitude_degrees] = 1.0;
  gps_data[am::strings::shifted] = true;

  (*message)[am::strings::msg_params][am::strings::gps] = gps_data;
  (*message)[am::strings::msg_params][am::strings::speed] = 0;

  NotificationPtr command(CreateCommandVI<OnVehicleDataNotification>(message));
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(message, _));

  command->Run();
}

}  // namespace on_vehicle_data_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
