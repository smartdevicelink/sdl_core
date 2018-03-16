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

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_

#include <string>
#include <vector>
#include <set>
#include "application_manager/application.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/commands/command.h"
#include "connection_handler/connection_handler.h"
#include "utils/data_accessor.h"
#include "utils/shared_ptr.h"
#include "telemetry_monitor/telemetry_observable.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/application_manager_settings.h"
#include "application_manager/state_controller.h"
#include "application_manager/hmi_interfaces.h"
#include "policy/policy_types.h"
#ifdef SDL_REMOTE_CONTROL
#include "functional_module/plugin_manager.h"
#endif

namespace resumption {
class LastState;
}

namespace app_launch {
class AppLaunchCtrl;
}  // namespace app_launch
namespace media_manager {
class MediaManager;
}

// Other compomnents class declaration
namespace hmi_message_handler {
class HMIMessageHandler;
}
namespace protocol_handler {
class ProtocolHandler;
}
namespace connection_handler {
class ConnectionHandler;
}
namespace resumption {
class ResumeCtrl;
}

namespace application_manager {

namespace event_engine {
class EventDispatcher;
}

class Application;
class StateControllerImpl;
struct CommandParametersPermissions;
using policy::RPCParams;
typedef std::vector<ApplicationSharedPtr> AppSharedPtrs;
struct ApplicationsAppIdSorter {
  bool operator()(const ApplicationSharedPtr lhs,
                  const ApplicationSharedPtr rhs) const {
    return lhs->app_id() < rhs->app_id();
  }
};

struct ApplicationsPolicyAppIdSorter {
  bool operator()(const ApplicationSharedPtr lhs,
                  const ApplicationSharedPtr rhs) {
    if (lhs->policy_app_id() == rhs->policy_app_id()) {
      return lhs->device() < rhs->device();
    }
    return lhs->policy_app_id() < rhs->policy_app_id();
  }
};

typedef std::set<ApplicationSharedPtr, ApplicationsAppIdSorter> ApplicationSet;

typedef std::set<ApplicationSharedPtr, ApplicationsPolicyAppIdSorter>
    AppsWaitRegistrationSet;

// typedef for Applications list iterator
typedef ApplicationSet::iterator ApplicationSetIt;

// typedef for Applications list const iterator
typedef ApplicationSet::const_iterator ApplicationSetConstIt;

class ApplicationManager {
 public:
  virtual ~ApplicationManager() {}

  /**
   * Inits application manager
   */
  virtual bool Init(resumption::LastState& last_state,
                    media_manager::MediaManager* media_manager) = 0;

  /**
   * @brief Stop work.
   *
   * @return TRUE on success otherwise FALSE.
   **/
  virtual bool Stop() = 0;

  virtual void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler) = 0;
  virtual void set_protocol_handler(
      protocol_handler::ProtocolHandler* handler) = 0;
  virtual void set_connection_handler(
      connection_handler::ConnectionHandler* handler) = 0;

  virtual DataAccessor<ApplicationSet> applications() const = 0;

  virtual ApplicationSharedPtr application(uint32_t app_id) const = 0;
  virtual ApplicationSharedPtr active_application() const = 0;

  /**
   * Function used only by HMI request/response/notification base classes
   * to change HMI app id to Mobile app id and vice versa.
   * Don't use it inside Core
   */
  virtual ApplicationSharedPtr application_by_hmi_app(
      uint32_t hmi_app_id) const = 0;

  virtual ApplicationSharedPtr application_by_policy_id(
      const std::string& policy_app_id) const = 0;

  virtual AppSharedPtrs applications_by_button(uint32_t button) = 0;
  virtual AppSharedPtrs applications_with_navi() = 0;

#ifdef SDL_REMOTE_CONTROL
  /**
 * @brief application find application by device and policy identifier
 * @param device_id device id
 * @param policy_app_id poilcy identifier
 * @return pointer to application in case if application exist, in other case
 * return empty shared pointer
 */
  virtual ApplicationSharedPtr application(
      const std::string& device_id, const std::string& policy_app_id) const = 0;

  virtual void ChangeAppsHMILevel(uint32_t app_id,
                                  mobile_apis::HMILevel::eType level) = 0;

  virtual std::vector<std::string> devices(
      const std::string& policy_app_id) const = 0;

  virtual void SendPostMessageToMobile(const MessagePtr& message) = 0;

  virtual void SendPostMessageToHMI(const MessagePtr& message) = 0;

