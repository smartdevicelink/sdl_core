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

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "hmi/vi_get_vehicle_data_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace vi_get_vehicle_data_response {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
namespace hmi_response = am::hmi_response;
using am::commands::MessageSharedPtr;
using vehicle_info_plugin::commands::VIGetVehicleDataResponse;
using am::event_engine::Event;
using test::components::event_engine_test::MockEventDispatcher;

typedef std::shared_ptr<VIGetVehicleDataResponse> VIGetVehicleDataResponsePtr;

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 1u;
const std::string kStrNumber = "123";
}  // namespace

class VIGetVehicleDataResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(VIGetVehicleDataResponseTest, RUN_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = kStrNumber;
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*command_msg)[strings::params][strings::message_type] =
      hmi_apis::messageType::response;

  VIGetVehicleDataResponsePtr command(
      CreateCommand<VIGetVehicleDataResponse>(command_msg));

  am::event_engine::Event event(
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData);
  event.set_smart_object(*command_msg);

  EXPECT_CALL(mock_policy_handler_, OnVehicleDataUpdated(*command_msg));

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));

  command->Run();
}

TEST_F(VIGetVehicleDataResponseTest, ErrorResponse_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = kStrNumber;
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*command_msg)[strings::params][strings::message_type] =
      hmi_apis::messageType::error_response;
  (*command_msg)[strings::params][strings::data][strings::slider_position] = 1;
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*command_msg)[strings::params][strings::correlation_id] = kCorrelationId;
  (*command_msg)[am::strings::params][am::strings::error_msg] = "test_error";
  (*command_msg)[am::strings::params][am::strings::protocol_type] =
      am::commands::CommandImpl::hmi_protocol_type_;
  (*command_msg)[strings::params][strings::protocol_version] =
      am::commands::CommandImpl::protocol_version_;

  smart_objects::SmartObject result(smart_objects::SmartType_Map);
  result[strings::msg_params] = (*command_msg)[strings::params][strings::data];
  result[strings::params][hmi_response::code] =
      (*command_msg)[strings::params][hmi_response::code];
  result[strings::params][strings::correlation_id] =
      (*command_msg)[strings::params][strings::correlation_id];
  result[strings::params][strings::error_msg] =
      (*command_msg)[strings::params][strings::error_msg];
  result[strings::params][strings::message_type] =
      (*command_msg)[strings::params][strings::message_type];
  result[strings::params][strings::protocol_type] =
      (*command_msg)[strings::params][strings::protocol_type];
  result[strings::params][strings::protocol_version] =
      (*command_msg)[strings::params][strings::protocol_version];

  VIGetVehicleDataResponsePtr command(
      CreateCommand<VIGetVehicleDataResponse>(command_msg));

  am::event_engine::Event event(
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData);
  event.set_smart_object(result);

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));

  command->Run();
}

}  // namespace vi_get_vehicle_data_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
