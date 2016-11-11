/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_H_

#include <string>
#include <map>
#include <set>
#include <list>
#include "utils/shared_ptr.h"
#include "utils/data_accessor.h"
#include "interfaces/MOBILE_API.h"
#include "connection_handler/device.h"
#include "application_manager/message.h"
#include "application_manager/hmi_state.h"
#include "application_manager/application_state.h"
#include "protocol_handler/protocol_handler.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace mobile_api = mobile_apis;
namespace custom_str = utils::custom_string;

typedef int32_t ErrorCode;

class UsageStatistics;

enum APIVersion {
  kUnknownAPI = -1,
  kAPIV0 = 0,
  kAPIV1 = 1,
  kAPIV2 = 2,
  kAPIV3 = 3,
  kAPIV4 = 4
};

enum TLimitSource { POLICY_TABLE = 0, CONFIG_FILE };

struct Version {
  APIVersion min_supported_api_version;
  APIVersion max_supported_api_version;

  Version()
      : min_supported_api_version(APIVersion::kUnknownAPI)
      , max_supported_api_version(APIVersion::kUnknownAPI) {}
};

struct AppFile {
  // need to use in std::map;
  AppFile() {}
  AppFile(const std::string& name,
          bool persistent,
          bool download_complete,
          mobile_apis::FileType::eType type)
      : file_name(name)
      , is_persistent(persistent)
      , is_download_complete(download_complete)
      , file_type(type) {}
  std::string file_name;
  bool is_persistent;
  bool is_download_complete;
  mobile_apis::FileType::eType file_type;
};
typedef std::map<std::string, AppFile> AppFilesMap;
class InitialApplicationData {
 public:
  virtual ~InitialApplicationData() {}

  virtual const smart_objects::SmartObject* app_types() const = 0;
  virtual const smart_objects::SmartObject* vr_synonyms() const = 0;
  virtual const std::string& mac_address() const = 0;
  virtual const std::string& bundle_id() const = 0;
  virtual void set_bundle_id(const std::string& bundle_id) = 0;
  virtual std::string policy_app_id() const = 0;
  virtual const smart_objects::SmartObject* tts_name() const = 0;
  virtual const smart_objects::SmartObject* ngn_media_screen_name() const = 0;
  virtual const mobile_api::Language::eType& language() const = 0;
  virtual const mobile_api::Language::eType& ui_language() const = 0;
  virtual void set_app_types(const smart_objects::SmartObject& app_types) = 0;
  virtual void set_vr_synonyms(
      const smart_objects::SmartObject& vr_synonyms) = 0;
  virtual void set_mobile_app_id(const std::string& policy_app_id) = 0;
  virtual void set_tts_name(const smart_objects::SmartObject& tts_name) = 0;
  virtual void set_ngn_media_screen_name(
      const smart_objects::SmartObject& ngn_name) = 0;
  virtual void set_language(const mobile_api::Language::eType& language) = 0;
  virtual void set_ui_language(
      const mobile_api::Language::eType& ui_language) = 0;
};

/*
 * @brief Typedef for supported commands in application menu
 */
typedef std::map<uint32_t, smart_objects::SmartObject*> CommandsMap;

/*
 * @brief Typedef for supported sub menu in application menu
 */
typedef std::map<uint32_t, smart_objects::SmartObject*> SubMenuMap;

/*
 * @brief Typedef for interaction choice set
 */
typedef std::map<uint32_t, smart_objects::SmartObject*> ChoiceSetMap;

/*
 * @brief Typedef for perform interaction choice
 * @param choice id
 * @param SmartObject choice
 */
typedef std::map<uint32_t, smart_objects::SmartObject*> PerformChoice;

/*
 * @brief Typedef for perform interaction choice set
 * @param request corellation id
 * @param map of choices
 */
typedef std::map<uint32_t, PerformChoice> PerformChoiceSetMap;

/**
 * @brief Defines id of SoftButton
 */
typedef std::set<uint32_t> SoftButtonID;

/**
 * @brief Defines set of vehicle info types
 */
typedef std::set<uint32_t> VehicleInfoSubscriptions;

/**
 * @brief Defines set of buttons subscription
 */
