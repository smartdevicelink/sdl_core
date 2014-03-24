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

namespace application_manager {
  log4cxx::LoggerPtr ResumeCtrl::logger_ = log4cxx::LoggerPtr(
        log4cxx::Logger::getLogger("ResumeCtrl"));
namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

ResumeCtrl::ResumeCtrl(ApplicationManagerImpl* app_mngr)
  : app_mngr_(app_mngr),
    timer_(this, &ResumeCtrl::onTimer) {
}

void ResumeCtrl::SaveAllApplications() {
  LOG4CXX_INFO(logger_, " ResumeCtrl::SaveApplications()");
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
  LOG4CXX_INFO(logger_, " ResumeCtrl::SaveApplication");
  DCHECK(application.get());

  Json::Value* json_app = NULL;
  const std::string& m_app_id = application->mobile_app_id()->asString();
  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
            it != saved_applications_.end(); ++it) {
    if ((*it)[strings::app_id].asString() == m_app_id) {
      json_app = &(*it);
      LOG4CXX_INFO(logger_, "ResumeCtrl Application with this id"
                            "is already exist ( update info ) " << m_app_id);
      break;
    }
  }

  if (json_app == NULL) {
    LOG4CXX_INFO(logger_, "ResumeCtrl Application with this ID does not"
         "exist. Add new" << m_app_id);
    saved_applications_.push_back(Json::Value());
    json_app = &(saved_applications_.back());
  }
  uint32_t hash = application->curHash();
  LOG4CXX_INFO(logger_, " Hash = " << hash);
  uint32_t connection_key = application->app_id();
  (*json_app)[strings::app_id] = m_app_id;
  (*json_app)[strings::connection_key] = connection_key;
  (*json_app)[strings::hmi_level] =
      static_cast<int32_t> (application->hmi_level());
  (*json_app)[strings::ign_off_count] = 0;
  (*json_app)[strings::hash_id] = hash;
  (*json_app)[strings::application_commands] =
      GetApplicationCommands(connection_key);
  (*json_app)[strings::application_submenus] =
      GetApplicationSubMenus(connection_key);
  (*json_app)[strings::application_choise_sets] =
      GetApplicationInteractionChoiseSets(connection_key);
  (*json_app)[strings::application_global_properties] =
      GetApplicationGlobalProperties(connection_key);
  (*json_app)[strings::application_subscribtions] =
      GetApplicationSubscriptions(connection_key);
  (*json_app)[strings::application_files] = GetApplicationFiles(connection_key);
  (*json_app)[strings::time_stamp] = (uint32_t)time(NULL);
  (*json_app)[strings::audio_streaming_state] = application->audio_streaming_state();
}

void ResumeCtrl::LoadApplications() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::LoadApplications");
  DCHECK(app_mngr_);

  const std::string& storage =
    profile::Profile::instance()->app_info_storage();
  Json::Value root;
  Json::Reader m_reader;
  std::ifstream file(file_system::FullPath(storage).c_str(),
                     std::ios::in);
  if (!file.is_open()) {
    LOG4CXX_ERROR(logger_, "Error while opening file");
    return;
  }

  m_reader.parse(file, root);
  if (root.isNull()) {
    LOG4CXX_INFO(logger_, "There are no Saved applications");
    return;
  }

  for (Json::Value::iterator it = root.begin(); it != root.end(); ++it) {
    Json::Value cur_app_data = (*it);
    LOG4CXX_INFO(logger_, "Restored Application: " << 
						  cur_app_data.toStyledString());
    saved_applications_.push_back(cur_app_data);
    uint32_t session_key = cur_app_data[strings::connection_key].asUInt();
  }
}

void ResumeCtrl::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::on_event ");
}