  virtual functional_modules::PluginManager& GetPluginManager() = 0;
#endif  // SDL_REMOTE_CONTROL

  virtual std::vector<ApplicationSharedPtr>
  applications_with_mobile_projection() = 0;
  /**
   * @brief Returns media application with LIMITED HMI Level if exists
   *
   * @return Shared pointer to application if application does not
   * exist returns empty shared pointer.
   */
  virtual ApplicationSharedPtr get_limited_media_application() const = 0;

  /**
   * @brief Returns navigation application with LIMITED HMI Level if exists
   *
   * @return Shared pointer to application if application does not
   * exist returns empty shared pointer
   */
  virtual ApplicationSharedPtr get_limited_navi_application() const = 0;

  /**
   * @brief Returns voice communication application with
   * LIMITED HMI Level if exists
   *
   * @return Shared pointer to application if application does not
   * exist returns empty shared pointer
   */
  virtual ApplicationSharedPtr get_limited_voice_application() const = 0;

  virtual ApplicationSharedPtr get_limited_mobile_projection_application()
      const = 0;

  /**
   * @brief Retrieves application id associated with correlation id
   *
   * @param correlation_id Correlation ID of the HMI request
   *
   * @return application id associated with correlation id
   */
  virtual uint32_t application_id(const int32_t correlation_id) = 0;

  /**
   * @brief Sets application id correlation id
   *
   * @param correlation_id Correlation ID of the HMI request
   * @param app_id Application ID
   */
  virtual void set_application_id(const int32_t correlation_id,
                                  const uint32_t app_id) = 0;

  /**
   * @brief OnHMILevelChanged the callback that allows SDL to react when
   * application's HMI level has been changed.
   *
   * @param app_id application identifier for which HMILevel has been chaned.
   *
   * @param from previous HMILevel.
   * @param to current HMILevel.
   */
  virtual void OnHMILevelChanged(uint32_t app_id,
                                 mobile_apis::HMILevel::eType from,
                                 mobile_apis::HMILevel::eType to) = 0;

  /**
   * @brief Sends HMI status notification to mobile
   *
   * @param application_impl application with changed HMI status
   *
   **/
  virtual void SendHMIStatusNotification(
      const utils::SharedPtr<Application> app) = 0;

  /**
   * DEPRECATED
   * @brief Checks if Application is subscribed for way points
   * @param Application AppID
   * @return true if Application is subscribed for way points
   * otherwise false
   */
  virtual bool IsAppSubscribedForWayPoints(const uint32_t app_id) const = 0;

  /**
   * DEPRECATED
   * @brief Subscribe Application for way points
   * @param Application AppID
   */
  virtual void SubscribeAppForWayPoints(const uint32_t app_id) = 0;

  /**
   * DEPRECATED
   * @brief Unsubscribe Application for way points
   * @param Application AppID
   */
  virtual void UnsubscribeAppFromWayPoints(const uint32_t app_id) = 0;

  /**
   * @brief Checks if Application is subscribed for way points
   * @param Application pointer
   * @return true if Application is subscribed for way points
   * otherwise false
   */
  virtual bool IsAppSubscribedForWayPoints(ApplicationSharedPtr app) const = 0;

  /**
   * @brief Subscribe Application for way points
   * @param Application pointer
   */
  virtual void SubscribeAppForWayPoints(ApplicationSharedPtr app) = 0;

  /**
   * @brief Unsubscribe Application for way points
   * @param Application pointer
   */
  virtual void UnsubscribeAppFromWayPoints(ApplicationSharedPtr app) = 0;

  /**
   * @brief Is Any Application is subscribed for way points
   * @return true if some app is subscribed otherwise false
   */
  virtual bool IsAnyAppSubscribedForWayPoints() const = 0;

  /**
   * @brief Get subscribed for way points
   * @return reference to set of subscribed apps for way points
   */
  virtual const std::set<int32_t> GetAppsSubscribedForWayPoints() const = 0;

  virtual void SendMessageToMobile(const commands::MessageSharedPtr message,
                                   bool final_message = false) = 0;

  virtual void SendMessageToHMI(const commands::MessageSharedPtr message) = 0;

  virtual void RemoveHMIFakeParameters(
      application_manager::MessagePtr& message) = 0;

  virtual bool ManageHMICommand(const commands::MessageSharedPtr message) = 0;
  virtual bool ManageMobileCommand(const commands::MessageSharedPtr message,
                                   commands::Command::CommandOrigin origin) = 0;

  virtual MessageValidationResult ValidateMessageBySchema(
      const Message& message) = 0;

