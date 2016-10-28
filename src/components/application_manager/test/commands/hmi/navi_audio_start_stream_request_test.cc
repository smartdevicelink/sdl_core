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
#include <utility>
#include "gtest/gtest.h"
#include "application_manager/commands/hmi/navi_audio_start_stream_request.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_event_observer.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace navi_audio_start_stream_request {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::AudioStartStreamRequest;
using application_manager::commands::CommandImpl;
using application_manager::event_engine::Event;
using application_manager::MockMessageHelper;
using protocol_handler::ServiceType;
using test::components::event_engine_test::MockEventDispatcher;
using test::components::event_engine_test::MockEventObserver;
using testing::Return;
using testing::ReturnRef;
using testing::_;

namespace strings = ::application_manager::strings;
namespace hmi_response = application_manager::hmi_response;

namespace {
const uint32_t kCorrelationID = 1u;
const uint32_t kAppID = 2u;
const uint32_t kConnectionKey = 3u;
const uint32_t kRetryNumberNull = 0u;
}

class AudioStartStreamRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  AudioStartStreamRequestTest() {}

  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
    stream_retry_.first = 2u;
    stream_retry_.second = 0;
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    InitGetters();
    command_ = CreateCommand<AudioStartStreamRequest>(msg_);
  }

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    CommandRequestTest<CommandsTestMocks::kIsNice>::InitCommand(timeout);

    ON_CALL(mock_app_manager_settings_, start_stream_retry_amount())
        .WillByDefault(ReturnRef(stream_retry_));
  }

  void InitGetters() {
    ON_CALL(mock_app_manager_, application(kAppID))
        .WillByDefault(Return(mock_app_));
    ON_CALL((*mock_app_), hmi_app_id()).WillByDefault(Return(kAppID));
    ON_CALL((*mock_app_), app_id()).WillByDefault(Return(kAppID));
    (*msg_)[strings::params][strings::correlation_id] = kCorrelationID;
    (*msg_)[strings::msg_params][strings::app_id] = kAppID;
  }

 protected:
  std::pair<uint32_t, int32_t> stream_retry_;
  MockAppPtr mock_app_;
  MessageSharedPtr msg_;
  SharedPtr<AudioStartStreamRequest> command_;
};

TEST_F(AudioStartStreamRequestTest, Run_ValidApp_SUCCESS) {
  EXPECT_CALL(event_dispatcher_,
              add_observer(hmi_apis::FunctionID::Navigation_StartAudioStream,
                           kCorrelationID,
                           _));
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, set_audio_streaming_allowed(true));
  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(msg_));
  command_->Run();
  EXPECT_EQ(CommandImpl::hmi_protocol_type_,
            (*msg_)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(CommandImpl::protocol_version_,
            (*msg_)[strings::params][strings::protocol_version].asInt());
}

TEST_F(AudioStartStreamRequestTest, Run_InvalidApp_UNSUCCESS) {
  EXPECT_CALL(event_dispatcher_,
              add_observer(hmi_apis::FunctionID::Navigation_StartAudioStream,
                           kCorrelationID,
                           _));
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app_, set_audio_streaming_allowed(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, SendMessageToHMI(_)).Times(0);
  command_->Run();
}

TEST_F(AudioStartStreamRequestTest,
       OnEvent_SuccessAndStreamingAllowed_SUCCESS) {
  (*msg_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_app_manager_,
              HMILevelAllowsStreaming(kAppID, ServiceType::kAudio))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, set_audio_streaming_approved(true));
  Event event(hmi_apis::FunctionID::Navigation_StartAudioStream);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AudioStartStreamRequestTest,
       OnEvent_SuccessAndStreamingNotAllowed_Cancelled) {
  (*msg_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_app_manager_,
              HMILevelAllowsStreaming(kAppID, ServiceType::kAudio))
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_app_, set_audio_streaming_approved(_)).Times(0);
  Event event(hmi_apis::FunctionID::Navigation_StartAudioStream);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AudioStartStreamRequestTest, OnEvent_Rejected_Cancelled) {
  (*msg_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::REJECTED;
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(mock_app_manager_, HMILevelAllowsStreaming(_, _)).Times(0);
  EXPECT_CALL(*mock_app_, set_audio_streaming_approved(_)).Times(0);
  Event event(hmi_apis::FunctionID::Navigation_StartAudioStream);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AudioStartStreamRequestTest,
       OnEvent_Navigation_StopAudioStream_SUCCESS) {
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_app_manager_, HMILevelAllowsStreaming(_, _)).Times(0);
  Event event(hmi_apis::FunctionID::Navigation_StopAudioStream);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AudioStartStreamRequestTest, OnEvent_InvalidApp_Cancelled) {
  (*msg_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::REJECTED;
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(mock_app_manager_, HMILevelAllowsStreaming(_, _)).Times(0);
  EXPECT_CALL(*mock_app_, set_audio_streaming_approved(_)).Times(0);
  Event event(hmi_apis::FunctionID::Navigation_StartAudioStream);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AudioStartStreamRequestTest, OnTimeOut_Terminated) {
  (*msg_)[strings::params][strings::connection_key] = kConnectionKey;
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(mock_app_manager_,
              TerminateRequest(kConnectionKey, kCorrelationID));
  command_->onTimeOut();
}

TEST_F(AudioStartStreamRequestTest,
       RetryStartSession_AudioStreamApproved_SUCCESS) {
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, audio_streaming_allowed()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, audio_streaming_approved()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, set_audio_stream_retry_number(kRetryNumberNull));
  command_->RetryStartSession();
}

TEST_F(AudioStartStreamRequestTest,
       RetryStartSession_AudioStreamNotApproved_UNSUCCESS) {
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, audio_streaming_allowed()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, audio_streaming_approved()).WillOnce(Return(false));
  EXPECT_CALL(*mock_app_, audio_stream_retry_number())
      .WillOnce(Return(kRetryNumberNull));
  EXPECT_CALL(mock_message_helper_, SendAudioStartStream(kAppID, _));
  const uint32_t retry_number_one = 1u;
  EXPECT_CALL(*mock_app_, set_audio_stream_retry_number(retry_number_one));
  command_->RetryStartSession();
}

TEST_F(AudioStartStreamRequestTest,
       RetryStartSession_AudioStreamCancelled_UNSUCCESS) {
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, audio_streaming_allowed()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, audio_streaming_approved()).WillOnce(Return(false));
  const uint32_t retry_number_two = 2u;
  EXPECT_CALL(*mock_app_, audio_stream_retry_number())
      .WillOnce(Return(retry_number_two));
  EXPECT_CALL(mock_app_manager_, EndNaviServices(kAppID));
  command_->RetryStartSession();
}

TEST_F(AudioStartStreamRequestTest, RetryStartSession_InvalidApp_Cancelled) {
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app_, audio_streaming_allowed()).Times(0);
  EXPECT_CALL(*mock_app_, audio_streaming_approved()).Times(0);
  EXPECT_CALL(*mock_app_, set_audio_stream_retry_number(kRetryNumberNull))
      .Times(0);
  command_->RetryStartSession();
}

TEST_F(AudioStartStreamRequestTest,
       RetryStartSession_StreamingNotAllowed_Cancelled) {
  EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, audio_streaming_allowed()).WillOnce(Return(false));
  EXPECT_CALL(*mock_app_, audio_streaming_approved()).Times(0);
  EXPECT_CALL(*mock_app_, set_audio_stream_retry_number(kRetryNumberNull))
      .Times(0);
  command_->RetryStartSession();
}

}  // namespace navi_audio_start_stream_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
