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

#include <memory>
#include <string>
#include <cstdint>

#include "mobile/close_application_request.h"

#include "gtest/gtest.h"

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_rpc_service.h"
#include "connection_handler/mock_connection_handler.h"
#include "protocol_handler/mock_session_observer.h"
#include "utils/macro.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace close_application_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using am::commands::MessageSharedPtr;
namespace am = ::application_manager;

typedef std::shared_ptr<sdl_rpc_plugin::commands::CloseApplicationRequest>
    CloseApplicationCommandPtr;

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 3u;
}

MessageSharedPtr CreateCloseAppMessage() {
  using namespace am;
  auto message = std::make_shared<SmartObject>(smart_objects::SmartType_Null);
  auto& params = (*message)[strings::params];
  params[strings::connection_key] = kConnectionKey;
  params[strings::function_id] = mobile_apis::FunctionID::CloseApplicationID;
  return message;
}

class CloseApplicationRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  CloseApplicationRequestTest()
      : mock_app_(CreateMockApp())
      , message_(CreateCloseAppMessage())
      , response_message_(CreateMessage()) {}

  void DeffaultRequestRunCheck(const bool success_send_to_hmi) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, hmi_level())
        .WillOnce(Return(mobile_apis::HMILevel::eType::HMI_FULL));

    ON_CALL(*am::MockMessageHelper::message_helper_mock(),
            GetBCActivateAppRequestToHMI(_, _, _, _, _, _))
        .WillByDefault(Return(
            std::make_shared<SmartObject>(smart_objects::SmartType_Null)));

    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_))
        .WillOnce(Return(success_send_to_hmi));
  }

  void SDLRecivesEventMessage(MessageSharedPtr& event_msg,
                              hmi_apis::Common_Result::eType event_result_code,
                              const bool is_app_exists) {
    (*event_msg)[am::strings::params][am::hmi_response::code] =
        event_result_code;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(is_app_exists ? mock_app_ : MockAppPtr()));

    EXPECT_CALL(mock_rpc_service_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandSource::SOURCE_SDL))
        .WillOnce(DoAll(SaveArg<0>(&response_message_), Return(true)));
  }

  void CheckResponseParameters(const bool is_success,
                               const mobile_apis::Result::eType result_code) {
    using namespace am;
    const auto message_result_code = static_cast<mobile_apis::Result::eType>(
        (*response_message_)[strings::msg_params][strings::result_code]
            .asInt());
    EXPECT_EQ(message_result_code, result_code);

    const bool message_success =
        (*response_message_)[strings::msg_params][strings::success].asBool();
    EXPECT_EQ(message_success, is_success);
  }

 protected:
  void SetUp() OVERRIDE {
    using namespace am;
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, GetRPCService())
        .WillByDefault(ReturnRef(mock_rpc_service_));
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId + 1));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(app_mngr_, connection_handler())
        .WillByDefault(ReturnRef(mock_connection_handler_));
    ON_CALL(mock_connection_handler_, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_handler_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  }

  MockAppPtr mock_app_;
  MessageSharedPtr message_;
  MessageSharedPtr response_message_;
  NiceMock<components::connection_handler_test::MockConnectionHandler>
      mock_connection_handler_;
  components::protocol_handler_test::MockSessionObserver mock_session_handler_;
  CloseApplicationCommandPtr command_;
};

TEST_F(
    CloseApplicationRequestTest,
    CloseApplicationRequest_SendsNegativeResponse_APPLICATION_NOT_REGISTERED) {
  using namespace am;
  auto message = CreateCloseAppMessage();

  CloseApplicationCommandPtr command(
      CreateCommand<sdl_rpc_plugin::commands::CloseApplicationRequest>(
          message));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_message_), Return(true)));
  command->Run();

  CheckResponseParameters(false,
                          mobile_apis::Result::APPLICATION_NOT_REGISTERED);
}

TEST_F(CloseApplicationRequestTest,
       CloseApplicationRequest_SendsNegativeResponse_IGNORED) {
  using namespace am;
  auto message = CreateCloseAppMessage();

  CloseApplicationCommandPtr command(
      CreateCommand<sdl_rpc_plugin::commands::CloseApplicationRequest>(
          message_));
  MockAppPtr mock_app(CreateMockApp());

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::eType::HMI_NONE));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_message_), Return(true)));
  command->Run();

  CheckResponseParameters(false, mobile_apis::Result::IGNORED);
}

TEST_F(CloseApplicationRequestTest,
       CloseApplicationRequest_SendsNegativeResponse_INVALID_DATA) {
  using namespace am;
  CloseApplicationCommandPtr command(
      CreateCommand<sdl_rpc_plugin::commands::CloseApplicationRequest>(
          message_));

  DeffaultRequestRunCheck(false);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_message_), Return(true)));

  command->Run();

  CheckResponseParameters(false, mobile_apis::Result::INVALID_DATA);
}

TEST_F(CloseApplicationRequestTest,
       CloseApplicationRequest_SendsNegativeResponse_GENERIC_ERROR) {
  using namespace am;

  CloseApplicationCommandPtr command(
      CreateCommand<sdl_rpc_plugin::commands::CloseApplicationRequest>(
          message_));

  DeffaultRequestRunCheck(true);

  command->Run();

  auto negative_response = CreateMessage();
  (*negative_response)[strings::msg_params][strings::result_code] =
      mobile_apis::Result::GENERIC_ERROR;
  ON_CALL(*am::MockMessageHelper::message_helper_mock(),
          CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
      .WillByDefault(Return(negative_response));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_message_), Return(true)));

  command->onTimeOut();

  CheckResponseParameters(false, mobile_apis::Result::GENERIC_ERROR);
}

TEST_F(CloseApplicationRequestTest,
       CloseApplicationRequest_OnEvent_SuccessResult_INVALID_DATA) {
  using namespace am;

  CloseApplicationCommandPtr command(
      CreateCommand<sdl_rpc_plugin::commands::CloseApplicationRequest>(
          message_));

  DeffaultRequestRunCheck(true);

  command->Run();

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  SDLRecivesEventMessage(event_msg, hmi_apis::Common_Result::SUCCESS, false);

  Event event(hmi_apis::FunctionID::BasicCommunication_ActivateApp);
  event.set_smart_object(*event_msg);

  command->on_event(event);

  CheckResponseParameters(false, mobile_apis::Result::INVALID_DATA);
}

TEST_F(CloseApplicationRequestTest,
       CloseApplicationRequest_OnEvent_SuccessResult_SUCCESS) {
  using namespace am;

  CloseApplicationCommandPtr command(
      CreateCommand<sdl_rpc_plugin::commands::CloseApplicationRequest>(
          message_));

  DeffaultRequestRunCheck(true);

  command->Run();

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  SDLRecivesEventMessage(event_msg, hmi_apis::Common_Result::SUCCESS, true);

  Event event(hmi_apis::FunctionID::BasicCommunication_ActivateApp);
  event.set_smart_object(*event_msg);

  command->on_event(event);

  CheckResponseParameters(true, mobile_apis::Result::SUCCESS);
}

}  // close_application_request
}  // mobile_commands_test
}  // commands_test
}  // components
}  // test