  virtual mobile_api::HMILevel::eType GetDefaultHmiLevel(
      ApplicationConstSharedPtr application) const = 0;
  /**
   * @brief hmi_capabilities return capabilities of hmi
   * @return capabilities of hmi
   */
  virtual HMICapabilities& hmi_capabilities() = 0;

  virtual const HMICapabilities& hmi_capabilities() const = 0;

  virtual void ProcessQueryApp(const smart_objects::SmartObject& sm_object,
                               const uint32_t connection_key) = 0;

  /**
   * @brief ProcessReconnection handles reconnection flow for application on
   * transport switch
   * @param application Pointer to switched application, must be validated
   * before passing
   * @param connection_key Connection key from registration request of switched
   * application
   */
  virtual void ProcessReconnection(ApplicationSharedPtr application,
                                   const uint32_t connection_key) = 0;

  virtual bool is_attenuated_supported() const = 0;

  /**
   * @brief Checks if application with the same HMI type
   *        (media, voice communication or navi) exists
   *        in HMI_FULL or HMI_LIMITED level.
   *
   * @param app Pointer to application to compare with
   *
   * @return true if exist otherwise false
   */
  virtual bool IsAppTypeExistsInFullOrLimited(
      ApplicationConstSharedPtr app) const = 0;

  /**
   * @brief Sets default HMI level and configure application after its
   * registration
   * @param app Application
   */
  virtual void OnApplicationRegistered(ApplicationSharedPtr app) = 0;

  /**
   * @brief OnApplicationSwitched starts activies postponed during application
   * transport switching
   * @param app Application
   */
  virtual void OnApplicationSwitched(ApplicationSharedPtr app) = 0;

  virtual connection_handler::ConnectionHandler& connection_handler() const = 0;
  virtual protocol_handler::ProtocolHandler& protocol_handler() const = 0;
  virtual policy::PolicyHandlerInterface& GetPolicyHandler() = 0;
  virtual const policy::PolicyHandlerInterface& GetPolicyHandler() const = 0;

  virtual uint32_t GetNextHMICorrelationID() = 0;
  virtual uint32_t GenerateNewHMIAppID() = 0;

  /**
   * @brief Ends opened navi services (audio/video) for application
   * @param app_id Application id
   */
  virtual void EndNaviServices(uint32_t app_id) = 0;

  /* @brief Starts audio passthru process
   *
   * @return true on success, false if passthru is already in process
   */
  virtual bool BeginAudioPassThrough() = 0;

  /*
   * @brief Finishes already started audio passthru process
   *
   * @return true on success, false if passthru is not active
   */
  virtual bool EndAudioPassThrough() = 0;

  virtual void ConnectToDevice(const std::string& device_mac) = 0;

  virtual void OnHMIStartedCooperation() = 0;

  virtual bool IsHMICooperating() const = 0;
  /**
   * @brief Notifies all components interested in Vehicle Data update
   * i.e. new value of odometer etc and returns list of applications
   * subscribed for event.
   * @param vehicle_info Enum value of type of vehicle data
   * @param new value (for integer values currently) of vehicle data
   */
  virtual AppSharedPtrs IviInfoUpdated(
      mobile_apis::VehicleDataType::eType vehicle_info, int value) = 0;

  virtual ApplicationSharedPtr RegisterApplication(const utils::SharedPtr<
      smart_objects::SmartObject>& request_for_registration) = 0;

  virtual void SendUpdateAppList() = 0;

  virtual void MarkAppsGreyOut(const connection_handler::DeviceHandle handle,
                               bool is_greyed_out) = 0;

  /**
   * @brief Returns pointer to application-to-be-registered (from QUERY_APP
   * list)
   * @param hmi_id HMI application id
   * @return Pointer to application or uninitialized shared pointer
   */
  virtual ApplicationConstSharedPtr WaitingApplicationByID(
      const uint32_t hmi_id) const = 0;

  /**
   * @brief Returns list of applications-to-be-registered (QUERY_APP list)
   * @return Locked list of applications
   */
  virtual DataAccessor<AppsWaitRegistrationSet> AppsWaitingForRegistration()
      const = 0;

  virtual bool IsAppsQueriedFrom(
      const connection_handler::DeviceHandle handle) const = 0;

  virtual bool IsStopping() const = 0;

  virtual void RemoveAppFromTTSGlobalPropertiesList(const uint32_t app_id) = 0;

