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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_DATA_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_DATA_IMPL_H_

#include <string>
#include "utils/lock.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace mobile_api = mobile_apis;

class InitialApplicationDataImpl : public virtual Application {
  public:
    InitialApplicationDataImpl();
    ~InitialApplicationDataImpl();

    const smart_objects::SmartObject* app_types() const;
    const smart_objects::SmartObject* vr_synonyms() const;
    virtual std::string mobile_app_id() const;
    const smart_objects::SmartObject* tts_name() const;
    const smart_objects::SmartObject* ngn_media_screen_name() const;
    const mobile_api::Language::eType& language() const;
    const mobile_api::Language::eType& ui_language() const;
    void set_app_types(const smart_objects::SmartObject& app_types);
    void set_vr_synonyms(const smart_objects::SmartObject& vr_synonyms);
    void set_mobile_app_id(const std::string& mobile_app_id);
    void set_tts_name(const smart_objects::SmartObject& tts_name);
    void set_ngn_media_screen_name(const smart_objects::SmartObject& ngn_name);
    void set_language(const mobile_api::Language::eType& language);
    void set_ui_language(const mobile_api::Language::eType& ui_language);

    void set_perform_interaction_layout(
        mobile_api::LayoutMode::eType layout) OVERRIDE;
    mobile_api::LayoutMode::eType perform_interaction_layout() const OVERRIDE;


  protected:
    smart_objects::SmartObject* app_types_;
    smart_objects::SmartObject* vr_synonyms_;
    std::string mobile_app_id_;
    smart_objects::SmartObject* tts_name_;
    smart_objects::SmartObject* ngn_media_screen_name_;
    mobile_api::Language::eType language_;
    mobile_api::Language::eType ui_language_;
    mobile_apis::LayoutMode::eType perform_interaction_layout_;
  private:
    DISALLOW_COPY_AND_ASSIGN(InitialApplicationDataImpl);
};

class DynamicApplicationDataImpl : public virtual Application {
  public:
    DynamicApplicationDataImpl();
    ~DynamicApplicationDataImpl();
    const smart_objects::SmartObject* help_prompt() const;
    const smart_objects::SmartObject* timeout_prompt() const;
    const smart_objects::SmartObject* vr_help_title() const;
    const smart_objects::SmartObject* vr_help() const;
    const mobile_api::TBTState::eType& tbt_state() const;
    const smart_objects::SmartObject* show_command() const;
    const smart_objects::SmartObject* tbt_show_command() const;
    const smart_objects::SmartObject* keyboard_props() const;
    const smart_objects::SmartObject* menu_title() const;
    const smart_objects::SmartObject* menu_icon() const;

    void load_global_properties(const smart_objects::SmartObject& properties_so);
    void set_help_prompt(const smart_objects::SmartObject& help_prompt);
    void set_timeout_prompt(const smart_objects::SmartObject& timeout_prompt);
    void set_vr_help_title(const smart_objects::SmartObject& vr_help_title);
    void reset_vr_help_title();
    void set_vr_help(const smart_objects::SmartObject& vr_help);
    void reset_vr_help();
    void set_tbt_state(const mobile_api::TBTState::eType& tbt_state);
    void set_show_command(const smart_objects::SmartObject& show_command);
    void set_tbt_show_command(const smart_objects::SmartObject& tbt_show);
    void set_keyboard_props(const smart_objects::SmartObject& keyboard_props);
    void set_menu_title(const smart_objects::SmartObject& menu_title);
    void set_menu_icon(const smart_objects::SmartObject& menu_icon);
    /*
     * @brief Adds a command to the in application menu
     */
    void AddCommand(uint32_t cmd_id,
                    const smart_objects::SmartObject& command);

    /*
     * @brief Deletes all commands from the application menu with the specified command id
     */
    void RemoveCommand(uint32_t cmd_id);

    /*
     * @brief Finds command with the specified command id
     */
    smart_objects::SmartObject* FindCommand(uint32_t cmd_id);

    /*
     * @brief Adds a menu to the application
     */
    void AddSubMenu(uint32_t menu_id, const smart_objects::SmartObject& menu);

    /*
     * @brief Deletes menu from the application menu
     */
    void RemoveSubMenu(uint32_t menu_id);

    /*
     * @brief Finds menu with the specified id
     */
    smart_objects::SmartObject* FindSubMenu(uint32_t menu_id) const;

    /*
     * @brief Returns true if sub menu with such name already exist
     */
    bool IsSubMenuNameAlreadyExist(const std::string& name);

    /*
     * @brief Adds a interaction choice set to the application
     *
     * @param choice_set_id Unique ID used for this interaction choice set
     * @param choice_set SmartObject that represent choice set
     */
    void AddChoiceSet(uint32_t choice_set_id,
                      const smart_objects::SmartObject& choice_set);

    /*
     * @brief Deletes choice set from the application
     *
     * @param choice_set_id Unique ID of the interaction choice set
     */
    void RemoveChoiceSet(uint32_t choice_set_id);

    /*
     * @brief Finds choice set with the specified choice_set_id id
     *
     * @param choice_set_id Unique ID of the interaction choice set
     */
    smart_objects::SmartObject* FindChoiceSet(uint32_t choice_set_id);

