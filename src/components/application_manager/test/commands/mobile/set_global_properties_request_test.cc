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
#include <set>

#include "application_manager/commands/mobile/set_global_properties_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "utils/custom_string.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/smart_object_keys.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {

namespace am = application_manager;
namespace hmi_response = ::application_manager::hmi_response;
namespace strings = ::application_manager::strings;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::testing::_;
using ::utils::SharedPtr;
using ::testing::Return;
using ::testing::ReturnRef;
using am::commands::SetGlobalPropertiesRequest;
using ::test::components::application_manager_test::MockApplication;

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
};

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_SUCCESS_Expect_MessageNotSend) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::msg_params][am::strings::info] = "test";

  am::event_engine::Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*response);

  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .Times(0);
  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UNSUPPORTED_RESOURCE_Expect_false) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::msg_params][am::strings::info] = "qwe";

  am::event_engine::Event event_tts(
      hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  am::event_engine::Event event_ui(
      hmi_apis::FunctionID::UI_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  MockMessageHelper* mock_message_helper =
      MockMessageHelper::message_helper_mock();
  EXPECT_CALL(*mock_message_helper, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

  MockHmiInterfaces hmi_interfaces;
  EXPECT_CALL(app_mngr_, hmi_interfaces())
      .WillRepeatedly(ReturnRef(hmi_interfaces));
  EXPECT_CALL(hmi_interfaces, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));
  command->Run();
  command->on_event(event_ui);
  command->on_event(event_tts);

  EXPECT_EQ((*response_to_mobile)[am::strings::msg_params][am::strings::success]
                .asBool(),
            false);
  EXPECT_EQ(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(mobile_apis::Result::INVALID_DATA));
}

}  // namespace commands_test
}  // namespace components
}  // namespace tests
