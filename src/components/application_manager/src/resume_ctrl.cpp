#include <fstream>

#include "application_manager/resume_ctrl.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "connection_handler/connection_handler_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "smart_objects/smart_object.h"
#include "connection_handler/connection.h"
#include "formatters/CFormatterJsonBase.hpp"
#include "application_manager/commands/command_impl.h"
#include "resumption/last_state.h"
#include "policy/policy_manager_impl.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ResumeCtrl")

namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

ResumeCtrl::ResumeCtrl(ApplicationManagerImpl* app_mngr)
  : app_mngr_(app_mngr),
    timer_("ResumeCtrl", this, &ResumeCtrl::onTimer) {
}

void ResumeCtrl::SaveAllApplications() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::SaveApplications()");
  DCHECK(app_mngr_);

  std::set<ApplicationSharedPtr>::iterator it =
                  app_mngr_->application_list_.begin();
  std::set<ApplicationSharedPtr>::iterator it_end =
                   app_mngr_->application_list_.end();
  for (; it != it_end; ++it) {
    SaveApplication(*it);
  }
}

void ResumeCtrl::SaveApplication(ApplicationConstSharedPtr application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::SaveApplication");

  DCHECK(application.get());

  Json::Value* json_app = NULL;

  const std::string& m_app_id = application->mobile_app_id()->asString();
  Json::Value::iterator it = GetSavedApplications().begin();
  for (; it != GetSavedApplications().end(); ++it) {
    if (m_app_id == (*it)[strings::app_id].asString()) {
      json_app = &(*it);
      LOG4CXX_INFO(logger_, "ResumeCtrl Application with this id "
                            "already exist ( update info )."
                            "mobile app_id = " << m_app_id);
      break;
    }
  }

  if (json_app == NULL) {
    LOG4CXX_INFO(logger_, "ResumeCtrl Application with this ID does not"
         "exist. Add new. mobile app_id = " << m_app_id);
    json_app = &(GetSavedApplications().append(Json::Value()));
  }

  uint32_t hash = application->curHash();
  uint32_t grammar_id = application->get_grammar_id();

  LOG4CXX_INFO(logger_, "Hash = " << hash);
  (*json_app)[strings::device_mac] =
      MessageHelper::GetDeviceMacAddressForHandle(application->device());
  (*json_app)[strings::app_id] = m_app_id;
  (*json_app)[strings::grammar_id] = grammar_id;
  (*json_app)[strings::connection_key] = application->app_id();
  (*json_app)[strings::hmi_app_id] = application->hmi_app_id();
  (*json_app)[strings::hmi_level] =
      static_cast<int32_t> (application->hmi_level());
  (*json_app)[strings::ign_off_count] = 0;
  (*json_app)[strings::hash_id] = hash;
  (*json_app)[strings::application_commands] =
      GetApplicationCommands(application);
  (*json_app)[strings::application_submenus] =
      GetApplicationSubMenus(application);
  (*json_app)[strings::application_choise_sets] =
      GetApplicationInteractionChoiseSets(application);
  (*json_app)[strings::application_global_properties] =
      GetApplicationGlobalProperties(application);
  (*json_app)[strings::application_subscribtions] =
      GetApplicationSubscriptions(application);
  (*json_app)[strings::application_files] = GetApplicationFiles(application);
  (*json_app)[strings::time_stamp] = (uint32_t)time(NULL);
  (*json_app)[strings::audio_streaming_state] = application->audio_streaming_state();
}

void ResumeCtrl::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::on_event ");
}

bool ResumeCtrl::RestoreApplicationHMILevel(ApplicationSharedPtr application) {
  if (false == application.valid()) {
    LOG4CXX_ERROR(logger_, " RestoreApplicationHMILevel() application pointer in invalid");
    return false;
  }
  Json::Value::iterator it = GetSavedApplications().begin();
  for (;it != GetSavedApplications().end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();

    if (saved_m_app_id == application->mobile_app_id()->asString()) {

      mobile_apis::HMILevel::eType saved_hmi_level;
      //mobile_apis::HMILevel::eType restored_hmi_level;

      mobile_apis::AudioStreamingState::eType audio_streaming_state =
          static_cast<mobile_apis::AudioStreamingState::eType>
      ((*it)[strings::audio_streaming_state].asInt());
        application->set_audio_streaming_state(audio_streaming_state);
      saved_hmi_level = static_cast<mobile_apis::HMILevel::eType>(
                            (*it)[strings::hmi_level].asInt());

      return SetupHMILevel(application, saved_hmi_level, audio_streaming_state);
    }
  }
  LOG4CXX_INFO(logger_, "Failed to restore application HMILevel");
  return false;
}

