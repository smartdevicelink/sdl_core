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
    mobile_app_id_(NULL),
    tts_name_(NULL),
    ngn_media_screen_name_(NULL),
    language_(mobile_api::Language::INVALID_ENUM),
    ui_language_(mobile_api::Language::INVALID_ENUM){
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

  if (mobile_app_id_) {
    delete mobile_app_id_;
    mobile_app_id_ = NULL;
  }

  if (tts_name_) {
    delete tts_name_;
    tts_name_ = NULL;
  }

  if (ngn_media_screen_name_) {
    delete ngn_media_screen_name_;
    ngn_media_screen_name_ = NULL;
  }
}

const smart_objects::SmartObject* InitialApplicationData::app_types() const {
  return app_types_;
}

const smart_objects::SmartObject* InitialApplicationData::vr_synonyms() const {
  return vr_synonyms_;
}

const smart_objects::SmartObject*
InitialApplicationData::mobile_app_id() const {
  return mobile_app_id_;
}

const smart_objects::SmartObject* InitialApplicationData::tts_name() const {
  return tts_name_;
}

const smart_objects::SmartObject*
InitialApplicationData::ngn_media_screen_name() const {
  return ngn_media_screen_name_;
}

const mobile_api::Language::eType& InitialApplicationData::language() const {
  return language_;
}

const mobile_api::Language::eType& InitialApplicationData::ui_language() const {
  return ui_language_;
}

void InitialApplicationData::set_app_types(
  const smart_objects::SmartObject& app_types) {
  if (app_types_) {
    delete app_types_;
  }

  app_types_ =
    new smart_objects::SmartObject(app_types);
}

void InitialApplicationData::set_vr_synonyms(
  const smart_objects::SmartObject& vr_synonyms) {
  if (vr_synonyms_) {
    delete vr_synonyms_;
  }
  vr_synonyms_ =
    new smart_objects::SmartObject(vr_synonyms);
}

void InitialApplicationData::set_mobile_app_id(
  const smart_objects::SmartObject& mobile_app_id) {
  if (mobile_app_id_) {
    delete mobile_app_id_;
  }
  mobile_app_id_ =
    new smart_objects::SmartObject(mobile_app_id);
}

void InitialApplicationData::set_tts_name(
  const smart_objects::SmartObject& tts_name) {
  if (tts_name_) {
    delete tts_name_;
  }

  tts_name_ =
    new smart_objects::SmartObject(tts_name);
}

void InitialApplicationData::set_ngn_media_screen_name(
  const smart_objects::SmartObject& ngn_name) {
  if (ngn_media_screen_name_) {
    delete ngn_media_screen_name_;
  }

  ngn_media_screen_name_ =
    new smart_objects::SmartObject(ngn_name);
}

void InitialApplicationData::set_language(
  const mobile_api::Language::eType& language) {
  language_ = language;
}

void InitialApplicationData::set_ui_language(
  const mobile_api::Language::eType& ui_language) {
  ui_language_ = ui_language;
}

DynamicApplicationData::DynamicApplicationData()
  : help_promt_(NULL),
    timeout_promt_(NULL),
    vr_help_title_(NULL),
    vr_help_(NULL),
    tbt_state_(mobile_api::TBTState::INVALID_ENUM),
    show_command_(NULL),
    tbt_show_command_(NULL),
    commands_(),
    sub_menu_(),
    choice_set_map_(),
    choice_set_vr_commands_map_(),
    is_perform_interaction_active_(false) {
}

