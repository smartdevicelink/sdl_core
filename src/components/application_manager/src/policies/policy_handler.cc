/*
 Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/policies/delegates/app_permission_delegate.h"

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

using namespace application_manager;

#define POLICY_LIB_CHECK(return_value) {\
  sync_primitives::AutoReadLock lock(policy_manager_lock_); \
  if (!policy_manager_) {\
    LOG4CXX_DEBUG(logger_, "The shared library of policy is not loaded");\
    return return_value;\
  }\
}

#define POLICY_LIB_CHECK_VOID() {\
  sync_primitives::AutoReadLock lock(policy_manager_lock_); \
  if (!policy_manager_) {\
    LOG4CXX_DEBUG(logger_, "The shared library of policy is not loaded");\
    return;\
  }\
}

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyHandler")


struct ApplicationListHmiLevelSorter {
  bool operator() (const application_manager::ApplicationSharedPtr& lhs,
                   const application_manager::ApplicationSharedPtr& rhs) {
    if (lhs && rhs) {
      mobile_apis::HMILevel::eType lhs_hmi_level = lhs->hmi_level();
      mobile_apis::HMILevel::eType rhs_hmi_level = rhs->hmi_level();

      if (lhs_hmi_level == rhs_hmi_level) {
        return lhs->app_id() < rhs->app_id();
      }
      return lhs_hmi_level < rhs_hmi_level;
    }
    return false;
  }
};

typedef std::set<application_manager::ApplicationSharedPtr, ApplicationListHmiLevelSorter>
HmiLevelOrderedApplicationList;

struct DeactivateApplication {
    explicit DeactivateApplication(
      const connection_handler::DeviceHandle& device_id)
      : device_id_(device_id) {}

    void operator()(const ApplicationSharedPtr& app) {
      if (device_id_ == app->device()) {
        if (mobile_api::HMILevel::HMI_NONE != app->hmi_level()) {
          ApplicationManagerImpl::instance()->ChangeAppsHMILevel(app->app_id(),
                                                                 mobile_apis::HMILevel::HMI_NONE);
          app->set_audio_streaming_state(mobile_api::AudioStreamingState::NOT_AUDIBLE);
          MessageHelper::SendActivateAppToHMI(
                app->app_id(), hmi_apis::Common_HMILevel::NONE);
          MessageHelper::SendHMIStatusNotification(*app.get());
        }
      }
    }

  private:
    connection_handler::DeviceHandle device_id_;
};

struct SDLAlowedNotification {
  SDLAlowedNotification(const connection_handler::DeviceHandle& device_id,
                        PolicyManager* policy_manager)
    : device_id_(device_id),
      policy_manager_(policy_manager){}

  void operator()(const ApplicationSharedPtr& app) {
    if (!policy_manager_) {
      return;
    }
    if (device_id_ == app->device()) {
        std::string hmi_level;
        hmi_apis::Common_HMILevel::eType default_hmi;
        mobile_apis::HMILevel::eType default_mobile_hmi;
        policy_manager_->GetDefaultHmi(app->mobile_app_id(), &hmi_level);
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
          LOG4CXX_DEBUG(logger_, "Application already in default hmi state.");
        } else {
          ApplicationManagerImpl::instance()->ChangeAppsHMILevel(app->app_id(),
                                                                 default_mobile_hmi);
          MessageHelper::SendHMIStatusNotification(*app);
        }
        MessageHelper::SendActivateAppToHMI(app->app_id(), default_hmi);
      }
    }
  private:
    connection_handler::DeviceHandle device_id_;
    PolicyManager* policy_manager_;
};

struct LinkAppToDevice {
  explicit LinkAppToDevice(
        std::map<std::string, std::string>& app_to_device_link)
    : app_to_device_link_(app_to_device_link) {
    app_to_device_link_.clear();
  }

  void operator()(const ApplicationSharedPtr& app) {
    if (!app.valid()) {
      LOG4CXX_WARN(logger_, "Invalid pointer to application was passed."
                   "Skip current application.");
      return;
    }
    DeviceParams device_params;
    MessageHelper::GetDeviceInfoForApp(
          app->app_id(),
          &device_params);
    const std::string app_id = app->mobile_app_id();
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

  : AsyncRunner("PolicyHandler async runner thread"),
    dl_handle_(0),
    last_activated_app_id_(0),
    app_to_device_link_lock_(true),
    statistic_manager_impl_(new StatisticManagerImpl()) {
}

PolicyHandler::~PolicyHandler() {

}

bool PolicyHandler::LoadPolicyLibrary() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoWriteLock lock(policy_manager_lock_);
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
      event_observer_= new PolicyEventObserver(this);
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
  return policy_manager_.valid();
}

bool PolicyHandler::InitPolicyTable() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);
  // Subscribing to notification for system readiness to be able to get system
  // info necessary for policy table
  event_observer_->subscribe_on_event(
        hmi_apis::FunctionID::BasicCommunication_OnReady);
  std::string preloaded_file =
    profile::Profile::instance()->preloaded_pt_file();
  if (file_system::FileExists(preloaded_file)) {
    return policy_manager_->InitPT(preloaded_file);
  }
  LOG4CXX_FATAL(logger_, "The file which contains preloaded PT is not exist");
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
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);
  return policy_manager_->ResetUserConsent();
}

uint32_t PolicyHandler::GetAppIdForSending() {
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  HmiLevelOrderedApplicationList app_list(accessor.begin(), accessor.end());

  LOG4CXX_INFO(logger_, "Apps size: " << app_list.size());

  DeviceParams device_param;
  for (HmiLevelOrderedApplicationList::const_iterator first = app_list.begin();
       first != app_list.end(); ++first) {
    if ((*first)->IsRegistered()) {
      const uint32_t app_id = (*first)->app_id();
      MessageHelper::GetDeviceInfoForApp(app_id, &device_param);
      if (kDeviceAllowed ==
          policy_manager_->GetUserConsentForDevice(device_param.device_mac_address)) {
        return app_id;
      }
    }
  }

  return 0;
}

void PolicyHandler::OnAppPermissionConsent(const uint32_t connection_key,
                                           const PermissionConsent& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  AsyncRun(new AppPermissionDelegate(connection_key, permissions));
}

void PolicyHandler::OnDeviceConsentChanged(const std::string& device_id,
                                           bool is_allowed) {
  POLICY_LIB_CHECK_VOID();
  connection_handler::DeviceHandle device_handle;
  ApplicationManagerImpl::instance()->connection_handler()
      ->GetDeviceID(device_id, &device_handle);
  // In case of changed consent for device, related applications will be
  // limited to pre_DataConsent permissions, if device disallowed, or switch
  // back to their own permissions, if device allowed again, and must be
  // notified about these changes

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  ApplicationManagerImpl::ApplictionSetConstIt it_app_list =
      accessor.begin();
  ApplicationManagerImpl::ApplictionSetConstIt it_app_list_end =
      accessor.end();
  for (; it_app_list != it_app_list_end; ++it_app_list) {
    if (device_handle == (*it_app_list).get()->device()) {

      const std::string policy_app_id =
          (*it_app_list)->mobile_app_id();

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

void PolicyHandler::OnPTExchangeNeeded() {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->ForcePTExchange();
}

void PolicyHandler::GetAvailableApps(std::queue<std::string>& apps) {
  LOG4CXX_INFO(logger_, "GetAvailable apps");
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const ApplicationManagerImpl::ApplictionSet app_list = accessor.applications();
  ApplicationManagerImpl::ApplictionSetConstIt iter = app_list.begin();

  for (;app_list.end() != iter; ++iter) {
    LOG4CXX_INFO(logger_, "one more app");
    apps.push((*iter)->mobile_app_id());
  }
}

void PolicyHandler::AddApplication(const std::string& application_id) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->AddApplication(application_id);
}

void PolicyHandler::SetDeviceInfo(std::string& device_id,
                                  const DeviceInfo& device_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetDeviceInfo(device_id, device_info);
}

void PolicyHandler::OnAppPermissionConsentInternal(
    const uint32_t connection_key, PermissionConsent &permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  if (connection_key) {
    ApplicationSharedPtr app =
        ApplicationManagerImpl::instance()
        ->application(connection_key);

    if (app.valid()) {
      permissions.policy_app_id = app->mobile_app_id();
      policy::DeviceParams device_params;
      MessageHelper::GetDeviceInfoForHandle(
        app->device(),
        &device_params);

      permissions.device_id = device_params.device_mac_address;
    }

    if (!permissions.policy_app_id.empty()) {
      policy_manager_->SetUserConsentForApp(permissions);
    }

    return;
  }

  sync_primitives::AutoLock lock(app_to_device_link_lock_);
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
    ApplicationSharedPtr app =
        ApplicationManagerImpl::instance()->
        application_by_policy_id(it->first);

    // If list of apps sent to HMI for user consents is not the same as current,
    // permissions should be set only for coincident to registered apps
    if (!app.valid()) {
      LOG4CXX_WARN(logger_, "Invalid pointer to application was passed."
                   "Permissions setting skipped.");
      continue;
    }

    policy::DeviceParams device_params;
    MessageHelper::GetDeviceInfoForHandle(
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
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  std::vector<UserFriendlyMessage> result = policy_manager_
      ->GetUserFriendlyMessages(message_codes, language);
  // Send response to HMI with gathered data
  MessageHelper::SendGetUserFriendlyMessageResponse(
    result, correlation_id);
}

void PolicyHandler::OnGetListOfPermissions(const uint32_t connection_key,
    const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  // If no specific app was passed, get permissions for all currently registered
  // applications
  if (!connection_key) {
    sync_primitives::AutoLock lock(app_to_device_link_lock_);
    LinkAppToDevice linker(app_to_device_link_);
    ApplicationManagerImpl::ApplicationListAccessor accessor;
    ApplicationManagerImpl::ApplictionSetConstIt it_app
        = accessor.begin();
    ApplicationManagerImpl::ApplictionSetConstIt it_app_end
        = accessor.end();

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

    MessageHelper::SendGetListOfPermissionsResponse(
      consolidator.GetConsolidatedPermissions(), correlation_id);

    return;
  }

  // Single app only
  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()->application(
      connection_key);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Connection key '" << connection_key << "' "
                 "not found within registered applications.");
    return;
  }

  DeviceParams device_params;
  MessageHelper::GetDeviceInfoForApp(connection_key,
      &device_params);
  std::vector<FunctionalGroupPermission> group_permissions;
  if (device_params.device_mac_address.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find device, which hosts application.");
  } else if (!app) {
    LOG4CXX_WARN(logger_, "Couldn't find application to get permissions.");
  } else {
    policy_manager_->GetUserConsentForApp(device_params.device_mac_address,
                                          app->mobile_app_id(),
                                          group_permissions);

    MessageHelper::SendGetListOfPermissionsResponse(
      group_permissions, correlation_id);
  }
}

void PolicyHandler::OnGetStatusUpdate(const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  MessageHelper::SendGetStatusUpdateResponse(
    policy_manager_->GetPolicyTableStatus(), correlation_id);
}

void PolicyHandler::OnUpdateStatusChanged(const std::string& status) {
  LOG4CXX_AUTO_TRACE(logger_);
  MessageHelper::SendOnStatusUpdate(status);
}

std::string PolicyHandler::OnCurrentDeviceIdUpdateRequired(
  const std::string& policy_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Application with id '" << policy_app_id << "' "
                 "not found within registered applications.");
    return "";
  }
  DeviceParams device_param;
  MessageHelper::GetDeviceInfoForApp(app->app_id(),
      &device_param);
  return device_param.device_mac_address;
}

void PolicyHandler::OnSystemInfoChanged(const std::string& language) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetSystemLanguage(language);
}

void PolicyHandler::OnGetSystemInfo(const std::string& ccpu_version,
                                    const std::string& wers_country_code,
                                    const std::string& language) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetSystemInfo(ccpu_version, wers_country_code, language);
}

void PolicyHandler::OnSystemInfoUpdateRequired() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  MessageHelper::SendGetSystemInfoRequest();
}

void PolicyHandler::OnVIIsReady() {
  const uint32_t correlation_id =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  std::vector<std::string> params;
  params.push_back(strings::vin);

  MessageHelper::CreateGetVehicleDataRequest(
        correlation_id, params);

}

void PolicyHandler::OnVehicleDataUpdated(
    const smart_objects::SmartObject& message) {
  POLICY_LIB_CHECK_VOID();
}

void PolicyHandler::OnPendingPermissionChange(
    const std::string& policy_app_id) {
  LOG4CXX_DEBUG(logger_, "PolicyHandler::OnPendingPermissionChange for "
               << policy_app_id);
  POLICY_LIB_CHECK_VOID();
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()
      ->application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_,
                 "No app found for " << policy_app_id << " policy app id.");
    return;
  }

  AppPermissions permissions = policy_manager_->GetAppPermissionsChanges(
                                 policy_app_id);

  const uint32_t app_id = app->app_id();

  using mobile_apis::HMILevel::eType;

  if (permissions.appRevoked) {
    application_manager::MessageHelper::SendOnAppPermissionsChangedNotification(
      app_id, permissions);

    ApplicationManagerImpl::instance()->ChangeAppsHMILevel(app->app_id(),
                                                           eType::HMI_NONE);
    app->set_audio_streaming_state(mobile_apis::AudioStreamingState::NOT_AUDIBLE);
    application_manager::MessageHelper::SendActivateAppToHMI(
          app_id, hmi_apis::Common_HMILevel::NONE);
    application_manager::MessageHelper::SendHMIStatusNotification(*app);
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
    return;
  }

  eType app_hmi_level = app->hmi_level();

  switch (app_hmi_level) {
  case eType::HMI_FULL:
  case eType::HMI_LIMITED: {
    if (permissions.appPermissionsConsentNeeded) {
      MessageHelper::
          SendOnAppPermissionsChangedNotification(app->app_id(), permissions);

      policy_manager_->RemovePendingPermissionChanges(policy_app_id);
    }
    break;
  }
  case eType::HMI_BACKGROUND: {
    if (permissions.isAppPermissionsRevoked) {
      MessageHelper::
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
      MessageHelper::
          SendOnAppPermissionsChangedNotification(app->app_id(), permissions);
    }
    MessageHelper::
        SendOnAppInterfaceUnregisteredNotificationToMobile(
          app->app_id(),
          mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED);

    ApplicationManagerImpl::instance()->OnAppUnauthorized(app->app_id());

    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }
}

bool PolicyHandler::SendMessageToSDK(const BinaryMessage& pt_string,
                                     const std::string& url) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);

  if (last_used_app_ids_.empty()) {
    LOG4CXX_WARN(logger_, "last_used_app_ids_ is empty");
    return false;
  }
  uint32_t app_id = last_used_app_ids_.back();

  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()->application(app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "There is no registered application with "
                 "connection key '" << app_id << "'");
    return false;
  }

  const std::string& mobile_app_id = app->mobile_app_id();
  if (mobile_app_id.empty()) {
    LOG4CXX_WARN(logger_, "Application with connection key '" << app_id << "'"
                 " has no application id.");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "Update url is " << url << " for application "
               << ApplicationManagerImpl::instance()
               ->application(app_id)->name());

  MessageHelper::SendPolicySnapshotNotification(
        app_id, pt_string, url, 0);

  return true;
}

bool PolicyHandler::ReceiveMessageFromSDK(const std::string& file,
    const BinaryMessage& pt_string) {
  POLICY_LIB_CHECK(false);

  bool ret = policy_manager_->LoadPT(file, pt_string);
  LOG4CXX_INFO(logger_, "Policy table is saved: " << std::boolalpha << ret);
  if (ret) {
    LOG4CXX_INFO(logger_, "PTU was successful.");
    policy_manager_->CleanupUnpairedDevices();
    int32_t correlation_id =
      ApplicationManagerImpl::instance()
      ->GetNextHMICorrelationID();

    event_observer_->subscribe_on_event(
#ifdef HMI_DBUS_API
      hmi_apis::FunctionID::VehicleInfo_GetOdometer, correlation_id
#else
      hmi_apis::FunctionID::VehicleInfo_GetVehicleData, correlation_id
#endif
    );
    std::vector<std::string> vehicle_data_args;
    vehicle_data_args.push_back(strings::odometer);
    MessageHelper::CreateGetVehicleDataRequest(
          correlation_id, vehicle_data_args);
  } else  {
    LOG4CXX_WARN(logger_, "Exchange wasn't successful, trying another one.");
    OnPTExchangeNeeded();
  }
  return ret;
}

bool PolicyHandler::UnloadPolicyLibrary() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "policy_manager_ = " << policy_manager_);
  bool ret = true;
  AsyncRunner::Stop();
  sync_primitives::AutoWriteLock lock(policy_manager_lock_);
  if (policy_manager_) {
    policy_manager_.reset();
  }
  if (dl_handle_) {
    ret = (dlclose(dl_handle_) == 0);
    dl_handle_ = 0;
  }
  LOG4CXX_TRACE(logger_, "exit");
  return ret;
}

void PolicyHandler::OnAllowSDLFunctionalityNotification(bool is_allowed,
    uint32_t device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  // Device ids, need to be changed
  std::set<uint32_t> device_ids;
  bool device_specific = device_id != 0;
  // Common devices consents change
  if (!device_specific) {
    ApplicationManagerImpl::ApplicationListAccessor accessor;
    const ApplicationManagerImpl::ApplictionSet app_list = accessor.applications();

    ApplicationManagerImpl::ApplictionSetConstIt it_app_list = app_list.begin();
    ApplicationManagerImpl::ApplictionSetConstIt it_app_end = app_list.end();

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
    MessageHelper::GetDeviceInfoForHandle(device_id,
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

}

void PolicyHandler::OnIgnitionCycleOver() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->IncrementIgnitionCycles();
}

void PolicyHandler::OnActivateApp(uint32_t connection_key,
                                  uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()->application(
      connection_key);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Activated App failed: no app found.");
    return;
  }
  std::string policy_app_id = app->mobile_app_id();

  AppPermissions permissions(policy_app_id);

  sync_primitives::AutoReadLock lock(policy_manager_lock_);
  if (!policy_manager_) {
    LOG4CXX_DEBUG(logger_, "The shared library of policy is not loaded");
    if (!PolicyEnabled()) {
      permissions.isSDLAllowed = true;
    }
  } else {
    permissions = policy_manager_->GetAppPermissionsChanges(
                    policy_app_id);

    permissions.isSDLAllowed = true;
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }
  // If application is revoked it should not be activated
  // In this case we need to activate application
  if (false == permissions.appRevoked && true == permissions.isSDLAllowed) {
        LOG4CXX_INFO(logger_, "Application will be activated");
        if (ApplicationManagerImpl::instance()->ActivateApplication(app)) {
          MessageHelper::SendHMIStatusNotification(*(app.get()));
          last_activated_app_id_ = 0;
        }
  } else {
    LOG4CXX_INFO(logger_, "Application should not be activated");
  }

  MessageHelper::SendSDLActivateAppResponse(permissions, correlation_id);
}

void PolicyHandler::KmsChanged(int kilometers) {
  LOG4CXX_DEBUG(logger_, "PolicyHandler::KmsChanged " << kilometers << " kilometers");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->KmsChanged(kilometers);
}

void PolicyHandler::PTExchangeAtUserRequest(uint32_t correlation_id) {
  LOG4CXX_TRACE(logger_, "PT exchange at user request");
  POLICY_LIB_CHECK_VOID();
  std::string update_status = policy_manager_->ForcePTExchange();
  MessageHelper::SendUpdateSDLResponse(update_status,
                                                            correlation_id);
}

void PolicyHandler::OnPermissionsUpdated(const std::string& policy_app_id,
                                         const Permissions& permissions,
                                         const HMILevel& default_hmi) {
  LOG4CXX_AUTO_TRACE(logger_);
  OnPermissionsUpdated(policy_app_id, permissions);

  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(
      logger_,
      "Connection_key not found for application_id:" << policy_app_id);
    return;
  }

  // The application currently not running (i.e. in NONE) should change HMI
  // level to default
  mobile_apis::HMILevel::eType current_hmi_level = app->hmi_level();
  mobile_apis::HMILevel::eType hmi_level =
    MessageHelper::StringToHMILevel(default_hmi);

  if (mobile_apis::HMILevel::INVALID_ENUM == hmi_level) {
    LOG4CXX_WARN(logger_, "Couldn't convert default hmi level "
                 << default_hmi << " to enum.");
    return;
  }
  if (current_hmi_level == hmi_level) {
    LOG4CXX_DEBUG(logger_, "Application already in default hmi state.");
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
        MessageHelper::SendActivateAppToHMI(app->app_id());
      } else {
        // Set application hmi level
        ApplicationManagerImpl::instance()->ChangeAppsHMILevel(app->app_id(),
                                                               hmi_level);
        // If hmi Level is full, it will be seted after ActivateApp response
        MessageHelper::SendHMIStatusNotification(*app.get());
      }
      break;
    }
    default:
      LOG4CXX_WARN(logger_, "Application " << policy_app_id << " is running."
                   "HMI level won't be changed.");
      break;
  }
}

void PolicyHandler::OnPermissionsUpdated(const std::string& policy_app_id,
                                         const Permissions& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(
      logger_,
      "Connection_key not found for application_id:" << policy_app_id);
    return;
  }

  MessageHelper::SendOnPermissionsChangeNotification(
    app->app_id(), permissions);

  LOG4CXX_DEBUG(
    logger_,
    "Notification sent for application_id:" << policy_app_id
    << " and connection_key " << app->app_id());
}

bool PolicyHandler::SaveSnapshot(const BinaryMessage& pt_string,
                                 std::string& snap_path) {
  using namespace profile;

  const std::string& policy_snapshot_file_name = Profile::instance()->policies_snapshot_file_name();
  const std::string& system_files_path = Profile::instance()->system_files_path();
  snap_path = system_files_path + '/' + policy_snapshot_file_name;

  bool result = false;
  if (file_system::CreateDirectoryRecursively(system_files_path)) {
    result = file_system::WriteBinaryFile(snap_path, pt_string);
  }

  if (!result) {
    LOG4CXX_ERROR(logger_, "Failed to write snapshot file to " << snap_path);
  }

  return result;
}

void PolicyHandler::OnSnapshotCreated(const BinaryMessage& pt_string,
                                      const std::vector<int>& retry_delay_seconds,
                                      int timeout_exchange) {
  SendMessageToSDK(pt_string, policy_manager_->GetUpdateUrl(POLICY));
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

void PolicyHandler::GetUpdateUrls(int service_type, EndpointUrls& end_points) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->GetUpdateUrls(service_type, end_points);
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

void PolicyHandler::OnSystemReady() {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnSystemReady();
}

void PolicyHandler::PTUpdatedAt(int kilometers, int days_after_epoch) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->PTUpdatedAt(kilometers, days_after_epoch);
}

void PolicyHandler::add_listener(PolicyHandlerObserver* listener) {
  sync_primitives::AutoLock lock(listeners_lock_);
  listeners_.push_back(listener);
}

void PolicyHandler::remove_listener(PolicyHandlerObserver* listener) {
  sync_primitives::AutoLock lock(listeners_lock_);
  listeners_.remove(listener);
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
  ApplicationSharedPtr app =
    ApplicationManagerImpl::instance()
    ->application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(
      logger_,
      "Connection_key not found for application_id:" << policy_app_id);
    return "";
  }
  return  app->name();
}

void PolicyHandler::OnUpdateHMIAppType(std::map<std::string, StringArray> app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    (*it)->OnUpdateHMIAppType(app_hmi_types);
  }
}

void PolicyHandler::OnCertificateUpdated(const std::string& certificate_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    (*it)->OnCertificateUpdated(certificate_data);
  }
}

bool PolicyHandler::CanUpdate() {
  return 0 != GetAppIdForSending();
}

void PolicyHandler::RemoveDevice(const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();

  policy_manager_->MarkUnpairedDevice(device_id);

}

bool PolicyHandler::IsApplicationRevoked(const std::string& app_id) {
  LOG4CXX_TRACE(logger_, "PolicyHandler::IsApplicationRevoked");
  POLICY_LIB_CHECK(false);

  return policy_manager_->IsApplicationRevoked(app_id);
}

void PolicyHandler::OnUpdateRequestSentToMobile() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnUpdateStarted();
}

bool PolicyHandler::CheckKeepContext(const std::string& policy_app_id) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->CanAppKeepContext(policy_app_id);
}

bool PolicyHandler::CheckStealFocus(const std::string& policy_app_id) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->CanAppStealFocus(policy_app_id);
}

bool PolicyHandler::CheckSystemAction(
    mobile_apis::SystemAction::eType system_action,
    const std::string& policy_app_id) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);
  switch (system_action) {
    case SystemAction::STEAL_FOCUS:
      return CheckStealFocus(policy_app_id);
    case SystemAction::KEEP_CONTEXT:
      return CheckKeepContext(policy_app_id);
    case SystemAction::DEFAULT_ACTION:
      return true;
    default:
      break;
  }
  LOG4CXX_DEBUG(logger_, "Unknown system action");
  return false;
}

uint16_t PolicyHandler::HeartBeatTimeout(const std::string& app_id) const {
  POLICY_LIB_CHECK(0);
  return policy_manager_->HeartBeatTimeout(app_id);
}

const std::string PolicyHandler::RemoteAppsUrl() const {
  const std::string default_url = "";
  POLICY_LIB_CHECK(default_url);
  return policy_manager_->RemoteAppsUrl();
}

void policy::PolicyHandler::OnAppsSearchStarted() {
  POLICY_LIB_CHECK();
  policy_manager_->OnAppsSearchStarted();
}

void policy::PolicyHandler::OnAppsSearchCompleted() {
  POLICY_LIB_CHECK();
  policy_manager_->OnAppsSearchCompleted();
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

