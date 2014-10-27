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

#include <string>
#include <strings.h>
#include <stdlib.h>
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "utils/logger.h"

namespace {

mobile_apis::FileType::eType StringToFileType(const char* str) {
  if (0 == strcasecmp(str, "json")) {
    return mobile_apis::FileType::JSON;
  } else if (0 == strcasecmp(str, "bmp")) {
    return mobile_apis::FileType::GRAPHIC_BMP;
  } else if (0 == strcasecmp(str, "jpeg")) {
    return mobile_apis::FileType::GRAPHIC_JPEG;
  } else if (0 == strcasecmp(str, "png")) {
    return mobile_apis::FileType::GRAPHIC_PNG;
  } else if (0 == strcasecmp(str, "wave")) {
    return mobile_apis::FileType::AUDIO_WAVE;
  } else if ((0 == strcasecmp(str, "m4a")) ||
             (0 == strcasecmp(str, "m4b")) ||
             (0 == strcasecmp(str, "m4p")) ||
             (0 == strcasecmp(str, "m4v")) ||
             (0 == strcasecmp(str, "m4r")) ||
             (0 == strcasecmp(str, "3gp")) ||
             (0 == strcasecmp(str, "mp4")) ||
             (0 == strcasecmp(str, "aac"))) {
    return mobile_apis::FileType::AUDIO_AAC;
  } else if (0 == strcasecmp(str, "mp3")) {
    return mobile_apis::FileType::AUDIO_MP3;
  } else {
    return mobile_apis::FileType::BINARY;
  }
}
}

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

ApplicationImpl::ApplicationImpl(uint32_t application_id,
    const std::string& mobile_app_id,
    const std::string& app_name,
    utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager)
    : grammar_id_(0),
      app_id_(application_id),
      active_message_(NULL),
      is_media_(false),
      allowed_support_navigation_(false),
      hmi_supports_navi_video_streaming_(false),
      hmi_supports_navi_audio_streaming_(false),
      is_app_allowed_(true),
      has_been_activated_(false),
      tts_speak_state_(false),
      tts_properties_in_none_(false),
      tts_properties_in_full_(false),
      hmi_level_(mobile_api::HMILevel::HMI_NONE),
      put_file_in_none_count_(0),
      delete_file_in_none_count_(0),
      list_files_in_none_count_(0),
      system_context_(mobile_api::SystemContext::SYSCTXT_MAIN),
      audio_streaming_state_(mobile_api::AudioStreamingState::NOT_AUDIBLE),
      device_(0),
      usage_report_(mobile_app_id, statistics_manager),
      protocol_version_(ProtocolVersion::kV3),
      is_voice_communication_application_(false),
      is_video_stream_retry_active_(false),
      is_audio_stream_retry_active_(false),
      video_stream_retry_number_(0),
      audio_stream_retry_number_(0) {

  cmd_number_to_time_limits_[mobile_apis::FunctionID::ReadDIDID] =
      {date_time::DateTime::getCurrentTime(), 0};
  cmd_number_to_time_limits_[mobile_apis::FunctionID::GetVehicleDataID] =
      {date_time::DateTime::getCurrentTime(), 0};


  set_mobile_app_id(smart_objects::SmartObject(mobile_app_id));
  set_name(app_name);

  // subscribe application to custom button by default
  SubscribeToButton(mobile_apis::ButtonName::CUSTOM_BUTTON);

  // load persistent files
  LoadPersistentFiles();
}

ApplicationImpl::~ApplicationImpl() {
  // TODO(AK): check if this is correct assimption
  if (active_message_) {
    delete active_message_;
    active_message_ = NULL;
  }

  subscribed_buttons_.clear();
  subscribed_vehicle_info_.clear();
  if (is_perform_interaction_active()) {
    set_perform_interaction_active(0);
    set_perform_interaction_mode(-1);
    DeletePerformInteractionChoiceSetMap();
  }
  CleanupFiles();
}

void ApplicationImpl::CloseActiveMessage() {
  delete active_message_;
  active_message_ = NULL;
}

