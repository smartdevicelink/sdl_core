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
#include <stdlib.h>
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "utils/logger.h"


namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

ApplicationImpl::ApplicationImpl(
    uint32_t application_id,
    const std::string& global_app_id,
    usage_statistics::StatisticsManager* statistics_manager)
    : app_id_(application_id),
      active_message_(NULL),
      is_media_(false),
      hmi_supports_navi_streaming_(false),
      allowed_support_navigation_(false),
      hmi_level_(mobile_api::HMILevel::HMI_NONE),
      put_file_in_none_count_(0),
      delete_file_in_none_count_(0),
      list_files_in_none_count_(0),
      system_context_(mobile_api::SystemContext::SYSCTXT_MAIN),
      audio_streaming_state_(mobile_api::AudioStreamingState::NOT_AUDIBLE),
      is_app_allowed_(true),
      has_been_activated_(false),
      tts_speak_state_(false),
      device_(0),
      grammar_id_(0),
      usage_report_(global_app_id, statistics_manager),
      protocol_version_(ProtocolVersion::kV3) {

  // subscribe application to custom button by default
  SubscribeToButton(mobile_apis::ButtonName::CUSTOM_BUTTON);
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

void ApplicationImpl::set_hmi_supports_navi_streaming(const bool& supports) {
  hmi_supports_navi_streaming_ = supports;
}

bool ApplicationImpl::hmi_supports_navi_streaming() const {
  return hmi_supports_navi_streaming_;
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
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::UpdateFile(AppFile& file) {
  if (app_files_.count(file.file_name) != 0) {
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::DeleteFile(const std::string& file_name) {
  AppFilesMap::iterator it = app_files_.find(file_name);
  if (it != app_files_.end()) {
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

    for (std::vector<std::string>::const_iterator it = files.begin();
         it != files.end(); ++it) {
      std::string file_name = directory_name;
      file_name += "/";
      file_name += *it;
      app_files_it = app_files_.find(file_name);
      if ((app_files_it == app_files_.end()) ||
          (!app_files_it->second.is_persistent)) {
          file_system::DeleteFile(file_name);
      }
    }

    file_system::RemoveDirectory(directory_name, false);
  }
  app_files_.clear();
}

}  // namespace application_manager
