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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_H_

#include <string>
#include <vector>

#include "gmock/gmock.h"

#include "application_manager/application_manager.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_manager_settings.h"
#include "application_manager/commands/command.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/vehicle_info_data.h"
#include "application_manager/state_controller.h"
#include "resumption/last_state.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/app_launch/app_launch_ctrl.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/state_controller.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockApplicationManager : public application_manager::ApplicationManager {
 public:
  MOCK_METHOD2(Init,
               bool(resumption::LastState& last_state,
                    media_manager::MediaManager* media_manager));
  MOCK_METHOD0(Stop, bool());
  MOCK_METHOD1(set_hmi_message_handler,
               void(hmi_message_handler::HMIMessageHandler* handler));
  MOCK_METHOD1(set_protocol_handler,
               void(protocol_handler::ProtocolHandler* handler));
  MOCK_METHOD1(set_connection_handler,
               void(connection_handler::ConnectionHandler* handler));
  MOCK_CONST_METHOD0(applications,
                     DataAccessor<application_manager::ApplicationSet>());
  MOCK_CONST_METHOD1(
      application, application_manager::ApplicationSharedPtr(uint32_t app_id));
  MOCK_CONST_METHOD0(active_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD1(
      application_by_hmi_app,
      application_manager::ApplicationSharedPtr(uint32_t hmi_app_id));
  MOCK_CONST_METHOD1(application_by_policy_id,
                     application_manager::ApplicationSharedPtr(
                         const std::string& policy_app_id));
  MOCK_METHOD1(
      applications_by_button,
      std::vector<application_manager::ApplicationSharedPtr>(uint32_t button));
  MOCK_METHOD0(applications_with_navi,
               std::vector<application_manager::ApplicationSharedPtr>());
  MOCK_CONST_METHOD0(get_limited_media_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_navi_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_voice_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_METHOD1(application_id, uint32_t(const int32_t correlation_id));
  MOCK_METHOD2(set_application_id,
               void(const int32_t correlation_id, const uint32_t app_id));
  MOCK_METHOD3(OnHMILevelChanged,
               void(uint32_t app_id,
                    mobile_apis::HMILevel::eType from,
                    mobile_apis::HMILevel::eType to));
  MOCK_METHOD1(
      SendHMIStatusNotification,
      void(const utils::SharedPtr<application_manager::Application> app));
  MOCK_METHOD2(SendMessageToMobile,
               void(application_manager::commands::MessageSharedPtr, bool));
  MOCK_METHOD1(
      SendMessageToHMI,
      void(const application_manager::commands::MessageSharedPtr message));
  MOCK_METHOD1(
      ManageHMICommand,
      bool(const application_manager::commands::MessageSharedPtr message));
  MOCK_METHOD2(
      ManageMobileCommand,
      bool(const application_manager::commands::MessageSharedPtr message,
           application_manager::commands::Command::CommandOrigin origin));
  MOCK_CONST_METHOD1(
      GetDefaultHmiLevel,
      mobile_apis::HMILevel::eType(
          application_manager::ApplicationConstSharedPtr application));
  MOCK_METHOD0(hmi_capabilities, application_manager::HMICapabilities&());
  MOCK_CONST_METHOD0(hmi_capabilities,
                     const application_manager::HMICapabilities&());
  MOCK_METHOD2(ProcessQueryApp,
               void(const smart_objects::SmartObject& sm_object,
                    const uint32_t connection_key));
  MOCK_CONST_METHOD0(is_attenuated_supported, bool());
  MOCK_CONST_METHOD1(IsAppTypeExistsInFullOrLimited,
                     bool(application_manager::ApplicationConstSharedPtr app));
  MOCK_METHOD1(OnApplicationRegistered,
               void(application_manager::ApplicationSharedPtr app));
  MOCK_CONST_METHOD0(connection_handler,
                     connection_handler::ConnectionHandler&());
  MOCK_CONST_METHOD0(protocol_handler, protocol_handler::ProtocolHandler&());
  MOCK_METHOD0(GetPolicyHandler, policy::PolicyHandlerInterface&());
  MOCK_METHOD0(GetNextHMICorrelationID, uint32_t());
  MOCK_METHOD0(GenerateNewHMIAppID, uint32_t());
  MOCK_METHOD1(EndNaviServices, void(uint32_t app_id));
  MOCK_METHOD0(BeginAudioPassThrough, bool());
  MOCK_METHOD0(EndAudioPassThrough, bool());
  MOCK_METHOD1(ConnectToDevice, void(const std::string& device_mac));
  MOCK_METHOD0(OnHMIStartedCooperation, void());
  MOCK_CONST_METHOD0(IsHMICooperating, bool());
  MOCK_METHOD2(IviInfoUpdated,
               std::vector<application_manager::ApplicationSharedPtr>(
                   application_manager::VehicleDataType vehicle_info,
                   int value));
  MOCK_METHOD1(RegisterApplication,
               application_manager::ApplicationSharedPtr(const utils::SharedPtr<
                   smart_objects::SmartObject>& request_for_registration));
  MOCK_METHOD0(SendUpdateAppList, void());
  MOCK_METHOD2(MarkAppsGreyOut,
               void(const connection_handler::DeviceHandle handle,
                    bool is_greyed_out));
  MOCK_CONST_METHOD1(IsAppsQueriedFrom,
                     bool(const connection_handler::DeviceHandle handle));
  MOCK_CONST_METHOD0(IsStopping, bool());
  MOCK_METHOD1(RemoveAppFromTTSGlobalPropertiesList,
               void(const uint32_t app_id));
  MOCK_METHOD4(
      SaveBinary,
      mobile_apis::Result::eType(const std::vector<uint8_t>& binary_data,
                                 const std::string& file_path,
                                 const std::string& file_name,
                                 const int64_t offset));
  MOCK_METHOD1(SetAllAppsAllowed, void(const bool allowed));
  MOCK_METHOD1(set_driver_distraction, void(bool is_distracting));
  MOCK_METHOD6(StartAudioPassThruThread,
               void(int32_t session_key,
                    int32_t correlation_id,
                    int32_t max_duration,
                    int32_t sampling_rate,
                    int32_t bits_per_sample,
                    int32_t audio_type));
  MOCK_METHOD0(StartDevicesDiscovery, void());
  MOCK_METHOD1(StopAudioPassThru, void(int32_t application_key));
  MOCK_METHOD3(TerminateRequest,
               void(const uint32_t connection_key,
                    const uint32_t corr_id,
                    const int32_t function_id));
  MOCK_METHOD4(UnregisterApplication,
               void(const uint32_t&, mobile_apis::Result::eType, bool, bool));
  MOCK_METHOD3(updateRequestTimeout,
               void(uint32_t connection_key,
                    uint32_t mobile_correlation_id,
                    uint32_t new_timeout_value));
  MOCK_METHOD0(state_controller, application_manager::StateController&());
  MOCK_METHOD1(SetUnregisterAllApplicationsReason,
               void(mobile_apis::AppInterfaceUnregisteredReason::eType reason));
  MOCK_METHOD1(HeadUnitReset,
               void(mobile_apis::AppInterfaceUnregisteredReason::eType reason));
  MOCK_CONST_METHOD2(HMILevelAllowsStreaming,
                     bool(uint32_t app_id,
                          protocol_handler::ServiceType service_type));
  MOCK_METHOD5(CheckPolicyPermissions,
               mobile_apis::Result::eType(
                   const std::string&,
                   mobile_apis::HMILevel::eType,
                   mobile_apis::FunctionID::eType,
                   const application_manager::RPCParams&,
                   application_manager::CommandParametersPermissions*));
  MOCK_CONST_METHOD2(IsApplicationForbidden,
                     bool(uint32_t connection_key,
                          const std::string& policy_app_id));
  MOCK_METHOD0(resume_controller, resumption::ResumeCtrl&());
  MOCK_METHOD0(hmi_interfaces, application_manager::HmiInterfaces&());
  MOCK_METHOD0(app_launch_ctrl, app_launch::AppLaunchCtrl&());
  MOCK_METHOD1(
      GetDeviceTransportType,
      hmi_apis::Common_TransportType::eType(const std::string& transport_type));
  MOCK_METHOD1(AddAppToTTSGlobalPropertiesList, void(const uint32_t app_id));
  MOCK_METHOD0(GenerateGrammarID, uint32_t());
  MOCK_CONST_METHOD1(GetUserConsentForDevice,
                     policy::DeviceConsent(const std::string& device_id));
  MOCK_METHOD1(OnAppUnauthorized, void(const uint32_t& app_id));
  MOCK_METHOD1(ActivateApplication,
               bool(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD3(OnAppStreaming,
               void(uint32_t app_id,
                    protocol_handler::ServiceType service_type,
                    bool state));
  MOCK_CONST_METHOD4(CreateRegularState,
                     application_manager::HmiStatePtr(
                         uint32_t app_id,
                         mobile_apis::HMILevel::eType hmi_level,
                         mobile_apis::AudioStreamingState::eType audio_state,
                         mobile_apis::SystemContext::eType system_context));
  MOCK_METHOD2(SendAudioPassThroughNotification,
               void(uint32_t session_key, std::vector<uint8_t>& binary_data));
  MOCK_CONST_METHOD2(CanAppStream,
                     bool(uint32_t app_id,
                          protocol_handler::ServiceType service_type));
  MOCK_METHOD1(ForbidStreaming, void(uint32_t app_id));
  MOCK_CONST_METHOD0(get_settings,
                     const application_manager::ApplicationManagerSettings&());
  MOCK_METHOD0(event_dispatcher,
               application_manager::event_engine::EventDispatcher&());

  MOCK_CONST_METHOD1(IsAppSubscribedForWayPoints, bool(const uint32_t));
  MOCK_METHOD1(SubscribeAppForWayPoints, void(const uint32_t));
  MOCK_METHOD1(UnsubscribeAppFromWayPoints, void(const uint32_t));
  MOCK_CONST_METHOD0(IsAnyAppSubscribedForWayPoints, bool());
  MOCK_CONST_METHOD0(GetAppsSubscribedForWayPoints, const std::set<int32_t>());
  MOCK_CONST_METHOD1(
      WaitingApplicationByID,
      application_manager::ApplicationConstSharedPtr(const uint32_t));
  MOCK_CONST_METHOD0(
      AppsWaitingForRegistration,
      DataAccessor<application_manager::AppsWaitRegistrationSet>());
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_H_
