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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_MOCK_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_MOCK_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <set>

#include "gmock/gmock.h"

#include "application_manager/hmi_command_factory.h"
#include "application_manager/application_manager.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/message.h"
#include "application_manager/request_controller.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/vehicle_info_data.h"
#include "application_manager/state_controller.h"
#include "application_manager/commands/command.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/protocol_handler.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "hmi_message_handler/hmi_message_sender.h"
#include "application_manager/policies/policy_handler_observer.h"

#include "media_manager/media_manager_impl.h"

#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"

#include "formatters/CSmartFactory.h"

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"
#ifdef TELEMETRY_MONITOR
#include "telemetry_observer.h"
#endif  // TELEMETRY_MONITOR

#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/prioritized_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/lock.h"
#include "utils/singleton.h"
#include "utils/data_accessor.h"
#include "resumption/last_state.h"

namespace application_manager {
enum VRTTSSessionChanging { kVRSessionChanging = 0, kTTSSessionChanging };

namespace impl {

struct MessageFromMobile : public utils::SharedPtr<Message> {
  explicit MessageFromMobile(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

struct MessageToMobile : public utils::SharedPtr<Message> {
  explicit MessageToMobile(const utils::SharedPtr<Message>& message,
                           bool final_message)
      : utils::SharedPtr<Message>(message), is_final(final_message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
  // Signals if connection to mobile must be closed after sending this message
  bool is_final;
};

struct MessageFromHmi : public utils::SharedPtr<Message> {
  explicit MessageFromHmi(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

struct MessageToHmi : public utils::SharedPtr<Message> {
  explicit MessageToHmi(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageFromMobile>>
    FromMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToMobile>>
    ToMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageFromHmi>>
    FromHmiQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToHmi>>
    ToHmiQueue;

// AudioPassThru
typedef struct {
  std::vector<uint8_t> binary_data;
  int32_t session_key;
} AudioData;
typedef std::queue<AudioData> RawAudioDataQueue;
typedef threads::MessageLoopThread<RawAudioDataQueue> AudioPassThruQueue;
}
typedef std::vector<std::string> RPCParams;

class ApplicationManagerImpl
    : public ApplicationManager,
      public hmi_message_handler::HMIMessageObserver,
      public protocol_handler::ProtocolObserver,
      public policy::PolicyHandlerObserver,
      public connection_handler::ConnectionHandlerObserver,
      public impl::FromMobileQueue::Handler,
      public impl::ToMobileQueue::Handler,
      public impl::FromHmiQueue::Handler,
      public impl::ToHmiQueue::Handler,
      public impl::AudioPassThruQueue::Handler,
      public utils::Singleton<ApplicationManagerImpl> {
  friend class ResumeCtrl;
  friend class CommandImpl;

 public:
  ApplicationManagerImpl() {
    std::cout << "ApplicationManagerImpl Mock created \n";
  }

  MOCK_METHOD2(Init, bool(resumption::LastState& last_state,
                          media_manager::MediaManager* media_manager));
  MOCK_METHOD0(Stop, bool());

  // ApplicationManager methods
  MOCK_METHOD1(set_hmi_message_handler,
               void(hmi_message_handler::HMIMessageHandler*));
  MOCK_METHOD1(set_protocol_handler, void(protocol_handler::ProtocolHandler*));
  MOCK_METHOD1(set_connection_handler,
               void(connection_handler::ConnectionHandler*));
  MOCK_CONST_METHOD0(connection_handler,
                     connection_handler::ConnectionHandler&());

  MOCK_CONST_METHOD0(applications, DataAccessor<ApplicationSet>());

  MOCK_METHOD1(GetUserConsentForDevice,
               policy::DeviceConsent(const std::string& device_id));
  MOCK_METHOD1(OnMessageReceived,
               void(utils::SharedPtr<application_manager::Message>));
  MOCK_METHOD1(OnErrorSending,
               void(utils::SharedPtr<application_manager::Message>));
  MOCK_METHOD1(OnMessageReceived,
               void(const ::protocol_handler::RawMessagePtr));
  MOCK_METHOD1(OnMobileMessageSent,
               void(const ::protocol_handler::RawMessagePtr));
  MOCK_METHOD1(OnDeviceListUpdated, void(const connection_handler::DeviceMap&));
  MOCK_METHOD0(OnFindNewApplicationsRequest, void());
  MOCK_METHOD1(RemoveDevice, void(const connection_handler::DeviceHandle&));
  MOCK_METHOD3(OnServiceStartedCallback,
               bool(const connection_handler::DeviceHandle&,
                    const int32_t&,
                    const protocol_handler::ServiceType&));
  MOCK_METHOD3(OnServiceEndedCallback,
               void(const int32_t&,
                    const protocol_handler::ServiceType&,
                    const connection_handler::CloseSessionReason&));
#ifdef ENABLE_SECURITY
  MOCK_CONST_METHOD1(
      GetHandshakeContext,
      security_manager::SSLContext::HandshakeContext(uint32_t key));
  MOCK_METHOD2(OnHandshakeDone,
               bool(uint32_t connection_key,
                    security_manager::SSLContext::HandshakeResult result));
#endif  // ENABLE_SECURITY
  MOCK_METHOD1(Handle, void(const impl::MessageFromMobile));
  MOCK_METHOD1(Handle, void(const impl::MessageToMobile));
  MOCK_METHOD1(Handle, void(const impl::MessageFromHmi));
  MOCK_METHOD1(Handle, void(const impl::MessageToHmi));
  MOCK_METHOD1(Handle, void(const impl::AudioData));

//  // ApplicationManager methods
//  MOCK_METHOD1(set_hmi_message_handler,
//               void(hmi_message_handler::HMIMessageHandler*));
//  MOCK_METHOD1(set_protocol_handler, void(protocol_handler::ProtocolHandler*));
//  MOCK_METHOD1(set_connection_handler,
//               void(connection_handler::ConnectionHandler*));
//  MOCK_CONST_METHOD0(connection_handler,
//                     connection_handler::ConnectionHandler&());
//  MOCK_METHOD0(GetPolicyHandler,
//                     policy::PolicyHandlerInterface&());

// ApplicationManagerImpl methods:
#ifdef TELEMETRY_MONITOR
  MOCK_METHOD1(SetTelemetryObserver, void(AMTelemetryObserver*));
#endif
  MOCK_METHOD1(RegisterApplication,
               ApplicationSharedPtr(
                   const utils::SharedPtr<smart_objects::SmartObject>&));
  MOCK_METHOD0(hmi_capabilities, HMICapabilities&());
  MOCK_METHOD0(is_attenuated_supported, bool());
  MOCK_METHOD1(ManageHMICommand,
               bool(const application_manager::commands::MessageSharedPtr message));
  MOCK_METHOD2(ManageMobileCommand,
               bool(const smart_objects::SmartObjectSPtr message,
                    commands::Command::CommandOrigin));
  MOCK_METHOD1(SendMessageToHMI,
               void(const application_manager::commands::MessageSharedPtr));
  MOCK_METHOD2(SendMessageToMobile,
               void(const application_manager::commands::MessageSharedPtr, bool));
  MOCK_METHOD1(SendMessageToMobile,
               void(const application_manager::commands::MessageSharedPtr));
  MOCK_METHOD1(GetDeviceName, std::string(connection_handler::DeviceHandle));
  MOCK_METHOD1(GetDeviceTransportType,
               hmi_apis::Common_TransportType::eType(const std::string&));
  MOCK_CONST_METHOD1(application_by_policy_id,
                     ApplicationSharedPtr(const std::string&));

  MOCK_METHOD1(RemoveAppDataFromHMI, bool(ApplicationSharedPtr));
  MOCK_METHOD1(HeadUnitReset,
               void(mobile_api::AppInterfaceUnregisteredReason::eType));
  MOCK_METHOD1(LoadAppDataToHMI, bool(ApplicationSharedPtr));
  MOCK_METHOD1(ActivateApplication, bool(ApplicationSharedPtr));
  MOCK_METHOD1(IsHmiLevelFullAllowed,
               mobile_api::HMILevel::eType(ApplicationSharedPtr));
  MOCK_METHOD3(OnHMILevelChanged,
               void(uint32_t,
                    mobile_apis::HMILevel::eType,
                    mobile_apis::HMILevel::eType));
  MOCK_METHOD1(SendHMIStatusNotification,
               void(const utils::SharedPtr<Application>));

  MOCK_METHOD2(UnregisterRevokedApplication,
               void(uint32_t, mobile_apis::Result::eType));
  MOCK_METHOD1(SetUnregisterAllApplicationsReason,
               void(mobile_api::AppInterfaceUnregisteredReason::eType));
  MOCK_METHOD0(UnregisterAllApplications, void());
  MOCK_METHOD0(connection_handler, connection_handler::ConnectionHandler*());
  MOCK_METHOD0(protocol_handler, protocol_handler::ProtocolHandler*());
  MOCK_METHOD0(hmi_message_handler, hmi_message_handler::HMIMessageHandler*());
  MOCK_METHOD5(CheckPolicyPermissions,
               mobile_apis::Result::eType(const std::string&,
                                          mobile_apis::HMILevel::eType,
                                          mobile_apis::FunctionID::eType,
                                          const RPCParams&,
                                          CommandParametersPermissions*));
  MOCK_METHOD3(updateRequestTimeout, void(uint32_t, uint32_t, uint32_t));
  MOCK_METHOD0(GenerateGrammarID, uint32_t());
  MOCK_METHOD0(GenerateNewHMIAppID, uint32_t());
  MOCK_METHOD1(GetAvailableSpaceForApp, uint32_t(const std::string&));
  MOCK_METHOD0(begin_audio_pass_thru, bool());
  MOCK_METHOD0(end_audio_pass_thru, uint32_t());
  MOCK_METHOD1(StopAudioPassThru, void(uint32_t));
  MOCK_METHOD1(applications_by_button,
               std::vector<ApplicationSharedPtr>(uint32_t));
  MOCK_METHOD0(applications_with_navi, std::vector<ApplicationSharedPtr>());
  MOCK_METHOD2(IviInfoUpdated,
               std::vector<utils::SharedPtr<Application>>(VehicleDataType,
                                                          int));
  MOCK_METHOD6(
      StartAudioPassThruThread,
      void(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t));
  MOCK_METHOD4(SaveBinary,
               mobile_apis::Result::eType(const std::vector<uint8_t>&,
                                          const std::string&,
                                          const std::string&,
                                          const int64_t));
  MOCK_METHOD1(ReplaceHMIByMobileAppId, void(smart_objects::SmartObject&));
  MOCK_METHOD1(ReplaceMobileByHMIAppId, void(smart_objects::SmartObject&));
  MOCK_METHOD0(resume_controller, resumption::ResumeCtrl&());
  MOCK_METHOD1(GetDefaultHmiLevel,
               mobile_api::HMILevel::eType(ApplicationSharedPtr));

  MOCK_METHOD2(HMILevelAllowsStreaming,
               bool(uint32_t, protocol_handler::ServiceType));
  MOCK_CONST_METHOD2(CanAppStream, bool(uint32_t, protocol_handler::ServiceType));
  MOCK_METHOD1(EndNaviServices, void(int32_t));
  MOCK_METHOD1(ForbidStreaming, void(uint32_t));
  MOCK_METHOD3(OnAppStreaming,
               void(int32_t, protocol_handler::ServiceType, bool));

  MOCK_METHOD1(Unmute, void(VRTTSSessionChanging));
  MOCK_METHOD1(Mute, void(VRTTSSessionChanging));
  MOCK_METHOD2(set_application_id, void(const int32_t, const uint32_t));
  MOCK_METHOD1(application_id, const uint32_t(const int32_t));
  MOCK_METHOD1(removeNotification, void(const commands::Command*));
  MOCK_METHOD1(addNotification, void(const CommandSharedPtr));
  MOCK_METHOD0(StartDevicesDiscovery, void());
  MOCK_METHOD2(SendAudioPassThroughNotification,
               void(uint32_t, std::vector<uint8_t>&));
  MOCK_METHOD1(set_all_apps_allowed, void(const bool));
  MOCK_METHOD4(CreateRegularState,
               HmiStatePtr(uint32_t,
                           mobile_api::HMILevel::eType,
                           mobile_apis::AudioStreamingState::eType,
                           mobile_apis::SystemContext::eType));

  template <bool SendActivateApp>
  MOCK_METHOD2(SetState, void(uint32_t, HmiState));
  template <bool SendActivateApp>
  MOCK_METHOD2(SetState, void(uint32_t, mobile_api::HMILevel::eType));
  template <bool SendActivateApp>
  MOCK_METHOD3(SetState,
               void(uint32_t,
                    mobile_api::HMILevel::eType,
                    mobile_apis::AudioStreamingState::eType));

  template <bool SendActivateApp>
  MOCK_METHOD4(SetState,
               void(uint32_t,
                    mobile_api::HMILevel::eType,
                    mobile_apis::AudioStreamingState::eType,
                    mobile_apis::SystemContext::eType));
  MOCK_METHOD2(SetState,
               void(uint32_t, mobile_apis::AudioStreamingState::eType));

  MOCK_CONST_METHOD0(all_apps_allowed, bool());
  MOCK_METHOD1(set_vr_session_started, void(const bool));
  MOCK_CONST_METHOD0(vr_session_started, bool());
  MOCK_METHOD1(set_driver_distraction, void(const bool));
  MOCK_CONST_METHOD0(driver_distraction, bool());
  MOCK_METHOD1(ConnectToDevice, void(uint32_t));
  MOCK_METHOD0(OnHMIStartedCooperation, void());
  MOCK_METHOD0(GetNextHMICorrelationID, uint32_t());
  MOCK_METHOD0(OnTimerSendTTSGlobalProperties, void());
  MOCK_METHOD0(CreatePhoneCallAppList, void());
  MOCK_METHOD0(ResetPhoneCallAppList, void());
  MOCK_METHOD2(ChangeAppsHMILevel,
               void(uint32_t, mobile_apis::HMILevel::eType));
  MOCK_METHOD1(AddAppToTTSGlobalPropertiesList, void(const uint32_t));
  MOCK_METHOD1(RemoveAppFromTTSGlobalPropertiesList, void(const uint32_t));
  MOCK_CONST_METHOD1(application_by_hmi_app, ApplicationSharedPtr(uint32_t));
  MOCK_METHOD2(UnregisterApplication,
               void(const uint32_t, mobile_apis::Result::eType));
  MOCK_METHOD3(UnregisterApplication,
               void(const uint32_t, mobile_apis::Result::eType, bool));
  MOCK_METHOD4(UnregisterApplication,
               void(const uint32_t, mobile_apis::Result::eType, bool, bool));
  MOCK_METHOD1(OnAppUnauthorized, void(const uint32_t&));
  MOCK_CONST_METHOD0(get_limited_media_application, ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_navi_application, ApplicationSharedPtr());
  MOCK_CONST_METHOD0(get_limited_voice_application, ApplicationSharedPtr());
  MOCK_CONST_METHOD1(IsAppTypeExistsInFullOrLimited,
                     bool(ApplicationConstSharedPtr));
  MOCK_METHOD1(OnApplicationRegistered, void(ApplicationSharedPtr));
  MOCK_CONST_METHOD0(active_application, ApplicationSharedPtr());
  MOCK_METHOD0(OnApplicationListUpdateTimer, void());
  MOCK_METHOD0(OnLowVoltage, void());
  MOCK_METHOD0(OnWakeUp, void());
  MOCK_CONST_METHOD2(IsApplicationForbidden,
                     bool(uint32_t, const std::string&));
  MOCK_METHOD1(OnUpdateHMIAppType,
               void(std::map<std::string, std::vector<std::string>>));
  MOCK_METHOD3(set_state,
               void(ApplicationSharedPtr app,
                    mobile_apis::HMILevel::eType,
                    mobile_apis::AudioStreamingState::eType));
  MOCK_CONST_METHOD1(GetDefaultHmiLevel,
                     mobile_apis::HMILevel::eType(ApplicationConstSharedPtr));
  MOCK_CONST_METHOD1(application, ApplicationSharedPtr(uint32_t app_id));

  bool IsHMICooperating() const {
    return true;
  };

  /**
   * Class for thread-safe access to applications list
   */
  class ApplicationListAccessor : public DataAccessor<ApplicationSet> {
   public:
    ApplicationListAccessor()
        : DataAccessor<ApplicationSet>(ApplicationSet(),
                                       sync_primitives::Lock()) {}
    MOCK_CONST_METHOD0(applications, const ApplicationSet());
    MOCK_METHOD0(begin, ApplicationSetConstIt());
    MOCK_METHOD0(end, ApplicationSetConstIt());
    MOCK_METHOD1(Erase, void(ApplicationSharedPtr));
    MOCK_METHOD1(Insert, void(ApplicationSharedPtr));
    MOCK_METHOD0(Empty, bool());
  };

  friend class ApplicationListAccessor;

private:
  FRIEND_BASE_SINGLETON_CLASS(ApplicationManagerImpl);
};

}  // application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_MOCK_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_
