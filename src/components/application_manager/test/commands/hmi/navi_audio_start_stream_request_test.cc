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

#include <utility>

#include "application_manager/commands/hmi/navi_audio_start_stream_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace navi_audio_start_stream_request {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace commands = am::commands;
using commands::MessageSharedPtr;
using commands::AudioStartStreamRequest;
using am::event_engine::Event;

namespace {
const uint32_t kHmiAppId = 13u;
const am::HmiInterfaces::InterfaceID kHmiInterface =
    am::HmiInterfaces::HMI_INTERFACE_Navigation;
}  // namespace

class AudioStartStreamRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  AudioStartStreamRequestTest() {
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(app_mngr_settings_, start_stream_retry_amount())
        .WillByDefault(ReturnRef(start_stream_retry_amount_));
    msg_ = CreateMessage();
    command_ = CreateCommand<AudioStartStreamRequest>(msg_);
  }

  std::pair<uint32_t, int32_t> start_stream_retry_amount_;
  MessageSharedPtr msg_;
  SharedPtr<AudioStartStreamRequest> command_;
  MOCK(am::MockHmiInterfaces) mock_hmi_interfaces_;
};

TEST_F(AudioStartStreamRequestTest, Run_HmiInterfaceNotAvailable_NoRequest) {
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(kHmiInterface))
      .WillOnce(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(app_mngr_, SendMessageToHMI(_)).Times(0);

  command_->Run();
}

TEST_F(AudioStartStreamRequestTest,
       Run_HmiInterfaceAvailableButNoApp_NoRequest) {
  (*msg_)[am::strings::msg_params][am::strings::app_id] = kHmiAppId;
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(kHmiInterface))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(app_mngr_, application_by_hmi_app(kHmiAppId))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(app_mngr_, SendMessageToHMI(_)).Times(0);

  command_->Run();
}

TEST_F(AudioStartStreamRequestTest, Run_HmiInterfaceAvailable_SentRequest) {
  (*msg_)[am::strings::msg_params][am::strings::app_id] = kHmiAppId;
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(kHmiInterface))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application_by_hmi_app(kHmiAppId))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_audio_streaming_allowed(true));
  EXPECT_CALL(app_mngr_, SendMessageToHMI(msg_));

  command_->Run();
}

}  // namespace navi_audio_start_stream_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
