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
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/include/application_manager/commands/mobile/read_did_response.h"
#include "application_manager/include/application_manager/commands/mobile/delete_command_response.h"
#include "application_manager/include/application_manager/commands/mobile/alert_maneuver_response.h"
#include "application_manager/include/application_manager/commands/mobile/alert_response.h"
#include "application_manager/include/application_manager/commands/mobile/list_files_response.h"
#include "application_manager/include/application_manager/commands/mobile/subscribe_button_response.h"
#include "application_manager/include/application_manager/commands/mobile/add_sub_menu_response.h"
#include "application_manager/include/application_manager/commands/mobile/diagnostic_message_response.h"
#include "application_manager/include/application_manager/commands/mobile/dial_number_response.h"
#include "application_manager/include/application_manager/commands/mobile/end_audio_pass_thru_response.h"
#include "application_manager/include/application_manager/commands/mobile/get_dtcs_response.h"
#include "application_manager/include/application_manager/commands/mobile/get_vehicle_data_response.h"
#include "application_manager/include/application_manager/commands/mobile/unregister_app_interface_response.h"
#include "application_manager/include/application_manager/commands/mobile/unsubscribe_button_response.h"
#include "application_manager/include/application_manager/commands/mobile/unsubscribe_way_points_response.h"
#include "application_manager/include/application_manager/commands/mobile/update_turn_list_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace simple_response_commands_test {

namespace commands = ::application_manager::commands;

using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;

using commands::MessageSharedPtr;

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
              commands::UnsubscribeButtonResponse> ResponseCommandsList;
TYPED_TEST_CASE(MobileResponseCommandsTest, ResponseCommandsList);

TYPED_TEST(MobileResponseCommandsTest, Run_SendResponseToMobile_SUCCESS) {
  ::utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(NotNull(), _));
  command->Run();
}

}  // namespace simple_response_commands_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