typedef std::set<mobile_apis::ButtonName::eType> ButtonSubscriptions;

class DynamicApplicationData {
 public:
  virtual ~DynamicApplicationData() {}
  virtual const smart_objects::SmartObject* help_prompt() const = 0;
  virtual const smart_objects::SmartObject* timeout_prompt() const = 0;
  virtual const smart_objects::SmartObject* vr_help_title() const = 0;
  virtual const smart_objects::SmartObject* vr_help() const = 0;
  virtual const mobile_api::TBTState::eType& tbt_state() const = 0;
  virtual const smart_objects::SmartObject* show_command() const = 0;
  virtual const smart_objects::SmartObject* tbt_show_command() const = 0;
  virtual DataAccessor<ButtonSubscriptions> SubscribedButtons() const = 0;
  virtual DataAccessor<VehicleInfoSubscriptions> SubscribedIVI() const = 0;
  virtual const smart_objects::SmartObject* keyboard_props() const = 0;
  virtual const smart_objects::SmartObject* menu_title() const = 0;
  virtual const smart_objects::SmartObject* menu_icon() const = 0;

  virtual void load_global_properties(const smart_objects::SmartObject& so) = 0;
  virtual void set_help_prompt(
      const smart_objects::SmartObject& help_prompt) = 0;
  virtual void set_timeout_prompt(
      const smart_objects::SmartObject& timeout_prompt) = 0;
  virtual void set_vr_help_title(
      const smart_objects::SmartObject& vr_help_title) = 0;
  virtual void reset_vr_help_title() = 0;
  virtual void set_vr_help(const smart_objects::SmartObject& vr_help) = 0;
  virtual void reset_vr_help() = 0;
  virtual void set_tbt_state(const mobile_api::TBTState::eType& tbt_state) = 0;
  virtual void set_show_command(
      const smart_objects::SmartObject& show_command) = 0;
  virtual void set_tbt_show_command(
      const smart_objects::SmartObject& tbt_show) = 0;
  virtual void set_keyboard_props(
      const smart_objects::SmartObject& keyboard_props) = 0;
  virtual void set_menu_title(const smart_objects::SmartObject& menu_title) = 0;
  virtual void set_menu_icon(const smart_objects::SmartObject& menu_icon) = 0;

  virtual uint32_t audio_stream_retry_number() const = 0;

  virtual void set_audio_stream_retry_number(
      const uint32_t& audio_stream_retry_number) = 0;

  virtual uint32_t video_stream_retry_number() const = 0;

  virtual void set_video_stream_retry_number(
      const uint32_t& video_stream_retry_number) = 0;

  /*
   * @brief Adds a command to the in application menu
   */
  virtual void AddCommand(uint32_t cmd_id,
                          const smart_objects::SmartObject& command) = 0;

  /*
   * @brief Deletes all commands from the application
   * menu with the specified command id
   */
  virtual void RemoveCommand(uint32_t cmd_id) = 0;

  /*
   * @brief Finds command with the specified command id
   */
  virtual smart_objects::SmartObject* FindCommand(uint32_t cmd_id) = 0;

  /*
   * @brief Adds a menu to the application
   */
  virtual void AddSubMenu(uint32_t menu_id,
                          const smart_objects::SmartObject& menu) = 0;

  /*
   * @brief Deletes menu from the application menu
   */
  virtual void RemoveSubMenu(uint32_t menu_id) = 0;

  /*
   * @brief Finds menu with the specified id
   */
  virtual smart_objects::SmartObject* FindSubMenu(uint32_t menu_id) const = 0;

  /*
   * @brief Returns true if sub menu with such name already exist
   */
  virtual bool IsSubMenuNameAlreadyExist(const std::string& name) = 0;

  /*
   * @brief Adds a interaction choice set to the application
   *
   * @param choice_set_id Unique ID used for this interaction choice set
   * @param choice_set SmartObject that represent choice set
   */
  virtual void AddChoiceSet(uint32_t choice_set_id,
                            const smart_objects::SmartObject& choice_set) = 0;

