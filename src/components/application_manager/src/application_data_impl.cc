/*
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

#include <algorithm>

#include "application_manager/application_data_impl.h"
#include "application_manager/smart_object_keys.h"
#include "utils/logger.h"

namespace application_manager {
CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

InitialApplicationDataImpl::InitialApplicationDataImpl()
    : app_types_(NULL),
      vr_synonyms_(NULL),
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

std::string InitialApplicationDataImpl::mobile_app_id() const {
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

  app_types_ = new smart_objects::SmartObject(app_types);
}

void InitialApplicationDataImpl::set_vr_synonyms(
    const smart_objects::SmartObject& vr_synonyms) {
  if (vr_synonyms_) {
    delete vr_synonyms_;
  }
  vr_synonyms_ = new smart_objects::SmartObject(vr_synonyms);
}

void InitialApplicationDataImpl::set_mobile_app_id(
    const std::string& mobile_app_id) {
  mobile_app_id_ = mobile_app_id;
}

void InitialApplicationDataImpl::set_tts_name(
    const smart_objects::SmartObject& tts_name) {
  if (tts_name_) {
    delete tts_name_;
  }

  tts_name_ = new smart_objects::SmartObject(tts_name);
}

void InitialApplicationDataImpl::set_ngn_media_screen_name(
    const smart_objects::SmartObject& ngn_name) {
  if (ngn_media_screen_name_) {
    delete ngn_media_screen_name_;
  }

  ngn_media_screen_name_ = new smart_objects::SmartObject(ngn_name);
}

void InitialApplicationDataImpl::set_language(
    const mobile_api::Language::eType& language) {
  language_ = language;
}

void InitialApplicationDataImpl::set_ui_language(
    const mobile_api::Language::eType& ui_language) {
  ui_language_ = ui_language;
}

void InitialApplicationDataImpl::set_perform_interaction_layout(mobile_apis::LayoutMode::eType layout) {
  perform_interaction_layout_ = layout;
}

mobile_apis::LayoutMode::eType InitialApplicationDataImpl::perform_interaction_layout() const {
  return perform_interaction_layout_;
}

DynamicApplicationDataImpl::DynamicApplicationDataImpl()
    : help_prompt_(NULL),
      timeout_prompt_(NULL),
      vr_help_title_(NULL),
      vr_help_(NULL),
      tbt_state_(mobile_api::TBTState::INVALID_ENUM),
      show_command_(NULL),
      keyboard_props_(NULL),
      menu_title_(NULL),
      menu_icon_(NULL),
      tbt_show_command_(NULL),
      commands_(),
      commands_lock_(true),
      sub_menu_(),
      choice_set_map_(),
      performinteraction_choice_set_map_(),
      performinteraction_choice_set_lock_(true),
      is_perform_interaction_active_(false),
      is_reset_global_properties_active_(false),
      perform_interaction_mode_(-1) {
}

DynamicApplicationDataImpl::~DynamicApplicationDataImpl() {
  if (help_prompt_) {
    delete help_prompt_;
    help_prompt_ = NULL;
  }

  if (timeout_prompt_) {
    delete timeout_prompt_;
    timeout_prompt_ = NULL;
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

  PerformChoiceSetMap::iterator it = performinteraction_choice_set_map_.begin();
  for (; performinteraction_choice_set_map_.end() != it; ++it) {
    PerformChoice::iterator choice_it = performinteraction_choice_set_map_[it->first].begin();
    for (; performinteraction_choice_set_map_[it->first].end() != choice_it; ++choice_it) {
      delete choice_it->second;      
    }
    performinteraction_choice_set_map_[it->first].clear();
  }
  performinteraction_choice_set_map_.clear();
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::help_prompt() const {
  return help_prompt_;
}

const smart_objects::SmartObject*
DynamicApplicationDataImpl::timeout_prompt() const {
  return timeout_prompt_;
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

const smart_objects::SmartObject* DynamicApplicationDataImpl::keyboard_props() const {
  return keyboard_props_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::menu_title() const {
  return menu_title_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::menu_icon() const {
  return menu_icon_;
}

void DynamicApplicationDataImpl::load_global_properties(
    const smart_objects::SmartObject& properties_so) {
  SetGlobalProperties(properties_so.getElement(strings::vr_help_title),
                       &DynamicApplicationData::set_vr_help_title);

  SetGlobalProperties(properties_so.getElement(strings::vr_help),
                       &DynamicApplicationData::set_vr_help);

  SetGlobalProperties(properties_so.getElement(strings::timeout_prompt),
                      &DynamicApplicationData::set_timeout_prompt);

  SetGlobalProperties(properties_so.getElement(strings::help_prompt),
                      &DynamicApplicationData::set_help_prompt);

  SetGlobalProperties(properties_so.getElement(strings::keyboard_properties),
                      &DynamicApplicationData::set_keyboard_props);

  SetGlobalProperties(properties_so.getElement(strings::menu_title),
                      &DynamicApplicationData::set_menu_title);

  SetGlobalProperties(properties_so.getElement(strings::menu_icon),
                      &DynamicApplicationData::set_menu_icon);
}

void DynamicApplicationDataImpl::set_help_prompt(
    const smart_objects::SmartObject& help_prompt) {
  if (help_prompt_) {
    delete help_prompt_;
  }
  help_prompt_ = new smart_objects::SmartObject(help_prompt);
}

void DynamicApplicationDataImpl::set_timeout_prompt(
    const smart_objects::SmartObject& timeout_prompt) {
  if (timeout_prompt_) {
    delete timeout_prompt_;
  }
  timeout_prompt_ = new smart_objects::SmartObject(timeout_prompt);
}

void DynamicApplicationDataImpl::set_vr_help_title(
    const smart_objects::SmartObject& vr_help_title) {
  if (vr_help_title_) {
    delete vr_help_title_;
  }
  vr_help_title_ = new smart_objects::SmartObject(vr_help_title);
}

void DynamicApplicationDataImpl::reset_vr_help_title() {
  if (vr_help_title_) {
    delete vr_help_title_;
    vr_help_title_ = NULL;
  }
}

void DynamicApplicationDataImpl::set_vr_help(
    const smart_objects::SmartObject& vr_help) {
  if (vr_help_) {
    delete vr_help_;
  }
  vr_help_ = new smart_objects::SmartObject(vr_help);
}

void DynamicApplicationDataImpl::reset_vr_help() {
  if (vr_help_) {
    delete vr_help_;
  }
  vr_help_ = NULL;
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

void DynamicApplicationDataImpl::set_keyboard_props(
    const smart_objects::SmartObject& keyboard_props) {
  if (keyboard_props_) {
    delete keyboard_props_;
  }
  keyboard_props_ = new smart_objects::SmartObject(keyboard_props);
}

void DynamicApplicationDataImpl::set_menu_title(
    const smart_objects::SmartObject& menu_title) {
  if (menu_title_) {
    delete menu_title_;
  }
  menu_title_ = new smart_objects::SmartObject(menu_title);
}

void DynamicApplicationDataImpl::set_menu_icon(
    const smart_objects::SmartObject& menu_icon) {
  if (menu_icon_) {
    delete menu_icon_;
  }
  menu_icon_= new smart_objects::SmartObject(menu_icon);
}

void DynamicApplicationDataImpl::SetGlobalProperties(
    const smart_objects::SmartObject& param,
    void (DynamicApplicationData::*callback)(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject&)) {

  smart_objects::SmartType so_type = param.getType();
  if (so_type != smart_objects::SmartType::SmartType_Invalid  &&
      so_type != smart_objects::SmartType::SmartType_Null) {
    if (callback) {
      (this->*callback)(param);
    }
  } else {
    LOG4CXX_WARN(logger_, "Invalid or Null smart object");
  }
}

void DynamicApplicationDataImpl::AddCommand(
  uint32_t cmd_id, const smart_objects::SmartObject& command) {
  sync_primitives::AutoLock lock(commands_lock_);
  CommandsMap::const_iterator it = commands_.find(cmd_id);
  if (commands_.end() == it) {
    commands_[cmd_id] = new smart_objects::SmartObject(command);
  }
}

void DynamicApplicationDataImpl::RemoveCommand(uint32_t cmd_id) {
  sync_primitives::AutoLock lock(commands_lock_);
  CommandsMap::iterator it = commands_.find(cmd_id);
  if (commands_.end() != it) {
    delete it->second;
    commands_.erase(it);
  }
}

smart_objects::SmartObject* DynamicApplicationDataImpl::FindCommand(
    uint32_t cmd_id) {
  sync_primitives::AutoLock lock(commands_lock_);
  CommandsMap::const_iterator it = commands_.find(cmd_id);
  if (it != commands_.end()) {
    return it->second;
  }

  return NULL;
}

// TODO(VS): Create common functions for processing collections
void DynamicApplicationDataImpl::AddSubMenu(
  uint32_t menu_id, const smart_objects::SmartObject& menu) {
  sync_primitives::AutoLock lock(sub_menu_lock_);
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (sub_menu_.end() == it) {
    sub_menu_[menu_id] = new smart_objects::SmartObject(menu);
  }
}

void DynamicApplicationDataImpl::RemoveSubMenu(uint32_t menu_id) {
  sync_primitives::AutoLock lock(sub_menu_lock_);
  SubMenuMap::iterator it = sub_menu_.find(menu_id);

  if (sub_menu_.end() != it) {
    delete it->second;
    sub_menu_.erase(menu_id);
  }
}

smart_objects::SmartObject* DynamicApplicationDataImpl::FindSubMenu(
    uint32_t menu_id) const {
  sync_primitives::AutoLock lock(sub_menu_lock_);
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (it != sub_menu_.end()) {
    return it->second;
  }

  return NULL;
}

bool DynamicApplicationDataImpl::IsSubMenuNameAlreadyExist(
    const std::string& name) {
  sync_primitives::AutoLock lock(sub_menu_lock_);
  for (SubMenuMap::iterator it = sub_menu_.begin();
       sub_menu_.end() != it;
       ++it) {
    smart_objects::SmartObject* menu = it->second;
    if ((*menu)[strings::menu_name] == name) {
      return true;
    }
  }
  return false;
}

void DynamicApplicationDataImpl::AddChoiceSet(
  uint32_t choice_set_id, const smart_objects::SmartObject& choice_set) {
  sync_primitives::AutoLock lock(choice_set_map_lock_);
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (choice_set_map_.end() == it) {
    choice_set_map_[choice_set_id] = new smart_objects::SmartObject(choice_set);
  }
}

void DynamicApplicationDataImpl::RemoveChoiceSet(uint32_t choice_set_id) {
  sync_primitives::AutoLock lock(choice_set_map_lock_);
  ChoiceSetMap::iterator it = choice_set_map_.find(choice_set_id);

  if (choice_set_map_.end() != it) {
    delete it->second;
    choice_set_map_.erase(choice_set_id);
  }
}

smart_objects::SmartObject* DynamicApplicationDataImpl::FindChoiceSet(
    uint32_t choice_set_id) {
  sync_primitives::AutoLock lock(choice_set_map_lock_);
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (it != choice_set_map_.end()) {
    return it->second;
  }

  return NULL;
}

void DynamicApplicationDataImpl::AddPerformInteractionChoiceSet(
  uint32_t correlation_id, uint32_t choice_set_id,
  const smart_objects::SmartObject& vr_commands) {
  sync_primitives::AutoLock lock(performinteraction_choice_set_lock_);
  performinteraction_choice_set_map_[correlation_id].insert(
      std::make_pair(choice_set_id, new smart_objects::SmartObject(vr_commands)));
}

void DynamicApplicationDataImpl::DeletePerformInteractionChoiceSet(
  uint32_t correlation_id) {
  sync_primitives::AutoLock lock(performinteraction_choice_set_lock_);
  PerformChoice::iterator it =
      performinteraction_choice_set_map_[correlation_id].begin();
  for (; performinteraction_choice_set_map_[correlation_id].end() != it; ++it) {
    delete it->second;
  }
  performinteraction_choice_set_map_[correlation_id].clear();
  performinteraction_choice_set_map_.erase(correlation_id);
}

void DynamicApplicationDataImpl::set_perform_interaction_active(
    uint32_t active) {
  is_perform_interaction_active_ = active;
}

void DynamicApplicationDataImpl::set_reset_global_properties_active(
    bool active) {
  is_reset_global_properties_active_ = active;
}

void DynamicApplicationDataImpl::set_perform_interaction_mode(int32_t mode) {
  perform_interaction_mode_ = mode;
}

}  // namespace application_manager
