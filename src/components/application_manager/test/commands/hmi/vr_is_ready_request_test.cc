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

#include "application_manager/commands/hmi/vr_is_ready_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace vr_is_ready_request {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::commands::VRIsReadyRequest;
using am::MockMessageHelper;
using am::event_engine::Event;

typedef SharedPtr<VRIsReadyRequest> VRIsReadyRequestPtr;

class VRIsReadyRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  VRIsReadyRequestTest() : command_(CreateCommand<VRIsReadyRequest>()) {}

  void SetUpExpectations(bool is_vr_cooperating_available,
                         bool is_send_message_to_hmi,
                         bool is_message_contain_param,
                         am::HmiInterfaces::InterfaceState state) {
    const bool is_send_message_by_timeout = false;
    if (is_send_message_to_hmi) {
      EXPECT_CALL(app_mngr_, hmi_capabilities())
          .WillRepeatedly(ReturnRef(mock_hmi_capabilities_));
      ExpectSendMessagesToHMI(is_send_message_by_timeout);
    } else {
      EXPECT_CALL(app_mngr_, hmi_capabilities())
          .WillOnce(ReturnRef(mock_hmi_capabilities_));
    }
    EXPECT_CALL(mock_hmi_capabilities_,
                set_is_vr_cooperating(is_vr_cooperating_available));

    if (is_message_contain_param) {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillRepeatedly(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(
          mock_hmi_interfaces_,
          SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR, state));
    } else {
      EXPECT_CALL(app_mngr_, hmi_interfaces())
          .WillOnce(ReturnRef(mock_hmi_interfaces_));
      EXPECT_CALL(mock_hmi_interfaces_, SetInterfaceState(_, _)).Times(0);
    }
    EXPECT_CALL(mock_hmi_interfaces_,
                GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
        .WillOnce(Return(state));
  }

  void ExpectSendMessagesToHMI(bool is_send_message_by_timeout) {
    if (is_send_message_by_timeout) {
      EXPECT_CALL(app_mngr_, hmi_capabilities())
          .WillOnce(ReturnRef(mock_hmi_capabilities_));
    }

    smart_objects::SmartObjectSPtr language(
        new smart_objects::SmartObject(smart_objects::SmartType_Map));
    EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
                CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetLanguage, _))
        .WillOnce(Return(language));
    EXPECT_CALL(mock_hmi_capabilities_, set_handle_response_for(*language));
    EXPECT_CALL(app_mngr_, ManageHMICommand(language));

    smart_objects::SmartObjectSPtr support_language(
        new smart_objects::SmartObject(smart_objects::SmartType_Map));
    EXPECT_CALL(
        *(MockMessageHelper::message_helper_mock()),
        CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetSupportedLanguages, _))
        .WillOnce(Return(support_language));
    EXPECT_CALL(app_mngr_, ManageHMICommand(support_language));

    smart_objects::SmartObjectSPtr capabilities(
        new smart_objects::SmartObject(smart_objects::SmartType_Map));
    EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
                CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetCapabilities, _))
        .WillOnce(Return(capabilities));
    EXPECT_CALL(app_mngr_, ManageHMICommand(capabilities));
  }

  void PrepareEvent(bool is_message_contain_param,
                    Event& event,
                    bool is_vr_cooperating_available = false) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    if (is_message_contain_param) {
      (*msg)[am::strings::msg_params][am::strings::available] =
          is_vr_cooperating_available;
    }
    event.set_smart_object(*msg);
  }

  VRIsReadyRequestPtr command_;
  am::MockHmiInterfaces mock_hmi_interfaces_;
  application_manager_test::MockHMICapabilities mock_hmi_capabilities_;
};

TEST_F(VRIsReadyRequestTest, Run_NoKeyAvailableInMessage_HmiInterfacesIgnored) {
  const bool is_vr_cooperating_available = false;
  const bool is_send_message_to_hmi = true;
  const bool is_message_contain_param = false;
  Event event(hmi_apis::FunctionID::VR_IsReady);
  PrepareEvent(is_message_contain_param, event);
  SetUpExpectations(is_vr_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_RESPONSE);
  command_->on_event(event);
}

TEST_F(VRIsReadyRequestTest, Run_KeyAvailableEqualToFalse_StateNotAvailable) {
  const bool is_vr_cooperating_available = false;
  const bool is_send_message_to_hmi = false;
  const bool is_message_contain_param = true;
  Event event(hmi_apis::FunctionID::VR_IsReady);
  PrepareEvent(is_message_contain_param, event);
  SetUpExpectations(is_vr_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_NOT_AVAILABLE);
  command_->on_event(event);
}

TEST_F(VRIsReadyRequestTest, Run_KeyAvailableEqualToTrue_StateAvailable) {
  const bool is_vr_cooperating_available = true;
  const bool is_send_message_to_hmi = true;
  const bool is_message_contain_param = true;
  Event event(hmi_apis::FunctionID::VR_IsReady);
  PrepareEvent(is_message_contain_param, event, is_vr_cooperating_available);
  SetUpExpectations(is_vr_cooperating_available,
                    is_send_message_to_hmi,
                    is_message_contain_param,
                    am::HmiInterfaces::STATE_AVAILABLE);
  command_->on_event(event);
}

TEST_F(VRIsReadyRequestTest, Run_HMIDoestRespond_SendMessageToHMIByTimeout) {
  const bool is_send_message_by_timeout = true;
  ExpectSendMessagesToHMI(is_send_message_by_timeout);
  command_->onTimeOut();
}

}  // namespace vr_is_ready_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
