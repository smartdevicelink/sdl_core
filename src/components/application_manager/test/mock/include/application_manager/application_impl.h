/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "utils/date_time.h"
#include "application_manager/application_data_impl.h"
#include "application_manager/usage_statistics.h"
#include "connection_handler/device.h"
#include "utils/timer_thread.h"
#include "utils/lock.h"

namespace usage_statistics {
class StatisticsManager;
}  // namespace usage_statistics

namespace application_manager {
namespace mobile_api = mobile_apis;

class ApplicationImpl : public virtual InitialApplicationDataImpl,
    public virtual DynamicApplicationDataImpl {
 public:
  ApplicationImpl(uint32_t application_id,
                  const std::string& mobile_app_id,
                  const std::string& app_name,
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
  bool MakeFullscreen();
  bool IsAudible() const;
  void MakeNotAudible();

  // navi
  bool allowed_support_navigation() const;
  void set_allowed_support_navigation(bool allow);
  bool hmi_supports_navi_video_streaming() const;
  void set_hmi_supports_navi_video_streaming(bool supports);
  bool hmi_supports_navi_audio_streaming() const;
  void set_hmi_supports_navi_audio_streaming(bool supports);

  virtual bool is_voice_communication_supported() const;
  virtual void set_voice_communication_supported(
      bool is_voice_communication_supported);
  inline bool app_allowed() const;
  bool has_been_activated() const;

  const Version& version() const;
  void set_hmi_application_id(uint32_t hmi_app_id);
  inline uint32_t hmi_app_id() const;
  inline uint32_t app_id() const;
  const std::string& name() const;
  const std::string folder_name() const;
  bool is_media_application() const;
  const mobile_api::HMILevel::eType& hmi_level() const;
  const uint32_t put_file_in_none_count() const;
  const uint32_t delete_file_in_none_count() const;
  const uint32_t list_files_in_none_count() const;
  const mobile_api::SystemContext::eType& system_context() const;
  inline const mobile_api::AudioStreamingState::eType&
  audio_streaming_state() const;
  const std::string& app_icon_path() const;
  connection_handler::DeviceHandle device() const;
  void set_tts_speak_state(bool state_tts_speak);
  bool tts_speak_state();
  void set_tts_properties_in_none(bool active);
  bool tts_properties_in_none();
  void set_tts_properties_in_full(bool active);
  bool tts_properties_in_full();
  void set_version(const Version& ver);
  void set_name(const std::string& name);
  void set_is_media_application(bool is_media);
  void set_hmi_level(const mobile_api::HMILevel::eType& hmi_level);
  void increment_put_file_in_none_count();
  void increment_delete_file_in_none_count();
  void increment_list_files_in_none_count();
  void set_system_context(
      const mobile_api::SystemContext::eType& system_context);
  void set_audio_streaming_state(
      const mobile_api::AudioStreamingState::eType& state);
  bool set_app_icon_path(const std::string& path);
  void set_app_allowed(const bool& allowed);
  void set_device(connection_handler::DeviceHandle device);
  virtual uint32_t get_grammar_id() const;
  virtual void set_grammar_id(uint32_t value);

  virtual void set_protocol_version(const ProtocolVersion& protocol_version);
  virtual ProtocolVersion protocol_version() const;

  bool AddFile(AppFile& file);
  bool UpdateFile(AppFile& file);

  bool DeleteFile(const std::string& file_name);
  virtual const AppFilesMap& getAppFiles() const;

  virtual const AppFile* GetFile(const std::string& file_name);

  bool SubscribeToButton(mobile_apis::ButtonName::eType btn_name);
  bool IsSubscribedToButton(mobile_apis::ButtonName::eType btn_name);
  bool UnsubscribeFromButton(mobile_apis::ButtonName::eType btn_name);

  bool SubscribeToIVI(uint32_t vehicle_info_type_);
  bool IsSubscribedToIVI(uint32_t vehicle_info_type_);
  bool UnsubscribeFromIVI(uint32_t vehicle_info_type_);

  virtual const std::set<mobile_apis::ButtonName::eType>& SubscribedButtons() const;
  virtual const  std::set<uint32_t>& SubscribesIVI() const;

  virtual uint32_t nextHash();
  virtual uint32_t curHash() const;

  /**
   * @brief Change Hash for current application
   * and send notification to mobile
   * @return updated_hash
   */
  virtual uint32_t UpdateHash();

  UsageStatistics& usage_report();

  bool IsCommandLimitsExceeded(mobile_apis::FunctionID::eType cmd_id,
                               TLimitSource source);
  virtual void SubscribeToSoftButtons(int32_t cmd_id,
                                      const SoftButtonID& softbuttons_id);
  virtual bool IsSubscribedToSoftButton(const uint32_t softbutton_id);

  virtual void UnsubscribeFromSoftButtons(int32_t cmd_id);

  /**
   * @brief Check's if it is media, voice communication or navigation application
   *
   * @return true if application is media, voice communication or navigation
   */
  virtual bool IsAudioApplication() const;

 protected:

  /**
   * @brief Clean up application folder. Persistent files will stay
   */
  void CleanupFiles();

  /**
   * @brief Load persistent files from application folder.
   */
  void LoadPersistentFiles();

 private:

  // interfaces for NAVI retry sequence
  bool video_stream_retry_active() const;
  void set_video_stream_retry_active(bool active);
  bool audio_stream_retry_active() const;
  void set_audio_stream_retry_active(bool active);
  void OnVideoStreamRetry();
  void OnAudioStreamRetry();

  uint32_t                                 hash_val_;
  uint32_t                                 grammar_id_;


  Version version_;
  std::string                              app_name_;
  uint32_t                                 hmi_app_id_;
  uint32_t                                 app_id_;
  smart_objects::SmartObject*              active_message_;
  bool                                     is_media_;
  bool                                     allowed_support_navigation_;
  bool                                     hmi_supports_navi_video_streaming_;
  bool                                     hmi_supports_navi_audio_streaming_;
  bool                                     is_app_allowed_;
  bool                                     has_been_activated_;
  bool                                     tts_speak_state_;
  bool                                     tts_properties_in_none_;
  bool                                     tts_properties_in_full_;
  mobile_api::HMILevel::eType              hmi_level_;
  uint32_t                                 put_file_in_none_count_;
  uint32_t                                 delete_file_in_none_count_;
  uint32_t                                 list_files_in_none_count_;
  mobile_api::SystemContext::eType         system_context_;
  mobile_api::AudioStreamingState::eType   audio_streaming_state_;
  std::string                              app_icon_path_;
  connection_handler::DeviceHandle         device_;

  AppFilesMap                              app_files_;
  std::set<mobile_apis::ButtonName::eType> subscribed_buttons_;
  std::set<uint32_t>                       subscribed_vehicle_info_;
  UsageStatistics                          usage_report_;
  ProtocolVersion                          protocol_version_;
  bool                                     is_voice_communication_application_;

  // NAVI retry stream
  volatile bool                            is_video_stream_retry_active_;
  volatile bool                            is_audio_stream_retry_active_;
  uint32_t                                 video_stream_retry_number_;
  uint32_t                                 audio_stream_retry_number_;
  utils::SharedPtr<timer::TimerThread<ApplicationImpl>> video_stream_retry_timer_;
  utils::SharedPtr<timer::TimerThread<ApplicationImpl>> audio_stream_retry_timer_;


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
  typedef std::map<int32_t, SoftButtonID>
  CommandSoftButtonID;
  CommandNumberTimeLimit cmd_number_to_time_limits_;
  CommandSoftButtonID cmd_softbuttonid_;
  // Lock for command soft button id
  sync_primitives::Lock cmd_softbuttonid_lock_;
  DISALLOW_COPY_AND_ASSIGN(ApplicationImpl);
};

uint32_t ApplicationImpl::hmi_app_id() const {
  return hmi_app_id_;
}

uint32_t ApplicationImpl::app_id() const {
  return app_id_;
}

const mobile_api::AudioStreamingState::eType&
ApplicationImpl::audio_streaming_state() const {
  return audio_streaming_state_;
}

bool ApplicationImpl::app_allowed() const {
  return is_app_allowed_;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
