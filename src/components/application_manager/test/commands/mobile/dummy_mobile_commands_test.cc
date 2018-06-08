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

#include "commands/command_request_test.h"

#include <stdint.h>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#include "mobile/add_command_request.h"
#include "mobile/add_command_response.h"
#include "mobile/add_sub_menu_request.h"
#include "mobile/add_sub_menu_response.h"
#include "mobile/alert_maneuver_request.h"
#include "mobile/alert_maneuver_response.h"
#include "mobile/alert_request.h"
#include "mobile/alert_response.h"
#include "mobile/change_registration_request.h"
#include "mobile/change_registration_response.h"
#include "mobile/create_interaction_choice_set_request.h"
#include "mobile/create_interaction_choice_set_response.h"
#include "mobile/delete_command_request.h"
#include "mobile/delete_command_response.h"
#include "mobile/delete_file_request.h"
#include "mobile/delete_file_response.h"
#include "mobile/delete_interaction_choice_set_request.h"
#include "mobile/delete_interaction_choice_set_response.h"
#include "mobile/delete_sub_menu_request.h"
#include "mobile/delete_sub_menu_response.h"
#include "mobile/diagnostic_message_request.h"
#include "mobile/diagnostic_message_response.h"
#include "mobile/dial_number_request.h"
#include "mobile/dial_number_response.h"
#include "mobile/end_audio_pass_thru_request.h"
#include "mobile/end_audio_pass_thru_response.h"
#include "mobile/generic_response.h"
#include "mobile/get_dtcs_request.h"
#include "mobile/get_dtcs_response.h"
#include "mobile/get_vehicle_data_request.h"
#include "mobile/get_vehicle_data_response.h"
#include "mobile/get_way_points_request.h"
#include "mobile/get_way_points_response.h"
#include "mobile/list_files_request.h"
#include "mobile/list_files_response.h"
#include "mobile/on_app_interface_unregistered_notification.h"
#include "mobile/on_audio_pass_thru_notification.h"
#include "mobile/on_button_event_notification.h"
#include "mobile/on_button_press_notification.h"
#include "mobile/on_command_notification.h"
#include "mobile/on_driver_distraction_notification.h"
#include "mobile/on_hash_change_notification.h"
#include "mobile/on_hmi_status_notification.h"
#include "mobile/on_hmi_status_notification_from_mobile.h"
#include "mobile/on_keyboard_input_notification.h"
#include "mobile/on_language_change_notification.h"
#include "mobile/on_permissions_change_notification.h"
#include "mobile/on_system_request_notification.h"
#include "mobile/on_tbt_client_state_notification.h"
#include "mobile/on_touch_event_notification.h"
#include "mobile/on_vehicle_data_notification.h"
#include "mobile/on_way_point_change_notification.h"
#include "mobile/perform_audio_pass_thru_request.h"
#include "mobile/perform_audio_pass_thru_response.h"
#include "mobile/perform_interaction_request.h"
#include "mobile/perform_interaction_response.h"
#include "mobile/put_file_request.h"
#include "mobile/put_file_response.h"
#include "mobile/read_did_request.h"
#include "mobile/read_did_response.h"
#include "mobile/register_app_interface_request.h"
#include "mobile/register_app_interface_response.h"
#include "mobile/reset_global_properties_request.h"
#include "mobile/reset_global_properties_response.h"
#include "mobile/scrollable_message_request.h"
#include "mobile/scrollable_message_response.h"
#include "mobile/send_location_request.h"
#include "mobile/send_location_response.h"
#include "mobile/set_app_icon_request.h"
#include "mobile/set_app_icon_response.h"
#include "mobile/set_display_layout_request.h"
#include "mobile/set_display_layout_response.h"
#include "mobile/set_global_properties_request.h"
#include "mobile/set_global_properties_response.h"
#include "mobile/set_media_clock_timer_request.h"
#include "mobile/set_media_clock_timer_response.h"
#include "mobile/show_constant_tbt_request.h"
#include "mobile/show_constant_tbt_response.h"
#include "mobile/show_request.h"
#include "mobile/show_response.h"
#include "mobile/slider_request.h"
#include "mobile/slider_response.h"
#include "mobile/speak_request.h"
#include "mobile/speak_response.h"
#include "mobile/subscribe_button_request.h"
#include "mobile/subscribe_button_response.h"
#include "mobile/subscribe_vehicle_data_request.h"
#include "mobile/subscribe_vehicle_data_response.h"
#include "mobile/subscribe_way_points_request.h"
#include "mobile/subscribe_way_points_response.h"
#include "mobile/system_response.h"
#include "mobile/unregister_app_interface_request.h"
#include "mobile/unregister_app_interface_response.h"
#include "mobile/unsubscribe_button_request.h"
#include "mobile/unsubscribe_button_response.h"
#include "mobile/unsubscribe_vehicle_data_request.h"
#include "mobile/unsubscribe_vehicle_data_response.h"
#include "mobile/unsubscribe_way_points_request.h"
#include "mobile/unsubscribe_way_points_response.h"
#include "mobile/update_turn_list_request.h"
#include "mobile/update_turn_list_response.h"

