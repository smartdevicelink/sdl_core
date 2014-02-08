#include "application_manager/resume_ctrl.h"

#include <fstream>

#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "connection_handler/connection_handler_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"


namespace application_manager {
  log4cxx::LoggerPtr ResumeCtrl::logger_ = log4cxx::LoggerPtr(
        log4cxx::Logger::getLogger("ResumeCtrl"));

ResumeCtrl::ResumeCtrl(const ApplicationManagerImpl* app_mngr)
  : app_mngr_(app_mngr),
    timer_(this, &ResumeCtrl::onTimer, true) {

}

void ResumeCtrl::SaveAllApplications() {
  LOG4CXX_INFO(logger_, " ResumeCtrl::SaveApplications()");

  if (!app_mngr_) {
    LOG4CXX_ERROR(logger_, "application_manager_ is NULL");
    return ;
  }
  std::set<Application*>::iterator it = 
								  app_mngr_->application_list_.begin();
  std::set<Application*>::iterator it_end =
								   app_mngr_->application_list_.end();
  for (; it != it_end; ++it) {
    SaveApplication(*it);
  }
}

void ResumeCtrl::SaveApplication(Application* application) {
  LOG4CXX_INFO(logger_, " ResumeCtrl::SaveApplication");

  std::string mac_adddress = GetMacAddress(application);
  if (mac_adddress == "") {
    LOG4CXX_INFO(logger_, "There were error during getting"
                          "mac adress with appID(connectionKey) = "
                          <<application->app_id());
    return;
  }

  if (application->hmi_level() == mobile_apis::HMILevel::HMI_NONE) {
    LOG4CXX_INFO(logger_, " HMI Level is None. Do not need to be saved");
    return ;
  }

  Json::Value* curr_app = NULL;
  uint32_t app_id = application->mobile_app_id()->asInt();
  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
            it != saved_applications_.end(); ++it) {
    if ((*it)[strings::app_id].asInt() == app_id) {
      curr_app = &(*it);
      LOG4CXX_INFO(logger_, "ResumeCtrl Application with this id"
                            "is already exist ( update info ) " << app_id);
      break;
    }
  }

  if (curr_app == NULL) {
    LOG4CXX_INFO(logger_, "ResumeCtrl Application with this ID does not"
						  "exists. Add new"
                  << app_id);
    saved_applications_.push_back(Json::Value());
    curr_app = &(saved_applications_.back());
  }

  (*curr_app)[strings::app_id] = app_id;
  (*curr_app)[strings::connection_key] = application->app_id();
  (*curr_app)[strings::hmi_level] = static_cast<int32_t>
											 (application->hmi_level());
  (*curr_app)["mac_address"] = mac_adddress; // in case of Wifi - IP adress

////TODO : move this logic to others classes responsibility
//  Json::Value app_files_data;
//  const std::vector<AppFile> &app_files = application->getAppFiles();
//  for(std::vector<AppFile>::const_iterator file_it = app_files.begin();
//                                file_it != app_files.end(); file_it++) {
//    if ((*file_it).is_persistent) {
//      Json::Value file_data;
//      file_data["is_persistent"] = (*file_it).is_persistent;
//      file_data["is_download_complete"] = (*file_it).is_download_complete;
//      file_data["file_name"] = (*file_it).file_name;
//      app_files_data.append(file_data);
//    }
//  }
//  (*curr_app)["app_files"] = app_files_data;
  (*curr_app)["ign_off_count"] = 0;

}

void ResumeCtrl::LoadApplications() {
  LOG4CXX_INFO(logger_, "ResumeCtrl::LoadApplications");

  if (!app_mngr_) {
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

  for (Json::Value::iterator it = root.begin(); it != root.end(); ++it) {
    Json::Value cur_app_data = (*it);
    LOG4CXX_INFO(logger_, "Restored Application: " << 
						  cur_app_data.toStyledString());
    saved_applications_.push_back(cur_app_data);
  }
}

void ResumeCtrl::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::on_event ");

  const smart_objects::SmartObject& smart_object = event.smart_object();
  uint32_t corr_id = smart_object[strings::params][strings::correlation_id].asUInt();
  Application* application;
  for (std::list<ResumingApp*>::iterator it = resuming_applications_.begin();
      it != resuming_applications_.end(); ) {
    if ((*it)->correlation_id == corr_id) {
      application = (*it)->app;
      delete (*it);
      it = resuming_applications_.erase(it);
    } else {
      ++it;
    }
  }
  if (!application) {
    LOG4CXX_WARN(logger_, "Resuming usaved APP! Impossible");
    return;
  }
  if (smart_object[hmi_notification::result].asBool()) {
    LOG4CXX_WARN(logger_, "Restoring HMI Level");
    if ((RestoreApplicationHMILevel(application)) &
         (application->hmi_level() == mobile_apis::HMILevel::HMI_FULL)) {
        MessageHelper::SendActivateAppToHMI(application->app_id());
      }
  } else {
    // TODO: restore HMI level from policy table
  }
  MessageHelper::SendHMIStatusNotification(*application);
  if (resuming_applications_.empty()) {
    timer_.stop();
  }
}

