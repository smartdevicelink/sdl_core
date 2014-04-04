/*
 Copyright (c) 2013, Ford Motor Company
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

#include <unistd.h>
#include <dlfcn.h>
#include <algorithm>
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/policy_retry_sequence.h"
#include "application_manager/policies/pt_exchange_handler_impl.h"
#include "application_manager/policies/pt_exchange_handler_ext.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "policy/policy_manager_impl.h"
#include "utils/macro.h"
#include "utils/date_time.h"
#include "json/value.h"
#include "config_profile/profile.h"
#include "application_manager/usage_statistics.h"

namespace policy {
typedef std::set<utils::SharedPtr<application_manager::Application>> ApplicationList;

PolicyHandler* PolicyHandler::instance_ = NULL;
const std::string PolicyHandler::kLibrary = "libPolicy.so";

log4cxx::LoggerPtr PolicyHandler::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("PolicyHandler"));

PolicyHandler::PolicyHandler()
  : policy_manager_(0),
    dl_handle_(0),
    exchange_handler_(NULL),
    is_exchange_in_progress_(false),
    retry_sequence_("RetrySequence", new RetrySequence(this)) {
}

PolicyHandler::~PolicyHandler() {
  UnloadPolicyLibrary();
}

PolicyManager* PolicyHandler::LoadPolicyLibrary() {
  return LoadPolicyLibrary("./");
}

PolicyManager* PolicyHandler::LoadPolicyLibrary(const std::string& path) {
  std::string filename = path + kLibrary;
  dl_handle_ = dlopen(filename.c_str(), RTLD_LAZY);

  char* error_string = dlerror();
  if (error_string == NULL) {
    policy_manager_ = CreateManager();
    policy_manager_->set_listener(this);
#if defined (EXTENDED_POLICY)
    exchange_handler_ = new PTExchangeHandlerExt(this);
#else
    exchange_handler_ = new PTExchangeHandlerImpl(this);
#endif
  } else {
    LOG4CXX_ERROR(logger_, error_string);
  }

  return policy_manager_;
}

PolicyManager* PolicyHandler::CreateManager() {
  typedef PolicyManager* (*CreateManager)();
  CreateManager create_manager = 0;
  *(void**)(&create_manager) = dlsym(dl_handle_, "CreateManager");
  char* error_string = dlerror();
  if (error_string == NULL) {
    policy_manager_ = (*create_manager)();
  } else {
    LOG4CXX_WARN(logger_, error_string);
  }
  return policy_manager_;
}

bool PolicyHandler::InitPolicyTable() {
  std::string preloaded_file =
    profile::Profile::instance()->preloaded_pt_file();
  DCHECK(policy_manager_);
  return policy_manager_->LoadPTFromFile(preloaded_file);
}

bool PolicyHandler::RevertPolicyTable() {
  LOG4CXX_INFO(logger_, "Removing user consent records in policy table.");
  DCHECK(policy_manager_);
  return policy_manager()->ResetUserConsent();
}

uint32_t PolicyHandler::GetAppIdForSending() {
  // Get app.list
  const ApplicationList app_list =
    application_manager::ApplicationManagerImpl::instance()->applications();

  if (app_list.empty()) {
    return 0;
  }

  // Choose application
  uint32_t selected_app_id = 0;
  AppIds app_ids_last_resort;
  AppIds app_ids_preferred;

  ApplicationList::const_iterator it_app_list = app_list.begin();
  ApplicationList::const_iterator it_app_list_end = app_list.end();
  for (; it_app_list != it_app_list_end; ++it_app_list) {
    switch ((*it_app_list)->hmi_level()) {
      case mobile_apis::HMILevel::HMI_NONE:
        app_ids_last_resort.push_back((*it_app_list)->app_id());
        break;
      default:
        app_ids_preferred.push_back((*it_app_list)->app_id());
        break;
    }
  }

  AppIds& app_ids_to_use =
    app_ids_preferred.empty() ? app_ids_last_resort : app_ids_preferred;

  // Checking, if some of currently known apps was not used already
  std::sort(last_used_app_ids_.begin(), last_used_app_ids_.end());
  std::sort(app_ids_to_use.begin(), app_ids_to_use.end());

  bool is_all_used = std::includes(last_used_app_ids_.begin(),
                                   last_used_app_ids_.end(),
                                   app_ids_to_use.begin(),
                                   app_ids_to_use.end());

  if (is_all_used) {
    last_used_app_ids_.clear();
  }

  // Leave only unused apps
  AppIds::iterator it_apps_to_use = app_ids_to_use.begin();
  AppIds::iterator it_apps_to_use_end = app_ids_to_use.end();

  AppIds::const_iterator it_last_used_app_ids =
    last_used_app_ids_.begin();
  AppIds::const_iterator it_last_used_app_ids_end =
    last_used_app_ids_.end();

  for (; it_last_used_app_ids != it_last_used_app_ids_end;
       ++it_last_used_app_ids) {

    std::remove(it_apps_to_use, it_apps_to_use_end,
                *it_last_used_app_ids);
  }

  // Random selection of filtered apps
  std::srand(time(0));
  selected_app_id = *(app_ids_to_use.begin()
                      + (rand() % app_ids_to_use.size()));

  last_used_app_ids_.push_back(selected_app_id);
  return selected_app_id;
}

DeviceConsent PolicyHandler::GetDeviceForSending(DeviceParams& device_params) {
  uint32_t app_id = 0;
  uint32_t app_id_previous = 0;
  while (true) {
    app_id = GetAppIdForSending();
    if (!app_id) {
      LOG4CXX_WARN(logger_,
                   "There is no appropriate application for sending PTS.");
      return kDeviceDisallowed;
    }

    // If only one application is available, return its device params
    if (app_id == app_id_previous) {
      return kDeviceDisallowed;
    }

    app_id_previous = app_id;

    char buffer[16];
    snprintf(buffer, 16, "%d", app_id);
    application_manager::MessageHelper::GetDeviceInfoForApp(std::string(buffer),
                                                            &device_params);

    DeviceConsent consent =
        policy_manager_->GetUserConsentForDevice(device_params.device_mac_address);
    switch (consent) {
    case kDeviceAllowed:
      return consent;
    case kDeviceDisallowed:
      continue;
    case kDeviceHasNoConsent:
      return consent;
    default:
      LOG4CXX_WARN(logger_, "Consent result is not impelemented.");
      return consent;
    }
  }
  return kDeviceDisallowed;
}

void PolicyHandler::SetDeviceInfo(std::string& device_id,
                                  const DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "SetDeviceInfo");
  policy_manager_->SetDeviceInfo(device_id, device_info);
}

void PolicyHandler::OnAppPermissionConsent(
    const PermissionConsent& permissions) {
  LOG4CXX_INFO(logger_, "OnAppPermissionConsent");
  DCHECK(policy_manager_);
  if (!permissions.policy_app_id.empty()) {
    policy_manager_->SetUserConsentForApp(permissions);
  }

  //TODO(AOleynik): Handle situation for all apps (policy_app_id is empty)
}

void PolicyHandler::OnGetUserFriendlyMessage(
    const std::vector<std::string>& message_codes, const std::string& language,
    uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnGetUserFriendlyMessage");
  std::vector<UserFriendlyMessage> result =
      policy_manager_->GetUserFriendlyMessages(message_codes, language);
  // Send response to HMI with gathered data
  application_manager::MessageHelper::SendGetUserFriendlyMessageResponse(
        result, correlation_id);
}

void PolicyHandler::OnGetListOfPermissions(const uint32_t connection_key,
                                           const uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnGetListOfPermissions");
  // Get policy_app_id
  std::string policy_app_id;
  const ApplicationList app_list =
    application_manager::ApplicationManagerImpl::instance()->applications();
  ApplicationList::const_iterator it = app_list.begin();
  ApplicationList::const_iterator it_end = app_list.end();
  for (; it != it_end; ++it) {
    if ((*(*it)).app_id() == connection_key) {
      policy_app_id = (*(*it)).mobile_app_id()->asString();
      break;
    }
  }

  DeviceParams device_params;
  char buffer[16];
  snprintf(buffer, 16, "%d", connection_key);
  application_manager::MessageHelper::GetDeviceInfoForApp(std::string(buffer),
                                                          &device_params);
  std::vector<FunctionalGroupPermission> group_permissions;
  if (device_params.device_mac_address.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find device, which hosts application.");
  } else if (policy_app_id.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find application to get permissions.");
  } else {
   policy_manager_->GetUserPermissionsForApp(device_params.device_mac_address,
                                             policy_app_id, group_permissions);
  }

  application_manager::MessageHelper::SendGetListOfPermissionsResponse(
        group_permissions, correlation_id);
}

void PolicyHandler::OnAppRevoked(const std::string& policy_app_id) {
  LOG4CXX_INFO(logger_, "OnAppRevoked");
  const ApplicationList app_list =
    application_manager::ApplicationManagerImpl::instance()->applications();
  ApplicationList::const_iterator it = app_list.begin();
  ApplicationList::const_iterator it_end = app_list.end();
  for (; it != it_end; ++it) {
    if ((*(*it)).mobile_app_id()->asString() == policy_app_id) {
      LOG4CXX_INFO(logger_,
                   "Application_id " << policy_app_id << " is revoked.");
      AppPermissions permissions(atoi(policy_app_id.c_str()));
      permissions.appRevoked = true;
      application_manager::MessageHelper::SendOnAppPermissionsChangedNotification(
        (*it)->app_id(), permissions);
      application_manager::ApplicationManagerImpl::instance()
      ->DeactivateApplication(*it);
      (*(*it)).set_hmi_level(mobile_apis::HMILevel::HMI_NONE);
      return;
    }
  }
  LOG4CXX_WARN(logger_,
               "Application_id " << policy_app_id << " is not registered.");
}

bool PolicyHandler::SendMessageToSDK(const BinaryMessage& pt_string) {
  LOG4CXX_INFO(logger_, "PolicyHandler::SendMessageToSDK");

  is_exchange_in_progress_ = true;

  std::string url;
  uint32_t app_id = last_used_app_ids_.back();
  if (policy_manager_) {
    const std::string& mobile_app_id =
      application_manager::ApplicationManagerImpl::instance()->application(
        app_id)->mobile_app_id()->asString();
    url = policy_manager_->GetUpdateUrl(mobile_app_id);
  }
  LOG4CXX_INFO(
    logger_,
    "Update url is " << url << " for application " << application_manager::ApplicationManagerImpl::instance()-> application(app_id)->name());

  application_manager::MessageHelper::SendPolicySnapshotNotification(app_id,
      pt_string,
      url, 0);
  return true;
}

bool PolicyHandler::ReceiveMessageFromSDK(const BinaryMessage& pt_string) {
  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    return false;
  }

  is_exchange_in_progress_ = false;

  bool ret = policy_manager_->LoadPT(pt_string);
  LOG4CXX_INFO(logger_, "Policy table is saved: " << std::boolalpha << ret);
  if (ret) {
    LOG4CXX_INFO(logger_, "PTU was successful.");
    retry_sequence_lock_.Ackquire();
    retry_sequence_.stop();
    retry_sequence_lock_.Release();
    int32_t correlation_id =
      application_manager::ApplicationManagerImpl::instance()
      ->GetNextHMICorrelationID();
    event_observer_ = new PolicyEventObserver(policy_manager_);
    event_observer_.get()->subscribe_on_event(
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData, correlation_id);
    application_manager::MessageHelper::CreateGetDeviceData(correlation_id);
    // TODO(KKolodiy): when we must reset counter of ignition cyles, update days
    // and kms?
  }
  return ret;
}

bool PolicyHandler::UnloadPolicyLibrary() {
  bool ret = true;
  delete policy_manager_;
  policy_manager_ = 0;
  if (dl_handle_) {
    ret = (dlclose(dl_handle_) == 0);
    dl_handle_ = 0;
  }
  return ret;
}

void PolicyHandler::StartPTExchange(bool skip_device_selection) {
  LOG4CXX_INFO(logger_, "PolicyHandler::StartPTExchange");
  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    return;
  }

  if (is_exchange_in_progress_) {
    LOG4CXX_INFO(logger_, "Starting exchange skipped, since another exchange "
                 "is in progress.");
    return;
  }

  if (!skip_device_selection) {
    DeviceParams device_params;
    DeviceConsent consent = GetDeviceForSending(device_params);
    switch (consent) {
    case kDeviceHasNoConsent:
      // Send OnSDLConsentNeeded to HMI for user consent on device usage
      pending_device_handles_.push_back(device_params.device_handle);
      application_manager::MessageHelper::SendOnSDLConsentNeeded(device_params);
      return;
    case kDeviceDisallowed:
      return;
    default:
      break;
    }
  }

  // TODO(KKolodiy): when we must reset counter of ignition cyles,
  // update days and kms?

  retry_sequence_lock_.Ackquire();
  retry_sequence_.stop();
  policy_manager_->ResetRetrySequence();
  retry_sequence_.start();
  retry_sequence_lock_.Release();
}

void PolicyHandler::StartNextRetry() {
  DCHECK(exchange_handler_);
  exchange_handler_->StartExchange();
}

void PolicyHandler::OnAllowSDLFunctionalityNotification(bool is_allowed,
    uint32_t device_id) {
  LOG4CXX_INFO(logger_, "OnAllowSDLFunctionalityNotification");
  if (device_id) {
    DeviceParams device_params;
    application_manager::MessageHelper::GetDeviceInfoForHandle(device_id,
                                                               &device_params);
    policy_manager_->SetUserConsentForDevice(device_params.device_mac_address,
                                             is_allowed);

    DeviceHandles::iterator it = std::find(pending_device_handles_.begin(),
                                           pending_device_handles_.end(),
                                           device_id);
    // If consent done from HMI menu
    if (it == pending_device_handles_.end()) {
      return;
    }

    pending_device_handles_.erase(it);
    if (is_allowed) {
      // Skip device selection, since user already consented device usage
      StartPTExchange(true);
    }

    return;
  }

  // TODO(AOleynik): Handle situation, if general functionality is concerned
}

void PolicyHandler::OnIgnitionCycleOver() {
  LOG4CXX_INFO(logger_, "OnIgnitionCycleOver");
  policy_manager_->IncrementIgnitionCycles();
}

void PolicyHandler::KmsChanged(int kms) {
  LOG4CXX_INFO(logger_, "PolicyHandler::KmsChanged " << kms << " kilometers");
  PTExchangeAtOdometer(kms);
}

void PolicyHandler::OnActivateApp(const std::string& policy_app_id,
                                  uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnActivateApp");
  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    return;
  }
  // Should be gathered:
  // - isSDLAllowed, i.e. device data usage consent

  AppPermissions permissions(atoi(policy_app_id.c_str()));

  application_manager::UsageStatistics& usage =
      application_manager::ApplicationManagerImpl::instance()->application(
          permissions.application_id)->usage_report();

  usage.RecordAppUserSelection();

  DeviceConsent consent = GetDeviceForSending(permissions.deviceInfo);
  permissions.isSDLAllowed = kDeviceAllowed == consent ? true : false;
  permissions.appRevoked = policy_manager_->IsApplicationRevoked(policy_app_id);

  if (permissions.appRevoked) {
    usage.RecordRunAttemptsWhileRevoked();
  }

  // If isSDLAllowed is false, we should provide device params for user consent
  if (!permissions.isSDLAllowed) {
    pending_device_handles_.push_back(permissions.deviceInfo.device_handle);
  }

  // TODO(AOleynik): Fill following:
  // - isPermissionsConsentNeeded, i.e. specific application permissions
  // - isAppPermissionsRevoked
  // - isAppRevoked

  application_manager::MessageHelper::SendActivateAppResponse(permissions,
      correlation_id);
}

void PolicyHandler::PTExchangeAtIgnition() {
  LOG4CXX_INFO(logger_, "PTExchangeAtIgnition");
  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    return;
  }

  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  int days = current_time.tv_sec / kSecondsInDay;

  // Start update on limits exhaustion, if update wasn't started already by any
  // other event
  LOG4CXX_INFO(
    logger_,
    "\nIgnition cycles exceeded: " << std::boolalpha << policy_manager_->ExceededIgnitionCycles() << "\nDays exceeded: " << std::boolalpha << policy_manager_->ExceededDays(days) << "\nStatusUpdateRequired: " << std::boolalpha << (policy_manager_->GetPolicyTableStatus() == StatusUpdateRequired));
  if (policy_manager_->ExceededIgnitionCycles()
      || policy_manager_->ExceededDays(days)
      || policy_manager_->GetPolicyTableStatus() == StatusUpdateRequired) {
    StartPTExchange();
  }
}

void PolicyHandler::PTExchangeAtOdometer(int kilometers) {
  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    return;
  }
  if (policy_manager_->ExceededKilometers(kilometers)) {
    LOG4CXX_INFO(logger_, "Enough kilometers passed to send for PT update.");
    StartPTExchange();
  }
}

void PolicyHandler::OnPTExchangeNeeded() {
  StartPTExchange();
}

void PolicyHandler::OnPermissionsUpdated(const std::string& policy_app_id,
    const Permissions& permissions) {
  const ApplicationList app_list =
    application_manager::ApplicationManagerImpl::instance()->applications();

  // Find appropriate connection_key for application_id
  ApplicationList::const_iterator it_app_list = app_list.begin();
  ApplicationList::const_iterator it_app_list_end = app_list.end();
  for (; it_app_list != it_app_list_end; ++it_app_list) {
    if (policy_app_id.compare((*it_app_list)->mobile_app_id()->asString())
        == 0) {
      application_manager::MessageHelper::SendOnPermissionsChangeNotification(
        (*it_app_list)->app_id(), permissions);

      LOG4CXX_INFO(
        logger_,
        "Notification sent for application_id:" << policy_app_id << " and connection_key " << (*it_app_list)->app_id());

      return;
    }
  }

  LOG4CXX_WARN(logger_,
               "Connection_key not found for application_id:" << policy_app_id);
}

void PolicyHandler::CheckAppPolicyState(const std::string& application_id) {
  LOG4CXX_INFO(logger_, "CheckAppPolicyState");
  policy_manager()->CheckAppPolicyState(application_id);
}

}  //  namespace policy