#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"

namespace am = application_manager;

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace dummy_mobile_commands_test {

namespace commands = ::application_manager::commands;

using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;
using commands::MessageSharedPtr;
using ::test::components::event_engine_test::MockEventDispatcher;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using ::application_manager::ApplicationSharedPtr;
using ::test::components::application_manager_test::MockApplication;

namespace {
const std::string kEmptyString_ = "";
}  // namespace

template <class Command>
class MobileCommandsTest : public components::commands_test::CommandRequestTest<
                               CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    EXPECT_CALL(app_mngr_settings_, default_timeout())
        .WillOnce(ReturnRef(timeout));
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kEmptyString_));
  }
};

template <class Command>
class MobileCommandsTestFirst : public MobileCommandsTest<Command> {
 public:
  using typename MobileCommandsTest<Command>::CommandType;
};

template <class Command>
class MobileCommandsTestSecond : public MobileCommandsTest<Command> {
 public:
  using typename MobileCommandsTest<Command>::CommandType;
};

template <class Command>
class MobileCommandsTestThird : public MobileCommandsTest<Command> {
 public:
  using typename MobileCommandsTest<Command>::CommandType;
};

/* macro TYPED_TEST_CASE takes max 50 args. That is why there are few
 * TYPED_TEST_CASE for HMI and mobile commands
 */

typedef Types<commands::AddCommandRequest,
              commands::AddCommandResponse,
              commands::AddSubMenuRequest,
              commands::AddSubMenuResponse,
              commands::AlertManeuverRequest,
              commands::AlertManeuverResponse,
              commands::AlertRequest,
              commands::AlertResponse,
              commands::ChangeRegistrationRequest,
              commands::ChangeRegistrationResponse,
              commands::CreateInteractionChoiceSetRequest,
              commands::CreateInteractionChoiceSetResponse,
              commands::DeleteCommandRequest,
              commands::DeleteCommandResponse,
              commands::DeleteFileRequest,
              commands::DeleteFileResponse,
              commands::DeleteInteractionChoiceSetRequest,
              commands::DeleteInteractionChoiceSetResponse,
              commands::DeleteSubMenuRequest,
              commands::DeleteSubMenuResponse,
              commands::DiagnosticMessageRequest,
              commands::DiagnosticMessageResponse,
              commands::DialNumberRequest,
              commands::DialNumberResponse,
              commands::EndAudioPassThruRequest,
              commands::EndAudioPassThruResponse,
              commands::GenericResponse,
              commands::GetDTCsRequest,
              commands::GetDTCsResponse,
              commands::GetVehicleDataRequest,
              commands::GetVehicleDataResponse,
              commands::GetWayPointsRequest,
              commands::GetWayPointsResponse,
              commands::ListFilesRequest,
              commands::ListFilesResponse,
              commands::OnAppInterfaceUnregisteredNotification,
              commands::OnAudioPassThruNotification,
              commands::mobile::OnButtonEventNotification,
              commands::mobile::OnButtonPressNotification,
              commands::OnCommandNotification,
              commands::mobile::OnDriverDistractionNotification,
              commands::mobile::OnHashChangeNotification,
              commands::OnHMIStatusNotification,
              commands::OnHMIStatusNotificationFromMobile,
              commands::mobile::OnKeyBoardInputNotification,
              commands::OnLanguageChangeNotification,
              commands::OnPermissionsChangeNotification,
              commands::mobile::OnSystemRequestNotification,
              commands::OnTBTClientStateNotification,
              commands::mobile::OnTouchEventNotification>
    MobileCommandsListFirst;