bool ResumeCtrl::RestoreApplicationHMILevel(Application* application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::RestoreApplicationHMILevel");

  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
            it != saved_applications_.end(); ++it) {
    if ((*it)[strings::app_id].asInt() == application->mobile_app_id()->asInt()) {
      mobile_apis::HMILevel::eType saved_hmi_level = 
      static_cast<mobile_apis::HMILevel::eType>((*it)[strings::hmi_level].asInt());
      application->set_hmi_level(saved_hmi_level);
      return true;
    }
  }
  return false;
}

bool ResumeCtrl::ApplicationIsSaved(const Application* application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::ApplicationIsSaved "
						<< application->mobile_app_id());

  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
            it != saved_applications_.end(); ++it) {
    if ((*it)[strings::app_id].asInt() == application->mobile_app_id()->asInt()) {
      return true;
    }
  }
  return false;
}

bool ResumeCtrl::RemoveApplicationFromSaved(const Application *application) {
  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
            it != saved_applications_.end(); ) {
    if ((*it)[strings::app_id].asInt() == application->mobile_app_id()->asInt()) {
      it = saved_applications_.erase(it);
      return true;
    } else {
      ++it;
    }
  }
  return false;
}

void ResumeCtrl::SavetoFS() {
  LOG4CXX_INFO(logger_, " SavetoFS ");

  const std::string& storage =
    profile::Profile::instance()->app_info_storage();
  std::ofstream file(file_system::FullPath(storage).c_str(),
                     std::ios::out);
  Json::Value to_save;
  for (std::vector<Json::Value>::iterator it = saved_applications_.begin();
       it != saved_applications_.end(); ++it) {
    uint32_t ign_off_count = (*it)["ign_off_count"].asUInt();
    if (ign_off_count < kApplicationLifes) {
      ign_off_count++;
      (*it)["ign_off_count"] = ign_off_count;
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

bool ResumeCtrl::StartResumption(Application* application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::StartResumption");

  bool is_saved = ApplicationIsSaved(application);
  if (is_saved) {
    ResumingApp* resuming_app = new ResumingApp();
    resuming_app->app = application;
    resuming_app->start_resuming_time = time(NULL);
    resuming_app->is_waiting_for_timer = true;
    resuming_applications_.push_back(resuming_app);
    LOG4CXX_INFO(logger_,"Resumming app start timer id =" 
				 << resuming_app->app->app_id() << " start time ="
                 <<resuming_app->is_waiting_for_timer);
    timer_.start(kTimeStep);
  }
  return is_saved;
}


void ResumeCtrl::onTimer() {
  for(std::list<ResumingApp*>::iterator it = resuming_applications_.begin();
      it != resuming_applications_.end(); ++it) {
    if (! ((*it)->is_waiting_for_timer )) {
      LOG4CXX_INFO(logger_, " " << (*it)->app->app_id() 
					<< "Is wiat for response"
					 << (*it)->is_waiting_for_timer);
      // if correlation_id has some state - 
      //it means that this application whait for response
      continue;
    }
    time_t expired = time(NULL) - (*it)->start_resuming_time ;
    if (expired >= profile::Profile::instance()->app_resuming_timeout()) {
      sendResumptionRequest((*it));
    }
  }
}

void ResumeCtrl::sendResumptionRequest(ResumingApp* application) {
  LOG4CXX_INFO(logger_, "ResumeCtrl::StartResumptionRequest");

  uint32_t correlation_id = ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
                                          smart_objects::SmartType_Map);
  if (!message) {
    LOG4CXX_ERROR(logger_, "OUT OF MEMORY");
    return;
  }
  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_GetResumeResult;
  (*message)[strings::params][strings::message_type] = MessageType::kRequest;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::msg_params][strings::app_id] = application->app->app_id();
  if (ApplicationManagerImpl::instance()->ManageHMICommand(message)) {
    application->is_waiting_for_timer = false;
    application->correlation_id = correlation_id;
    subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_GetResumeResult,
					   correlation_id);
    LOG4CXX_INFO(logger_, "Subscribed to event "
				<< hmi_apis::FunctionID::BasicCommunication_GetResumeResult
				<< " corID "<<correlation_id);
  } else {
    LOG4CXX_ERROR(logger_, "ManageHMICommand Error")
  }
}


std::string ResumeCtrl::GetMacAddress(const Application* application) {
   LOG4CXX_INFO(logger_, "ResumeCtrl::GetMacAddress");

   if (0 == app_mngr_->connection_handler_) {
     LOG4CXX_ERROR(logger_,
                   "Unable to get necessary parameters during saving "
                   "applications information.");
     return "";
   }
   connection_handler::ConnectionHandlerImpl* conn_handler =
     static_cast<connection_handler::ConnectionHandlerImpl*>(
       app_mngr_->connection_handler_);
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
                     "There is an error occurs during getting device MAC.");
     }
   }
   return mac_adddress;
}
}  // namespace application_manager

