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

#include "gtest/gtest.h"
#include "utils/lock.h"
#include "utils/helpers.h"
#include "hmi/sdl_activate_app_request.h"
#include "application_manager/mock_application.h"
#include "application_manager/application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_state_controller.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace sdl_activate_app_request {

namespace am = ::application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::SDLActivateAppRequest;
using am::ApplicationSet;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;
using testing::Mock;
using ::testing::NiceMock;
using policy_test::MockPolicyHandlerInterface;
using am::event_engine::Event;

namespace {
const uint32_t kCorrelationID = 1u;
const uint32_t kAppID = 2u;
const uint32_t kAppIDFirst = 1u;
const connection_handler::DeviceHandle kHandle = 2u;
}  // namespace

MATCHER_P2(CheckMsgParams, result, corr_id, "") {
  const bool is_func_id_valid =
      hmi_apis::FunctionID::SDL_ActivateApp ==
      static_cast<int32_t>(
          (*arg)[strings::params][strings::function_id].asInt());

  const bool is_result_code_valid =
      hmi_apis::Common_Result::APPLICATION_NOT_REGISTERED ==
      static_cast<int32_t>(
          (*arg)[strings::msg_params][strings::result_code].asInt());

  const bool is_result_valid =
      result == (*arg)[strings::msg_params][strings::success].asBool();

  const bool is_corr_id_valid =
      corr_id == ((*arg)[strings::params][strings::correlation_id].asUInt());

  using namespace helpers;
  return Compare<bool, EQ, ALL>(true,
                                is_func_id_valid,
                                is_result_code_valid,
                                is_result_valid,
                                is_corr_id_valid);
}

class SDLActivateAppRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SDLActivateAppRequestTest()
      : lock_(std::make_shared<sync_primitives::Lock>()) {}

 protected:
  ~SDLActivateAppRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&app_mngr_);
  }

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    MockAppPtr mock_app = CreateMockApp();
    CommandRequestTest<CommandsTestMocks::kIsNice>::InitCommand(timeout);
    ON_CALL((*mock_app), app_id()).WillByDefault(Return(kAppID));
    ON_CALL(app_mngr_, application_by_hmi_app(kAppID))
        .WillByDefault(Return(mock_app));
  }
  void SetCorrelationAndAppID(MessageSharedPtr msg) {
    (*msg)[strings::params][strings::correlation_id] = kCorrelationID;
    (*msg)[strings::msg_params][strings::app_id] = kAppID;
  }

  ApplicationSet app_list_;
  std::shared_ptr<sync_primitives::Lock> lock_;
  policy_test::MockPolicyHandlerInterface policy_handler_;
  application_manager_test::MockStateController mock_state_controller_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
};

#ifdef EXTERNAL_PROPRIETARY_MODE

TEST_F(SDLActivateAppRequestTest, Run_ActivateApp_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));

  EXPECT_CALL(mock_policy_handler_, OnActivateApp(kAppID, kCorrelationID));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, DISABLED_Run_DactivateApp_REJECTED) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);
  (*msg)[strings::msg_params][strings::function_id] =
      hmi_apis::FunctionID::SDL_ActivateApp;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(true));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::SDL_ActivateApp)))
      .WillOnce(Return(true));

  command->Run();
}
#else

