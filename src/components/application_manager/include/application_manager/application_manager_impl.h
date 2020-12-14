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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_IMPL_H_

#include <stdint.h>
#include <algorithm>
#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "application_manager/app_launch/app_launch_data.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/application_manager_settings.h"
#include "application_manager/command_factory.h"
#include "application_manager/command_holder.h"
#include "application_manager/event_engine/event_dispatcher_impl.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/hmi_interfaces_impl.h"
#include "application_manager/message.h"
#include "application_manager/message_helper.h"
#include "application_manager/request_controller.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/rpc_handler.h"
#include "application_manager/rpc_service.h"
#include "application_manager/state_controller_impl.h"

#include "application_manager/rpc_handler.h"

#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/policies/policy_handler_observer.h"
#include "connection_handler/connection_handler.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "formatters/CSmartFactory.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "hmi_message_handler/hmi_message_sender.h"
#include "policies/policy_handler.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/service_status_update_handler_listener.h"

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager_listener.h"
#include "security_manager/ssl_context.h"
#endif  // ENABLE_SECURITY

#ifdef TELEMETRY_MONITOR
#include "telemetry_observer.h"
#endif  // TELEMETRY_MONITOR

#include "utils/macro.h"

#include "smart_objects/smart_object.h"
#include "utils/data_accessor.h"
#include "utils/lock.h"
#include "utils/message_queue.h"
#include "utils/prioritized_queue.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/threads/thread.h"
#include "utils/timer.h"

struct BsonObject;

namespace threads {
class Thread;
}
class CommandNotificationImpl;

