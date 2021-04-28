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
 * Neither the name of the copyright holders nor the names of their contributors
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

#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/smart_object_keys.h"
#include "gtest/gtest.h"
#include "hmi/ui_get_capabilities_response.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace ui_get_capabilities_response {

using ::testing::NiceMock;
namespace am = ::application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;
namespace hmi_interface = ::application_manager::hmi_interface;
using am::commands::CommandImpl;
using application_manager::commands::ResponseFromHMI;
using sdl_rpc_plugin::commands::UIGetCapabilitiesResponse;

typedef std::shared_ptr<ResponseFromHMI> ResponseFromHMIPtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class UIGetCapabilitiesResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateCommandMsg() {
    MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
    (*command_msg)[strings::msg_params][strings::number] = "123";
    (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*command_msg)[strings::params][hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;
    (*command_msg)[strings::msg_params][hmi_response::capabilities] =
        (capabilities_);

    return command_msg;
  }

  SmartObject capabilities_;
};

TEST_F(UIGetCapabilitiesResponseTest, RUN_SetDisplay_SUCCESSS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][hmi_response::display_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*command_msg)[strings::msg_params][hmi_response::display_capabilities]
                [hmi_response::display_type] = "GEN2_8_DMA";
  (*command_msg)[strings::msg_params][hmi_response::display_capabilities]
                [hmi_response::display_name] = "GENERIC_DISPLAY";

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject display_capabilities_so =
      (*command_msg)[strings::msg_params][hmi_response::display_capabilities];

  EXPECT_CALL(mock_hmi_capabilities_,
              set_display_capabilities(display_capabilities_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetSoftButton_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][hmi_response::soft_button_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*command_msg)[strings::msg_params][hmi_response::soft_button_capabilities]
                [hmi_response::image_supported] = true;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject soft_button_capabilities_so =
      (*command_msg)[strings::msg_params]
                    [hmi_response::soft_button_capabilities];

  EXPECT_CALL(mock_hmi_capabilities_,
              set_soft_button_capabilities(soft_button_capabilities_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetHmiZone_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][hmi_response::hmi_zone_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*command_msg)[strings::msg_params][hmi_response::hmi_zone_capabilities][0] =
      "FRONT";

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject hmi_zone_capabilities_so =
      (*command_msg)[strings::msg_params][hmi_response::hmi_zone_capabilities];

  EXPECT_CALL(mock_hmi_capabilities_,
              set_hmi_zone_capabilities(hmi_zone_capabilities_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetAudioPassThru_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::audio_pass_thru_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject audio_pass_thru_capabilities_so =
      (*command_msg)[strings::msg_params]
                    [strings::audio_pass_thru_capabilities];

  // hmi_capabilities will receive a list of capabilities, the first element
  // being audio_pass_thru_capabilities_so
  smart_objects::SmartObject audio_pass_thru_capabilities_list_so =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  audio_pass_thru_capabilities_list_so[0] = audio_pass_thru_capabilities_so;
  EXPECT_CALL(
      mock_hmi_capabilities_,
      set_audio_pass_thru_capabilities(audio_pass_thru_capabilities_list_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetAudioPassThruList_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();

  // if both audioPassThruCapabilities and audioPassThruCapabilitiesList are
  // supplied, audioPassThruCapabilitiesList should be used
  smart_objects::SmartObject audio_pass_thru_capabilities_so =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject audio_pass_thru_capabilities_list_so =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  (*command_msg)[strings::msg_params][strings::audio_pass_thru_capabilities] =
      audio_pass_thru_capabilities_so;
  (*command_msg)[strings::msg_params]
                [strings::audio_pass_thru_capabilities_list] =
                    audio_pass_thru_capabilities_list_so;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  EXPECT_CALL(
      mock_hmi_capabilities_,
      set_audio_pass_thru_capabilities(audio_pass_thru_capabilities_list_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetNavigation_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::hmi_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*command_msg)[strings::msg_params][strings::hmi_capabilities]
                [strings::navigation] = true;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject hmi_capabilities_so =
      (*command_msg)[strings::msg_params][strings::hmi_capabilities];
  EXPECT_CALL(mock_hmi_capabilities_,
              set_navigation_supported(
                  hmi_capabilities_so[strings::navigation].asBool()));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetPhoneCall_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::hmi_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*command_msg)[strings::msg_params][strings::hmi_capabilities]
                [strings::phone_call] = true;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject hmi_capabilities_so =
      (*command_msg)[strings::msg_params][strings::hmi_capabilities];
  EXPECT_CALL(mock_hmi_capabilities_,
              set_phone_call_supported(
                  hmi_capabilities_so[strings::phone_call].asBool()));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetVideoStreaming_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::hmi_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*command_msg)[strings::msg_params][strings::hmi_capabilities]
                [strings::video_streaming] = true;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject hmi_capabilities_so =
      (*command_msg)[strings::msg_params][strings::hmi_capabilities];
  EXPECT_CALL(mock_hmi_capabilities_,
              set_video_streaming_supported(
                  hmi_capabilities_so[strings::video_streaming].asBool()));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetNavigationCapability_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::system_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*command_msg)[strings::msg_params][strings::system_capabilities]
                [strings::navigation_capability]["sendLocationEnabled"] = true;

  (*command_msg)[strings::msg_params][strings::system_capabilities]
                [strings::navigation_capability]["getWayPointsEnabled"] = true;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject navigation_capability_so =
      (*command_msg)[strings::msg_params][strings::system_capabilities]
                    [strings::navigation_capability];

  EXPECT_CALL(mock_hmi_capabilities_,
              set_navigation_capability(navigation_capability_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetPhonenCapability_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::system_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*command_msg)[strings::msg_params][strings::system_capabilities]
                [strings::phone_capability]["dialNumberEnabled"] = true;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  smart_objects::SmartObject phone_capability_so =
      (*command_msg)[strings::msg_params][strings::system_capabilities]
                    [strings::phone_capability];

  EXPECT_CALL(mock_hmi_capabilities_,
              set_phone_capability(phone_capability_so));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetVideoStreamingCapability_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::system_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*command_msg)[strings::msg_params][strings::system_capabilities]
                [strings::video_streaming_capability] =
                    smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& video_streaming_capability =
      (*command_msg)[strings::msg_params][strings::system_capabilities]
                    [strings::video_streaming_capability];

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

  video_streaming_capability[strings::supported_formats][0][strings::protocol] =
      hmi_apis::Common_VideoStreamingProtocol::RAW;

  video_streaming_capability[strings::supported_formats][0][strings::codec] =
      hmi_apis::Common_VideoStreamingCodec::H264;

  video_streaming_capability[strings::haptic_spatial_data_supported] = true;

  video_streaming_capability[strings::diagonal_screen_size] = 7.47;

  video_streaming_capability[strings::pixel_per_inch] = 117.f;

  video_streaming_capability[strings::scale] = 1.f;
  video_streaming_capability[strings::preferred_fps] = 30;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_video_streaming_capability(video_streaming_capability));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest, SetSystemDisplayCapabilities_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::system_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  const auto& display_capability_so =
      (*command_msg)[strings::msg_params][strings::system_capabilities]
                    [strings::display_capabilities];

  EXPECT_CALL(mock_hmi_capabilities_,
              set_system_display_capabilities(display_capability_so));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest,
       SaveCachedCapabilitiesToFileCall_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::msg_params][strings::system_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              SaveCachedCapabilitiesToFile(hmi_interface::ui, _, _));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(UIGetCapabilitiesResponseTest,
       onTimeOut_Run_ResponseForInterface_ReceivedError) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;

  ResponseFromHMIPtr command(
      CreateCommand<UIGetCapabilitiesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              UpdateRequestsRequiredForCapabilities(
                  hmi_apis::FunctionID::UI_GetCapabilities));
  ASSERT_TRUE(command->Init());

  command->Run();
}

}  // namespace ui_get_capabilities_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
