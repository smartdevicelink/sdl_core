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
#include "application_manager/commands/mobile/update_turn_list_request.h"
#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace update_turn_list_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Ref;
using ::testing::Eq;

using am::commands::UpdateTurnListRequest;
using am::commands::MessageSharedPtr;
using application_manager_test::MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 3u;
const hmi_apis::FunctionID::eType kFunctionId =
    hmi_apis::FunctionID::Buttons_OnButtonEvent;
const uint32_t kAppId = 5u;
}  // namespace

class UpdateTurnListRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  UpdateTurnListRequestTest()
      : mock_message_helper_(*am::MockMessageHelper::message_helper_mock())
      , command_msg_(CreateMessage(smart_objects::SmartType_Map))
      , command_(CreateCommand<UpdateTurnListRequest>(command_msg_)) {
    (*command_msg_)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    (*command_msg_)[am::strings::params][am::strings::function_id] =
        kFunctionId;
  }

  am::MockMessageHelper& mock_message_helper_;
  MessageSharedPtr command_msg_;
  ::utils::SharedPtr<UpdateTurnListRequest> command_;
  TypeIf<kMocksAreNice,
         NiceMock<policy_test::MockPolicyHandlerInterface>,
         policy_test::MockPolicyHandlerInterface>::Result mock_policy_handler_;
};

TEST_F(UpdateTurnListRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command_->Run();
}

TEST_F(UpdateTurnListRequestTest, Run_InvalidNavigationText_UNSUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::navigation_text] = "invalid_navigation_text\t\n";

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command_->Run();
}

TEST_F(UpdateTurnListRequestTest, Run_InvalidTurnIcon_UNSUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::navigation_text] = "valid_navigation_text";
  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::turn_icon][am::strings::value] =
                     "invalid_turn_icon\t\n";

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command_->Run();
}

TEST_F(UpdateTurnListRequestTest,
       Run_ProcessSoftButtonsNotSucceeded_UNSUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::navigation_text] = "valid_navigation_text";
  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::turn_icon][am::strings::value] =
                     "valid_turn_icon";

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));

  const mobile_result::eType kExpectedResult = mobile_result::INVALID_ENUM;
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*command_msg_)[am::strings::msg_params],
                                 Eq(mock_app),
                                 Ref(mock_policy_handler_),
                                 Ref(app_mngr_)))
      .WillOnce(Return(kExpectedResult));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(kExpectedResult), _));

  command_->Run();
}

TEST_F(UpdateTurnListRequestTest, Run_NoTurnList_UNSUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*command_msg_)[am::strings::msg_params],
                                 Eq(mock_app),
                                 Ref(mock_policy_handler_),
                                 Ref(app_mngr_)))
      .WillOnce(Return(mobile_result::SUCCESS));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command_->Run();
}

TEST_F(UpdateTurnListRequestTest, Run_ValidTurnList_SUCCESS) {
  const std::string kNavigationText = "valid_navigation_text";

  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::navigation_text] = kNavigationText;
  (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                 [am::strings::turn_icon][am::strings::value] =
                     "valid_turn_icon";
  (*command_msg_)[am::strings::msg_params][am::strings::soft_buttons] = 0;

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*command_msg_)[am::strings::msg_params],
                                 Eq(mock_app),
                                 Ref(mock_policy_handler_),
                                 Ref(app_mngr_)))
      .WillOnce(Return(mobile_result::SUCCESS));

  EXPECT_CALL(
      mock_message_helper_,
      VerifyImage(
          (*command_msg_)[am::strings::msg_params][am::strings::turn_list][0]
                         [am::strings::turn_icon],
          Eq(mock_app),
          Ref(app_mngr_))).WillOnce(Return(mobile_result::SUCCESS));

  EXPECT_CALL(mock_message_helper_,
              SubscribeApplicationToSoftButton(_, _, kFunctionId));

  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command_)));
  ASSERT_TRUE(result_msg);
  EXPECT_EQ(
      hmi_apis::FunctionID::Navigation_UpdateTurnList,
      (*result_msg)[am::strings::params][am::strings::function_id].asInt());

  ASSERT_TRUE((*result_msg)[am::strings::msg_params][am::strings::turn_list][0]
                  .keyExists(am::hmi_request::navi_text));

  EXPECT_TRUE((*result_msg)[am::strings::msg_params][am::strings::turn_list][0]
                           [am::hmi_request::navi_text].keyExists(
                               am::hmi_request::field_name));
  EXPECT_EQ(
      hmi_apis::Common_TextFieldName::turnText,
      (*result_msg)[am::strings::msg_params][am::strings::turn_list][0]
                   [am::hmi_request::navi_text][am::hmi_request::field_name]
                       .asInt());

  EXPECT_TRUE((*result_msg)[am::strings::msg_params][am::strings::turn_list][0]
                           [am::hmi_request::navi_text].keyExists(
                               am::hmi_request::field_text));
  EXPECT_EQ(
      kNavigationText,
      (*result_msg)[am::strings::msg_params][am::strings::turn_list][0]
                   [am::hmi_request::navi_text][am::hmi_request::field_text]
                       .asString());
}

TEST_F(UpdateTurnListRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command_->on_event(event);
}

TEST_F(UpdateTurnListRequestTest, OnEvent_UnsupportedResource_SUCCESS) {
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  const hmi_apis::Common_Result::eType hmi_response_code =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const mobile_result::eType mobile_response_code =
      mobile_result::UNSUPPORTED_RESOURCE;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_response_code;
  (*event_msg)[am::strings::msg_params] = 0;

  Event event(hmi_apis::FunctionID::Navigation_UpdateTurnList);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_response_code))
      .WillOnce(Return(mobile_response_code));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_response_code), _));

  command_->on_event(event);
}

TEST_F(UpdateTurnListRequestTest,
       OnEvent_ReceivedNavigationUpdateTurnListEvent_SUCCESS) {
  const hmi_apis::Common_Result::eType hmi_response_code =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_result::eType mobile_response_code = mobile_result::SUCCESS;

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_response_code;
  (*event_msg)[am::strings::msg_params] = 0;

  Event event(hmi_apis::FunctionID::Navigation_UpdateTurnList);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_response_code))
      .WillOnce(Return(mobile_response_code));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_response_code), _));

  command_->on_event(event);
}

}  // namespace update_turn_list_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