bool ResumeCtrl::SetupDefaultHMILevel(ApplicationSharedPtr application) {
  LOG4CXX_TRACE_ENTER(logger_);
  if (false == application.valid()) {
    LOG4CXX_ERROR(logger_, "SetupDefaultHMILevel application pointer is invalid");
    return false;
  }

  mobile_apis::HMILevel::eType default_hmi = mobile_apis::HMILevel::HMI_NONE;

  if (policy::PolicyHandler::instance()->PolicyEnabled()) {
    std::string policy_app_id = application->mobile_app_id()->asString();
    std::string default_hmi_string = "";
    bool result_get_hmi = policy::PolicyHandler::instance()->GetDefaultHmi(
          policy_app_id, &default_hmi_string);
    if (true == result_get_hmi) {
      if ("BACKGROUND" == default_hmi_string) {
        default_hmi = mobile_apis::HMILevel::HMI_BACKGROUND;
      } else if ("FULL" == default_hmi_string) {
        default_hmi = mobile_apis::HMILevel::HMI_FULL;
      } else if ("LIMITED" == default_hmi_string) {
        default_hmi = mobile_apis::HMILevel::HMI_LIMITED;
      } else if ("NONE" == default_hmi_string) {
        default_hmi = mobile_apis::HMILevel::HMI_NONE;
      } else {
        LOG4CXX_ERROR(logger_, "Unable to convert " + default_hmi_string + "to HMILevel");
        return false;
      }
    } else {
      LOG4CXX_ERROR(logger_, "SetupDefaultHMILevel() unable to get default hmi_level for "
                    << policy_app_id);
    }
  }

  return SetupHMILevel(application, default_hmi,
                       mobile_apis::AudioStreamingState::NOT_AUDIBLE, false);
}

bool ResumeCtrl::SetupHMILevel(ApplicationSharedPtr application,
                               mobile_apis::HMILevel::eType hmi_level,
                               mobile_apis::AudioStreamingState::eType audio_streaming_state,
                               bool check_policy) {

  if (false == application.valid()) {
    LOG4CXX_ERROR(logger_, "SetupHMILevel() application pointer in invalid");
    return false;
  }

  const std::string device_id =
      MessageHelper::GetDeviceMacAddressForHandle(application->device());

  if (check_policy &&
      policy::PolicyHandler::instance()->GetUserConsentForDevice(device_id)
      != policy::DeviceConsent::kDeviceAllowed) {
    LOG4CXX_ERROR(logger_, "Resumption abort. Data consent wasn't allowed");
    SetupDefaultHMILevel(application);
    return false;
  }


  if ((hmi_level == application->hmi_level()) &&
      (hmi_level != mobile_apis::HMILevel::HMI_NONE)) {
    LOG4CXX_WARN(logger_, "Hmi level " << hmi_level << " should not be set to "
                 << application->mobile_app_id()->asString() << "  " << application->hmi_level());

    return false;
  }

  if (mobile_apis::HMILevel::HMI_FULL == hmi_level) {
    hmi_level = app_mngr_->PutApplicationInFull(application);

    if ((mobile_apis::HMILevel::HMI_FULL == hmi_level ||
        mobile_apis::HMILevel::HMI_LIMITED == hmi_level) &&
        (mobile_apis::AudioStreamingState::AUDIBLE == audio_streaming_state)) {
      application->set_audio_streaming_state(audio_streaming_state);
    }
  } else if (mobile_apis::HMILevel::HMI_LIMITED == hmi_level) {
    if ((false == application->IsAudioApplication()) ||
        app_mngr_->DoesAudioAppWithSameHMITypeExistInFullOrLimited(application)) {
      hmi_level = mobile_apis::HMILevel::HMI_BACKGROUND;
    } else {
      if (audio_streaming_state == mobile_apis::AudioStreamingState::AUDIBLE) {
        //implemented SDLAQ-CRS-839
        //checking the existence of application with AudioStreamingState=AUDIBLE
        //notification resumeAudioSource is sent if only resumed application has
        //AudioStreamingState=AUDIBLE
        bool application_exist_with_audible_state = false;
        ApplicationManagerImpl::ApplicationListAccessor accessor;
        const std::set<ApplicationSharedPtr> app_list = accessor.applications();
        std::set<ApplicationSharedPtr>::const_iterator app_list_it = app_list
            .begin();
        uint32_t app_id = application->app_id();
        for (; app_list.end() != app_list_it; ++app_list_it) {
          if ((mobile_apis::AudioStreamingState::AUDIBLE ==
              (*app_list_it)->audio_streaming_state())
              && ((*app_list_it))->app_id() != app_id) {
            application_exist_with_audible_state = true;
            break;
          }
        }
        if (application_exist_with_audible_state) {
          application->set_audio_streaming_state(
              mobile_apis::AudioStreamingState::NOT_AUDIBLE);
        } else {
          MessageHelper::SendOnResumeAudioSourceToHMI(application->app_id());
        }
      }
    }
  }

  if (hmi_level != mobile_apis::HMILevel::HMI_FULL) {
    application->set_hmi_level(hmi_level);
  }

  MessageHelper::SendHMIStatusNotification(*(application.get()));

  LOG4CXX_INFO(logger_, "Set up application "
               << application->mobile_app_id()->asString()
               << " to HMILevel " << hmi_level);
  return true;
}

