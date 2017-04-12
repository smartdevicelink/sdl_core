/*

 Copyright (c) 2016, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>
#include "gtest/gtest.h"
#include "mobile/speak_request.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
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
namespace speak_request {

namespace am = application_manager;
namespace hmi_response = ::application_manager::hmi_response;
namespace strings = ::application_manager::strings;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::testing::_;
using ::utils::SharedPtr;
using ::testing::Return;
using ::testing::ReturnRef;
using am::commands::SpeakRequest;
using ::test::components::application_manager_test::MockApplication;

class SpeakRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SpeakRequestTest()
      : mock_message_helper_(*am::MockMessageHelper::message_helper_mock())
      , request_(CreateMessage(smart_objects::SmartType_Map))
      , response_(CreateMessage(smart_objects::SmartType_Map)) {
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  ~SpeakRequestTest() {
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  MessageSharedPtr ManageResponse() {
    return response_;
  }
  MessageSharedPtr ManageRequest() {
    return request_;
  }

  void CheckExpectations(const hmi_apis::Common_Result::eType hmi_response,
                         const mobile_apis::Result::eType mobile_response,
                         const am::HmiInterfaces::InterfaceState state,
                         const bool success) {
    utils::SharedPtr<SpeakRequest> command =
        CreateCommand<SpeakRequest>(ManageRequest());

    (*ManageResponse())[strings::params][hmi_response::code] = hmi_response;
    (*ManageResponse())[strings::msg_params] = 0;

    am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
    event_tts.set_smart_object(*ManageResponse());

    MockAppPtr mock_app(CreateMockApp());
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

    MessageSharedPtr response_to_mobile;
    MockHmiInterfaces hmi_interfaces;
    EXPECT_CALL(app_mngr_, hmi_interfaces())
        .WillOnce(ReturnRef(hmi_interfaces));
    EXPECT_CALL(hmi_interfaces, GetInterfaceState(_)).WillOnce(Return(state));
    MockMessageHelper* mock_message_helper =
        MockMessageHelper::message_helper_mock();
    EXPECT_CALL(*mock_message_helper, HMIToMobileResult(_))
        .WillOnce(Return(mobile_response));

    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

    command->on_event(event_tts);

    EXPECT_EQ(
        (*response_to_mobile)[strings::msg_params][strings::success].asBool(),
        success);
    EXPECT_EQ((*response_to_mobile)[strings::msg_params][strings::result_code]
                  .asInt(),
              static_cast<int32_t>(mobile_response));
  }

  am::MockMessageHelper& mock_message_helper_;

 private:
  MessageSharedPtr request_;
  MessageSharedPtr response_;
};

TEST_F(SpeakRequestTest, OnEvent_SUCCESS_Expect_true) {
  utils::SharedPtr<SpeakRequest> command =
      CreateCommand<SpeakRequest>(ManageRequest());

  (*ManageResponse())[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*ManageResponse())[strings::msg_params] = 0;

  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*ManageResponse());

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  MessageSharedPtr response_to_mobile;

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->on_event(event_tts);

  EXPECT_EQ(
      (*response_to_mobile)[strings::msg_params][strings::success].asBool(),
      true);
  EXPECT_EQ(
      (*response_to_mobile)[strings::msg_params][strings::result_code].asInt(),
      static_cast<int32_t>(mobile_apis::Result::SUCCESS));
}

TEST_F(SpeakRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_STATE_AVAILABLE_Expect_true) {
  CheckExpectations(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_AVAILABLE,
                    true);
}

TEST_F(SpeakRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_STATE_NOT_AVAILABLE_Expect_false) {
  CheckExpectations(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE,
                    false);
}

TEST_F(SpeakRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_STATE_NOT_RESPONSE_Expect_true) {
  CheckExpectations(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
                    mobile_apis::Result::UNSUPPORTED_RESOURCE,
                    am::HmiInterfaces::STATE_NOT_RESPONSE,
                    true);
}

}  // namespace speak_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace component
}  // namespace test