bool ApplicationImpl::IsFullscreen() const {
  return mobile_api::HMILevel::HMI_FULL == hmi_level_;
}

bool ApplicationImpl::MakeFullscreen() {
  hmi_level_ = mobile_api::HMILevel::HMI_FULL;
  if (is_media_ && !tts_speak_state_) {
    audio_streaming_state_ = mobile_api::AudioStreamingState::AUDIBLE;
  }
  system_context_ = mobile_api::SystemContext::SYSCTXT_MAIN;
  if (!has_been_activated_) {
    has_been_activated_ = true;
  }
  return true;
}

bool ApplicationImpl::IsAudible() const {
  return mobile_api::HMILevel::HMI_FULL == hmi_level_
      || mobile_api::HMILevel::HMI_LIMITED == hmi_level_;
}

void ApplicationImpl::MakeNotAudible() {
  hmi_level_ = mobile_api::HMILevel::HMI_BACKGROUND;
  audio_streaming_state_ = mobile_api::AudioStreamingState::NOT_AUDIBLE;
}

bool ApplicationImpl::allowed_support_navigation() const {
  return allowed_support_navigation_;
}

void ApplicationImpl::set_allowed_support_navigation(bool allow) {
  allowed_support_navigation_ = allow;
}

bool ApplicationImpl::is_voice_communication_supported() const {
  return is_voice_communication_application_;
}

void ApplicationImpl::set_voice_communication_supported(
    bool is_voice_communication_supported) {
  is_voice_communication_application_ = is_voice_communication_supported;
}

bool ApplicationImpl::IsAudioApplication() const {
  return is_media_ ||
         is_voice_communication_application_ ||
         allowed_support_navigation_;
}

const smart_objects::SmartObject* ApplicationImpl::active_message() const {
  return active_message_;
}

const Version& ApplicationImpl::version() const {
  return version_;
}

void ApplicationImpl::set_hmi_application_id(uint32_t hmi_app_id) {
  hmi_app_id_ = hmi_app_id;
}

const std::string& ApplicationImpl::name() const {
  return app_name_;
}

const std::string ApplicationImpl::folder_name() const {
  return name() + mobile_app_id()->asString();
}

bool ApplicationImpl::is_media_application() const {
  return is_media_;
}

const mobile_api::HMILevel::eType& ApplicationImpl::hmi_level() const {
  return hmi_level_;
}

const uint32_t ApplicationImpl::put_file_in_none_count() const {
  return put_file_in_none_count_;
}

const uint32_t ApplicationImpl::delete_file_in_none_count() const {
  return delete_file_in_none_count_;
}

const uint32_t ApplicationImpl::list_files_in_none_count() const {
  return list_files_in_none_count_;
}

const mobile_api::SystemContext::eType&
ApplicationImpl::system_context() const {
  return system_context_;
}

const std::string& ApplicationImpl::app_icon_path() const {
  return app_icon_path_;
}

connection_handler::DeviceHandle ApplicationImpl::device() const {
  return device_;
}

void ApplicationImpl::set_version(const Version& ver) {
  version_ = ver;
}

void ApplicationImpl::set_name(const std::string& name) {
  app_name_ = name;
}

void ApplicationImpl::set_is_media_application(bool is_media) {
  is_media_ = is_media;
  // Audio streaming state for non-media application can not be different
  // from NOT_AUDIBLE
  if (!is_media)
    set_audio_streaming_state(mobile_api::AudioStreamingState::NOT_AUDIBLE);
}

void ApplicationImpl::set_tts_speak_state(bool state_tts_speak) {
  tts_speak_state_ = state_tts_speak;
}

bool ApplicationImpl::tts_speak_state() {
  return tts_speak_state_;
}

void ApplicationImpl::set_tts_properties_in_none(
    bool active) {
  tts_properties_in_none_ = active;
}

bool ApplicationImpl::tts_properties_in_none() {
  return tts_properties_in_none_;
}

void ApplicationImpl::set_tts_properties_in_full(
    bool active) {
  tts_properties_in_full_ = active;
}