  /*
   * @brief Deletes choice set from the application
   *
   * @param choice_set_id Unique ID of the interaction choice set
   */
  virtual void RemoveChoiceSet(uint32_t choice_set_id) = 0;

  /*
   * @brief Finds choice set with the specified choice_set_id id
   *
   * @param choice_set_id Unique ID of the interaction choice set
   */
  virtual smart_objects::SmartObject* FindChoiceSet(uint32_t choice_set_id) = 0;

  /*
   * @brief Adds perform interaction choice set to the application
   *
   * @param correlation_id Unique ID of the request that added this choice set
   * @param choice_set_id  Unique ID used for this interaction choice set
   * @param choice_set SmartObject that represents choice set
   */
  virtual void AddPerformInteractionChoiceSet(
      uint32_t correlation_id,
      uint32_t choice_set_id,
      const smart_objects::SmartObject& choice_set) = 0;

  /*
   * @brief Deletes entirely perform interaction choice set for request
   * @param correlation_id Unique ID of the request that added this choice set
   *
   */
  virtual void DeletePerformInteractionChoiceSet(uint32_t correlation_id) = 0;

  /*
   * @brief Retrieves entirely ChoiceSet - VR commands map
   *
   * @return ChoiceSet map that is currently in use
   */
  virtual DataAccessor<PerformChoiceSetMap> performinteraction_choice_set_map()
      const = 0;

  /*
   * @brief Retrieve application commands
   */
  virtual DataAccessor<CommandsMap> commands_map() const = 0;

  /*
   * @brief Retrieve application sub menus
   */
  virtual DataAccessor<SubMenuMap> sub_menu_map() const = 0;

  /*
   * @brief Retrieve application choice set map
   */
  virtual DataAccessor<ChoiceSetMap> choice_set_map() const = 0;

  /*
   * @brief Sets perform interaction state
   *
   * @param active Current state of the perform interaction
   */
  virtual void set_perform_interaction_active(uint32_t active) = 0;

  /*
   * @brief Retrieves perform interaction state
   *
   * @return TRUE if perform interaction active, otherwise FALSE
   */
  virtual uint32_t is_perform_interaction_active() const = 0;

  /*
  * @brief Set perform interaction layout
  *
  * @param Current Interaction layout of the perform interaction
  */
  virtual void set_perform_interaction_layout(
      mobile_api::LayoutMode::eType layout) = 0;

  /*
   * @brief Retrieve perform interaction layout
   */
  virtual mobile_api::LayoutMode::eType perform_interaction_layout() const = 0;

  /*
     * @brief Sets the mode for perform interaction: UI/VR/BOTH
     *
     * @param mode Mode that was selected (MENU; VR; BOTH)
     */
  virtual void set_perform_interaction_mode(int32_t mode) = 0;

  /*
   * @brief Retrieve the mode that was PerformInteraction sent in
   *
   * @return mode of PerformInteraction
   */
  virtual int32_t perform_interaction_mode() const = 0;

  /*
   * @brief Sets reset global properties state
   *
   * @param active Current state of the reset global properties
   */
  virtual void set_reset_global_properties_active(bool active) = 0;

  /*
   * @brief Retrieves reset global properties state
   *
   * @return TRUE if perform interaction active, otherwise FALSE
   */
  virtual bool is_reset_global_properties_active() const = 0;
};