namespace application_manager {
namespace mobile_api = mobile_apis;
using namespace utils;
using namespace timer;
namespace custom_str = custom_string;

class ApplicationManagerImpl;

enum VRTTSSessionChanging { kVRSessionChanging = 0, kTTSSessionChanging };

typedef std::map<protocol_handler::ServiceType, std::set<uint32_t> >
    ServiceStreamingStatusMap;

struct CommandParametersPermissions;
typedef std::map<std::string, hmi_apis::Common_TransportType::eType>
    DeviceTypes;

struct AppIconInfo {
  std::string endpoint;
  bool pending_request;
  AppIconInfo();
  AppIconInfo(std::string ws_endpoint, bool pending)
      : endpoint(ws_endpoint), pending_request(pending) {}
};

SDL_CREATE_LOG_VARIABLE("ApplicationManager")
typedef std::shared_ptr<timer::Timer> TimerSPtr;

class ApplicationManagerImpl
    : public ApplicationManager,
      public connection_handler::ConnectionHandlerObserver,
      public policy::PolicyHandlerObserver,
      public protocol_handler::ServiceStatusUpdateHandlerListener
#ifdef ENABLE_SECURITY
    ,
      public security_manager::SecurityManagerListener
#endif  // ENABLE_SECURITY
#ifdef TELEMETRY_MONITOR
    ,
      public telemetry_monitor::TelemetryObservable<AMTelemetryObserver>
#endif  // TELEMETRY_MONITOR
{

  friend class ResumeCtrl;
  friend class CommandImpl;

 public:
  ApplicationManagerImpl(const ApplicationManagerSettings& am_settings,
                         const policy::PolicySettings& policy_settings);
  ~ApplicationManagerImpl();

  DEPRECATED
  bool Init(resumption::LastState&,
            media_manager::MediaManager* media_manager) OVERRIDE;

  /**
   * Inits application manager
   */
  bool Init(resumption::LastStateWrapperPtr last_state_wrapper,
            media_manager::MediaManager* media_manager) OVERRIDE;

  /**
   * @brief Stop work.
   *
   * @return TRUE on success otherwise FALSE.
   **/
  bool Stop() OVERRIDE;

  DataAccessor<ApplicationSet> applications() const OVERRIDE;
  DataAccessor<AppsWaitRegistrationSet> pending_applications() const OVERRIDE;
  DataAccessor<ReregisterWaitList> reregister_applications() const OVERRIDE;
  ApplicationSharedPtr application(uint32_t app_id) const OVERRIDE;

  ApplicationSharedPtr active_application() const OVERRIDE;

  ApplicationSharedPtr get_full_or_limited_application() const OVERRIDE;

  ApplicationSharedPtr application_by_hmi_app(
      uint32_t hmi_app_id) const OVERRIDE;
  ApplicationSharedPtr application_by_policy_id(
      const std::string& policy_app_id) const OVERRIDE;
  DEPRECATED ApplicationSharedPtr
  application_by_name(const std::string& app_name) const OVERRIDE;
  ApplicationSharedPtr pending_application_by_policy_id(
      const std::string& policy_app_id) const OVERRIDE;
  ApplicationSharedPtr reregister_application_by_policy_id(
      const std::string& policy_app_id) const OVERRIDE;

  std::vector<ApplicationSharedPtr> applications_by_name(
      const std::string& app_name) const OVERRIDE;
  std::vector<ApplicationSharedPtr> applications_by_button(
      uint32_t button) OVERRIDE;
  std::vector<ApplicationSharedPtr> applications_with_navi() OVERRIDE;
  std::vector<ApplicationSharedPtr> applications_with_mobile_projection()
      OVERRIDE;

  ApplicationSharedPtr get_limited_media_application() const OVERRIDE;
  ApplicationSharedPtr get_limited_navi_application() const OVERRIDE;
  ApplicationSharedPtr get_limited_voice_application() const OVERRIDE;
  ApplicationSharedPtr get_limited_mobile_projection_application()
      const OVERRIDE;

  uint32_t application_id(const int32_t correlation_id) OVERRIDE;
  void set_application_id(const int32_t correlation_id,
                          const uint32_t app_id) OVERRIDE;

  uint32_t get_current_audio_source() const OVERRIDE;

  void set_current_audio_source(const uint32_t source) OVERRIDE;

  void OnHMIStateChanged(const uint32_t app_id,
                         const HmiStatePtr from,
                         const HmiStatePtr to) OVERRIDE;

  void ProcessOnDataStreamingNotification(
      const protocol_handler::ServiceType service_type,
      const uint32_t app_id,
      const bool streaming_data_available) FINAL;

  void SendDriverDistractionState(ApplicationSharedPtr application);

  void SendGetIconUrlNotifications(const uint32_t connection_key,
                                   ApplicationSharedPtr application);

  ApplicationSharedPtr application(
      const std::string& device_id,
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief ChangeAppsHMILevel the function that will change application's
   * hmi level.
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param level new hmi level for certain application.
   */
  void ChangeAppsHMILevel(uint32_t app_id, mobile_apis::HMILevel::eType level);

  virtual plugin_manager::RPCPluginManager& GetPluginManager() OVERRIDE {
    DCHECK(plugin_manager_);
    return *plugin_manager_;
  }

  virtual AppServiceManager& GetAppServiceManager() OVERRIDE {
    DCHECK(app_service_manager_);
    return *app_service_manager_;
  }

  std::vector<std::string> devices(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Checks if application with the same HMI type
   *        (media, voice communication or navi) exists
   *        in HMI_FULL or HMI_LIMITED level.
   *
   * @param app Pointer to application to compare with
   *
   * @return true if exist otherwise false
   */
  bool IsAppTypeExistsInFullOrLimited(ApplicationConstSharedPtr app) const;

  /**
   * @brief Checks if Application is subscribed for way points
   * @param Application id
   * @return true if Application is subscribed for way points
   * otherwise false
   */
  bool IsAppSubscribedForWayPoints(uint32_t app_id) const OVERRIDE;

  /**
   * @brief Checks if Application is subscribed for way points
   * @param Application reference
   * @return true if Application is subscribed for way points
   * otherwise false
   */
  bool IsAppSubscribedForWayPoints(Application& app) const OVERRIDE;

  void SaveWayPointsMessage(smart_objects::SmartObjectSPtr way_points_message,
                            uint32_t app_id = 0) OVERRIDE;

  void SubscribeAppForWayPoints(uint32_t app_id,
                                bool response_from_hmi = true) OVERRIDE;

  void SubscribeAppForWayPoints(ApplicationSharedPtr app,
                                bool response_from_hmi = true) OVERRIDE;

  void UnsubscribeAppFromWayPoints(uint32_t app_id,
                                   bool response_from_hmi = true) OVERRIDE;

  void UnsubscribeAppFromWayPoints(ApplicationSharedPtr app,
                                   bool response_from_hmi = true) OVERRIDE;

  bool IsSubscribedToHMIWayPoints() const OVERRIDE;

  /**
   * @brief Is Any Application is subscribed for way points
   * @return true if some app is subscribed otherwise false
   */
  bool IsAnyAppSubscribedForWayPoints() const OVERRIDE;

  /**
   * @brief Get subscribed for way points
   * @return reference to set of subscribed apps for way points
   */
  const std::set<uint32_t> GetAppsSubscribedForWayPoints() const OVERRIDE;

  /**
   * @brief Notifies all components interested in Vehicle Data update
   * i.e. new value of odometer etc and returns list of applications
   * subscribed for event.
   * @param vehicle_info Enum value of type of vehicle data
   * @param new value (for integer values currently) of vehicle data
   */
  void IviInfoUpdated(const std::string& vehicle_info, int value) OVERRIDE;

  void OnApplicationRegistered(ApplicationSharedPtr app) OVERRIDE;

  /**
   * @brief OnApplicationSwitched starts processing of commands collected
   * during device switching process
   * @param app Application
   */
  void OnApplicationSwitched(ApplicationSharedPtr app) OVERRIDE;

  HMICapabilities& hmi_capabilities() OVERRIDE;
  const HMICapabilities& hmi_capabilities() const OVERRIDE;

  /**
   * @brief ProcessQueryApp executes logic related to QUERY_APP system request.
   *
   * @param sm_object smart object wich is actually parsed json obtained within
   * system request.
   * @param connection_key connection key for app, which sent system request
   */
  void ProcessQueryApp(const smart_objects::SmartObject& sm_object,
                       const uint32_t connection_key) OVERRIDE;

  bool is_attenuated_supported() const OVERRIDE;

#ifdef TELEMETRY_MONITOR
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTelemetryObserver(AMTelemetryObserver* observer) OVERRIDE;
#endif  // TELEMETRY_MONITOR

  ApplicationSharedPtr RegisterApplication(
      const std::shared_ptr<smart_objects::SmartObject>&
          request_for_registration) OVERRIDE;

  void FinalizeAppRegistration(ApplicationSharedPtr application,
                               const uint32_t connection_key) OVERRIDE;
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
  void UnregisterApplication(const uint32_t& app_id,
                             mobile_apis::Result::eType reason,
                             bool is_resuming = false,
                             bool is_unexpected_disconnect = false) OVERRIDE;

  /**
   * @brief Handle sequence for unauthorized application
   * @param app_id Application id
   */
  void OnAppUnauthorized(const uint32_t& app_id) OVERRIDE;

  /*
   * @brief Sets unregister reason for closing all registered applications
   * duringHU switching off
   *
   * @param reason Describes the reason for HU switching off
   */
  void SetUnregisterAllApplicationsReason(
      mobile_api::AppInterfaceUnregisteredReason::eType reason) OVERRIDE;

  /*
   * @brief Called on Master_reset or Factory_defaults
   * when User chooses to reset HU.
   * Resets Policy Table if applicable.
   */
  void HeadUnitReset(
      mobile_api::AppInterfaceUnregisteredReason::eType reason) OVERRIDE;

  /*
   * @brief Closes all registered applications
   */
  void UnregisterAllApplications();

  bool ActivateApplication(ApplicationSharedPtr app) OVERRIDE;

  /**
   * @brief Put application in FULL HMI Level if possible,
   *        otherwise put applicatuion other HMI level.
   *        do not send any notifications to mobile
   * @param app, application, that need to be puted in FULL
   * @return seted HMI Level
   */
  mobile_api::HMILevel::eType IsHmiLevelFullAllowed(ApplicationSharedPtr app);

  void ConnectToDevice(const std::string& device_mac) OVERRIDE;

  void OnHMIReady() OVERRIDE;

  void RequestForInterfacesAvailability() OVERRIDE;

  void DisconnectCloudApp(ApplicationSharedPtr app) OVERRIDE;

  void RefreshCloudAppInformation() OVERRIDE;

  void CreatePendingApplication(
      const transport_manager::ConnectionUID connection_id,
      const transport_manager::DeviceInfo& device_info,
      connection_handler::DeviceHandle device_id) OVERRIDE;

  void OnWebEngineDeviceCreated() OVERRIDE;

  void CreatePendingLocalApplication(const std::string& policy_app_id) OVERRIDE;

  void RemovePendingApplication(const std::string& policy_app_id) OVERRIDE;

  void SetPendingApplicationState(
      const transport_manager::ConnectionUID connection_id,
      const transport_manager::DeviceInfo& device_info);

  std::string PolicyIDByIconUrl(const std::string url) OVERRIDE;

  void SetIconFileFromSystemRequest(const std::string policy_id) OVERRIDE;

  /**
   * @brief Notifies the applicaiton manager that a cloud connection status has
   * updated and should trigger an UpdateAppList RPC to the HMI
   */
  void OnConnectionStatusUpdated();

  /**
   * @brief Retrieve the current connection status of a cloud app
   * @param app A cloud application
   * @return The current CloudConnectionStatus of app
   */
  hmi_apis::Common_CloudConnectionStatus::eType GetCloudAppConnectionStatus(
      ApplicationConstSharedPtr app) const;

  /*
   * @brief Returns unique correlation ID for to mobile request
   *
   * @return Unique correlation ID
   */
  uint32_t GetNextMobileCorrelationID() OVERRIDE;

  /*
   * @brief Returns unique correlation ID for HMI request
   *
   * @return Unique correlation ID
   */
  uint32_t GetNextHMICorrelationID() OVERRIDE;

  /**
   * @brief Starts AudioPassThru process by given application
   * @param app_id ID of the application which starts the process
   * @return true if AudioPassThru can be started, false otherwise
   */
  bool BeginAudioPassThru(uint32_t app_id) OVERRIDE;

  /**
   * @brief Finishes already started AudioPassThru process by given application
   * @param app_id ID of the application which started the process
   * @return true if AudioPassThru process has been started with given
   * application and thus it can be stopped, false otherwise
   */
  bool EndAudioPassThru(uint32_t app_id) OVERRIDE;

  /*
   * @brief Retrieves driver distraction state
   *
   * @return Current state of the distraction state
   */
  hmi_apis::Common_DriverDistractionState::eType driver_distraction_state()
      const;

  /*
   * @brief Sets state for driver distraction
   *
   * @param state New state to be set
   */
  void set_driver_distraction_state(
      const hmi_apis::Common_DriverDistractionState::eType state) OVERRIDE;

  /*
   * @brief Retrieves SDL access to all mobile apps
   *
   * @return Currently active state of the access
   */
  inline bool all_apps_allowed() const;

  /*
   * @brief Sets SDL access to all mobile apps
   *
   * @param allowed SDL access to all mobile apps
   */
  void SetAllAppsAllowed(const bool allowed) OVERRIDE;

  /**
   * @brief CreateRegularState create regular HMI state for application
   * @param app Application
   * @param window_type type of window
   * @param hmi_level of returned state
   * @param audio_state of returned state
   * @param system_context of returned state
   * @return new regular HMI state
   */
  HmiStatePtr CreateRegularState(
      std::shared_ptr<Application> app,
      const mobile_apis::WindowType::eType window_type,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state,
      const mobile_apis::SystemContext::eType system_context) const OVERRIDE;

  /**
   * @brief Checks, if given RPC is allowed at current HMI level for specific
   * application in policy table
   * @param app Application
   * @param window_id id of application's window
   * @param function_id FunctionID of RPC
   * @param params_permissions Permissions for RPC parameters (e.g.
   * SubscribeVehicleData) defined in policy table
   * @return SUCCESS, if allowed, otherwise result code of check
   */
  mobile_apis::Result::eType CheckPolicyPermissions(
      const ApplicationSharedPtr app,
      const WindowID window_id,
      const std::string& function_id,
      const RPCParams& rpc_params,
      CommandParametersPermissions* params_permissions = NULL) OVERRIDE;

  /**
   * @brief IsApplicationForbidden allows to distinguish if application is
   * not allowed to register, becuase of spaming.
   *
   * @param connection_key the conection key ofthe required application
   *
   * @param mobile_app_id application's mobile(policy) identifier.
   *
   * @return true in case application is allowed to register, false otherwise.
   */
  bool IsApplicationForbidden(uint32_t connection_key,
                              const std::string& mobile_app_id);

  /**
   * @brief Notification from PolicyHandler about PTU.
   * Compares AppHMIType between saved in app and received from PTU. If they are
   * different method sends:
   * UI_ChangeRegistration with list new AppHMIType;
   * ActivateApp with HMI level BACKGROUND;
   * OnHMIStatus notification;
   * for app with HMI level FULL or LIMITED.
   * method sends:
   * UI_ChangeRegistration with list new AppHMIType
   * for app with HMI level BACKGROUND.
   */
  void OnUpdateHMIAppType(
      std::map<std::string, std::vector<std::string> > app_hmi_types) OVERRIDE;

  /**
   * @brief OnPTUFinished is called on policy table update coming
   * @param ptu_result True if PTU is succeeded, otherwise - false
   */
  void OnPTUFinished(const bool ptu_result) FINAL;

#if defined(EXTERNAL_PROPRIETARY_MODE) && defined(ENABLE_SECURITY)
  /**
   * @brief OnCertDecryptFailed is called when certificate decryption fails in
   * external flow
   * @return since this callback is a part of SecurityManagerListener, bool
   * return value is used to indicate whether listener instance can be deleted
   * by calling entity. if true - listener can be deleted and removed from
   * listeners by SecurityManager, false - listener retains its place within
   * SecurityManager.
   */
  bool OnCertDecryptFailed() FINAL;

  /**
   * @brief OnCertDecryptFinished is called when certificate decryption is
   * finished in the external flow
   * @param decrypt_result bool value indicating whether decryption was
   * successful
   */
  void OnCertDecryptFinished(const bool decrypt_result) FINAL;
#endif

  /**
   * @brief OnPTUTimeoutExceeded is called on policy table update timed out
   */
  void OnPTUTimeoutExceeded() FINAL;

  /**
   *@brief ProcessServiceStatusUpdate callback that is invoked in case of
   *service status update
   *@param connection_key - connection key
   *@param service_type enum value containing type of service.
   *@param service_event enum value containing event that occured during service
   *start.
   *@param service_update_reason enum value containing reason why service_event
   *occured.
   **/
  void ProcessServiceStatusUpdate(
      const uint32_t connection_key,
      hmi_apis::Common_ServiceType::eType service_type,
      hmi_apis::Common_ServiceEvent::eType service_event,
      utils::Optional<hmi_apis::Common_ServiceStatusUpdateReason::eType>
          service_update_reason) FINAL;

#ifdef ENABLE_SECURITY
  bool OnPTUFailed() FINAL;
#endif  // ENABLE_SECURITY
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
  void StartAudioPassThruThread(int32_t session_key,
                                int32_t correlation_id,
                                int32_t max_duration,
                                int32_t sampling_rate,
                                int32_t bits_per_sample,
                                int32_t audio_type) OVERRIDE;

  /*
   * @brief Terminates audio pass thru thread
   * @param application_key Id of application for which
   * audio pass thru should be stopped
   */
  void StopAudioPassThru(int32_t application_key) OVERRIDE;

  std::string GetDeviceName(connection_handler::DeviceHandle handle);

  /*
   * @brief Converts connection string transport type representation
   * to HMI Common_TransportType
   *
   * @param transport_type String representing connection type
   *
   * @return Corresponding HMI TransporType value
   */
  hmi_apis::Common_TransportType::eType GetDeviceTransportType(
      const std::string& transport_type);

  void set_hmi_message_handler(hmi_message_handler::HMIMessageHandler* handler);
  void set_connection_handler(connection_handler::ConnectionHandler* handler);
  void set_protocol_handler(protocol_handler::ProtocolHandler* handler);

  void StartDevicesDiscovery();

  void RemoveHMIFakeParameters(
      application_manager::commands::MessageSharedPtr& message,
      const hmi_apis::FunctionID::eType& function_id) OVERRIDE;

  /**
   * @brief TerminateRequest forces termination of request
   * @param connection_key - application id of request
   * @param corr_id correlation id of request
   * @param function_id function id of request
   */
  void TerminateRequest(const uint32_t connection_key,
                        const uint32_t corr_id,
                        const int32_t function_id) OVERRIDE;

  void OnQueryAppsRequest(
      const connection_handler::DeviceHandle device) OVERRIDE;

  // Overriden ConnectionHandlerObserver method
  void OnDeviceListUpdated(
      const connection_handler::DeviceMap& device_list) OVERRIDE;
  void OnFindNewApplicationsRequest() OVERRIDE;
  void RemoveDevice(
      const connection_handler::DeviceHandle& device_handle) OVERRIDE;

  /**
   * @brief OnDeviceSwitchingStart is invoked on device transport switching
   * start (e.g. from Bluetooth to USB) and creates waiting list of applications
   * expected to be re-registered after switching is complete
   * @param device_from device params being switched to the new transport
   * @param device_to device params on the new transport
   */
  void OnDeviceSwitchingStart(
      const connection_handler::Device& device_from,
      const connection_handler::Device& device_to) FINAL;

  /**
   * @brief OnDeviceSwitchingFinish is invoked on device trasport switching end
   * i.e. timeout for switching is expired, unregisters applications from
   * waiting list which haven't been re-registered and clears the waiting list
   * @param device_uid UID of device being switched
   */
  void OnDeviceSwitchingFinish(const std::string& device_uid) FINAL;

  void OnServiceStartedCallback(
      const connection_handler::DeviceHandle& device_handle,
      const int32_t& session_key,
      const protocol_handler::ServiceType& type,
      const BsonObject* params) OVERRIDE;
  void OnServiceEndedCallback(
      const int32_t& session_key,
      const protocol_handler::ServiceType& type,
      const connection_handler::CloseSessionReason& close_reason) OVERRIDE;
  void OnSecondaryTransportStartedCallback(
      const connection_handler::DeviceHandle device_handle,
      const int32_t session_key) OVERRIDE;
  void OnSecondaryTransportEndedCallback(const int32_t session_key) OVERRIDE;

  /**
   * @brief Check if application with specified app_id has NAVIGATION HMI type
   * @param app_id id of application to check
   * @return true if application is navi otherwise returns false
   */
  bool CheckAppIsNavi(const uint32_t app_id) const OVERRIDE;

#ifdef ENABLE_SECURITY
  /**
   * @brief Notification about protection result
   * @param connection_key Unique key of session which triggers handshake
   * @param result result of connection protection
   * @return true on success notification handling or false otherwise
   */
  bool OnHandshakeDone(
      uint32_t connection_key,
      security_manager::SSLContext::HandshakeResult result) OVERRIDE;

  /**
   * @brief Notification about handshake failure
   * @return true on success notification handling or false otherwise
   */
  bool OnGetSystemTimeFailed() OVERRIDE;

  /**
   * @brief Notification that certificate update is required.
   */
  void OnCertificateUpdateRequired() OVERRIDE;

  /**
   * @brief Get certificate data from policy
   * @param reference to string where to save certificate data
   * @return true if listener saved some data to string otherwise false
   */
  bool GetPolicyCertificateData(std::string& data) const OVERRIDE;

  /**
   * @brief Get unique handshake context by application id
   * @param key id of application
   * @return generated handshake context or empty context if application with
   * provided id does not exist
   */
  security_manager::SSLContext::HandshakeContext GetHandshakeContext(
      uint32_t key) const OVERRIDE;
#endif  // ENABLE_SECURITY

  /**
   * @ Add notification to collection
   *
   * @param ptr Reference to shared pointer that point on hmi notification
   */
  void addNotification(const CommandSharedPtr ptr);

  /**
   * @ Add notification to collection
   *
   * @param ptr Reference to shared pointer that point on hmi notification
   */
  void removeNotification(const commands::Command* notification);

  /**
   * @ Updates request timeout
   *
   * @param connection_key Connection key of application
   * @param mobile_correlation_id Correlation ID of the mobile request
   * @param new_timeout_value New timeout in milliseconds to be set
   */
  void updateRequestTimeout(uint32_t connection_key,
                            uint32_t mobile_correlation_id,
                            uint32_t new_timeout_value) OVERRIDE;

  /**
   * @brief TODO
   *
   * @param connection_key Connection key of application
   * @param mobile_correlation_id Correlation ID of the mobile request
   */
  void IncreaseForwardedRequestTimeout(uint32_t connection_key,
                                       uint32_t mobile_correlation_id) OVERRIDE;

  /**
   * @brief AddPolicyObserver allows to subscribe needed component to events
   * from policy.
   *
   * @param listener the component to subscribe.
   */
  void AddPolicyObserver(PolicyHandlerObserver* listener);

  /**
   * @brief RemovePolicyObserver allows to remove observer from collection.
   *
   * @param listener observer to remove.
   */
  void RemovePolicyObserver(PolicyHandlerObserver* listener);

  /**
   * @brief Checks application HMI state and returns true if streaming is
   * allowed
   * @param app_id Application id
   * @param service_type Service type to check
   * @return True if streaming is allowed, false in other case
   */
  bool HMIStateAllowsStreaming(
      uint32_t app_id,
      protocol_handler::ServiceType service_type) const OVERRIDE;

  /**
   * @brief Checks if application can stream (streaming service is started and
   * streaming is enabled in application)
   * @param app_id Application id
   * @param service_type Service type to check
   * @return True if streaming is allowed, false in other case
   */
  bool CanAppStream(uint32_t app_id,
                    protocol_handler::ServiceType service_type) const OVERRIDE;

  /**
   * @brief Ends opened navi services (audio/video) for application
   * @param app_id Application id
   */
  void EndNaviServices(uint32_t app_id) OVERRIDE;

  DEPRECATED
  void ForbidStreaming(uint32_t app_id) OVERRIDE;

  void ForbidStreaming(uint32_t app_id,
                       protocol_handler::ServiceType service_type) OVERRIDE;

  void OnStreamingConfigurationSuccessful(
      uint32_t app_id, protocol_handler::ServiceType service_type) OVERRIDE;

  void OnStreamingConfigurationFailed(uint32_t app_id,
                                      std::vector<std::string>& rejected_params,
                                      const std::string& reason) OVERRIDE;

  void OnAppStreaming(uint32_t app_id,
                      protocol_handler::ServiceType service_type,
                      const Application::StreamingState new_state) OVERRIDE;

  mobile_api::HMILevel::eType GetDefaultHmiLevel(
      ApplicationConstSharedPtr application) const;

  /**
   * @brief Checks if required transport for resumption is available
   *
   * The required transport can be configured through smartDeviceLink.ini file.
   *
   * @param application an instance of the app to check
   * @return true if the app is connected through one of the required
   *         transports, false otherwise
   */
  bool CheckResumptionRequiredTransportAvailable(
      ApplicationConstSharedPtr application) const;

  /**
   * Getter for resume_controller
   * @return Resume Controller
   */
  resumption::ResumeCtrl& resume_controller() OVERRIDE {
    return *resume_ctrl_.get();
  }

  HmiInterfaces& hmi_interfaces() OVERRIDE {
    return hmi_interfaces_;
  }

  /**
   * Generate grammar ID
   *
   * @return New grammar ID
   */
  uint32_t GenerateGrammarID() OVERRIDE;

  /**
   * Generate new HMI application ID
   *
   * @return New HMI application ID
   */
  uint32_t GenerateNewHMIAppID() OVERRIDE;

  /*
   * @brief Save binary data to specified directory
   *
   * @param binary data
   * @param path for saving data
   * @param file_name File name
   * @param offset for saving data to existing file with offset.
   *        If offset is 0 - create new file ( overrite existing )
   *
   *
   * @return SUCCESS if file was saved, other code otherwise
   */
  mobile_apis::Result::eType SaveBinary(const std::vector<uint8_t>& binary_data,
                                        const std::string& file_path,
                                        const std::string& file_name,
                                        const uint64_t offset) OVERRIDE;

  /**
   * @brief Get available app space
   * @param name of the app folder(make + mobile app id)
   * @return free app space.
   */
  uint32_t GetAvailableSpaceForApp(const std::string& folder_name);

  /*
   * @brief returns true if HMI is cooperating
   */
  bool IsHMICooperating() const OVERRIDE;

  void SetHMICooperating(const bool hmi_cooperating) OVERRIDE;
  /**
   * @brief Method used to send default app tts globalProperties
   * in case they were not provided from mobile side after defined time
   */
  void OnTimerSendTTSGlobalProperties();

  /**
   * @brief method adds application
   * to tts_global_properties_app_list_
   * @param app_id contains application which will
   * send TTS global properties after timeout
   */
  void AddAppToTTSGlobalPropertiesList(const uint32_t app_id) OVERRIDE;

  /**
   * @brief method removes application
   * from tts_global_properties_app_list_
   * @param app_id contains application which will
   * send TTS global properties after timeout
   */
  void RemoveAppFromTTSGlobalPropertiesList(const uint32_t app_id) OVERRIDE;

  ResetGlobalPropertiesResult ResetGlobalProperties(
      const smart_objects::SmartObject& global_properties_ids,
      const uint32_t app_id) OVERRIDE;

  ResetGlobalPropertiesResult ResetAllApplicationGlobalProperties(
      const uint32_t app_id) OVERRIDE;

  // TODO(AOleynik): Temporary added, to fix build. Should be reworked.
  connection_handler::ConnectionHandler& connection_handler() const OVERRIDE;
  protocol_handler::ProtocolHandler& protocol_handler() const OVERRIDE;

  policy::PolicyHandlerInterface& GetPolicyHandler() OVERRIDE {
    return *policy_handler_;
  }

  const policy::PolicyHandlerInterface& GetPolicyHandler() const OVERRIDE {
    return *policy_handler_;
  }

  rpc_service::RPCService& GetRPCService() const OVERRIDE {
    return *rpc_service_;
  }

  rpc_handler::RPCHandler& GetRPCHandler() const OVERRIDE {
    return *rpc_handler_;
  }

  void SetRPCService(std::unique_ptr<rpc_service::RPCService>& rpc_service) {
    rpc_service_ = std::move(rpc_service);
  }

  bool is_stopping() const OVERRIDE;

  bool is_audio_pass_thru_active() const OVERRIDE;
  /*
   * @brief Function Should be called when Low Voltage is occured
   */
  void OnLowVoltage();

  /*
   * @brief Function Should be called when WakeUp occures after Low Voltage
   */
  void OnWakeUp();

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
  bool IsApplicationForbidden(uint32_t connection_key,
                              const std::string& policy_app_id) const OVERRIDE;

  policy::DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const OVERRIDE;

  // typedef for Applications list
  typedef std::set<std::string> ForbiddenApps;

  struct AppIdPredicate {
    uint32_t app_id_;
    AppIdPredicate(uint32_t app_id) : app_id_(app_id) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? app_id_ == app->app_id() : false;
    }
  };

  struct HmiAppIdPredicate {
    uint32_t hmi_app_id_;
    HmiAppIdPredicate(uint32_t hmi_app_id) : hmi_app_id_(hmi_app_id) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? hmi_app_id_ == app->hmi_app_id() : false;
    }
  };

  struct PolicyAppIdPredicate {
    std::string policy_app_id_;
    PolicyAppIdPredicate(const std::string& policy_app_id)
        : policy_app_id_(policy_app_id) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? policy_app_id_ == app->policy_app_id() : false;
    }
  };

