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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/include/application_manager/commands/mobile/subscribe_way_points_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/smart_object_keys.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace subscribe_way_points_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::DoAll;
using ::testing::SaveArg;
using ::testing::InSequence;
namespace am = ::application_manager;
using am::commands::SubscribeWayPointsRequest;
using am::commands::MessageSharedPtr;

typedef SharedPtr<SubscribeWayPointsRequest> CommandPtr;

class SubscribeWayPointsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(SubscribeWayPointsRequestTest, Run_SUCCESS) {
  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>());
  MockAppPtr app(CreateMockApp());

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(true));

  {
    InSequence dummy;
    EXPECT_CALL(app_mngr_, SubscribeAppForWayPoints(_));
    EXPECT_CALL(*app, UpdateHash());
  }

  MessageSharedPtr mobile_result_msg(
      CatchMobileCommandResult(CallRun(*command)));

  EXPECT_EQ(mobile_apis::Result::SUCCESS,
            static_cast<mobile_apis::Result::eType>(
                (*mobile_result_msg)[am::strings::msg_params]
                                    [am::strings::result_code].asInt()));
}

TEST_F(SubscribeWayPointsRequestTest, OnEvent_SUCCESS) {
  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>());
  MockAppPtr app(CreateMockApp());
  Event event(hmi_apis::FunctionID::Navigation_SubscribeWayPoints);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::msg_params] = 0;

  event.set_smart_object(*event_msg);

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  {
    InSequence dummy;
    EXPECT_CALL(app_mngr_, SubscribeAppForWayPoints(_));
    EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
    EXPECT_CALL(*app, UpdateHash());
  }

  command->on_event(event);
}

}  // namespace subscribe_way_points_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