bool ResumeCtrl::RestoreApplicationHMILevel(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::RestoreApplicationHMILevel");
  DCHECK(application.get());

  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
      it != saved_applications_.end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id ==
        application->mobile_app_id()->asString()) {

      mobile_apis::HMILevel::eType saved_hmi_level;
      mobile_apis::HMILevel::eType restored_hmi_level;

      mobile_apis::AudioStreamingState::eType audio_streaming_state =
          static_cast<mobile_apis::AudioStreamingState::eType>
      ((*it)[strings::audio_streaming_state].asInt());
        application->set_audio_streaming_state(audio_streaming_state);
      saved_hmi_level = static_cast<mobile_apis::HMILevel::eType>(
                            (*it)[strings::hmi_level].asInt());

      if (saved_hmi_level == mobile_apis::HMILevel::HMI_FULL) {
        restored_hmi_level = app_mngr_->PutApplicationInFull(application);
      } else if (saved_hmi_level == mobile_apis::HMILevel::HMI_LIMITED) {
        restored_hmi_level = app_mngr_->PutApplicationInLimited(application);
      } else {
        restored_hmi_level = saved_hmi_level;
      }
      application->set_hmi_level(restored_hmi_level);
      MessageHelper::SendHMIStatusNotification(*(application.get()));
      LOG4CXX_INFO(logger_, "Restore Application "
                   << saved_m_app_id
                   << " to HMILevel " << restored_hmi_level);
      return true;
    }
  }

  return false;
}

bool ResumeCtrl::RestoreApplicationData(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "RestoreApplicationData");
  DCHECK(application.get());

  std::vector<Json::Value>::iterator it = saved_applications_.begin();
  for (; it != saved_applications_.end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id ==
        application->mobile_app_id()->asString()) {
      break;
    }
  }
  if (it == saved_applications_.end()) {
    LOG4CXX_WARN(logger_,"Application not saved");
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
  Json::Value& application_files= saved_app[strings::application_files];

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
    application->AddChoiceSet(choice_set_id, msg_param);
  }

  //setglobal properties
  if (!global_properties.isNull()) {
    smart_objects::SmartObject properties_so = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(global_properties , properties_so);

    application->set_vr_help_title(
        properties_so.getElement(strings::vr_help_title));
    application->set_vr_help(
        properties_so.getElement(strings::vr_help));
    application->set_timeout_prompt(
        properties_so.getElement(strings::timeout_prompt));
    application->set_help_prompt(
        properties_so.getElement(strings::help_prompt));
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
      bool result = application->SubscribeToIVI(ivi);
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

bool ResumeCtrl::ApplicationIsSaved(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::ApplicationIsSaved " << app_id);

  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
      it != saved_applications_.end(); ++it) {
    if ((*it)[strings::connection_key].asInt() == app_id) {
      return true;
    }
  }
  return false;
}

bool ResumeCtrl::RemoveApplicationFromSaved(ApplicationConstSharedPtr application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::RemoveApplicationFromSaved ");
  DCHECK(application.get());

  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
      it != saved_applications_.end(); ) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id == application->mobile_app_id()->asString()) {
      saved_applications_.erase(it);
      return true;
    } else {
      ++it;
    }
  }
  return false;
}

void ResumeCtrl::SavetoFileSystem() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::SavetoFileSystem()");

  const std::string& storage =
      profile::Profile::instance()->app_info_storage();
  std::ofstream file(file_system::FullPath(storage).c_str(),
                     std::ios::out);
  Json::Value to_save;
  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
      it != saved_applications_.end(); ++it) {
    uint32_t ign_off_count = (*it)[strings::ign_off_count].asUInt();
    if (ign_off_count < kApplicationLifes) {
      ign_off_count++;
      (*it)[strings::ign_off_count] = ign_off_count;
      to_save.append(*it);
    }
  }

  if (file.is_open()) {
    file << to_save.toStyledString();
  } else {
    LOG4CXX_ERROR(logger_, "An error occurred during save");
  }
  file.close();
}


