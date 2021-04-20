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
SDL_CREATE_LOG_VARIABLE("ApplicationManager")

namespace {
struct CommandIdComparator {
  CommandIdComparator(const std::string& key, const uint32_t id)
      : key_(key), target_id_(id) {}

  bool operator()(const CommandsMap::value_type& new_command) const {
    smart_objects::SmartObject& command = *(new_command.second);
    if (command.keyExists(key_)) {
      return command[key_].asUInt() == target_id_;
    }
    return false;
  }

 private:
  std::string key_;
  uint32_t target_id_;
};
}  // namespace

InitialApplicationDataImpl::InitialApplicationDataImpl()
    : app_types_(NULL)
    , vr_synonyms_(NULL)
    , tts_name_(NULL)
    , ngn_media_screen_name_(NULL)

    , language_(mobile_api::Language::INVALID_ENUM)
    , ui_language_(mobile_api::Language::INVALID_ENUM) {}

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

const smart_objects::SmartObject* InitialApplicationDataImpl::app_types()
    const {
  return app_types_;
}

const smart_objects::SmartObject* InitialApplicationDataImpl::vr_synonyms()
    const {
  return vr_synonyms_;
}

std::string InitialApplicationDataImpl::policy_app_id() const {
  return mobile_app_id_;
}

const smart_objects::SmartObject* InitialApplicationDataImpl::tts_name() const {
  return tts_name_;
}

const smart_objects::SmartObject*
InitialApplicationDataImpl::ngn_media_screen_name() const {
  return ngn_media_screen_name_;
}

const mobile_api::Language::eType& InitialApplicationDataImpl::language()
    const {
  return language_;
}

const mobile_api::Language::eType& InitialApplicationDataImpl::ui_language()
    const {
  return ui_language_;
}