  struct SubscribedToButtonPredicate {
    mobile_apis::ButtonName::eType button_;
    SubscribedToButtonPredicate(mobile_apis::ButtonName::eType button)
        : button_(button) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? app->IsSubscribedToButton(button_) : false;
    }
  };

  struct AppV4DevicePredicate {
    connection_handler::DeviceHandle handle_;
    AppV4DevicePredicate(const connection_handler::DeviceHandle handle)
        : handle_(handle) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? handle_ == app->device() &&
                       Message::is_sufficient_version(
                           protocol_handler::MajorProtocolVersion::
                               PROTOCOL_VERSION_4,
                           app->protocol_version())
                 : false;
    }
  };

  struct DevicePredicate {
    connection_handler::DeviceHandle handle_;
    DevicePredicate(const connection_handler::DeviceHandle handle)
        : handle_(handle) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return handle_ == app->device() ? true : false;
    }
  };

  struct GrammarIdPredicate {
    uint32_t grammar_id_;
    GrammarIdPredicate(uint32_t grammar_id) : grammar_id_(grammar_id) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? grammar_id_ == app->get_grammar_id() : false;
    }
  };

  struct AppNamePredicate {
    std::string app_name_;
    AppNamePredicate(const std::string& app_name) : app_name_(app_name) {}
    bool operator()(const ApplicationSharedPtr app) const {
      return app ? app->name() == app_name_ : false;
    }
  };

  /**
   * @brief Sends UpdateAppList notification to HMI
   */
  void SendUpdateAppList() OVERRIDE;

  /**
   * @brief Marks applications received through QueryApps as should be
   * greyed out on HMI
   * @param is_greyed_out, true, if should be greyed out, otherwise - false
   * @param handle, device handle
   */
  void MarkAppsGreyOut(const connection_handler::DeviceHandle handle,
                       bool is_greyed_out) OVERRIDE;

  ApplicationConstSharedPtr WaitingApplicationByID(
      const uint32_t hmi_id) const OVERRIDE;

  DataAccessor<AppsWaitRegistrationSet> AppsWaitingForRegistration()
      const OVERRIDE;

  /**
   * @brief Checks, if apps list had been queried already from certain device
   * @param handle, Device handle
   * @return true, if list had been queried already, otherwise - false
   */
  bool IsAppsQueriedFrom(
      const connection_handler::DeviceHandle handle) const OVERRIDE;

  /**
   * @brief IsAppInReconnectMode check if application belongs to session
   * affected by transport switching at the moment by checking internal
   * waiting list prepared on switching start
   * @param device_id device identifier
   * @param policy_app_id Application id
   * @return True if application is in the waiting list, otherwise - false
   */
  bool IsAppInReconnectMode(const connection_handler::DeviceHandle& device_id,
                            const std::string& policy_app_id) const FINAL;

  bool IsStopping() const OVERRIDE {
    return is_stopping_;
  }

  /**
   * @brief ProcessReconnection handles reconnection flow for application on
   * transport switch
   * @param application Pointer to switched application, must be validated
   * before passing
   * @param connection_key Connection key from registration request of
   * switched
   * application
   */
  void ProcessReconnection(ApplicationSharedPtr application,
                           const uint32_t connection_key) FINAL;

  /**
   * @brief Clears all applications' persistent data
   */
  void ClearAppsPersistentData();

  StateController& state_controller() OVERRIDE;
  const ApplicationManagerSettings& get_settings() const OVERRIDE;
  std::string GetCorrectMobileIDFromMessage(
      const commands::MessageSharedPtr& message) const OVERRIDE;
  virtual event_engine::EventDispatcher& event_dispatcher() OVERRIDE;

  app_launch::AppLaunchCtrl& app_launch_ctrl() OVERRIDE;

  bool IsSOStructValid(const hmi_apis::StructIdentifiers::eType struct_id,
                       const smart_objects::SmartObject& display_capabilities);

  /**
   * @brief Function returns supported SDL Protocol Version
   * @return protocol version depends on parameters from smartDeviceLink.ini.
   */
  protocol_handler::MajorProtocolVersion SupportedSDLVersion() const OVERRIDE;

  void ApplyFunctorForEachPlugin(
      std::function<void(plugin_manager::RPCPlugin&)> functor) OVERRIDE;

 private:
  /**
   * @brief Adds application to registered applications list and marks it as
   * registered
   * @param application Application that should be added to registered
   * applications list.
   */
  void AddAppToRegisteredAppList(const ApplicationSharedPtr application);

  /**
   * @brief Removes service status record for service that failed to start
   * @param app Application whose service status record should be removed
   * @param Service type which status record should be removed
   */
  bool HandleRejectedServiceStatus(
      ApplicationSharedPtr app,
      const hmi_apis::Common_ServiceType::eType service_type);
  /**
   * @brief PullLanguagesInfo allows to pull information about languages.
   *
   * @param app_data entry to parse
   *
   * @param ttsName tts name that should be filled.
   * @param vrSynonym vr synonymus that should be filled.
   */
  void PullLanguagesInfo(const smart_objects::SmartObject& app_data,
                         smart_objects::SmartObject& ttsName,
                         smart_objects::SmartObject& vrSynonym);

  /**
   * @brief Method compares arrays of app HMI type
   * @param from_policy contains app HMI type from policy
   * @param from_application contains app HMI type from application
   * @return return TRUE if arrays of appHMIType equal, otherwise return FALSE
   */
  bool CompareAppHMIType(const smart_objects::SmartObject& from_policy,
                         const smart_objects::SmartObject& from_application);

  hmi_apis::HMI_API& hmi_so_factory();
  mobile_apis::MOBILE_API& mobile_so_factory();

  bool ConvertSOtoMessage(const smart_objects::SmartObject& message,
                          Message& output,
                          const bool allow_unknown_parameters = false);

  template <typename ApplicationList>
  void PrepareApplicationListSO(ApplicationList& app_list,
                                smart_objects::SmartObject& applications,
                                ApplicationManager& app_mngr) {
    smart_objects::SmartArray* app_array = applications.asArray();
    uint32_t app_count = NULL == app_array ? 0 : app_array->size();
    typename ApplicationList::const_iterator it;
    for (it = app_list.begin(); it != app_list.end(); ++it) {
      if (it->use_count() == 0) {
        SDL_LOG_ERROR("Application not found");
        continue;
      }

      smart_objects::SmartObject hmi_application(smart_objects::SmartType_Map);
      const protocol_handler::SessionObserver& session_observer =
          connection_handler().get_session_observer();
      if (MessageHelper::CreateHMIApplicationStruct(*it,
                                                    session_observer,
                                                    GetPolicyHandler(),
                                                    &hmi_application,
                                                    app_mngr)) {
        applications[app_count++] = hmi_application;
      } else {
        SDL_LOG_DEBUG("Can't CreateHMIApplicationStruct");
      }
    }

    if (0 == app_count) {
      SDL_LOG_WARN("Empty applications list");
    }
  }

  void OnApplicationListUpdateTimer();

  /**
   * @brief CreateApplications creates aplpication adds it to application list
   * and prepare data for sending AppIcon request.
   *
   * @param obj_array applications array.
   *
   * @param connection_key connection key of app, which provided app list to
   * be created
   */
  void CreateApplications(smart_objects::SmartArray& obj_array,
                          const uint32_t connection_key);

  /*
   * @brief Function is called on IGN_OFF, Master_reset or Factory_defaults
   * to notify HMI that SDL is shutting down.
   */
  void SendOnSDLClose();

  /**
   * @brief returns true if low voltage state is active
   */
  bool IsLowVoltage() const OVERRIDE;

  /**
   * @brief Allows to process postponed commands for application
   * when its HMI level has been changed.
   * @param app_id the application id for processing.
   * @param from the old HMILevel.
   * @param to the new HMILevel for the certain app.
   */
  void ProcessPostponedMessages(const uint32_t app_id);

  /**
   * @brief Allows to process applications after HMILevel has been changed.
   * @param app_id the application id for processing.
   * @param from the old HMILevel.
   * @param to the new HMILevel for the certain app.
   */
  void ProcessApp(const uint32_t app_id,
                  const HmiStatePtr from,
                  const HmiStatePtr to);

  /**
   * @brief Starts EndStream timer for a specified application
   * @param app_id Application to process
   */
  void StartEndStreamTimer(const uint32_t app_id);

  /**
   * @brief Allows to send appropriate message to mobile device.
   * @param message The smart object which contains all neccesary info to send
   * notification.
   */
  void SendMobileMessage(smart_objects::SmartObjectSPtr message);

  /**
   * @brief Sets default value of the HELPPROMT global property
   * to the first vrCommand of the Command Menu registered in application
   *
   * @param app Registered application
   * @param is_timeout_promp Flag indicating that timeout prompt
   * should be reset
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool ResetHelpPromt(ApplicationSharedPtr app) const;

  /**
   * @brief  Sets default value of the TIMEOUTPROMT global property
   * to the first vrCommand of the Command Menu registered in application
   *
   * @param app Registered application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool ResetTimeoutPromt(ApplicationSharedPtr app) const;

  /**
   * @brief Sets default value of the VRHELPTITLE global property
   * to the application name and value of the VRHELPITEMS global property
   * to value equal to registered command -1(default command “Help / Cancel”.)
   *
   * @param app Registered application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool ResetVrHelpTitleItems(ApplicationSharedPtr app) const;

 private:
  /*
   * NaviServiceStatusMap shows which navi service (audio/video) is opened
   * for specified application. Two bool values in std::pair mean:
   * 1st value - is video service opened or not
   * 2nd value - is audio service opened or not
   */
  typedef std::map<uint32_t, std::pair<bool, bool> > NaviServiceStatusMap;

  /**
   * @brief GetHashedAppID allows to obtain unique application id as a string.
   * It concatenates device mac and application id to obtain unique id.
   *
   * @param connection_key connection key for which need to obtain device mac;
   *
   * @param policy_app_id mobile(policy) application id on particular device.
   * This parameter will be concatenated with device id.
   *
   * @return unique aplication identifier.
   */
  std::string GetHashedAppID(uint32_t connection_key,
                             const std::string& policy_app_id) const;

  /**
   * @brief CreateAllAppGlobalPropsIDList creates an array of all application
   * global properties IDs. Used as utility to call
   * ApplicationManger::ResetGlobalProperties
   * with all global properties.
   * @return array smart object with global properties identifiers.
   */
  const smart_objects::SmartObjectSPtr CreateAllAppGlobalPropsIDList(
      const uint32_t app_id) const;

  /**
   * @brief Removes suspended and stopped timers from timer pool
   */
  void ClearTimerPool();

  /**
   * @brief CloseNaviApp allows to unregister application in case the
   * EndServiceEndedAck
   * didn't come for at least one of services(audio or video)
   */
  void CloseNaviApp();

  /**
   * @brief Suspends streaming ability of application in case application's HMI
   * level
   * has been changed to not allowed for streaming
   */
  void EndNaviStreaming();

  /**
   * @brief Starts specified navi service for application
   * @param app_id Application to proceed
   * @param service_type Type of service to start
   * @param params configuration parameters specified by mobile
   * @return True if service is immediately started or configuration
   * parameters are sent to HMI, false on other cases
   */
  bool StartNaviService(uint32_t app_id,
                        protocol_handler::ServiceType service_type,
                        const BsonObject* params);

  /**
   * @brief Stops specified navi service for application
   * @param app_id Application to proceed
   * @param service_type Type of service to stop
   */
  void StopNaviService(uint32_t app_id,
                       protocol_handler::ServiceType service_type);

  /**
   * @brief Allows streaming for application if it was disallowed by
   * DisallowStreaming()
   * @param app_id Application to proceed
   */
  void AllowStreaming(uint32_t app_id);

  /**
   * @brief Disallows streaming for application, but doesn't close
   * opened services. Streaming ability could be restored by AllowStreaming();
   * @param app_id Application to proceed
   */
  void DisallowStreaming(uint32_t app_id);

  /**
   * @brief Types of directories used by Application Manager
   */
  enum DirectoryType { TYPE_STORAGE, TYPE_SYSTEM, TYPE_ICONS };

  typedef std::map<DirectoryType, std::string> DirectoryTypeMap;
  DirectoryTypeMap dir_type_to_string_map_;

  /**
   * @brief Converts directory type to string
   * @param type Directory type
   * @return Stringified type
   */
  const std::string DirectoryTypeToString(DirectoryType type) const;

  /**
   * @brief Creates directory path, if necessary
   * @param path Directory path
   * @param type Directory type
   * @return true, if succedeed, otherwise - false
   */
  bool InitDirectory(const std::string& path, DirectoryType type) const;

  /**
   * @brief Checks, whether r/w permissions are present for particular path
   * @param path Directory path
   * @param type Directory type
   * @return true, if allowed, otherwise - false
   */
  bool IsReadWriteAllowed(const std::string& path, DirectoryType type) const;

  /**
   * @brief Removes apps, waiting for registration related to
   * certain device handle
   * @param handle, Device handle
   */
  void RemoveAppsWaitingForRegistration(
      const connection_handler::DeviceHandle handle);

  void ClearTTSGlobalPropertiesList();

  /**
   * @brief EraseAppFromReconnectionList drops application from reconnection
   * list on transport switch success
   * @param app Pointer to application
   */
  void EraseAppFromReconnectionList(const ApplicationSharedPtr& app);

  /**
   * @brief SwitchApplication updates parameters of switched application and
   * internal applications list
   * @param app Pointer to switched application, must be validated before
   * passing in
   * @param connection_key Connection key of switched application from its
   * registration request
   * @param device_id Device id of switched application
   * @param mac_address New device mac address
   */
  void SwitchApplication(ApplicationSharedPtr app,
                         const uint32_t connection_key,
                         const size_t device_id,
                         const std::string& mac_address);

  /**
   * @brief Converts device handle to transport type string used in
   * smartDeviceLink.ini file, e.g. "TCP_WIFI"
   * @param device_handle A device handle
   * @return string representation of the transport of the device
   */
  const std::string GetTransportTypeProfileString(
      connection_handler::DeviceHandle device_handle) const;

  /**
   * @brief Converts BSON object containing video parameters to
   * smart object's map object
   * @param output the smart object to add video parameters
   * @param input BSON object to read parameters from
   */
  static void ConvertVideoParamsToSO(smart_objects::SmartObject& output,
                                     const BsonObject* input);

  /**
   * @brief Converts rejected parameters' names acquired from HMI to
   * SDL protocol's parameter names
   * @param list of rejected parameters' names
   * @return converted parameters' names
   */
  static std::vector<std::string> ConvertRejectedParamList(
      const std::vector<std::string>& input);

 private:
  const ApplicationManagerSettings& settings_;
  /**
   * @brief List of applications
   */
  ApplicationSet applications_;
  AppsWaitRegistrationSet apps_to_register_;
  ForbiddenApps forbidden_applications;
  ReregisterWaitList reregister_wait_list_;

  // Lock for applications list
  mutable std::shared_ptr<sync_primitives::RecursiveLock>
      applications_list_lock_ptr_;
  mutable std::shared_ptr<sync_primitives::Lock>
      apps_to_register_list_lock_ptr_;
  mutable std::shared_ptr<sync_primitives::Lock> reregister_wait_list_lock_ptr_;
  mutable sync_primitives::Lock subscribed_way_points_apps_lock_;

  /**
   * @brief Map of correlation id  and associated application id.
   */
  std::map<const int32_t, const uint32_t> appID_list_;

  /**
   * @brief Set AppIDs of subscribed apps for way points
   */
  std::set<uint32_t> subscribed_way_points_apps_list_;

  bool subscribed_to_hmi_way_points_;

  smart_objects::SmartObjectSPtr hmi_way_points_data_;

  std::map<uint32_t, smart_objects::SmartObject> mobile_way_points_data_;

  /**
   * @brief Map contains applications which
   * will send TTS global properties to HMI after timeout
   */
  std::map<uint32_t, date_time::TimeDuration> tts_global_properties_app_list_;
  std::set<connection_handler::DeviceHandle> query_apps_devices_;
  bool audio_pass_thru_active_;
  uint32_t audio_pass_thru_app_id_;
  sync_primitives::Lock audio_pass_thru_lock_;
  sync_primitives::Lock tts_global_properties_app_list_lock_;
  mutable sync_primitives::Lock query_apps_devices_lock_;
  hmi_apis::Common_DriverDistractionState::eType driver_distraction_state_;
  bool is_vr_session_strated_;
  bool hmi_cooperating_;
  bool is_all_apps_allowed_;
  uint32_t current_audio_source_;

  event_engine::EventDispatcherImpl event_dispatcher_;
  media_manager::MediaManager* media_manager_;

  hmi_message_handler::HMIMessageHandler* hmi_handler_;
  connection_handler::ConnectionHandler* connection_handler_;
  std::unique_ptr<policy::PolicyHandlerInterface> policy_handler_;
  protocol_handler::ProtocolHandler* protocol_handler_;
  std::unique_ptr<plugin_manager::RPCPluginManager> plugin_manager_;
  request_controller::RequestController request_ctrl_;
  std::unique_ptr<application_manager::AppServiceManager> app_service_manager_;

  /**
   * @brief Map contains apps with HMI state before incoming call
   * After incoming call ends previous HMI state must restore
   *
   */
  struct AppState {
    AppState(const mobile_apis::HMILevel::eType& level,
             const mobile_apis::AudioStreamingState::eType& streaming_state,
             const mobile_apis::SystemContext::eType& context)
        : hmi_level(level)
        , audio_streaming_state(streaming_state)
        , system_context(context) {}

    mobile_apis::HMILevel::eType hmi_level;
    mobile_apis::AudioStreamingState::eType audio_streaming_state;
    mobile_apis::SystemContext::eType system_context;
  };

  hmi_apis::HMI_API* hmi_so_factory_;
  mobile_apis::MOBILE_API* mobile_so_factory_;

  static uint32_t mobile_corelation_id_;
  static uint32_t corelation_id_;
  static const uint32_t max_corelation_id_;

  std::unique_ptr<HMICapabilities> hmi_capabilities_;
  // The reason of HU shutdown
  mobile_api::AppInterfaceUnregisteredReason::eType unregister_reason_;

  /**
   * @brief Resume controler is responcible for save and load information
   * about persistent application data on disk, and save session ID for resuming
   * application in case INGITION_OFF or MASTER_RESSET
   */
  std::unique_ptr<resumption::ResumeCtrl> resume_ctrl_;

  HmiInterfacesImpl hmi_interfaces_;

  NaviServiceStatusMap navi_service_status_;
  sync_primitives::Lock navi_service_status_lock_;
  std::deque<uint32_t> navi_app_to_stop_;
  sync_primitives::Lock navi_app_to_stop_lock_;
  std::deque<uint32_t> navi_app_to_end_stream_;
  uint32_t navi_close_app_timeout_;
  uint32_t navi_end_stream_timeout_;

  std::vector<TimerSPtr> close_app_timer_pool_;
  std::vector<TimerSPtr> end_stream_timer_pool_;
  sync_primitives::Lock close_app_timer_pool_lock_;
  sync_primitives::Lock end_stream_timer_pool_lock_;

  StateControllerImpl state_ctrl_;
  std::unique_ptr<app_launch::AppLaunchData> app_launch_dto_;
  std::unique_ptr<app_launch::AppLaunchCtrl> app_launch_ctrl_;

  // This is a cache to remember DeviceHandle of secondary transports. Only used
  // during RegisterApplication().
  typedef std::map<int32_t, connection_handler::DeviceHandle> DeviceMap;

  DeviceMap secondary_transport_devices_cache_;

  mutable std::shared_ptr<sync_primitives::RecursiveLock>
      pending_device_map_lock_ptr_;
  std::map<std::string, std::string> pending_device_map_;

  sync_primitives::Lock app_icon_map_lock_ptr_;
  std::map<std::string, AppIconInfo> app_icon_map_;

