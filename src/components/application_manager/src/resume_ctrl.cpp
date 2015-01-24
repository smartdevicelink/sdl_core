#include <fstream>
#include <algorithm>

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
    restore_hmi_level_timer_("RsmCtrlRstore",
                             this, &ResumeCtrl::ApplicationResumptiOnTimer),
    save_persistent_data_timer_("RsmCtrlPercist",
                                this, &ResumeCtrl::SaveDataOnTimer, true),
    is_data_saved(true) {
  save_persistent_data_timer_.start(profile::Profile::instance()->app_resumption_save_persistent_data_timeout());
}

void ResumeCtrl::SaveAllApplications() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(app_mngr_);
  if (app_mngr_) {
    ApplicationManagerImpl::ApplicationListAccessor accessor;
    ApplicationManagerImpl::TAppList apps(accessor.applications());
    std::for_each(apps.begin(),
                  apps.end(),
                  std::bind1st(std::mem_fun(&ResumeCtrl::SaveApplication), this));
  } else {
    LOG4CXX_FATAL(logger_, "Application manager object is NULL.");
  }
}

void ResumeCtrl::SaveApplication(ApplicationConstSharedPtr application) {
  DCHECK(application.get());

  if (!application) {
    LOG4CXX_FATAL(logger_, "Application object is NULL.");
    return;
  }

  const std::string& m_app_id = application->mobile_app_id()->asString();
  LOG4CXX_TRACE(logger_, "ENTER app_id : " << application->app_id()
                << " mobile app_id : " << m_app_id);

  const uint32_t hash = application->curHash();
  const uint32_t grammar_id = application->get_grammar_id();

  LOG4CXX_INFO(logger_, "hash = " << hash);
  LOG4CXX_INFO(logger_, "grammar_id = " << grammar_id);

  Json::Value& json_app = GetFromSavedOrAppend(m_app_id);

  json_app[strings::device_mac] =
    MessageHelper::GetDeviceMacAddressForHandle(application->device());
  json_app[strings::app_id] = m_app_id;
  json_app[strings::grammar_id] = grammar_id;
  json_app[strings::connection_key] = application->app_id();
  json_app[strings::hmi_app_id] = application->hmi_app_id();
  json_app[strings::hmi_level] =
    static_cast<int32_t> (application->hmi_level());
  json_app[strings::ign_off_count] = 0;
  json_app[strings::hash_id] = hash;
  json_app[strings::application_commands] =
    GetApplicationCommands(application);
  json_app[strings::application_submenus] =
    GetApplicationSubMenus(application);
  json_app[strings::application_choise_sets] =
    GetApplicationInteractionChoiseSets(application);
  json_app[strings::application_global_properties] =
    GetApplicationGlobalProperties(application);
  json_app[strings::application_subscribtions] =
    GetApplicationSubscriptions(application);
  json_app[strings::application_files] = GetApplicationFiles(application);
  json_app[strings::time_stamp] = (uint32_t)time(NULL);
  json_app[strings::audio_streaming_state] = application->audio_streaming_state();
  LOG4CXX_DEBUG(logger_, "SaveApplication : " << json_app.toStyledString());
}

void ResumeCtrl::on_event(const event_engine::Event& event) {
  LOG4CXX_TRACE(logger_, "Response from HMI command");
}

bool ResumeCtrl::RestoreApplicationHMILevel(ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  if (!application) {
    LOG4CXX_ERROR(logger_, " RestoreApplicationHMILevel() application pointer in invalid");
    return false;
  }
  LOG4CXX_DEBUG(logger_, "ENTER app_id : " << application->app_id());

  const int idx = GetObjectIndex(application->mobile_app_id()->asString());
  if (-1 != idx) {
    const Json::Value& json_app = GetSavedApplications()[idx];
    if (json_app.isMember(strings::audio_streaming_state) &&
        json_app.isMember(strings::hmi_level)) {

      const AudioStreamingState::eType audio_streaming_state =
          static_cast<mobile_apis::AudioStreamingState::eType>(
            json_app[strings::audio_streaming_state].asInt());

      application->set_audio_streaming_state(audio_streaming_state);

      const HMILevel::eType saved_hmi_level =
          static_cast<mobile_apis::HMILevel::eType>(
            json_app[strings::hmi_level].asInt());
      LOG4CXX_DEBUG(logger_, "Saved HMI Level is : " << saved_hmi_level);
      return SetupHMILevel(application, saved_hmi_level, audio_streaming_state);
    } else {
      LOG4CXX_FATAL(logger_, "There are some unknown keys among the stored apps");
    }
  }
  LOG4CXX_INFO(logger_, "Failed to restore application HMILevel");
  return false;
}

