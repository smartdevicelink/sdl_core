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
#include "application_manager/application_impl.h"
#include "utils/file_system.h"

namespace application_manager {

ApplicationImpl::ApplicationImpl(unsigned int app_id)
  : app_id_(app_id),
    active_message_(NULL),
    is_media_(false),
    allowed_support_navigation_(false),
    hmi_level_(mobile_api::HMILevel::HMI_NONE),
    system_context_(mobile_api::SystemContext::SYSCTXT_MAIN),
    audio_streaming_state_(mobile_api::AudioStreamingState::NOT_AUDIBLE),
    is_app_allowed_(true),
    has_been_activated_(false),
    device_(0) {
}

ApplicationImpl::~ApplicationImpl() {
  // TODO(AK): check if this is correct assimption
  if (active_message_) {
    delete active_message_;
    active_message_ = NULL;
  }

  subscribed_buttons_.clear();
  subscribed_vehicle_info_.clear();

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
  if (is_media_) {
    audio_streaming_state_ = mobile_api::AudioStreamingState::AUDIBLE;
  }
  system_context_ = mobile_api::SystemContext::SYSCTXT_MAIN;
  if (!has_been_activated_) {
    has_been_activated_ = true;
  }
  return true;
}

bool ApplicationImpl::IsAudible() const {
  return mobile_api::HMILevel::HMI_FULL == hmi_level_ ||
         mobile_api::HMILevel::HMI_LIMITED == hmi_level_;
}

void ApplicationImpl::MakeNotAudible() {
  hmi_level_ = mobile_api::HMILevel::HMI_BACKGROUND;
  audio_streaming_state_ = mobile_api::AudioStreamingState::NOT_AUDIBLE;
}

bool ApplicationImpl::SupportsNavigation() const {
  return allowed_support_navigation_;
}

void ApplicationImpl::AllowNavigation(bool allow) {
  allowed_support_navigation_ = allow;
}

const smart_objects::SmartObject* ApplicationImpl::active_message() const {
  return active_message_;
}

const Version& ApplicationImpl::version() const {
  return version_;
}

const std::string& ApplicationImpl::name() const {
  return app_name_;
}

bool ApplicationImpl::is_media_application() const {
  return is_media_;
}

const mobile_api::HMILevel::eType& ApplicationImpl::hmi_level() const {
  return hmi_level_;
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

void ApplicationImpl::set_version(const Version& version) {
  version_ = version;
}

void ApplicationImpl::set_name(const std::string& name) {
  app_name_ = name;
}

void ApplicationImpl::set_is_media_application(bool is_media) {
  is_media_ = is_media;
}

void ApplicationImpl::set_hmi_level(
  const mobile_api::HMILevel::eType& hmi_level) {
  hmi_level_ = hmi_level;
}

void ApplicationImpl::set_system_context(
  const mobile_api::SystemContext::eType& system_context) {
  system_context_ = system_context;
}

void ApplicationImpl::set_audio_streaming_state(
  const mobile_api::AudioStreamingState::eType& state) {
  audio_streaming_state_ = state;
}
bool ApplicationImpl::set_app_icon_path(const std::string& file_name) {
  for (std::vector<AppFile>::iterator it = app_files_.begin();
       app_files_.end() != it;
       ++it) {
    if (0 == it->file_name.compare(file_name)) {
      app_icon_path_ = file_name;
      return true;
    }
  }
  return false;
}

void ApplicationImpl::set_app_allowed(const bool& allowed) {
  is_app_allowed_ = allowed;
}

void ApplicationImpl::set_device(connection_handler::DeviceHandle device) {
  device_ = device;
}

bool ApplicationImpl::HasBeenActivated() const {
  return has_been_activated_;
}

bool ApplicationImpl::AddFile(
  const std::string& file_name, bool is_persistent) {
  for (std::vector<AppFile>::iterator it = app_files_.begin();
       app_files_.end() != it;
       ++it) {
    if (0 == file_name.compare(it->file_name)) {
      return false;
    }
  }
  AppFile app_file(file_name, is_persistent);
  app_files_.push_back(app_file);
  return true;
}

bool ApplicationImpl::DeleteFile(const std::string& file_name) {
  for (std::vector<AppFile>::iterator it = app_files_.begin();
       app_files_.end() != it;
       ++it) {
    if (0 == it->file_name.compare(file_name)) {
      app_files_.erase(it);
      return true;
    }
  }
  return false;
}

bool ApplicationImpl::SubscribeToButton(unsigned int btn_name) {
  size_t old_size = subscribed_buttons_.size();
  subscribed_buttons_.insert(btn_name);
  return (subscribed_buttons_.size() == old_size + 1);
}

bool ApplicationImpl::IsSubscribedToButton(unsigned int btn_name) {
  std::set<unsigned int>::iterator it = subscribed_buttons_.find(btn_name);
  return (subscribed_buttons_.end() != it);
}
bool ApplicationImpl::UnsubscribeFromButton(unsigned int btn_name) {
  size_t old_size = subscribed_buttons_.size();
  subscribed_buttons_.erase(btn_name);
  return (subscribed_buttons_.size() == old_size - 1);
}

bool ApplicationImpl::SubscribeToIVI(unsigned int vehicle_info_type_) {
  size_t old_size = subscribed_vehicle_info_.size();
  subscribed_vehicle_info_.insert(vehicle_info_type_);
  return (subscribed_vehicle_info_.size() == old_size + 1);
}

bool ApplicationImpl::IsSubscribedToIVI(
  unsigned int vehicle_info_type_) {
  std::set<unsigned int>::iterator it = subscribed_vehicle_info_.find(
                                          vehicle_info_type_);
  return (subscribed_vehicle_info_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromIVI(
  unsigned int vehicle_info_type_) {
  size_t old_size = subscribed_vehicle_info_.size();
  subscribed_vehicle_info_.erase(vehicle_info_type_);
  return (subscribed_vehicle_info_.size() == old_size - 1);
}

void ApplicationImpl::CleanupFiles() {
  std::string directory_name = file_system::FullPath(name());
  if (file_system::DirectoryExists(directory_name)) {
    for (size_t i = 0; i < app_files_.size(); ++i) {
      if (!app_files_[i].is_persistent) {
        std::string file_name = directory_name;
        file_name += "/";
        file_name += app_files_[i].file_name;
        file_system::DeleteFile(file_name);
      }
    }
    std::vector<std::string> persistent_files = file_system::ListFiles(
          directory_name);
    if (0 == persistent_files.size()) {
      file_system::RemoveDirectory(directory_name);
    }
  }
  app_files_.clear();
}

}  // namespace application_manager