TEST_F(SDLActivateAppRequestTest, FindAppToRegister_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kAppID)).WillOnce(Return(mock_app));

  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_app, IsRegistered()).WillOnce(Return(false));
  ON_CALL(*mock_app, device()).WillByDefault(Return(kHandle));

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));

  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillOnce(Return(false));
  ON_CALL(*mock_app_first, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));
  ON_CALL(*mock_app, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));

  const std::string url = "url";
  ON_CALL(*mock_app_first, SchemaUrl()).WillByDefault(Return(url));
  const std::string package = "package";
  ON_CALL(*mock_app_first, PackageName()).WillByDefault(Return(package));

  EXPECT_CALL(mock_message_helper_, SendLaunchApp(_, _, _, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, AppIdNotFound_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kAppID))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(app_mngr_, WaitingApplicationByID(kAppID))
      .WillOnce(Return(ApplicationSharedPtr()));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, DevicesAppsEmpty_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));

  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_app, IsRegistered()).WillOnce(Return(false));
  ON_CALL(*mock_app, device()).WillByDefault(Return(kHandle));

  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));
  app_list_ = accessor.GetData();

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppActive_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));

  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));

  app_list_ = accessor.GetData();

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(app_mngr_, application(kAppIDFirst))
      .WillByDefault(Return(mock_app_first));

  app_list_.insert(mock_app_first);
  ON_CALL(*mock_app_first, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));

  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillRepeatedly(Return(true));

  EXPECT_CALL(mock_message_helper_, SendLaunchApp(_, _, _, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppNotActive_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_app, IsRegistered()).WillOnce(Return(true));

  EXPECT_CALL(mock_policy_handler_, OnActivateApp(kAppID, kCorrelationID));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppIsForeground_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());

  const std::string schema("schema");
  mock_app->SetShemaUrl(schema);
  const std::string package_name("package_name");
  mock_app->SetPackageName(package_name);

  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(*mock_app, IsRegistered()).WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));
  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));
  ON_CALL(*mock_app_first, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillOnce(Return(true));

  EXPECT_CALL(mock_message_helper_, SendLaunchApp(_, schema, package_name, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppNotRegisteredAndEmpty_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));
  ON_CALL(*mock_app_first, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillOnce(Return(false));

  EXPECT_CALL(mock_message_helper_, SendLaunchApp(_, _, _, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppNotRegistered_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, state_controller())
      .WillOnce(ReturnRef(mock_state_controller_));
  EXPECT_CALL(mock_state_controller_,
              IsStateActive(am::HmiState::StateID::STATE_ID_DEACTIVATE_HMI))
      .WillOnce(Return(false));
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));

  app_list_ = accessor.GetData();

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  ON_CALL(*mock_app_first, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillRepeatedly(Return(true));

  EXPECT_CALL(mock_message_helper_, SendLaunchApp(_, _, _, _));

  command->Run();
}
#endif

TEST_F(SDLActivateAppRequestTest, OnTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  SetCorrelationAndAppID(msg);

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));
  ON_CALL(mock_event_dispatcher_, remove_observer(_, _));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).WillOnce(Return(true));

  command->onTimeOut();
}

TEST_F(SDLActivateAppRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  MessageSharedPtr event_msg = CreateMessage();
  (*event_msg)[strings::params][strings::correlation_id] = kCorrelationID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>());

  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  event.set_smart_object(*event_msg);
  EXPECT_CALL(app_mngr_, application_by_hmi_app(_)).Times(0);

  command->on_event(event);
}

TEST_F(SDLActivateAppRequestTest, OnEvent_InvalidAppId_UNSUCCESS) {
  MessageSharedPtr event_msg = CreateMessage();
  (*event_msg)[strings::msg_params][strings::application][strings::app_id] =
      kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>());

  Event event(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
  event.set_smart_object(*event_msg);

  MockAppPtr invalid_mock_app;
  EXPECT_CALL(app_mngr_, application_by_hmi_app(_))
      .WillOnce(Return(invalid_mock_app));

  command->on_event(event);
}

TEST_F(SDLActivateAppRequestTest, OnEvent_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::correlation_id] = kCorrelationID;
  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MessageSharedPtr event_msg = CreateMessage();
  (*event_msg)[strings::msg_params][strings::application][strings::app_id] =
      kAppID;

  Event event(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
  event.set_smart_object(*event_msg);

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application_by_hmi_app(_)).WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppID));

  EXPECT_CALL(mock_policy_handler_, OnActivateApp(kAppID, kCorrelationID));

  command->on_event(event);
}

}  // namespace sdl_activate_app_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
