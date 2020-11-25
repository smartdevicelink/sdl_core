/*
 * Copyright (c) 2018, Ford Motor Company
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
#include <set>
#include <string>

#include "mobile/perform_interaction_request.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "gtest/gtest.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "utils/helpers.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace perform_interaction_request {

namespace am = application_manager;
using am::ApplicationManager;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::PerformInteractionRequest;
using ::test::components::application_manager_test::MockApplication;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

namespace {
const int32_t kCorrelationId = 1u;
const int32_t kCommandId = 1;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const int32_t kUiChoiceID = 1u;
const int32_t kVrChoiceID = 2u;
const int32_t kInvalidChoiceId = -1;
}  // namespace

class PerformInteractionRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformInteractionRequestTest()
      : mock_app_(CreateMockApp())
      , performinteraction_choice_set_lock_ptr_(
            std::make_shared<sync_primitives::RecursiveLock>()) {}

  void SetUp() OVERRIDE {
    smart_objects::SmartObject choice_set1(
        smart_objects::SmartType::SmartType_Map);
    smart_objects::SmartObject choice_set2(
        smart_objects::SmartType::SmartType_Map);

    choice_set1[strings::choice_set] =
        smart_objects::SmartType::SmartType_Array;
    choice_set2[strings::choice_set] =
        smart_objects::SmartType::SmartType_Array;

    choice_set1[strings::choice_set][0][strings::choice_id] = kUiChoiceID;
    choice_set2[strings::choice_set][0][strings::choice_id] = kVrChoiceID;

    choice_set_map_[kCorrelationId].insert(std::make_pair(
        kUiChoiceID, new smart_objects::SmartObject(choice_set1)));
    choice_set_map_[kCorrelationId].insert(std::make_pair(
        kVrChoiceID, new smart_objects::SmartObject(choice_set2)));

    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(*mock_app_, performinteraction_choice_set_map())
        .WillByDefault(Return(DataAccessor<app_mngr::PerformChoiceSetMap>(
            choice_set_map_, performinteraction_choice_set_lock_ptr_)));
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 bool success,
                                 hmi_apis::Common_Result::eType result_code,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[strings::msg_params][strings::success].asBool(), success);
    EXPECT_EQ((*msg)[strings::msg_params][strings::result_code].asInt(),
              static_cast<int32_t>(result_code));
    EXPECT_EQ((*msg)[strings::msg_params][strings::info].asString(), info);
  }

  void HMIRequestExpectations(MessageSharedPtr msg,
                              hmi_apis::FunctionID::eType function_id,
                              const std::string& method_name) {
    EXPECT_EQ((*msg)[strings::params][strings::function_id].asInt(),
              static_cast<int32_t>(function_id));
    EXPECT_EQ(
        (*msg)[strings::msg_params][am::hmi_request::method_name].asString(),
        method_name);
  }

  MessageSharedPtr CreateRequestMessage(
      const mobile_apis::InteractionMode::eType& interaction_mode) {
    MessageSharedPtr request_msg = CreateMessage(smart_objects::SmartType_Map);
    (*request_msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*request_msg)[strings::params][strings::correlation_id] = kCorrelationId;
    (*request_msg)[strings::msg_params][strings::interaction_mode] =
        interaction_mode;
    return request_msg;
  }

  MessageSharedPtr CreateHMIResponseMessage(
      const hmi_apis::Common_Result::eType& response_code,
      const std::string& message_info,
      const int32_t command_id = kCommandId) {
    MessageSharedPtr response_msg = CreateMessage(smart_objects::SmartType_Map);
    (*response_msg)[strings::params][hmi_response::code] = response_code;
    (*response_msg)[strings::msg_params][strings::cmd_id] = command_id;
    (*response_msg)[strings::msg_params][strings::info] = message_info;
    return response_msg;
  }

  MessageSharedPtr CreateHMIResponseMessageWithChoiceID(
      const hmi_apis::Common_Result::eType& response_code,
      const std::string& message_info,
      const int32_t choice_id,
      const int32_t command_id = kCommandId) {
    MessageSharedPtr response_msg =
        CreateHMIResponseMessage(response_code, message_info, kCommandId);
    (*response_msg)[strings::msg_params][strings::choice_id] = choice_id;
    return response_msg;
  }

  sync_primitives::Lock lock_;
  MockAppPtr mock_app_;
  app_mngr::PerformChoiceSetMap choice_set_map_;
  mutable std::shared_ptr<sync_primitives::RecursiveLock>
      performinteraction_choice_set_lock_ptr_;
};

class PerformInteractionRequestTestClass : public PerformInteractionRequest {
 public:
  PerformInteractionRequestTestClass(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler)
      : PerformInteractionRequest(message,
                                  application_manager,
                                  rpc_service,
                                  hmi_capabilities,
                                  policy_handler) {}

  void StartAwaitForInterfaces() {
    StartAwaitForInterface(am::HmiInterfaces::HMI_INTERFACE_VR);
    StartAwaitForInterface(am::HmiInterfaces::HMI_INTERFACE_UI);
  }
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
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(request_msg);
  MockAppPtr mock_app;

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  event.set_smart_object(*response_msg_vr);

  command->Init();
  command->StartAwaitForInterfaces();
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
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));
  command->OnTimeOut();

  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::success].asBool(),
      false);
  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::result_code].asInt(),
      static_cast<int32_t>(am::mobile_api::Result::GENERIC_ERROR));
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_BOTHMode_UIChoiceIdReceivedFirst) {
  MessageSharedPtr msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::BOTH);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  MessageSharedPtr response_msg_vr = CreateHMIResponseMessage(
      hmi_apis::Common_Result::SUCCESS, "", kInvalidChoiceId);
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui = CreateHMIResponseMessageWithChoiceID(
      hmi_apis::Common_Result::SUCCESS, "", kUiChoiceID);
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->StartAwaitForInterfaces();

  command->on_event(event_ui);
  command->on_event(event_vr);

  EXPECT_EQ(
      kUiChoiceID,
      (*response_to_mobile)[strings::msg_params][strings::choice_id].asInt());
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_BOTHMode_VRChoiceIdReceivedFirst) {
  MessageSharedPtr msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::BOTH);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  MessageSharedPtr response_msg_vr = CreateHMIResponseMessageWithChoiceID(
      hmi_apis::Common_Result::SUCCESS, "", kVrChoiceID);
  MessageSharedPtr response_msg_ui = CreateHMIResponseMessageWithChoiceID(
      hmi_apis::Common_Result::SUCCESS, "", kInvalidChoiceId);

  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  _, am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->StartAwaitForInterfaces();

  command->on_event(event_vr);
  EXPECT_EQ(hmi_apis::FunctionID::UI_ClosePopUp,
            (*request_to_hmi)[strings::params][strings::function_id].asInt());

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));
  command->on_event(event_ui);

  EXPECT_EQ(
      kVrChoiceID,
      (*response_to_mobile)[strings::msg_params][strings::choice_id].asInt());
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::VR_ONLY);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MockAppPtr mock_app;
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app));

  MessageSharedPtr response_msg_vr =
      CreateHMIResponseMessage(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                               "VR is not supported by system");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateHMIResponseMessage(hmi_apis::Common_Result::SUCCESS, "");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));

  MessageSharedPtr response_to_mobile;

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  ResultCommandExpectations(response_to_mobile,
                            false,
                            hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                            "VR is not supported by system");
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_UIHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::VR_ONLY);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MockAppPtr mock_app;
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app));

  MessageSharedPtr response_msg_vr =
      CreateHMIResponseMessage(hmi_apis::Common_Result::SUCCESS, "");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateHMIResponseMessage(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                               "UI is not supported by system");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  ResultCommandExpectations(response_to_mobile,
                            true,
                            hmi_apis::Common_Result::SUCCESS,
                            "UI is not supported by system");
}

TEST_F(
    PerformInteractionRequestTest,
    PrepareResultCodeAndResponseForMobile_GetVRResultCodeOnly_InVR_OnlyMode_SUCCESS) {
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  MessageSharedPtr msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::VR_ONLY);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();
  command->StartAwaitForInterfaces();

  MockAppPtr mock_app;
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app));

  MessageSharedPtr response_msg_vr =
      CreateHMIResponseMessage(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                               "VR is not supported by system");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateHMIResponseMessage(hmi_apis::Common_Result::SUCCESS, "");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  ResultCommandExpectations(response_to_mobile,
                            false,
                            hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                            "VR is not supported by system");
}

TEST_F(
    PerformInteractionRequestTest,
    PrepareResultCodeAndResponseForMobile_GetVR_ErrorInfoAndCode_InBOTH_Mode_With_UI_success_result_code) {
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  auto msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::BOTH);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MessageSharedPtr response_msg_vr =
      CreateHMIResponseMessage(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                               "VR is not supported by system");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateHMIResponseMessage(hmi_apis::Common_Result::SUCCESS, "");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  _, am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  HMIRequestExpectations(request_to_hmi,
                         hmi_apis::FunctionID::UI_ClosePopUp,
                         "UI.PerformInteraction");

  ResultCommandExpectations(response_to_mobile,
                            true,
                            hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                            "VR is not supported by system");
}

TEST_F(
    PerformInteractionRequestTest,
    PrepareResultCodeAndResponseForMobile_Send_GENERIC_ERROR_To_Mobile_When_different_valid_choice_ids_received_in_BOTH_mode_SUCCESS) {
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  MessageSharedPtr msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::BOTH);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MessageSharedPtr response_msg_vr = CreateHMIResponseMessageWithChoiceID(
      hmi_apis::Common_Result::SUCCESS, "", kVrChoiceID);
  MessageSharedPtr response_msg_ui = CreateHMIResponseMessageWithChoiceID(
      hmi_apis::Common_Result::SUCCESS, "", kUiChoiceID);

  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  _, am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  HMIRequestExpectations(request_to_hmi,
                         hmi_apis::FunctionID::UI_ClosePopUp,
                         "UI.PerformInteraction");

  ResultCommandExpectations(response_to_mobile,
                            false,
                            hmi_apis::Common_Result::GENERIC_ERROR,
                            "Received two different choice IDs");
}

TEST_F(
    PerformInteractionRequestTest,
    VR_response_WARNINGS_UI_response_SUCCESS_MobileResponseContains_true_WARNINGS_warnings_info) {
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  auto msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::VR_ONLY);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MessageSharedPtr response_msg_vr = CreateHMIResponseMessage(
      hmi_apis::Common_Result::WARNINGS, "WARNING MESSAGE");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui =
      CreateHMIResponseMessage(hmi_apis::Common_Result::SUCCESS, "");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  _, am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  HMIRequestExpectations(request_to_hmi,
                         hmi_apis::FunctionID::UI_ClosePopUp,
                         "UI.PerformInteraction");

  ResultCommandExpectations(response_to_mobile,
                            true,
                            hmi_apis::Common_Result::WARNINGS,
                            "WARNING MESSAGE");
}

TEST_F(
    PerformInteractionRequestTest,
    VR_response_SUCCESS_UI_response_WARNINGS_MobileResponseContains_true_WARNINGS_warnings_info) {
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  auto msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::BOTH);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MessageSharedPtr response_msg_vr =
      CreateHMIResponseMessage(hmi_apis::Common_Result::SUCCESS, "");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui = CreateHMIResponseMessage(
      hmi_apis::Common_Result::WARNINGS, "WARNING MESSAGE");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  _, am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  HMIRequestExpectations(request_to_hmi,
                         hmi_apis::FunctionID::UI_ClosePopUp,
                         "UI.PerformInteraction");

  ResultCommandExpectations(response_to_mobile,
                            true,
                            hmi_apis::Common_Result::WARNINGS,
                            "WARNING MESSAGE");
}

TEST_F(
    PerformInteractionRequestTest,
    VR_response_UNSUPPORTED_RESOURCE_UI_response_WARNINGS_MobileResponseContains_false_UNSUPPORTED_RESOURSE_error_info) {
  ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  auto msg_from_mobile =
      CreateRequestMessage(mobile_apis::InteractionMode::BOTH);
  std::shared_ptr<PerformInteractionRequestTestClass> command =
      CreateCommand<PerformInteractionRequestTestClass>(msg_from_mobile);

  ASSERT_TRUE(command->Init());

  command->StartAwaitForInterfaces();

  MessageSharedPtr response_msg_vr = CreateHMIResponseMessage(
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE, "VR error message");
  am::event_engine::Event event_vr(hmi_apis::FunctionID::VR_PerformInteraction);
  event_vr.set_smart_object(*response_msg_vr);

  MessageSharedPtr response_msg_ui = CreateHMIResponseMessage(
      hmi_apis::Common_Result::WARNINGS, "UI warning message");
  am::event_engine::Event event_ui(hmi_apis::FunctionID::UI_PerformInteraction);
  event_ui.set_smart_object(*response_msg_ui);

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  _, am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->on_event(event_vr);
  command->on_event(event_ui);

  HMIRequestExpectations(request_to_hmi,
                         hmi_apis::FunctionID::UI_ClosePopUp,
                         "UI.PerformInteraction");

  ResultCommandExpectations(response_to_mobile,
                            true,
                            hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                            "UI warning message, VR error message");
}

}  // namespace perform_interaction_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