bool ResumeCtrl::SetupDefaultHMILevel(ApplicationSharedPtr application) {
  if (false == application.valid()) {
    LOG4CXX_ERROR(logger_, "SetupDefaultHMILevel application pointer is invalid");
    return false;
  }
  LOG4CXX_TRACE(logger_, "ENTER app_id : " << application->app_id());
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
  bool result = SetupHMILevel(application, default_hmi,
                         mobile_apis::AudioStreamingState::NOT_AUDIBLE, false);
  return result;
}

bool ResumeCtrl::SetupHMILevel(ApplicationSharedPtr application,
                               mobile_apis::HMILevel::eType hmi_level,
                               mobile_apis::AudioStreamingState::eType audio_streaming_state,
                               bool check_policy) {
  if (false == application.valid()) {
    LOG4CXX_ERROR(logger_, "Application pointer in invalid");
    return false;
  }
  LOG4CXX_TRACE(logger_, " ENTER Params : ( " << application->app_id()
                << "," << hmi_level
                << "," << audio_streaming_state
                << "," << check_policy << " )");
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
    MessageHelper::SendHMIStatusNotification(*(application.get()));
    // HMI status for full wil be get after ActivateApp response
  }
  LOG4CXX_INFO(logger_, "Set up application "
               << application->mobile_app_id()->asString()
               << " to HMILevel " << hmi_level);
  return true;
}

bool ResumeCtrl::RestoreApplicationData(ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!application.valid()) {
    LOG4CXX_ERROR(logger_, "Application pointer in invalid");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "ENTER app_id : " << application->app_id());


  const int idx = GetObjectIndex(application->mobile_app_id()->asString());
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  const Json::Value& saved_app = GetSavedApplications()[idx];
  if(saved_app.isMember(strings::grammar_id)) {
    const uint32_t app_grammar_id = saved_app[strings::grammar_id].asUInt();
    application->set_grammar_id(app_grammar_id);

    AddFiles(application, saved_app);
    AddSubmenues(application, saved_app);
    AddCommands(application, saved_app);
    AddChoicesets(application, saved_app);
    SetGlobalProperties(application, saved_app);
    AddSubscriptions(application, saved_app);
  }
  return true;
}

bool ResumeCtrl::IsHMIApplicationIdExist(uint32_t hmi_app_id) {
  LOG4CXX_TRACE(logger_, "ENTER hmi_app_id :"  << hmi_app_id);

  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it).isMember(strings::hmi_app_id)) {
      if ((*it)[strings::hmi_app_id].asUInt() == hmi_app_id) {
        return true;
      }
    }
  }
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  std::set<ApplicationSharedPtr>::iterator it = accessor.applications().begin();
  std::set<ApplicationSharedPtr>::iterator it_end = accessor.applications().end();

  for (;it != it_end; ++it) {
    if (hmi_app_id == (*it)->hmi_app_id()) {
      LOG4CXX_TRACE(logger_, "EXIT result = true");
      return true;
    }
  }
  LOG4CXX_TRACE(logger_, "EXIT result = false");
  return false;
}

bool ResumeCtrl::IsApplicationSaved(const std::string& mobile_app_id) {
  LOG4CXX_TRACE(logger_, "ENTER mobile_app_id :"  << mobile_app_id);
  bool result = false;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it).isMember(strings::app_id)) {
      if ((*it)[strings::app_id].asString() == mobile_app_id) {
        result = true;
      }
    }
  }
  LOG4CXX_TRACE(logger_, "EXIT mobile_app_id :" <<
                mobile_app_id <<
                " result: " <<
                (result ? "true" : "false"));
  return result;
}

uint32_t ResumeCtrl::GetHMIApplicationID(const std::string& mobile_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t hmi_app_id = 0;

  const int idx = GetObjectIndex(mobile_app_id);
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return hmi_app_id;
  }

  const Json::Value& json_app = GetSavedApplications()[idx];
  if (json_app.isMember(strings::app_id)) {
      hmi_app_id = json_app[strings::hmi_app_id].asUInt();
  }
  LOG4CXX_DEBUG(logger_, "hmi_app_id :"  << hmi_app_id);
  return hmi_app_id;
}

