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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_

#include <map>
#include <set>
#include <vector>
#include <utility>
#include <list>
#include <stdint.h>

#include "utils/date_time.h"
#include "application_manager/application_data_impl.h"
#include "application_manager/usage_statistics.h"
#include "application_manager/hmi_state.h"
#include "protocol_handler/protocol_handler.h"

#include "connection_handler/device.h"
#include "utils/lock.h"
#include "utils/atomic_object.h"
#include "utils/custom_string.h"
#include "utils/timer.h"

namespace usage_statistics {

class StatisticsManager;
}  // namespace usage_statistics

namespace application_manager {
using namespace utils;
using namespace timer;

namespace mobile_api = mobile_apis;
namespace custom_str = custom_string;

class ApplicationImpl : public virtual InitialApplicationDataImpl,
                        public virtual DynamicApplicationDataImpl {
 public:
  ApplicationImpl(
      uint32_t application_id,
      const std::string& policy_app_id,
      const std::string& mac_address,
      const custom_str::CustomString& app_name,
      utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
      ApplicationManager& application_manager);

  ~ApplicationImpl();

  /**
   * @brief Returns message belonging to the application
   * that is currently executed (i.e. on HMI).
   * @return smart_objects::SmartObject * Active message
   */
  const smart_objects::SmartObject* active_message() const OVERRIDE;
  void CloseActiveMessage() OVERRIDE;
  bool IsFullscreen() const OVERRIDE;

  /**
   * @brief change supporting COMMUNICATION NAVIGATION
   */
  void ChangeSupportingAppHMIType() OVERRIDE;
  ;

  inline bool is_navi() const OVERRIDE {
    return is_navi_;
  }
  void set_is_navi(bool allow) OVERRIDE;

  bool video_streaming_approved() const OVERRIDE;
  void set_video_streaming_approved(bool state) OVERRIDE;
  bool audio_streaming_approved() const OVERRIDE;
  void set_audio_streaming_approved(bool state) OVERRIDE;

  bool video_streaming_allowed() const OVERRIDE;
  void set_video_streaming_allowed(bool state) OVERRIDE;
  bool audio_streaming_allowed() const OVERRIDE;
  void set_audio_streaming_allowed(bool state) OVERRIDE;

  void StartStreaming(protocol_handler::ServiceType service_type) OVERRIDE;
  void StopStreamingForce(protocol_handler::ServiceType service_type) OVERRIDE;
  void StopStreaming(protocol_handler::ServiceType service_type) OVERRIDE;
  void SuspendStreaming(protocol_handler::ServiceType service_type) OVERRIDE;
  void WakeUpStreaming(protocol_handler::ServiceType service_type) OVERRIDE;

  bool is_voice_communication_supported() const OVERRIDE;
  ;
  virtual void set_voice_communication_supported(
      bool is_voice_communication_supported) OVERRIDE;
  inline bool app_allowed() const OVERRIDE;
  bool has_been_activated() const OVERRIDE;
  bool set_activated(bool is_active) OVERRIDE;

  const Version& version() const OVERRIDE;
  void set_hmi_application_id(uint32_t hmi_app_id) OVERRIDE;
  inline uint32_t hmi_app_id() const OVERRIDE;
  inline uint32_t app_id() const OVERRIDE;
  const custom_str::CustomString& name() const OVERRIDE;
  void set_folder_name(const std::string& folder_name) OVERRIDE;
  const std::string folder_name() const OVERRIDE;
  bool is_media_application() const OVERRIDE;
  bool is_foreground() const OVERRIDE;
  void set_foreground(const bool is_foreground) OVERRIDE;
  const mobile_apis::HMILevel::eType hmi_level() const OVERRIDE;
  const uint32_t put_file_in_none_count() const OVERRIDE;
  const uint32_t delete_file_in_none_count() const OVERRIDE;
  const uint32_t list_files_in_none_count() const OVERRIDE;
  const mobile_api::SystemContext::eType system_context() const OVERRIDE;
  inline const mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE;
  const std::string& app_icon_path() const OVERRIDE;
  connection_handler::DeviceHandle device() const OVERRIDE;
  const std::string& mac_address() const OVERRIDE;
  void set_tts_properties_in_none(bool active) OVERRIDE;
  bool tts_properties_in_none() OVERRIDE;
  void set_tts_properties_in_full(bool active) OVERRIDE;
  bool tts_properties_in_full() OVERRIDE;
  void set_version(const Version& ver) OVERRIDE;
  void set_name(const custom_str::CustomString& name) OVERRIDE;
  void set_is_media_application(bool is_media) OVERRIDE;
  void increment_put_file_in_none_count() OVERRIDE;
  void increment_delete_file_in_none_count() OVERRIDE;
  void increment_list_files_in_none_count() OVERRIDE;
  bool set_app_icon_path(const std::string& path) OVERRIDE;
  void set_app_allowed(const bool allowed) OVERRIDE;
  void set_device(connection_handler::DeviceHandle device) OVERRIDE;
  uint32_t get_grammar_id() const OVERRIDE;
  ;
  void set_grammar_id(uint32_t value) OVERRIDE;
  ;

  virtual void set_protocol_version(
      const ProtocolVersion& protocol_version) OVERRIDE;
  ProtocolVersion protocol_version() const OVERRIDE;
  ;

  void set_is_resuming(bool is_resuming) OVERRIDE;
  ;
  bool is_resuming() const OVERRIDE;
  ;

  bool AddFile(const AppFile& file) OVERRIDE;
  bool UpdateFile(const AppFile& file) OVERRIDE;
  bool DeleteFile(const std::string& file_name) OVERRIDE;
  const AppFilesMap& getAppFiles() const OVERRIDE;
  ;

  const AppFile* GetFile(const std::string& file_name) OVERRIDE;
  ;

  bool SubscribeToButton(mobile_apis::ButtonName::eType btn_name) OVERRIDE;
  bool IsSubscribedToButton(mobile_apis::ButtonName::eType btn_name) OVERRIDE;
  bool UnsubscribeFromButton(mobile_apis::ButtonName::eType btn_name) OVERRIDE;

  bool SubscribeToIVI(uint32_t vehicle_info_type) OVERRIDE;
  bool IsSubscribedToIVI(uint32_t vehicle_info_type) const OVERRIDE;
  bool UnsubscribeFromIVI(uint32_t vehicle_info_type) OVERRIDE;
  DataAccessor<VehicleInfoSubscriptions> SubscribedIVI() const OVERRIDE;
  inline bool IsRegistered() const OVERRIDE;

  /**
   * @brief ResetDataInNone reset data counters in NONE
   */
  void ResetDataInNone() OVERRIDE;
  ;

  DataAccessor<ButtonSubscriptions> SubscribedButtons() const OVERRIDE;
  ;

  const std::string& curHash() const OVERRIDE;
  ;
#ifdef CUSTOMER_PASA
  virtual bool flag_sending_hash_change_after_awake() const;
  void set_flag_sending_hash_change_after_awake(bool flag) OVERRIDE;
  ;
#endif  // CUSTOMER_PASA
        /**
         * @brief Change Hash for current application
         * and send notification to mobile
         * @return updated_hash
         */
  void UpdateHash() OVERRIDE;
  ;

  UsageStatistics& usage_report() OVERRIDE;

  bool AreCommandLimitsExceeded(mobile_apis::FunctionID::eType cmd_id,
                                TLimitSource source) OVERRIDE;
  virtual void SubscribeToSoftButtons(
      int32_t cmd_id, const SoftButtonID& softbuttons_id) OVERRIDE;
  bool IsSubscribedToSoftButton(const uint32_t softbutton_id) OVERRIDE;
  ;

  void UnsubscribeFromSoftButtons(int32_t cmd_id) OVERRIDE;
  ;

  bool is_application_data_changed() const OVERRIDE;
  ;

  virtual void set_is_application_data_changed(
      bool state_application_data) OVERRIDE;

  /**
   * @brief Check's if it is media, voice communication or navigation
   * application
   *
   * @return true if application is media, voice communication or navigation
   */
  bool IsAudioApplication() const OVERRIDE;
  ;

  /**
  * @brief SetRegularState set permanent state of application
  *
  * @param state state to setup
  */
  void SetRegularState(HmiStatePtr state) OVERRIDE;
  ;

  /**
  * @brief SetPostponedState sets postponed state to application.
  * This state could be set as regular later
  *
  * @param state state to setup
  */
  void SetPostponedState(HmiStatePtr state) OVERRIDE;
  ;

  void RemovePostponedState() OVERRIDE;
  ;

  /**
   * @brief AddHMIState the function that will change application's
   * hmi state.
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state new hmi state for certain application.
   */
  void AddHMIState(HmiStatePtr state) OVERRIDE;
  ;

  /**
   * @brief RemoveHMIState the function that will turn back hmi_level after end
   * of some event
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state_id that should be removed
   */
  void RemoveHMIState(HmiState::StateID state_id) OVERRIDE;
  ;

  /**
   * @brief HmiState of application within active events PhoneCall, TTS< etc ...
   * @return Active HmiState of application
   */
  const HmiStatePtr CurrentHmiState() const OVERRIDE;
  ;

  /**
   * @brief RegularHmiState of application without active events VR, TTS etc ...
   * @return HmiState of application
   */
  const HmiStatePtr RegularHmiState() const OVERRIDE;
  ;

  /**
   * @brief PostponedHmiState returns postponed hmi state of application
   * if it's present
   *
   * @return Postponed hmi state of application
   */
  const HmiStatePtr PostponedHmiState() const OVERRIDE;
  ;

  uint32_t audio_stream_retry_number() const OVERRIDE;

  void set_audio_stream_retry_number(
      const uint32_t& audio_stream_retry_number) OVERRIDE;

  uint32_t video_stream_retry_number() const OVERRIDE;

  void set_video_stream_retry_number(
      const uint32_t& video_stream_retry_number) OVERRIDE;

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

  bool IsAppSubscribedForWayPoints(const uint32_t app_id) const OVERRIDE;

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

  std::string hash_val_;
  uint32_t grammar_id_;

  Version version_;
  custom_str::CustomString app_name_;
  uint32_t hmi_app_id_;
  uint32_t app_id_;
  smart_objects::SmartObject* active_message_;
  bool is_media_;
  bool is_navi_;

  bool video_streaming_approved_;
  bool audio_streaming_approved_;
  bool video_streaming_allowed_;
  bool audio_streaming_allowed_;
  bool video_streaming_suspended_;
  bool audio_streaming_suspended_;
  sync_primitives::Lock video_streaming_suspended_lock_;
  sync_primitives::Lock audio_streaming_suspended_lock_;

  bool is_app_allowed_;
  bool has_been_activated_;
  bool tts_properties_in_none_;
  bool tts_properties_in_full_;
  bool is_foreground_;
  bool is_application_data_changed_;
  uint32_t put_file_in_none_count_;
  uint32_t delete_file_in_none_count_;
  uint32_t list_files_in_none_count_;
  std::string app_icon_path_;
  connection_handler::DeviceHandle device_;
  const std::string mac_address_;

  AppFilesMap app_files_;
  std::set<mobile_apis::ButtonName::eType> subscribed_buttons_;
  VehicleInfoSubscriptions subscribed_vehicle_info_;
  UsageStatistics usage_report_;
  ProtocolVersion protocol_version_;
  bool is_voice_communication_application_;
  sync_primitives::atomic_bool is_resuming_;

  uint32_t video_stream_retry_number_;
  uint32_t audio_stream_retry_number_;
  uint32_t video_stream_suspend_timeout_;
  uint32_t audio_stream_suspend_timeout_;
  Timer video_stream_suspend_timer_;
  Timer audio_stream_suspend_timer_;

  /**
   * @brief Defines number per time in seconds limits
   */
  typedef std::pair<TimevalStruct, uint32_t> TimeToNumberLimit;

  /**
   * @brief Defines specific command number per time in seconds limits
   */
  typedef std::map<mobile_apis::FunctionID::eType, TimeToNumberLimit>
      CommandNumberTimeLimit;

  /**
   * @brief Defines id of SoftButton which is related from name of command
   */
  typedef std::map<int32_t, SoftButtonID> CommandSoftButtonID;
  CommandNumberTimeLimit cmd_number_to_time_limits_;
  CommandSoftButtonID cmd_softbuttonid_;
  // Lock for command soft button id
  sync_primitives::Lock cmd_softbuttonid_lock_;
  mutable sync_primitives::Lock vi_lock_;
  sync_primitives::Lock button_lock_;
  std::string folder_name_;
  ApplicationManager& application_manager_;
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
  const HmiStatePtr hmi_state = CurrentHmiState();
  return hmi_state ? hmi_state->audio_streaming_state()
                   : AudioStreamingState::INVALID_ENUM;
}

bool ApplicationImpl::app_allowed() const {
  return is_app_allowed_;
}

bool ApplicationImpl::IsRegistered() const {
  return app_state_ == kRegistered;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
