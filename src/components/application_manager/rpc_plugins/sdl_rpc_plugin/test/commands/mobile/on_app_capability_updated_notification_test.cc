/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "mobile/on_app_capability_updated_notification.h"

#include "application_manager/commands/commands_test.h"
#include "gtest/gtest.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_app_capability_updated_notification {

using sdl_rpc_plugin::commands::mobile::OnAppCapabilityUpdatedNotification;
using ::testing::_;

typedef std::shared_ptr<OnAppCapabilityUpdatedNotification>
    OnAppCapabilityUpdatedNotificationPtr;

namespace strings = application_manager::strings;
namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kApplicationId = 2u;
}  // namespace

MATCHER_P(CheckAppCapability, app_capability, "") {
  return app_capability == (*arg)[strings::msg_params][strings::app_capability];
}

class OnAppCapabilityUpdatedNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    (*message_)[strings::params][strings::connection_key] = kConnectionKey;
    //    (*message_)[strings::app_id] = kApplicationId;

    command_ = CreateCommand<OnAppCapabilityUpdatedNotification>(message_);
    mock_app_ = CreateMockApp();
  }

  void FillVideoStreamingCapability(
      smart_objects::SmartObject& video_streaming_capability) {
    video_streaming_capability[strings::preferred_resolution] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    video_streaming_capability[strings::preferred_resolution]
                              [strings::resolution_width] = 800;
    video_streaming_capability[strings::preferred_resolution]
                              [strings::resolution_height] = 354;

    video_streaming_capability[strings::max_bitrate] = 10000;

    video_streaming_capability[strings::supported_formats] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    video_streaming_capability[strings::supported_formats][0] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    video_streaming_capability[strings::supported_formats][0]
                              [strings::protocol] =
                                  hmi_apis::Common_VideoStreamingProtocol::RAW;

    video_streaming_capability[strings::supported_formats][0][strings::codec] =
        hmi_apis::Common_VideoStreamingCodec::H264;

    video_streaming_capability[strings::haptic_spatial_data_supported] = true;

    video_streaming_capability[strings::diagonal_screen_size] = 7.47;

    video_streaming_capability[strings::pixel_per_inch] = 117.f;

    video_streaming_capability[strings::scale] = 1.f;
  }

  OnAppCapabilityUpdatedNotificationPtr command_;
  MockAppPtr mock_app_;
  MessageSharedPtr message_;
};

TEST_F(OnAppCapabilityUpdatedNotificationTest,
       Run_OnAppCapabilityUpdatedNotificationSentToHMI) {
  smart_objects::SmartObject app_capability =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  app_capability[strings::app_capability_type] =
      mobile_apis::AppCapabilityType::VIDEO_STREAMING;
  app_capability[strings::video_streaming_capability] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  FillVideoStreamingCapability(
      app_capability[strings::video_streaming_capability]);

  (*message_)[strings::msg_params][strings::app_capability] = app_capability;

  ASSERT_TRUE(command_->Init());

  EXPECT_CALL(mock_rpc_service_,
              SendMessageToHMI(CheckAppCapability(app_capability)));
  command_->Run();
}

}  // namespace on_app_capability_updated_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
