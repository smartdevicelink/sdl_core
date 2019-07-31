/*
 * Copyright (c) 2019, Ford Motor Company
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

#include "hmi/vi_get_vehicle_data_request.h"

#include "gtest/gtest.h"

#include "smart_objects/enum_schema_item.h"
#include "smart_objects/smart_object.h"
#include "vehicle_info_plugin/commands/vi_command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace vi_get_vehicle_data_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

using vehicle_info_plugin::commands::VIGetVehicleDataRequest;

namespace {
const std::string kMobileParam = "mobile_param";
const std::string kHMIParam = "hmi_param";
}  // namespace

class VIGetVehicleDataRequestTest
    : public VICommandRequestTest<CommandsTestMocks::kIsNice> {};

MATCHER_P(EqualMsgParams, msg_params, "") {
  using namespace app_mngr;
  auto message = static_cast<smart_objects::SmartObject>(*arg);
  if (!message.keyExists(strings::msg_params)) {
    return false;
  }

  return msg_params == message[strings::msg_params];
}

TEST_F(VIGetVehicleDataRequestTest, Run_Success) {
  using namespace app_mngr;
  auto command_msg = CreateMessage(smart_objects::SmartType_Map);
  (*command_msg)[strings::msg_params][strings::rpm] = true;
  (*command_msg)[strings::msg_params][kMobileParam] = true;

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(
      strings::rpm, mobile_apis::VehicleDataType::VEHICLEDATA_RPM));
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  std::string oem_vehicle_data_type_str;
  smart_objects::EnumConversionHelper<mobile_apis::VehicleDataType::eType>::
      EnumToString(mobile_apis::VehicleDataType::VEHICLEDATA_OEM_CUSTOM_DATA,
                   &oem_vehicle_data_type_str);

  smart_objects::SmartObject hmi_params;
  hmi_params[kHMIParam] = smart_objects::SmartType_Map;
  hmi_params[kHMIParam][strings::data_type] = oem_vehicle_data_type_str;
  ON_CALL(mock_custom_vehicle_data_manager_, CreateHMIMessageParams(_))
      .WillByDefault(Return(hmi_params));

  smart_objects::SmartObject hmi_request_msg;
  hmi_request_msg[strings::rpm] =
      (*command_msg)[strings::msg_params][strings::rpm];
  hmi_request_msg[kHMIParam] = hmi_params[kHMIParam];

  EXPECT_CALL(mock_rpc_service_,
              SendMessageToHMI(EqualMsgParams(hmi_request_msg)));

  auto command = CreateCommandVI<VIGetVehicleDataRequest>(command_msg);
  command->Run();
}

}  // namespace vi_get_vehicle_data_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
