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
#include "utils/shared_ptr_helpers.h"
#include "smart_objects/smart_object.h"
#include "interfaces/HMI_API.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/request_controller_settings.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/commands/hmi/update_device_list_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace update_device_list_request {


using testing::_;
using testing::ReturnRef;
using testing::Return;
using test::components::event_engine_test::MockEventDispatcher;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
namespace am = ::application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;
using am::event_engine::Event;
using am::commands::UpdateDeviceListRequest;
using am::commands::CommandImpl;

typedef std::shared_ptr<UpdateDeviceListRequest> UpdateDeviceListRequestPtr;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class UpdateDeviceListRequestTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateCommandMsg() {
    MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
    (*command_msg)[strings::msg_params][strings::number] = "123";
    (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*command_msg)[strings::params][hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;

    return command_msg;
  }

  MockApplicationManagerSettings settings_;
  MockEventDispatcher mock_event_dispatcher_;
};

TEST_F(UpdateDeviceListRequestTest, RUN_LaunchHMIReturnsFalse) {
  MessageSharedPtr command_msg = CreateCommandMsg();

  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher_));
  EXPECT_CALL(mock_event_dispatcher_, remove_observer(_));

  UpdateDeviceListRequestPtr command(
      CreateCommand<UpdateDeviceListRequest>(command_msg));

  EXPECT_CALL(app_mngr_, get_settings()).WillOnce(ReturnRef(settings_));

  EXPECT_CALL(settings_, launch_hmi()).WillOnce(Return(false));

  EXPECT_CALL(app_mngr_, IsHMICooperating()).Times(0);

  EXPECT_CALL(app_mngr_, SendMessageToHMI(command_msg));

  command->Run();

  EXPECT_EQ((*command_msg)[strings::params][strings::protocol_type].asInt(),
            CommandImpl::hmi_protocol_type_);
  EXPECT_EQ((*command_msg)[strings::params][strings::protocol_version].asInt(),
            CommandImpl::protocol_version_);
}

TEST_F(UpdateDeviceListRequestTest, RUN_HMICooperatingReturnsTrue_SUCCESSS) {
  MessageSharedPtr command_msg = CreateCommandMsg();

  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher_));
  EXPECT_CALL(mock_event_dispatcher_, remove_observer(_));

  UpdateDeviceListRequestPtr command(
      CreateCommand<UpdateDeviceListRequest>(command_msg));

  EXPECT_CALL(app_mngr_, get_settings()).WillOnce(ReturnRef(settings_));

  EXPECT_CALL(settings_, launch_hmi()).WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, IsHMICooperating()).WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, SendMessageToHMI(command_msg));

  command->Run();

  EXPECT_EQ((*command_msg)[strings::params][strings::protocol_type].asInt(),
            CommandImpl::hmi_protocol_type_);
  EXPECT_EQ((*command_msg)[strings::params][strings::protocol_version].asInt(),
            CommandImpl::protocol_version_);
}

TEST_F(UpdateDeviceListRequestTest, OnEvent_WrongEventId_UNSUCCESS) {
  Event event(Event::EventID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher_));
  EXPECT_CALL(mock_event_dispatcher_, remove_observer(_));

  UpdateDeviceListRequestPtr command(CreateCommand<UpdateDeviceListRequest>());

  command->on_event(event);
}

TEST_F(UpdateDeviceListRequestTest, OnEvent_SUCCESS) {
  Event event(Event::EventID::BasicCommunication_OnReady);

  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher_));
  EXPECT_CALL(mock_event_dispatcher_, remove_observer(_, _));
  EXPECT_CALL(mock_event_dispatcher_, remove_observer(_));

  UpdateDeviceListRequestPtr command(CreateCommand<UpdateDeviceListRequest>());

  command->on_event(event);
}

}  // namespace update_device_list_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