    /*
     * @brief Adds perform interaction choice set to the application
     *
     * @param correlation_id Unique ID of the request that added this choice set
     * @param choice_set_id Unique ID used for this interaction choice set
     * @param choice_set SmartObject that represents choice set
     */
    void AddPerformInteractionChoiceSet(uint32_t correlation_id,
      uint32_t choice_set_id, const smart_objects::SmartObject& choice_set);

    /*
     * @brief Deletes entirely perform interaction choice set map
     * @param correlation_id Unique ID of the request that added this choice set
     *
     */
    void DeletePerformInteractionChoiceSet(uint32_t correlation_id);

    /*
     * @brief Retrieves entirely ChoiceSet - VR commands map
     *
     * @return ChoiceSet map that is currently in use
     */
    inline DataAccessor<PerformChoiceSetMap> performinteraction_choice_set_map() const;

    /*
     * @brief Retrieve application commands
     */
    inline DataAccessor<CommandsMap> commands_map() const;

    /*
     * @brief Retrieve application sub menus
     */
    inline DataAccessor<SubMenuMap> sub_menu_map() const;

    /*
     * @brief Retrieve application choice set map
     */
    inline DataAccessor<ChoiceSetMap> choice_set_map() const;

    /*
     * @brief Sets perform interaction state
     *
     * @param active Current state of the perform interaction
     */
    void set_perform_interaction_active(uint32_t active);

    /*
     * @brief Retrieves perform interaction state
     *
     * @return TRUE if perform interaction active, otherwise FALSE
     */
    inline uint32_t is_perform_interaction_active() const;

    /*
     * @brief Sets the mode for perform interaction: UI/VR/BOTH
     *
     * @param mode Mode that was selected (MENU; VR; BOTH)
     */
    void set_perform_interaction_mode(int32_t mode);

    /*
     * @brief Retrieve the mode that was PerformInteraction sent in
     *
     * @return mode of PerformInteraction
     */
    inline int32_t perform_interaction_mode() const;

    /*
     * @brief Sets reset global properties state
     *
     * @param active Current state of the reset global properties
     */
    void set_reset_global_properties_active(bool active);

    /*
     * @brief Retrieves reset global properties state
     *
     * @return TRUE if perform interaction active, otherwise FALSE
     */
    inline bool is_reset_global_properties_active() const;

protected:
    smart_objects::SmartObject* help_prompt_;
    smart_objects::SmartObject* timeout_prompt_;
    smart_objects::SmartObject* vr_help_title_;
    smart_objects::SmartObject* vr_help_;
    mobile_api::TBTState::eType tbt_state_;
    smart_objects::SmartObject* show_command_;
    smart_objects::SmartObject* keyboard_props_;
    smart_objects::SmartObject* menu_title_;
    smart_objects::SmartObject* menu_icon_;
    smart_objects::SmartObject* tbt_show_command_;


    CommandsMap commands_;
    mutable sync_primitives::Lock commands_lock_;
    SubMenuMap sub_menu_;
    mutable sync_primitives::Lock sub_menu_lock_;
    ChoiceSetMap choice_set_map_;
    mutable sync_primitives::Lock choice_set_map_lock_;
    PerformChoiceSetMap performinteraction_choice_set_map_;
    mutable sync_primitives::Lock performinteraction_choice_set_lock_;
    uint32_t is_perform_interaction_active_;
    bool is_reset_global_properties_active_;
    int32_t perform_interaction_mode_;

private:
    void SetGlobalProperties(const smart_objects::SmartObject& param,
                             void (DynamicApplicationData::*callback)(
                               const NsSmartDeviceLink::NsSmartObjects::SmartObject&));
    DISALLOW_COPY_AND_ASSIGN(DynamicApplicationDataImpl);
};

DataAccessor<CommandsMap> DynamicApplicationDataImpl::commands_map() const {
  return DataAccessor<CommandsMap>(commands_, commands_lock_);
}

DataAccessor<SubMenuMap> DynamicApplicationDataImpl::sub_menu_map() const {
  return DataAccessor<SubMenuMap>(sub_menu_, sub_menu_lock_);
}

DataAccessor<ChoiceSetMap> DynamicApplicationDataImpl::choice_set_map() const {
  return DataAccessor<ChoiceSetMap>(choice_set_map_, choice_set_map_lock_);
}

DataAccessor<PerformChoiceSetMap>
DynamicApplicationDataImpl::performinteraction_choice_set_map() const {
  return DataAccessor<PerformChoiceSetMap>(
           performinteraction_choice_set_map_,
           performinteraction_choice_set_lock_);
}

uint32_t DynamicApplicationDataImpl::is_perform_interaction_active() const {
  return is_perform_interaction_active_;
}

bool DynamicApplicationDataImpl::is_reset_global_properties_active() const {
  return is_reset_global_properties_active_;
}

inline int32_t DynamicApplicationDataImpl::perform_interaction_mode() const {
  return perform_interaction_mode_;
}

}  //  namespace application_manager

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_DATA_IMPL_H_
