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
#include "interfaces/MOBILE_API.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/hmi/rc_get_capabilities_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace rc_get_capabilities_response {


using ::testing::NiceMock;
namespace am = ::application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;
using am::commands::RCGetCapabilitiesResponse;
using am::commands::CommandImpl;

typedef std::shared_ptr<RCGetCapabilitiesResponse> RCGetCapabilitiesResponsePtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class RCGetCapabilitiesResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateCommandMsg() {
    MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
    (*command_msg)[strings::msg_params][strings::number] = "123";
    (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*command_msg)[strings::params][hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;
    (*command_msg)[strings::msg_params][strings::rc_capability] =
        (capabilities_);

    return command_msg;
  }

  MockHMICapabilities mock_hmi_capabilities_;
  SmartObject capabilities_;
};

TEST_F(RCGetCapabilitiesResponseTest, RUN_SUCCESSS) {
  MessageSharedPtr command_msg = CreateCommandMsg();

  (*command_msg)[strings::msg_params][strings::system_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*command_msg)[strings::msg_params][strings::system_capabilities]
                [strings::rc_capability] =
                    smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& remote_control_capability =
      (*command_msg)[strings::msg_params][strings::system_capabilities]
                    [strings::rc_capability];

  remote_control_capability["climateControlCapabilities"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  remote_control_capability["climateControlCapabilities"][0] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject& climate_control_capability =
      remote_control_capability["climateControlCapabilities"][0];

  climate_control_capability["moduleName"] = "Climate";
  climate_control_capability["fanSpeedAvailable"] = true;
  climate_control_capability["desiredTemperatureAvailable"] = true;
  climate_control_capability["acEnableAvailable"] = true;
  climate_control_capability["acMaxEnableAvailable"] = true;
  climate_control_capability["circulateAirEnableAvailable"] = true;
  climate_control_capability["autoModeEnableAvailable"] = true;
  climate_control_capability["dualModeEnableAvailable"] = true;

  climate_control_capability["defrostZoneAvailable"] = true;
  climate_control_capability["defrostZone"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  climate_control_capability["defrostZone"][0] = "ALL";

  climate_control_capability["ventilationModeAvailable"] = true;
  climate_control_capability["ventilationMode"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  climate_control_capability["ventilationMode"][0] = "BOTH";

  remote_control_capability["radioControlCapabilities"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  remote_control_capability["radioControlCapabilities"][0] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject& radio_control_capability =
      remote_control_capability["radioControlCapabilities"][0];

  radio_control_capability["moduleName"] = "Radio";
  radio_control_capability["radioEnableAvailable"] = true;
  radio_control_capability["radioBandAvailable"] = true;
  radio_control_capability["radioFrequencyAvailable"] = true;
  radio_control_capability["hdChannelAvailable"] = true;
  radio_control_capability["rdsDataAvailable"] = true;
  radio_control_capability["availableHDsAvailable"] = true;
  radio_control_capability["stateAvailable"] = true;
  radio_control_capability["signalStrengthAvailable"] = true;
  radio_control_capability["signalChangeThresholdAvailable"] = true;

  remote_control_capability[hmi_response::button_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  remote_control_capability[hmi_response::button_capabilities][0] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject& button_capability =
      remote_control_capability[hmi_response::button_capabilities][0];

  button_capability[strings::button_name] = "OK";
  button_capability["shortPressAvailable"] = true;
  button_capability["longPressAvailable"] = true;
  button_capability["upDownAvailable"] = true;

  RCGetCapabilitiesResponsePtr command(
      CreateCommand<RCGetCapabilitiesResponse>(command_msg));

  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));

  smart_objects::SmartObject rc_capability_so =
      (*command_msg)[strings::msg_params][strings::rc_capability];

  EXPECT_CALL(mock_hmi_capabilities_, set_rc_capability(rc_capability_so));
  EXPECT_CALL(mock_hmi_capabilities_, set_rc_supported(true));

  command->Run();
}

}  // namespace rc_get_capabilities_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
