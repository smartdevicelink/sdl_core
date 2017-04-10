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
#include <set>

#include "application_manager/commands/mobile/perform_interaction_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace perform_interaction_request {

namespace am = application_manager;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::testing::_;
using ::testing::Mock;
using ::utils::SharedPtr;
using ::testing::Return;
using ::testing::ReturnRef;
using am::commands::PerformInteractionRequest;
using ::test::components::application_manager_test::MockApplication;

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

namespace {
const int32_t kCommandId = 1;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
}  // namespace

class PerformInteractionRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformInteractionRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {}

  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              true);
    EXPECT_EQ(
        (*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
        static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  sync_primitives::Lock lock_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(PerformInteractionRequestTest, OnTimeout_VR_GENERIC_ERROR) {
  MessageSharedPtr response_msg_vr =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_vr)[strings::params][hmi_response::code] =
      static_cast<uint64_t>(hmi_apis::Common_Result::SUCCESS);
  (*response_msg_vr)[strings::msg_params][strings::info] = "info";
  MessageSharedPtr request_msg = CreateMessage(smart_objects::SmartType_Map);
  (*request_msg)[strings::msg_params][strings::interaction_mode] =
      mobile_apis::InteractionMode::BOTH;
  utils::SharedPtr<PerformInteractionRequest> command =
      CreateCommand<PerformInteractionRequest>(request_msg);
  MockAppPtr mock_app;

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  event.set_smart_object(*response_msg_vr);

  command->Init();
  command->on_event(event);
  MessageSharedPtr response_to_mobile =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_to_mobile)[strings::msg_params][strings::result_code] =
      static_cast<uint64_t>(am::mobile_api::Result::GENERIC_ERROR);
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(response_to_mobile));

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));
  command->onTimeOut();

  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::success].asBool(),
      false);
  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::result_code].asInt(),
      static_cast<int32_t>(am::mobile_api::Result::GENERIC_ERROR));
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_from_mobile =
      CreateMessage(smart_objects::SmartType_Map);
  (*msg_from_mobile)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg_from_mobile)[strings::msg_params][strings::interaction_mode] =
      mobile_apis::InteractionMode::VR_ONLY;
  utils::SharedPtr<PerformInteractionRequest> command =
      CreateCommand<PerformInteractionRequest>(msg_from_mobile);
  command->Init();

  MockAppPtr mock_app;
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app));

  MockHmiInterfaces hmi_interfaces;
  EXPECT_CALL(app_mngr_, hmi_interfaces())
      .WillRepeatedly(ReturnRef(hmi_interfaces));

  MessageSharedPtr response_msg_vr =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_vr)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*response_msg_vr)[strings::msg_params][strings::cmd_id] = kCommandId;
  (*response_msg_vr)[am::strings::msg_params][am::strings::info] =
      "VR is not supported by system";

  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_ui)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  EXPECT_CALL(hmi_interfaces,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(hmi_interfaces,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_to_mobile;

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  ResultCommandExpectations(response_to_mobile,
                            "VR is not supported by system");
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_UIHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_from_mobile =
      CreateMessage(smart_objects::SmartType_Map);
  (*msg_from_mobile)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg_from_mobile)[strings::msg_params][strings::interaction_mode] =
      mobile_apis::InteractionMode::VR_ONLY;
  utils::SharedPtr<PerformInteractionRequest> command =
      CreateCommand<PerformInteractionRequest>(msg_from_mobile);

  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_msg_vr =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_vr)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_ui)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*response_msg_ui)[strings::msg_params][strings::cmd_id] = kCommandId;
  (*response_msg_ui)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";

  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->on_event(event_vr);

  EXPECT_CALL(*mock_app_, is_perform_interaction_active())
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_app_, DeletePerformInteractionChoiceSet(_));

  command->on_event(event_ui);

  ResultCommandExpectations(response_to_mobile,
                            "UI is not supported by system");
}

}  // namespace perform_interaction_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
