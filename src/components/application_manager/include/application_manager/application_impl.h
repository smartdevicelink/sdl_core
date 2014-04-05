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

#include "application_manager/application_data_impl.h"
#include "application_manager/usage_statistics.h"
#include "connection_handler/device.h"

namespace usage_statistics {
class StatisticsManager;
}  // namespace usage_statistics

namespace application_manager {

namespace mobile_api = mobile_apis;

class ApplicationImpl : public virtual InitialApplicationDataImpl,
    public virtual DynamicApplicationDataImpl {
 public:
  ApplicationImpl(uint32_t application_id,
                  const std::string& global_app_id,
                  usage_statistics::StatisticsManager* statistics_manager);
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
  bool allowed_support_navigation() const;
  void set_allowed_support_navigation(bool allow);
  bool hmi_supports_navi_streaming() const ;
  void set_hmi_supports_navi_streaming(const bool& supports);
  inline bool app_allowed() const;
  bool has_been_activated() const;

  const Version& version() const;
  inline uint32_t app_id() const;
  const std::string& name() const;
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
  virtual uint32_t get_grammar_id();
  virtual void set_grammar_id(uint32_t value);

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

 protected:
  void CleanupFiles();

 private:

  uint32_t hash_val_;
  uint32_t grammar_id_;


  smart_objects::SmartObject* active_message_;

  Version version_;
  uint32_t app_id_;
  std::string app_name_;
  bool is_media_;
  bool allowed_support_navigation_;
  bool is_app_allowed_;
  bool has_been_activated_;
  bool tts_speak_state_;
  bool hmi_supports_navi_streaming_;

  mobile_api::HMILevel::eType hmi_level_;
  uint32_t put_file_in_none_count_;
  uint32_t delete_file_in_none_count_;
  uint32_t list_files_in_none_count_;
  mobile_api::SystemContext::eType system_context_;
  mobile_api::AudioStreamingState::eType audio_streaming_state_;
  std::string app_icon_path_;
  connection_handler::DeviceHandle device_;

  AppFilesMap app_files_;
  std::set<mobile_apis::ButtonName::eType> subscribed_buttons_;
  std::set<uint32_t> subscribed_vehicle_info_;
  UsageStatistics usage_report_;
  DISALLOW_COPY_AND_ASSIGN(ApplicationImpl);
};

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