#ifdef TELEMETRY_MONITOR
  AMTelemetryObserver* metric_observer_;
#endif  // TELEMETRY_MONITOR

  Timer application_list_update_timer_;

  Timer tts_global_properties_timer_;

  Timer clear_pool_timer_;

  bool is_low_voltage_;

  uint32_t apps_size_;

  std::atomic<bool> registered_during_timer_execution_;

  std::atomic<bool> is_stopping_;

  std::unique_ptr<CommandHolder> commands_holder_;

  std::unique_ptr<rpc_service::RPCService> rpc_service_;
  std::unique_ptr<rpc_handler::RPCHandler> rpc_handler_;

  ServiceStreamingStatusMap streaming_application_services_;
  sync_primitives::Lock streaming_services_lock_;

#ifdef BUILD_TESTS
 public:
  /**
   * @brief register a mock application without going through the formal
   * registration process. Only for unit testing.
   * @param mock_app the mock app to be registered
   */
  void AddMockApplication(ApplicationSharedPtr mock_app);

  /**
   * @brief Add a mock application to the pending application list without going
   * through the formal registration process. Only for unit testing.
   * @param mock_app the mock app to be added to the pending application list
   */
  void AddMockPendingApplication(ApplicationSharedPtr mock_app);

  /**
   * @brief set a mock media manager without running Init(). Only for unit
   * testing.
   * @param mock_media_manager the mock media manager to be assigned
   */
  void SetMockMediaManager(media_manager::MediaManager* mock_media_manager);

  /**
   * @brief set a mock rpc service directly. Only for unit
   * testing.
   * @param mock_app the mock rpc service to be assigned
   */
  void SetMockRPCService(rpc_service::RPCService* rpc_service) {
    rpc_service_.reset(rpc_service);
  }

  /**
   * @brief set a mock rpc service directly. Only for unit
   * testing.
   * @param mock_app the mock rpc service to be assigned
   */
  void SetMockPolicyHandler(policy::PolicyHandlerInterface* policy_handler) {
    policy_handler_.reset(policy_handler);
  }

  virtual void SetPluginManager(
      std::unique_ptr<plugin_manager::RPCPluginManager>& plugin_manager)
      OVERRIDE {
    plugin_manager_.reset(plugin_manager.release());
  }

  virtual void SetAppServiceManager(AppServiceManager* app_service_manager) {
    app_service_manager_.reset(app_service_manager);
  }

 private:
#endif

  DISALLOW_COPY_AND_ASSIGN(ApplicationManagerImpl);
};

inline bool ApplicationManagerImpl::all_apps_allowed() const {
  return is_all_apps_allowed_;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_IMPL_H_
