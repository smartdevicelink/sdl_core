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

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "mobile/set_display_layout_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_display_layout_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::Mock;
using ::testing::_;

using am::commands::SetDisplayLayoutRequest;
using am::commands::MessageSharedPtr;

typedef ::utils::SharedPtr<SetDisplayLayoutRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kCorrelationKey = 2u;
const uint32_t kAppId = 3u;
}  // namespace

MATCHER_P(CheckMshCorrId, corr_id, "") {
  return (*arg)[am::strings::params][am::strings::correlation_id].asUInt() ==
         corr_id;
}

class SetDisplayLayoutRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  typedef TypeIf<kMocksAreNice,
                 NiceMock<application_manager_test::MockHMICapabilities>,
                 application_manager_test::MockHMICapabilities>::Result
      MockHMICapabilities;
};
typedef SetDisplayLayoutRequestTest::MockHMICapabilities MockHMICapabilities;

TEST_F(SetDisplayLayoutRequestTest, Run_InvalidApp_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  MockAppPtr invalid_mock_app;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(invalid_mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SetDisplayLayoutRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationKey));
  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(CheckMshCorrId(kCorrelationKey)))
      .WillOnce(Return(true));

  command->Run();
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>());
  am::event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);
  SmartObject msg(smart_objects::SmartType_Map);

  event.set_smart_object(msg);

  EXPECT_CALL(mock_app_manager_, hmi_capabilities()).Times(0);
  command->on_event(event);
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_SUCCESS) {
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>());

  am::event_engine::Event event(hmi_apis::FunctionID::UI_SetDisplayLayout);
  MessageSharedPtr msg = CreateMessage();

  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::msg_params][am::hmi_response::display_capabilities] = 0;
  event.set_smart_object(*msg);

  MockHMICapabilities hmi_capabilities;
  MessageSharedPtr dispaly_capabilities_msg = CreateMessage();
  (*dispaly_capabilities_msg)[am::hmi_response::templates_available] =
      "templates_available";

  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(hmi_capabilities));

  EXPECT_CALL(hmi_capabilities, display_capabilities())
      .WillOnce(Return(dispaly_capabilities_msg.get()));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->on_event(event);
}

}  // namespace set_display_layout_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
