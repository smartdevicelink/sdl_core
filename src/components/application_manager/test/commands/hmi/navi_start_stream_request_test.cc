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
#include <utility>

#include "hmi/navi_start_stream_request.h"
#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_event_dispatcher.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace navi_start_stream_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using test::components::event_engine_test::MockEventDispatcher;
using am::commands::NaviStartStreamRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

typedef SharedPtr<NaviStartStreamRequest> NaviStartStreamRequestPtr;

namespace {
const int32_t kConnectionKey = 2;
const uint32_t kAppId = 3u;
const uint32_t kCorId = 5u;
}  // namespace

class NaviStartStreamRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    command_msg = CreateMessage(smart_objects::SmartType_Map);
    (*command_msg)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    (*command_msg)[am::strings::params][am::strings::correlation_id] = kCorId;
    app = CreateMockApp();

    ON_CALL(mock_app_manager_settings_, start_stream_retry_amount())
        .WillByDefault(ReturnRef(stream_retry));
  }

  MockAppPtr app;
  MessageSharedPtr command_msg;
  std::pair<uint32_t, int32_t> stream_retry;
  MockEventDispatcher mock_event_dispatcher_;
};

MATCHER_P(CheckSendRequest, connection_key, "") {
  return connection_key ==
         (*arg)[am::strings::params][am::strings::connection_key].asInt();
}

TEST_F(NaviStartStreamRequestTest, Run_ApplicationNotExists_RequestNotSend) {
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(_))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(_)).Times(0);

  command->Run();
}

TEST_F(NaviStartStreamRequestTest,
       Run_AppIDGivenAndApplicationExists_RequestSend) {
  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppId))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, set_video_streaming_allowed(true));

  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(command_msg));

  command->Run();
}

TEST_F(NaviStartStreamRequestTest,
       Run_AppNotGivenApplicationExists_RequestSend) {
  // AppId is not given
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  EXPECT_CALL(*app, hmi_app_id()).Times(0);
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  // Get default app id
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(0))
      .WillOnce(Return(app));

  EXPECT_CALL(*app, set_video_streaming_allowed(true));

  EXPECT_CALL(mock_app_manager_,
              SendMessageToHMI(CheckSendRequest(kConnectionKey)));

  command->Run();
}

TEST_F(NaviStartStreamRequestTest,
       OnEvent_StartStreamingSuccessful_StreamingApproved) {
  const hmi_apis::Common_Result::eType code = hmi_apis::Common_Result::SUCCESS;

  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  (*event_msg)[am::strings::params][am::hmi_response::code] = code;

  Event event(hmi_apis::FunctionID::Navigation_StartStream);
  event.set_smart_object(*event_msg);
  // AppId is given
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppId))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(mock_app_manager_,
              HMILevelAllowsStreaming(
                  kAppId, protocol_handler::ServiceType::kMobileNav))
      .WillOnce(Return(true));
  EXPECT_CALL(*app, set_video_streaming_approved(true));

  command->on_event(event);
}

TEST_F(NaviStartStreamRequestTest,
       OnEvent_StartStreamingRejected_StreamingNotStarted) {
  const hmi_apis::Common_Result::eType code = hmi_apis::Common_Result::REJECTED;

  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  (*event_msg)[am::strings::params][am::hmi_response::code] = code;

  Event event(hmi_apis::FunctionID::Navigation_StartStream);
  event.set_smart_object(*event_msg);

  // AppId is given
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppId))
      .WillOnce(Return(app))
      .WillOnce(Return(app));

  EXPECT_CALL(mock_app_manager_, HMILevelAllowsStreaming(_, _)).Times(0);
  // Streaming is not approved
  EXPECT_CALL(*app, set_video_streaming_approved(_)).Times(0);

  EXPECT_CALL(mock_app_manager_, TerminateRequest(kConnectionKey, kCorId));

  command->on_event(event);
}
TEST_F(NaviStartStreamRequestTest,
       OnEvent_StartStreamingNotRejectedAndNotSuccessful_StreamingNotStarted) {
  const hmi_apis::Common_Result::eType code = hmi_apis::Common_Result::ABORTED;

  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] = code;

  Event event(hmi_apis::FunctionID::Navigation_StartStream);
  event.set_smart_object(*event_msg);

  // AppId is given
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppId))
      .WillOnce(Return(app));

  EXPECT_CALL(mock_app_manager_, HMILevelAllowsStreaming(_, _)).Times(0);
  // Streaming is not approved
  EXPECT_CALL(*app, set_video_streaming_approved(_)).Times(0);

  // Send nothing
  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(_)).Times(0);
  command->on_event(event);
}

TEST_F(NaviStartStreamRequestTest, OnTimeOut_VideoStreamingApproved) {
  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;

  const int32_t retry_number = 10;
  stream_retry.first = retry_number;

  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(_))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, video_streaming_allowed()).WillOnce(Return(true));
  EXPECT_CALL(*app, video_streaming_approved()).WillOnce(Return(true));
  EXPECT_CALL(*app, set_video_stream_retry_number(0));

  EXPECT_CALL(mock_app_manager_, TerminateRequest(kConnectionKey, kCorId));
  command->onTimeOut();
}

TEST_F(NaviStartStreamRequestTest,
       OnTimeOut_VideoStreamingNotApproved_RetryVideo) {
  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;

  const int32_t retry_number = 10;
  stream_retry.first = retry_number;

  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  ON_CALL(*app, app_id()).WillByDefault(Return(kAppId));
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(_))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, video_streaming_allowed()).WillOnce(Return(true));
  // Video has not approved yet
  EXPECT_CALL(*app, video_streaming_approved()).WillOnce(Return(false));
  EXPECT_CALL(*app, set_video_stream_retry_number(0)).Times(0);

  EXPECT_CALL(*am::MockMessageHelper::message_helper_mock(),
              SendNaviStartStream(kAppId, _));

  // Retry number increased
  EXPECT_CALL(*app, set_video_stream_retry_number(1));

  EXPECT_CALL(mock_app_manager_, TerminateRequest(kConnectionKey, kCorId));
  command->onTimeOut();
}

TEST_F(NaviStartStreamRequestTest,
       OnTimeOut_VideoStreamingNotApproved_RetryNumberExceeded_VideoStopped) {
  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;

  const int32_t retry_number = 10;
  stream_retry.first = retry_number;

  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app));
  EXPECT_CALL(*app, hmi_app_id())
      .WillOnce(Return(kAppId))
      .WillOnce(Return(kAppId));
  NaviStartStreamRequestPtr command(
      CreateCommand<NaviStartStreamRequest>(command_msg));

  ON_CALL(*app, app_id()).WillByDefault(Return(kAppId));
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(_))
      .WillRepeatedly(Return(app));
  EXPECT_CALL(*app, video_streaming_allowed()).WillRepeatedly(Return(true));
  // Video has not approved yet
  EXPECT_CALL(*app, video_streaming_approved()).WillRepeatedly(Return(false));
  EXPECT_CALL(*app, set_video_stream_retry_number(0)).Times(0);

  // Retry number exceeded
  EXPECT_CALL(*app, video_stream_retry_number()).WillOnce(Return(retry_number));

  EXPECT_CALL(*am::MockMessageHelper::message_helper_mock(),
              SendNaviStartStream(kAppId, _)).Times(0);
  EXPECT_CALL(*app, set_video_stream_retry_number(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, EndNaviServices(kAppId));
  EXPECT_CALL(mock_app_manager_, TerminateRequest(kConnectionKey, kCorId));
  command->onTimeOut();
}

}  // namespace navi_start_stream_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
