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

InitialApplicationData::InitialApplicationData()
  : app_types_(NULL),
    vr_synonyms_(NULL),
    ngn_media_screen_name_(NULL),
    mobile_app_id_(NULL),
    tts_name_(NULL) {
}

InitialApplicationData::~InitialApplicationData() {
  if (app_types_) {
    delete app_types_;
    app_types_ = NULL;
  }

  if (vr_synonyms_) {
    delete vr_synonyms_;
    vr_synonyms_ = NULL;
  }

  if (ngn_media_screen_name_) {
    delete ngn_media_screen_name_;
    ngn_media_screen_name_ = NULL;
  }

  if (mobile_app_id_) {
    delete mobile_app_id_;
    mobile_app_id_ = NULL;
  }

  if (tts_name_) {
    delete tts_name_;
    tts_name_ = NULL;
  }
}

ApplicationImpl::ApplicationImpl(int app_id)
  : app_id_(app_id),
    active_message_(NULL),
    is_media_(false),
    hmi_level_(mobile_api::HMILevel::INVALID_ENUM),
    system_context_(mobile_api::SystemContext::INVALID_ENUM),
    tbt_state_(mobile_api::TBTState::INVALID_ENUM),
    help_promt_(NULL),
    timeout_promt_(NULL),
    vr_help_title_(NULL),
    vr_help_(NULL) {
}

