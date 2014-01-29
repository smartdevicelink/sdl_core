#include "application_manager/resume_ctrl.h"

#include <fstream>

#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "connection_handler/connection_handler_impl.h"
#include "application_manager/application_manager_impl.h"
namespace application_manager {
  log4cxx::LoggerPtr ResumeCtrl::logger_ = log4cxx::LoggerPtr(
        log4cxx::Logger::getLogger("ResumeCtrl"));

ResumeCtrl::ResumeCtrl(ApplicationManagerImpl *application_manager)
          :application_manager_(application_manager) {
}



ResumeCtrl::~ResumeCtrl() {
  LOG4CXX_INFO(logger_, " ResumeCtrl::~ResumeCtrl");
  const std::string& storage =
    profile::Profile::instance()->app_info_storage();
  std::ofstream file(file_system::FullPath(storage).c_str(),
                     std::ios::out);
  Json::Value to_save;
  for (std::vector<Json::Value>::iterator it = this->saved_applications_vector.begin();
       it != saved_applications_vector.end(); it++) {
    if ((*it)["ing_off_count"].asInt() < 3) {
      (*it)["ing_off_count"] = (*it)["ing_off_count"].asInt() + 1;
      to_save.append(*it);
    }
  }
  if (file.is_open()) {
    file << to_save.toStyledString();
  } else {
    LOG4CXX_ERROR(logger_,
                  "There is an error occurs during saving application info.");
  }
  file.close();
}

void ResumeCtrl::SaveAllApplications() {
  LOG4CXX_INFO(logger_, " ResumeCtrl::SaveApplications()");
  if (!this->application_manager_) {
    LOG4CXX_ERROR(logger_, "application_manager_ is NULL");
    return ;
  }
  std::set<Application*>::iterator it = application_manager_->application_list_.begin();
  std::set<Application*>::iterator it_end = application_manager_->application_list_.end();
  for (; it != it_end; ++it) {
    this->SaveApplication(*it);
  }  // end of app.list
}

void ResumeCtrl::SaveApplication(Application *application) {

  LOG4CXX_INFO(logger_, " ResumeCtrl::SaveApplication");

  std::string mac_adddress = this->GetMacAddress(application);
  if(mac_adddress == "") {
    LOG4CXX_INFO(logger_, "ResumeCtrl there were error during getting mac adress with appID(connectionKey) = "
                  <<application->app_id());
    return;
  }
  Json::Value *curr_app = NULL;
  uint32_t app_id = application->mobile_app_id()->asInt();
  for (std::vector<Json::Value>::iterator it = this->saved_applications_vector.begin();
            it != saved_applications_vector.end(); it++) {
    if ((*it)[strings::app_id].asInt() == app_id) {
      curr_app = &(*it);
      LOG4CXX_INFO(logger_, "ResumeCtrl Application with this id is already exist ( update info ) " << app_id);
      break;
    }
  }
  if(curr_app == NULL) {
    LOG4CXX_INFO(logger_, "ResumeCtrl Application with this ID does not exists. Add new"
                  << app_id);
    saved_applications_vector.push_back(Json::Value());
    curr_app = &(saved_applications_vector.back());
  }
  (*curr_app)[strings::app_id] = app_id;
  (*curr_app)[strings::connection_key] = application->app_id();
  (*curr_app)[strings::hmi_level] = static_cast<int32_t>(application->hmi_level());
  (*curr_app)["mac_address"] = mac_adddress; // in case of Wifi it is IP adress
  Json::Value app_files_data;
  const std::vector<AppFile> &app_files = application->getAppFiles();
  for(std::vector<AppFile>::const_iterator file_it = app_files.begin();
                                file_it != app_files.end(); file_it++) {
    if ((*file_it).is_persistent) {
      Json::Value file_data;
      file_data["is_persistent"] = (*file_it).is_persistent;
      file_data["is_download_complete"] = (*file_it).is_download_complete;
      file_data["file_name"] = (*file_it).file_name;
      app_files_data.append(file_data);
    }
  }
  (*curr_app)["app_files"] = app_files_data;
  (*curr_app)["ing_off_count"] = 0;
}

void ResumeCtrl::LoadApplications() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::LoadApplications");
  if (!this->application_manager_) {
    LOG4CXX_ERROR(logger_, "application_manager_ is NULL");
    return;
  }
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
  for (Json::Value::iterator it = root.begin(); it != root.end(); it++) {
    Json::Value cur_app_data = (*it);
    LOG4CXX_INFO(logger_, "Restored Application: "<<cur_app_data.toStyledString());
    this->saved_applications_vector.push_back(cur_app_data);
  }
}

bool ResumeCtrl::RestoreApplicationFiles(Application *application,
                                                                 bool only_persistent) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::RestoreApplicationPercictentData");
  for (std::vector<Json::Value>::iterator it = this->saved_applications_vector.begin();
            it != saved_applications_vector.end(); it++) {
    if ((*it)[strings::app_id].asInt() == application->mobile_app_id()->asInt()) {
      std::string relative_file_path =
          file_system::CreateDirectory(application->name());
      relative_file_path += "/";
      for (Json::Value::iterator files_it = (*it)["app_files"].begin();
          files_it != (*it)["app_files"].end(); files_it++) {
        if (!file_system::FileExists(relative_file_path + (*files_it)["file_name"].asString())) {
          continue;
        }
        if ( !only_persistent | (*files_it)["is_persistent"].asBool()) {
          if (application->AddFile((*files_it)["file_name"].asString(),
                                   (*files_it)["is_persistent"].asBool(),
                                   (*files_it)["is_download_complete"].asBool())) {
            LOG4CXX_INFO(logger_,"File restored: " << (*files_it).toStyledString() <<
                         "For application " <<  application->mobile_app_id()->asInt());
          }
        }
      }
      //saved_applications_vector.erase(it);
      LOG4CXX_INFO(logger_,"Restore Succesfull!");
      return true;
    }
  }
  LOG4CXX_INFO(logger_,"Couldn't Restore!");
  return false;
}

bool ResumeCtrl::RestoreApplicationHMILevel(Application *application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::RestoreApplicationHMILevel");
  for (std::vector<Json::Value>::iterator it = this->saved_applications_vector.begin();
            it != saved_applications_vector.end(); it++) {
    if ((*it)[strings::app_id].asInt() == application->mobile_app_id()->asInt()) {
      mobile_apis::HMILevel::eType saved_hmi_level = static_cast<mobile_apis::HMILevel::eType>((*it)[strings::hmi_level].asInt());
      application->set_hmi_level(saved_hmi_level);
      return true;
    }
  }
  return false;
}


std::string ResumeCtrl::GetMacAddress(Application *application) {
   LOG4CXX_INFO(logger_, "ResumeCtrl::GetMacAddress");
   if (0 == this->application_manager_->connection_handler_) {
     LOG4CXX_ERROR(logger_,
                   "Unable to get necessary parameters during saving "
                   "applications information.");
     return "";
   }
   connection_handler::ConnectionHandlerImpl* conn_handler =
     static_cast<connection_handler::ConnectionHandlerImpl*>(
       application_manager_->connection_handler_);
   uint32_t device_id = 0;
   std::string mac_adddress = "";
   if (-1 != conn_handler->GetDataOnSessionKey(
         application->app_id(),
         NULL,
         NULL,
         &device_id)) {

     if (-1 != conn_handler->GetDataOnDeviceID(
           device_id,
           NULL,
           NULL,
           &mac_adddress)) {

       LOG4CXX_ERROR(logger_,
                     "There is an error occurs during getting of device MAC.");
     }
   }
  return mac_adddress;
}
}  // namespace application_manager