bool ResumeCtrl::StartResumption(ApplicationSharedPtr application,
                                 uint32_t hash) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::StartResumption");
  DCHECK(application.get());
  LOG4CXX_INFO(logger_, "app_id = " << application->app_id());
  LOG4CXX_INFO(logger_, "mobile_id = " << application->mobile_app_id()->asString());

  std::vector<Json::Value>::const_iterator it = saved_applications_.begin();
  for (; it != saved_applications_.end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id ==
        application->mobile_app_id()->asString()) {

      uint32_t saved_hash = (*it)[strings::hash_id].asUInt();
      uint32_t time_stamp= (*it)[strings::time_stamp].asUInt();

      if (hash == saved_hash) {
        RestoreApplicationData(application);
      }
      application->UpdateHash();
      if ( !timer_.isRunning() && app_mngr_->applications().size() > 1 ) {
        RestoreApplicationHMILevel(application);
        RemoveApplicationFromSaved(application);
      } else {
        sync_primitives::AutoLock auto_lock(queue_lock_);
        MessageHelper::SendHMIStatusNotification(*application);
        waiting_for_timer_.insert(std::make_pair(application->app_id(),
                                                 time_stamp));
        timer_.start(kTimeStep);
      }
      return true;
    }
  }
  LOG4CXX_INFO(logger_, "ResumeCtrl::Applicaton didn't saved");
  MessageHelper::SendHMIStatusNotification(*application);
  return false;
}

bool ResumeCtrl::StartResumptionOnlyHMILevel(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::StartResumptionOnlyHMILevel");
  DCHECK(application.get());

  LOG4CXX_INFO(logger_, "app_id = " << application->app_id());
  LOG4CXX_INFO(logger_, "mobile_id = " << application->mobile_app_id()->asString());

  std::vector<Json::Value>::const_iterator it = saved_applications_.begin();
  for (; it != saved_applications_.end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id ==
        application->mobile_app_id()->asString()) {
      uint32_t time_stamp= (*it)[strings::time_stamp].asUInt();
      if ( !timer_.isRunning() && app_mngr_->applications().size() > 1 ) {
        RestoreApplicationHMILevel(application);
        RemoveApplicationFromSaved(application);
      } else {
        sync_primitives::AutoLock auto_lock(queue_lock_);
        MessageHelper::SendHMIStatusNotification(*application);
        waiting_for_timer_.insert(std::make_pair(application->app_id(),
                                                 time_stamp));
        timer_.start(kTimeStep);
      }
      return true;
    }
  }
  LOG4CXX_INFO(logger_, "ResumeCtrl::Applicaton didn't saved");
  MessageHelper::SendHMIStatusNotification(*application);
  return false;
}

bool ResumeCtrl::CheckPersistenceFilesForResumption(ApplicationSharedPtr application) {
  LOG4CXX_INFO(logger_, "RestoreApplicationData");
  DCHECK(application.get());

  std::vector<Json::Value>::iterator it = saved_applications_.begin();
  for (; it != saved_applications_.end(); ++it) {
    const std::string& saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id ==
        application->mobile_app_id()->asString()) {
      break;
    }
  }
  if (it == saved_applications_.end()) {
    LOG4CXX_WARN(logger_,"Application not saved");
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
    smart_objects::SmartObject message = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_command, message);
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyImageFiles(message, application);
    if (verification_result == mobile_apis::Result::INVALID_DATA) {
      LOG4CXX_WARN(logger_,"app_commands missed icons");
      return false;
    }
  }

  //add choisets
  for (Json::Value::iterator json_it = app_choise_sets.begin();
      json_it != app_choise_sets.end(); ++json_it)  {
    Json::Value& json_choiset = *json_it;
    smart_objects::SmartObject msg_param = smart_objects::SmartObject(
                                         smart_objects::SmartType::SmartType_Map);
    Formatters::CFormatterJsonBase::jsonValueToObj(json_choiset , msg_param);
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyImageFiles(msg_param, application);
    if (verification_result == mobile_apis::Result::INVALID_DATA) {
      LOG4CXX_WARN(logger_,"app_choise_sets missed icons");
      return false;
    }
  }

  return true;
}

