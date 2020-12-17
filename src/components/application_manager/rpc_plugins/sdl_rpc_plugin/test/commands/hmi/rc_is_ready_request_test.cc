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

#include "hmi/rc_is_ready_request.h"

#include "gtest/gtest.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace rc_is_ready_request {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using sdl_rpc_plugin::commands::RCIsReadyRequest;
typedef std::shared_ptr<RCIsReadyRequest> RCIsReadyRequestPtr;

class RCIsReadyRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  RCIsReadyRequestTest() : command_(CreateCommand<RCIsReadyRequest>()) {}

  void SetUpExpectations(bool is_rc_cooperating_available,
                         bool is_send_message_to_hmi,
                         bool is_message_contain_param,
                         am::HmiInterfaces::InterfaceState state) {
    if (is_send_message_to_hmi) {
      ExpectSendMessagesToHMI();
    }
    EXPECT_CALL(mock_hmi_capabilities_,
                set_is_rc_cooperating(is_rc_cooperating_available));
    if (!is_rc_cooperating_available) {
      EXPECT_CALL(mock_hmi_capabilities_, set_rc_supported(false));
    }

    if (is_message_contain_param) {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillRepeatedly(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(
          mock_hmi_interfaces_,
          SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_RC, state));
    } else {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillOnce(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(mock_hmi_interfaces_, SetInterfaceState(_, _)).Times(0);
    }
    EXPECT_CALL(mock_hmi_interfaces_,
                GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_RC))
        .WillOnce(Return(state));
  }

  void ExpectSendMessagesToHMI() {
    smart_objects::SmartObjectSPtr capabilities(
        new smart_objects::SmartObject(smart_objects::SmartType_Map));
    EXPECT_CALL(mock_message_helper_,
                CreateModuleInfoSO(hmi_apis::FunctionID::RC_GetCapabilities, _))
        .WillOnce(Return(capabilities));
    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(capabilities, _));
  }

  MessageSharedPtr CreateResultMessage(
      bool is_rc_cooperating_available,
      hmi_apis::Common_Result::eType result_code =
          hmi_apis::Common_Result::INVALID_ENUM) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::msg_params][am::strings::available] =
        is_rc_cooperating_available;
    if (hmi_apis::Common_Result::INVALID_ENUM != result_code) {
      (*msg)[am::strings::params][am::hmi_response::code] = result_code;
    }
    return msg;
  }

  MessageSharedPtr CreateErrorMessage() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::error_msg] = true;
    return msg;
  }

  void HMICapabilitiesExpectations() {
    EXPECT_CALL(mock_hmi_capabilities_,
                IsRequestsRequiredForCapabilities(
                    hmi_apis::FunctionID::RC_GetCapabilities))
        .WillOnce(Return(true));
  }

  RCIsReadyRequestPtr command_;
};

TEST_F(RCIsReadyRequestTest, Run_NoKeyAvailableInMessage_HmiInterfacesIgnored) {
  const bool is_rc_cooperating_available = false;
  const bool is_send_message_to_hmi = true;
  const bool is_message_contain_param = false;
  Event event(hmi_apis::FunctionID::RC_IsReady);

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  event.set_smart_object(*msg);
  HMICapabilitiesExpectations();

  SetUpExpectations(is_rc_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_RESPONSE);
  command_->on_event(event);
}

TEST_F(RCIsReadyRequestTest, Run_KeyAvailableEqualToFalse_StateNotAvailable) {
  const bool is_rc_cooperating_available = false;
  const bool is_send_message_to_hmi = false;
  const bool is_message_contain_param = true;
  Event event(hmi_apis::FunctionID::RC_IsReady);
  MessageSharedPtr msg = CreateResultMessage(is_rc_cooperating_available);
  event.set_smart_object(*msg);
  SetUpExpectations(is_rc_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE);
  command_->on_event(event);
}

TEST_F(RCIsReadyRequestTest,
       Run_KeyAvailableEqualToTrueResultCodeSuccess_StateAvailable) {
  const bool is_rc_cooperating_available = true;
  const bool is_send_message_to_hmi = true;
  const bool is_message_contain_param = true;
  const hmi_apis::Common_Result::eType result_code =
      hmi_apis::Common_Result::SUCCESS;
  Event event(hmi_apis::FunctionID::RC_IsReady);

  MessageSharedPtr msg =
      CreateResultMessage(is_rc_cooperating_available, result_code);
  event.set_smart_object(*msg);
  HMICapabilitiesExpectations();

  SetUpExpectations(is_rc_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_AVAILABLE);
  command_->on_event(event);
}

TEST_F(RCIsReadyRequestTest,
       Run_KeyAvailableEqualToTrueResultCodeUnsuccess_StateNotAvailable) {
  const bool is_rc_cooperating_available = false;
  const bool is_send_message_to_hmi = false;
  const bool is_message_contain_param = true;
  const hmi_apis::Common_Result::eType result_code =
      hmi_apis::Common_Result::GENERIC_ERROR;
  Event event(hmi_apis::FunctionID::RC_IsReady);
  MessageSharedPtr msg =
      CreateResultMessage(is_rc_cooperating_available, result_code);
  event.set_smart_object(*msg);
  SetUpExpectations(is_rc_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE);
  command_->on_event(event);
}

TEST_F(RCIsReadyRequestTest, Run_ErrorMessage_StateNotAvailable) {
  const bool is_rc_cooperating_available = false;
  const bool is_send_message_to_hmi = false;
  const bool is_message_contain_param = true;
  Event event(hmi_apis::FunctionID::RC_IsReady);
  MessageSharedPtr msg = CreateErrorMessage();
  event.set_smart_object(*msg);
  SetUpExpectations(is_rc_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE);
  command_->on_event(event);
}

TEST_F(RCIsReadyRequestTest, Run_HMIDoestRespond_SendMessageToHMIByTimeout) {
  HMICapabilitiesExpectations();
  ExpectSendMessagesToHMI();
  command_->onTimeOut();
}

}  // namespace rc_is_ready_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
