/*
 Copyright (c) 2016, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/display_capabilities_builder.h"

#include <algorithm>
#include <fstream>

#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"

#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/resumption/resumption_data_db.h"
#include "application_manager/resumption/resumption_data_json.h"
#include "application_manager/resumption/resumption_data_processor_impl.h"
#include "application_manager/state_controller.h"
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler_impl.h"
#include "policy/policy_manager_impl.h"
#include "utils/file_system.h"
#include "utils/helpers.h"

#include "utils/timer_task_impl.h"

namespace resumption {
using namespace application_manager;

static mobile_api::HMILevel::eType PickHigherHmiLevel(
    mobile_api::HMILevel::eType val1, mobile_api::HMILevel::eType val2);
static mobile_api::HMILevel::eType PickLowerHmiLevel(
    mobile_api::HMILevel::eType val1, mobile_api::HMILevel::eType val2);
static mobile_api::HMILevel::eType ConvertHmiLevelString(
    const std::string& str);

SDL_CREATE_LOG_VARIABLE("Resumption")

ResumeCtrlImpl::ResumeCtrlImpl(ApplicationManager& application_manager)
    : restore_hmi_level_timer_(
          "RsmCtrlRstore",
          new timer::TimerTaskImpl<ResumeCtrlImpl>(
              this, &ResumeCtrlImpl::ApplicationResumptiOnTimer))
    , save_persistent_data_timer_("RsmCtrlPercist",
                                  new timer::TimerTaskImpl<ResumeCtrlImpl>(
                                      this, &ResumeCtrlImpl::SaveDataOnTimer))
    , is_resumption_active_(false)
    , is_data_saved_(false)
    , is_suspended_(false)
    , launch_time_(time(nullptr))
    , low_voltage_time_(0)
    , wake_up_time_(0)
    , application_manager_(application_manager)
    , resumption_data_processor_(
          new ResumptionDataProcessorImpl(application_manager)) {}
#ifdef BUILD_TESTS
void ResumeCtrlImpl::set_resumption_storage(
    std::shared_ptr<ResumptionData> mock_storage) {
  resumption_storage_ = mock_storage;
}

bool ResumeCtrlImpl::get_resumption_active() const {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  return is_resumption_active_;
}
#endif  // BUILD_TESTS

bool ResumeCtrlImpl::Init(resumption::LastStateWrapperPtr last_state_wrapper) {
  bool use_db = application_manager_.get_settings().use_db_for_resumption();
  if (use_db) {
    resumption_storage_.reset(
        new ResumptionDataDB(In_File_Storage, application_manager_));
    if (!resumption_storage_->Init()) {
      return false;
    }

    ResumptionDataDB* db =
        dynamic_cast<ResumptionDataDB*>(resumption_storage_.get());

    if (!db->IsDBVersionActual()) {
      SDL_LOG_INFO(
          "DB version had been changed. "
          "Rebuilding resumption DB.");

      smart_objects::SmartObject data;
      db->GetAllData(data);

      if (!db->RefreshDB()) {
        return false;
      }

      db->SaveAllData(data);
      db->UpdateDBVersion();
    }
  } else {
    resumption_storage_.reset(
        new ResumptionDataJson(last_state_wrapper, application_manager_));
    if (!resumption_storage_->Init()) {
      SDL_LOG_DEBUG("Resumption storage initialisation failed");
      return false;
    }
  }
  LoadResumeData();
  save_persistent_data_timer_.Start(
      application_manager_.get_settings()
          .app_resumption_save_persistent_data_timeout(),
      timer::kPeriodic);

  resumption_storage_->IncrementGlobalIgnOnCounter();
  return true;
}

ResumeCtrlImpl::~ResumeCtrlImpl() {}

void ResumeCtrlImpl::SaveAllApplications() {
  DataAccessor<ApplicationSet> accessor(application_manager_.applications());
  std::for_each(
      accessor.GetData().begin(),
      accessor.GetData().end(),
      std::bind1st(std::mem_fun(&ResumeCtrlImpl::SaveApplication), this));
}

void ResumeCtrlImpl::SaveApplication(ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(application);
  if (application_manager_.IsLowVoltage()) {
    SDL_LOG_DEBUG("Low Voltage state is active");
    return;
  }
  SDL_LOG_DEBUG("application with appID " << application->app_id()
                                          << " will be saved");
  resumption_storage_->SaveApplication(application);
}

void ResumeCtrlImpl::RestoreAppHMIState(ApplicationSharedPtr application) {
  using namespace mobile_apis;
  SDL_LOG_AUTO_TRACE();
  DCHECK(application);
  SDL_LOG_DEBUG("app_id : " << application->app_id() << "; policy_app_id : "
                            << application->policy_app_id());
  const std::string& device_mac = application->mac_address();
  smart_objects::SmartObject saved_app(smart_objects::SmartType_Map);
  bool result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  if (result) {
    if (saved_app.keyExists(strings::hmi_level)) {
      HMILevel::eType saved_hmi_level;
      if (HMILevel::eType::INVALID_ENUM !=
          application->deferred_resumption_hmi_level()) {
        saved_hmi_level = application->deferred_resumption_hmi_level();
        SDL_LOG_INFO("Retry resuming into HMI level : " << saved_hmi_level);
        application->set_deferred_resumption_hmi_level(
            HMILevel::eType::INVALID_ENUM);
      } else {
        saved_hmi_level = static_cast<mobile_apis::HMILevel::eType>(
            saved_app[strings::hmi_level].asInt());
        SDL_LOG_DEBUG("Saved HMI Level is : " << saved_hmi_level);
      }

      // Check one of the high-bandwidth transports (configured through
      // smartDeviceLink.ini file) is available. If not, then the HMI level to
      // resume into will be modified.
      if (!application_manager_.CheckResumptionRequiredTransportAvailable(
              application)) {
        mobile_apis::HMILevel::eType low_bandwidth_level =
            GetHmiLevelOnLowBandwidthTransport(application);

        application->set_deferred_resumption_hmi_level(saved_hmi_level);

        saved_hmi_level =
            PickLowerHmiLevel(saved_hmi_level, low_bandwidth_level);
        SDL_LOG_DEBUG(
            "High-bandwidth transport not available, app will resume into : "
            << saved_hmi_level);
      }
      const bool app_exists_in_full_or_limited =
          application_manager_.get_full_or_limited_application().use_count() !=
          0;
      SetAppHMIState(application, saved_hmi_level, true);
      if (app_exists_in_full_or_limited) {
        SDL_LOG_DEBUG("App exists in full or limited. Do not resume");
        return;
      }
    } else {
      SDL_LOG_ERROR("saved app data corrupted");
    }
  } else {
    SDL_LOG_ERROR("Application not saved");
  }
}

void ResumeCtrlImpl::ProcessSystemCapabilityUpdated(
    Application& app, const smart_objects::SmartObject& display_capabilities) {
  SDL_LOG_AUTO_TRACE();

  auto notification = MessageHelper::CreateDisplayCapabilityUpdateToMobile(
      display_capabilities, app);

  application_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::SOURCE_SDL);
}

bool ResumeCtrlImpl::SetupDefaultHMILevel(ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(application, false);
  mobile_apis::HMILevel::eType hmi_level =
      application_manager_.GetDefaultHmiLevel(application);

  // Check one of the high-bandwidth transports (configured through
  // smartDeviceLink.ini file) is available. If not, then the HMI level to
  // resume into will be modified.
  if (!application_manager_.CheckResumptionRequiredTransportAvailable(
          application)) {
    mobile_apis::HMILevel::eType low_bandwidth_level =
        GetHmiLevelOnLowBandwidthTransport(application);
    hmi_level = PickLowerHmiLevel(hmi_level, low_bandwidth_level);
    SDL_LOG_DEBUG(
        "High-bandwidth transport not available, default HMI level is set to : "
        << hmi_level);
  }
  return SetAppHMIState(application, hmi_level, false);
}

void ResumeCtrlImpl::ApplicationResumptiOnTimer() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(queue_lock_);
  WaitingForTimerList::iterator it = waiting_for_timer_.begin();

  for (; it != waiting_for_timer_.end(); ++it) {
    ApplicationSharedPtr app = application_manager_.application(*it);
    if (!app) {
      SDL_LOG_ERROR("Invalid app_id = " << *it);
      continue;
    }
    StartAppHmiStateResumption(app);
    app->set_is_resuming(false);
  }
  is_resumption_active_ = false;
  waiting_for_timer_.clear();
  StartSavePersistentDataTimer();
}

void ResumeCtrlImpl::OnAppActivated(ApplicationSharedPtr application) {
  if (is_resumption_active_) {
    RemoveFromResumption(application->app_id());
    application->set_is_resuming(false);
  }
}

void ResumeCtrlImpl::RemoveFromResumption(uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  queue_lock_.Acquire();
  waiting_for_timer_.remove(app_id);
  queue_lock_.Release();
}

bool ResumeCtrlImpl::Init(LastState&) {
  return false;
}

bool ResumeCtrlImpl::SetAppHMIState(
    ApplicationSharedPtr application,
    const mobile_apis::HMILevel::eType hmi_level,
    bool check_policy) {
  using namespace mobile_apis;
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(application, false);
  SDL_LOG_TRACE(" app_id : " << application->app_id()
                             << ", hmi_level : " << hmi_level
                             << ", check_policy : " << check_policy);
  const std::string& device_mac = application->mac_address();
  if (check_policy &&
      application_manager_.GetUserConsentForDevice(device_mac) !=
          policy::DeviceConsent::kDeviceAllowed) {
    SDL_LOG_ERROR("Resumption abort. Data consent wasn't allowed.");
    SetupDefaultHMILevel(application);
    return false;
  }

  application_manager_.state_controller().SetRegularState(
      application, mobile_apis::PredefinedWindows::DEFAULT_WINDOW, hmi_level);
  SDL_LOG_INFO("Application with policy id " << application->policy_app_id()
                                             << " got HMI level " << hmi_level);

  return true;
}

bool ResumeCtrlImpl::IsHMIApplicationIdExist(uint32_t hmi_app_id) {
  SDL_LOG_DEBUG("hmi_app_id :" << hmi_app_id);
  return resumption_storage_->IsHMIApplicationIdExist(hmi_app_id);
}

bool ResumeCtrlImpl::IsApplicationSaved(const std::string& policy_app_id,
                                        const std::string& device_id) {
  return -1 !=
         resumption_storage_->IsApplicationSaved(policy_app_id, device_id);
}

uint32_t ResumeCtrlImpl::GetHMIApplicationID(
    const std::string& policy_app_id, const std::string& device_mac) const {
  return resumption_storage_->GetHMIApplicationID(policy_app_id, device_mac);
}

bool ResumeCtrlImpl::RemoveApplicationFromSaved(
    ApplicationConstSharedPtr application) {
  if (application_manager_.IsLowVoltage()) {
    SDL_LOG_DEBUG("Low Voltage state is active");
    return false;
  }
  const std::string& device_mac = application->mac_address();
  return resumption_storage_->RemoveApplicationFromSaved(
      application->policy_app_id(), device_mac);
}

void ResumeCtrlImpl::OnSuspend() {
  SDL_LOG_AUTO_TRACE();
  is_suspended_ = true;
  FinalPersistData();
}

void ResumeCtrlImpl::OnIgnitionOff() {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(resumption_storage_);
  if (!application_manager_.IsLowVoltage()) {
    resumption_storage_->IncrementIgnOffCount();
    resumption_storage_->ResetGlobalIgnOnCount();
    FinalPersistData();
  }
}

void ResumeCtrlImpl::OnAwake() {
  SDL_LOG_AUTO_TRACE();
  is_suspended_ = false;
  ResetLaunchTime();
  StartSavePersistentDataTimer();
}

void ResumeCtrlImpl::SaveLowVoltageTime() {
  low_voltage_time_ = time(nullptr);
  SDL_LOG_DEBUG("Low Voltage timestamp : " << low_voltage_time_ << " saved");
}

void ResumeCtrlImpl::SaveWakeUpTime() {
  wake_up_time_ = std::time(nullptr);
  SDL_LOG_DEBUG("Wake Up timestamp : " << wake_up_time_ << " saved");
}

time_t ResumeCtrlImpl::LowVoltageTime() const {
  return low_voltage_time_;
}

time_t ResumeCtrlImpl::WakeUpTime() const {
  return wake_up_time_;
}

bool ResumeCtrlImpl::is_suspended() const {
  return is_suspended_;
}

void ResumeCtrlImpl::StartSavePersistentDataTimer() {
  SDL_LOG_AUTO_TRACE();
  if (!save_persistent_data_timer_.is_running()) {
    save_persistent_data_timer_.Start(
        application_manager_.get_settings()
            .app_resumption_save_persistent_data_timeout(),
        timer::kPeriodic);
  }
}

ResumptionDataProcessor& ResumeCtrlImpl::resumption_data_processor() {
  return *resumption_data_processor_;
}

void ResumeCtrlImpl::StopSavePersistentDataTimer() {
  SDL_LOG_AUTO_TRACE();
  if (save_persistent_data_timer_.is_running()) {
    save_persistent_data_timer_.Stop();
  }
}

bool ResumeCtrlImpl::StartResumption(ApplicationSharedPtr application,
                                     const std::string& hash,
                                     ResumptionCallBack callback) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(application, false);
  SDL_LOG_DEBUG(" Resume app_id = "
                << application->app_id()
                << " hmi_app_id = " << application->hmi_app_id()
                << " policy_id = " << application->policy_app_id()
                << " received hash = " << hash);
  application->set_is_resuming(true);

  smart_objects::SmartObject saved_app;
  const std::string& device_mac = application->mac_address();
  bool result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  if (result) {
    const std::string& saved_hash = saved_app[strings::hash_id].asString();
    result = saved_hash == hash ? RestoreApplicationData(application, callback)
                                : false;
  }
  return result;
}

void ResumeCtrlImpl::HandleOnTimeOut(
    const uint32_t cor_id, const hmi_apis::FunctionID::eType function_id) {
  SDL_LOG_AUTO_TRACE();
  resumption_data_processor_->HandleOnTimeOut(cor_id, function_id);
}

bool ResumeCtrlImpl::StartResumptionOnlyHMILevel(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  if (!application) {
    SDL_LOG_WARN("Application does not exist.");
    return false;
  }

  application->set_is_resuming(true);
  SDL_LOG_DEBUG("HMI level resumption requested for application id: "
                << application->app_id()
                << " with hmi_app_id: " << application->hmi_app_id()
                << ", policy_app_id " << application->policy_app_id());
  if (!application->is_cloud_app()) {
    // Default HMI Level is already set before resumption in
    // ApplicationManager::OnApplicationRegistered, and handling low bandwidth
    // transports doesn't apply to cloud apps, so this step can be skipped for
    // such apps
    SetupDefaultHMILevel(application);
  }
  const std::string& device_mac = application->mac_address();
  smart_objects::SmartObject saved_app;
  bool result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  if (result) {
    AddToResumptionTimerQueue(application->app_id());
  }
  SDL_LOG_INFO("StartResumptionOnlyHMILevel::Result = " << result);
  return result;
}

void ResumeCtrlImpl::RetryResumption(const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(app_id);
  if (!app) {
    SDL_LOG_WARN("Invalid app_id = " << app_id);
    return;
  }

  {
    sync_primitives::AutoLock auto_lock(queue_lock_);
    // check and update resumption deferred flag in queue_lock_
    if (mobile_api::HMILevel::eType::INVALID_ENUM ==
        app->deferred_resumption_hmi_level()) {
      SDL_LOG_DEBUG("No need to retry resumption for app: " << app_id);
      return;
    }
  }

  AddToResumptionTimerQueue(app_id);
}

void ResumeCtrlImpl::StartAppHmiStateResumption(
    ApplicationSharedPtr application) {
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();
  DCHECK(application);
  smart_objects::SmartObject saved_app;
  const std::string& device_mac = application->mac_address();
  const bool get_saved_app_result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  if (!get_saved_app_result) {
    SDL_LOG_ERROR("Application was not saved");
    return;
  }

  const bool is_hmi_level_applicable_to_resume =
      CheckAppRestrictions(application, saved_app);

  if (!is_hmi_level_applicable_to_resume) {
    SDL_LOG_DEBUG("No applicable HMI level found for resuming");
    return;
  }
  const bool is_resume_allowed_by_low_voltage =
      CheckLowVoltageRestrictions(saved_app);

  const time_t sdl_launch_time = LaunchTime();
  const bool is_unexpected_disconnect_in_current_ign_cycle =
      sdl_launch_time < saved_app[strings::time_stamp].asInt();
  SDL_LOG_DEBUG("sdl_launch_time: " << sdl_launch_time
                                    << ", App disconnect time: "
                                    << saved_app[strings::time_stamp].asInt());
  const bool is_app_revoked =
      application_manager_.GetPolicyHandler().IsApplicationRevoked(
          application->policy_app_id());

  bool restore_hmi_level_allowed = true;
  if (!is_unexpected_disconnect_in_current_ign_cycle) {
    const bool is_hmi_level_allowed_by_ign_cycle =
        CheckIgnCycleRestrictions(saved_app);
    restore_hmi_level_allowed = is_resume_allowed_by_low_voltage &&
                                is_hmi_level_allowed_by_ign_cycle &&
                                !is_app_revoked;
  } else {
    const bool is_resume_app_data_not_expired =
        !IsAppDataResumptionExpired(saved_app);
    restore_hmi_level_allowed = is_resume_allowed_by_low_voltage &&
                                is_resume_app_data_not_expired &&
                                !is_app_revoked;
  }

  if (restore_hmi_level_allowed) {
    SDL_LOG_INFO("Resume application " << application->policy_app_id());
    RestoreAppHMIState(application);
    if (mobile_apis::HMILevel::eType::INVALID_ENUM !=
        application->deferred_resumption_hmi_level()) {
      // the application has not been fully resumed
      return;
    }
    RemoveApplicationFromSaved(application);
  } else {
    SDL_LOG_INFO("Do not need to resume application "
                 << application->policy_app_id());
  }
}

void ResumeCtrlImpl::ResetLaunchTime() {
  SDL_LOG_AUTO_TRACE();
  launch_time_ = time(NULL);
}

bool ResumeCtrlImpl::CheckPersistenceFilesForResumption(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(application, false);
  SDL_LOG_DEBUG(" Resume app_id = " << application->app_id() << " policy_id = "
                                    << application->policy_app_id());
  smart_objects::SmartObject saved_app;
  const std::string& device_mac = application->mac_address();
  bool result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  if (result) {
    if (saved_app.keyExists(strings::application_commands)) {
      if (!CheckIcons(application, saved_app[strings::application_commands])) {
        return false;
      }
    }
    if (saved_app.keyExists(strings::application_choice_sets)) {
      if (!CheckIcons(application,
                      saved_app[strings::application_choice_sets])) {
        return false;
      }
    }
  }
  return true;
}

bool ResumeCtrlImpl::CheckApplicationHash(ApplicationSharedPtr application,
                                          const std::string& hash) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(application, false);
  SDL_LOG_DEBUG("app_id : " << application->app_id() << " hash : " << hash);
  smart_objects::SmartObject saved_app;
  const std::string& device_mac = application->mac_address();
  const bool get_app_result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  const bool check_result =
      get_app_result ? saved_app[strings::hash_id].asString() == hash : false;
  if (check_result) {
    application->set_app_data_resumption_allowance(true);
  }
  return check_result;
}

void ResumeCtrlImpl::SaveDataOnTimer() {
  SDL_LOG_AUTO_TRACE();
  if (is_resumption_active_) {
    SDL_LOG_WARN("Resumption timer is active skip saving");
    return;
  }

  if (false == is_data_saved_) {
    SaveAllApplications();
    is_data_saved_ = true;
    if (!application_manager_.get_settings().use_db_for_resumption()) {
      resumption_storage_->Persist();
    }
  }
}

void ResumeCtrlImpl::FinalPersistData() {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(resumption_storage_);
  StopSavePersistentDataTimer();
  SaveAllApplications();
  resumption_storage_->Persist();
}

bool ResumeCtrlImpl::IsDeviceMacAddressEqual(
    ApplicationSharedPtr application, const std::string& saved_device_mac) {
  SDL_LOG_AUTO_TRACE();
  const std::string device_mac = application->mac_address();
  return device_mac == saved_device_mac;
}

bool ResumeCtrlImpl::RestoreApplicationData(ApplicationSharedPtr application,
                                            ResumptionCallBack callback) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(application, false);
  SDL_LOG_DEBUG("app_id : " << application->app_id());

  smart_objects::SmartObject saved_app(smart_objects::SmartType_Map);
  const std::string& device_mac = application->mac_address();
  bool result = resumption_storage_->GetSavedApplication(
      application->policy_app_id(), device_mac, saved_app);
  if (result) {
    if (saved_app.keyExists(strings::grammar_id)) {
      const uint32_t app_grammar_id = saved_app[strings::grammar_id].asUInt();
      application->set_grammar_id(app_grammar_id);
      resumption_data_processor_->Restore(application, saved_app, callback);
      result = true;
    } else {
      SDL_LOG_WARN("Saved data of application does not contain grammar_id");
      result = false;
    }
  } else {
    SDL_LOG_WARN("Application not saved");
  }
  return result;
}

void ResumeCtrlImpl::StartWaitingForDisplayCapabilitiesUpdate(
    app_mngr::ApplicationSharedPtr application, const bool is_resume_app) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObject saved_app(smart_objects::SmartType_Map);
  resumption_storage_->GetSavedApplication(
      application->policy_app_id(), application->mac_address(), saved_app);
  auto resume_callback =
      [this](Application& app,
             const smart_objects::SmartObject& display_capabilities) -> void {
    SDL_LOG_AUTO_TRACE();
    ProcessSystemCapabilityUpdated(app, display_capabilities);
  };
  auto& builder = application->display_capabilities_builder();

  smart_objects::SmartObject windows_info(smart_objects::SmartType_Null);
  if (is_resume_app && saved_app.keyExists(strings::windows_info)) {
    windows_info = saved_app[strings::windows_info];
  }
  builder.InitBuilder(resume_callback, windows_info);
}

bool ResumeCtrlImpl::CheckIgnCycleRestrictions(
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  if (!CheckDelayAfterIgnOn()) {
    SDL_LOG_DEBUG("Application was connected long after ign on");
    return false;
  }

  if (!CheckDelayBeforeIgnOff(saved_app)) {
    SDL_LOG_DEBUG("Application was disconnected long before ign off");
    return false;
  }
  return true;
}

bool ResumeCtrlImpl::CheckLowVoltageRestrictions(
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  if (!CheckDelayBeforeLowVoltage(saved_app)) {
    SDL_LOG_DEBUG("Application was disconnected long before low voltage");
    return false;
  }

  if (!CheckDelayAfterWakeUp()) {
    SDL_LOG_DEBUG("Application was connected long after wake up");
    return false;
  }

  SDL_LOG_DEBUG("HMI Level resuming in not restricted by Low Voltage");
  return true;
}

bool ResumeCtrlImpl::CheckDelayBeforeIgnOff(
    const smart_objects::SmartObject& saved_app) const {
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(saved_app.keyExists(strings::time_stamp), false);

  const time_t time_stamp =
      static_cast<time_t>(saved_app[strings::time_stamp].asInt());
  const time_t ign_off_time =
      static_cast<time_t>(resumption_storage_->GetIgnOffTime());

  if (CheckIgnCyclesData() && 0 == ign_off_time) {
    SDL_LOG_DEBUG(
        "No IGNITION OFF records found: This is first Ignition cycle");
    return true;
  }

  // This means that ignition off timestamp was not saved
  // Possible reasons: Low Voltage event, core crash etc.
  if (ign_off_time < time_stamp) {
    SDL_LOG_DEBUG("Last IGNITION OFF record missed");
    return true;
  }

  const uint32_t sec_spent_before_ign = labs(ign_off_time - time_stamp);
  SDL_LOG_DEBUG(
      "ign_off_time "
      << ign_off_time << "; app_disconnect_time " << time_stamp
      << "; sec_spent_before_ign " << sec_spent_before_ign
      << "; resumption_delay_before_ign "
      << application_manager_.get_settings().resumption_delay_before_ign());
  return sec_spent_before_ign <=
         application_manager_.get_settings().resumption_delay_before_ign();
}

bool ResumeCtrlImpl::CheckDelayBeforeLowVoltage(
    const smart_objects::SmartObject& saved_app) const {
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(saved_app.keyExists(strings::time_stamp), false);

  if (0 == LowVoltageTime()) {
    SDL_LOG_DEBUG("No Low Voltage signal timestamp saved");
    return true;
  }

  const time_t unregistration_time_stamp =
      static_cast<time_t>(saved_app[strings::time_stamp].asInt());
  const time_t low_voltage_timestamp = static_cast<time_t>(LowVoltageTime());
  const int32_t sec_spent_before_low_voltage =
      (low_voltage_timestamp - unregistration_time_stamp);
  if (0 > sec_spent_before_low_voltage) {
    SDL_LOG_DEBUG("Low Voltage time: "
                  << low_voltage_timestamp
                  << "; App disconnect time: " << unregistration_time_stamp
                  << "; Secs between app disconnect and low voltage event "
                  << sec_spent_before_low_voltage);
    return true;
  }

  const uint32_t secs_between_app_disconnect_and_low_voltage =
      static_cast<uint32_t>(sec_spent_before_low_voltage);
  const uint32_t wait_time =
      application_manager_.get_settings().resumption_delay_before_ign();
  SDL_LOG_DEBUG("Low Voltage time: "
                << low_voltage_timestamp
                << "; App disconnect time: " << unregistration_time_stamp
                << "; Secs between app disconnect and low voltage event "
                << secs_between_app_disconnect_and_low_voltage
                << "; Timeout for HMI level resuming:  " << wait_time);
  return secs_between_app_disconnect_and_low_voltage <= wait_time;
}

bool ResumeCtrlImpl::CheckDelayAfterWakeUp() const {
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();

  if (0 == WakeUpTime()) {
    SDL_LOG_DEBUG("No WakeUp signal timestamp saved");
    return true;
  }

  const time_t current_time = time(nullptr);
  const time_t wake_up_timestamp = static_cast<time_t>(WakeUpTime());

  const uint32_t seconds_from_wake_up_signal =
      labs(current_time - wake_up_timestamp);
  const uint32_t wait_time =
      application_manager_.get_settings().resumption_delay_after_ign();
  SDL_LOG_DEBUG("Current time: "
                << current_time << "; WakeUp Signal time: " << wake_up_timestamp
                << "; Seconds passed from wake up: "
                << seconds_from_wake_up_signal
                << "; Timeout for HMI level resuming:  " << wait_time);
  return seconds_from_wake_up_signal <= wait_time;
}

bool ResumeCtrlImpl::CheckAppRestrictions(
    ApplicationConstSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  using namespace mobile_apis;
  using namespace helpers;
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(saved_app.keyExists(strings::hmi_level), false);

  HMILevel::eType hmi_level;
  if (mobile_api::HMILevel::eType::INVALID_ENUM !=
      application->deferred_resumption_hmi_level()) {
    hmi_level = application->deferred_resumption_hmi_level();
  } else {
    hmi_level =
        static_cast<HMILevel::eType>(saved_app[strings::hmi_level].asInt());
  }
  const bool result = Compare<HMILevel::eType, EQ, ONE>(
                          hmi_level, HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)
                          ? true
                          : false;
  SDL_LOG_DEBUG("is_media_app: " << application->is_media_application()
                                 << "; hmi_level: " << hmi_level << "; result: "
                                 << (result ? "Applicable for resume"
                                            : "Non-applicable for resume"));
  return result;
}

bool ResumeCtrlImpl::CheckIcons(ApplicationSharedPtr application,
                                smart_objects::SmartObject& obj) {
  using namespace smart_objects;
  SDL_LOG_AUTO_TRACE();
  const mobile_apis::Result::eType verify_images =
      MessageHelper::VerifyImageFiles(obj, application, application_manager_);
  return mobile_apis::Result::INVALID_DATA != verify_images;
}

bool ResumeCtrlImpl::CheckIgnCyclesData() const {
  SDL_LOG_AUTO_TRACE();
  const uint32_t global_ign_on_count =
      resumption_storage_->GetGlobalIgnOnCounter();
  const uint32_t the_first_ignition = 1;
  const bool is_emergency_ign_off_occurred =
      global_ign_on_count > the_first_ignition;
  // global_ign_on_count is reseting to 0 at ignition off
  // global_ign_on_count is incrementing at ignition on
  // global_ign_on_count > 1 means that correct ignition off was not present.
  if (is_emergency_ign_off_occurred) {
    SDL_LOG_WARN("Emergency IGN OFF occurred. Possibly after Low Voltage");
    return false;
  }
  return true;
}

bool ResumeCtrlImpl::CheckDelayAfterIgnOn() const {
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();
  const time_t ign_off_time = GetIgnOffTime();

  if (CheckIgnCyclesData() && 0 == ign_off_time) {
    SDL_LOG_DEBUG("This is first Ignition cycle");
    return true;
  }
  const time_t curr_time = time(nullptr);
  const time_t sdl_launch_time = LaunchTime();

  const uint32_t seconds_from_sdl_start = labs(curr_time - sdl_launch_time);
  const uint32_t wait_time =
      application_manager_.get_settings().resumption_delay_after_ign();
  SDL_LOG_DEBUG("curr_time " << curr_time << "; sdl_launch_time "
                             << sdl_launch_time << "; seconds_from_sdl_start "
                             << seconds_from_sdl_start << "; wait_time "
                             << wait_time);
  return seconds_from_sdl_start <= wait_time;
}

time_t ResumeCtrlImpl::LaunchTime() const {
  return launch_time_;
}

time_t ResumeCtrlImpl::GetIgnOffTime() const {
  return resumption_storage_->GetIgnOffTime();
}

void ResumeCtrlImpl::AddToResumptionTimerQueue(const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  bool run_resumption = false;
  queue_lock_.Acquire();
  waiting_for_timer_.push_back(app_id);

  if (!is_resumption_active_) {
    is_resumption_active_ = true;
    run_resumption = true;
  }
  queue_lock_.Release();
  SDL_LOG_DEBUG("Application ID " << app_id
                                  << " have been added"
                                     " to resumption queue.");
  if (run_resumption) {
    SDL_LOG_DEBUG("Application ID " << app_id << " will be restored by timer");
    restore_hmi_level_timer_.Start(
        application_manager_.get_settings().app_resuming_timeout(),
        timer::kSingleShot);
  }
}

void ResumeCtrlImpl::LoadResumeData() {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObject so_applications_data;
  resumption_storage_->GetDataForLoadResumeData(so_applications_data);
  size_t length = so_applications_data.length();
  for (size_t i = 0; i < length; ++i) {
    smart_objects::SmartObject& application = so_applications_data[i];
    if (IsAppDataResumptionExpired(application)) {
      const std::string device_id = application[strings::device_id].asString();
      const std::string app_id = application[strings::app_id].asString();
      SDL_LOG_INFO("Data resumption is expired.");
      SDL_LOG_DEBUG("Resumption data for application "
                    << app_id << " and device id " << device_id
                    << " will be dropped.");
      resumption_storage_->RemoveApplicationFromSaved(app_id, device_id);
      continue;
    }
  }
}

void ResumeCtrlImpl::OnAppRegistrationStart(const std::string& policy_app_id,
                                            const std::string& device_id) {
  SDL_LOG_AUTO_TRACE();
  if (IsApplicationSaved(policy_app_id, device_id)) {
    SDL_LOG_INFO(
        "Application is found in resumption "
        "data and will try to resume. Stopping resume data persistent timer");
    StopSavePersistentDataTimer();
  }
}

void ResumeCtrlImpl::OnAppRegistrationEnd() {
  SDL_LOG_AUTO_TRACE();
  StartSavePersistentDataTimer();
}

int32_t ResumeCtrlImpl::GetSavedAppHmiLevel(
    const std::string& app_id, const std::string& device_id) const {
  using namespace mobile_apis;
  smart_objects::SmartObject saved_app;
  if (resumption_storage_->GetSavedApplication(app_id, device_id, saved_app)) {
    const HMILevel::eType saved_hmi_level =
        static_cast<mobile_apis::HMILevel::eType>(
            saved_app[strings::hmi_level].asInt());

    return saved_hmi_level;
  }
  return static_cast<int32_t>(mobile_apis::HMILevel::INVALID_ENUM);
}

bool ResumeCtrlImpl::IsAppDataResumptionExpired(
    const smart_objects::SmartObject& application) const {
  const int32_t max_ign_off_count = 3;
  return max_ign_off_count <= application[strings::ign_off_count].asInt();
}

mobile_apis::HMILevel::eType ResumeCtrlImpl::GetHmiLevelOnLowBandwidthTransport(
    ApplicationConstSharedPtr application) const {
  using namespace mobile_apis;
  SDL_LOG_AUTO_TRACE();

  const ApplicationManagerSettings& settings =
      application_manager_.get_settings();
  const std::string& level_for_navi_app =
      settings.navigation_lowbandwidth_resumption_level();
  const std::string& level_for_projection_app =
      settings.projection_lowbandwidth_resumption_level();
  const std::string& level_for_media_app =
      settings.media_lowbandwidth_resumption_level();

  HMILevel::eType result_level = HMILevel::HMI_NONE;
  HMILevel::eType level;

  // NAVIGATION, PROJECTION and media apps have special exceptions. Their HMI
  // level can be configured through .ini file. when the app has multiple
  // AppHMIType, then the highest level will be applied.
  if (application->is_navi()) {
    level = ConvertHmiLevelString(level_for_navi_app);
    SDL_LOG_DEBUG("NAVIGATION apps may have level "
                  << level
                  << " while high-bandwidth transport is not available.");
    result_level = PickHigherHmiLevel(level, result_level);
  }
  if (application->mobile_projection_enabled()) {
    level = ConvertHmiLevelString(level_for_projection_app);
    SDL_LOG_DEBUG("PROJECTION apps may have level "
                  << level
                  << " while high-bandwidth transport is not available.");
    result_level = PickHigherHmiLevel(level, result_level);
  }
  if (application->is_media_application()) {
    level = ConvertHmiLevelString(level_for_media_app);
    SDL_LOG_DEBUG("media apps may have level "
                  << level
                  << " while high-bandwidth transport is not available.");
    result_level = PickHigherHmiLevel(level, result_level);
  }

  return result_level;
}

static mobile_api::HMILevel::eType PickHigherHmiLevel(
    mobile_api::HMILevel::eType val1, mobile_api::HMILevel::eType val2) {
  using namespace mobile_apis;

  if (val1 == HMILevel::INVALID_ENUM) {
    return val2;
  } else if (val2 == HMILevel::INVALID_ENUM) {
    return val1;
  }

  // smaller enum value has higher level
  return val1 < val2 ? val1 : val2;
}

static mobile_api::HMILevel::eType PickLowerHmiLevel(
    mobile_api::HMILevel::eType val1, mobile_api::HMILevel::eType val2) {
  using namespace mobile_apis;

  if (val1 == HMILevel::INVALID_ENUM) {
    return val2;
  } else if (val2 == HMILevel::INVALID_ENUM) {
    return val1;
  }

  // bigger enum value has lower level
  return val1 > val2 ? val1 : val2;
}

static mobile_api::HMILevel::eType ConvertHmiLevelString(
    const std::string& str) {
  using namespace mobile_apis;

  if ("BACKGROUND" == str) {
    return HMILevel::HMI_BACKGROUND;
  } else if ("FULL" == str) {
    return HMILevel::HMI_FULL;
  } else if ("LIMITED" == str) {
    return HMILevel::HMI_LIMITED;
  } else {
    return HMILevel::HMI_NONE;
  }
}

}  // namespace resumption