bool ApplicationImpl::tts_properties_in_full() {
  return tts_properties_in_full_;
}

void ApplicationImpl::set_hmi_level(
    const mobile_api::HMILevel::eType& hmi_level) {
  if (mobile_api::HMILevel::HMI_NONE != hmi_level_ &&
      mobile_api::HMILevel::HMI_NONE == hmi_level) {
    put_file_in_none_count_ = 0;
    delete_file_in_none_count_ = 0;
    list_files_in_none_count_ = 0;
  }

  hmi_level_ = hmi_level;
  usage_report_.RecordHmiStateChanged(hmi_level);
}

void ApplicationImpl::set_hmi_supports_navi_video_streaming(bool supports) {
  hmi_supports_navi_video_streaming_ = supports;

  if ((!supports) && (!video_stream_retry_active())) {
    std::pair<uint32_t, int32_t> stream_retry =
        profile::Profile::instance()->start_stream_retry_amount();
    set_video_stream_retry_active(true);
    video_stream_retry_number_ = stream_retry.first;
    video_stream_retry_timer_ =
        utils::SharedPtr<timer::TimerThread<ApplicationImpl>>(
            new timer::TimerThread<ApplicationImpl>(
                "VideoStreamRetry", this, &ApplicationImpl::OnVideoStreamRetry, true));
    // start separate pthread for timer without delays
    video_stream_retry_timer_->start(0);
  }
}

bool ApplicationImpl::hmi_supports_navi_video_streaming() const {
  return hmi_supports_navi_video_streaming_;
}

void ApplicationImpl::set_hmi_supports_navi_audio_streaming(bool supports) {
  hmi_supports_navi_audio_streaming_ = supports;

  if ((!supports) && (!audio_stream_retry_active())) {
    std::pair<uint32_t, int32_t> stream_retry =
        profile::Profile::instance()->start_stream_retry_amount();
    set_audio_stream_retry_active(true);
    audio_stream_retry_number_ = stream_retry.first;
    audio_stream_retry_timer_ =
        utils::SharedPtr<timer::TimerThread<ApplicationImpl>>(
            new timer::TimerThread<ApplicationImpl>(
                "AudioStreamRetry", this, &ApplicationImpl::OnAudioStreamRetry, true));
    // start separate pthread for timer without delays
    audio_stream_retry_timer_->start(0);
  }
}

bool ApplicationImpl::hmi_supports_navi_audio_streaming() const {
  return hmi_supports_navi_audio_streaming_;
}

bool ApplicationImpl::video_stream_retry_active() const {
  return is_video_stream_retry_active_;
}

void ApplicationImpl::set_video_stream_retry_active(bool active) {
  is_video_stream_retry_active_ = active;
}

bool ApplicationImpl::audio_stream_retry_active() const {
  return is_audio_stream_retry_active_;
}

void ApplicationImpl::set_audio_stream_retry_active(bool active) {
  is_audio_stream_retry_active_ = active;
}

void ApplicationImpl::OnVideoStreamRetry() {
  if (video_stream_retry_number_) {
    LOG4CXX_INFO(logger_, "Send video stream retry "
                 << video_stream_retry_number_);

    application_manager::MessageHelper::SendNaviStartStream(app_id());
    --video_stream_retry_number_;

    std::pair<uint32_t, int32_t> stream_retry =
        profile::Profile::instance()->start_stream_retry_amount();
    int32_t time_out = stream_retry.second;
    video_stream_retry_timer_->updateTimeOut(time_out);
  } else {
    LOG4CXX_INFO(logger_, "Stop video streaming retry");
    video_stream_retry_timer_.release();
    set_video_stream_retry_active(false);
  }
}

