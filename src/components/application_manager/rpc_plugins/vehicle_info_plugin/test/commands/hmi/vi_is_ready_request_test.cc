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

#include "hmi/vi_is_ready_request.h"

#include <memory>
#include <set>

#include "gtest/gtest.h"

#include "application_manager/event_engine/event.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "vehicle_info_plugin/commands/vi_command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace vi_is_ready_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::commands::RequestToHMI;
using am::event_engine::Event;
using vehicle_info_plugin::commands::VIIsReadyRequest;

typedef std::shared_ptr<VIIsReadyRequest> VIIsReadyRequestPtr;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class VIIsReadyRequestTest
    : public VICommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  VIIsReadyRequestTest() : command_(CreateCommandVI<VIIsReadyRequest>()) {}

  void SetUpExpectations(bool is_vi_cooperating_available,
                         bool should_message_be_sent,
                         bool message_contains_param,
                         am::HmiInterfaces::InterfaceState state) {
    EXPECT_CALL(mock_hmi_capabilities_,
                set_is_ivi_cooperating(is_vi_cooperating_available));

    if (message_contains_param) {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillRepeatedly(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(mock_hmi_interfaces_,
                  SetInterfaceState(
                      am::HmiInterfaces::HMI_INTERFACE_VehicleInfo, state));
    } else {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillOnce(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(mock_hmi_interfaces_, SetInterfaceState(_, _)).Times(0);
    }
    EXPECT_CALL(mock_policy_handler_, OnVIIsReady());

    EXPECT_CALL(mock_hmi_interfaces_,
                GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VehicleInfo))
        .WillOnce(Return(state));

    if (should_message_be_sent) {
      ExpectSendMessagesToHMI();
    }
  }

  void ExpectSendMessagesToHMI() {
    smart_objects::SmartObjectSPtr ivi_type;
    EXPECT_CALL(
        mock_message_helper_,
        CreateModuleInfoSO(hmi_apis::FunctionID::VehicleInfo_GetVehicleType, _))
        .WillOnce(Return(ivi_type));
    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(ivi_type, _));
  }

  void PrepareEvent(bool message_contains_param,
                    bool is_vi_cooperating_available,
                    Event& out_event) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    if (message_contains_param) {
      (*msg)[am::strings::msg_params][am::strings::available] =
          is_vi_cooperating_available;
    }
    out_event.set_smart_object(*msg);
  }

  void HMICapabilitiesExpectations() {
    EXPECT_CALL(mock_hmi_capabilities_,
                IsRequestsRequiredForCapabilities(
                    hmi_apis::FunctionID::VehicleInfo_GetVehicleType))
        .WillOnce(Return(true));
  }

  VIIsReadyRequestPtr command_;
};

TEST_F(VIIsReadyRequestTest, RUN_SendRequest_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  VIIsReadyRequestPtr command = CreateCommandVI<VIIsReadyRequest>(command_msg);
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(command_msg));
  ASSERT_TRUE(command->Init());

  command->Run();

  EXPECT_EQ(CommandImpl::hmi_protocol_type_,
            (*command_msg)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(CommandImpl::protocol_version_,
            (*command_msg)[strings::params][strings::protocol_version].asInt());
}

TEST_F(VIIsReadyRequestTest, Run_NoKeyAvailableInMessage_HmiInterfacesIgnored) {
  const bool is_vi_cooperating_available = false;
  const bool should_message_be_sent = true;
  const bool message_contains_param = false;
  const auto state = am::HmiInterfaces::STATE_NOT_RESPONSE;
  Event event(hmi_apis::FunctionID::VehicleInfo_IsReady);
  PrepareEvent(message_contains_param, is_vi_cooperating_available, event);
  HMICapabilitiesExpectations();
  SetUpExpectations(is_vi_cooperating_available,
                    should_message_be_sent,
                    message_contains_param,
                    state);
  ASSERT_TRUE(command_->Init());

  command_->Run();
  command_->on_event(event);
}

TEST_F(VIIsReadyRequestTest, Run_KeyAvailableEqualToFalse_StateNotAvailable) {
  const bool is_vi_cooperating_available = false;
  const bool should_message_be_sent = false;
  const bool message_contains_param = true;
  const auto state = am::HmiInterfaces::STATE_NOT_AVAILABLE;
  Event event(hmi_apis::FunctionID::VehicleInfo_IsReady);
  PrepareEvent(message_contains_param, is_vi_cooperating_available, event);
  SetUpExpectations(is_vi_cooperating_available,
                    should_message_be_sent,
                    message_contains_param,
                    state);
  ASSERT_TRUE(command_->Init());

  command_->Run();
  command_->on_event(event);
}

TEST_F(VIIsReadyRequestTest, Run_KeyAvailableEqualToTrue_StateAvailable) {
  const bool is_vi_cooperating_available = true;
  const bool should_message_be_sent = true;
  const bool message_contains_param = true;
  const auto state = am::HmiInterfaces::STATE_AVAILABLE;
  Event event(hmi_apis::FunctionID::VehicleInfo_IsReady);
  HMICapabilitiesExpectations();
  PrepareEvent(message_contains_param, is_vi_cooperating_available, event);
  SetUpExpectations(is_vi_cooperating_available,
                    should_message_be_sent,
                    message_contains_param,
                    state);
  ASSERT_TRUE(command_->Init());

  command_->Run();
  command_->on_event(event);
}

TEST_F(VIIsReadyRequestTest, Run_HMIDoestRespond_SendMessageToHMIByTimeout) {
  HMICapabilitiesExpectations();
  ExpectSendMessagesToHMI();
  ASSERT_TRUE(command_->Init());

  command_->Run();
  command_->OnTimeOut();
}

}  // namespace vi_is_ready_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
