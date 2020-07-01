/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "mobile/on_app_interface_unregistered_notification.h"

#include <utility>

#include "gtest/gtest.h"

#include "application_manager/commands/commands_test.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_app_interface_unregistered_notification_test {

namespace am = ::application_manager;

using sdl_rpc_plugin::commands::OnAppInterfaceUnregisteredNotification;
using Reason = mobile_apis::AppInterfaceUnregisteredReason::eType;
using ::testing::Return;

namespace {
const uint32_t kAppId = 1u;
typedef std::shared_ptr<MockApplication> MockAppPtr;
}  // namespace

const auto retrieve_data_to_check = [](const smart_objects::SmartObject& so) {
  return std::make_pair(
      so[am::strings::params][am::strings::connection_key].asUInt(),
      so[am::strings::msg_params][am::strings::reason].asInt());
};

MATCHER_P(CheckMessageData, initial_data, "") {
  return retrieve_data_to_check(*arg) == initial_data;
}

class OnAppInterfaceUnregisteredNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnAppInterfaceUnregisteredNotificationTest()
      : message_(CreateMessage()), command_(), mock_app_(CreateMockApp()) {
    (*message_)[am::strings::params][am::strings::connection_key] = kAppId;
    ON_CALL(app_mngr_, application(kAppId)).WillByDefault(Return(mock_app_));
    CommandsTest<CommandsTestMocks::kIsNice>::InitCommand(kDefaultTimeout_);
    command_.reset(
        new OnAppInterfaceUnregisteredNotification(message_,
                                                   app_mngr_,
                                                   mock_rpc_service_,
                                                   mock_hmi_capabilities_,
                                                   mock_policy_handler_));
  }

  void CheckResponseParameters(const bool is_wep_mode_enabled,
                               const bool is_final_message) {
    ON_CALL(*mock_app_, webengine_projection_enabled())
        .WillByDefault(Return(is_wep_mode_enabled));

    const auto initial_data = retrieve_data_to_check(*message_);
    EXPECT_CALL(
        mock_rpc_service_,
        SendMessageToMobile(CheckMessageData(initial_data), is_final_message));
  }

  am::commands::MessageSharedPtr message_;
  std::unique_ptr<OnAppInterfaceUnregisteredNotification> command_;
  MockAppPtr mock_app_;
};

TEST_F(OnAppInterfaceUnregisteredNotificationTest,
       Run_AnyReasonButResourceConstraint_NonFinalMessageSent) {
  (*message_)[am::strings::msg_params][am::strings::reason] =
      Reason::IGNITION_OFF;

  CheckResponseParameters(true, false);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(OnAppInterfaceUnregisteredNotificationTest,
       Run_ResourceConstraintForNonWebApp_NonFinalMessageSent) {
  (*message_)[am::strings::msg_params][am::strings::reason] =
      Reason::RESOURCE_CONSTRAINT;

  CheckResponseParameters(false, true);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(OnAppInterfaceUnregisteredNotificationTest,
       Run_ResourceConstraintForWebApp_FinalMessageSent) {
  (*message_)[am::strings::msg_params][am::strings::reason] =
      Reason::RESOURCE_CONSTRAINT;

  CheckResponseParameters(true, true);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

}  // namespace on_app_interface_unregistered_notification_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