const utils::SemanticVersion& InitialApplicationDataImpl::msg_version() const {
  return msg_version_;
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

void InitialApplicationDataImpl::set_msg_version(
    const utils::SemanticVersion& version) {
  msg_version_ = version;
}

void InitialApplicationDataImpl::set_perform_interaction_layout(
    mobile_apis::LayoutMode::eType layout) {
  perform_interaction_layout_ = layout;
}

mobile_apis::LayoutMode::eType
InitialApplicationDataImpl::perform_interaction_layout() const {
  return perform_interaction_layout_;
}

DynamicApplicationDataImpl::DynamicApplicationDataImpl()
    : help_prompt_(nullptr)
    , timeout_prompt_(nullptr)
    , vr_help_title_(nullptr)
    , vr_help_(nullptr)
    , tbt_state_(mobile_api::TBTState::INVALID_ENUM)
    , show_command_(nullptr)
    , keyboard_props_(nullptr)
    , menu_title_(nullptr)
    , menu_icon_(nullptr)
    , tbt_show_command_(nullptr)
    , commands_()
    , commands_lock_ptr_(std::make_shared<sync_primitives::RecursiveLock>())
    , sub_menu_()
    , sub_menu_lock_ptr_(std::make_shared<sync_primitives::RecursiveLock>())
    , choice_set_map_()
    , choice_set_map_lock_ptr_(
          std::make_shared<sync_primitives::RecursiveLock>())
    , performinteraction_choice_set_map_()
    , performinteraction_choice_set_lock_ptr_(
          std::make_shared<sync_primitives::RecursiveLock>())
    , window_params_map_()
    , window_params_map_lock_ptr_(
          std::make_shared<sync_primitives::RecursiveLock>())
    , is_perform_interaction_active_(false)
    , is_reset_global_properties_active_(false)
    , perform_interaction_mode_(-1)
    , display_capabilities_builder_(*this) {}

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

  if (keyboard_props_) {
    delete keyboard_props_;
    keyboard_props_ = NULL;
  }

  if (menu_title_) {
    delete menu_title_;
    menu_title_ = NULL;
  }

  if (menu_icon_) {
    delete menu_icon_;
    menu_icon_ = NULL;
  }

  if (tbt_show_command_) {
    delete tbt_show_command_;
    tbt_show_command_ = NULL;
  }

  for (CommandsMap::iterator command_it = commands_.begin();
       commands_.end() != command_it;
       ++command_it) {
    delete command_it->second;
  }
  commands_.clear();

  for (SubMenuMap::iterator sub_menu_it = sub_menu_.begin();
       sub_menu_.end() != sub_menu_it;
       ++sub_menu_it) {
    delete sub_menu_it->second;
  }
  sub_menu_.clear();

  for (auto command : choice_set_map_) {
    delete command.second;
  }
  choice_set_map_.clear();

  PerformChoiceSetMap::iterator it = performinteraction_choice_set_map_.begin();
  for (; performinteraction_choice_set_map_.end() != it; ++it) {
    PerformChoice::iterator choice_it =
        performinteraction_choice_set_map_[it->first].begin();
    for (; performinteraction_choice_set_map_[it->first].end() != choice_it;
         ++choice_it) {
      delete choice_it->second;
    }
    performinteraction_choice_set_map_[it->first].clear();
  }
  performinteraction_choice_set_map_.clear();

  window_params_map_.clear();
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::help_prompt()
    const {
  return help_prompt_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::timeout_prompt()
    const {
  return timeout_prompt_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::vr_help_title()
    const {
  return vr_help_title_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::vr_help() const {
  return vr_help_;
}

const mobile_api::TBTState::eType& DynamicApplicationDataImpl::tbt_state()
    const {
  return tbt_state_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::show_command()
    const {
  return show_command_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::tbt_show_command()
    const {
  return tbt_show_command_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::keyboard_props()
    const {
  return keyboard_props_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::menu_title()
    const {
  return menu_title_;
}

const smart_objects::SmartObject* DynamicApplicationDataImpl::menu_icon()
    const {
  return menu_icon_;
}

smart_objects::SmartObject DynamicApplicationDataImpl::day_color_scheme()
    const {
  using namespace mobile_apis::PredefinedWindows;
  auto default_window_it = window_templates_.find(DEFAULT_WINDOW);

  if (window_templates_.end() != default_window_it) {
    const smart_objects::SmartObject template_config =
        default_window_it->second;
    if (template_config.keyExists(strings::day_color_scheme)) {
      return template_config[strings::day_color_scheme];
    }
  }

  return smart_objects::SmartObject(smart_objects::SmartType::SmartType_Null);
}

smart_objects::SmartObject DynamicApplicationDataImpl::night_color_scheme()
    const {
  using namespace mobile_apis::PredefinedWindows;
  auto default_window_it = window_templates_.find(DEFAULT_WINDOW);

  if (window_templates_.end() != default_window_it) {
    const smart_objects::SmartObject template_config =
        default_window_it->second;
    if (template_config.keyExists(strings::night_color_scheme)) {
      return template_config[strings::night_color_scheme];
    }
  }

  return smart_objects::SmartObject(smart_objects::SmartType::SmartType_Null);
}

std::string DynamicApplicationDataImpl::display_layout() const {
  using namespace mobile_apis::PredefinedWindows;
  auto default_window_it = window_templates_.find(DEFAULT_WINDOW);

  if (window_templates_.end() != default_window_it) {
    smart_objects::SmartObject template_config = default_window_it->second;
    if (template_config.keyExists(strings::template_layout)) {
      return template_config[strings::template_layout].asString();
    }
  }

  return std::string();
}

smart_objects::SmartObjectSPtr
DynamicApplicationDataImpl::display_capabilities() const {
  return display_capabilities_;
}

smart_objects::SmartObjectSPtr DynamicApplicationDataImpl::display_capabilities(
    const WindowID window_id) const {
  SDL_LOG_AUTO_TRACE();

  if (!display_capabilities_) {
    SDL_LOG_WARN("Current window capabilities are empty");
    // SDL still needs to retreive display capabilities
    return display_capabilities_;
  }

  smart_objects::SmartObject result_window_caps(
      smart_objects::SmartType::SmartType_Map);

  const auto window_caps =
      (*display_capabilities_)[0][strings::window_capabilities].asArray();
  if (window_caps) {
    auto find_res = std::find_if(
        window_caps->begin(),
        window_caps->end(),
        [&window_id](const smart_objects::SmartObject& element) {
          return (window_id == element[strings::window_id].asInt());
        });

    if (find_res != window_caps->end()) {
      result_window_caps = *find_res;
    }
  }

  auto result_display_caps = std::make_shared<smart_objects::SmartObject>(
      smart_objects::SmartType_Array);

  const auto disp_caps_keys = (*display_capabilities_)[0].enumerate();
  for (const auto& key : disp_caps_keys) {
    if (strings::window_capabilities == key) {
      continue;
    }
    (*result_display_caps)[0][key] = (*display_capabilities_)[0][key];
  }

  (*result_display_caps)[0][strings::window_capabilities][0] =
      result_window_caps;

  return result_display_caps;
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
  menu_icon_ = new smart_objects::SmartObject(menu_icon);
}

void DynamicApplicationDataImpl::set_day_color_scheme(
    const smart_objects::SmartObject& color_scheme) {
  using namespace mobile_apis::PredefinedWindows;
  DCHECK(color_scheme.getType() == smart_objects::SmartType_Map);
  window_templates_[DEFAULT_WINDOW][strings::day_color_scheme] = color_scheme;
}

void DynamicApplicationDataImpl::set_night_color_scheme(
    const smart_objects::SmartObject& color_scheme) {
  using namespace mobile_apis::PredefinedWindows;
  DCHECK(color_scheme.getType() == smart_objects::SmartType_Map);
  window_templates_[DEFAULT_WINDOW][strings::night_color_scheme] = color_scheme;
}

void DynamicApplicationDataImpl::set_display_layout(const std::string& layout) {
  SDL_LOG_AUTO_TRACE();
  using namespace mobile_apis::PredefinedWindows;
  smart_objects::SmartObject template_config(smart_objects::SmartType_Map);
  template_config[strings::template_layout] = layout;
  window_templates_[DEFAULT_WINDOW] = template_config;
}

void DynamicApplicationDataImpl::set_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  SDL_LOG_AUTO_TRACE();
  const auto& incoming_window_capabilities =
      display_capabilities[0][strings::window_capabilities];

  smart_objects::SmartObject tmp_window_capabilities;
  if (display_capabilities_) {
    tmp_window_capabilities =
        (*display_capabilities_)[0][strings::window_capabilities];
  }

  display_capabilities_.reset(
      new smart_objects::SmartObject(display_capabilities));

  auto get_window_index = [&tmp_window_capabilities](const WindowID window_id) {
    const auto tmp_window_capabilities_arr = tmp_window_capabilities.asArray();
    if (!tmp_window_capabilities_arr) {
      return -1;
    }

    int index = 0;
    for (auto element : *tmp_window_capabilities_arr) {
      if (element.keyExists(strings::window_id)) {
        if (window_id == element[strings::window_id].asInt())
          return index;
      } else if (window_id == 0) {
        return index;
      }
      ++index;
    }
    return -1;
  };

  for (uint32_t i = 0; i < incoming_window_capabilities.length(); ++i) {
    int64_t window_id = 0;
    if (incoming_window_capabilities[i].keyExists(strings::window_id)) {
      window_id = incoming_window_capabilities[i][strings::window_id].asInt();
    }
    int found_index = get_window_index(window_id);
    if (0 <= found_index) {
      // Update the existing window capability
      tmp_window_capabilities[found_index] = incoming_window_capabilities[i];
    } else {
      tmp_window_capabilities[tmp_window_capabilities.length()] =
          incoming_window_capabilities[i];
    }
  }

  (*display_capabilities_)[0][strings::window_capabilities] =
      tmp_window_capabilities;
}

void DynamicApplicationDataImpl::remove_window_capability(
    const WindowID window_id) {
  SDL_LOG_AUTO_TRACE();

  if (!display_capabilities_) {
    SDL_LOG_ERROR("Application display capabilities are not available");
    return;
  }

  auto window_capabilities =
      (*display_capabilities_)[0][strings::window_capabilities].asArray();
  DCHECK_OR_RETURN_VOID(window_capabilities);

  for (auto it = window_capabilities->begin(); it != window_capabilities->end();
       ++it) {
    const auto cur_window_id = (*it).keyExists(strings::window_id)
                                   ? (*it)[strings::window_id].asInt()
                                   : 0;
    if (window_id == cur_window_id) {
      window_capabilities->erase(it);
      return;
    }
  }

  SDL_LOG_WARN("No window id " << window_id
                               << " found in display capabilities");
}

bool DynamicApplicationDataImpl::menu_layout_supported(
    const mobile_apis::MenuLayout::eType layout) const {
  if (!display_capabilities_)
    return false;

  const auto tmp_window_capabilities_arr =
      (*display_capabilities_)[0][strings::window_capabilities].asArray();

  if (!tmp_window_capabilities_arr)
    return false;

  for (auto element : *tmp_window_capabilities_arr) {
    if ((!element.keyExists(strings::window_id) ||
         element[strings::window_id].asInt() == 0) &&
        element.keyExists(strings::menu_layouts_available)) {
      for (uint32_t i = 0;
           i < element[strings::menu_layouts_available].length();
           ++i) {
        if (static_cast<mobile_apis::MenuLayout::eType>(
                element[strings::menu_layouts_available][i].asUInt()) ==
            layout) {
          return true;
        }
      }
    }
  }

  return false;
}

void DynamicApplicationDataImpl::set_window_layout(const WindowID window_id,
                                                   const std::string& layout) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObject template_config(smart_objects::SmartType_Map);

  template_config[strings::template_layout] = layout;
  window_templates_[window_id] = template_config;
}

void DynamicApplicationDataImpl::set_day_color_scheme(
    const WindowID window_id, const smart_objects::SmartObject& color_scheme) {
  SDL_LOG_AUTO_TRACE();
  DCHECK(color_scheme.getType() == smart_objects::SmartType_Map);
  window_templates_[window_id][strings::day_color_scheme] = color_scheme;
}

void DynamicApplicationDataImpl::set_night_color_scheme(
    const WindowID window_id, const smart_objects::SmartObject& color_scheme) {
  SDL_LOG_AUTO_TRACE();
  DCHECK(color_scheme.getType() == smart_objects::SmartType_Map);
  window_templates_[window_id][strings::night_color_scheme] = color_scheme;
}

std::string DynamicApplicationDataImpl::window_layout(
    const WindowID window_id) const {
  SDL_LOG_AUTO_TRACE();
  AppWindowsTemplates::const_iterator window_template_it =
      window_templates_.find(window_id);

  if (window_templates_.end() != window_template_it) {
    const smart_objects::SmartObject template_config =
        window_template_it->second;
    if (template_config.keyExists(strings::template_layout)) {
      return template_config[strings::template_layout].asString();
    }
  }

  return std::string();
}

smart_objects::SmartObject DynamicApplicationDataImpl::day_color_scheme(
    const WindowID window_id) const {
  SDL_LOG_AUTO_TRACE();
  AppWindowsTemplates::const_iterator window_template_it =
      window_templates_.find(window_id);

  if (window_templates_.end() != window_template_it) {
    const smart_objects::SmartObject template_config =
        window_template_it->second;
    if (template_config.keyExists(strings::day_color_scheme)) {
      return template_config[strings::day_color_scheme];
    }
  }

  return smart_objects::SmartObject(smart_objects::SmartType::SmartType_Null);
}

smart_objects::SmartObject DynamicApplicationDataImpl::night_color_scheme(
    const WindowID window_id) const {
  SDL_LOG_AUTO_TRACE();
  AppWindowsTemplates::const_iterator window_template_it =
      window_templates_.find(window_id);

  if (window_templates_.end() != window_template_it) {
    const smart_objects::SmartObject template_config =
        window_template_it->second;
    if (template_config.keyExists(strings::night_color_scheme)) {
      return template_config[strings::night_color_scheme];
    }
  }

  return smart_objects::SmartObject(smart_objects::SmartType::SmartType_Null);
}

void DynamicApplicationDataImpl::SetGlobalProperties(
    const smart_objects::SmartObject& param,
    void (DynamicApplicationData::*callback)(
        const ns_smart_device_link::ns_smart_objects::SmartObject&)) {
  smart_objects::SmartType so_type = param.getType();
  if (so_type != smart_objects::SmartType::SmartType_Invalid &&
      so_type != smart_objects::SmartType::SmartType_Null) {
    if (callback) {
      (this->*callback)(param);
    }
  } else {
    SDL_LOG_WARN("Invalid or Null smart object");
  }
}

void DynamicApplicationDataImpl::AddCommand(
    const uint32_t internal_id, const smart_objects::SmartObject& command) {
  sync_primitives::AutoLock lock(commands_lock_ptr_);

  CommandsMap::const_iterator it = commands_.find(internal_id);
  if (commands_.end() == it) {
    commands_[internal_id] = new smart_objects::SmartObject(command);
    SDL_LOG_DEBUG("Command with internal number "
                  << internal_id << " and id "
                  << (*commands_[internal_id])[strings::cmd_id].asUInt()
                  << " is added.");
  }
}

void DynamicApplicationDataImpl::RemoveCommand(const uint32_t cmd_id) {
  sync_primitives::AutoLock lock(commands_lock_ptr_);

  CommandIdComparator is_id_equal(strings::cmd_id, cmd_id);
  CommandsMap::iterator it =
      find_if(commands_.begin(), commands_.end(), is_id_equal);

  if (it != commands_.end()) {
    delete it->second;
    SDL_LOG_DEBUG("Command with internal number " << (it->first) << " and id "
                                                  << cmd_id << " is removed.");
    commands_.erase(it);

    return;
  }
  SDL_LOG_WARN("Command with id " << cmd_id
                                  << " is not found. Removal skipped.");
}

smart_objects::SmartObject DynamicApplicationDataImpl::FindCommand(
    const uint32_t cmd_id) {
  sync_primitives::AutoLock lock(commands_lock_ptr_);

  CommandIdComparator is_id_equal(strings::cmd_id, cmd_id);
  CommandsMap::const_iterator it =
      find_if(commands_.begin(), commands_.end(), is_id_equal);

  if (it != commands_.end()) {
    SDL_LOG_DEBUG("Command with internal number " << (it->first) << " and id "
                                                  << cmd_id << " is found.");
    smart_objects::SmartObject command(*it->second);
    return command;
  }

  return smart_objects::SmartObject(smart_objects::SmartType_Null);
}

// TODO(VS): Create common functions for processing collections
void DynamicApplicationDataImpl::AddSubMenu(
    uint32_t menu_id, const smart_objects::SmartObject& menu) {
  sync_primitives::AutoLock lock(sub_menu_lock_ptr_);
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (sub_menu_.end() == it) {
    sub_menu_[menu_id] = new smart_objects::SmartObject(menu);
  }
}

void DynamicApplicationDataImpl::RemoveSubMenu(uint32_t menu_id) {
  sync_primitives::AutoLock lock(sub_menu_lock_ptr_);
  SubMenuMap::iterator it = sub_menu_.find(menu_id);

  if (sub_menu_.end() != it) {
    delete it->second;
    sub_menu_.erase(menu_id);
  }
}

smart_objects::SmartObject DynamicApplicationDataImpl::FindSubMenu(
    uint32_t menu_id) const {
  sync_primitives::AutoLock lock(sub_menu_lock_ptr_);
  SubMenuMap::const_iterator it = sub_menu_.find(menu_id);
  if (it != sub_menu_.end()) {
    smart_objects::SmartObject sub_menu(*it->second);
    return sub_menu;
  }

  return smart_objects::SmartObject(smart_objects::SmartType_Null);
}

bool DynamicApplicationDataImpl::IsSubMenuNameAlreadyExist(
    const std::string& name, const uint32_t parent_id) {
  sync_primitives::AutoLock lock(sub_menu_lock_ptr_);
  for (SubMenuMap::iterator it = sub_menu_.begin(); sub_menu_.end() != it;
       ++it) {
    smart_objects::SmartObject* menu = it->second;
    if ((*menu)[strings::menu_name].asString() == name &&
        (*menu)[strings::parent_id].asInt() == parent_id) {
      return true;
    }
  }
  return false;
}

DataAccessor<WindowParamsMap>
DynamicApplicationDataImpl::window_optional_params_map() const {
  return DataAccessor<WindowParamsMap>(window_params_map_,
                                       window_params_map_lock_ptr_);
}

void DynamicApplicationDataImpl::SetWindowInfo(
    const WindowID window_id, const smart_objects::SmartObject& window_info) {
  SDL_LOG_AUTO_TRACE();
  const auto it = window_params_map_.find(window_id);
  if (window_params_map_.end() == it) {
    window_params_map_[window_id] =
        std::make_shared<smart_objects::SmartObject>(window_info);
  }
}

DisplayCapabilitiesBuilder&
DynamicApplicationDataImpl::display_capabilities_builder() {
  SDL_LOG_AUTO_TRACE();
  return display_capabilities_builder_;
}

void DynamicApplicationDataImpl::RemoveWindowInfo(const WindowID window_id) {
  SDL_LOG_AUTO_TRACE();
  window_params_map_.erase(window_id);
}

void DynamicApplicationDataImpl::AddChoiceSet(
    uint32_t choice_set_id, const smart_objects::SmartObject& choice_set) {
  sync_primitives::AutoLock lock(choice_set_map_lock_ptr_);
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (choice_set_map_.end() == it) {
    choice_set_map_[choice_set_id] = new smart_objects::SmartObject(choice_set);
  }
}

void DynamicApplicationDataImpl::RemoveChoiceSet(uint32_t choice_set_id) {
  sync_primitives::AutoLock lock(choice_set_map_lock_ptr_);
  ChoiceSetMap::iterator it = choice_set_map_.find(choice_set_id);

  if (choice_set_map_.end() != it) {
    delete it->second;
    choice_set_map_.erase(choice_set_id);
  }
}

smart_objects::SmartObject DynamicApplicationDataImpl::FindChoiceSet(
    uint32_t choice_set_id) {
  sync_primitives::AutoLock lock(choice_set_map_lock_ptr_);
  ChoiceSetMap::const_iterator it = choice_set_map_.find(choice_set_id);
  if (it != choice_set_map_.end()) {
    smart_objects::SmartObject choice_set(*it->second);
    return choice_set;
  }

  return smart_objects::SmartObject(smart_objects::SmartType_Null);
}

void DynamicApplicationDataImpl::AddPerformInteractionChoiceSet(
    uint32_t correlation_id,
    uint32_t choice_set_id,
    const smart_objects::SmartObject& vr_commands) {
  sync_primitives::AutoLock lock(performinteraction_choice_set_lock_ptr_);
  performinteraction_choice_set_map_[correlation_id].insert(std::make_pair(
      choice_set_id, new smart_objects::SmartObject(vr_commands)));
}

void DynamicApplicationDataImpl::DeletePerformInteractionChoiceSet(
    uint32_t correlation_id) {
  sync_primitives::AutoLock lock(performinteraction_choice_set_lock_ptr_);
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
