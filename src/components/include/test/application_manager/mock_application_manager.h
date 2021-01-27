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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_H_

#include <memory>
#include <string>
#include <vector>

#include "gmock/gmock.h"

#include "application_manager/app_launch/app_launch_ctrl.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager_settings.h"
#include "application_manager/command_factory.h"
#include "application_manager/commands/command.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/message.h"
#include "application_manager/message_helper.h"
#include "application_manager/plugin_manager/rpc_plugin_manager.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/rpc_handler.h"
#include "application_manager/rpc_service.h"
#include "application_manager/state_controller.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "resumption/last_state_wrapper.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace application_manager_test {
using application_manager::plugin_manager::RPCPluginManager;

class MockApplicationManager : public application_manager::ApplicationManager {
 public:
  MOCK_METHOD2(Init,
               bool(resumption::LastStateWrapperPtr last_state,
                    media_manager::MediaManager* media_manager));
  DEPRECATED
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
  MOCK_CONST_METHOD0(
      pending_applications,
      DataAccessor<application_manager::AppsWaitRegistrationSet>());
  MOCK_CONST_METHOD0(reregister_applications,
                     DataAccessor<application_manager::ReregisterWaitList>());
  MOCK_METHOD1(CreatePendingLocalApplication,
               void(const std::string& policy_app_id));
  MOCK_METHOD1(RemovePendingApplication,
               void(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(
      application, application_manager::ApplicationSharedPtr(uint32_t app_id));
  MOCK_CONST_METHOD0(active_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_full_or_limited_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD2(application,
                     application_manager::ApplicationSharedPtr(
                         const std::string& device_id,
                         const std::string& policy_app_id));
  MOCK_METHOD2(ChangeAppsHMILevel,
               void(uint32_t app_id, mobile_apis::HMILevel::eType level));
  MOCK_CONST_METHOD1(
      devices, std::vector<std::string>(const std::string& policy_app_id));
  MOCK_METHOD0(GetPluginManager, RPCPluginManager&());

  MOCK_METHOD1(SetPluginManager,
               void(std::unique_ptr<RPCPluginManager>& plugin_manager));

  MOCK_METHOD0(GetAppServiceManager, application_manager::AppServiceManager&());

  MOCK_CONST_METHOD1(
      application_by_hmi_app,
      application_manager::ApplicationSharedPtr(uint32_t hmi_app_id));
  MOCK_CONST_METHOD1(application_by_policy_id,
                     application_manager::ApplicationSharedPtr(
                         const std::string& policy_app_id));
  MOCK_CONST_METHOD1(
      application_by_name,
      application_manager::ApplicationSharedPtr(const std::string& app_name));
  MOCK_CONST_METHOD1(pending_application_by_policy_id,
                     application_manager::ApplicationSharedPtr(
                         const std::string& policy_app_id));
  MOCK_CONST_METHOD1(reregister_application_by_policy_id,
                     application_manager::ApplicationSharedPtr(
                         const std::string& policy_app_id));
  MOCK_CONST_METHOD1(applications_by_name,
                     std::vector<application_manager::ApplicationSharedPtr>(
                         const std::string& app_name));
  MOCK_METHOD1(
      applications_by_button,
      std::vector<application_manager::ApplicationSharedPtr>(uint32_t button));
  MOCK_METHOD0(applications_with_navi,
               std::vector<application_manager::ApplicationSharedPtr>());
  MOCK_METHOD0(applications_with_mobile_projection,
               std::vector<application_manager::ApplicationSharedPtr>());
  MOCK_CONST_METHOD0(get_limited_media_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_navi_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_voice_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_mobile_projection_application,
                     application_manager::ApplicationSharedPtr());
  MOCK_METHOD1(application_id, uint32_t(const int32_t correlation_id));
  MOCK_METHOD2(set_application_id,
               void(const int32_t correlation_id, const uint32_t app_id));
  MOCK_METHOD3(OnHMIStateChanged,
               void(const uint32_t app_id,
                    const application_manager::HmiStatePtr from,
                    const application_manager::HmiStatePtr to));
  MOCK_METHOD3(ProcessOnDataStreamingNotification,
               void(const protocol_handler::ServiceType service_type,
                    const uint32_t app_id,
                    const bool streaming_data_available));
  MOCK_METHOD1(SendDriverDistractionState,
               void(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD2(SendGetIconUrlNotifications,
               void(const uint32_t connection_key,
                    application_manager::ApplicationSharedPtr application));
  MOCK_METHOD2(RemoveHMIFakeParameters,
               void(application_manager::commands::MessageSharedPtr& message,
                    const hmi_apis::FunctionID::eType& function_id));
  MOCK_CONST_METHOD1(
      GetDefaultHmiLevel,
      mobile_apis::HMILevel::eType(
          application_manager::ApplicationConstSharedPtr application));
  MOCK_METHOD0(hmi_capabilities, application_manager::HMICapabilities&());
  MOCK_CONST_METHOD0(hmi_capabilities,
                     const application_manager::HMICapabilities&());
  MOCK_CONST_METHOD1(
      CheckResumptionRequiredTransportAvailable,
      bool(application_manager::ApplicationConstSharedPtr application));
  MOCK_METHOD2(ProcessQueryApp,
               void(const smart_objects::SmartObject& sm_object,
                    const uint32_t connection_key));
  MOCK_CONST_METHOD0(is_attenuated_supported, bool());
  MOCK_CONST_METHOD0(IsLowVoltage, bool());
  MOCK_CONST_METHOD1(IsAppTypeExistsInFullOrLimited,
                     bool(application_manager::ApplicationConstSharedPtr app));
  MOCK_METHOD1(OnApplicationRegistered,
               void(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD1(OnApplicationSwitched,
               void(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD0(mobile_v4_protocol_so_factory,
               ns_smart_device_link_rpc::V1::v4_protocol_v1_2_no_extra&());
  MOCK_CONST_METHOD0(connection_handler,
                     connection_handler::ConnectionHandler&());
  MOCK_CONST_METHOD0(protocol_handler, protocol_handler::ProtocolHandler&());
  MOCK_METHOD0(GetPolicyHandler, policy::PolicyHandlerInterface&());
  MOCK_CONST_METHOD0(GetPolicyHandler, const policy::PolicyHandlerInterface&());
  MOCK_CONST_METHOD0(GetRPCService,
                     application_manager::rpc_service::RPCService&());
  MOCK_CONST_METHOD0(GetRPCHandler,
                     application_manager::rpc_handler::RPCHandler&());
  MOCK_CONST_METHOD0(is_stopping, bool());
  MOCK_CONST_METHOD0(is_audio_pass_thru_active, bool());
  MOCK_METHOD0(GetNextHMICorrelationID, uint32_t());
  MOCK_METHOD0(GetNextMobileCorrelationID, uint32_t());
  MOCK_METHOD0(GenerateNewHMIAppID, uint32_t());
  MOCK_METHOD1(EndNaviServices, void(uint32_t app_id));
  MOCK_METHOD1(BeginAudioPassThru, bool(uint32_t app_id));
  MOCK_METHOD1(EndAudioPassThru, bool(uint32_t app_id));
  MOCK_METHOD1(ConnectToDevice, void(const std::string& device_mac));
  MOCK_METHOD0(OnHMIReady, void());
  MOCK_METHOD0(RequestForInterfacesAvailability, void());
  MOCK_METHOD1(DisconnectCloudApp,
               void(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD0(RefreshCloudAppInformation, void());
  MOCK_CONST_METHOD1(GetCloudAppConnectionStatus,
                     hmi_apis::Common_CloudConnectionStatus::eType(
                         application_manager::ApplicationConstSharedPtr app));
  MOCK_METHOD1(PolicyIDByIconUrl, std::string(const std::string url));
  MOCK_METHOD1(SetIconFileFromSystemRequest, void(const std::string policy_id));
  MOCK_CONST_METHOD0(IsHMICooperating, bool());
  MOCK_METHOD1(SetHMICooperating, void(const bool hmi_cooperating));
  MOCK_METHOD2(IviInfoUpdated,
               void(const std::string& vehicle_info, int value));
  MOCK_METHOD1(RegisterApplication,
               application_manager::ApplicationSharedPtr(
                   const std::shared_ptr<smart_objects::SmartObject>&
                       request_for_registration));
  MOCK_METHOD2(FinalizeAppRegistration,
               void(application_manager::ApplicationSharedPtr,
                    const uint32_t connection_key));
  MOCK_METHOD0(SendUpdateAppList, void());
  MOCK_METHOD2(MarkAppsGreyOut,
               void(const connection_handler::DeviceHandle handle,
                    bool is_greyed_out));
  MOCK_CONST_METHOD1(IsAppsQueriedFrom,
                     bool(const connection_handler::DeviceHandle handle));
  MOCK_CONST_METHOD0(IsStopping, bool());
  MOCK_METHOD1(RemoveAppFromTTSGlobalPropertiesList,
               void(const uint32_t app_id));
  MOCK_METHOD2(ResetGlobalProperties,
               application_manager::ResetGlobalPropertiesResult(
                   const smart_objects::SmartObject& global_properties_ids,
                   const uint32_t app_id));
  MOCK_METHOD1(
      ResetAllApplicationGlobalProperties,
      application_manager::ResetGlobalPropertiesResult(const uint32_t app_id));
  MOCK_METHOD4(
      SaveBinary,
      mobile_apis::Result::eType(const std::vector<uint8_t>& binary_data,
                                 const std::string& file_path,
                                 const std::string& file_name,
                                 const uint64_t offset));
  MOCK_METHOD1(SetAllAppsAllowed, void(const bool allowed));
  MOCK_METHOD1(
      set_driver_distraction_state,
      void(const hmi_apis::Common_DriverDistractionState::eType state));
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
  MOCK_METHOD1(OnQueryAppsRequest,
               void(const connection_handler::DeviceHandle));
  MOCK_METHOD4(UnregisterApplication,
               void(const uint32_t&, mobile_apis::Result::eType, bool, bool));
  MOCK_METHOD3(updateRequestTimeout,
               void(uint32_t connection_key,
                    uint32_t mobile_correlation_id,
                    uint32_t new_timeout_value));
  MOCK_METHOD2(IncreaseForwardedRequestTimeout,
               void(uint32_t connection_key, uint32_t mobile_correlation_id));
  MOCK_METHOD0(state_controller, application_manager::StateController&());
  MOCK_METHOD1(SetUnregisterAllApplicationsReason,
               void(mobile_apis::AppInterfaceUnregisteredReason::eType reason));
  MOCK_METHOD1(HeadUnitReset,
               void(mobile_apis::AppInterfaceUnregisteredReason::eType reason));
  MOCK_CONST_METHOD2(HMIStateAllowsStreaming,
                     bool(uint32_t app_id,
                          protocol_handler::ServiceType service_type));
  MOCK_METHOD5(CheckPolicyPermissions,
               mobile_apis::Result::eType(
                   const application_manager::ApplicationSharedPtr app,
                   const application_manager::WindowID window_id,
                   const std::string& function_id,
                   const application_manager::RPCParams& rpc_params,
                   application_manager::CommandParametersPermissions*
                       params_permissions));
  MOCK_CONST_METHOD2(IsApplicationForbidden,
                     bool(uint32_t connection_key,
                          const std::string& policy_app_id));
  MOCK_METHOD0(resume_controller, resumption::ResumeCtrl&());
  MOCK_METHOD0(hmi_interfaces, application_manager::HmiInterfaces&());
  MOCK_METHOD0(app_launch_ctrl, app_launch::AppLaunchCtrl&());
  MOCK_CONST_METHOD0(SupportedSDLVersion,
                     protocol_handler::MajorProtocolVersion());
  MOCK_METHOD1(
      ApplyFunctorForEachPlugin,
      void(std::function<void(application_manager::plugin_manager::RPCPlugin&)>
               functor));
  MOCK_METHOD1(SetVINCode, void(const std::string& vin_code));
  MOCK_CONST_METHOD0(GetVINCode, const std::string());
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
  MOCK_METHOD3(
      OnAppStreaming,
      void(uint32_t app_id,
           protocol_handler::ServiceType service_type,
           application_manager::Application::StreamingState new_state));
  MOCK_CONST_METHOD6(CreateRegularState,
                     application_manager::HmiStatePtr(
                         application_manager::ApplicationSharedPtr app,
                         const mobile_apis::WindowType::eType window_type,
                         mobile_apis::HMILevel::eType hmi_level,
                         mobile_apis::AudioStreamingState::eType audio_state,
                         mobile_apis::VideoStreamingState::eType video_state,
                         mobile_apis::SystemContext::eType system_context));

  MOCK_METHOD2(SendAudioPassThroughNotification,
               void(uint32_t session_key, std::vector<uint8_t>& binary_data));
  MOCK_CONST_METHOD2(CanAppStream,
                     bool(uint32_t app_id,
                          protocol_handler::ServiceType service_type));
  DEPRECATED
  MOCK_METHOD1(ForbidStreaming, void(uint32_t app_id));
  MOCK_METHOD2(ForbidStreaming,
               void(uint32_t app_id,
                    protocol_handler::ServiceType service_type));
  MOCK_CONST_METHOD0(get_settings,
                     const application_manager::ApplicationManagerSettings&());
  MOCK_CONST_METHOD1(
      GetCorrectMobileIDFromMessage,
      std::string(
          const application_manager::commands::MessageSharedPtr& message));
  MOCK_METHOD0(event_dispatcher,
               application_manager::event_engine::EventDispatcher&());

  MOCK_METHOD2(IsSOStructValid,
               bool(const hmi_apis::StructIdentifiers::eType struct_id,
                    const smart_objects::SmartObject& display_capabilities));
  MOCK_CONST_METHOD1(IsAppSubscribedForWayPoints, bool(uint32_t));
  MOCK_CONST_METHOD1(IsAppSubscribedForWayPoints,
                     bool(application_manager::Application& app));
  MOCK_METHOD2(SubscribeAppForWayPoints, void(uint32_t, bool));
  MOCK_METHOD2(SubscribeAppForWayPoints,
               void(application_manager::ApplicationSharedPtr, bool));
  MOCK_METHOD2(UnsubscribeAppFromWayPoints, void(uint32_t, bool));
  MOCK_METHOD2(UnsubscribeAppFromWayPoints,
               void(application_manager::ApplicationSharedPtr, bool));
  MOCK_CONST_METHOD0(IsSubscribedToHMIWayPoints, bool());
  MOCK_CONST_METHOD0(IsAnyAppSubscribedForWayPoints, bool());
  MOCK_CONST_METHOD0(GetAppsSubscribedForWayPoints, const std::set<uint32_t>());
  MOCK_METHOD2(SaveWayPointsMessage,
               void(std::shared_ptr<smart_objects::SmartObject>, uint32_t));
  MOCK_CONST_METHOD1(
      WaitingApplicationByID,
      application_manager::ApplicationConstSharedPtr(const uint32_t));
  MOCK_CONST_METHOD0(
      AppsWaitingForRegistration,
      DataAccessor<application_manager::AppsWaitRegistrationSet>());

  MOCK_METHOD1(ReplaceMobileWithHMIAppId,
               void(smart_objects::SmartObject& message));
  MOCK_METHOD1(ReplaceHMIWithMobileAppId,
               void(smart_objects::SmartObject& message));
  MOCK_METHOD1(GetAvailableSpaceForApp,
               uint32_t(const std::string& folder_name));
  MOCK_METHOD0(OnTimerSendTTSGlobalProperties, void());
  MOCK_METHOD0(OnLowVoltage, void());
  MOCK_METHOD0(OnWakeUp, void());
  MOCK_METHOD2(OnStreamingConfigurationSuccessful,
               void(uint32_t app_id,
                    protocol_handler::ServiceType service_type));
  MOCK_METHOD3(OnStreamingConfigurationFailed,
               void(uint32_t app_id,
                    std::vector<std::string>& rejected_params,
                    const std::string& reason));
  MOCK_METHOD2(ProcessReconnection,
               void(application_manager::ApplicationSharedPtr application,
                    const uint32_t connection_key));
  MOCK_CONST_METHOD2(IsAppInReconnectMode,
                     bool(const connection_handler::DeviceHandle& device_id,
                          const std::string& policy_app_id));
  MOCK_CONST_METHOD0(GetCommandFactory, application_manager::CommandFactory&());
  MOCK_CONST_METHOD0(get_current_audio_source, uint32_t());
  MOCK_METHOD1(set_current_audio_source, void(const uint32_t));
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_APPLICATION_MANAGER_H_