bool ResumeCtrl::RestoreApplicationData(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "RestoreApplicationData");
  DCHECK(application.get());

  Json::Value::iterator it = GetSavedApplications().begin();
  for (; it != GetSavedApplications().end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id == application->mobile_app_id()->asString()) {
      break;
    }
  }

  if (it == GetSavedApplications().end()) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  Json::Value& saved_app = *it;
  MessageHelper::SmartObjectList requests;

  LOG4CXX_INFO(logger_, saved_app.toStyledString());
  Json::Value& app_commands = saved_app[strings::application_commands];
  Json::Value& app_submenus = saved_app[strings::application_submenus];
  Json::Value& app_choise_sets = saved_app[strings::application_choise_sets];
  Json::Value& global_properties = saved_app[strings::application_global_properties];
  Json::Value& subscribtions = saved_app[strings::application_subscribtions];
  Json::Value& application_files = saved_app[strings::application_files];
  uint32_t app_grammar_id = saved_app[strings::grammar_id].asUInt();
  application->set_grammar_id(app_grammar_id);


  // files
  for (Json::Value::iterator json_it = application_files.begin();
      json_it != application_files.end(); ++json_it)  {
    Json::Value& file_data = *json_it;

    bool is_persistent = file_data[strings::persistent_file].asBool();
    if (is_persistent) {
      AppFile file;
      file.is_persistent = is_persistent;
      file.is_download_complete = file_data[strings::is_download_complete].asBool();
      file.file_name = file_data[strings::sync_file_name].asString();
      file.file_type = static_cast<mobile_apis::FileType::eType> (
                         file_data[strings::file_type].asInt());
      LOG4CXX_INFO(logger_, "RestoreApplicationData file " << file.file_name);
      application->AddFile(file);
    }
  }

  //add submenus
  for (Json::Value::iterator json_it = app_submenus.begin();
      json_it != app_submenus.end(); ++json_it)  {
    Json::Value& json_submenu = *json_it;
    smart_objects::SmartObject message = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_submenu, message);
    application->AddSubMenu(message[strings::menu_id].asUInt(), message);
  }
  requests = MessageHelper::CreateAddSubMenuRequestToHMI(application);

  for (MessageHelper::SmartObjectList::iterator it = requests.begin();
       it != requests.end(); ++it) {
    ProcessHMIRequest(*it, true);
  }

  //add commands
  for (Json::Value::iterator json_it = app_commands.begin();
      json_it != app_commands.end(); ++json_it)  {
    Json::Value& json_command = *json_it;
    smart_objects::SmartObject message = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_command, message);
    application->AddCommand(message[strings::cmd_id].asUInt(), message);
  }

  requests = MessageHelper::CreateAddCommandRequestToHMI(application);

  for (MessageHelper::SmartObjectList::iterator it = requests.begin();
       it != requests.end(); ++it) {
    ProcessHMIRequest(*it, true);
  }

  //add choisets
  for (Json::Value::iterator json_it = app_choise_sets.begin();
      json_it != app_choise_sets.end(); ++json_it)  {
    Json::Value& json_choiset = *json_it;
    smart_objects::SmartObject msg_param = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_choiset , msg_param);
    const int32_t choice_set_id = msg_param
        [strings::interaction_choice_set_id].asInt();
    uint32_t choice_grammar_id = msg_param[strings::grammar_id].asUInt();
    application->AddChoiceSet(choice_set_id, msg_param);

    for (size_t j = 0; j < msg_param[strings::choice_set].length(); ++j) {
      smart_objects::SmartObject choise_params = smart_objects::SmartObject(
                                                smart_objects::SmartType_Map);
      choise_params[strings::app_id] = application->app_id();
      choise_params[strings::cmd_id] =
          msg_param[strings::choice_set][j][strings::choice_id];
      choise_params[strings::vr_commands] = smart_objects::SmartObject(
                                           smart_objects::SmartType_Array);
      choise_params[strings::vr_commands] =
          msg_param[strings::choice_set][j][strings::vr_commands];

      choise_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
      choise_params[strings::grammar_id] =  choice_grammar_id;
      SendHMIRequest(hmi_apis::FunctionID::VR_AddCommand, &choise_params);
    }
  }

  //setglobal properties
  if (!global_properties.isNull()) {
    smart_objects::SmartObject properties_so = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(global_properties , properties_so);

    const smart_objects::SmartObject& vr_help_title =
        properties_so.getElement(strings::vr_help_title);
    if (vr_help_title.getType() != smart_objects::SmartType::SmartType_Invalid &&
        vr_help_title.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_vr_help_title(vr_help_title);
    }

    const smart_objects::SmartObject& vr_help =
        properties_so.getElement(strings::vr_help);
    if (vr_help.getType() != smart_objects::SmartType::SmartType_Invalid  &&
        vr_help.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_vr_help(vr_help);
    }

    const smart_objects::SmartObject& timeout_prompt =
        properties_so.getElement(strings::timeout_prompt);
    if (timeout_prompt.getType() != smart_objects::SmartType::SmartType_Invalid  &&
        timeout_prompt.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_timeout_prompt(timeout_prompt);
    }

    const smart_objects::SmartObject& help_prompt =
        properties_so.getElement(strings::help_prompt);
    if (help_prompt.getType() != smart_objects::SmartType::SmartType_Invalid &&
        help_prompt.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_help_prompt(help_prompt);
    }

    const smart_objects::SmartObject& keyboard_properties =
        properties_so.getElement(strings::keyboard_properties);
    if (keyboard_properties.getType() != smart_objects::SmartType::SmartType_Invalid  &&
        keyboard_properties.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_keyboard_props(keyboard_properties);
    }

    const smart_objects::SmartObject& menu_title =
        properties_so.getElement(strings::menu_title);
    if (menu_title.getType() != smart_objects::SmartType::SmartType_Invalid  &&
        menu_title.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_menu_title(menu_title);
    }

    const smart_objects::SmartObject& menu_icon =
        properties_so.getElement(strings::menu_icon);
    if (menu_icon.getType() != smart_objects::SmartType::SmartType_Invalid  &&
        menu_icon.getType() != smart_objects::SmartType::SmartType_Null) {
      application->set_menu_icon(menu_icon);
    }

    MessageHelper::SendGlobalPropertiesToHMI(application);
  }

  //subscribes
  if (!subscribtions.isNull()) {
    Json::Value& subscribtions_buttons = subscribtions[strings::application_buttons];
    Json::Value& subscribtions_ivi= subscribtions[strings::application_vehicle_info];
    for (Json::Value::iterator json_it = subscribtions_buttons.begin();
         json_it != subscribtions_buttons.end(); ++json_it) {
      mobile_apis::ButtonName::eType btn;
      btn = static_cast<mobile_apis::ButtonName::eType>((*json_it).asInt());
      application->SubscribeToButton(btn);
    }

    for (Json::Value::iterator json_it = subscribtions_ivi.begin();
         json_it != subscribtions_ivi.end(); ++json_it) {
      VehicleDataType ivi;
      ivi = static_cast<VehicleDataType>((*json_it).asInt());
      LOG4CXX_INFO(logger_, "VehicleDataType :" <<  ivi);
#ifdef ENABLE_LOG
      bool result =
#endif
      application->SubscribeToIVI(ivi);
      LOG4CXX_INFO(logger_, "result = :" <<  result);
    }
    requests = MessageHelper::GetIVISubscribtionRequests(application->app_id());

    for (MessageHelper::SmartObjectList::iterator it = requests.begin();
         it != requests.end(); ++it) {
      ProcessHMIRequest(*it,true);
    }
  }
  return true;
}