bool ResumeCtrl::RemoveApplicationFromSaved(ApplicationConstSharedPtr application) {
  if (false == application.valid()) {
    LOG4CXX_ERROR(logger_, "Application pointer in invalid");
    return false;
  }
  LOG4CXX_TRACE(logger_, "ENTER app_id :"  << application->app_id()
                << "; mobile_app_id " << application->mobile_app_id()->asString());

  bool result = false;
  std::vector<Json::Value> temp;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it).isMember(strings::app_id)) {
      const std::string& saved_m_app_id = (*it)[strings::app_id].asString();

      if (saved_m_app_id != application->mobile_app_id()->asString()) {
        temp.push_back((*it));
      } else {
        result = true;
      }
    }
  }

  if (false == result) {
    LOG4CXX_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
    return result;
  }

  GetSavedApplications().clear();
  for (std::vector<Json::Value>::iterator it = temp.begin();
      it != temp.end(); ++it) {
    GetSavedApplications().append((*it));
  }
  LOG4CXX_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
  return result;
}

void ResumeCtrl::IgnitionOff() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value to_save;
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if ((*it).isMember(strings::ign_off_count)) {
      const uint32_t ign_off_count = (*it)[strings::ign_off_count].asUInt();
      if (ign_off_count < kApplicationLifes) {
        (*it)[strings::ign_off_count] = ign_off_count + 1;
        to_save.append(*it);
      }
    } else {
      LOG4CXX_FATAL(logger_, "Unknown key among saved applications");
      return;
    }
  }
  SetSavedApplication(to_save);
}


bool ResumeCtrl::StartResumption(ApplicationSharedPtr application,
                                 uint32_t hash) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!application) {
    LOG4CXX_WARN(logger_, "Application not exist");
    return false;
  }
  LOG4CXX_DEBUG(logger_, " Resume app_id = " << application->app_id()
                        << " hmi_app_id = " << application->hmi_app_id()
                        << " mobile_id = " << application->mobile_app_id()->asString()
                        << "recieved hash = " << hash);

  const int idx = GetObjectIndex(application->mobile_app_id()->asString());
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    MessageHelper::SendHMIStatusNotification(*application);
    return false;
  }

  const Json::Value& json_app = GetSavedApplications()[idx];
  if (json_app.isMember(strings::hash_id) && json_app.isMember(strings::time_stamp)) {
    const uint32_t saved_hash = json_app[strings::hash_id].asUInt();

    if (saved_hash == hash) {
      RestoreApplicationData(application);
    }
    application->UpdateHash();
    RestoreHmiLevel(json_app[strings::time_stamp].asUInt(), application);
    return true;
  } else {
    LOG4CXX_INFO(logger_, "There are some unknown keys in the dictionary.");
  }

  return false;
}

void ResumeCtrl::RestoreHmiLevel(uint32_t time_stamp,
                                 ApplicationSharedPtr application) {
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  if (!restore_hmi_level_timer_.isRunning() &&
      accessor.applications().size() > 1) {
    // resume in case there is already registered app
    RestoreApplicationHMILevel(application);
    RemoveApplicationFromSaved(application);
  } else {
    // please avoid AutoLock usage to avoid deadlock
    SetupDefaultHMILevel(application);
    InsertToTimerQueue(application->app_id(), time_stamp);
    // woun't start timer if it is active already
    LOG4CXX_DEBUG(logger_, "Application " << application->app_id() << " inserted to timer queue. "
                  << "timer started for " << profile::Profile::instance()->app_resuming_timeout());
    restore_hmi_level_timer_.start(profile::Profile::instance()->app_resuming_timeout());
  }
}

bool ResumeCtrl::StartResumptionOnlyHMILevel(ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!application.valid()) {
    LOG4CXX_WARN(logger_, "Application do not exists");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "ENTER app_id = " << application->app_id()
                        << "mobile_id = "
                        << application->mobile_app_id()->asString());

  const int idx = GetObjectIndex(application->mobile_app_id()->asString());
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    MessageHelper::SendHMIStatusNotification(*application);
    return false;
  }

  const Json::Value& json = GetSavedApplications()[idx];
  if (json.isMember(strings::time_stamp)) {
    const uint32_t time_stamp = json[strings::time_stamp].asUInt();
    RestoreHmiLevel(time_stamp, application);
    return true;
  } else {
    LOG4CXX_FATAL(logger_, "The key " << strings::time_stamp <<
                  " doesn't exists among saved apps.");
  }

  LOG4CXX_INFO(logger_, "ResumeCtrl::Application wasn't saved");
  MessageHelper::SendHMIStatusNotification(*application);
  LOG4CXX_TRACE(logger_, "EXIT false");
  return false;
}

