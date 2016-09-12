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

#include "gtest/gtest.h"
#include "application_manager/commands/mobile/set_media_clock_timer_request.h"
#include "utils/shared_ptr.h"
#include "commands/command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::SetMediaClockRequest;
using utils::SharedPtr;
using testing::_;
using testing::Return;

namespace strings = application_manager::strings;
namespace UpdateMode = mobile_apis::UpdateMode;
namespace hmi_response = application_manager::hmi_response;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kAppID = 2u;
const uint32_t kHours = 2u;
const uint32_t kMinutes = 26u;
const uint32_t kSeconds = 1u;
}  // namespace

class SetMediaClockRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetMediaClockRequestTest() {
    mock_app_ = CreateMockApp();
  }

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    return msg;
  }

  void ExpectationsSetupHelper(bool is_media) {
    EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, is_media_application()).WillOnce(Return(is_media));
    EXPECT_CALL(*mock_app_, app_id()).Times(0);
    EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _));
  }

  MockAppPtr mock_app_;
};

TEST_F(SetMediaClockRequestTest, Run_UpdateCountUp_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::update_mode] = UpdateMode::COUNTUP;
  (*msg)[strings::msg_params][strings::start_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::start_time][strings::minutes] = kMinutes;
  (*msg)[strings::msg_params][strings::end_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::end_time][strings::minutes] = kMinutes;
  (*msg)[strings::msg_params][strings::end_time][strings::seconds] = kSeconds;

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, is_media_application()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_UpdateCountDown_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::update_mode] = UpdateMode::COUNTDOWN;
  (*msg)[strings::msg_params][strings::start_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::start_time][strings::minutes] = kMinutes;
  (*msg)[strings::msg_params][strings::start_time][strings::seconds] = kSeconds;
  (*msg)[strings::msg_params][strings::end_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::end_time][strings::minutes] = kMinutes;

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, is_media_application()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_UpdateCountUpWrongTime_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::update_mode] = UpdateMode::COUNTUP;
  (*msg)[strings::msg_params][strings::start_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::start_time][strings::minutes] = kMinutes;
  (*msg)[strings::msg_params][strings::start_time][strings::seconds] = kSeconds;
  (*msg)[strings::msg_params][strings::end_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::end_time][strings::minutes] = kMinutes;

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  ExpectationsSetupHelper(true);

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_UpdateCountDownWrongTime_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::update_mode] = UpdateMode::COUNTDOWN;
  (*msg)[strings::msg_params][strings::start_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::start_time][strings::minutes] = kMinutes;
  (*msg)[strings::msg_params][strings::end_time][strings::hours] = kHours;
  (*msg)[strings::msg_params][strings::end_time][strings::minutes] = kMinutes;
  (*msg)[strings::msg_params][strings::end_time][strings::seconds] = kSeconds;

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, is_media_application()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _));

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_NoStartTime_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::update_mode] = UpdateMode::COUNTDOWN;

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  ExpectationsSetupHelper(true);

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_NoUpdateMode_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  ExpectationsSetupHelper(true);

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_NotMediaApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  ExpectationsSetupHelper(false);

  command->Run();
}

TEST_F(SetMediaClockRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app_, is_media_application()).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _));

  command->Run();
}

TEST_F(SetMediaClockRequestTest, OnEvent_Success) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][hmi_response::code] = mobile_apis::Result::SUCCESS;
  (*msg)[strings::msg_params] = SmartObject(smart_objects::SmartType_Null);

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _));

  Event event(hmi_apis::FunctionID::UI_SetMediaClockTimer);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetMediaClockRequestTest, OnEvent_Canceled) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<SetMediaClockRequest> command(
      CreateCommand<SetMediaClockRequest>(msg));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::UI_Slider);
  event.set_smart_object(*msg);

  command->on_event(event);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