bool ResumeCtrl::IsHMIApplicationIdExist(uint32_t hmi_app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::IsHMIApplicationIdExist " << hmi_app_id);

  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it)[strings::hmi_app_id].asUInt() == hmi_app_id) {
      return true;
    }
  }

  std::set<ApplicationSharedPtr>::iterator it =
                                  app_mngr_->application_list_.begin();
  std::set<ApplicationSharedPtr>::iterator it_end =
                                   app_mngr_->application_list_.end();
  for (;it != it_end; ++it) {
    if (hmi_app_id == (*it)->hmi_app_id()) {
      return true;
    }
  }

  return false;
}

bool ResumeCtrl::IsApplicationSaved(const std::string& mobile_app_id) {
  bool result = false;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it)[strings::app_id].asString() == mobile_app_id) {
      result = true;
    }
  }
  LOG4CXX_INFO(logger_, "IsApplicationSaved " << mobile_app_id << " : " << (result?"true":"false"));
  return result;
}

uint32_t ResumeCtrl::GetHMIApplicationID(const std::string& mobile_app_id) {
  uint32_t hmi_app_id = 0;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it)[strings::app_id].asString() == mobile_app_id) {
      hmi_app_id = (*it)[strings::hmi_app_id].asUInt();
    }
  }

  return hmi_app_id;
}

