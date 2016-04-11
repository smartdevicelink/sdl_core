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
      const std::string& mobile_app_id,
      const std::string& mac_address,
      const custom_str::CustomString& app_name,
      utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager);

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
  void set_is_navi(bool allow);

  bool video_streaming_approved() const;
  void set_video_streaming_approved(bool state);
  bool audio_streaming_approved() const;
  void set_audio_streaming_approved(bool state);

  bool video_streaming_allowed() const;
  void set_video_streaming_allowed(bool state);
  bool audio_streaming_allowed() const;
  void set_audio_streaming_allowed(bool state);

  void StartStreaming(
      protocol_handler::ServiceType service_type);
  void StopStreamingForce(
      protocol_handler::ServiceType service_type);
  void StopStreaming(
      protocol_handler::ServiceType service_type);
  void SuspendStreaming(
      protocol_handler::ServiceType service_type);
  void WakeUpStreaming(
      protocol_handler::ServiceType service_type);

  virtual bool is_voice_communication_supported() const;
  virtual void set_voice_communication_supported(
      bool is_voice_communication_supported);
  inline bool app_allowed() const;
  bool has_been_activated() const;
  bool set_activated(bool is_active);

  const Version& version() const;
  void set_hmi_application_id(uint32_t hmi_app_id);
  inline uint32_t hmi_app_id() const;
  inline uint32_t app_id() const;
  const custom_str::CustomString& name() const;
  void set_folder_name(const std::string& folder_name) OVERRIDE;
  const std::string folder_name() const;
  bool is_media_application() const;
  virtual bool is_foreground() const;
  virtual void set_foreground(bool is_foreground);
  virtual const mobile_api::HMILevel::eType hmi_level() const;
  const uint32_t put_file_in_none_count() const;
  const uint32_t delete_file_in_none_count() const;
  const uint32_t list_files_in_none_count() const;
  const mobile_api::SystemContext::eType system_context() const;
  inline const mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const;
  const std::string& app_icon_path() const;
  connection_handler::DeviceHandle device() const;
  const std::string& mac_address() const OVERRIDE;
  void set_tts_properties_in_none(bool active);
  bool tts_properties_in_none();
  void set_tts_properties_in_full(bool active);
  bool tts_properties_in_full();
  void set_version(const Version& ver);
  void set_name(const custom_str::CustomString& name);
  void set_is_media_application(bool is_media);
  void increment_put_file_in_none_count();
  void increment_delete_file_in_none_count();
  void increment_list_files_in_none_count();
  bool set_app_icon_path(const std::string& path);
  void set_app_allowed(const bool& allowed);
  void set_device(connection_handler::DeviceHandle device);
  virtual uint32_t get_grammar_id() const;
  virtual void set_grammar_id(uint32_t value);

  virtual void set_protocol_version(const ProtocolVersion& protocol_version);
  virtual ProtocolVersion protocol_version() const;

  virtual void set_is_resuming(bool is_resuming);
  virtual bool is_resuming() const;

  bool AddFile(const AppFile& file);
  bool UpdateFile(const AppFile& file);
  bool DeleteFile(const std::string& file_name);
  virtual const AppFilesMap& getAppFiles() const;

  virtual const AppFile* GetFile(const std::string& file_name);

  bool SubscribeToButton(mobile_apis::ButtonName::eType btn_name);
  bool IsSubscribedToButton(mobile_apis::ButtonName::eType btn_name);
  bool UnsubscribeFromButton(mobile_apis::ButtonName::eType btn_name);

  bool SubscribeToIVI(uint32_t vehicle_info_type) OVERRIDE;
  bool IsSubscribedToIVI(uint32_t vehicle_info_type) const OVERRIDE;
  bool UnsubscribeFromIVI(uint32_t vehicle_info_type) OVERRIDE;
  DataAccessor<VehicleInfoSubscriptions> SubscribedIVI() const OVERRIDE;

  /**
   * @brief ResetDataInNone reset data counters in NONE
   */
  virtual void ResetDataInNone();

  virtual DataAccessor<ButtonSubscriptions> SubscribedButtons() const OVERRIDE;

  virtual const std::string& curHash() const;
  /**
   * @brief Change Hash for current application
   * and send notification to mobile
   * @return updated_hash
   */
  virtual void UpdateHash();

  UsageStatistics& usage_report();

  bool IsCommandLimitsExceeded(mobile_apis::FunctionID::eType cmd_id,
                               TLimitSource source);
  virtual void SubscribeToSoftButtons(int32_t cmd_id,
                                      const SoftButtonID& softbuttons_id);
  virtual bool IsSubscribedToSoftButton(const uint32_t softbutton_id);

  virtual void UnsubscribeFromSoftButtons(int32_t cmd_id);

  virtual bool is_application_data_changed() const;

  virtual void set_is_application_data_changed(bool state_application_data);

  /**
   * @brief Check's if it is media, voice communication or navigation
   * application
   *
   * @return true if application is media, voice communication or navigation
   */
  virtual bool IsAudioApplication() const;

  /**
   * @brief Load persistent files from application folder.
   */
  virtual void LoadPersistentFiles();

  /**
  * @brief SetRegularState set permanent state of application
  *
  * @param state state to setup
  */
  virtual void SetRegularState(HmiStatePtr state);

  /**
  * @brief SetPostponedState sets postponed state to application.
  * This state could be set as regular later
  *
  * @param state state to setup
  */
  virtual void SetPostponedState(HmiStatePtr state);

  virtual void RemovePostponedState();

  /**
   * @brief AddHMIState the function that will change application's
   * hmi state.
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state new hmi state for certain application.
   */
  virtual void AddHMIState(HmiStatePtr state);

  /**
   * @brief RemoveHMIState the function that will turn back hmi_level after end
   * of some event
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state_id that should be removed
   */
  virtual void RemoveHMIState(HmiState::StateID state_id);

  /**
   * @brief HmiState of application within active events PhoneCall, TTS< etc ...
   * @return Active HmiState of application
   */
  virtual const HmiStatePtr CurrentHmiState() const;

  /**
   * @brief RegularHmiState of application without active events VR, TTS etc ...
   * @return HmiState of application
   */
  virtual const HmiStatePtr RegularHmiState() const;

  /**
   * @brief PostponedHmiState returns postponed hmi state of application
   * if it's present
   *
   * @return Postponed hmi state of application
   */
  virtual const HmiStatePtr PostponedHmiState() const;

  uint32_t audio_stream_retry_number() const;

  void set_audio_stream_retry_number(const uint32_t& audio_stream_retry_number);

  uint32_t video_stream_retry_number() const;

  void set_video_stream_retry_number(const uint32_t& video_stream_retry_number);

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

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
