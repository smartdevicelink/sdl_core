﻿/*
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
#include <vector>
#include "application_manager/smart_object_keys.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/pt_exchange_handler_impl.h"
#include "application_manager/policies/pt_exchange_handler_ext.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "policy/policy_manager_impl.h"
#include "connection_handler/connection_handler.h"
#include "utils/macro.h"
#include "utils/date_time.h"
#include "json/value.h"
#include "json/writer.h"
#include "config_profile/profile.h"
#include "application_manager/usage_statistics.h"
#include "policy/policy_types.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"

namespace policy {

#define POLICY_LIB_CHECK(return_value) {\
  if (!policy_manager_) {\
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");\
    return return_value;\
  }\
}

#define POLICY_LIB_CHECK_VOID() {\
  if (!policy_manager_) {\
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");\
    return;\
  }\
}

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyHandler")

typedef std::set<application_manager::ApplicationSharedPtr> ApplicationList;

struct DeactivateApplication {
    explicit DeactivateApplication(
      const connection_handler::DeviceHandle& device_id)
      : device_id_(device_id) {}

    void operator()(const application_manager::ApplicationSharedPtr& app) {
      if (device_id_ == app->device()) {
        app->set_hmi_level(mobile_apis::HMILevel::HMI_NONE);
        application_manager::MessageHelper::SendActivateAppToHMI(
          app->app_id(), hmi_apis::Common_HMILevel::NONE);
      }
    }

  private:
    connection_handler::DeviceHandle device_id_;
};

struct SDLAlowedNotification {
  explicit SDLAlowedNotification(
    const connection_handler::DeviceHandle& device_id,
        utils::SharedPtr<PolicyManager> policy_manager)
    : device_id_(device_id),
      policy_manager_(policy_manager){}

  void operator()(const application_manager::ApplicationSharedPtr& app) {
    if (!policy_manager_) {
      return;
    }
    if (device_id_ == app->device()) {
        std::string hmi_level;
        hmi_apis::Common_HMILevel::eType default_hmi;
        mobile_apis::HMILevel::eType default_mobile_hmi;
        policy_manager_->GetDefaultHmi(app->mobile_app_id()->asString(), &hmi_level);
        if ("BACKGROUND" == hmi_level) {
          default_hmi = hmi_apis::Common_HMILevel::BACKGROUND;
          default_mobile_hmi = mobile_apis::HMILevel::HMI_BACKGROUND;
        } else if ("FULL" == hmi_level) {
          default_hmi = hmi_apis::Common_HMILevel::FULL;
          default_mobile_hmi = mobile_apis::HMILevel::HMI_FULL;
        } else if ("LIMITED" == hmi_level) {
          default_hmi = hmi_apis::Common_HMILevel::LIMITED;
          default_mobile_hmi = mobile_apis::HMILevel::HMI_LIMITED;
        } else if ("NONE" == hmi_level) {
          default_hmi = hmi_apis::Common_HMILevel::NONE;
          default_mobile_hmi = mobile_apis::HMILevel::HMI_NONE;
        } else {
          return ;
        }
        if (app->hmi_level() == default_mobile_hmi) {
          LOG4CXX_INFO(logger_, "Application already in default hmi state.");
        } else {
          app->set_hmi_level(default_mobile_hmi);
          application_manager::MessageHelper::SendHMIStatusNotification(*app);
        }
        application_manager::MessageHelper::SendActivateAppToHMI(app->app_id(), default_hmi);
      }
    }
  private:
    connection_handler::DeviceHandle device_id_;
    utils::SharedPtr<PolicyManager> policy_manager_;
};

struct LinkAppToDevice {
  explicit LinkAppToDevice(
        std::map<std::string, std::string>& app_to_device_link)
    : app_to_device_link_(app_to_device_link) {
    app_to_device_link_.clear();
  }

  void operator()(const application_manager::ApplicationSharedPtr& app) {
    if (!app.valid()) {
      LOG4CXX_WARN(logger_, "Invalid pointer to application was passed."
                   "Skip current application.");
      return;
    }
    DeviceParams device_params;
    application_manager::MessageHelper::GetDeviceInfoForApp(
          app->app_id(),
          &device_params);
    const std::string app_id = app->mobile_app_id()->asString();
    if (device_params.device_mac_address.empty()) {
      LOG4CXX_WARN(logger_, "Couldn't find device, which hosts application "
                   << app_id);
      return;
    }
    app_to_device_link_[app_id] = device_params.device_mac_address;
  }

private:
  std::map<std::string, std::string>& app_to_device_link_;
};

struct PermissionsConsolidator {
  void Consolidate(
      const std::vector<policy::FunctionalGroupPermission>& permissions) {
    std::vector<policy::FunctionalGroupPermission>::const_iterator it =
        permissions.begin();
    std::vector<policy::FunctionalGroupPermission>::const_iterator it_end =
        permissions.end();
    for (;it != it_end; ++it) {
      std::vector<policy::FunctionalGroupPermission>::iterator it_consolidated =
          std::find(consolidated_permissions_.begin(),
                    consolidated_permissions_.end(),
                    *it);

      // If found, consolidate states
      if (consolidated_permissions_.end() != it_consolidated) {
        if (policy::kGroupAllowed == it->state ||
            policy::kGroupAllowed == it_consolidated->state) {
          it_consolidated->state = policy::kGroupAllowed;
        } else if (policy::kGroupDisallowed == it->state ||
                   policy::kGroupDisallowed == it_consolidated->state) {
          it_consolidated->state = policy::kGroupDisallowed;
        }
        continue;
      }

      // If not found, add permissions
      consolidated_permissions_.push_back(*it);
    }
  }

  const std::vector<policy::FunctionalGroupPermission>&
  GetConsolidatedPermissions() const {
    return consolidated_permissions_;
  }

private:
  std::vector<policy::FunctionalGroupPermission> consolidated_permissions_;
};

PolicyHandler* PolicyHandler::instance_ = NULL;
const std::string PolicyHandler::kLibrary = "libPolicy.so";

PolicyHandler::PolicyHandler()

  : dl_handle_(0),
// EXTENDED_POLICY
    exchange_handler_(new PTExchangeHandlerImpl(this)),
    on_ignition_check_done_(false),
    last_activated_app_id_(0),
    registration_in_progress(false),
    is_user_requested_policy_table_update_(false),
    statistic_manager_impl_(new StatisticManagerImpl()) {
}

PolicyHandler::~PolicyHandler() {

}

bool PolicyHandler::LoadPolicyLibrary() {
  if (!PolicyEnabled()) {
    LOG4CXX_WARN(logger_, "System is configured to work without policy "
                 "functionality.");
    policy_manager_ = NULL;
    return NULL;
  }
  dl_handle_ = dlopen(kLibrary.c_str(), RTLD_LAZY);

  char* error_string = dlerror();
  if (error_string == NULL) {
    if (CreateManager()) {
      policy_manager_->set_listener(this);
      event_observer_= new PolicyEventObserver(policy_manager_);
    }
  } else {
    LOG4CXX_ERROR(logger_, error_string);
  }

  return policy_manager_.valid();
}

bool PolicyHandler::PolicyEnabled() {
  return profile::Profile::instance()->enable_policy();
}

bool PolicyHandler::CreateManager() {
  typedef PolicyManager* (*CreateManager)();
  CreateManager create_manager = reinterpret_cast<CreateManager>(dlsym(dl_handle_, "CreateManager"));
  char* error_string = dlerror();
  if (error_string == NULL) {
    policy_manager_ = create_manager();
  } else {
    LOG4CXX_WARN(logger_, error_string);
  }
  return policy_manager_;
}

bool PolicyHandler::InitPolicyTable() {
  LOG4CXX_TRACE(logger_, "Init policy table from preloaded.");
  POLICY_LIB_CHECK(false);
  // Subscribing to notification for system readiness to be able to get system
  // info necessary for policy table
  event_observer_.get()->subscribe_on_event(
        hmi_apis::FunctionID::BasicCommunication_OnReady);
  std::string preloaded_file =
    profile::Profile::instance()->preloaded_pt_file();
  if (file_system::FileExists(preloaded_file)) {
    return policy_manager_->InitPT(preloaded_file);
  }
  LOG4CXX_WARN(logger_, "The file which contains preloaded PT is not exist");
  return false;
}

bool PolicyHandler::ResetPolicyTable() {
  LOG4CXX_TRACE(logger_, "Reset policy table.");
  POLICY_LIB_CHECK(false);
  std::string preloaded_file =
    profile::Profile::instance()->preloaded_pt_file();
  if (file_system::FileExists(preloaded_file)) {
    return policy_manager_->ResetPT(preloaded_file);
  }
  LOG4CXX_WARN(logger_, "The file which contains preloaded PT is not exist");
  return false;
}

bool PolicyHandler::ClearUserConsent() {
  LOG4CXX_INFO(logger_, "Removing user consent records in policy table.");
  POLICY_LIB_CHECK(false);
  return policy_manager_->ResetUserConsent();
}

uint32_t PolicyHandler::GetAppIdForSending() {
  // Get app.list
  application_manager::ApplicationManagerImpl::ApplicationListAccessor accessor;
  const ApplicationList app_list = accessor.applications();

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

  AppIds::const_iterator it_last_used_app_ids = last_used_app_ids_.begin();
  AppIds::const_iterator it_last_used_app_ids_end = last_used_app_ids_.end();

  for (; it_last_used_app_ids != it_last_used_app_ids_end;
       ++it_last_used_app_ids) {

    std::remove(it_apps_to_use, it_apps_to_use_end, *it_last_used_app_ids);
  }

  // Random selection of filtered apps
  std::srand(time(0));
  selected_app_id =
    *(app_ids_to_use.begin() + (rand() % app_ids_to_use.size()));

  last_used_app_ids_.push_back(selected_app_id);
  return selected_app_id;
}

DeviceConsent PolicyHandler::GetDeviceForSending(DeviceParams& device_params) {
  POLICY_LIB_CHECK(kDeviceDisallowed);
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
    application_manager::MessageHelper::GetDeviceInfoForApp(app_id,
        &device_params);

    DeviceConsent consent = policy_manager_->GetUserConsentForDevice(
                              device_params.device_mac_address);
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

const std::string PolicyHandler::ConvertUpdateStatus(PolicyTableStatus status) {
  switch (status) {
    case policy::StatusUpdatePending:
      return "UPDATING";
    case policy::StatusUpdateRequired:
      return "UPDATE_NEEDED";
    case policy::StatusUpToDate:
      return "UP_TO_DATE";
    default: {
      return "UNKNOWN";
    }
  }
}

void PolicyHandler::OnDeviceConsentChanged(const std::string& device_id,
                                           bool is_allowed) {
  POLICY_LIB_CHECK_VOID();
  connection_handler::DeviceHandle device_handle;
  application_manager::ApplicationManagerImpl::instance()->connection_handler()
      ->GetDeviceID(device_id, &device_handle);
  // In case of changed consent for device, related applications will be
  // limited to pre_DataConsent permissions, if device disallowed, or switch
  // back to their own permissions, if device allowed again, and must be
  // notified about these changes

  application_manager::ApplicationManagerImpl::ApplicationListAccessor accessor;
  ApplicationList app_list = accessor.applications();
  ApplicationList::const_iterator it_app_list = app_list.begin();
  ApplicationList::const_iterator it_app_list_end = app_list.end();
  for (; it_app_list != it_app_list_end; ++it_app_list) {
    if (device_handle == (*it_app_list).get()->device()) {

      const std::string policy_app_id =
          (*it_app_list)->mobile_app_id()->asString();

      // If app has predata policy, which is assigned without device consent or
      // with negative data consent, there no necessity to change smth and send
      // notification for such app in case of device consent is not allowed
      if (policy_manager_->IsPredataPolicy(policy_app_id) &&
          !is_allowed) {
        continue;
      }

      policy_manager_->ReactOnUserDevConsentForApp(policy_app_id,
                                                   is_allowed);

      policy_manager_->SendNotificationOnPermissionsUpdated(policy_app_id);
    }
  }
}

void PolicyHandler::AddApplication(const std::string& application_id) {
  // TODO (AGaliuzov): remove this workaround during refactoring.
  registration_in_progress = true;
  POLICY_LIB_CHECK_VOID();
  policy_manager_->AddApplication(application_id);
}

void PolicyHandler::SetDeviceInfo(std::string& device_id,
                                  const DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "SetDeviceInfo");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetDeviceInfo(device_id, device_info);
}

void PolicyHandler::OnAppPermissionConsent(const uint32_t connection_key,
  PermissionConsent &permissions) {
  LOG4CXX_INFO(logger_, "OnAppPermissionConsent");
  POLICY_LIB_CHECK_VOID();
  if (connection_key) {
    application_manager::ApplicationSharedPtr app =
        application_manager::ApplicationManagerImpl::instance()
        ->application(connection_key);

    if (app.valid()) {
      permissions.policy_app_id = app->mobile_app_id()->asString();
      policy::DeviceParams device_params;
      application_manager::MessageHelper::GetDeviceInfoForHandle(
        app->device(),
        &device_params);

      permissions.device_id = device_params.device_mac_address;
    }

    if (!permissions.policy_app_id.empty()) {
      policy_manager_->SetUserConsentForApp(permissions);
    }

    return;
  }

  if (!app_to_device_link_.size()) {
    LOG4CXX_WARN(logger_, "There are no applications previously stored for "
                 "setting common permissions.");
    return;
  }

  std::map<std::string, std::string>::const_iterator it =
      app_to_device_link_.begin();
  std::map<std::string, std::string>::const_iterator it_end =
      app_to_device_link_.end();
  for (;it != it_end; ++it) {
    application_manager::ApplicationSharedPtr app =
        application_manager::ApplicationManagerImpl::instance()->
        application_by_policy_id(it->first);

    // If list of apps sent to HMI for user consents is not the same as current,
    // permissions should be set only for coincident to registered apps
    if (!app.valid()) {
      LOG4CXX_WARN(logger_, "Invalid pointer to application was passed."
                   "Permissions setting skipped.");
      continue;
    }

    policy::DeviceParams device_params;
    application_manager::MessageHelper::GetDeviceInfoForHandle(
      app->device(),
      &device_params);

    if (device_params.device_mac_address != it->second) {
      LOG4CXX_WARN(logger_, "Device_id of application is changed."
                   "Permissions setting skipped.");
      continue;
    }

    permissions.policy_app_id = it->first;
    permissions.device_id = it->second;
    policy_manager_->SetUserConsentForApp(permissions);
  }
}

void PolicyHandler::OnGetUserFriendlyMessage(
  const std::vector<std::string>& message_codes, const std::string& language,
  uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnGetUserFriendlyMessage");
  POLICY_LIB_CHECK_VOID();
  std::vector<UserFriendlyMessage> result = policy_manager_
      ->GetUserFriendlyMessages(message_codes, language);
  // Send response to HMI with gathered data
  application_manager::MessageHelper::SendGetUserFriendlyMessageResponse(
    result, correlation_id);
}

void PolicyHandler::OnGetListOfPermissions(const uint32_t connection_key,
    const uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnGetListOfPermissions");
  POLICY_LIB_CHECK_VOID();
  // If no specific app was passed, get permissions for all currently registered
  // applications
  if (!connection_key) {
    LinkAppToDevice linker(app_to_device_link_);
    application_manager::ApplicationManagerImpl::ApplicationListAccessor accessor;
    const ApplicationList app_list = accessor.applications();
    std::set<application_manager::ApplicationSharedPtr>::const_iterator it_app =
        app_list.begin();
    std::set<application_manager::ApplicationSharedPtr>::const_iterator
        it_app_end = app_list.end();

    // Add all currently registered applications
    std::for_each(it_app, it_app_end, linker);

    PermissionsConsolidator consolidator;
    std::vector<policy::FunctionalGroupPermission> group_permissions;
    std::map<std::string, std::string>::const_iterator it =
        app_to_device_link_.begin();
    for (;it != app_to_device_link_.end(); ++it) {
      policy_manager_->GetUserConsentForApp(it->second, it->first,
                                            group_permissions);
      consolidator.Consolidate(group_permissions);
    }

    application_manager::MessageHelper::SendGetListOfPermissionsResponse(
      consolidator.GetConsolidatedPermissions(), correlation_id);

    return;
  }

  // Single app only
  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()->application(
      connection_key);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Connection key '" << connection_key << "' "
                 "not found within registered applications.");
    return;
  }

  DeviceParams device_params;
  application_manager::MessageHelper::GetDeviceInfoForApp(connection_key,
      &device_params);
  std::vector<FunctionalGroupPermission> group_permissions;
  if (device_params.device_mac_address.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find device, which hosts application.");
  } else if (!app) {
    LOG4CXX_WARN(logger_, "Couldn't find application to get permissions.");
  } else {
    policy_manager_->GetUserConsentForApp(device_params.device_mac_address,
                                          app->mobile_app_id()->asString(),
                                          group_permissions);

    application_manager::MessageHelper::SendGetListOfPermissionsResponse(
      group_permissions, correlation_id);
  }
}

void PolicyHandler::OnGetStatusUpdate(const uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnGetStatusUpdate");
  POLICY_LIB_CHECK_VOID();
  policy::PolicyTableStatus status = policy_manager_->GetPolicyTableStatus();
  application_manager::MessageHelper::SendGetStatusUpdateResponse(
    ConvertUpdateStatus(status), correlation_id);
}

void PolicyHandler::OnUpdateStatusChanged(PolicyTableStatus status) {
  LOG4CXX_INFO(logger_, "OnUpdateStatusChanged");
  application_manager::MessageHelper::SendOnStatusUpdate(
    ConvertUpdateStatus(status));
}

std::string PolicyHandler::OnCurrentDeviceIdUpdateRequired(
  const std::string& policy_app_id) {
  LOG4CXX_INFO(logger_, "OnCurrentDeviceIdUpdateRequired");
  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Application with id '" << policy_app_id << "' "
                 "not found within registered applications.");
    return "";
  }
  DeviceParams device_param;
  application_manager::MessageHelper::GetDeviceInfoForApp(app->app_id(),
      &device_param);
  return device_param.device_mac_address;
}

void PolicyHandler::OnSystemInfoChanged(const std::string& language) {
  LOG4CXX_INFO(logger_, "OnSystemInfoChanged");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetSystemLanguage(language);
}

void PolicyHandler::OnGetSystemInfo(const std::string& ccpu_version,
                                    const std::string& wers_country_code,
                                    const std::string& language) {
  LOG4CXX_INFO(logger_, "OnGetSystemInfo");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetSystemInfo(ccpu_version, wers_country_code, language);
}

void PolicyHandler::OnSystemInfoUpdateRequired() {
  LOG4CXX_INFO(logger_, "OnSystemInfoUpdateRequired");
  POLICY_LIB_CHECK_VOID();
  application_manager::MessageHelper::SendGetSystemInfoRequest();
}

void PolicyHandler::OnVIIsReady() {
  const uint32_t correlation_id = application_manager::
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  std::vector<std::string> params;
  params.push_back(application_manager::strings::vin);

  application_manager::MessageHelper::CreateGetVehicleDataRequest(
        correlation_id, params);

}

void PolicyHandler::OnVehicleDataUpdated(
    const smart_objects::SmartObject& message) {
  POLICY_LIB_CHECK_VOID();
}

void PolicyHandler::OnAppRevoked(const std::string& policy_app_id) {
  LOG4CXX_TRACE(logger_, "OnAppRevoked with policy_app_id " << policy_app_id << " is revoked.");
  POLICY_LIB_CHECK_VOID();
  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);
  if (app.valid()) {
    DeviceParams device_params;
    application_manager::MessageHelper::GetDeviceInfoForApp(app->app_id(),
                                                            &device_params);
    AppPermissions permissions = policy_manager_->GetAppPermissionsChanges(
                                   device_params.device_mac_address,
                                   policy_app_id);
    permissions.appRevoked = true;
    application_manager::MessageHelper::SendOnAppPermissionsChangedNotification(
      app->app_id(), permissions);
    app->set_hmi_level(mobile_apis::HMILevel::HMI_NONE);
    application_manager::MessageHelper::SendActivateAppToHMI(
          app->app_id(), hmi_apis::Common_HMILevel::NONE);
    application_manager::MessageHelper::SendHMIStatusNotification(*app);
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
    return;
  }
}

void PolicyHandler::OnPendingPermissionChange(
    const std::string& policy_app_id) {
  LOG4CXX_INFO(logger_, "PolicyHandler::OnPendingPermissionChange for "
               << policy_app_id);
  POLICY_LIB_CHECK_VOID();
  application_manager::ApplicationSharedPtr app =
      application_manager::ApplicationManagerImpl::instance()
      ->application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_,
                 "No app found for " << policy_app_id << " policy app id.");
    return;
  }

  DeviceParams device_params;
  application_manager::MessageHelper::GetDeviceInfoForApp(app->app_id(),
                                                          &device_params);
  AppPermissions permissions = policy_manager_->GetAppPermissionsChanges(
                                 device_params.device_mac_address,
                                 policy_app_id);

  mobile_apis::HMILevel::eType app_hmi_level = app->hmi_level();

  switch (app_hmi_level) {
  case mobile_apis::HMILevel::HMI_FULL:
  case mobile_apis::HMILevel::HMI_LIMITED: {
    if (permissions.appPermissionsConsentNeeded) {
      application_manager::MessageHelper::
          SendOnAppPermissionsChangedNotification(app->app_id(), permissions);

      policy_manager_->RemovePendingPermissionChanges(policy_app_id);
    }
    break;
  }
  case mobile_apis::HMILevel::HMI_BACKGROUND: {
    if (permissions.isAppPermissionsRevoked) {
      application_manager::MessageHelper::
          SendOnAppPermissionsChangedNotification(app->app_id(), permissions);

      policy_manager_->RemovePendingPermissionChanges(policy_app_id);      
    }
    break;
  }
  default:
    break;
  }

  if (permissions.appUnauthorized) {
    if (mobile_apis::HMILevel::HMI_FULL == app_hmi_level ||
        mobile_apis::HMILevel::HMI_LIMITED == app_hmi_level) {
      application_manager::MessageHelper::
          SendOnAppPermissionsChangedNotification(app->app_id(), permissions);
    }
    application_manager::MessageHelper::
        SendOnAppInterfaceUnregisteredNotificationToMobile(
          app->app_id(),
          mobile_apis::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED);

    application_manager::ApplicationManagerImpl::instance()->
        UnregisterRevokedApplication(app->app_id(),
                                     mobile_apis::Result::INVALID_ENUM);

    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }
}

bool PolicyHandler::SendMessageToSDK(const BinaryMessage& pt_string) {
  LOG4CXX_INFO(logger_, "PolicyHandler::SendMessageToSDK");
  POLICY_LIB_CHECK(false);

  std::string url;
  uint32_t app_id = last_used_app_ids_.back();

  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application(app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "There is no registered application with "
                 "connection key '" << app_id << "'");
    return false;
  }

  const std::string& mobile_app_id = app->mobile_app_id()->asString();
  if (mobile_app_id.empty()) {
    LOG4CXX_WARN(logger_, "Application with connection key '" << app_id << "'"
                 " has no application id.");
    return false;
  }
  url = policy_manager_->GetUpdateUrl(PolicyServiceTypes::POLICY);

  LOG4CXX_INFO(logger_, "Update url is " << url << " for application "
               << application_manager::ApplicationManagerImpl::instance()
               ->application(app_id)->name());

  application_manager::MessageHelper::SendPolicySnapshotNotification(app_id,
      pt_string,
      url, 0);

  return true;
}

bool PolicyHandler::ReceiveMessageFromSDK(const std::string& file,
    const BinaryMessage& pt_string) {
  POLICY_LIB_CHECK(false);

  if (policy_manager_->GetPolicyTableStatus() !=
      PolicyTableStatus::StatusUpdatePending) {
    LOG4CXX_WARN(logger_, "PTU processing skipped, since current status is "
                          "different from pending.");
    return false;
  }

  bool ret = policy_manager_->LoadPT(file, pt_string);
  LOG4CXX_INFO(logger_, "Policy table is saved: " << std::boolalpha << ret);
  if (ret) {
    LOG4CXX_INFO(logger_, "PTU was successful.");
    exchange_handler_->Stop();
    policy_manager_->CleanupUnpairedDevices();
    int32_t correlation_id =
      application_manager::ApplicationManagerImpl::instance()
      ->GetNextHMICorrelationID();

    event_observer_.get()->subscribe_on_event(
#ifdef HMI_DBUS_API
      hmi_apis::FunctionID::VehicleInfo_GetOdometer, correlation_id
#else
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData, correlation_id
#endif
    );
    std::vector<std::string> vehicle_data_args;
    vehicle_data_args.push_back(application_manager::strings::odometer);
    application_manager::MessageHelper::CreateGetVehicleDataRequest(
          correlation_id, vehicle_data_args);
  } else  {
    LOG4CXX_WARN(logger_, "Exchange wasn't successful, trying another one.");
    OnPTExchangeNeeded();
  }
  return ret;
}

bool PolicyHandler::UnloadPolicyLibrary() {
  LOG4CXX_TRACE(logger_, "enter. policy_manager_ = " << policy_manager_);
  bool ret = true;
  if (policy_manager_) {
    policy_manager_.release();
  }
  if (dl_handle_) {
    ret = (dlclose(dl_handle_) == 0);
    dl_handle_ = 0;
  }
  exchange_handler_->Stop();
  LOG4CXX_TRACE(logger_, "exit");
  return ret;
}

void PolicyHandler::StartPTExchange(bool skip_device_selection) {
  LOG4CXX_INFO(logger_, "PolicyHandler::StartPTExchange");
  POLICY_LIB_CHECK_VOID();

  if (policy_manager_->GetPolicyTableStatus() ==
      PolicyTableStatus::StatusUpdatePending) {
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
        application_manager::MessageHelper::SendOnSDLConsentNeeded(
          device_params);
        return;
      case kDeviceDisallowed:
        return;
      default:
        break;
    }
  }

  exchange_handler_->Start();
}

void PolicyHandler::OnAllowSDLFunctionalityNotification(bool is_allowed,
    uint32_t device_id) {
  LOG4CXX_INFO(logger_, "OnAllowSDLFunctionalityNotification");
  POLICY_LIB_CHECK_VOID();
  // Device ids, need to be changed
  std::set<uint32_t> device_ids;
  bool device_specific = device_id != 0;
  // Common devices consents change
  if (!device_specific) {
    application_manager::ApplicationManagerImpl::ApplicationListAccessor accessor;
    const std::set<application_manager::ApplicationSharedPtr> app_list =
        accessor.applications();

    std::set<application_manager::ApplicationSharedPtr>::const_iterator
        it_app_list = app_list.begin();
    std::set<application_manager::ApplicationSharedPtr>::const_iterator
        it_app_end = app_list.end();

    for (;it_app_list != it_app_end; ++it_app_list) {
      if (!(*it_app_list).valid()) {
        continue;
      }
      device_ids.insert(it_app_list->get()->device());
    }
  } else {
    device_ids.insert(device_id);
  }

  std::set<uint32_t>::const_iterator it_ids = device_ids.begin();
  std::set<uint32_t>::const_iterator it_ids_end = device_ids.end();
  for (;it_ids != it_ids_end; ++it_ids) {
    const uint32_t device_id = *it_ids;

    DeviceParams device_params;
    application_manager::MessageHelper::GetDeviceInfoForHandle(device_id,
        &device_params);
    device_params.device_handle = device_id;
    if (kDefaultDeviceMacAddress == device_params.device_mac_address) {
      LOG4CXX_WARN(logger_, "Device with handle " << device_id
                   << " wasn't found.");
      return;
    }
    policy_manager_->SetUserConsentForDevice(device_params.device_mac_address,
        is_allowed);

  }

  // Case, when specific device was changed
  if (device_id) {
    DeviceHandles::iterator it = std::find(pending_device_handles_.begin(),
                                           pending_device_handles_.end(),
                                           device_id);
    // If consent done from HMI menu
    if (it == pending_device_handles_.end()) {
      return;
    }

    pending_device_handles_.erase(it);
  }

// EXTENDED_POLICY
  // Skip device selection, since user already consented device usage
  StartPTExchange(true);
}

void PolicyHandler::OnIgnitionCycleOver() {
  LOG4CXX_INFO(logger_, "OnIgnitionCycleOver");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->IncrementIgnitionCycles();
}

void PolicyHandler::KmsChanged(int kms) {
  LOG4CXX_INFO(logger_, "PolicyHandler::KmsChanged " << kms << " kilometers");
  POLICY_LIB_CHECK_VOID();
  PTExchangeAtOdometer(kms);
}

void PolicyHandler::OnActivateApp(uint32_t connection_key,
                                  uint32_t correlation_id) {
  LOG4CXX_INFO(logger_, "OnActivateApp");

  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()->application(
      connection_key);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Activated App failed: no app found.");
    return;
  }
  std::string policy_app_id = app->mobile_app_id()->asString();

  AppPermissions permissions(policy_app_id);

  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "The shared library of policy is not loaded");
    if (!PolicyEnabled()) {
      permissions.isSDLAllowed = true;
    }
  } else {
    DeviceParams device_params;
    application_manager::MessageHelper::GetDeviceInfoForApp(connection_key,
                                                            &device_params);
    permissions = policy_manager_->GetAppPermissionsChanges(
                    device_params.device_mac_address,
                    policy_app_id);

    permissions.isSDLAllowed = true;

    if (permissions.isSDLAllowed &&
        PolicyTableStatus::StatusUpdateRequired ==
        policy_manager_->GetPolicyTableStatus()) {
      StartPTExchange();
    }
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }

  bool is_app_activated = false;
  // If application is revoked it should not be activated
  // In this case we need to activate application
  if (false == permissions.appRevoked && true == permissions.isSDLAllowed) {
    is_app_activated =
        application_manager::ApplicationManagerImpl::instance()->
        ActivateApplication(app);
  }

  last_activated_app_id_ = connection_key;
  application_manager::MessageHelper::SendSDLActivateAppResponse(permissions,
                                                              correlation_id);
  if (is_app_activated) {
    application_manager::MessageHelper::SendHMIStatusNotification(*app.get());
  }
}

void PolicyHandler::PTExchangeAtRegistration(const std::string& app_id) {
  LOG4CXX_INFO(logger_, "PTExchangeAtIgnition");
  POLICY_LIB_CHECK_VOID();

  if (policy_manager_->IsAppInUpdateList(app_id)) {
    StartPTExchange();
  } else if (false == on_ignition_check_done_) { // TODO(AG): add cond. var to handle this case.
    TimevalStruct current_time = date_time::DateTime::getCurrentTime();
    const int kSecondsInDay = 60 * 60 * 24;
    int days = current_time.tv_sec / kSecondsInDay;

    LOG4CXX_INFO(
      logger_,
      "\nIgnition cycles exceeded: " << std::boolalpha <<
      policy_manager_->ExceededIgnitionCycles()
      << "\nDays exceeded: " << std::boolalpha
      << policy_manager_->ExceededDays(days)
      << "\nStatusUpdateRequired: " << std::boolalpha
      << (policy_manager_->GetPolicyTableStatus() == StatusUpdateRequired));
    if (policy_manager_->ExceededIgnitionCycles()
        || policy_manager_->ExceededDays(days)
        || policy_manager_->GetPolicyTableStatus() == StatusUpdateRequired) {
      StartPTExchange();
    }
  }
  on_ignition_check_done_ = true;
}

void PolicyHandler::PTExchangeAtOdometer(int kilometers) {
  POLICY_LIB_CHECK_VOID();
  if (policy_manager_->ExceededKilometers(kilometers)) {
    LOG4CXX_INFO(logger_, "Enough kilometers passed to send for PT update.");
    StartPTExchange();
  }
}

void PolicyHandler::PTExchangeAtUserRequest(uint32_t correlation_id) {
  LOG4CXX_TRACE(logger_, "PT exchange at user request");
  POLICY_LIB_CHECK_VOID();
  policy::PolicyTableStatus status = policy_manager_->GetPolicyTableStatus();
  if (status != policy::StatusUpdatePending) {
    OnPTExchangeNeeded();
    status = policy::StatusUpdatePending;
  } else {
    is_user_requested_policy_table_update_ = true;
  }
  application_manager::MessageHelper::SendUpdateSDLResponse(
    ConvertUpdateStatus(status), correlation_id);
}

void PolicyHandler::OnPTExchangeNeeded() {
  StartPTExchange();
}

void PolicyHandler::OnPermissionsUpdated(const std::string& policy_app_id,
                                         const Permissions& permissions,
                                         const HMILevel& default_hmi) {
  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(
      logger_,
      "Connection_key not found for application_id:" << policy_app_id);
    return;
  }

  application_manager::MessageHelper::SendOnPermissionsChangeNotification(
    app->app_id(), permissions);

  LOG4CXX_INFO(
    logger_,
    "Notification sent for application_id:" << policy_app_id
    << " and connection_key " << app->app_id());

  // The application currently not running (i.e. in NONE) should change HMI
  // level to default
  mobile_apis::HMILevel::eType current_hmi_level = app->hmi_level();
  mobile_apis::HMILevel::eType hmi_level =
    application_manager::MessageHelper::StringToHMILevel(default_hmi);

  if (mobile_apis::HMILevel::INVALID_ENUM == hmi_level) {
    LOG4CXX_WARN(logger_, "Couldn't convert default hmi level "
                 << default_hmi << " to enum.");
    return;
  }
  if (current_hmi_level == hmi_level) {
    LOG4CXX_INFO(logger_, "Application already in default hmi state.");
    return;
  }
  switch (current_hmi_level) {
    case mobile_apis::HMILevel::HMI_NONE: {
      LOG4CXX_INFO(logger_, "Changing hmi level of application "
                   << policy_app_id
                   << " to default hmi level " << default_hmi);
      // If default is FULL, send request to HMI. Notification to mobile will be
      // sent on response receiving.
      if (mobile_apis::HMILevel::HMI_FULL == hmi_level) {
        application_manager::MessageHelper::SendActivateAppToHMI(app->app_id());
        break;
      }

      // Set application hmi level
      app->set_hmi_level(hmi_level);

      // Send notification to mobile
      application_manager::MessageHelper::SendHMIStatusNotification(*app.get());
      }
      break;
    default:
      LOG4CXX_WARN(logger_, "Application " << policy_app_id << " is running."
                   "HMI level won't be changed.");
      break;
  }
}

bool PolicyHandler::GetPriority(const std::string& policy_app_id,
                                std::string* priority) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetPriority(policy_app_id, priority);
}

void PolicyHandler::CheckPermissions(const PTString& app_id,
                                     const PTString& hmi_level,
                                     const PTString& rpc,
                                     const RPCParams& rpc_params,
                                     CheckPermissionResult& result) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->CheckPermissions(app_id, hmi_level, rpc, rpc_params, result);
}

uint32_t PolicyHandler::GetNotificationsNumber(const std::string& priority) {
  POLICY_LIB_CHECK(0);
 return policy_manager_->GetNotificationsNumber(priority);
}

DeviceConsent PolicyHandler::GetUserConsentForDevice(
    const std::string& device_id) {
  POLICY_LIB_CHECK(kDeviceDisallowed);
  return policy_manager_->GetUserConsentForDevice(device_id);
}

bool PolicyHandler::GetDefaultHmi(const std::string& policy_app_id,
                                  std::string* default_hmi) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetDefaultHmi(policy_app_id, default_hmi);
}

bool PolicyHandler::GetInitialAppData(const std::string& application_id,
                                      StringArray* nicknames,
                                      StringArray* app_hmi_types) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetInitialAppData(application_id, nicknames, app_hmi_types);
}

EndpointUrls PolicyHandler::GetUpdateUrls(int service_type) {
  POLICY_LIB_CHECK(EndpointUrls());
  return policy_manager_->GetUpdateUrls(service_type);
}

std::string PolicyHandler::GetLockScreenIconUrl() const {
  POLICY_LIB_CHECK(std::string(""));
  return policy_manager_->GetLockScreenIconUrl();
}

void PolicyHandler::ResetRetrySequence() {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->ResetRetrySequence();
}

int PolicyHandler::NextRetryTimeout() {
  POLICY_LIB_CHECK(0);
  return policy_manager_->NextRetryTimeout();
}

int PolicyHandler::TimeoutExchange() {
  POLICY_LIB_CHECK(0);
  return policy_manager_->TimeoutExchange();
}

void PolicyHandler::OnExceededTimeout() {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnExceededTimeout();
}

BinaryMessageSptr PolicyHandler::RequestPTUpdate() {
  POLICY_LIB_CHECK(BinaryMessageSptr());
  return policy_manager_->RequestPTUpdate();
}

const std::vector<int> PolicyHandler::RetrySequenceDelaysSeconds() {
  POLICY_LIB_CHECK(std::vector<int>());
  return policy_manager_->RetrySequenceDelaysSeconds();
}

utils::SharedPtr<usage_statistics::StatisticsManager>
PolicyHandler::GetStatisticManager() {
  return statistic_manager_impl_;
}

void PolicyHandler::AddStatisticsInfo(int type) {
  POLICY_LIB_CHECK_VOID();
  switch (static_cast<hmi_apis::Common_StatisticsType::eType>(type)) {
    case hmi_apis::Common_StatisticsType::iAPP_BUFFER_FULL: {
      usage_statistics::GlobalCounter count_of_iap_buffer_full(
        GetStatisticManager(), usage_statistics::IAP_BUFFER_FULL);
      ++count_of_iap_buffer_full;
      break;
    }
    default: {
      LOG4CXX_WARN(logger_, "Type of statistics is unknown");
    }
  }
}

void PolicyHandler::OnSystemError(int code) {
  POLICY_LIB_CHECK_VOID();
  switch (static_cast<hmi_apis::Common_SystemError::eType>(code)) {
    case hmi_apis::Common_SystemError::SYNC_REBOOTED: {
      usage_statistics::GlobalCounter count_of_sync_reboots(
        GetStatisticManager(), usage_statistics::SYNC_REBOOTS);
      ++count_of_sync_reboots;
      break;
    }
    case hmi_apis::Common_SystemError::SYNC_OUT_OF_MEMMORY: {
      usage_statistics::GlobalCounter count_sync_out_of_memory(
        GetStatisticManager(), usage_statistics::SYNC_OUT_OF_MEMORY);
      ++count_sync_out_of_memory;
      break;
    }
    default: {
      LOG4CXX_WARN(logger_, "System error is unknown");
    }
  }
}

std::string PolicyHandler::GetAppName(const std::string& policy_app_id) {
  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(
      logger_,
      "Connection_key not found for application_id:" << policy_app_id);
    return "";
  }
  return  app->name();
}

void PolicyHandler::OnUserRequestedUpdateCheckRequired() {
  LOG4CXX_INFO(logger_, "OnUserRequestedUpdateCheckRequired");
  POLICY_LIB_CHECK_VOID();
  policy::PolicyTableStatus status = policy_manager_->GetPolicyTableStatus();
  if (is_user_requested_policy_table_update_ &&
      status != policy::StatusUpdatePending) {
    is_user_requested_policy_table_update_ = false;
    OnPTExchangeNeeded();
    return;
  }
  LOG4CXX_WARN(logger_, "There is another pending update is present."
               "User-requested update is postponed.");
}

void PolicyHandler::RemoveDevice(const std::string& device_id) {
  LOG4CXX_INFO(logger_, "PolicyHandler::RemoveDevice");
  POLICY_LIB_CHECK_VOID();

  policy_manager_->MarkUnpairedDevice(device_id);

}

bool PolicyHandler::IsApplicationRevoked(const std::string& app_id) {
  LOG4CXX_TRACE(logger_, "PolicyHandler::IsApplicationRevoked");
  POLICY_LIB_CHECK(false);

  return policy_manager_->IsApplicationRevoked(app_id);
}

void PolicyHandler::OnUpdateRequestSentToMobile() {
  LOG4CXX_INFO(logger_, "OnUpdateRequestSentToMobile");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnUpdateStarted();
}

bool PolicyHandler::CheckKeepContext(int system_action,
                                     const std::string& policy_app_id) {
  POLICY_LIB_CHECK(false);
  const bool keep_context = system_action
      == mobile_apis::SystemAction::KEEP_CONTEXT;
  const bool allowed = policy_manager_->CanAppKeepContext(policy_app_id);
  return !(keep_context && !allowed);
}

bool PolicyHandler::CheckStealFocus(int system_action,
                                    const std::string& policy_app_id) {
  POLICY_LIB_CHECK(false);
  const bool steal_focus = system_action
      == mobile_apis::SystemAction::STEAL_FOCUS;
  const bool allowed = policy_manager_->CanAppStealFocus(policy_app_id);
  return !(steal_focus && !allowed);
}

uint16_t PolicyHandler::HeartBeatTimeout(const std::string& app_id) const {
  POLICY_LIB_CHECK(0);
  return policy_manager_->HeartBeatTimeout(app_id);
}

void PolicyHandler::Increment(usage_statistics::GlobalCounterId type) {
  POLICY_LIB_CHECK();
  policy_manager_->Increment(type);
}

void PolicyHandler::Increment(const std::string& app_id, usage_statistics::AppCounterId type) {
  POLICY_LIB_CHECK();
  policy_manager_->Increment(app_id, type);
}

void PolicyHandler::Set(const std::string& app_id,
                        usage_statistics::AppInfoId type,
                        const std::string& value) {
  POLICY_LIB_CHECK();
  policy_manager_->Set(app_id, type, value);
}

void PolicyHandler::Add(const std::string& app_id,
                        usage_statistics::AppStopwatchId type,
                        int32_t timespan_seconds) {
  POLICY_LIB_CHECK();
  policy_manager_->Add(app_id, type, timespan_seconds);
}

}  //  namespace policy

