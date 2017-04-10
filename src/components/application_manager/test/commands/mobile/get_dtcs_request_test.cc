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
#include <vector>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/commands/mobile/get_dtcs_request.h"
#include "application_manager/mock_message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace get_dtcs_request {

using ::testing::_;
using ::testing::Return;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::commands::GetDTCsRequest;
using am::event_engine::Event;
using am::MockMessageHelper;
namespace mobile_result = mobile_apis::Result;

typedef SharedPtr<GetDTCsRequest> GetDTCsRequestPtr;

class GetDTCsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(GetDTCsRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  GetDTCsRequestPtr command(CreateCommand<GetDTCsRequest>());

  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(GetDTCsRequestTest, Run_SUCCESS) {
  const uint32_t kConnectionKey = 2u;
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::dtc_mask] = 0;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  GetDTCsRequestPtr command(CreateCommand<GetDTCsRequest>(command_msg));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(
                  HMIResultCodeIs(hmi_apis::FunctionID::VehicleInfo_GetDTCs)));

  command->Run();
}

TEST_F(GetDTCsRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  GetDTCsRequestPtr command(CreateCommand<GetDTCsRequest>());

  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command->on_event(event);
}

TEST_F(GetDTCsRequestTest, OnEvent_SUCCESS) {
  GetDTCsRequestPtr command(CreateCommand<GetDTCsRequest>());

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::msg_params] = 0;
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;

  Event event(hmi_apis::FunctionID::VehicleInfo_GetDTCs);
  event.set_smart_object(*event_msg);
  MockMessageHelper& mock_message_helper =
      *MockMessageHelper::message_helper_mock();

  ON_CALL(mock_message_helper, HMIToMobileResult(_))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));

  command->on_event(event);
}

}  // namespace get_dtcs_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