bool ResumeCtrl::CheckPersistenceFilesForResumption(ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!application.valid()) {
    LOG4CXX_WARN(logger_, "Application do not exists");
    return false;
  }
  LOG4CXX_DEBUG(logger_, "Process app_id = " << application->app_id());

  const int idx = GetObjectIndex(application->mobile_app_id()->asString());
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  const Json::Value& saved_app = GetSavedApplications()[idx];

  if (!saved_app.isMember(strings::application_commands) ||
      !saved_app.isMember(strings::application_choise_sets)) {
    return false;
  }

  if (!CheckIcons(application, saved_app[strings::application_commands])) {
    return false;
  }
  if (!CheckIcons(application, saved_app[strings::application_choise_sets])) {
    return false;
  }

  return true;
}

bool ResumeCtrl::CheckApplicationHash(ApplicationSharedPtr application,
                                      uint32_t hash) {
  if (!application) {
    LOG4CXX_ERROR(logger_, "Application pointer is invalid");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "ENTER app_id : " << application->app_id()
                << " hash : " << hash);

  const int idx = GetObjectIndex(application->mobile_app_id()->asString());
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  const Json::Value& json_app = GetSavedApplications()[idx];

  if (json_app.isMember(strings::hash_id)) {
    const uint32_t saved_hash = json_app[strings::hash_id].asUInt();

    LOG4CXX_TRACE(logger_, "Found saved application : " << json_app.toStyledString());
    LOG4CXX_INFO(logger_, "received hash = " << hash);
    LOG4CXX_INFO(logger_, "saved hash = " << saved_hash);
    if (hash == saved_hash) {
      return true;
    }
  }

  return false;
}

void ResumeCtrl::ApplicationResumptiOnTimer() {
  LOG4CXX_TRACE(logger_, "ENTER waiting for resumption HMI_Level count is :"
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
  LOG4CXX_TRACE(logger_, "EXIT");
}

void ResumeCtrl::SaveDataOnTimer() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (waiting_for_timer_.size() > 0) {
    LOG4CXX_INFO(logger_, "There are some applications, that are waiting for resumption HMILevel. Data should not be saved");
    return;
  }
  if (false == is_data_saved) {
    SaveAllApplications();
    is_data_saved = true;
    resumption::LastState::instance()->SaveToFileSystem();
  }
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
  LOG4CXX_AUTO_TRACE(logger_);
  Json::Value empty_json;

  SetSavedApplication(empty_json);
  resumption::LastState::instance()->SaveToFileSystem();
}

