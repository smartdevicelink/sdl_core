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
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/commands/mobile/read_did_request.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/smart_object_keys.h"
#include "event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace read_did_request {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
namespace am = ::application_manager;
using am::commands::ReadDIDRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using ::utils::SharedPtr;

class ReadDIDRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(ReadDIDRequestTest, OnEvent_WrongEventId_UNSUCCESS) {
  Event event(Event::EventID::INVALID_ENUM);
  SharedPtr<ReadDIDRequest> command(CreateCommand<ReadDIDRequest>());

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  command->on_event(event);
}

TEST_F(ReadDIDRequestTest, OnEvent_SUCCESS) {
  Event event(Event::EventID::VehicleInfo_ReadDID);

  SharedPtr<ReadDIDRequest> command(CreateCommand<ReadDIDRequest>());

  const hmi_apis::Common_Result::eType hmi_response_code =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::SUCCESS;
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_response_code;
  (*event_msg)[am::strings::msg_params] = 0;

  event.set_smart_object(*event_msg);

  am::MockMessageHelper& mock_message_helper(
      *am::MockMessageHelper::message_helper_mock());
  EXPECT_CALL(mock_message_helper, HMIToMobileResult(hmi_response_code))
      .WillOnce(Return(mobile_response_code));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_response_code), _));

  command->on_event(event);

  testing::Mock::VerifyAndClearExpectations(&mock_message_helper);
}

TEST_F(ReadDIDRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  SharedPtr<ReadDIDRequest> command(CreateCommand<ReadDIDRequest>());

  ON_CALL(app_mngr_, application(_))
      .WillByDefault(Return(SharedPtr<am::Application>()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(ReadDIDRequestTest, Run_CommandLimitsExceeded_UNSUCCESS) {
  SharedPtr<ReadDIDRequest> command(CreateCommand<ReadDIDRequest>());

  MockAppPtr app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(true));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(ReadDIDRequestTest, Run_EmptyDidLocation_UNSUCCESS) {
  MockAppPtr app(CreateMockApp());
  SharedPtr<ReadDIDRequest> command(CreateCommand<ReadDIDRequest>());

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(false));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(ReadDIDRequestTest, Run_SUCCESS) {
  MockAppPtr app(CreateMockApp());
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::msg_params][am::strings::did_location]["SomeData"] = 0;
  SharedPtr<ReadDIDRequest> command(CreateCommand<ReadDIDRequest>(msg));

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  ON_CALL(*app, AreCommandLimitsExceeded(_, _)).WillByDefault(Return(false));

  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command)));
  EXPECT_EQ(hmi_apis::FunctionID::VehicleInfo_ReadDID,
            static_cast<hmi_apis::FunctionID::eType>(
                (*result_msg)[am::strings::params][am::strings::function_id]
                    .asInt()));
}

}  // namespace read_did_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