  virtual mobile_apis::Result::eType SaveBinary(
      const std::vector<uint8_t>& binary_data,
      const std::string& file_path,
      const std::string& file_name,
      const int64_t offset) = 0;
  /*
   * @brief Sets SDL access to all mobile apps
   *
   * @param allowed SDL access to all mobile apps
   */
  virtual void SetAllAppsAllowed(const bool allowed) = 0;

  /*
   * @brief Sets state for driver distraction
   *
   * @param state New state to be set
   */
  virtual void set_driver_distraction(bool is_distracting) = 0;

  /*
   * @brief Starts audio pass thru thread
   *
   * @param session_key     Session key of connection for Mobile side
   * @param correlation_id  Correlation id for response for Mobile side
   * @param max_duration    Max duration of audio recording in milliseconds
   * @param sampling_rate   Value for rate(8, 16, 22, 44 kHz)
   * @param bits_per_sample The quality the audio is recorded.
   * @param audio_type      Type of audio data
   */
  virtual void StartAudioPassThruThread(int32_t session_key,
                                        int32_t correlation_id,
                                        int32_t max_duration,
                                        int32_t sampling_rate,
                                        int32_t bits_per_sample,
                                        int32_t audio_type) = 0;

  virtual void StartDevicesDiscovery() = 0;

  virtual void StopAudioPassThru(int32_t application_key) = 0;

  /**
   * @brief TerminateRequest forces termination of request
   * @param connection_key - application id of request
   * @param corr_id correlation id of request
   * @param function_id function id of request
   */
  virtual void TerminateRequest(const uint32_t connection_key,
                                const uint32_t corr_id,
                                const int32_t function_id) = 0;

  /*
   * @brief Closes application by id
   *
   * @param app_id Application id
   * @param reason reason of unregistering application
   * @param is_resuming describes - is this unregister
   *        is normal or need to be resumed\
   * @param is_unexpected_disconnect
   * Indicates if connection was unexpectedly lost(TM layer, HB)
   */
  virtual void UnregisterApplication(const uint32_t& app_id,
                                     mobile_apis::Result::eType reason,
                                     bool is_resuming = false,
                                     bool is_unexpected_disconnect = false) = 0;

  /**
   * @ Updates request timeout
   *
   * @param connection_key Connection key of application
   * @param mobile_correlation_id Correlation ID of the mobile request
   * @param new_timeout_value New timeout in milliseconds to be set
   */
  virtual void updateRequestTimeout(uint32_t connection_key,
                                    uint32_t mobile_correlation_id,
                                    uint32_t new_timeout_value) = 0;

  virtual StateController& state_controller() = 0;

  virtual void SetUnregisterAllApplicationsReason(
      mobile_api::AppInterfaceUnregisteredReason::eType reason) = 0;

  /*
   * @brief Called on Master_reset or Factory_defaults
   * when User chooses to reset HU.
   * Resets Policy Table if applicable.
   */
  virtual void HeadUnitReset(
      mobile_api::AppInterfaceUnregisteredReason::eType reason) = 0;

  /**
   * @brief Checks HMI level and returns true if streaming is allowed
   * @param app_id Application id
   * @param service_type Service type to check
   * @return True if streaming is allowed, false in other case
   */
  virtual bool HMILevelAllowsStreaming(
      uint32_t app_id, protocol_handler::ServiceType service_type) const = 0;

  /**
   * @brief Checks, if given RPC is allowed at current HMI level for specific
   * application in policy table
   * @param policy_app_id Application id
   * @param hmi_level Current HMI level of application
   * @param function_id FunctionID of RPC
   * @param params_permissions Permissions for RPC parameters (e.g.
   * SubscribeVehicleData) defined in policy table
   * @return SUCCESS, if allowed, otherwise result code of check
   */
  virtual mobile_apis::Result::eType CheckPolicyPermissions(
      const ApplicationSharedPtr app,
      const std::string& function_id,
      const RPCParams& rpc_params,
      CommandParametersPermissions* params_permissions = NULL) = 0;

  /**
   * @brief IsApplicationForbidden allows to distinguish if application is
   * not allowed to register, because of spamming.
   *
   * @param connection_key the connection key ofthe required application
   *
   * @param policy_app_id application's mobile(policy) identifier.
   *
   * @return true in case application is allowed to register, false otherwise.
   */
  virtual bool IsApplicationForbidden(
      uint32_t connection_key, const std::string& policy_app_id) const = 0;