DynamicApplicationData::~DynamicApplicationData() {
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

  if (show_command_) {
    delete show_command_;
    show_command_ = NULL;
  }

  if (tbt_show_command_) {
    delete tbt_show_command_;
    tbt_show_command_ = NULL;
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

  for (ChoiceSetVRCmdMap::iterator it = choice_set_vr_commands_map_.begin();
      choice_set_vr_commands_map_.end() != it;
       ++it) {
    delete it->second;
  }
  choice_set_vr_commands_map_.clear();
}

const smart_objects::SmartObject* DynamicApplicationData::help_promt() const {
  return help_promt_;
}

const smart_objects::SmartObject*
DynamicApplicationData::timeout_promt() const {
  return timeout_promt_;
}

const smart_objects::SmartObject*
DynamicApplicationData::vr_help_title() const {
  return vr_help_title_;
}

const smart_objects::SmartObject*
DynamicApplicationData::vr_help() const {
  return vr_help_;
}

const mobile_api::TBTState::eType& DynamicApplicationData::tbt_state() const {
  return tbt_state_;
}

const smart_objects::SmartObject*
DynamicApplicationData::show_command() const {
  return show_command_;
}

const smart_objects::SmartObject*
DynamicApplicationData::tbt_show_command() const {
  return tbt_show_command_;
}

void DynamicApplicationData::set_help_prompt(
  const smart_objects::SmartObject& help_promt) {
  if (help_promt_) {
    delete help_promt_;
  }
  help_promt_ = new smart_objects::SmartObject(help_promt);
}

void DynamicApplicationData::set_timeout_prompt(
  const smart_objects::SmartObject& timeout_promt) {
  if (timeout_promt_) {
    delete timeout_promt_;
  }
  timeout_promt_ = new smart_objects::SmartObject(timeout_promt);
}

void DynamicApplicationData::set_vr_help_title(
  const smart_objects::SmartObject& vr_help_title) {
  if (vr_help_title_) {
    delete vr_help_title_;
  }
  vr_help_title_ = new smart_objects::SmartObject(vr_help_title);
}

void DynamicApplicationData::set_vr_help(
  const smart_objects::SmartObject& vr_help) {
  if (vr_help_) {
    delete vr_help_;
  }
  vr_help_ = new smart_objects::SmartObject(vr_help);
}

void DynamicApplicationData::set_tbt_state(
  const mobile_api::TBTState::eType& tbt_state) {
  tbt_state_ = tbt_state;
}

void DynamicApplicationData::set_show_command(
  const smart_objects::SmartObject& show_command) {
  if (show_command_) {
    delete show_command_;
  }
  show_command_ = new smart_objects::SmartObject(show_command);
}

void DynamicApplicationData::set_tbt_show_command(
  const smart_objects::SmartObject& tbt_show) {
  if (tbt_show_command_) {
    delete tbt_show_command_;
  }
  tbt_show_command_ = new smart_objects::SmartObject(tbt_show);
}

void
DynamicApplicationData::AddCommand(unsigned int cmd_id,
                                   const smart_objects::SmartObject& command) {
  commands_[cmd_id] = new smart_objects::SmartObject(command);
}

void DynamicApplicationData::RemoveCommand(unsigned int cmd_id) {
  CommandsMap::iterator it = commands_.find(cmd_id);

  if (commands_.end() != it) {
    delete it->second;
    commands_.erase(cmd_id);
  }
}

smart_objects::SmartObject*  DynamicApplicationData::FindCommand(
  unsigned int cmd_id) {
  CommandsMap::const_iterator it = commands_.find(cmd_id);
  if (it != commands_.end()) {
    return it->second;
  }

  return NULL;
}

// TODO(VS): Create common functions for processing collections
void
DynamicApplicationData::AddSubMenu(unsigned int menu_id,
                                   const smart_objects::SmartObject& menu) {
  sub_menu_[menu_id] = new smart_objects::SmartObject(menu);
}

void DynamicApplicationData::RemoveSubMenu(unsigned int menu_id) {
  SubMenuMap::iterator it = sub_menu_.find(menu_id);

  if (sub_menu_.end() != it) {
    delete it->second;
    sub_menu_.erase(menu_id);
  }
}

smart_objects::SmartObject*  DynamicApplicationData::FindSubMenu(
  unsigned int menu_id) {
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (it != sub_menu_.end()) {
    return it->second;
  }

  return NULL;
}

bool DynamicApplicationData::IsSubMenuNameAlreadyExist(
  const std::string& name) {
  for (SubMenuMap::iterator it = sub_menu_.begin();
       sub_menu_.end() != it; ++it) {
    smart_objects::SmartObject* menu = it->second;
    if ((*menu)[strings::menu_name] == name) {
      return true;
    }
  }
  return false;
}

void DynamicApplicationData::AddChoiceSet(unsigned int choice_set_id,
    const smart_objects::SmartObject& choice_set) {
  choice_set_map_[choice_set_id] = new smart_objects::SmartObject(choice_set);
}

void DynamicApplicationData::RemoveChoiceSet(unsigned int choice_set_id) {
  ChoiceSetMap::iterator it = choice_set_map_.find(choice_set_id);

  if (choice_set_map_.end() != it) {
    delete it->second;
    choice_set_map_.erase(choice_set_id);
  }
}

smart_objects::SmartObject*  DynamicApplicationData::FindChoiceSet(
  unsigned int choice_set_id) {
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (it != choice_set_map_.end()) {
    return it->second;
  }

  return NULL;
}

void DynamicApplicationData::AddChoiceSetVRCommands(unsigned int choice_set_id,
                  const smart_objects::SmartObject& vr_commands) {
  choice_set_vr_commands_map_[choice_set_id] =
      new smart_objects::SmartObject(vr_commands);
}

void DynamicApplicationData::DeleteChoiceSetVRCommands() {
  for (ChoiceSetVRCmdMap::iterator it = choice_set_vr_commands_map_.begin();
      choice_set_vr_commands_map_.end() != it;
       ++it) {
    delete it->second;
  }
  choice_set_vr_commands_map_.clear();
}

smart_objects::SmartObject*  DynamicApplicationData::FindChoiceSetVRCommands(
    unsigned int choice_set_id) const {
  ChoiceSetVRCmdMap::const_iterator it =
      choice_set_vr_commands_map_.find(choice_set_id);

  if (it != choice_set_vr_commands_map_.end()) {
    return it->second;
  }

  return NULL;
}

void DynamicApplicationData::set_perform_interaction_active(bool active) {
  is_perform_interaction_active_ = active;
}

ApplicationImpl::ApplicationImpl(int app_id)
  : app_id_(app_id),
    active_message_(NULL),
    is_media_(false),
    allowed_support_navigation_(false),
    hmi_level_(mobile_api::HMILevel::INVALID_ENUM),
    system_context_(mobile_api::SystemContext::INVALID_ENUM),
    audio_streaming_state_(mobile_api::AudioStreamingState::INVALID_ENUM),
    is_app_allowed_(true) {
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

void ApplicationImpl::ProcessMessage(smart_objects::SmartObject* message) {
  // TODO(PV): add code here when factory is created.
  // factory::create command.
  // if (command.Run()) {
  if (NULL == active_message_) {
    active_message_ = message;
  }
}

void ApplicationImpl::ReportError(smart_objects::SmartObject* message,
                                  ErrorCode error_code) {
  // factory->create response(message, error_code);
  // response.Run();
}

void ApplicationImpl::CloseActiveMessage() {
  delete active_message_;
  active_message_ = NULL;
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

int ApplicationImpl::app_id() const {
  return app_id_;
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