void ApplicationImpl::OnAudioStreamRetry() {
  if (audio_stream_retry_number_) {
    LOG4CXX_INFO(logger_, "Send audio streaming retry "
                 << audio_stream_retry_number_);

    application_manager::MessageHelper::SendAudioStartStream(app_id());
    --audio_stream_retry_number_;

    std::pair<uint32_t, int32_t> stream_retry =
        profile::Profile::instance()->start_stream_retry_amount();
    int32_t time_out = stream_retry.second;
    audio_stream_retry_timer_->updateTimeOut(time_out);
  } else {
    LOG4CXX_INFO(logger_, "Stop audio streaming retry");
    audio_stream_retry_timer_.release();
    set_audio_stream_retry_active(false);
  }
}

void ApplicationImpl::increment_put_file_in_none_count() {
  ++put_file_in_none_count_;
}

void ApplicationImpl::increment_delete_file_in_none_count() {
  ++delete_file_in_none_count_;
}

void ApplicationImpl::increment_list_files_in_none_count() {
  ++list_files_in_none_count_;
}

void ApplicationImpl::set_system_context(
    const mobile_api::SystemContext::eType& system_context) {
  system_context_ = system_context;
}

void ApplicationImpl::set_audio_streaming_state(
    const mobile_api::AudioStreamingState::eType& state) {
  if (!is_media_application()
      && state != mobile_api::AudioStreamingState::NOT_AUDIBLE) {
    LOG4CXX_WARN(logger_, "Trying to set audio streaming state"
                  " for non-media application to different from NOT_AUDIBLE");
    return;
  }
  audio_streaming_state_ = state;
}

bool ApplicationImpl::set_app_icon_path(const std::string& path) {
  if (app_files_.find(path) != app_files_.end()) {
    app_icon_path_ = path;
    return true;
  }
  return false;
}

void ApplicationImpl::set_app_allowed(const bool& allowed) {
  is_app_allowed_ = allowed;
}

void ApplicationImpl::set_device(connection_handler::DeviceHandle device) {
  device_ = device;
}

uint32_t ApplicationImpl::get_grammar_id() const {
  return grammar_id_;
}

void ApplicationImpl::set_grammar_id(uint32_t value) {
  grammar_id_ = value;
}

bool ApplicationImpl::has_been_activated() const {
  return has_been_activated_;
}

void ApplicationImpl::set_protocol_version(
    const ProtocolVersion& protocol_version) {
  protocol_version_ = protocol_version;
}

ProtocolVersion ApplicationImpl::protocol_version() const {
  return protocol_version_;
}