typedef Types<commands::OnVehicleDataNotification,
              commands::OnWayPointChangeNotification,
              commands::PerformAudioPassThruRequest,
              commands::PerformAudioPassThruResponse,
              commands::PerformInteractionRequest,
              commands::PerformInteractionResponse,
              commands::PutFileRequest,
              commands::PutFileResponse,
              commands::ReadDIDRequest,
              commands::ReadDIDResponse,
              commands::RegisterAppInterfaceRequest,
              commands::RegisterAppInterfaceResponse,
              commands::ResetGlobalPropertiesRequest,
              commands::ResetGlobalPropertiesResponse,
              commands::ScrollableMessageRequest,
              commands::ScrollableMessageResponse,
              commands::SendLocationRequest,
              commands::SendLocationResponse,
              commands::SetAppIconRequest,
              commands::SetAppIconResponse,
              commands::SetDisplayLayoutRequest,
              commands::SetDisplayLayoutResponse,
              commands::SetGlobalPropertiesRequest,
              commands::SetGlobalPropertiesResponse,
              commands::SetMediaClockRequest,
              commands::SetMediaClockTimerResponse,
              commands::ShowConstantTBTRequest,
              commands::ShowConstantTBTResponse,
              commands::ShowRequest,
              commands::ShowResponse,
              commands::SliderRequest,
              commands::SliderResponse,
              commands::SpeakRequest,
              commands::SpeakResponse,
              commands::SubscribeButtonRequest,
              commands::SubscribeButtonResponse,
              commands::SubscribeVehicleDataRequest,
              commands::SubscribeVehicleDataResponse,
              commands::SubscribeWayPointsRequest,
              commands::SubscribeWayPointsResponse,
              commands::SystemResponse,
              commands::UnregisterAppInterfaceRequest,
              commands::UnregisterAppInterfaceResponse,
              commands::UnsubscribeButtonRequest,
              commands::UnsubscribeButtonResponse,
              commands::UnsubscribeVehicleDataRequest> MobileCommandsListSecond;

typedef Types<commands::UnsubscribeVehicleDataResponse,
              commands::UnSubscribeWayPointsRequest,
              commands::UnsubscribeWayPointsResponse,
              commands::UpdateTurnListRequest,
              commands::UpdateTurnListResponse> MobileCommandsListThird;

TYPED_TEST_CASE(MobileCommandsTestFirst, MobileCommandsListFirst);
TYPED_TEST_CASE(MobileCommandsTestSecond, MobileCommandsListSecond);
TYPED_TEST_CASE(MobileCommandsTestThird, MobileCommandsListThird);

TYPED_TEST(MobileCommandsTestFirst, CtorAndDtorCall) {
  std::shared_ptr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

TYPED_TEST(MobileCommandsTestSecond, CtorAndDtorCall) {
  std::shared_ptr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}
TYPED_TEST(MobileCommandsTestThird, CtorAndDtorCall) {
  std::shared_ptr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

}  // namespace dummy_mobile_commands_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