ApplicationImpl::~ApplicationImpl() {
  // TODO(AK): check if this is correct assimption
  if (active_message_) {
    delete active_message_;
    active_message_ = NULL;
  }

  if (help_promt_) {
    delete help_promt_;
    help_promt_ = NULL;
  }

  if (timeout_promt_) {
    delete timeout_promt_;
    timeout_promt_ = NULL;
  }

  if (vr_help_title_) {
    delete vr_help_title_;
    vr_help_title_ = NULL;
  }

  if (vr_help_) {
    delete vr_help_;
    vr_help_ = NULL;
  }

  for (CommandsMap::iterator command_it = commands_.begin();
       commands_.end() != command_it; ++command_it) {
    delete command_it->second;
  }

  commands_.clear();

  for (SubMenuMap::iterator sub_menu_it = sub_menu_.begin();
       sub_menu_.end() != sub_menu_it; ++sub_menu_it) {
    delete sub_menu_it->second;
  }

  sub_menu_.clear();

  CleanupFiles();
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

void ApplicationImpl::ProcessMessage(smart_objects::CSmartObject* message) {
}

void ApplicationImpl::ReportError(smart_objects::CSmartObject* message,
                                  ErrorCode error_code) {
}

const smart_objects::CSmartObject* ApplicationImpl::active_message() const {
  return active_message_;
}

void ApplicationImpl::CloseActiveMessage() {
  delete active_message_;
  active_message_ = NULL;
}

const Version& ApplicationImpl::version() const {
  return version_;
}

int ApplicationImpl::app_id() const {
  return app_id_;
}

const std::string& ApplicationImpl::name() const {
  return app_name_;
}

bool ApplicationImpl::is_media_application() const {
  return is_media_;
}

const smart_objects::CSmartObject* ApplicationImpl::app_types() const {
  return initial_app_data_.app_types_;
}

const smart_objects::CSmartObject* ApplicationImpl::vr_synonyms() const {
  return initial_app_data_.vr_synonyms_;
}

const smart_objects::CSmartObject* ApplicationImpl::mobile_app_id() const {
  return initial_app_data_.mobile_app_id_;
}

const smart_objects::CSmartObject* ApplicationImpl::tts_name() const {
  return initial_app_data_.tts_name_;
}

const smart_objects::CSmartObject*
ApplicationImpl::ngn_media_screen_name() const {
  return initial_app_data_.ngn_media_screen_name_;
}

const mobile_api::HMILevel::eType& ApplicationImpl::hmi_level() const {
  return hmi_level_;
}

const mobile_api::SystemContext::eType&
ApplicationImpl::system_context() const {
  return system_context_;
}

const mobile_api::Language::eType& ApplicationImpl::language() const {
  return initial_app_data_.language_;
}

const mobile_api::Language::eType& ApplicationImpl::ui_language() const {
  return initial_app_data_.ui_language_;
}

const smart_objects::CSmartObject* ApplicationImpl::help_promt() const {
  return help_promt_;
}

const smart_objects::CSmartObject* ApplicationImpl::timeout_promt() const {
  return timeout_promt_;
}

const smart_objects::CSmartObject* ApplicationImpl::vr_help_title() const {
  return vr_help_title_;
}

const smart_objects::CSmartObject* ApplicationImpl::vr_help() const {
  return vr_help_;
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

void ApplicationImpl::set_language(
  const mobile_api::Language::eType& language) {
  initial_app_data_.language_ = language;
}

void ApplicationImpl::set_ui_language(
  const mobile_api::Language::eType& ui_language) {
  initial_app_data_.ui_language_ = ui_language;
}

void ApplicationImpl::set_tts_name(
  const smart_objects::CSmartObject& tts_name) {
  if (initial_app_data_.tts_name_) {
    delete initial_app_data_.tts_name_;
  }

  initial_app_data_.tts_name_ =
    new smart_objects::CSmartObject(tts_name);
}

void ApplicationImpl::set_ngn_media_screen_name(
  const smart_objects::CSmartObject& ngn_name) {
  if (initial_app_data_.ngn_media_screen_name_) {
    delete initial_app_data_.ngn_media_screen_name_;
  }

  initial_app_data_.ngn_media_screen_name_ =
    new smart_objects::CSmartObject(ngn_name);
}

void ApplicationImpl::set_app_types(
  const smart_objects::CSmartObject& app_types) {
  if (initial_app_data_.app_types_) {
    delete initial_app_data_.app_types_;
  }

  initial_app_data_.app_types_ =
    new smart_objects::CSmartObject(app_types);
}

void ApplicationImpl::set_vr_synonyms(
  const smart_objects::CSmartObject& vr_synonyms) {
  if (initial_app_data_.vr_synonyms_) {
    delete initial_app_data_.vr_synonyms_;
  }
  initial_app_data_.vr_synonyms_ =
    new smart_objects::CSmartObject(vr_synonyms);
}

void ApplicationImpl::set_mobile_app_id(
  const smart_objects::CSmartObject& mobile_app_id) {
  if (initial_app_data_.mobile_app_id_) {
    delete initial_app_data_.mobile_app_id_;
  }
  initial_app_data_.mobile_app_id_ =
    new smart_objects::CSmartObject(mobile_app_id);
}

void ApplicationImpl::set_help_prompt(
  const smart_objects::CSmartObject& help_promt) {
  if (help_promt_) {
    delete help_promt_;
  }
  help_promt_ = new smart_objects::CSmartObject(help_promt);
}

void ApplicationImpl::set_timeout_prompt(
  const smart_objects::CSmartObject& timeout_promt) {
  if (timeout_promt_) {
    delete timeout_promt_;
  }
  timeout_promt_ = new smart_objects::CSmartObject(timeout_promt);
}

void ApplicationImpl::set_vr_help_title(
  const smart_objects::CSmartObject& vr_help_title) {
  if (vr_help_title_) {
    delete vr_help_title_;
  }
  vr_help_title_ = new smart_objects::CSmartObject(vr_help_title);
}

void ApplicationImpl::set_vr_help(
  const smart_objects::CSmartObject& vr_help) {
  if (vr_help_) {
    delete vr_help_;
  }
  vr_help_ = new smart_objects::CSmartObject(vr_help);
}

void ApplicationImpl::set_tbt_state(
  const mobile_api::TBTState::eType& tbt_state) {
  tbt_state_ = tbt_state;
}

const mobile_api::TBTState::eType& ApplicationImpl::tbt_state() const {
  return tbt_state_;
}

void ApplicationImpl::AddCommand(unsigned int cmd_id,
                                 const smart_objects::CSmartObject& command) {
  commands_[cmd_id] = new smart_objects::CSmartObject(command);
}

void ApplicationImpl::RemoveCommand(unsigned int cmd_id) {
  CommandsMap::iterator it = commands_.find(cmd_id);

  if (commands_.end() != it) {
    delete it->second;
    commands_.erase(cmd_id);
  }
}

smart_objects::CSmartObject*  ApplicationImpl::FindCommand(
  unsigned int cmd_id) {
  CommandsMap::const_iterator it = commands_.find(cmd_id);
  if (it != commands_.end()) {
    return it->second;
  }

  return NULL;
}

// TODO(VS): Create common functions for processing collections
void ApplicationImpl::AddSubMenu(unsigned int menu_id,
                                 const smart_objects::CSmartObject& menu) {
  sub_menu_[menu_id] = new smart_objects::CSmartObject(menu);
}

void ApplicationImpl::RemoveSubMenu(unsigned int menu_id) {
  SubMenuMap::iterator it = sub_menu_.find(menu_id);

  if (sub_menu_.end() != it) {
    delete it->second;
    sub_menu_.erase(menu_id);
  }
}

bool ApplicationImpl::IsSubMenuNameAlreadyExist(const std::string& name)
{
  for (SubMenuMap::iterator it = sub_menu_.begin();
        sub_menu_.end() != it; ++it) {
    smart_objects::CSmartObject* menu = it->second;
    if ((*menu)[strings::menu_name] == name) {
      return true;
    }
  }
  return false;
}

smart_objects::CSmartObject*  ApplicationImpl::FindSubMenu(unsigned int menu_id)
{
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (it != sub_menu_.end()) {
    return it->second;
  }

  return NULL;
}

void ApplicationImpl::AddChoiceSet(unsigned int choice_set_id,
                                     const smart_objects::CSmartObject& choice_set) {
  choice_set_map_[choice_set_id] = new smart_objects::CSmartObject(choice_set);
}

void ApplicationImpl::RemoveChoiceSet(unsigned int choice_set_id) {
  ChoiceSetMap::iterator it = choice_set_map_.find(choice_set_id);

  if (choice_set_map_.end() != it) {
    delete it->second;
    choice_set_map_.erase(choice_set_id);
  }
}

smart_objects::CSmartObject*  ApplicationImpl::FindChoiceSet(unsigned int choice_set_id) {
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (it != choice_set_map_.end()) {
    return it->second;
  }

  return NULL;
}
bool ApplicationImpl::IsFullscreen() const {
  return mobile_api::HMILevel::HMI_FULL == hmi_level_;
}

bool ApplicationImpl::IsAudible() const {
  return mobile_api::HMILevel::HMI_FULL == hmi_level_ ||
         mobile_api::HMILevel::HMI_LIMITED == hmi_level_;
}

bool ApplicationImpl::HasbeenActivated() const {
  return mobile_api::HMILevel::HMI_NONE != hmi_level_;
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

bool ApplicationImpl::IsSubscribedToIVI(unsigned int vehicle_info_type_) {
  std::set<unsigned int>::iterator it = subscribed_vehicle_info_.find(
                                          vehicle_info_type_);
  return (subscribed_vehicle_info_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromIVI(unsigned int vehicle_info_type_) {
  size_t old_size = subscribed_vehicle_info_.size();
  subscribed_vehicle_info_.erase(vehicle_info_type_);
  return (subscribed_vehicle_info_.size() == old_size - 1);
}

bool ApplicationImpl::AddFile(
  const std::string& file_name, bool is_persistent) {
  for (size_t i = 0; i < app_files_.size(); ++i) {
    if (0 == file_name.compare(app_files_[i].file_name)) {
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

}  // namespace application_manager