bool ResumeCtrl::RemoveApplicationFromSaved(ApplicationConstSharedPtr application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::RemoveApplicationFromSaved ");
  DCHECK(application.get());

  bool result = false;
  std::vector<Json::Value> temp;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();

    if (saved_m_app_id != application->mobile_app_id()->asString()) {
      temp.push_back((*it));
    } else {
      result = true;
    }
  }

  if (false == result) {
    return result;
  }

  GetSavedApplications().clear();
  for (std::vector<Json::Value>::iterator it = temp.begin();
      it != temp.end(); ++it) {
    GetSavedApplications().append((*it));
  }
  return result;
}

void ResumeCtrl::IgnitionOff() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::IgnitionOff()");

  Json::Value to_save;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    uint32_t ign_off_count = (*it)[strings::ign_off_count].asUInt();
    if (ign_off_count < kApplicationLifes) {
      ign_off_count++;
      (*it)[strings::ign_off_count] = ign_off_count;
      to_save.append(*it);
    }
  }
  SetSavedApplication(to_save);
}

bool ResumeCtrl::StartResumption(ApplicationSharedPtr application,
                                 uint32_t hash) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::StartResumption");
  if (!application.valid()) {
    LOG4CXX_WARN(logger_, "Application not exist");
    return false;
  }

  LOG4CXX_INFO(logger_, " app_id = " << application->app_id()
                        << " hmi_app_id = " << application->hmi_app_id()
                        << " mobile_id = "
                        << application->mobile_app_id()->asString());

  Json::Value::iterator it = GetSavedApplications().begin();
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  for (; it != GetSavedApplications().end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();

    if (saved_m_app_id == application->mobile_app_id()->asString()) {

      uint32_t saved_hash = (*it)[strings::hash_id].asUInt();
      uint32_t time_stamp= (*it)[strings::time_stamp].asUInt();

      if (hash == saved_hash) {
        RestoreApplicationData(application);
      }

      application->UpdateHash();
      if (!timer_.isRunning() && accessor.applications().size() > 1) {
        RestoreApplicationHMILevel(application);
        RemoveApplicationFromSaved(application);
      } else {
        sync_primitives::AutoLock auto_lock(queue_lock_);
        SetupDefaultHMILevel(application);
        waiting_for_timer_.insert(std::make_pair(application->app_id(),
                                                 time_stamp));
        timer_.start(kTimeStep);
      }
      return true;
    }
  }

  LOG4CXX_INFO(logger_, "ResumeCtrl::Application wasn't saved");
  MessageHelper::SendHMIStatusNotification(*application);
  return false;
}