  /**
   * @brief IsAppInReconnectMode check if application belongs to session
   * affected by transport switching at the moment
   * @param policy_app_id Application id
   * @return True if application is registered within session being switched,
   * otherwise - false
   */
  virtual bool IsAppInReconnectMode(const std::string& policy_app_id) const = 0;

  virtual resumption::ResumeCtrl& resume_controller() = 0;

  /**
  * @brief hmi_interfaces getter for hmi_interfaces component, that handle
  * hmi_instrfaces state
  * @return reference to hmi_interfaces component
  */
  virtual HmiInterfaces& hmi_interfaces() = 0;

  virtual app_launch::AppLaunchCtrl& app_launch_ctrl() = 0;

  /*
   * @brief Converts connection string transport type representation
   * to HMI Common_TransportType
   *
   * @param transport_type String representing connection type
   *
   * @return Corresponding HMI TransporType value
   */
  virtual hmi_apis::Common_TransportType::eType GetDeviceTransportType(
      const std::string& transport_type) = 0;

  /**
   * @brief method adds application
   * to tts_global_properties_app_list_
   * @param app_id contains application which will
   * send TTS global properties after timeout
   */
  virtual void AddAppToTTSGlobalPropertiesList(const uint32_t app_id) = 0;

  /**
  * Generate grammar ID
  *
  * @return New grammar ID
  */
  virtual uint32_t GenerateGrammarID() = 0;

  virtual policy::DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const = 0;

  /**
  * @brief Handle sequence for unauthorized application
  * @param app_id Application id
  */
  virtual void OnAppUnauthorized(const uint32_t& app_id) = 0;

  virtual bool ActivateApplication(ApplicationSharedPtr app) = 0;

  /**
   * @brief Callback calls when application starts/stops data streaming
   * @param app_id Streaming application id
   * @param service_type Streaming service type
   * @param state Shows if streaming started or stopped
   */
  virtual void OnAppStreaming(uint32_t app_id,
                              protocol_handler::ServiceType service_type,
                              bool state) = 0;

  /**
   * @brief CreateRegularState create regular HMI state for application
   * @param app Application
   * @param hmi_level of returned state
   * @param audio_state of returned state
   * @param system_context of returned state
   * @return new regular HMI state
   */
  virtual HmiStatePtr CreateRegularState(
      utils::SharedPtr<Application> app,
      mobile_apis::HMILevel::eType hmi_level,
      mobile_apis::AudioStreamingState::eType audio_state,
      mobile_apis::VideoStreamingState::eType video_state,
      mobile_apis::SystemContext::eType system_context) const = 0;

  /**
   * DEPRECATED
   * @brief CreateRegularState create regular HMI state for application
   * @param app_id Application id
   * @param hmi_level of returned state
   * @param audio_state of returned state
   * @param system_context of returned state
   * @return new regular HMI state
   */
  virtual HmiStatePtr CreateRegularState(
      uint32_t app_id,
      mobile_apis::HMILevel::eType hmi_level,
      mobile_apis::AudioStreamingState::eType audio_state,
      mobile_apis::SystemContext::eType system_context) const = 0;

  virtual void SendAudioPassThroughNotification(
      uint32_t session_key, std::vector<uint8_t>& binary_data) = 0;

  /**
   * @brief Checks if application can stream (streaming service is started and
   * streaming is enabled in application)
   * @param app_id Application id
   * @param service_type Service type to check
   * @return True if streaming is allowed, false in other case
   */
  virtual bool CanAppStream(
      uint32_t app_id, protocol_handler::ServiceType service_type) const = 0;

  /**
   * @brief ForbidStreaming forbid the stream over the certain application.
   * @param app_id the application's id which should stop streaming.
   */
  virtual void ForbidStreaming(uint32_t app_id) = 0;

  /**
   * @brief Called when application completes streaming configuration
   * @param app_id Streaming application id
   * @param service_type Streaming service type
   * @param result true if configuration is successful, false otherwise
   * @param rejected_params list of rejected parameters' name. Valid
   *                        only when result is false.
   */
  virtual void OnStreamingConfigured(
      uint32_t app_id,
      protocol_handler::ServiceType service_type,
      bool result,
      std::vector<std::string>& rejected_params) = 0;

  virtual const ApplicationManagerSettings& get_settings() const = 0;

  virtual event_engine::EventDispatcher& event_dispatcher() = 0;

  virtual uint32_t GetAvailableSpaceForApp(const std::string& folder_name) = 0;
  virtual void OnTimerSendTTSGlobalProperties() = 0;
  virtual void OnLowVoltage() = 0;
  virtual void OnWakeUp() = 0;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_
