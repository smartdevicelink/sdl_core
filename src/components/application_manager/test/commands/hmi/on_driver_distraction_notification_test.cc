/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "utils/shared_ptr.h"
#include "utils/lock.h"
#include "utils/make_shared.h"
#include "utils/data_accessor.h"
#include "commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "hmi/on_driver_distraction_notification.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace on_driver_distraction_notification {

using ::testing::_;
using ::testing::Return;
using ::testing::Eq;
using ::utils::SharedPtr;

namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::commands::hmi::OnDriverDistractionNotification;
using namespace am::commands;

typedef ::utils::SharedPtr<OnDriverDistractionNotification> NotificationPtr;

class HMIOnDriverDistractionNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  ::sync_primitives::Lock app_set_lock_;
  policy_test::MockPolicyHandlerInterface mock_policy_handler_interface_;
};

MATCHER_P2(CheckNotificationParams, function_id, state, "") {
  bool is_function_id_matched =
      function_id ==
      static_cast<am::mobile_api::FunctionID::eType>(
          (*arg)[am::strings::params][am::strings::function_id].asInt());
  bool is_state_matched =
      state ==
      static_cast<hmi_apis::Common_DriverDistractionState::eType>(
          (*arg)[am::strings::msg_params][am::mobile_notification::state]
              .asInt());
  return is_function_id_matched && is_state_matched;
}

ACTION_P(GetArg3, result) {
  arg3 = *result;
}

TEST_F(HMIOnDriverDistractionNotificationTest, Run_PushMobileMessage_SUCCESS) {
  const hmi_apis::Common_DriverDistractionState::eType state =
      hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;

  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(Eq(state)));

  MockAppPtr mock_app = CreateMockApp();
  am::ApplicationSet app_set;
  app_set.insert(mock_app);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));
  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcDisallowed;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_interface_));
  EXPECT_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _))
      .WillOnce(GetArg3(&result));

  EXPECT_CALL(*mock_app,
              PushMobileMessage(CheckNotificationParams(
                  am::mobile_api::FunctionID::OnDriverDistractionID, state)));

  command->Run();
}

TEST_F(HMIOnDriverDistractionNotificationTest,
       Run_SendNotificationToMobile_SUCCESS) {
  const hmi_apis::Common_DriverDistractionState::eType state =
      hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;

  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(Eq(state)));

  MockAppPtr mock_app = CreateMockApp();
  am::ApplicationSet app_set;
  app_set.insert(mock_app);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcAllowed;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_interface_));
  EXPECT_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _))
      .WillOnce(GetArg3(&result));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(
                  CheckNotificationParams(
                      am::mobile_api::FunctionID::OnDriverDistractionID, state),
                  Command::CommandOrigin::ORIGIN_SDL));

  command->Run();
}

}  // on_driver_distraction_notification
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
