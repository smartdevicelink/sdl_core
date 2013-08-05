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

#include <map>
#include <set>
#include <vector>
/*#include <string>
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "interfaces/MOBILE_API.h"*/

#include "application_manager/application_data_impl.h"

namespace application_manager {

namespace mobile_api = mobile_apis;

/*struct InitialAppDataContainer {
  smart_objects::SmartObject* app_types_;
  smart_objects::SmartObject* vr_synonyms_;
  smart_objects::SmartObject* mobile_app_id_;
  smart_objects::SmartObject* tts_name_;
  smart_objects::SmartObject* ngn_media_screen_name_;
  mobile_api::Language::eType  language_;
  mobile_api::Language::eType  ui_language_;
};

struct DynamicAppDataContainer {
  smart_objects::SmartObject* help_promt_;
  smart_objects::SmartObject* timeout_promt_;
  smart_objects::SmartObject* vr_help_title_;
  smart_objects::SmartObject* vr_help_;
  mobile_api::TBTState::eType  tbt_state_;
  smart_objects::SmartObject* show_command_;
  smart_objects::SmartObject* tbt_show_command_;

  CommandsMap                  commands_;
  SubMenuMap                   sub_menu_;
  ChoiceSetMap                 choice_set_map_;
  ChoiceSetVRCmdMap            choice_set_vr_commands_map_;
  bool                         is_perform_interaction_active_;
};
*/
struct AppFile {
  AppFile(const std::string& name, bool persistent)
    : is_persistent(persistent),
      file_name(name) {}
  std::string file_name;
  bool is_persistent;
};

class ApplicationImpl : public virtual InitialApplicationDataImpl,
  public virtual DynamicApplicationDataImpl {
  public:
    explicit ApplicationImpl(int app_id);
    ~ApplicationImpl();

    /**
     * @brief Returns message belonging to the application
     * that is currently executed (i.e. on HMI).
     * @return smart_objects::SmartObject * Active message
     */
    const smart_objects::SmartObject* active_message() const;
    void CloseActiveMessage();
    bool IsFullscreen() const;
    bool MakeFullscreen();
    bool IsAudible() const;
    void MakeNotAudible();
    bool SupportsNavigation() const;
    void AllowNavigation(bool allow);
    inline bool app_allowed() const;
    bool HasBeenActivated() const;

    const Version& version() const;
    int app_id() const;
    const std::string& name() const;
    bool is_media_application() const;
    const mobile_api::HMILevel::eType& hmi_level() const;
    const mobile_api::SystemContext::eType& system_context() const;
    inline const mobile_api::AudioStreamingState::eType&
    audio_streaming_state() const;
    const std::string& app_icon_path() const;

    void set_version(const Version& version);
    void set_name(const std::string& name);
    void set_is_media_application(bool is_media);
    void set_hmi_level(const mobile_api::HMILevel::eType& hmi_level);
    void set_system_context(
      const mobile_api::SystemContext::eType& system_context);
    void set_audio_streaming_state(
      const mobile_api::AudioStreamingState::eType& state);
    bool set_app_icon_path(const std::string& file_name);
    void set_app_allowed(const bool& allowed);

    bool AddFile(const std::string& file_name, bool is_persistent);
    bool DeleteFile(const std::string& file_name);

    bool SubscribeToButton(unsigned int btn_name);
    bool IsSubscribedToButton(unsigned int btn_name);
    bool UnsubscribeFromButton(unsigned int btn_name);

    bool SubscribeToIVI(unsigned int vehicle_info_type_);
    bool IsSubscribedToIVI(unsigned int vehicle_info_type_);
    bool UnsubscribeFromIVI(unsigned int vehicle_info_type_);

  protected:
    void CleanupFiles();

  private:
    smart_objects::SmartObject*           active_message_;

    Version                                version_;
    int                                    app_id_;
    std::string                            app_name_;
    bool                                   is_media_;
    bool                                   allowed_support_navigation_;
    bool                                   is_app_allowed_;
    bool has_been_activated_;

    mobile_api::HMILevel::eType            hmi_level_;
    mobile_api::SystemContext::eType       system_context_;
    mobile_api::AudioStreamingState::eType audio_streaming_state_;
    std::string                            app_icon_path_;

    std::vector<AppFile>                   app_files_;
    std::set<unsigned int>                 subscribed_buttons_;
    std::set<unsigned int>                 subscribed_vehicle_info_;
};

const mobile_api::AudioStreamingState::eType&
ApplicationImpl::audio_streaming_state() const {
  return audio_streaming_state_;
}

bool ApplicationImpl::app_allowed() const {
  return is_app_allowed_;
}


}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
