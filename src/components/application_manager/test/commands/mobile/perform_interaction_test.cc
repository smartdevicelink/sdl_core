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

  void SetHMIInterfaceState(const am::HmiInterfaces::InterfaceState ui_state,
                            const am::HmiInterfaces::InterfaceState vr_state) {
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(ui_state));
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
        .WillByDefault(Return(vr_state));
  }

  MessageSharedPtr PrepareResponseFromHMI(
      const hmi_apis::Common_Result::eType result_code, const char* info) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::hmi_response::code] = result_code;
    (*msg)[am::strings::msg_params] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (info) {
      (*msg)[am::strings::msg_params][am::strings::info] = info;
    }
    return msg;
  }

  void CheckExpectations(const hmi_apis::Common_Result::eType ui_hmi_response,
                         const hmi_apis::Common_Result::eType vr_hmi_response,
                         const char* ui_info,
                         const char* vr_info,
                         const mobile_apis::Result::eType mobile_response,
                         const char* mobile_info,
                         const am::HmiInterfaces::InterfaceState ui_state,
                         const am::HmiInterfaces::InterfaceState vr_state,
                         const bool success) {
    MessageSharedPtr msg_from_mobile =
        CreateMessage(smart_objects::SmartType_Map);
    (*msg_from_mobile)[strings::params][strings::connection_key] =
        kConnectionKey;
    (*msg_from_mobile)[strings::msg_params][strings::interaction_mode] =
        mobile_apis::InteractionMode::VR_ONLY;
    utils::SharedPtr<PerformInteractionRequest> command =
        CreateCommand<PerformInteractionRequest>(msg_from_mobile);
    SetHMIInterfaceState(ui_state, vr_state);
    command->Init();
    MessageSharedPtr response_msg_vr =
        PrepareResponseFromHMI(vr_hmi_response, vr_info);
    am::event_engine::Event event_vr(
        hmi_apis::FunctionID::VR_PerformInteraction);
    event_vr.set_smart_object(*response_msg_vr);

    MessageSharedPtr response_msg_ui =
        PrepareResponseFromHMI(ui_hmi_response, ui_info);
    am::event_engine::Event event_ui(
        hmi_apis::FunctionID::UI_PerformInteraction);
    event_ui.set_smart_object(*response_msg_ui);

    MessageSharedPtr response_to_mobile;
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

    command->on_event(event_vr);
    command->on_event(event_ui);

    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::success]
            .asBool(),
        success);
    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
            .asInt(),
        static_cast<int32_t>(mobile_response));
    if (mobile_info) {
      EXPECT_EQ(
          (*response_to_mobile)[am::strings::msg_params][am::strings::info]
              .asString(),
          mobile_info);
    }
  }

  sync_primitives::Lock lock_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(PerformInteractionRequestTest, DISABLED_OnTimeout_VR_GENERIC_ERROR) {
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
  EXPECT_FALSE((*vr_command_result)[strings::msg_params][strings::info].asString().empty());
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_VRUnsupportedResource_UISuccess_MobileResultUNSUPPORTED_RESOURCE) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const char* ui_info = NULL;
  const char* vr_info = "VR is not supported by system";
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = "VR is not supported by system";
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_UIUnsupportedResource_VRSuccess_MobileResultUNSUPPORTED_RESOURCE) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const char* ui_info = "UI is not supported by system";
  const char* vr_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = "UI is not supported by system";
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_BothInterfaceIsAvailable_VRResultSUCCESS_UIResultWARNINGS_MobileResultWarning) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const char* ui_info = NULL;
  const char* vr_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::WARNINGS;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_VRInterfaceNotRespond_VRResultUnsupported_UIResultWARNINGS_MobileResultUnsupported) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const char* ui_info = NULL;
  const char* vr_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_NOT_RESPONSE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

}  // namespace perform_interaction_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