class Application : public virtual InitialApplicationData,
                    public virtual DynamicApplicationData {
 public:
  enum ApplicationRegisterState { kRegistered = 0, kWaitingForRegistration };

 public:
  Application() : is_greyed_out_(false) {}
  virtual ~Application() {}

  /**
   * @brief Returns message belonging to the application
   * that is currently executed (i.e. on HMI).
   * @return smart_objects::SmartObject * Active message
   */
  virtual const smart_objects::SmartObject* active_message() const = 0;

  /**
   * @brief returns current hash value
   * @return current hash value
   */
  virtual const std::string& curHash() const = 0;

  /**
   * @brief Change Hash for current application
   * and send notification to mobile
   * @return updated_hash
   */
  virtual void UpdateHash() = 0;

  /**
   * @brief method is called when SDL is saving application data for resumption
   * @return TRUE if data of application need to save for resumption, otherwise
   * return FALSE
   */
  virtual bool is_application_data_changed() const = 0;

  /**
   * @brief method is called after SDL saved application data for resumption
   * @param state_application_data contains FALSE after saving data
   */
  virtual void set_is_application_data_changed(bool state_application_data) = 0;

  virtual void CloseActiveMessage() = 0;
  virtual bool IsFullscreen() const = 0;
  virtual void ChangeSupportingAppHMIType() = 0;

  virtual bool is_navi() const = 0;
  virtual void set_is_navi(bool allow) = 0;

  virtual bool video_streaming_approved() const = 0;
  virtual void set_video_streaming_approved(bool state) = 0;
  virtual bool audio_streaming_approved() const = 0;
  virtual void set_audio_streaming_approved(bool state) = 0;

  virtual bool video_streaming_allowed() const = 0;
  virtual void set_video_streaming_allowed(bool state) = 0;
  virtual bool audio_streaming_allowed() const = 0;
  virtual void set_audio_streaming_allowed(bool state) = 0;

  /**
   * @brief Starts streaming service for application
   * @param service_type Type of streaming service
   */
  virtual void StartStreaming(protocol_handler::ServiceType service_type) = 0;

  /**
   * @brief Stops streaming service for application
   * @param service_type Type of streaming service
   */
  virtual void StopStreaming(protocol_handler::ServiceType service_type) = 0;

  /**
   * @brief Stops streaming for application whether it is allowed or not HMI
   * @param service_type video or audio
   */
  virtual void StopStreamingForce(
      protocol_handler::ServiceType service_type) = 0;

  /**
   * @brief Suspends streaming process for application
   * @param service_type Type of streaming service
   */
  virtual void SuspendStreaming(protocol_handler::ServiceType service_type) = 0;

  /**
   * @brief Wakes up streaming process for application
   * @param service_type Type of streaming service
   */
  virtual void WakeUpStreaming(protocol_handler::ServiceType service_type) = 0;

  virtual bool is_voice_communication_supported() const = 0;
  virtual void set_voice_communication_supported(
      bool is_voice_communication_supported) = 0;
  virtual bool app_allowed() const = 0;
  virtual bool has_been_activated() const = 0;
  virtual bool set_activated(bool is_active) = 0;

  virtual const Version& version() const = 0;
  virtual void set_hmi_application_id(uint32_t hmi_app_id) = 0;
  virtual uint32_t hmi_app_id() const = 0;
  virtual uint32_t app_id() const = 0;
  virtual const custom_str::CustomString& name() const = 0;
  /**
   * @brief Sets application folder name, which is used for storing of related
   * files, e.g. icons
   * @param folder_name Name of folder
   */
  virtual void set_folder_name(const std::string& folder_name) = 0;
  virtual const std::string folder_name() const = 0;
  virtual bool is_media_application() const = 0;
  virtual bool is_foreground() const = 0;
  virtual void set_foreground(const bool is_foreground) = 0;
  virtual const mobile_api::HMILevel::eType hmi_level() const = 0;
  virtual const uint32_t put_file_in_none_count() const = 0;
  virtual const uint32_t delete_file_in_none_count() const = 0;
  virtual const uint32_t list_files_in_none_count() const = 0;
  virtual const mobile_api::SystemContext::eType system_context() const = 0;
  virtual const mobile_api::AudioStreamingState::eType audio_streaming_state()
      const = 0;
  virtual const std::string& app_icon_path() const = 0;
  virtual connection_handler::DeviceHandle device() const = 0;

  /**
   * @brief sets true if application has sent TTS GlobalProperties
   * request with empty array help_prompt to HMI with level
   * NONE BACKGROUND
   * @param active contains state of sending TTS GlobalProperties
   */
  virtual void set_tts_properties_in_none(bool active) = 0;
  /**
   * @brief returns true if application has sent TTS GlobalProperties
   * otherwise return false
   * @return flag tts_properties_in_none
   */
  virtual bool tts_properties_in_none() = 0;
  /**
   * @brief sets true if application has sent TTS GlobalProperties
   * request with default array help_prompt to HMI with level
   * FULL LIMITED
   * @param active contains state of sending TTS GlobalProperties
   */
  virtual void set_tts_properties_in_full(bool active) = 0;
  /**
   * @brief  returns true if application has sent TTS GlobalProperties
   * otherwise return false
   * @return flag tts_properties_in_full
   */
  virtual bool tts_properties_in_full() = 0;
  virtual void set_version(const Version& version) = 0;
  virtual void set_name(const custom_str::CustomString& name) = 0;
  virtual void set_is_media_application(bool is_media) = 0;
  virtual void increment_put_file_in_none_count() = 0;
  virtual void increment_delete_file_in_none_count() = 0;
  virtual void increment_list_files_in_none_count() = 0;
  virtual bool set_app_icon_path(const std::string& file_name) = 0;
  virtual void set_app_allowed(const bool allowed) = 0;
  virtual void set_device(connection_handler::DeviceHandle device) = 0;
  virtual uint32_t get_grammar_id() const = 0;
  virtual void set_grammar_id(uint32_t value) = 0;

  virtual void set_protocol_version(
      const ProtocolVersion& protocol_version) = 0;
  virtual ProtocolVersion protocol_version() const = 0;

  virtual void set_is_resuming(bool is_resuming) = 0;
  virtual bool is_resuming() const = 0;

  virtual bool AddFile(const AppFile& file) = 0;
  virtual const AppFilesMap& getAppFiles() const = 0;

  /**
   * @brief Updates fields of existing file
   * @param file_name File name, that need to update
   * @param is_persistent Bollean describes is file persistent?
   * @param is_download_complete Bollean describes is file downloaded fully on
   * need to finish downloading?
   * @return TRUE if file exist and updated sucsesfuly, othervise return false
   */
  virtual bool UpdateFile(const AppFile& file) = 0;
  virtual bool DeleteFile(const std::string& file_name) = 0;
  virtual const AppFile* GetFile(const std::string& file_name) = 0;

  virtual bool SubscribeToButton(mobile_apis::ButtonName::eType btn_name) = 0;
  virtual bool IsSubscribedToButton(
      mobile_apis::ButtonName::eType btn_name) = 0;
  virtual bool UnsubscribeFromButton(
      mobile_apis::ButtonName::eType btn_name) = 0;

  virtual bool SubscribeToIVI(uint32_t vehicle_info_type) = 0;
  virtual bool IsSubscribedToIVI(uint32_t vehicle_info_type) const = 0;
  virtual bool UnsubscribeFromIVI(uint32_t vehicle_info_type) = 0;

  /**
   * @brief ResetDataInNone reset data counters in NONE
   */
  virtual void ResetDataInNone() = 0;

  /**
   * @brief Check, if limits for command number per time is exceeded
   * @param cmd_id Unique command id from mobile API
   * @param source Limits source, e.g. policy table, config file etc.
   * @return true, if - excedeed, otherwise - false
   */
  virtual bool AreCommandLimitsExceeded(mobile_apis::FunctionID::eType cmd_id,
                                        TLimitSource source) = 0;

  /**
   * Returns object for recording statistics
   * @return object for recording statistics
   */
  virtual UsageStatistics& usage_report() = 0;

  /**
   * @brief SetRegularState set permanent state of application
   *
   * @param state state to setup
   */
  virtual void SetRegularState(HmiStatePtr state) = 0;

  /**
  * @brief SetPostponedState sets postponed state to application.
  * This state could be set as regular later
  *
  * @param state state to setup
  */
  virtual void SetPostponedState(HmiStatePtr state) = 0;

  virtual void RemovePostponedState() = 0;

  /**
   * @brief AddHMIState the function that will change application's
   * hmi state.
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state new hmi state for certain application.
   */
  virtual void AddHMIState(HmiStatePtr state) = 0;

  /**
   * @brief RemoveHMIState the function that will turn back hmi_level after end
   * of some event
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state_id that should be removed
   */
  virtual void RemoveHMIState(HmiState::StateID state_id) = 0;

  /**
   * @brief HmiState of application within active events PhoneCall, TTS< etc ...
   * @return Active HmiState of application
   */
  virtual const HmiStatePtr CurrentHmiState() const = 0;

  /**
   * @brief RegularHmiState of application without active events VR, TTS etc ...
   * @return HmiState of application
   */
  virtual const HmiStatePtr RegularHmiState() const = 0;

  /**
   * @brief PostponedHmiState returns postponed hmi state of application
   * if it's present
   *
   * @return Postponed hmi state of application
   */
  virtual const HmiStatePtr PostponedHmiState() const = 0;

  /**
   * @brief Keeps id of softbuttons which is created in commands:
   * Alert, Show, ScrollableMessage, ShowConstantTBT, AlertManeuver,
   * UpdateTurnList
   * @param cmd_id Unique command id from mobile API
   * @param list of softbuttons were created by command.
   */
  virtual void SubscribeToSoftButtons(int32_t cmd_id,
                                      const SoftButtonID& softbuttons_id) = 0;

  /**
   * @brief Determine the existence of softbutton
   * @param Softbutton_id contains id of softbutton
   * @return Returns true if application contains softbutton id otherwise
   * returns false.
   */
  virtual bool IsSubscribedToSoftButton(const uint32_t softbutton_id) = 0;

  /**
   * @brief Removes list of softbuttons which is created in commands
   * @param cmd_id Unique command id from mobile API
   */
  virtual void UnsubscribeFromSoftButtons(int32_t cmd_id) = 0;

  /**
   * @brief Check's if it is media, voice communication or navigation
   * application
   *
   * @return true if application is media, voice communication or navigation
   */
  virtual bool IsAudioApplication() const = 0;

  /**
   * @brief IsRegistered allows to distinguish if this
   * application has been registered.
   *
   * @return true if registered, false otherwise.
   */
  virtual bool IsRegistered() const = 0;
  /**
   * @brief MarkRegistered allows to mark application as registered.
   */
  void MarkRegistered() {
    app_state_ = kRegistered;
  }

  /**
   * @brief MarkUnregistered allows to mark application as unregistered.
   */
  void MarkUnregistered() {
    app_state_ = kWaitingForRegistration;
  }

  /**
   * @brief schemaUrl contains application's url (for 4th protocol version)
   *
   * @return application's url.
   */
  std::string SchemaUrl() const {
    return url_;
  }

  /**
   * @brief SetShemaUrl allows to store schema url for application.
   *
   * @param url url to store.
   */
  void SetShemaUrl(const std::string& url) {
    url_ = url;
  }

  /**
   * @brief packagName allows to obtain application's package name.
   *
   * @return pakage name.
   */
  std::string PackageName() const {
    return package_name_;
  }

  /**
   * @brief SetPackageName allows to store package name for application.
   *
   * @param packageName package name to store.
   */
  void SetPackageName(const std::string& packageName) {
    package_name_ = packageName;
  }

  /**
   * @brief GetDeviceId allows to obtain device id which posseses
   * by this application.
   *
   * @return device the device id.
   */
  std::string GetDeviceId() const {
    return device_id_;
  }

  /**
   * @brief Returns is application should be greyed out on HMI
   */
  bool is_greyed_out() const {
    return is_greyed_out_;
  }

  /**
   * @brief Sets application as should be greyed out on HMI
   * @param is_greyed_out True, if should be greyed out on HMI,
   * otherwise - false
   */
  void set_greyed_out(bool is_greyed_out) {
    is_greyed_out_ = is_greyed_out;
  }
  /**
   * @brief Load persistent files from application folder.
   */
  virtual void LoadPersistentFiles() = 0;

  /**
   * @brief Get available app space
   * @param name of the app folder(make + mobile app id)
   * @return free app space.
   */
  virtual uint32_t GetAvailableDiskSpace() = 0;

 protected:
  mutable sync_primitives::Lock hmi_states_lock_;

  ApplicationRegisterState app_state_;
  ApplicationState state_;
  std::string url_;
  std::string package_name_;
  std::string device_id_;
  ssize_t connection_id_;
  bool is_greyed_out_;
};

typedef utils::SharedPtr<Application> ApplicationSharedPtr;
typedef utils::SharedPtr<const Application> ApplicationConstSharedPtr;

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_H_
