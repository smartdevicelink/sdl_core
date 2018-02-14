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
#include "utils/helpers.h"
#include "utils/shared_ptr.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application_manager.h"
#include "mobile/read_did_response.h"
#include "mobile/delete_command_response.h"
#include "mobile/alert_maneuver_response.h"
#include "mobile/alert_response.h"
#include "mobile/list_files_response.h"
#include "mobile/subscribe_button_response.h"
#include "mobile/add_sub_menu_response.h"
#include "mobile/diagnostic_message_response.h"
#include "mobile/dial_number_response.h"
#include "mobile/end_audio_pass_thru_response.h"
#include "mobile/get_dtcs_response.h"
#include "mobile/get_vehicle_data_response.h"
#include "mobile/unregister_app_interface_response.h"
#include "mobile/unsubscribe_button_response.h"
#include "mobile/unsubscribe_way_points_response.h"
#include "mobile/update_turn_list_response.h"
#include "mobile/slider_response.h"
#include "mobile/speak_response.h"
#include "mobile/subscribe_vehicle_data_response.h"
#include "mobile/subscribe_way_points_response.h"
#include "mobile/system_response.h"
#include "mobile/get_way_points_response.h"
#include "mobile/perform_interaction_response.h"
#include "mobile/perform_audio_pass_thru_response.h"
#include "mobile/set_global_properties_response.h"
#include "mobile/set_media_clock_timer_response.h"
#include "mobile/show_constant_tbt_response.h"
#include "mobile/show_response.h"
#include "mobile/add_command_response.h"
#include "mobile/send_location_response.h"
#include "mobile/set_app_icon_response.h"
#include "mobile/set_display_layout_response.h"
#include "mobile/generic_response.h"
#include "mobile/set_app_icon_response.h"
#include "mobile/scrollable_message_response.h"
#include "mobile/change_registration_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace simple_response_commands_test {

namespace commands = sdl_rpc_plugin::commands;
namespace am = ::application_manager;

using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;

using am::commands::MessageSharedPtr;

template <class Command>
class MobileResponseCommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

typedef Types<commands::ListFilesResponse,
              commands::ReadDIDResponse,
              commands::DeleteCommandResponse,
              commands::AlertManeuverResponse,
              commands::AlertResponse,
              commands::SubscribeButtonResponse,
              commands::AddSubMenuResponse,
              commands::DiagnosticMessageResponse,
              commands::DialNumberResponse,
              commands::EndAudioPassThruResponse,
              commands::GetDTCsResponse,
              commands::GetVehicleDataResponse,
              commands::UnregisterAppInterfaceResponse,
              commands::UnsubscribeWayPointsResponse,
              commands::UpdateTurnListResponse,
              commands::UnsubscribeButtonResponse,
              commands::SliderResponse,
              commands::SpeakResponse,
              commands::SubscribeVehicleDataResponse,
              commands::SubscribeWayPointsResponse,
              commands::SystemResponse,
              commands::GetWayPointsResponse,
              commands::PerformInteractionResponse,
              commands::PerformAudioPassThruResponse,
              commands::SetGlobalPropertiesResponse,
              commands::SetMediaClockTimerResponse,
              commands::ShowConstantTBTResponse,
              commands::ShowResponse,
              commands::SystemResponse,
              commands::AddCommandResponse,
              commands::SendLocationResponse,
              commands::SetAppIconResponse,
              commands::SetDisplayLayoutResponse,
              commands::ChangeRegistrationResponse> ResponseCommandsList;

TYPED_TEST_CASE(MobileResponseCommandsTest, ResponseCommandsList);

TYPED_TEST(MobileResponseCommandsTest, Run_SendResponseToMobile_SUCCESS) {
  ::utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  EXPECT_CALL(this->app_mngr_, GetRPCService())
      .WillOnce(ReturnRef(this->mock_rpc_service_));
  EXPECT_CALL(this->mock_rpc_service_, SendMessageToMobile(NotNull(), _));
  command->Run();
}

class GenericResponseFromHMICommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

MATCHER_P2(CheckMessageParams, success, result, "") {
  const bool is_msg_type_correct =
      (am::MessageType::kResponse) ==
      static_cast<int32_t>(
          (*arg)[am::strings::params][am::strings::message_type].asInt());
  const bool is_success_correct =
      success == (*arg)[am::strings::msg_params][am::strings::success].asBool();
  const bool is_result_code_correct =
      result ==
      static_cast<int32_t>(
          (*arg)[am::strings::msg_params][am::strings::result_code].asInt());

  using namespace helpers;
  return Compare<bool, EQ, ALL>(
      true, is_msg_type_correct, is_success_correct, is_result_code_correct);
}

TEST_F(GenericResponseFromHMICommandsTest, Run_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SharedPtr<commands::GenericResponse> command(
      CreateCommand<commands::GenericResponse>(command_msg));

  EXPECT_CALL(this->app_mngr_, GetRPCService())
      .WillOnce(ReturnRef(this->mock_rpc_service_));
  EXPECT_CALL(
      this->mock_rpc_service_,
      SendMessageToMobile(
          CheckMessageParams(false, mobile_apis::Result::INVALID_DATA), false));

  command->Run();
}

class ScrollableMessageResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(ScrollableMessageResponseTest, Run_SUCCESS) {
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::SUCCESS;

  MockAppPtr app(CreateMockApp());

  SharedPtr<commands::ScrollableMessageResponse> command(
      CreateCommand<commands::ScrollableMessageResponse>(message));
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(app));
  ON_CALL(app_mngr_, GetRPCService())
      .WillByDefault(ReturnRef(mock_rpc_service_));
  EXPECT_CALL(*app, UnsubscribeFromSoftButtons(_));
  command->Run();
}

}  // namespace simple_response_commands_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