bool ResumeCtrl::StartResumptionOnlyHMILevel(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::StartResumptionOnlyHMILevel");
  if (!application.valid()) {
    LOG4CXX_WARN(logger_, "Application not exist");
    return false;
  }

  LOG4CXX_INFO(logger_, "app_id = " << application->app_id()
                        << "mobile_id = "
                        << application->mobile_app_id()->asString());

  Json::Value::iterator it = GetSavedApplications().begin();
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  for (; it != GetSavedApplications().end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id == application->mobile_app_id()->asString()) {
      uint32_t time_stamp= (*it)[strings::time_stamp].asUInt();
      if (!timer_.isRunning() && accessor.applications().size() > 1) {
        // resume in case there is already registered app
        RestoreApplicationHMILevel(application);
        RemoveApplicationFromSaved(application);
      } else {
        sync_primitives::AutoLock auto_lock(queue_lock_);
        SetupDefaultHMILevel(application);
        waiting_for_timer_.insert(std::make_pair(application->app_id(),
                                                 time_stamp));
        // woun't start timer if it is active already
        timer_.start(kTimeStep);
      }
      return true;
    }
  }

  LOG4CXX_INFO(logger_, "ResumeCtrl::Application wasn't saved");
  MessageHelper::SendHMIStatusNotification(*application);
  return false;
}

bool ResumeCtrl::CheckPersistenceFilesForResumption(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "CheckPersistenceFilesForResumption");
  DCHECK(application.get());

  Json::Value::iterator it = GetSavedApplications().begin();
  for (; it != GetSavedApplications().end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();

    if (saved_m_app_id == application->mobile_app_id()->asString()) {
      break;
    }
  }

  if (it == GetSavedApplications().end()) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  Json::Value& saved_app = *it;
  MessageHelper::SmartObjectList requests;

  LOG4CXX_INFO(logger_, saved_app.toStyledString());
  Json::Value& app_commands = saved_app[strings::application_commands];
  Json::Value& app_choise_sets = saved_app[strings::application_choise_sets];

  //add commands
  for (Json::Value::iterator json_it = app_commands.begin();
      json_it != app_commands.end(); ++json_it)  {
    Json::Value& json_command = *json_it;
    smart_objects::SmartObject message =
        smart_objects::SmartObject(smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_command, message);
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyImageFiles(message, application);
    if (verification_result == mobile_apis::Result::INVALID_DATA) {
      LOG4CXX_WARN(logger_, "app_commands missed icons");
      return false;
    }
  }

  //add choice sets
  for (Json::Value::iterator json_it = app_choise_sets.begin();
      json_it != app_choise_sets.end(); ++json_it)  {
    Json::Value& json_choiset = *json_it;
    smart_objects::SmartObject msg_param =
        smart_objects::SmartObject(smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_choiset , msg_param);
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyImageFiles(msg_param, application);
    if (verification_result == mobile_apis::Result::INVALID_DATA) {
      LOG4CXX_WARN(logger_, "app_choise_sets missed icons");
      return false;
    }
  }

  return true;
}

