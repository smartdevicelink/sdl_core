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

#include <string>
#include <map>

#include "SmartObjects/CSmartObject.hpp"

#include "application_manager/message.h"
#include "application_manager/application.h"
#include "application_manager/commands/command.h"

// TODO(AK): Include the directory when naming .h files
#include "v4_protocol_v2_0_revT.h"

namespace application_manager {

namespace mobile_api = NsSmartDeviceLinkRPC::V2;

struct InitialApplicationData {
  InitialApplicationData();
  ~InitialApplicationData();

  smart_objects::CSmartObject language_;
  smart_objects::CSmartObject ui_language_;
  smart_objects::CSmartObject* app_types_;
  smart_objects::CSmartObject* vr_synonyms_;
  smart_objects::CSmartObject* ngn_media_screen_name_;
  smart_objects::CSmartObject* mobile_app_id_;
  smart_objects::CSmartObject* tts_name_;
};

/*
 * @brief Typedef for supported commands in application menu
 */
typedef std::map<unsigned int, smart_objects::CSmartObject*> CommandsMap;
typedef std::map<unsigned int, smart_objects::CSmartObject*> SubMenuMap;

class ApplicationImpl : public Application {
  public:
    explicit ApplicationImpl(int app_id);
    ~ApplicationImpl();

    void ProcessMessage(smart_objects::CSmartObject* message);
    void ReportError(smart_objects::CSmartObject* message,
                     ErrorCode error_code);
    const smart_objects::CSmartObject* active_message() const;
    void CloseActiveMessage();
    const Version& version() const;
    int app_id() const;
    const std::string& name() const;

    bool is_media_application() const;
    const smart_objects::CSmartObject* app_types() const;
    const smart_objects::CSmartObject* vr_synonyms() const;
    const smart_objects::CSmartObject* mobile_app_id() const;
    const smart_objects::CSmartObject* tts_name() const;
    const smart_objects::CSmartObject* ngn_media_screen_name() const;
    const smart_objects::CSmartObject& hmi_level() const;
    const smart_objects::CSmartObject& system_context() const;
    const smart_objects::CSmartObject& language() const;
    const smart_objects::CSmartObject& ui_language() const;
    const smart_objects::CSmartObject* help_promt() const;
    const smart_objects::CSmartObject* timeout_promt() const;
    const smart_objects::CSmartObject* vr_help_title() const;
    const smart_objects::CSmartObject* vr_help() const;

    void set_version(const Version& version);
    void set_name(const std::string& name);
    void set_is_media_application(bool is_media);
    void set_hmi_level(const smart_objects::CSmartObject& hmi_level);
    void set_system_context(const smart_objects::CSmartObject& system_context);
    void set_language(const smart_objects::CSmartObject& language);
    void set_ui_language(const smart_objects::CSmartObject& ui_language);
    void set_app_types(const smart_objects::CSmartObject& app_types);
    void set_vr_synonyms(const smart_objects::CSmartObject& vr_synonyms);
    void set_mobile_app_id(const smart_objects::CSmartObject& mobile_app_id);
    void set_tts_name(const smart_objects::CSmartObject& tts_name);
    void set_ngn_media_screen_name(const smart_objects::CSmartObject& ngn_name);
    void set_help_prompt(const smart_objects::CSmartObject& help_promt);
    void set_timeout_prompt(const smart_objects::CSmartObject& timeout_promt);
    void set_vr_help_title(const smart_objects::CSmartObject& vr_help_title);
    void set_vr_help(const smart_objects::CSmartObject& vr_help);

    /*
     * @brief Adds a command to the in application menu
     */
    void AddCommand(unsigned int cmd_id,
                    const smart_objects::CSmartObject& command);

    /*
     * @brief Deletes all commands from the application menu with the specified command id
     */
    void RemoveCommand(unsigned int cmd_id);

    /*
     * @brief Finds command with the specified command id
     */
    smart_objects::CSmartObject*  FindCommand(unsigned int cmd_id);

  /*
   * @brief Adds a menu to the application
   */
  void AddSubMenu(unsigned int menu_id,
                  const smart_objects::CSmartObject& menu);

  /*
   * @brief Deletes menu from the application menu
   */
  void RemoveSubMenu(unsigned int menu_id);

  /*
   * @brief Finds menu with the specified id
   */
  smart_objects::CSmartObject*  FindSubMenu(unsigned int menu_id);

  /*
   * @brief Retrieve application commands
   */
  inline const CommandsMap& commands_map() const;

  private:
    Version version_;
    int app_id_;
    std::string app_name_;

    smart_objects::CSmartObject* active_message_;
    InitialApplicationData initial_app_data_;

    bool is_media_;
    smart_objects::CSmartObject hmi_level_;
    smart_objects::CSmartObject system_context_;

    smart_objects::CSmartObject* help_promt_;
    smart_objects::CSmartObject* timeout_promt_;
    smart_objects::CSmartObject* vr_help_title_;
    smart_objects::CSmartObject* vr_help_;

    CommandsMap commands_;
    SubMenuMap sub_menu_;

};

const CommandsMap& ApplicationImpl::commands_map() const {
  return commands_;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