bool ResumeCtrl::CheckApplicationHash(std::string mobile_app_id, uint32_t hash) {
  std::vector<Json::Value>::iterator it = saved_applications_.begin();
  for (; it != saved_applications_.end(); ++it) {
    std::string saved_m_app_id = (*it)[strings::app_id].asString();
    if (saved_m_app_id == mobile_app_id) {
      uint32_t saved_hash = (*it)[strings::hash_id].asUInt();

      LOG4CXX_INFO(logger_, "recived hash = " << hash);
      LOG4CXX_INFO(logger_, "saved hash = " << saved_hash);
      if (hash == saved_hash) {
        return true;
      }
    }
  }

  return false;
}

void ResumeCtrl::onTimer() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::onTimer()" << waiting_for_timer_.size());
  sync_primitives::AutoLock auto_lock(queue_lock_);

  std::multiset<application_timestamp, TimeStampComparator>::iterator it=
      waiting_for_timer_.begin();

  for (; it != waiting_for_timer_.end(); ++it) {
    ApplicationSharedPtr app =
        ApplicationManagerImpl::instance()->application((*it).first);
    if (!app.get()) {
      LOG4CXX_ERROR(logger_, "Invalid app_id = " << (*it).first);
      break;
    }

    RestoreApplicationHMILevel(app);
    RemoveApplicationFromSaved(app);
  }

  waiting_for_timer_.clear();
}

Json::Value ResumeCtrl::GetApplicationCommands(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationCommands" << app_id);

  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  DCHECK(app.get());

  Json::Value result;
  const CommandsMap& commands = app->commands_map();
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

Json::Value ResumeCtrl::GetApplicationSubMenus(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationCommands" << app_id);

  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  DCHECK(app.get());

  Json::Value result;
  const SubMenuMap& sub_menus = app->sub_menu_map();
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
    const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationInteractionChoiseSets"
               << app_id);

  Json::Value result;
  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  DCHECK(app.get());

  const ChoiceSetMap& choices = app->choice_set_map();
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

Json::Value ResumeCtrl::GetApplicationGlobalProperties(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationGlobalProperties" << app_id);

  Json::Value result;
  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  DCHECK(app.get());

  const smart_objects::SmartObject* help_promt = app->help_prompt();
  const smart_objects::SmartObject* timeout_prompt = app->timeout_prompt();
  const smart_objects::SmartObject* vr_help = app->vr_help();
  const smart_objects::SmartObject* vr_help_title = app->vr_help_title();
  const smart_objects::SmartObject* vr_synonyms = app->vr_synonyms();

  Json::Value sgp;
  sgp[strings::help_prompt] = JsonFromSO(help_promt);
  sgp[strings::timeout_prompt] = JsonFromSO(timeout_prompt);
  sgp[strings::vr_help] = JsonFromSO(vr_help);
  sgp[strings::vr_help_title] = JsonFromSO(vr_help_title);
  sgp[strings::vr_synonyms] = JsonFromSO(vr_synonyms);

  return sgp;
}

Json::Value ResumeCtrl::GetApplicationSubscriptions(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationSubscriptions" << app_id);

  Json::Value result;
  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  DCHECK(app.get());

  std::set<mobile_apis::ButtonName::eType>::iterator it_button ;
  std::set<uint32_t>::iterator it_vehicle;

  for (it_button = app->SubscribedButtons().begin() ;
       it_button != app->SubscribedButtons().end(); ++it_button) {
    result[strings::application_buttons].append(*it_button);
  }
  for (it_vehicle = app->SubscribesIVI().begin();
       it_vehicle != app->SubscribesIVI().end(); ++it_vehicle) {
    result[strings::application_vehicle_info].append(*it_vehicle);
  }
  return result;
}

Json::Value ResumeCtrl::GetApplicationFiles(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::GetApplicationFiles" << app_id);

  Json::Value result;
  ApplicationConstSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  DCHECK(app.get());

  const AppFilesMap& app_files = app->getAppFiles();
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



