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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_

#include <stdint.h>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "application_manager/application.h"
#include "application_manager/application_data_impl.h"
#include "application_manager/help_prompt_manager_impl.h"
#include "application_manager/hmi_state.h"
#include "application_manager/usage_statistics.h"
#include "protocol_handler/protocol_handler.h"

#include <atomic>
#include "connection_handler/device.h"
#include "utils/custom_string.h"
#include "utils/date_time.h"
#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/timer.h"

namespace usage_statistics {

class StatisticsManager;
}  // namespace usage_statistics

namespace application_manager {
using namespace utils;
using namespace timer;

namespace mobile_api = mobile_apis;
namespace custom_str = custom_string;

/**
 * @brief SwitchApplicationParameters updates application internal parameters
 * on transport switch. Must be used only for switching flow.
 * @param app Pointer to switched application
 * @param app_id New application id (connection key)
 * @param device_id New device id
 * @param mac_address New device MAC address
 */
void SwitchApplicationParameters(ApplicationSharedPtr app,
                                 const uint32_t app_id,
                                 const size_t device_id,
                                 const std::string& mac_address);

class ApplicationImpl : public virtual Application,
                        public virtual InitialApplicationDataImpl,
                        public virtual DynamicApplicationDataImpl {
 public:
  ApplicationImpl(
      uint32_t application_id,
      const std::string& policy_app_id,
      const std::string& mac_address,
      const connection_handler::DeviceHandle device_id,
      const custom_str::CustomString& app_name,
      std::shared_ptr<usage_statistics::StatisticsManager> statistics_manager,
      ApplicationManager& application_manager);

  ~ApplicationImpl();

  /**
   * @brief Returns message belonging to the application
   * that is currently executed (i.e. on HMI).
   * @return smart_objects::SmartObject * Active message
   */
  const smart_objects::SmartObject* active_message() const;
  void CloseActiveMessage();
  bool IsFullscreen() const;

  /**
   * @brief change supporting COMMUNICATION NAVIGATION
   */
  virtual void ChangeSupportingAppHMIType();

  inline bool is_navi() const {
    return is_navi_;
  }
  void set_is_navi(bool option);

  virtual bool is_remote_control_supported() const;

  void set_remote_control_supported(const bool allow);

  void set_mobile_projection_enabled(bool option);

  bool mobile_projection_enabled() const;

  void set_webengine_projection_enabled(const bool option) OVERRIDE;
  bool webengine_projection_enabled() const OVERRIDE;

  bool video_streaming_approved() const;
  void set_video_streaming_approved(bool state);
  bool audio_streaming_approved() const;
  void set_audio_streaming_approved(bool state);

  bool video_streaming_allowed() const;
  void set_video_streaming_allowed(bool state);
  bool audio_streaming_allowed() const;
  void set_audio_streaming_allowed(bool state);

  bool SetVideoConfig(protocol_handler::ServiceType service_type,
                      const smart_objects::SmartObject& params);
  void StartStreaming(protocol_handler::ServiceType service_type);
  void StopStreamingForce(protocol_handler::ServiceType service_type);
  void StopStreaming(protocol_handler::ServiceType service_type);
  void SuspendStreaming(protocol_handler::ServiceType service_type);
  void WakeUpStreaming(protocol_handler::ServiceType service_type);

  virtual bool is_voice_communication_supported() const;
  virtual void set_voice_communication_supported(bool option);
  inline bool app_allowed() const;
  bool has_been_activated() const;
  bool set_activated(bool is_active);
  bool is_ready() const;
  bool set_is_ready(bool is_ready);

  const Version& version() const;
  void set_hmi_application_id(uint32_t hmi_app_id);
  inline uint32_t hmi_app_id() const;
  inline uint32_t app_id() const;
  const custom_str::CustomString& name() const;
  void set_folder_name(const std::string& folder_name) OVERRIDE;
  const std::string folder_name() const;
  bool is_media_application() const;
  bool is_foreground() const OVERRIDE;
  void set_foreground(const bool is_foreground) OVERRIDE;
  const mobile_apis::HMILevel::eType hmi_level(
      const WindowID window_id) const OVERRIDE;
  const uint32_t put_file_in_none_count() const;
  const uint32_t delete_file_in_none_count() const;
  const uint32_t list_files_in_none_count() const;
  const mobile_api::SystemContext::eType system_context(
      const WindowID window_id) const OVERRIDE;
  inline const mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const;
  inline const mobile_apis::VideoStreamingState::eType video_streaming_state()
      const;
  const std::string& app_icon_path() const;
  connection_handler::DeviceHandle device() const;
  connection_handler::DeviceHandle secondary_device() const;
  const std::string& mac_address() const OVERRIDE;
  const std::string& bundle_id() const OVERRIDE;
  void set_bundle_id(const std::string& bundle_id) OVERRIDE;
  void set_tts_properties_in_none(bool active);
  bool tts_properties_in_none();
  void set_tts_properties_in_full(bool active);
  bool tts_properties_in_full();
  void set_keep_context(bool keep_context);
  bool keep_context();
  void set_version(const Version& ver);
  void set_name(const custom_str::CustomString& name);
  void set_is_media_application(bool option);
  void increment_put_file_in_none_count();
  void increment_delete_file_in_none_count();
  void increment_list_files_in_none_count();
  bool set_app_icon_path(const std::string& path);
  void set_app_allowed(const bool allowed);
  void set_device(connection_handler::DeviceHandle device);
  void set_secondary_device(connection_handler::DeviceHandle secondary_device);
  virtual uint32_t get_grammar_id() const;
  virtual void set_grammar_id(uint32_t value);
  bool is_audio() const OVERRIDE;

  virtual void set_protocol_version(
      const protocol_handler::MajorProtocolVersion& protocol_version);
  virtual protocol_handler::MajorProtocolVersion protocol_version() const;

  virtual void set_is_resuming(bool is_resuming);
  virtual bool is_resuming() const;

  /**
   * @brief Remembers the HMI level which the app would resume into if high-
   * bandwidth transport were available.
   * @param level The HMI level which the app would resume into. Specify
   * INVALID_ENUM to clear the state.
   */
  void set_deferred_resumption_hmi_level(
      mobile_api::HMILevel::eType level) OVERRIDE;
  /**
   * @brief Returns the HMI level which the app would resume into if high-
   * bandwidth transport were available.
   *
   * A value of INVALID_ENUM indicates that the app does not have deferred
   * HMI level.
   * @return HMI level which the app would resume into
   */
  mobile_api::HMILevel::eType deferred_resumption_hmi_level() const OVERRIDE;

  bool AddFile(const AppFile& file);
  bool UpdateFile(const AppFile& file);
  bool DeleteFile(const std::string& file_name);
  virtual const AppFilesMap& getAppFiles() const;

  virtual const AppFile* GetFile(const std::string& file_name);

  bool SubscribeToButton(mobile_apis::ButtonName::eType btn_name);
  bool IsSubscribedToButton(mobile_apis::ButtonName::eType btn_name);
  bool UnsubscribeFromButton(mobile_apis::ButtonName::eType btn_name);

  WindowID GetSoftButtonWindowID(const uint32_t button_id) OVERRIDE;

  inline bool IsRegistered() const OVERRIDE;

  /**
   * @brief ResetDataInNone reset data counters in NONE
   */
  virtual void ResetDataInNone() OVERRIDE;

  virtual DataAccessor<ButtonSubscriptions> SubscribedButtons() const OVERRIDE;

  virtual const std::string& curHash() const;

  /**
   * @brief Change Hash for current application
   * and send notification to mobile
   * @return updated_hash
   */
  virtual void UpdateHash();

  /**
   * @brief checks is hashID was changed during suspended state
   * @return Returns TRUE if hashID was changed during suspended state
   * otherwise returns FALSE.
   */
  bool IsHashChangedDuringSuspend() const OVERRIDE;

  /**
   * @brief changes state of the flag which tracks is hashID was changed during
   * suspended state or not
   * @param state new state of the flag
   */
  void SetHashChangedDuringSuspend(const bool state) OVERRIDE;

  UsageStatistics& usage_report();

  /**
   * @brief Access to HelpPromptManager interface
   * @return object for Handling VR help
   */
  HelpPromptManager& help_prompt_manager() OVERRIDE;

  bool AreCommandLimitsExceeded(mobile_apis::FunctionID::eType cmd_id,
                                TLimitSource source);
  virtual void SubscribeToSoftButtons(
      int32_t cmd_id, const WindowSoftButtons& window_softbuttons);
  virtual bool IsSubscribedToSoftButton(const uint32_t softbutton_id);

  virtual void UnsubscribeFromSoftButtons(int32_t cmd_id);

  virtual bool is_application_data_changed() const;

  bool is_app_data_resumption_allowed() const OVERRIDE;
  void set_app_data_resumption_allowance(const bool allowed) OVERRIDE;

  virtual void set_is_application_data_changed(bool state_application_data);

  /**
   * @brief Check's if it is media, voice communication or navigation
   * application
   *
   * @return true if application is media, voice communication or navigation
   */
  bool IsAudioApplication() const OVERRIDE;

  /**
   * @brief Checks whether the application is navigation or projection
   *
   * @return true if application is navigation or projection
   */
  bool IsVideoApplication() const OVERRIDE;

  /**
   * @brief SetInitialState sets initial HMI state for application on
   * registration
   * @param window_id window id for HMI state
   * @param window_name name of inited window
   * @param state Hmi state value
   */
  void SetInitialState(const WindowID window_id,
                       const std::string& window_name,
                       HmiStatePtr state) FINAL;

  /**
   * @brief SetRegularState set permanent state of application
   * @param window_id window id for HMI state
   * @param state state to setup
   */
  void SetRegularState(const WindowID window_id, HmiStatePtr state) FINAL;

  /**
   * @brief SetPostponedState sets postponed state to application.
   * This state could be set as regular later
   * @param window_id window id for HMI state
   * @param state state to setup
   */
  void SetPostponedState(const WindowID window_id, HmiStatePtr state) FINAL;

  /**
   * @brief RemovePostponedState removes postponed state for application
   * After removal, this state will not be set as regular later
   * @param window_id window id for HMI state
   */
  void RemovePostponedState(const WindowID window_id) FINAL;

  /**
   * @brief AddHMIState the function that will change application's
   * hmi state.
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state new hmi state for certain application.
   */
  void AddHMIState(const WindowID window_id, HmiStatePtr state) FINAL;

  /**
   * @brief RemoveHMIState the function that will turn back hmi_level after end
   * of some event
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state_id that should be removed
   */
  void RemoveHMIState(const WindowID window_id,
                      HmiState::StateID state_id) FINAL;

  /**
   * @brief HmiState of application within active events PhoneCall, TTS< etc ...
   * @return Active HmiState of application
   */
  const HmiStatePtr CurrentHmiState(const WindowID window_i) const FINAL;

  WindowNames GetWindowNames() const FINAL;

  WindowIds GetWindowIds() const FINAL;

  bool WindowIdExists(const WindowID window_id) const FINAL;

  /**
   * @brief Checks if app is allowed to change audio source
   * @return True - if allowed, otherwise - False
   */
  virtual bool IsAllowedToChangeAudioSource() const;

  /**
   * @brief RegularHmiState of application without active events VR, TTS etc ...
   * @return HmiState of application
   */
  const HmiStatePtr RegularHmiState(const WindowID window_id) const FINAL;

  /**
   * @brief PostponedHmiState returns postponed hmi state of application
   * if it's present
   *
   * @return Postponed hmi state of application
   */
  const HmiStatePtr PostponedHmiState(const WindowID window_id) const FINAL;

  uint32_t audio_stream_retry_number() const;

  void set_audio_stream_retry_number(const uint32_t& audio_stream_retry_number);

  uint32_t video_stream_retry_number() const;

  void set_video_stream_retry_number(const uint32_t& video_stream_retry_number);

  /**
   * @brief Load persistent files from application folder.
   */
  void LoadPersistentFiles() OVERRIDE;

  /**
   * @brief Get available app space
   * @param name of the app folder(make + mobile app id)
   * @return free app space.
   */
  uint32_t GetAvailableDiskSpace() OVERRIDE;

  /**
   * @brief Sets current system context
   * @param system_context new system context
   */
  void set_system_context(
      const WindowID window_id,
      const mobile_api::SystemContext::eType& system_context) OVERRIDE;
  /**
   * @brief Sets current audio streaming state
   * @param state new audio streaming state
   */
  void set_audio_streaming_state(
      const mobile_api::AudioStreamingState::eType& state) OVERRIDE;
  /**
   * @brief Sets current HMI level
   * @param hmi_level new HMI level
   */
  void set_hmi_level(const WindowID window_id,
                     const mobile_api::HMILevel::eType& hmi_level) OVERRIDE;

  void PushMobileMessage(
      smart_objects::SmartObjectSPtr mobile_message) OVERRIDE;

  void SwapMobileMessageQueue(MobileMessageQueue& mobile_messages) OVERRIDE;

  /**
   * @brief Get cloud app endpoint for websocket connection
   * @return cloud app endpoint
   */
  const std::string& cloud_app_endpoint() const OVERRIDE;

  /**
   * @brief Get cloud app auth token to be used in connection handshake after
   * websocket open.
   * @return cloud app auth token
   */
  const std::string& auth_token() const OVERRIDE;

  /**
   * @brief Get cloud app transport type. Defines the type of websocket
   * connection used.
   * @return cloud app transport type
   */
  const std::string& cloud_app_transport_type() const OVERRIDE;

  /**
   * @brief Get hybrid app preference. Defines behaviour for when a similar
   * mobile and cloud app are connected simultaneously.
   * @return hybrid app preference
   */
  const mobile_apis::HybridAppPreference::eType& hybrid_app_preference()
      const OVERRIDE;

  /**
   * @brief Get cloud app certificate. Used for secured websocket connections.
   * @return cloud app certificate.
   */
  const std::string& cloud_app_certificate() const OVERRIDE;

  /**
   * @brief Check whether the given application is a cloud app.
   * @return true if the application is a cloud application, false otherwise.
   */
  bool is_cloud_app() const OVERRIDE;

  /**
   * @brief Set cloud app endpoint
   */
  void set_cloud_app_endpoint(const std::string& endpoint) OVERRIDE;

  /**
   * @brief Set cloud app auth token
   */
  void set_auth_token(const std::string& auth_token) OVERRIDE;

  /**
   * @brief Set cloud app transport type
   */
  void set_cloud_app_transport_type(const std::string& transport_type) OVERRIDE;

  /**
   * @brief Set hybrid app preference
   */
  void set_hybrid_app_preference(const mobile_apis::HybridAppPreference::eType&
                                     hybrid_app_preference) OVERRIDE;

  /**
   * @brief Set cloud app certificate
   */
  void set_cloud_app_certificate(const std::string& certificate) OVERRIDE;

  void set_user_location(
      const smart_objects::SmartObject& user_location) OVERRIDE;

  const smart_objects::SmartObject& get_user_location() const OVERRIDE;

 protected:
  /**
   * @brief Clean up application folder. Persistent files will stay
   */
  void CleanupFiles();

 private:
  /**
   * @brief Callback for video streaming suspend timer.
   * Suspends video streaming process for application
   */
  void OnVideoStreamSuspend();

  /**
   * @brief Stops video streaming for application
   */
  inline void StopNaviStreaming();

  /**
   * @brief Stops audio streaming for application
   */
  inline void StopAudioStreaming();

  /**
   * @brief Callback for audio streaming suspend timer.
   * Suspends audio streaming process for application
   */
  void OnAudioStreamSuspend();

  AppExtensionPtr QueryInterface(AppExtensionUID uid) OVERRIDE;

  /**
   * @brief Add extension to application
   * @param extension pointer to extension
   * @return true if success, false if extension already initialized
   */
  bool AddExtension(AppExtensionPtr extention) OVERRIDE;

  /**
   * @brief Remove extension from application
   * @param uid uid of extension
   * @return true if success, false if extension is not present
   */
  bool RemoveExtension(AppExtensionUID uid) OVERRIDE;

  const std::list<AppExtensionPtr>& Extensions() const OVERRIDE;

  std::string hash_val_;
  uint32_t grammar_id_;

  Version version_;
  custom_str::CustomString app_name_;
  uint32_t hmi_app_id_;
  uint32_t app_id_;
  smart_objects::SmartObject* active_message_;
  bool is_media_;
  bool is_navi_;
  bool is_remote_control_supported_;
  bool mobile_projection_enabled_;
  bool webengine_projection_enabled_;

  bool video_streaming_approved_;
  bool audio_streaming_approved_;
  bool video_streaming_allowed_;
  bool audio_streaming_allowed_;
  bool video_streaming_suspended_;
  bool audio_streaming_suspended_;
  sync_primitives::Lock video_streaming_suspended_lock_;
  sync_primitives::Lock audio_streaming_suspended_lock_;
  sync_primitives::Lock streaming_stop_lock_;

  bool is_app_allowed_;
  bool is_app_data_resumption_allowed_;
  bool has_been_activated_;
  bool is_ready_;
  bool tts_properties_in_none_;
  bool tts_properties_in_full_;
  bool keep_context_;
  bool is_foreground_;
  bool is_application_data_changed_;
  uint32_t put_file_in_none_count_;
  uint32_t delete_file_in_none_count_;
  uint32_t list_files_in_none_count_;
  std::string app_icon_path_;
  std::string mac_address_;
  connection_handler::DeviceHandle device_id_;
  connection_handler::DeviceHandle secondary_device_id_;
  std::string bundle_id_;
  AppFilesMap app_files_;
  std::set<mobile_apis::ButtonName::eType> subscribed_buttons_;
  UsageStatistics usage_report_;
  HelpPromptManagerImpl help_prompt_manager_impl_;
  protocol_handler::MajorProtocolVersion protocol_version_;
  bool is_voice_communication_application_;
  std::atomic_bool is_resuming_;
  mobile_api::HMILevel::eType deferred_resumption_hmi_level_;
  bool is_hash_changed_during_suspend_;
  smart_objects::SmartObject user_location_;

  uint32_t video_stream_retry_number_;
  uint32_t audio_stream_retry_number_;
  uint32_t video_stream_suspend_timeout_;
  uint32_t audio_stream_suspend_timeout_;
  Timer video_stream_suspend_timer_;
  Timer audio_stream_suspend_timer_;

  std::list<AppExtensionPtr> extensions_;

  // Cloud app properties
  std::string endpoint_;
  std::string auth_token_;
  std::string cloud_transport_type_;
  mobile_apis::HybridAppPreference::eType hybrid_app_preference_;
  std::string certificate_;

  /**
   * @brief Defines number per time in seconds limits
   */
  typedef std::pair<date_time::TimeDuration, uint32_t> TimeToNumberLimit;

  /**
   * @brief Defines specific command number per time in seconds limits
   */
  typedef std::map<mobile_apis::FunctionID::eType, TimeToNumberLimit>
      CommandNumberTimeLimit;

  /**
   * @brief Defines id of SoftButton which is related from name of command
   */
  typedef std::map<int32_t, SoftButtonIDs> CommandSoftButtonID;
  CommandNumberTimeLimit cmd_number_to_time_limits_;
  CommandSoftButtonID cmd_softbuttonid_;
  // Lock for command soft button id
  sync_primitives::Lock cmd_softbuttonid_lock_;
  mutable std::shared_ptr<sync_primitives::Lock> vi_lock_ptr_;
  mutable std::shared_ptr<sync_primitives::Lock> button_lock_ptr_;
  std::string folder_name_;
  ApplicationManager& application_manager_;

  sync_primitives::Lock mobile_message_lock_;
  MobileMessageQueue mobile_message_queue_;

  friend void SwitchApplicationParameters(ApplicationSharedPtr app,
                                          const uint32_t app_id,
                                          const size_t device_id,
                                          const std::string& mac_address);

  DISALLOW_COPY_AND_ASSIGN(ApplicationImpl);
};

uint32_t ApplicationImpl::hmi_app_id() const {
  return hmi_app_id_;
}

uint32_t ApplicationImpl::app_id() const {
  return app_id_;
}

const mobile_api::AudioStreamingState::eType
ApplicationImpl::audio_streaming_state() const {
  using namespace mobile_apis;
  const HmiStatePtr hmi_state =
      CurrentHmiState(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  return hmi_state ? hmi_state->audio_streaming_state()
                   : AudioStreamingState::INVALID_ENUM;
}

const mobile_api::VideoStreamingState::eType
ApplicationImpl::video_streaming_state() const {
  using namespace mobile_apis;
  const HmiStatePtr hmi_state =
      CurrentHmiState(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  return hmi_state ? hmi_state->video_streaming_state()
                   : VideoStreamingState::INVALID_ENUM;
}

bool ApplicationImpl::app_allowed() const {
  return is_app_allowed_;
}

bool ApplicationImpl::IsRegistered() const {
  return app_state_ == kRegistered;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
