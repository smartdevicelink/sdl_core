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

#include "application_manager/application_data_impl.h"
#include "application_manager/smart_object_keys.h"

namespace application_manager {

InitialApplicationDataImpl::InitialApplicationDataImpl()
  : app_types_(NULL),
    vr_synonyms_(NULL),
    mobile_app_id_(NULL),
    tts_name_(NULL),
    ngn_media_screen_name_(NULL),
    language_(mobile_api::Language::INVALID_ENUM),
    ui_language_(mobile_api::Language::INVALID_ENUM) {
}

InitialApplicationDataImpl::~InitialApplicationDataImpl() {
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

const smart_objects::SmartObject*
InitialApplicationDataImpl::app_types() const {
  return app_types_;
}

const smart_objects::SmartObject*
InitialApplicationDataImpl::vr_synonyms() const {
  return vr_synonyms_;
}

const smart_objects::SmartObject*
InitialApplicationDataImpl::mobile_app_id() const {
  return mobile_app_id_;
}

const smart_objects::SmartObject* InitialApplicationDataImpl::tts_name() const {
  return tts_name_;
}

const smart_objects::SmartObject*
InitialApplicationDataImpl::ngn_media_screen_name() const {
  return ngn_media_screen_name_;
}

const mobile_api::Language::eType&
InitialApplicationDataImpl::language() const {
  return language_;
}

const mobile_api::Language::eType&
InitialApplicationDataImpl::ui_language() const {
  return ui_language_;
}

void InitialApplicationDataImpl::set_app_types(
  const smart_objects::SmartObject& app_types) {
  if (app_types_) {
    delete app_types_;
  }

  app_types_ =
    new smart_objects::SmartObject(app_types);
}

void InitialApplicationDataImpl::set_vr_synonyms(
  const smart_objects::SmartObject& vr_synonyms) {
  if (vr_synonyms_) {
    delete vr_synonyms_;
  }
  vr_synonyms_ =
    new smart_objects::SmartObject(vr_synonyms);
}

void InitialApplicationDataImpl::set_mobile_app_id(
  const smart_objects::SmartObject& mobile_app_id) {
  if (mobile_app_id_) {
    delete mobile_app_id_;
  }
  mobile_app_id_ =
    new smart_objects::SmartObject(mobile_app_id);
}

void InitialApplicationDataImpl::set_tts_name(
  const smart_objects::SmartObject& tts_name) {
  if (tts_name_) {
    delete tts_name_;
  }

  tts_name_ =
    new smart_objects::SmartObject(tts_name);
}

void InitialApplicationDataImpl::set_ngn_media_screen_name(
  const smart_objects::SmartObject& ngn_name) {
  if (ngn_media_screen_name_) {
    delete ngn_media_screen_name_;
  }

  ngn_media_screen_name_ =
    new smart_objects::SmartObject(ngn_name);
}

void InitialApplicationDataImpl::set_language(
  const mobile_api::Language::eType& language) {
  language_ = language;
}

void InitialApplicationDataImpl::set_ui_language(
  const mobile_api::Language::eType& ui_language) {
  ui_language_ = ui_language;
}

DynamicApplicationDataImpl::DynamicApplicationDataImpl()
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

DynamicApplicationDataImpl::~DynamicApplicationDataImpl() {
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

const smart_objects::SmartObject*
DynamicApplicationDataImpl::help_promt() const {
  return help_promt_;
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::timeout_promt() const {
  return timeout_promt_;
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::vr_help_title() const {
  return vr_help_title_;
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::vr_help() const {
  return vr_help_;
}

const mobile_api::TBTState::eType&
DynamicApplicationDataImpl::tbt_state() const {
  return tbt_state_;
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::show_command() const {
  return show_command_;
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::tbt_show_command() const {
  return tbt_show_command_;
}

void DynamicApplicationDataImpl::set_help_prompt(
  const smart_objects::SmartObject& help_promt) {
  if (help_promt_) {
    delete help_promt_;
  }
  help_promt_ = new smart_objects::SmartObject(help_promt);
}

void DynamicApplicationDataImpl::set_timeout_prompt(
  const smart_objects::SmartObject& timeout_promt) {
  if (timeout_promt_) {
    delete timeout_promt_;
  }
  timeout_promt_ = new smart_objects::SmartObject(timeout_promt);
}

void DynamicApplicationDataImpl::set_vr_help_title(
  const smart_objects::SmartObject& vr_help_title) {
  if (vr_help_title_) {
    delete vr_help_title_;
  }
  vr_help_title_ = new smart_objects::SmartObject(vr_help_title);
}

void DynamicApplicationDataImpl::set_vr_help(
  const smart_objects::SmartObject& vr_help) {
  if (vr_help_) {
    delete vr_help_;
  }
  vr_help_ = new smart_objects::SmartObject(vr_help);
}

void DynamicApplicationDataImpl::set_tbt_state(
  const mobile_api::TBTState::eType& tbt_state) {
  tbt_state_ = tbt_state;
}

void DynamicApplicationDataImpl::set_show_command(
  const smart_objects::SmartObject& show_command) {
  if (show_command_) {
    delete show_command_;
  }
  show_command_ = new smart_objects::SmartObject(show_command);
}

void DynamicApplicationDataImpl::set_tbt_show_command(
  const smart_objects::SmartObject& tbt_show) {
  if (tbt_show_command_) {
    delete tbt_show_command_;
  }
  tbt_show_command_ = new smart_objects::SmartObject(tbt_show);
}

void
DynamicApplicationDataImpl::AddCommand(
  unsigned int cmd_id,
  const smart_objects::SmartObject& command) {
  commands_[cmd_id] = new smart_objects::SmartObject(command);
}

void DynamicApplicationDataImpl::RemoveCommand(unsigned int cmd_id) {
  CommandsMap::iterator it = commands_.find(cmd_id);

  if (commands_.end() != it) {
    delete it->second;
    commands_.erase(cmd_id);
  }
}

smart_objects::SmartObject*  DynamicApplicationDataImpl::FindCommand(
  unsigned int cmd_id) {
  CommandsMap::const_iterator it = commands_.find(cmd_id);
  if (it != commands_.end()) {
    return it->second;
  }

  return NULL;
}

// TODO(VS): Create common functions for processing collections
void
DynamicApplicationDataImpl::AddSubMenu(unsigned int menu_id,
                                       const smart_objects::SmartObject& menu) {
  sub_menu_[menu_id] = new smart_objects::SmartObject(menu);
}

void DynamicApplicationDataImpl::RemoveSubMenu(unsigned int menu_id) {
  SubMenuMap::iterator it = sub_menu_.find(menu_id);

  if (sub_menu_.end() != it) {
    delete it->second;
    sub_menu_.erase(menu_id);
  }
}

smart_objects::SmartObject*  DynamicApplicationDataImpl::FindSubMenu(
  unsigned int menu_id) {
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (it != sub_menu_.end()) {
    return it->second;
  }

  return NULL;
}

bool DynamicApplicationDataImpl::IsSubMenuNameAlreadyExist(
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

void DynamicApplicationDataImpl::AddChoiceSet(unsigned int choice_set_id,
    const smart_objects::SmartObject& choice_set) {
  choice_set_map_[choice_set_id] = new smart_objects::SmartObject(choice_set);
}

void DynamicApplicationDataImpl::RemoveChoiceSet(unsigned int choice_set_id) {
  ChoiceSetMap::iterator it = choice_set_map_.find(choice_set_id);

  if (choice_set_map_.end() != it) {
    delete it->second;
    choice_set_map_.erase(choice_set_id);
  }
}

smart_objects::SmartObject*  DynamicApplicationDataImpl::FindChoiceSet(
  unsigned int choice_set_id) {
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (it != choice_set_map_.end()) {
    return it->second;
  }

  return NULL;
}

void DynamicApplicationDataImpl::AddChoiceSetVRCommands(
  unsigned int choice_set_id,
  const smart_objects::SmartObject& vr_commands) {
  choice_set_vr_commands_map_[choice_set_id] =
    new smart_objects::SmartObject(vr_commands);
}

void DynamicApplicationDataImpl::DeleteChoiceSetVRCommands() {
  for (ChoiceSetVRCmdMap::iterator it = choice_set_vr_commands_map_.begin();
       choice_set_vr_commands_map_.end() != it;
       ++it) {
    delete it->second;
  }
  choice_set_vr_commands_map_.clear();
}

smart_objects::SmartObject*
DynamicApplicationDataImpl::FindChoiceSetVRCommands(
  unsigned int choice_set_id) const {
  ChoiceSetVRCmdMap::const_iterator it =
    choice_set_vr_commands_map_.find(choice_set_id);

  if (it != choice_set_vr_commands_map_.end()) {
    return it->second;
  }

  return NULL;
}

void DynamicApplicationDataImpl::set_perform_interaction_active(bool active) {
  is_perform_interaction_active_ = active;
}

}  //