bool ResumeCtrl::CheckApplicationHash(ApplicationSharedPtr application,
                                      uint32_t hash) {
  Json::Value::iterator it = GetSavedApplications().begin();
  for (; it != GetSavedApplications().end(); ++it) {
    std::string saved_m_app_id = (*it)[strings::app_id].asString();

    if (saved_m_app_id == application->mobile_app_id()->asString()) {
      uint32_t saved_hash = (*it)[strings::hash_id].asUInt();
      LOG4CXX_INFO(logger_, "received hash = " << hash);
      LOG4CXX_INFO(logger_, "saved hash = " << saved_hash);
      if (hash == saved_hash) {
        return true;
      }
    }
  }

  return false;
}

void ResumeCtrl::onTimer() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::onTimer() size is "
                         << waiting_for_timer_.size());
  sync_primitives::AutoLock auto_lock(queue_lock_);

  std::multiset<application_timestamp, TimeStampComparator>::iterator it=
      waiting_for_timer_.begin();

  for (; it != waiting_for_timer_.end(); ++it) {
    ApplicationSharedPtr app =
        ApplicationManagerImpl::instance()->application((*it).first);
    if (!app.get()) {
      LOG4CXX_ERROR(logger_, "Invalid app_id = " << (*it).first);
      continue;
    }

    RestoreApplicationHMILevel(app);
    RemoveApplicationFromSaved(app);
  }

  waiting_for_timer_.clear();
}

bool ResumeCtrl::IsDeviceMacAddressEqual(ApplicationSharedPtr application,
                                         const std::string& saved_device_mac) {
  const std::string device_mac =
      MessageHelper::GetDeviceMacAddressForHandle(application->device());

  return device_mac == saved_device_mac;
}

Json::Value& ResumeCtrl::GetSavedApplications() {
  return resumption::LastState::instance()->dictionary[strings::resumption];
}

void ResumeCtrl::SetSavedApplication(Json::Value& apps_json) {
  resumption::LastState::instance()->dictionary[strings::resumption] = apps_json ;
}

void ResumeCtrl::ClearResumptionInfo() {
	Json::Value empty_json;

	SetSavedApplication(empty_json);
	resumption::LastState::instance()->SaveToFileSystem();
}

