/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_STATE_CONTROLLER_INCLUDE_APPLICATION_MANAGER_MOCK_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_STATE_CONTROLLER_INCLUDE_APPLICATION_MANAGER_MOCK_H_
#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "application_manager/application_manager.h"
#include "application_manager/usage_statistics.h"
#include "application_manager/commands/command.h"
#include "media_manager/media_manager.h"
#include "resumption/last_state.h"
#include "application_manager/policies/policy_handler.h"
namespace test {
namespace components {
namespace state_controller_test {
namespace am = application_manager;

class ApplicationManagerMock : public application_manager::ApplicationManager {
 public:
  MOCK_METHOD1(Init, bool(resumption::LastState& last_state));
  MOCK_METHOD0(Stop, bool());

  MOCK_METHOD1(set_hmi_message_handler,
               void(hmi_message_handler::HMIMessageHandler*));
  MOCK_METHOD1(set_protocol_handler, void(protocol_handler::ProtocolHandler*));
  MOCK_METHOD1(set_connection_handler,
               void(connection_handler::ConnectionHandler*));
  MOCK_CONST_METHOD0(applications, DataAccessor<am::ApplicationSet>());
  MOCK_CONST_METHOD1(application, am::ApplicationSharedPtr(uint32_t app_id));
  MOCK_CONST_METHOD1(application_by_hmi_app,
                     am::ApplicationSharedPtr(uint32_t));
  MOCK_CONST_METHOD0(active_application, am::ApplicationSharedPtr());
  MOCK_CONST_METHOD1(application_by_policy_id,
                     am::ApplicationSharedPtr(const std::string&));
  MOCK_METHOD1(applications_by_button,
               std::vector<am::ApplicationSharedPtr>(uint32_t));
  MOCK_METHOD0(applications_with_navi, std::vector<am::ApplicationSharedPtr>());
  MOCK_CONST_METHOD0(get_limited_media_application, am::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_navi_application, am::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_voice_application, am::ApplicationSharedPtr());
  MOCK_METHOD1(application_id, const uint32_t(const int32_t));
  MOCK_METHOD2(set_application_id, void(const int32_t, const uint32_t));
  MOCK_METHOD3(OnHMILevelChanged,
               void(uint32_t,
                    mobile_apis::HMILevel::eType,
                    mobile_apis::HMILevel::eType));
  MOCK_METHOD1(SendHMIStatusNotification, void(const am::ApplicationSharedPtr));
  MOCK_CONST_METHOD1(
      GetDefaultHmiLevel,
      mobile_apis::HMILevel::eType(am::ApplicationConstSharedPtr));
  MOCK_METHOD0(hmi_capabilities, am::HMICapabilities&());
  MOCK_METHOD0(is_attenuated_supported, bool());
  MOCK_CONST_METHOD1(IsAppTypeExistsInFullOrLimited,
                     bool(am::ApplicationConstSharedPtr));
  MOCK_METHOD1(OnApplicationRegistered, void(am::ApplicationSharedPtr));
  MOCK_METHOD2(SendMessageToMobile,
               void(const smart_objects::SmartObjectSPtr message,
                    bool final_message));
  MOCK_METHOD1(SendMessageToMobile,
               void(const smart_objects::SmartObjectSPtr message));
  MOCK_METHOD1(SendMessageToHMI,
               void(const smart_objects::SmartObjectSPtr message));
  MOCK_METHOD2(ManageMobileCommand,
               bool(const smart_objects::SmartObjectSPtr message,
                    am::commands::Command::CommandOrigin origin));
  MOCK_METHOD1(ManageHMICommand,
               bool(const smart_objects::SmartObjectSPtr message));
  MOCK_CONST_METHOD2(CanAppStream, bool (uint32_t app_id,
                                         protocol_handler::ServiceType service_type));
  MOCK_METHOD1(ForbidStreaming, void (uint32_t app_id));
  MOCK_METHOD2(SendAudioPassThroughNotification, void (uint32_t session_key,
                                                       std::vector<uint8_t>& binary_data));
  MOCK_CONST_METHOD0(connection_handler,
                     connection_handler::ConnectionHandler&());
};
}  // namespace state_controller_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_STATE_CONTROLLER_INCLUDE_APPLICATION_MANAGER_MOCK_H_