Json::Value ResumeCtrl::GetApplicationCommands(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
               << application->app_id());

  Json::Value result;
  const DataAccessor<CommandsMap> accessor = application->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();
  for (;it != commands.end(); ++it) {
    smart_objects::SmartObject* so = it->second;
    Json::Value curr;
    Formatters::CFormatterJsonBase::objToJsonValue(*so, curr);
    result.append(curr);
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationSubMenus(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
               << application->app_id());

  Json::Value result;
  const DataAccessor<SubMenuMap> accessor = application->sub_menu_map();
  const SubMenuMap& sub_menus = accessor.GetData();
  SubMenuMap::const_iterator it = sub_menus.begin();
  for (;it != sub_menus.end(); ++it) {
    smart_objects::SmartObject* so = it->second;
    Json::Value curr;
    Formatters::CFormatterJsonBase::objToJsonValue(*so, curr);
    result.append(curr);
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationInteractionChoiseSets(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
               << application->app_id());

  Json::Value result;
  const DataAccessor<ChoiceSetMap> accessor = application->choice_set_map();
  const ChoiceSetMap& choices = accessor.GetData();
  ChoiceSetMap::const_iterator it = choices.begin();
  for ( ;it != choices.end(); ++it) {
    smart_objects::SmartObject* so = it->second;
    Json::Value curr;
    Formatters::CFormatterJsonBase::objToJsonValue(*so, curr);
    result.append(curr);
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationGlobalProperties(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
               << application->app_id());

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
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(application.get());
  Json::Value result;
  if (!application) {
    LOG4CXX_DEBUG(logger_, "ENTER app_id:" << application->app_id());

    Append(application->SubscribedButtons().begin(),
                        application->SubscribedButtons().end(),
                        strings::application_buttons, result);

    Append(application->SubscribedButtons().begin(),
                        application->SubscribedButtons().end(),
                        strings::application_vehicle_info, result);
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationFiles(
    ApplicationConstSharedPtr application) {
  DCHECK(application.get());
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
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
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
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
  LOG4CXX_AUTO_TRACE(logger_);
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

void ResumeCtrl::InsertToTimerQueue(uint32_t app_id, uint32_t time_stamp) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(queue_lock_);
  LOG4CXX_DEBUG(logger_,"After queue_lock_ Accure");
  waiting_for_timer_.insert(std::make_pair(app_id, time_stamp));
}

void ResumeCtrl::AddFiles(ApplicationSharedPtr application, const Json::Value& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (saved_app.isMember(strings::application_files)) {
    const Json::Value& application_files = saved_app[strings::application_files];
    for (Json::Value::iterator json_it = application_files.begin();
        json_it != application_files.end(); ++json_it)  {
      const Json::Value& file_data = *json_it;

      const bool is_persistent = file_data.isMember(strings::persistent_file) &&
          file_data[strings::persistent_file].asBool();
      if (is_persistent) {
        AppFile file;
        file.is_persistent = is_persistent;
        file.is_download_complete = file_data[strings::is_download_complete].asBool();
        file.file_name = file_data[strings::sync_file_name].asString();
        file.file_type = static_cast<mobile_apis::FileType::eType> (
                           file_data[strings::file_type].asInt());
        application->AddFile(file);
      }
    }
  } else {
    LOG4CXX_FATAL(logger_, "application_files section is not exists");
  }
}

void ResumeCtrl::AddSubmenues(ApplicationSharedPtr application, const Json::Value& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (saved_app.isMember(strings::application_submenus)) {
    const Json::Value& app_submenus = saved_app[strings::application_submenus];
    for (Json::Value::iterator json_it = app_submenus.begin();
        json_it != app_submenus.end(); ++json_it) {
      const Json::Value& json_submenu = *json_it;
      smart_objects::SmartObject message(smart_objects::SmartType::SmartType_Map);
      Formatters::CFormatterJsonBase::jsonValueToObj(json_submenu, message);
      application->AddSubMenu(message[strings::menu_id].asUInt(), message);
    }

    ProcessHMIRequests(MessageHelper::CreateAddSubMenuRequestToHMI(application));
  } else {
    LOG4CXX_FATAL(logger_, "application_submenus section is not exists");
  }
}

void ResumeCtrl::AddCommands(ApplicationSharedPtr application, const Json::Value& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (saved_app.isMember(strings::application_commands)) {
    const Json::Value& app_commands = saved_app[strings::application_commands];
    for (Json::Value::iterator json_it = app_commands.begin();
        json_it != app_commands.end(); ++json_it)  {
      const Json::Value& json_command = *json_it;
      smart_objects::SmartObject message(smart_objects::SmartType::SmartType_Map);
      Formatters::CFormatterJsonBase::jsonValueToObj(json_command, message);
      application->AddCommand(message[strings::cmd_id].asUInt(), message);
    }

    ProcessHMIRequests(MessageHelper::CreateAddCommandRequestToHMI(application));
  } else {
     LOG4CXX_FATAL(logger_, "application_commands section is not exists");
   }
}

void ResumeCtrl::AddChoicesets(ApplicationSharedPtr application, const Json::Value& saved_app) {
  if(saved_app.isMember(strings::application_choise_sets)) {
    const Json::Value& app_choise_sets = saved_app[strings::application_choise_sets];
    for (Json::Value::iterator json_it = app_choise_sets.begin();
        json_it != app_choise_sets.end(); ++json_it)  {
      const Json::Value& json_choiset = *json_it;
      smart_objects::SmartObject msg_param(smart_objects::SmartType::SmartType_Map);
      Formatters::CFormatterJsonBase::jsonValueToObj(json_choiset , msg_param);
      const int32_t choice_set_id = msg_param
          [strings::interaction_choice_set_id].asInt();
      uint32_t choice_grammar_id = msg_param[strings::grammar_id].asUInt();
      application->AddChoiceSet(choice_set_id, msg_param);

      const size_t size = msg_param[strings::choice_set].length();
      for (size_t j = 0; j < size; ++j) {
        smart_objects::SmartObject choise_params(smart_objects::SmartType_Map);
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
  } else {
    LOG4CXX_FATAL(logger_, "There is no any choicesets");
  }
}

void ResumeCtrl::SetGlobalProperties(ApplicationSharedPtr application, const Json::Value& saved_app) {
  const Json::Value& global_properties = saved_app[strings::application_global_properties];
  if (!global_properties.isNull()) {
    smart_objects::SmartObject properties_so(smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(global_properties , properties_so);
    application->load_global_properties(properties_so);
    MessageHelper::SendGlobalPropertiesToHMI(application);
  }
}

void ResumeCtrl::AddSubscriptions(ApplicationSharedPtr application, const Json::Value& saved_app) {
  if (saved_app.isMember(strings::application_subscribtions)) {
    const Json::Value& subscribtions = saved_app[strings::application_subscribtions];

    if (subscribtions.isMember(strings::application_buttons)) {
      const Json::Value& subscribtions_buttons = subscribtions[strings::application_buttons];
      mobile_apis::ButtonName::eType btn;
      for (Json::Value::iterator json_it = subscribtions_buttons.begin();
           json_it != subscribtions_buttons.end(); ++json_it) {
        btn = static_cast<mobile_apis::ButtonName::eType>((*json_it).asInt());
        application->SubscribeToButton(btn);
      }
    }
    if (subscribtions.isMember(strings::application_vehicle_info)) {
      const Json::Value& subscribtions_ivi= subscribtions[strings::application_vehicle_info];
      VehicleDataType ivi;
      for (Json::Value::iterator json_it = subscribtions_ivi.begin();
           json_it != subscribtions_ivi.end(); ++json_it) {
        ivi = static_cast<VehicleDataType>((*json_it).asInt());
        application->SubscribeToIVI(ivi);
      }
    }

    ProcessHMIRequests(MessageHelper::GetIVISubscriptionRequests(application));
  }
}

void ResumeCtrl::ProcessHMIRequests(const MessageHelper::SmartObjectList& requests) {
  for (MessageHelper::SmartObjectList::const_iterator it = requests.begin(),
       total = requests.end();
       it != total; ++it) {
    ProcessHMIRequest(*it, true);
  }
}

bool ResumeCtrl::CheckIcons(ApplicationSharedPtr application,
                            const Json::Value& json_object) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool result = true;
  if (!json_object.isNull()) {
    Json::Value::const_iterator json_it = json_object.begin();
    for (;json_it != json_object.end() && result; ++json_it) {
      const Json::Value& json_command = *json_it;
      if (!json_command.isNull()) {
        smart_objects::SmartObject message(smart_objects::SmartType::SmartType_Map);
        Formatters::CFormatterJsonBase::jsonValueToObj(json_command, message);

        result = (mobile_apis::Result::INVALID_DATA ==
            MessageHelper::VerifyImageFiles(message, application));
      } else {
        LOG4CXX_WARN(logger_, "Invalid json object");
      }
    }
  } else {
        LOG4CXX_WARN(logger_, "Passed json object is null");
  }
  return result;
}

Json::Value& ResumeCtrl::GetFromSavedOrAppend(const std::string& mobile_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  for (Json::Value::iterator it = GetSavedApplications().begin();
      it != GetSavedApplications().end(); ++it) {
    if (mobile_app_id == (*it)[strings::app_id].asString()) {
      return *it;
    }
  }

  return GetSavedApplications().append(Json::Value());
}

int ResumeCtrl::GetObjectIndex(const std::string& mobile_app_id) {

  const Json::Value& apps = GetSavedApplications();

  const Json::ArrayIndex size = apps.size();
  Json::ArrayIndex idx = apps.size();
  for (; idx != size; ++idx) {
    if (mobile_app_id == apps[idx][strings::app_id].asString()) {
      return idx;
    }
  }
  return -1;
}

void ResumeCtrl::SendHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const smart_objects::SmartObject* msg_params, bool use_events) {
  LOG4CXX_AUTO_TRACE(logger_);
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