Json::Value ResumeCtrl::GetApplicationCommands(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationCommands "
               << application->app_id());

  Json::Value result;
  const CommandsMap& commands = application->commands_map();
  CommandsMap::const_iterator it = commands.begin();
  for (;it != commands.end(); ++it) {
    smart_objects::SmartObject* so = it->second;
    Json::Value curr;
    Formatters::CFormatterJsonBase::objToJsonValue(*so, curr);
    result.append(curr);
    LOG4CXX_INFO(logger_, "Converted:" << curr.toStyledString());
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationSubMenus(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationCommands "
               << application->app_id());

  Json::Value result;
  const SubMenuMap& sub_menus = application->sub_menu_map();
  SubMenuMap::const_iterator it = sub_menus.begin();
  for (;it != sub_menus.end(); ++it) {
    smart_objects::SmartObject* so = it->second;
    Json::Value curr;
    Formatters::CFormatterJsonBase::objToJsonValue(*so, curr);
    result.append(curr);
    LOG4CXX_INFO(logger_, "Converted:" << curr.toStyledString());
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationInteractionChoiseSets(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationInteractionChoiseSets "
               << application->app_id());

  Json::Value result;
  const ChoiceSetMap& choices = application->choice_set_map();
  ChoiceSetMap::const_iterator it = choices.begin();
  for ( ;it != choices.end(); ++it) {
    smart_objects::SmartObject* so = it->second;
    Json::Value curr;
    Formatters::CFormatterJsonBase::objToJsonValue(*so, curr);
    result.append(curr);
    LOG4CXX_INFO(logger_, "Converted:" << curr.toStyledString());
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationGlobalProperties(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationGlobalProperties "
               << application->app_id());

  Json::Value result;
  const smart_objects::SmartObject* help_promt = application->help_prompt();
  const smart_objects::SmartObject* timeout_prompt = application->timeout_prompt();
  const smart_objects::SmartObject* vr_help = application->vr_help();
  const smart_objects::SmartObject* vr_help_title = application->vr_help_title();
  const smart_objects::SmartObject* vr_synonyms = application->vr_synonyms();
  const smart_objects::SmartObject* keyboard_props = application->keyboard_props();
  const smart_objects::SmartObject* menu_title = application->menu_title();
  const smart_objects::SmartObject* menu_icon = application->menu_icon();

  Json::Value sgp;
  sgp[strings::help_prompt] = JsonFromSO(help_promt);
  sgp[strings::timeout_prompt] = JsonFromSO(timeout_prompt);
  sgp[strings::vr_help] = JsonFromSO(vr_help);
  sgp[strings::vr_help_title] = JsonFromSO(vr_help_title);
  sgp[strings::vr_synonyms] = JsonFromSO(vr_synonyms);
  sgp[strings::keyboard_properties] = JsonFromSO(keyboard_props);
  sgp[strings::menu_title] = JsonFromSO(menu_title);
  sgp[strings::menu_icon] = JsonFromSO(menu_icon);

  return sgp;
}

Json::Value ResumeCtrl::GetApplicationSubscriptions(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationSubscriptions "
               << application->app_id());

  Json::Value result;
  std::set<mobile_apis::ButtonName::eType>::iterator it_button ;
  std::set<uint32_t>::iterator it_vehicle;

  for (it_button = application->SubscribedButtons().begin() ;
       it_button != application->SubscribedButtons().end(); ++it_button) {
    result[strings::application_buttons].append(*it_button);
  }
  for (it_vehicle = application->SubscribesIVI().begin();
       it_vehicle != application->SubscribesIVI().end(); ++it_vehicle) {
    result[strings::application_vehicle_info].append(*it_vehicle);
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationFiles(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationFiles "
               << application->app_id());

  Json::Value result;
  const AppFilesMap& app_files = application->getAppFiles();
  for(AppFilesMap::const_iterator file_it = app_files.begin();
      file_it != app_files.end(); file_it++) {
    const AppFile& file = file_it->second;
    if (file.is_persistent) {
      Json::Value file_data;
      file_data[strings::persistent_file] = file.is_persistent;
      file_data[strings::is_download_complete] = file.is_download_complete;
      file_data[strings::sync_file_name] = file.file_name;
      file_data[strings::file_type] = file.file_type;
      result.append(file_data);
    }
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationShow(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationShow "
                 << application->app_id());

  Json::Value result;
  const smart_objects::SmartObject* show_so = application->show_command();
  if (!show_so) {
    return result;
  }
  result = JsonFromSO(show_so);
  return result;
}

Json::Value ResumeCtrl::JsonFromSO(const smart_objects::SmartObject *so) {
  Json::Value temp;
  if (so) {
    Formatters::CFormatterJsonBase::objToJsonValue(*so, temp);
  }
  return temp;
}

bool ResumeCtrl::ProcessHMIRequest(smart_objects::SmartObject* request,
                                   bool use_events) {
  if (use_events) {
    const hmi_apis::FunctionID::eType function_id =
        static_cast<hmi_apis::FunctionID::eType>(
            (*request)[strings::function_id].asInt());

    const int32_t hmi_correlation_id =
        (*request)[strings::correlation_id].asInt();
    subscribe_on_event(function_id, hmi_correlation_id);
  }
  if (!ApplicationManagerImpl::instance()->ManageHMICommand(request)) {
    LOG4CXX_ERROR(logger_, "Unable to send request");
    return true;
  }
  return false;
}

void ResumeCtrl::SendHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const smart_objects::SmartObject* msg_params, bool use_events) {

  NsSmartDeviceLink::NsSmartObjects::SmartObject* result =
      MessageHelper::CreateModuleInfoSO(function_id);
  int32_t hmi_correlation_id =
      (*result)[strings::params][strings::correlation_id].asInt();
  if (use_events) {
    subscribe_on_event(function_id, hmi_correlation_id);
  }

  if (msg_params) {
    (*result)[strings::msg_params] = *msg_params;
  }

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(result)) {
    LOG4CXX_ERROR(logger_, "Unable to send request");
  }
}

}  // namespace application_manager