bool ApplicationImpl::AddFile(AppFile& file) {
  if (app_files_.count(file.file_name) == 0) {
    LOG4CXX_INFO(logger_, "AddFile file " << file.file_name
                           << " File type is " << file.file_type);
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::UpdateFile(AppFile& file) {
  if (app_files_.count(file.file_name) != 0) {
    LOG4CXX_INFO(logger_, "UpdateFile file " << file.file_name
                           << " File type is " << file.file_type);
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::DeleteFile(const std::string& file_name) {
  AppFilesMap::iterator it = app_files_.find(file_name);
  if (it != app_files_.end()) {
    LOG4CXX_INFO(logger_, "DeleteFile file " << it->second.file_name
                           << " File type is " << it->second.file_type);
    app_files_.erase(it);
    return true;
  }
  return false;
}

const AppFilesMap& ApplicationImpl::getAppFiles() const {
  return this->app_files_;
}

const AppFile* ApplicationImpl::GetFile(const std::string& file_name) {
   if (app_files_.find(file_name) != app_files_.end()) {
     return &(app_files_[file_name]);
   }
   return NULL;
}

bool ApplicationImpl::SubscribeToButton(mobile_apis::ButtonName::eType btn_name) {
  size_t old_size = subscribed_buttons_.size();
  subscribed_buttons_.insert(btn_name);
  return (subscribed_buttons_.size() == old_size + 1);
}

bool ApplicationImpl::IsSubscribedToButton(mobile_apis::ButtonName::eType btn_name) {
  std::set<mobile_apis::ButtonName::eType>::iterator it = subscribed_buttons_.find(btn_name);
  return (subscribed_buttons_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromButton(mobile_apis::ButtonName::eType btn_name) {
  size_t old_size = subscribed_buttons_.size();
  subscribed_buttons_.erase(btn_name);
  return (subscribed_buttons_.size() == old_size - 1);
}

bool ApplicationImpl::SubscribeToIVI(uint32_t vehicle_info_type_) {
  size_t old_size = subscribed_vehicle_info_.size();
  subscribed_vehicle_info_.insert(vehicle_info_type_);
  return (subscribed_vehicle_info_.size() == old_size + 1);
}

bool ApplicationImpl::IsSubscribedToIVI(uint32_t vehicle_info_type_) {
  std::set<uint32_t>::iterator it = subscribed_vehicle_info_.find(
      vehicle_info_type_);
  return (subscribed_vehicle_info_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromIVI(uint32_t vehicle_info_type_) {
  size_t old_size = subscribed_vehicle_info_.size();
  subscribed_vehicle_info_.erase(vehicle_info_type_);
  return (subscribed_vehicle_info_.size() == old_size - 1);
}

UsageStatistics& ApplicationImpl::usage_report() {
  return usage_report_;
}

bool ApplicationImpl::IsCommandLimitsExceeded(
    mobile_apis::FunctionID::eType cmd_id,
    TLimitSource source) {
  TimevalStruct current = date_time::DateTime::getCurrentTime();
  switch (source) {
  // In case of config file values there is COMMON limitations for number of
  // commands per certain time in seconds, i.e. 5 requests per 10 seconds with
  // any interval between them
  case CONFIG_FILE: {
    CommandNumberTimeLimit::iterator it =
        cmd_number_to_time_limits_.find(cmd_id);
    if (cmd_number_to_time_limits_.end() == it) {
      LOG4CXX_WARN(logger_, "Limits for command id " << cmd_id
                   << "had not been set.");
      return true;
    }

    TimeToNumberLimit& limit = it->second;

    std::pair<uint32_t, int32_t> frequency_restrictions;

    if (mobile_apis::FunctionID::ReadDIDID == cmd_id) {
      frequency_restrictions =
          profile::Profile::instance()->read_did_frequency();

    } else if (mobile_apis::FunctionID::GetVehicleDataID == cmd_id) {
      frequency_restrictions =
          profile::Profile::instance()->get_vehicle_data_frequency();
    } else {
      LOG4CXX_INFO(logger_, "No restrictions for request");
      return false;
    }

    LOG4CXX_INFO(logger_, "Time Info: " <<
                 "\n Current: " << current.tv_sec <<
                 "\n Limit: (" << limit.first.tv_sec << "," << limit.second << ")"
                 "\n frequency_restrictions: (" << frequency_restrictions.first << "," << frequency_restrictions.second << ")"
                 );
    if (current.tv_sec < limit.first.tv_sec + frequency_restrictions.second) {
      if (limit.second < frequency_restrictions.first) {
        ++limit.second;
        return false;
      }
      return true;
    }

    limit.first = current;
    limit.second = 1;

    return false;

    break;
  }
  // In case of policy table values, there is EVEN limitation for number of
  // commands per minute, e.g. 10 command per minute i.e. 1 command per 6 sec
  case POLICY_TABLE: {
    uint32_t cmd_limit = application_manager::MessageHelper::GetAppCommandLimit(
          mobile_app_id_->asString());

    if (0 == cmd_limit) {
      return true;
    }

    const uint32_t dummy_limit = 1;
    CommandNumberTimeLimit::iterator it =
        cmd_number_to_time_limits_.find(cmd_id);
    // If no command with cmd_id had been executed yet, just add to limits
    if (cmd_number_to_time_limits_.end() == it) {
      cmd_number_to_time_limits_[cmd_id] = {current, dummy_limit};
      return false;
    }

    const uint32_t minute = 60;

    TimeToNumberLimit& limit = it->second;

    // Checking even limitation for command
    if (static_cast<uint32_t>(current.tv_sec - limit.first.tv_sec) <
        minute/cmd_limit) {
      return true;
    }

    cmd_number_to_time_limits_[cmd_id] = {current, dummy_limit};

    return false;
    break;
  }
  default: {
    LOG4CXX_WARN(logger_, "Limit source is not implemented.");
    break;
  }
  }

  return true;
}

const std::set<mobile_apis::ButtonName::eType>& ApplicationImpl::SubscribedButtons() const {
  return subscribed_buttons_;
}

const std::set<uint32_t>& ApplicationImpl::SubscribesIVI() const {
  return subscribed_vehicle_info_;
}

uint32_t ApplicationImpl::nextHash() {
  hash_val_ = rand();
  return hash_val_;
}

uint32_t ApplicationImpl::curHash() const {
  return hash_val_;
}

uint32_t ApplicationImpl::UpdateHash() {
  uint32_t new_hash= nextHash();
  MessageHelper::SendHashUpdateNotification(app_id());
  return new_hash;
}

void ApplicationImpl::CleanupFiles() {
  std::string directory_name =
      profile::Profile::instance()->app_storage_folder();
  directory_name += "/" + folder_name();

  if (file_system::DirectoryExists(directory_name)) {
    std::vector<std::string> files = file_system::ListFiles(
            directory_name);
    AppFilesMap::const_iterator app_files_it;

    std::vector<std::string>::const_iterator it = files.begin();
    for (; it != files.end(); ++it) {
      std::string file_name = directory_name;
      file_name += "/";
      file_name += *it;
      app_files_it = app_files_.find(file_name);
      if ((app_files_it == app_files_.end()) ||
          (!app_files_it->second.is_persistent)) {
        LOG4CXX_INFO(logger_, "DeleteFile file " << file_name);
          file_system::DeleteFile(file_name);
      }
    }

    file_system::RemoveDirectory(directory_name, false);
  }
  app_files_.clear();
}

void ApplicationImpl::LoadPersistentFiles() {
  std::string directory_name =
      profile::Profile::instance()->app_storage_folder();
  directory_name += "/" + folder_name();

  if (file_system::DirectoryExists(directory_name)) {
    std::vector<std::string> persistent_files =
        file_system::ListFiles(directory_name);

    std::vector<std::string>::const_iterator it = persistent_files.begin();
    for (; it != persistent_files.end(); ++it) {
      AppFile file;
      file.is_persistent = true;
      file.is_download_complete = true;
      file.file_name = directory_name;
      file.file_name += "/";
      file.file_name += *it;
      file.file_type = mobile_apis::FileType::BINARY;
      // Search file extension and convert it to the type
      std::size_t index = it->find_last_of('.');
      if (index != std::string::npos) {
        std::string file_type = it->substr(++index);
        file.file_type = StringToFileType(file_type.c_str());
      }

      LOG4CXX_INFO(logger_, "Loaded persistent file " << file.file_name
                             << " File type is " << file.file_type);
      AddFile(file);
    }
  }
}

void ApplicationImpl::SubscribeToSoftButtons(int32_t cmd_id,
                                        const SoftButtonID& softbuttons_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  if (static_cast<int32_t>(mobile_apis::FunctionID::ScrollableMessageID) == cmd_id) {
    CommandSoftButtonID::iterator it = cmd_softbuttonid_.find(cmd_id);
    if (cmd_softbuttonid_.end() == it) {
      cmd_softbuttonid_[cmd_id] = softbuttons_id;
    }
  } else {
    cmd_softbuttonid_[cmd_id] = softbuttons_id;
  }
}

bool ApplicationImpl::IsSubscribedToSoftButton(const uint32_t softbutton_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  CommandSoftButtonID::iterator it = cmd_softbuttonid_.begin();
  for (; it != cmd_softbuttonid_.end(); ++it) {
    if((it->second).find(softbutton_id) != (it->second).end()) {
      return true;
    }
  }
  return false;
}

void ApplicationImpl::UnsubscribeFromSoftButtons(int32_t cmd_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  CommandSoftButtonID::iterator it = cmd_softbuttonid_.find(cmd_id);
  if(it != cmd_softbuttonid_.end()) {
    cmd_softbuttonid_.erase(it);
  }
}

}  // namespace application_manager
