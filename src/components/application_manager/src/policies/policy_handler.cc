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
#include "application_manager/policies/policy_handler.h"
#include <unistd.h>
#include <dlfcn.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <utility>
#include "application_manager/smart_object_keys.h"
#include "application_manager/policies/delegates/app_permission_delegate.h"
#include "policy/status.h"
#include "application_manager/application_manager.h"
#include "application_manager/state_controller.h"
#include "application_manager/message_helper.h"
#include "policy/policy_manager_impl.h"
#include "connection_handler/connection_handler.h"
#include "utils/macro.h"
#include "utils/date_time.h"
#include "json/value.h"
#include "json/writer.h"
#include "application_manager/usage_statistics.h"
#include "policy/policy_types.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "utils/scope_guard.h"
#include "utils/make_shared.h"
#include "policy/policy_manager.h"
#ifdef SDL_REMOTE_CONTROL
#include "functional_module/plugin_manager.h"
#endif  // SDL_REMOTE_CONTROL

namespace policy {

using namespace application_manager;

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyHandler")

namespace {
using namespace mobile_apis;
typedef std::map<mobile_apis::RequestType::eType, std::string> RequestTypeMap;
RequestTypeMap TypeToString = {
    {mobile_apis::RequestType::INVALID_ENUM, "INVALID_ENUM"},
    {mobile_apis::RequestType::HTTP, "HTTP"},
    {mobile_apis::RequestType::FILE_RESUME, "FILE_RESUME"},
    {mobile_apis::RequestType::AUTH_REQUEST, "AUTH_REQUEST"},
    {mobile_apis::RequestType::AUTH_CHALLENGE, "AUTH_CHALLENGE"},
    {mobile_apis::RequestType::AUTH_ACK, "AUTH_ACK"},
    {mobile_apis::RequestType::PROPRIETARY, "PROPRIETARY"},
    {mobile_apis::RequestType::QUERY_APPS, "QUERY_APPS"},
    {mobile_apis::RequestType::LAUNCH_APP, "LAUNCH_APP"},
    {mobile_apis::RequestType::LOCK_SCREEN_ICON_URL, "LOCK_SCREEN_ICON_URL"},
    {mobile_apis::RequestType::TRAFFIC_MESSAGE_CHANNEL,
     "TRAFFIC_MESSAGE_CHANNEL"},
    {mobile_apis::RequestType::DRIVER_PROFILE, "DRIVER_PROFILE"},
    {mobile_apis::RequestType::VOICE_SEARCH, "VOICE_SEARCH"},
    {mobile_apis::RequestType::NAVIGATION, "NAVIGATION"},
    {mobile_apis::RequestType::PHONE, "PHONE"},
    {mobile_apis::RequestType::CLIMATE, "CLIMATE"},
    {mobile_apis::RequestType::SETTINGS, "SETTINGS"},
    {mobile_apis::RequestType::VEHICLE_DIAGNOSTICS, "VEHICLE_DIAGNOSTICS"},
    {mobile_apis::RequestType::EMERGENCY, "EMERGENCY"},
    {mobile_apis::RequestType::MEDIA, "MEDIA"},
    {mobile_apis::RequestType::FOTA, "FOTA"}};

const std::string RequestTypeToString(mobile_apis::RequestType::eType type) {
  RequestTypeMap::const_iterator it = TypeToString.find(type);
  if (TypeToString.end() != it) {
    return (*it).second;
  }
  return "";
}

const policy::DeviceParams GetDeviceParams(
    connection_handler::DeviceHandle device_handle,
    const protocol_handler::SessionObserver& session_observer) {
  CREATE_LOGGERPTR_LOCAL(logger_, "PolicyHandler")
  policy::DeviceParams device_params;
  if (-1 ==
      session_observer.GetDataOnDeviceID(
          device_handle,
          &device_params.device_name,
          NULL,
          &device_params.device_mac_address,
          &device_params.device_connection_type)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to extract information for device " << device_handle);
  }
  device_params.device_handle = device_handle;
  return device_params;
}

struct HMILevelPredicate
    : public std::unary_function<ApplicationSharedPtr, bool> {
  explicit HMILevelPredicate(const mobile_api::HMILevel::eType level)
      : level_(level) {}

  bool operator()(const ApplicationSharedPtr app) const {
    return level_ == app->hmi_level() ? true : false;
  }

 private:
  mobile_api::HMILevel::eType level_;
};

}  // namespace

#define POLICY_LIB_CHECK(return_value)                                      \
  {                                                                         \
    sync_primitives::AutoReadLock lock(policy_manager_lock_);               \
    if (!policy_manager_) {                                                 \
      LOG4CXX_DEBUG(logger_, "The shared library of policy is not loaded"); \
      return return_value;                                                  \
    }                                                                       \
  }

#define POLICY_LIB_CHECK_VOID()                                             \
  {                                                                         \
    sync_primitives::AutoReadLock lock(policy_manager_lock_);               \
    if (!policy_manager_) {                                                 \
      LOG4CXX_DEBUG(logger_, "The shared library of policy is not loaded"); \
      return;                                                               \
    }                                                                       \
  }

static const std::string kCerficateFileName = "certificate";

struct DeactivateApplication {
  explicit DeactivateApplication(
      const connection_handler::DeviceHandle& device_id,
      StateController& state_ctrl)
      : device_id_(device_id), state_ctrl_(state_ctrl) {}

  void operator()(const ApplicationSharedPtr& app) {
    if (device_id_ == app->device()) {
      state_ctrl_.SetRegularState(
          app,
          mobile_apis::HMILevel::HMI_NONE,
          mobile_apis::AudioStreamingState::NOT_AUDIBLE,
          mobile_apis::VideoStreamingState::NOT_STREAMABLE,
          true);
    }
  }

 private:
  connection_handler::DeviceHandle device_id_;
  StateController& state_ctrl_;
};

struct SDLAllowedNotification {
  SDLAllowedNotification(const connection_handler::DeviceHandle& device_id,
                         PolicyManager* policy_manager,
                         StateController& state_controller)
      : device_id_(device_id)
      , policy_manager_(policy_manager)
      , state_controller_(state_controller) {}

  void operator()(const ApplicationSharedPtr& app) {
    DCHECK_OR_RETURN_VOID(policy_manager_);
    if (device_id_ == app->device()) {
      std::string hmi_level = "NONE";
      mobile_apis::HMILevel::eType default_mobile_hmi;
      policy_manager_->GetDefaultHmi(app->policy_app_id(), &hmi_level);
      if ("BACKGROUND" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_BACKGROUND;
      } else if ("FULL" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_FULL;
      } else if ("LIMITED" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_LIMITED;
      } else if ("NONE" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_NONE;
      } else {
        return;
      }
      state_controller_.SetRegularState(app, default_mobile_hmi, true);
    }
  }

 private:
  connection_handler::DeviceHandle device_id_;
  PolicyManager* policy_manager_;
  StateController& state_controller_;
};

/**
 * @brief Gets from system list of currently registered applications and
 * create collection of links device-to-application
 */
struct LinksCollector {
  LinksCollector(const ApplicationManager& application_manager,
                 std::map<std::string, std::string>& out_app_to_device_link)
      : application_manager_(application_manager)
      , out_app_to_device_link_(out_app_to_device_link) {
    out_app_to_device_link_.clear();
  }

  void operator()(const ApplicationSharedPtr& app) {
    if (!app.valid()) {
      LOG4CXX_WARN(logger_,
                   "Invalid pointer to application was passed."
                   "Skip current application.");
      return;
    }
    DeviceParams device_params = GetDeviceParams(
        app->device(),
        application_manager_.connection_handler().get_session_observer());
    const std::string app_id = app->policy_app_id();
    if (device_params.device_mac_address.empty()) {
      LOG4CXX_WARN(logger_,
                   "Couldn't find device, which hosts application " << app_id);
      return;
    }
    out_app_to_device_link_.insert(
        std::make_pair(device_params.device_mac_address, app_id));
  }

 private:
  const ApplicationManager& application_manager_;
  std::map<std::string, std::string>& out_app_to_device_link_;
};

struct LinkAppToDevice {
  explicit LinkAppToDevice(
      std::map<std::string, std::string>& app_to_device_link,
      const ApplicationManager& application_manager)
      : app_to_device_link_(app_to_device_link)
      , application_manager_(application_manager) {
    app_to_device_link_.clear();
  }

  void operator()(const ApplicationSharedPtr& app) {
    if (!app.valid()) {
      LOG4CXX_WARN(logger_,
                   "Invalid pointer to application was passed."
                   "Skip current application.");
      return;
    }
    DeviceParams device_params = GetDeviceParams(
        app->device(),
        application_manager_.connection_handler().get_session_observer());
    const std::string app_id = app->policy_app_id();
    if (device_params.device_mac_address.empty()) {
      LOG4CXX_WARN(logger_,
                   "Couldn't find device, which hosts application " << app_id);
      return;
    }
    app_to_device_link_[app_id] = device_params.device_mac_address;
  }

 private:
  std::map<std::string, std::string>& app_to_device_link_;
  const ApplicationManager& application_manager_;
};

struct PermissionsConsolidator {
  void Consolidate(
      const std::vector<policy::FunctionalGroupPermission>& permissions) {
    std::vector<policy::FunctionalGroupPermission>::const_iterator it =
        permissions.begin();
    std::vector<policy::FunctionalGroupPermission>::const_iterator it_end =
        permissions.end();
    for (; it != it_end; ++it) {
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

const std::string PolicyHandler::kLibrary = "libPolicy.so";

PolicyHandler::PolicyHandler(const PolicySettings& settings,
                             ApplicationManager& application_manager)
    : AsyncRunner("PolicyHandler async runner thread")
    , dl_handle_(0)
    , last_activated_app_id_(0)
    , app_to_device_link_lock_(true)
    , statistic_manager_impl_(utils::MakeShared<StatisticManagerImpl>(this))
    , settings_(settings)
    , application_manager_(application_manager) {}

PolicyHandler::~PolicyHandler() {}

bool PolicyHandler::PolicyEnabled() const {
  return get_settings().enable_policy();
}

bool PolicyHandler::LoadPolicyLibrary() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoWriteLock lock(policy_manager_lock_);
  if (!PolicyEnabled()) {
    LOG4CXX_WARN(logger_,
                 "System is configured to work without policy "
                 "functionality.");
    policy_manager_.reset();
    return false;
  }
  dl_handle_ = dlopen(kLibrary.c_str(), RTLD_LAZY);

  const char* error = dlerror();
  if (!error) {
    if (CreateManager()) {
      policy_manager_->set_listener(this);
      event_observer_ =
          utils::SharedPtr<PolicyEventObserver>(new PolicyEventObserver(
              this, application_manager_.event_dispatcher()));
    }
  } else {
    LOG4CXX_ERROR(logger_, error);
  }

  return policy_manager_.valid();
}

bool PolicyHandler::CreateManager() {
  typedef PolicyManager* (*CreateManager)();
  typedef void (*DeleteManager)(PolicyManager*);
  CreateManager create_manager =
      reinterpret_cast<CreateManager>(dlsym(dl_handle_, "CreateManager"));
  DeleteManager delete_manager =
      reinterpret_cast<DeleteManager>(dlsym(dl_handle_, "DeleteManager"));
  char* error_string = dlerror();
  if (NULL == error_string) {
    policy_manager_ =
        utils::SharedPtr<PolicyManager>(create_manager(), delete_manager);
  } else {
    LOG4CXX_WARN(logger_, error_string);
  }
  return policy_manager_.valid();
}

const PolicySettings& PolicyHandler::get_settings() const {
  return settings_;
}

bool PolicyHandler::InitPolicyTable() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);
  // Subscribing to notification for system readiness to be able to get system
  // info necessary for policy table
  event_observer_->subscribe_on_event(
      hmi_apis::FunctionID::BasicCommunication_OnReady);
  std::string preloaded_file = get_settings().preloaded_pt_file();
  if (file_system::FileExists(preloaded_file)) {
    return policy_manager_->InitPT(preloaded_file, &get_settings());
  }
  LOG4CXX_FATAL(logger_, "The file which contains preloaded PT is not exist");
  return false;
}

bool PolicyHandler::ResetPolicyTable() {
  LOG4CXX_TRACE(logger_, "Reset policy table.");
  POLICY_LIB_CHECK(false);
  std::string preloaded_file = get_settings().preloaded_pt_file();
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

uint32_t PolicyHandler::GetAppIdForSending() const {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(0);
  const ApplicationSet& accessor =
      application_manager_.applications().GetData();

  HMILevelPredicate has_none_level(mobile_api::HMILevel::HMI_NONE);
  Applications apps_without_none_level;
  std::copy_if(accessor.begin(),
               accessor.end(),
               std::back_inserter(apps_without_none_level),
               std::not1(has_none_level));

  LOG4CXX_DEBUG(logger_,
                "Number of apps with different from NONE level: "
                    << apps_without_none_level.size());

  uint32_t choosen_app_id =
      ChooseRandomAppForPolicyUpdate(apps_without_none_level);

  if (choosen_app_id) {
    return choosen_app_id;
  }

  Applications apps_with_none_level;
  std::copy_if(accessor.begin(),
               accessor.end(),
               std::back_inserter(apps_with_none_level),
               has_none_level);

  LOG4CXX_DEBUG(
      logger_,
      "Number of apps with NONE level: " << apps_with_none_level.size());

  return ChooseRandomAppForPolicyUpdate(apps_with_none_level);
}

#ifdef EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnAppPermissionConsent(
    const uint32_t connection_key,
    const PermissionConsent& permissions,
    const ExternalConsentStatus& external_consent_status) {
  LOG4CXX_AUTO_TRACE(logger_);
  AsyncRun(new AppPermissionDelegate(
      connection_key, permissions, external_consent_status, *this));
}
#else
void PolicyHandler::OnAppPermissionConsent(
    const uint32_t connection_key, const PermissionConsent& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  AsyncRun(new AppPermissionDelegate(connection_key, permissions, *this));
}

#endif

void PolicyHandler::OnDeviceConsentChanged(const std::string& device_id,
                                           const bool is_allowed) {
  POLICY_LIB_CHECK_VOID();
  connection_handler::DeviceHandle device_handle;
  if (!application_manager_.connection_handler().GetDeviceID(device_id,
                                                             &device_handle)) {
    LOG4CXX_ERROR(logger_,
                  "Unable to get device handle for device_id: " << device_id);
    return;
  }
  // In case of changed consent for device, related applications will be
  // limited to pre_DataConsent permissions, if device disallowed, or switch
  // back to their own permissions, if device allowed again, and must be
  // notified about these changes

  const ApplicationSet& accessor =
      application_manager_.applications().GetData();
  ApplicationSetConstIt it_app_list = accessor.begin();
  ApplicationSetConstIt it_app_list_end = accessor.end();
  for (; it_app_list != it_app_list_end; ++it_app_list) {
    if (device_handle == (*it_app_list).get()->device()) {
      const std::string policy_app_id = (*it_app_list)->policy_app_id();

      // If app has predata policy, which is assigned without device consent
      // or
      // with negative data consent, there no necessity to change smth and
      // send
      // notification for such app in case of device consent is not allowed
      if (policy_manager_->IsPredataPolicy(policy_app_id) && !is_allowed) {
        continue;
      }

      policy_manager_->ReactOnUserDevConsentForApp(policy_app_id, is_allowed);

      policy_manager_->SendNotificationOnPermissionsUpdated(policy_app_id);
    }
  }
}

void PolicyHandler::SendOnAppPermissionsChanged(
    const AppPermissions& permissions, const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "PolicyHandler::SendOnAppPermissionsChanged for "
                    << policy_app_id);
  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "No app found for policy app id = " << policy_app_id);
    return;
  }
  MessageHelper::SendOnAppPermissionsChangedNotification(
      app->app_id(), permissions, application_manager_);
}

void PolicyHandler::OnPTExchangeNeeded() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->ForcePTExchange();
}

void PolicyHandler::GetAvailableApps(std::queue<std::string>& apps) {
  LOG4CXX_INFO(logger_, "GetAvailable apps");
  const ApplicationSet& app_list =
      application_manager_.applications().GetData();
  ApplicationSetConstIt iter = app_list.begin();

  for (; app_list.end() != iter; ++iter) {
    LOG4CXX_INFO(logger_, "one more app");
    apps.push((*iter)->policy_app_id());
  }
}

struct SmartObjectToInt {
  int operator()(const smart_objects::SmartObject& item) const {
    return item.asInt();
  }
};

StatusNotifier PolicyHandler::AddApplication(
    const std::string& application_id,
    const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) {
  POLICY_LIB_CHECK(utils::MakeShared<utils::CallNothing>());
  return policy_manager_->AddApplication(application_id, hmi_types);
}

void PolicyHandler::AddDevice(const std::string& device_id,
                              const std::string& connection_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->AddDevice(device_id, connection_type);
}

void PolicyHandler::SetDeviceInfo(const std::string& device_id,
                                  const DeviceInfo& device_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SetDeviceInfo(device_id, device_info);
}

#ifdef EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnAppPermissionConsentInternal(
    const uint32_t connection_key,
    const ExternalConsentStatus& external_consent_status,
    PermissionConsent& out_permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  const PolicyManager::NotificationMode mode =
      external_consent_status.empty() ? PolicyManager::kNotifyApplicationMode
                                      : PolicyManager::kSilentMode;
#else
void PolicyHandler::OnAppPermissionConsentInternal(
    const uint32_t connection_key, PermissionConsent& out_permissions) {
#endif
  POLICY_LIB_CHECK_VOID();

  if (connection_key) {
    ApplicationSharedPtr app = application_manager_.application(connection_key);

    if (app.valid()) {
      out_permissions.policy_app_id = app->policy_app_id();
      DeviceParams device_params = GetDeviceParams(
          app->device(),
          application_manager_.connection_handler().get_session_observer());

      out_permissions.device_id = device_params.device_mac_address;
    }

    if (!out_permissions.policy_app_id.empty()) {
#ifdef EXTERNAL_PROPRIETARY_MODE
      policy_manager_->SetUserConsentForApp(out_permissions, mode);
#else
      policy_manager_->SetUserConsentForApp(out_permissions);
#endif
    }
  } else if (!app_to_device_link_.empty()) {
    sync_primitives::AutoLock lock(app_to_device_link_lock_);
    std::map<std::string, std::string>::const_iterator it =
        app_to_device_link_.begin();
    for (; app_to_device_link_.end() != it; ++it) {
      ApplicationSharedPtr app =
          application_manager_.application_by_policy_id(it->second);

      // If list of apps sent to HMI for user consents is not the same as
      // current,
      // permissions should be set only for coincident to registered apps
      if (!app.valid()) {
        LOG4CXX_WARN(logger_,
                     "Invalid pointer to application was passed."
                     "Permissions setting skipped.");
        continue;
      }

      DeviceParams device_params = GetDeviceParams(
          app->device(),
          application_manager_.connection_handler().get_session_observer());

      if (device_params.device_mac_address != it->first) {
        LOG4CXX_WARN(logger_,
                     "Device_id of application is changed."
                     "Permissions setting skipped.");
        continue;
      }

      out_permissions.policy_app_id = it->second;
      out_permissions.device_id = it->first;
#ifdef EXTERNAL_PROPRIETARY_MODE
      policy_manager_->SetUserConsentForApp(out_permissions, mode);
#else
      policy_manager_->SetUserConsentForApp(out_permissions);
#endif
    }
  } else {
    LOG4CXX_WARN(logger_,
                 "There are no applications previously stored for "
                 "setting common permissions.");
  }
#ifdef EXTERNAL_PROPRIETARY_MODE
  if (!policy_manager_->SetExternalConsentStatus(external_consent_status)) {
    LOG4CXX_WARN(logger_,
                 "External User Consent Settings status has not been set!");
  }
#endif
}

void policy::PolicyHandler::SetDaysAfterEpoch() {
  POLICY_LIB_CHECK_VOID();
  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  int days_after_epoch = current_time.tv_sec / kSecondsInDay;
  PTUpdatedAt(Counters::DAYS_AFTER_EPOCH, days_after_epoch);
}

#ifdef ENABLE_SECURITY
std::string PolicyHandler::RetrieveCertificate() const {
  POLICY_LIB_CHECK(std::string(""));
  return policy_manager_->RetrieveCertificate();
}
#endif  // ENABLE_SECURITY

void PolicyHandler::OnGetUserFriendlyMessage(
    const std::vector<std::string>& message_codes,
    const std::string& language,
    uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();

#ifdef EXTERNAL_PROPRIETARY_MODE
  const std::string active_hmi_language =
      application_manager::MessageHelper::CommonLanguageToString(
          application_manager_.hmi_capabilities().active_ui_language());
  const std::vector<UserFriendlyMessage> result =
      policy_manager_->GetUserFriendlyMessages(
          message_codes, language, active_hmi_language);
#else
  const std::vector<UserFriendlyMessage> result =
      policy_manager_->GetUserFriendlyMessages(message_codes, language);
#endif  // EXTERNAL_PROPRIETARY_MODE
  // Send response to HMI with gathered data
  MessageHelper::SendGetUserFriendlyMessageResponse(
      result, correlation_id, application_manager_);
}

void PolicyHandler::GetRegisteredLinks(
    std::map<std::string, std::string>& out_links) const {
  DataAccessor<ApplicationSet> accessor = application_manager_.applications();
  ApplicationSetConstIt it_app = accessor.GetData().begin();
  ApplicationSetConstIt it_app_end = accessor.GetData().end();

  LinksCollector linker(application_manager_, out_links);
  std::for_each(it_app, it_app_end, linker);
}

std::vector<policy::FunctionalGroupPermission>
PolicyHandler::CollectRegisteredAppsPermissions() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(std::vector<policy::FunctionalGroupPermission>());
  // If no specific app was passed, get permissions for all currently registered
  // applications
  sync_primitives::AutoLock lock(app_to_device_link_lock_);

  GetRegisteredLinks(app_to_device_link_);

  PermissionsConsolidator consolidator;
  std::vector<policy::FunctionalGroupPermission> group_permissions;
  std::map<std::string, std::string>::const_iterator it =
      app_to_device_link_.begin();
  for (; it != app_to_device_link_.end(); ++it) {
    policy_manager_->GetUserConsentForApp(
        it->first, it->second, group_permissions);
    consolidator.Consolidate(group_permissions);
  }
  return consolidator.GetConsolidatedPermissions();
}

std::vector<FunctionalGroupPermission> PolicyHandler::CollectAppPermissions(
    const uint32_t connection_key) {
  // Single app only
  ApplicationSharedPtr app = application_manager_.application(connection_key);
  std::vector<FunctionalGroupPermission> group_permissions;

  if (NULL == app.get() || !app.valid()) {
    LOG4CXX_WARN(logger_,
                 "Connection key '"
                     << connection_key
                     << "' "
                        "not found within registered applications.");

    return group_permissions;
  }

  DeviceParams device_params = GetDeviceParams(
      app->device(),
      application_manager_.connection_handler().get_session_observer());

  if (device_params.device_mac_address.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find device, which hosts application.");
    return group_permissions;
  }

  policy_manager_->GetUserConsentForApp(device_params.device_mac_address,
                                        app->policy_app_id(),
                                        group_permissions);

  return group_permissions;
}

void PolicyHandler::OnGetListOfPermissions(const uint32_t connection_key,
                                           const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();

  application_manager::ApplicationSharedPtr app =
      application_manager_.application(connection_key);
  const bool is_app_registered = NULL != app.get();
  const bool is_connection_key_valid = is_app_registered && connection_key;

  const std::vector<policy::FunctionalGroupPermission> permissions =
      is_connection_key_valid ? CollectAppPermissions(connection_key)
                              : CollectRegisteredAppsPermissions();

  if (permissions.empty() && is_connection_key_valid) {
    LOG4CXX_ERROR(logger_,
                  "No permissions found for application with connection key:"
                      << connection_key);
    return;
  }

  MessageHelper::SendGetListOfPermissionsResponse(
      permissions,
#ifdef EXTERNAL_PROPRIETARY_MODE
      policy_manager_->GetExternalConsentStatus(),
#endif  // EXTERNAL_PROPRIETARY_MODE
      correlation_id,
      application_manager_);
}

void PolicyHandler::LinkAppsToDevice() {
  sync_primitives::AutoLock lock(app_to_device_link_lock_);
  LinkAppToDevice linker(app_to_device_link_, application_manager_);
  LOG4CXX_DEBUG(logger_, "add links to app.  no specific app was passed");
  {
    const ApplicationSet& accessor =
        application_manager_.applications().GetData();
    if (accessor.empty()) {
      LOG4CXX_WARN(logger_,
                   "application_manager doesn't have any applications");
    } else {
      // Add all currently registered applications
      std::for_each(accessor.begin(), accessor.end(), linker);
    }
  }
}

bool PolicyHandler::IsAppSuitableForPolicyUpdate(
    const Applications::value_type value) const {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!value->IsRegistered()) {
    LOG4CXX_DEBUG(logger_,
                  "Application " << value->app_id()
                                 << " is not marked as registered.");
    return false;
  }

  LOG4CXX_DEBUG(logger_,
                "Application " << value->app_id()
                               << " marked as registered."
                                  "Checking its parameters.");

  DeviceParams device_params = GetDeviceParams(
      value->device(),
      application_manager_.connection_handler().get_session_observer());

  const bool is_device_allowed = (kDeviceAllowed ==
                                  policy_manager_->GetUserConsentForDevice(
                                      device_params.device_mac_address));

  LOG4CXX_DEBUG(logger_,
                "Is device " << device_params.device_mac_address << " allowed "
                             << std::boolalpha << is_device_allowed);
  return is_device_allowed;
}

uint32_t PolicyHandler::ChooseRandomAppForPolicyUpdate(
    Applications& app_list) const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::random_shuffle(app_list.begin(), app_list.end());

  Applications::const_iterator choosen_app = std::find_if(
      app_list.begin(),
      app_list.end(),
      std::bind1st(std::mem_fun(&PolicyHandler::IsAppSuitableForPolicyUpdate),
                   this));

  if (app_list.end() != choosen_app) {
    return (*choosen_app)->app_id();
  }

  return 0;
}

void PolicyHandler::OnDeviceSwitching(const std::string& device_id_from,
                                      const std::string& device_id_to) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnDeviceSwitching(device_id_from, device_id_to);
}

void PolicyHandler::OnGetStatusUpdate(const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  MessageHelper::SendGetStatusUpdateResponse(
      policy_manager_->GetPolicyTableStatus(),
      correlation_id,
      application_manager_);
}

void PolicyHandler::OnUpdateStatusChanged(const std::string& status) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SaveUpdateStatusRequired(policy::kUpToDate != status);
  MessageHelper::SendOnStatusUpdate(status, application_manager_);
}

std::string PolicyHandler::OnCurrentDeviceIdUpdateRequired(
    const std::string& policy_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_,
                 "Application with id '"
                     << policy_app_id
                     << "' "
                        "not found within registered applications.");
    return "";
  }
  DeviceParams device_params = GetDeviceParams(
      app->device(),
      application_manager_.connection_handler().get_session_observer());

  return device_params.device_mac_address;
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
  MessageHelper::SendGetSystemInfoRequest(application_manager_);
}

void PolicyHandler::OnVIIsReady() {
  LOG4CXX_AUTO_TRACE(logger_);
  const uint32_t correlation_id =
      application_manager_.GetNextHMICorrelationID();

  std::vector<std::string> params;
  params.push_back(strings::vin);

  MessageHelper::CreateGetVehicleDataRequest(
      correlation_id, params, application_manager_);
}

void PolicyHandler::OnVehicleDataUpdated(
    const smart_objects::SmartObject& message) {
  POLICY_LIB_CHECK_VOID();
#ifdef EXTERNAL_PROPRIETARY_MODE
  if (!message.keyExists(strings::msg_params)) {
    LOG4CXX_ERROR(logger_,
                  "Message does not contains mandatory section "
                      << strings::msg_params);
    return;
  }
  if (message[strings::msg_params].keyExists(strings::vin)) {
    policy_manager_->SetVINValue(
        message[strings::msg_params][strings::vin].asString());
  }
#else
  LOG4CXX_DEBUG(logger_,
                "This functionality is not available for not external policy");
#endif
}

void PolicyHandler::OnPendingPermissionChange(
    const std::string& policy_app_id) {
  LOG4CXX_DEBUG(logger_,
                "PolicyHandler::OnPendingPermissionChange for "
                    << policy_app_id);
  POLICY_LIB_CHECK_VOID();
  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_,
                 "No app found for " << policy_app_id << " policy app id.");
    return;
  }

  AppPermissions permissions =
      policy_manager_->GetAppPermissionsChanges(policy_app_id);

  const uint32_t app_id = app->app_id();

  if (permissions.appRevoked) {
    application_manager::MessageHelper::SendOnAppPermissionsChangedNotification(
        app_id, permissions, application_manager_);
    application_manager_.state_controller().SetRegularState(
        app,
        mobile_apis::HMILevel::HMI_NONE,
        mobile_apis::AudioStreamingState::NOT_AUDIBLE,
        mobile_apis::VideoStreamingState::NOT_STREAMABLE,
        true);
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
    return;
  }

  mobile_apis::HMILevel::eType app_hmi_level = app->hmi_level();

  switch (app_hmi_level) {
    case mobile_apis::HMILevel::eType::HMI_FULL:
    case mobile_apis::HMILevel::eType::HMI_LIMITED: {
      if (permissions.appPermissionsConsentNeeded) {
        MessageHelper::SendOnAppPermissionsChangedNotification(
            app->app_id(), permissions, application_manager_);

        policy_manager_->RemovePendingPermissionChanges(policy_app_id);
        // "Break" statement has to be here to continue processing in case of
        // there is another "true" flag in permissions struct
        break;
      }
    }
    case mobile_apis::HMILevel::eType::HMI_BACKGROUND: {
      if (permissions.isAppPermissionsRevoked) {
        MessageHelper::SendOnAppPermissionsChangedNotification(
            app->app_id(), permissions, application_manager_);

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
      MessageHelper::SendOnAppPermissionsChangedNotification(
          app->app_id(), permissions, application_manager_);
    }
    application_manager_.ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app->app_id(),
            mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED),
        commands::Command::ORIGIN_SDL);

    application_manager_.OnAppUnauthorized(app->app_id());

    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }

  if (permissions.requestTypeChanged) {
    MessageHelper::SendOnAppPermissionsChangedNotification(
        app->app_id(), permissions, application_manager_);
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }
}

bool PolicyHandler::SendMessageToSDK(const BinaryMessage& pt_string,
                                     const std::string& url) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);

  const uint32_t app_id = GetAppIdForSending();
  ApplicationSharedPtr app = application_manager_.application(app_id);

  if (!app) {
    LOG4CXX_WARN(logger_,
                 "There is no registered application with "
                 "connection key '"
                     << app_id << "'");
    return false;
  }

  const std::string& mobile_app_id = app->policy_app_id();
  if (mobile_app_id.empty()) {
    LOG4CXX_WARN(logger_,
                 "Application with connection key '"
                     << app_id << "'"
                                  " has no application id.");
    return false;
  }

  LOG4CXX_DEBUG(logger_,
                "Update url is " << url << " for application "
                                           "with connection key " << app_id);

  MessageHelper::SendPolicySnapshotNotification(
      app_id, pt_string, url, application_manager_);

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
    int32_t correlation_id = application_manager_.GetNextHMICorrelationID();

    SetDaysAfterEpoch();

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
        correlation_id, vehicle_data_args, application_manager_);
  } else {
    LOG4CXX_WARN(logger_, "Exchange wasn't successful, trying another one.");
    policy_manager_->ForcePTExchange();
  }
  OnPTUFinished(ret);
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

#ifdef EXTERNAL_PROPRIETARY_MODE
struct SDLAlowedNotification {
  SDLAlowedNotification(const connection_handler::DeviceHandle& device_id,
                        PolicyManager* policy_manager,
                        StateController& state_controller)
      : device_id_(device_id)
      , policy_manager_(policy_manager)
      , state_controller_(state_controller) {}

  void operator()(const ApplicationSharedPtr& app) {
    DCHECK_OR_RETURN_VOID(policy_manager_);
    if (app->device() == device_id_) {
      std::string hmi_level;
      mobile_apis::HMILevel::eType default_mobile_hmi;
      policy_manager_->GetDefaultHmi(app->policy_app_id(), &hmi_level);
      if ("BACKGROUND" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_BACKGROUND;
      } else if ("FULL" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_FULL;
      } else if ("LIMITED" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_LIMITED;
      } else if ("NONE" == hmi_level) {
        default_mobile_hmi = mobile_apis::HMILevel::HMI_NONE;
      } else {
        return;
      }
      state_controller_.SetRegularState(app, default_mobile_hmi, true);
    }
  }

 private:
  connection_handler::DeviceHandle device_id_;
  PolicyManager* policy_manager_;
  StateController& state_controller_;
};
#endif  // EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::OnAllowSDLFunctionalityNotification(
    bool is_allowed, const std::string& device_mac) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  using namespace mobile_apis;
  const bool device_specific = !device_mac.empty();
  // Common devices consents change
  connection_handler::ConnectionHandler& connection_handler =
      application_manager_.connection_handler();

  if (!device_specific) {
    // Device ids, need to be changed
    std::vector<std::string> device_macs;
    connection_handler.GetConnectedDevicesMAC(device_macs);
    std::vector<std::string>::const_iterator it_ids = device_macs.begin();
    std::vector<std::string>::const_iterator it_ids_end = device_macs.end();
    for (; it_ids != it_ids_end; ++it_ids) {
      const std::string device_id = *it_ids;

      if (kDefaultDeviceMacAddress == device_id) {
        LOG4CXX_WARN(logger_,
                     "Device with id " << device_id << " wasn't found.");
        continue;
      }
      policy_manager_->SetUserConsentForDevice(device_id, is_allowed);
      connection_handler::DeviceHandle device_handle = 0;
      if (!connection_handler.GetDeviceID(device_id, &device_handle)) {
        LOG4CXX_WARN(logger_,
                     "Device handle with mac " << device_id
                                               << " wasn't found.");
      }

#ifdef EXTERNAL_PROPRIETARY_MODE

      DataAccessor<ApplicationSet> accessor =
          application_manager_.applications();
      if (!is_allowed) {
        std::for_each(
            accessor.GetData().begin(),
            accessor.GetData().end(),
            DeactivateApplication(device_handle,
                                  application_manager_.state_controller()));
#ifdef SDL_REMOTE_CONTROL
        application_manager_.GetPluginManager().OnPolicyEvent(
            functional_modules::PolicyEvent::kApplicationsDisabled);
#endif  // SDL_REMOTE_CONTROL
      } else {
        std::for_each(
            accessor.GetData().begin(),
            accessor.GetData().end(),
            SDLAlowedNotification(device_handle,
                                  policy_manager_.get(),
                                  application_manager_.state_controller()));
      }
#endif  // EXTERNAL_PROPRIETARY_MODE
    }
  }

  // Case, when specific device was changed
  connection_handler::DeviceHandle device_handle = 0u;
  if (device_specific) {
    policy_manager_->SetUserConsentForDevice(device_mac, is_allowed);
    if (!connection_handler.GetDeviceID(device_mac, &device_handle)) {
      LOG4CXX_WARN(logger_,
                   "Device hadle with mac " << device_mac << " wasn't found.");
    }
    DeviceHandles::iterator it = std::find(pending_device_handles_.begin(),
                                           pending_device_handles_.end(),
                                           device_handle);
    // If consent done from HMI menu
    if (it == pending_device_handles_.end()) {
      return;
    }

    pending_device_handles_.erase(it);
  }

#ifdef EXTERNAL_PROPRIETARY_MODE

  if (last_activated_app_id_) {
    ApplicationSharedPtr app =
        application_manager_.application(last_activated_app_id_);

    if (!app) {
      LOG4CXX_WARN(logger_,
                   "Application with id '"
                       << last_activated_app_id_
                       << "' not found among registered applications.");
      return;
    }
    if (is_allowed) {
      // Send HMI status notification to mobile
      // Put application in full
      AudioStreamingState::eType state = app->is_audio()
                                             ? AudioStreamingState::AUDIBLE
                                             : AudioStreamingState::NOT_AUDIBLE;

      application_manager_.state_controller().SetRegularState(
          app, mobile_apis::HMILevel::HMI_FULL, state, true);
      last_activated_app_id_ = 0;
    } else {
      DeactivateApplication deactivate_notification(
          device_handle, application_manager_.state_controller());
      deactivate_notification(app);
    }
  }
#endif  // EXTERNAL_PROPRIETARY_MODE
}

void PolicyHandler::OnIgnitionCycleOver() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->IncrementIgnitionCycles();
}

void PolicyHandler::OnActivateApp(uint32_t connection_key,
                                  uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  ApplicationSharedPtr app = application_manager_.application(connection_key);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Activated App failed: no app found.");
    return;
  }
  std::string policy_app_id = app->policy_app_id();

  AppPermissions permissions(policy_app_id);

  sync_primitives::AutoReadLock lock(policy_manager_lock_);
  if (!policy_manager_) {
    LOG4CXX_DEBUG(logger_, "The shared library of policy is not loaded");
    if (!PolicyEnabled()) {
      permissions.isSDLAllowed = true;
    }
  } else {
    permissions = policy_manager_->GetAppPermissionsChanges(policy_app_id);
#ifdef EXTERNAL_PROPRIETARY_MODE
    UsageStatistics& usage = app->usage_report();

    usage.RecordAppUserSelection();
    DeviceParams device_params = GetDeviceParams(
        app->device(),
        application_manager_.connection_handler().get_session_observer());
    permissions.deviceInfo = device_params;

    DeviceConsent consent = policy_manager_->GetUserConsentForDevice(
        permissions.deviceInfo.device_mac_address);
    permissions.isSDLAllowed = kDeviceAllowed == consent;

    // According to the SDLAQ-CRS-2794, p.9
    // 'priority' should be omitted in case when device
    // is not allowed.
    if (!permissions.isSDLAllowed) {
      permissions.priority.clear();
      last_activated_app_id_ = connection_key;
    }

    if (permissions.appRevoked) {
      usage.RecordRunAttemptsWhileRevoked();
    }

    // If isSDLAllowed is false, we should provide device params for user
    // consent
    if (!permissions.isSDLAllowed) {
      pending_device_handles_.push_back(permissions.deviceInfo.device_handle);
    }

    if (permissions.appPermissionsConsentNeeded) {
      MessageHelper::SendOnAppPermissionsChangedNotification(
          app->app_id(), permissions, application_manager_);
    }
#else   // EXTERNAL_PROPRIETARY_MODE
    permissions.isSDLAllowed = true;
#endif  // EXTERNAL_PROPRIETARY_MODE
    policy_manager_->RemovePendingPermissionChanges(policy_app_id);
  }
  // If application is revoked it should not be activated
  // In this case we need to activate application
  if (false == permissions.appRevoked && true == permissions.isSDLAllowed) {
    LOG4CXX_INFO(logger_, "Application will be activated");
    if (application_manager_.ActivateApplication(app)) {
      last_activated_app_id_ = 0;
    }
  } else {
    LOG4CXX_WARN(logger_, "Application should not be activated");
  }

  MessageHelper::SendSDLActivateAppResponse(
      permissions, correlation_id, application_manager_);
}

void PolicyHandler::KmsChanged(int kilometers) {
  LOG4CXX_DEBUG(logger_,
                "PolicyHandler::KmsChanged " << kilometers << " kilometers");
  POLICY_LIB_CHECK_VOID();
  policy_manager_->KmsChanged(kilometers);
}

void PolicyHandler::PTExchangeAtUserRequest(uint32_t correlation_id) {
  LOG4CXX_TRACE(logger_, "PT exchange at user request");
  POLICY_LIB_CHECK_VOID();
  std::string update_status = policy_manager_->ForcePTExchangeAtUserRequest();
  MessageHelper::SendUpdateSDLResponse(
      update_status, correlation_id, application_manager_);
}

void PolicyHandler::OnPermissionsUpdated(const std::string& policy_app_id,
                                         const Permissions& permissions,
                                         const HMILevel& default_hmi) {
  LOG4CXX_AUTO_TRACE(logger_);
  OnPermissionsUpdated(policy_app_id, permissions);

  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);
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
    LOG4CXX_WARN(logger_,
                 "Couldn't convert default hmi level " << default_hmi
                                                       << " to enum.");
    return;
  }
  if (current_hmi_level == hmi_level) {
    LOG4CXX_DEBUG(logger_, "Application already in default hmi state.");
    return;
  }
  switch (current_hmi_level) {
    case mobile_apis::HMILevel::HMI_NONE: {
      LOG4CXX_INFO(logger_,
                   "Changing hmi level of application "
                       << policy_app_id << " to default hmi level "
                       << default_hmi);

      if (hmi_level == mobile_apis::HMILevel::HMI_FULL) {
        application_manager_.state_controller().SetRegularState(
            app, hmi_level, true);
      } else {
        application_manager_.state_controller().SetRegularState(
            app, hmi_level, false);
      }
      break;
    }
    default:
      LOG4CXX_WARN(logger_,
                   "Application " << policy_app_id
                                  << " is running."
                                     "HMI level won't be changed.");
      break;
  }
}

void PolicyHandler::OnPermissionsUpdated(const std::string& policy_app_id,
                                         const Permissions& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(
        logger_,
        "Connection_key not found for application_id:" << policy_app_id);
    return;
  }

  MessageHelper::SendOnPermissionsChangeNotification(
      app->app_id(), permissions, application_manager_);

  LOG4CXX_DEBUG(logger_,
                "Notification sent for application_id:"
                    << policy_app_id << " and connection_key "
                    << app->app_id());
}

bool PolicyHandler::SaveSnapshot(const BinaryMessage& pt_string,
                                 std::string& snap_path) {
  const std::string& policy_snapshot_file_name =
      get_settings().policies_snapshot_file_name();
  const std::string& system_files_path = get_settings().system_files_path();
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

#ifdef EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnSnapshotCreated(
    const BinaryMessage& pt_string,
    const std::vector<int>& retry_delay_seconds,
    const uint32_t timeout_exchange_ms) {
  std::string policy_snapshot_full_path;
  if (SaveSnapshot(pt_string, policy_snapshot_full_path)) {
    const uint32_t timeout_exchange_s =
        timeout_exchange_ms / date_time::DateTime::MILLISECONDS_IN_SECOND;
    MessageHelper::SendPolicyUpdate(policy_snapshot_full_path,
                                    timeout_exchange_s,
                                    retry_delay_seconds,
                                    application_manager_);
  }
}
#else  // EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnSnapshotCreated(const BinaryMessage& pt_string) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
#ifdef PROPRIETARY_MODE
  std::string policy_snapshot_full_path;
  if (!SaveSnapshot(pt_string, policy_snapshot_full_path)) {
    LOG4CXX_ERROR(logger_, "Snapshot processing skipped.");
    return;
  }
  MessageHelper::SendPolicyUpdate(policy_snapshot_full_path,
                                  TimeoutExchangeSec(),
                                  policy_manager_->RetrySequenceDelaysSeconds(),
                                  application_manager_);
#else   // PROPRIETARY_MODE
  LOG4CXX_ERROR(logger_, "HTTP policy");
  EndpointUrls urls;
  policy_manager_->GetUpdateUrls("0x07", urls);

  if (urls.empty()) {
    LOG4CXX_ERROR(logger_, "Service URLs are empty! NOT sending PT to mobile!");
    return;
  }

  AppIdURL app_url = policy_manager_->GetNextUpdateUrl(urls);
  while (!IsUrlAppIdValid(app_url.first, urls)) {
    app_url = policy_manager_->GetNextUpdateUrl(urls);
  }
  const std::string& url = urls[app_url.first].url[app_url.second];
  SendMessageToSDK(pt_string, url);
#endif  // PROPRIETARY_MODE
  // reset update required false
  OnUpdateRequestSentToMobile();
}
#endif  // EXTERNAL_PROPRIETARY_MODE

bool PolicyHandler::GetPriority(const std::string& policy_app_id,
                                std::string* priority) const {
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetPriority(policy_app_id, priority);
}

void PolicyHandler::CheckPermissions(
    const application_manager::ApplicationSharedPtr app,
    const PTString& rpc,
    const RPCParams& rpc_params,
    CheckPermissionResult& result) {
  POLICY_LIB_CHECK_VOID();
  const std::string hmi_level =
      MessageHelper::StringifiedHMILevel(app->hmi_level());
  const std::string device_id = MessageHelper::GetDeviceMacAddressForHandle(
      app->device(), application_manager_);
  LOG4CXX_INFO(logger_,
               "Checking permissions for  " << app->policy_app_id() << " in "
                                            << hmi_level << " on device "
                                            << device_id << " rpc " << rpc);
#ifdef EXTERNAL_PROPRIETARY_MODE
  policy_manager_->CheckPermissions(
      app->policy_app_id(), hmi_level, rpc, rpc_params, result);
#else   // EXTERNAL_PROPRIETARY_MODE
  policy_manager_->CheckPermissions(
      device_id, app->policy_app_id(), hmi_level, rpc, rpc_params, result);
#endif  // EXTERNAL_PROPRIETARY_MODE
}

uint32_t PolicyHandler::GetNotificationsNumber(
    const std::string& priority) const {
  POLICY_LIB_CHECK(0);
  return policy_manager_->GetNotificationsNumber(priority);
}

DeviceConsent PolicyHandler::GetUserConsentForDevice(
    const std::string& device_id) const {
  POLICY_LIB_CHECK(kDeviceDisallowed);
  return policy_manager_->GetUserConsentForDevice(device_id);
}

bool PolicyHandler::GetDefaultHmi(const std::string& policy_app_id,
                                  std::string* default_hmi) const {
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetDefaultHmi(policy_app_id, default_hmi);
}

bool PolicyHandler::GetInitialAppData(const std::string& application_id,
                                      StringArray* nicknames,
                                      StringArray* app_hmi_types) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetInitialAppData(
      application_id, nicknames, app_hmi_types);
}

void PolicyHandler::GetUpdateUrls(const std::string& service_type,
                                  EndpointUrls& out_end_points) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->GetUpdateUrls(service_type, out_end_points);
}

void PolicyHandler::GetUpdateUrls(const uint32_t service_type,
                                  EndpointUrls& out_end_points) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->GetUpdateUrls(service_type, out_end_points);
}

std::string PolicyHandler::GetLockScreenIconUrl() const {
  POLICY_LIB_CHECK(std::string(""));
  return policy_manager_->GetLockScreenIconUrl();
}

uint32_t PolicyHandler::NextRetryTimeout() {
  POLICY_LIB_CHECK(0);
  LOG4CXX_AUTO_TRACE(logger_);
  return policy_manager_->NextRetryTimeout();
}

uint32_t PolicyHandler::TimeoutExchangeSec() const {
  return TimeoutExchangeMSec() / date_time::DateTime::MILLISECONDS_IN_SECOND;
}

uint32_t PolicyHandler::TimeoutExchangeMSec() const {
  POLICY_LIB_CHECK(0);
  return policy_manager_->TimeoutExchangeMSec();
}

void PolicyHandler::OnExceededTimeout() {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnExceededTimeout();
}

void PolicyHandler::OnSystemReady() {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnSystemReady();
}

void PolicyHandler::PTUpdatedAt(Counters counter, int value) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->PTUpdatedAt(counter, value);
}

void PolicyHandler::add_listener(PolicyHandlerObserver* listener) {
  if (NULL == listener) {
    return;
  }
  sync_primitives::AutoLock lock(listeners_lock_);
  listeners_.push_back(listener);
}

void PolicyHandler::remove_listener(PolicyHandlerObserver* listener) {
  if (NULL == listener) {
    return;
  }
  sync_primitives::AutoLock lock(listeners_lock_);
  listeners_.remove(listener);
}

utils::SharedPtr<usage_statistics::StatisticsManager>
PolicyHandler::GetStatisticManager() const {
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
    default: { LOG4CXX_WARN(logger_, "Type of statistics is unknown"); }
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
    default: { LOG4CXX_WARN(logger_, "System error is unknown"); }
  }
}

custom_str::CustomString PolicyHandler::GetAppName(
    const std::string& policy_app_id) {
  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);

  if (!app.valid()) {
    LOG4CXX_WARN(
        logger_,
        "Connection_key not found for application_id:" << policy_app_id);
    return custom_str::CustomString("");
  }
  return app->name();
}

void PolicyHandler::OnUpdateHMIAppType(
    std::map<std::string, StringArray> app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    (*it)->OnUpdateHMIAppType(app_hmi_types);
  }
}
#ifdef EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::OnCertificateUpdated(const std::string& certificate_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (certificate_data.empty()) {
    OnEmptyCertificateArrived();
    return;
  }

  const std::string file_name =
      file_system::GetAbsolutePath(get_settings().app_storage_folder()) + "/" +
      kCerficateFileName;
  const bool is_written = file_system::Write(
      file_name,
      std::vector<uint8_t>(certificate_data.begin(), certificate_data.end()));
  LOG4CXX_DEBUG(logger_,
                "Saving encrypted certificate data: \"" << certificate_data
                                                        << '"');
  if (!is_written) {
    LOG4CXX_ERROR(logger_,
                  "Unable to save encrypted certificate to file " << file_name);
    return;
  }
  LOG4CXX_DEBUG(logger_, "Saved encrypted certificate data" << file_name);

  MessageHelper::SendDecryptCertificateToHMI(file_name, application_manager_);
}

void PolicyHandler::OnEmptyCertificateArrived() const {
  LOG4CXX_DEBUG(logger_, "Empty certificate arrived");
  const std::string empty_certificate;
  sync_primitives::AutoLock lock(listeners_lock_);
  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(std::mem_fun(&PolicyHandlerObserver::OnCertificateUpdated),
                   empty_certificate));
}

void PolicyHandler::OnCertificateDecrypted(bool is_succeeded) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();

  const std::string file_name =
      file_system::GetAbsolutePath(get_settings().app_storage_folder()) + +"/" +
      kCerficateFileName;

  LOG4CXX_DEBUG(logger_, "Loading certificate data from file " << file_name);

  utils::ScopeGuard file_deleter =
      utils::MakeGuard(file_system::DeleteFile, file_name);
  UNUSED(file_deleter);

  if (!is_succeeded) {
    LOG4CXX_ERROR(logger_, "Couldn't delete file " << file_name);
    return;
  }

  std::string certificate_data;
  if (!file_system::ReadFile(file_name, certificate_data)) {
    LOG4CXX_ERROR(logger_,
                  "Unable to read certificate from file " << file_name);
    return;
  }
  LOG4CXX_DEBUG(logger_,
                "Loaded decrypted certificate data: \"" << certificate_data
                                                        << '"');

  policy_manager_->SetDecryptedCertificate(certificate_data);

  sync_primitives::AutoLock lock(listeners_lock_);
  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(std::mem_fun(&PolicyHandlerObserver::OnCertificateUpdated),
                   certificate_data));
}
#else   // EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnCertificateUpdated(const std::string& certificate_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    PolicyHandlerObserver* observer = *it;
    observer->OnCertificateUpdated(certificate_data);
  }
}
#endif  // EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::OnPTUFinished(const bool ptu_result) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(listeners_lock_);

  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(std::mem_fun(&PolicyHandlerObserver::OnPTUFinished),
                   ptu_result));
}

bool PolicyHandler::CanUpdate() {
  return 0 != GetAppIdForSending();
}

void PolicyHandler::RemoveDevice(const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();

  policy_manager_->MarkUnpairedDevice(device_id);
#ifdef EXTERNAL_PROPRIETARY_MODE
  connection_handler::DeviceHandle device_uid;
  if (application_manager_.connection_handler().GetDeviceID(device_id,
                                                            &device_uid)) {
    DataAccessor<ApplicationSet> accessor = application_manager_.applications();
    std::for_each(accessor.GetData().begin(),
                  accessor.GetData().end(),
                  DeactivateApplication(
                      device_uid, application_manager_.state_controller()));
  }
#endif  // EXTERNAL_PROPRIETARY_MODE
}

bool PolicyHandler::IsApplicationRevoked(const std::string& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);

  return policy_manager_->IsApplicationRevoked(app_id);
}

void PolicyHandler::OnUpdateRequestSentToMobile() {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnUpdateStarted();
}

bool PolicyHandler::CheckKeepContext(const std::string& policy_app_id) const {
  POLICY_LIB_CHECK(false);
  return policy_manager_->CanAppKeepContext(policy_app_id);
}

bool PolicyHandler::CheckStealFocus(const std::string& policy_app_id) const {
  POLICY_LIB_CHECK(false);
  return policy_manager_->CanAppStealFocus(policy_app_id);
}

bool PolicyHandler::CheckSystemAction(
    mobile_apis::SystemAction::eType system_action,
    const std::string& policy_app_id) const {
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

uint32_t PolicyHandler::HeartBeatTimeout(const std::string& app_id) const {
  POLICY_LIB_CHECK(0);
  return policy_manager_->HeartBeatTimeout(app_id);
}

const std::string PolicyHandler::RemoteAppsUrl() const {
  const std::string default_url;
  POLICY_LIB_CHECK(default_url);
  EndpointUrls endpoints;
  policy_manager_->GetUpdateUrls("queryAppsUrl", endpoints);
  if (endpoints.empty() || endpoints[0].url.empty()) {
    return default_url;
  }

  return endpoints[0].url[0];
}

void PolicyHandler::OnAppsSearchStarted() {
  POLICY_LIB_CHECK();
  policy_manager_->OnAppsSearchStarted();
}

void PolicyHandler::OnAppsSearchCompleted(const bool trigger_ptu) {
  POLICY_LIB_CHECK();
  policy_manager_->OnAppsSearchCompleted(trigger_ptu);
}

void PolicyHandler::OnAppRegisteredOnMobile(const std::string& application_id) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->OnAppRegisteredOnMobile(application_id);
}

bool PolicyHandler::IsRequestTypeAllowed(
    const std::string& policy_app_id,
    mobile_apis::RequestType::eType type) const {
  POLICY_LIB_CHECK(false);
  using namespace mobile_apis;

  std::string stringified_type = RequestTypeToString(type);
  if (stringified_type.empty()) {
    LOG4CXX_ERROR(logger_, "Unknown request type.");
    return false;
  }

  std::vector<std::string> request_types =
      policy_manager_->GetAppRequestTypes(policy_app_id);

  // If no request types are assigned to app - any is allowed
  if (request_types.empty()) {
    return true;
  }

  std::vector<std::string>::const_iterator it =
      std::find(request_types.begin(), request_types.end(), stringified_type);
  return request_types.end() != it;
}

const std::vector<std::string> PolicyHandler::GetAppRequestTypes(
    const std::string& policy_app_id) const {
  POLICY_LIB_CHECK(std::vector<std::string>());
  return policy_manager_->GetAppRequestTypes(policy_app_id);
}

const VehicleInfo policy::PolicyHandler::GetVehicleInfo() const {
  POLICY_LIB_CHECK(VehicleInfo());
  return policy_manager_->GetVehicleInfo();
}

#ifdef EXTERNAL_PROPRIETARY_MODE
const MetaInfo PolicyHandler::GetMetaInfo() const {
  POLICY_LIB_CHECK(MetaInfo());
  return policy_manager_->GetMetaInfo();
}
#endif  // EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::Increment(usage_statistics::GlobalCounterId type) {
  POLICY_LIB_CHECK();
  policy_manager_->Increment(type);
}

void PolicyHandler::Increment(const std::string& app_id,
                              usage_statistics::AppCounterId type) {
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

bool PolicyHandler::IsUrlAppIdValid(const uint32_t app_idx,
                                    const EndpointUrls& urls) const {
  const EndpointData& app_data = urls[app_idx];
  const std::vector<std::string> app_urls = app_data.url;
  const ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(app_data.app_id);

  const bool is_registered = (app && (app->IsRegistered()));
  const bool is_default = (app_data.app_id == policy::kDefaultId);
  const bool is_empty_urls = app_urls.empty();

  return ((is_registered && !is_empty_urls) || is_default);
}

#ifdef SDL_REMOTE_CONTROL

std::vector<std::string> PolicyHandler::GetDevicesIds(
    const std::string& policy_app_id) {
  return application_manager_.devices(policy_app_id);
}

void PolicyHandler::UpdateHMILevel(ApplicationSharedPtr app,
                                   mobile_apis::HMILevel::eType level) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  if (app->hmi_level() == mobile_apis::HMILevel::HMI_NONE) {
    // If default is FULL, send request to HMI. Notification to mobile will be
    // sent on response receiving.
    if (mobile_apis::HMILevel::HMI_FULL == level) {
      MessageHelper::SendActivateAppToHMI(app->app_id(), application_manager_);
    } else {
      LOG4CXX_INFO(logger_,
                   "Changing hmi level of application "
                       << app->app_id() << " to default hmi level " << level);
      // Set application hmi level
      application_manager_.ChangeAppsHMILevel(app->app_id(), level);
      // If hmi Level is full, it will be seted after ActivateApp response
      MessageHelper::SendHMIStatusNotification(*app, application_manager_);
    }
  }
}

bool PolicyHandler::CheckModule(const PTString& app_id,
                                const PTString& module) {
  POLICY_LIB_CHECK(false);
  return policy_manager_->CheckModule(app_id, module);
}

void PolicyHandler::OnRemoteAppPermissionsChanged(
    const std::string& device_id, const std::string& application_id) {
  POLICY_LIB_CHECK_VOID();
  policy_manager_->SendAppPermissionsChanged(device_id, application_id);
}

void PolicyHandler::OnUpdateHMIStatus(const std::string& device_id,
                                      const std::string& policy_app_id,
                                      const std::string& hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (!app) {
    LOG4CXX_WARN(logger_,
                 "Could not find application: " << device_id << " - "
                                                << policy_app_id);
    return;
  }
  mobile_apis::HMILevel::eType level =
      MessageHelper::StringToHMILevel(hmi_level);
  if (mobile_apis::HMILevel::INVALID_ENUM == level) {
    LOG4CXX_WARN(logger_,
                 "Couldn't convert default hmi level " << hmi_level
                                                       << " to enum.");
    return;
  }

  LOG4CXX_INFO(logger_,
               "Changing hmi level of application "
                   << app->app_id() << " to default hmi level " << level);
  // Set application hmi level
  application_manager_.ChangeAppsHMILevel(app->app_id(), level);
  MessageHelper::SendHMIStatusNotification(*app, application_manager_);
}

bool PolicyHandler::GetModuleTypes(const std::string& policy_app_id,
                                   std::vector<std::string>* modules) const {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);
  return policy_manager_->GetModuleTypes(policy_app_id, modules);
}

void PolicyHandler::SetDefaultHmiTypes(
    const std::string& application_id,
    const smart_objects::SmartObject* app_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK_VOID();
  std::vector<int> hmi_types;
  if (app_types && app_types->asArray()) {
    smart_objects::SmartArray* hmi_list = app_types->asArray();
    std::transform(hmi_list->begin(),
                   hmi_list->end(),
                   std::back_inserter(hmi_types),
                   SmartObjectToInt());
  }
  policy_manager_->SetDefaultHmiTypes(application_id, hmi_types);
}

bool PolicyHandler::CheckHMIType(const std::string& application_id,
                                 mobile_apis::AppHMIType::eType hmi,
                                 const smart_objects::SmartObject* app_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  POLICY_LIB_CHECK(false);
  std::vector<int> policy_hmi_types;
  bool ret = policy_manager_->GetHMITypes(application_id, &policy_hmi_types);

  std::vector<int> additional_hmi_types;
  if (app_types && app_types->asArray()) {
    smart_objects::SmartArray* hmi_list = app_types->asArray();
    std::transform(hmi_list->begin(),
                   hmi_list->end(),
                   std::back_inserter(additional_hmi_types),
                   SmartObjectToInt());
  }
  const std::vector<int>& hmi_types =
      ret ? policy_hmi_types : additional_hmi_types;
  return std::find(hmi_types.begin(), hmi_types.end(), hmi) != hmi_types.end();
}

void PolicyHandler::OnUpdateHMILevel(const std::string& device_id,
                                     const std::string& policy_app_id,
                                     const std::string& hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (!app) {
    LOG4CXX_WARN(logger_,
                 "Could not find application: " << device_id << " - "
                                                << policy_app_id);
    return;
  }
  mobile_apis::HMILevel::eType level =
      MessageHelper::StringToHMILevel(hmi_level);
  if (mobile_apis::HMILevel::INVALID_ENUM == level) {
    LOG4CXX_WARN(logger_,
                 "Couldn't convert default hmi level " << hmi_level
                                                       << " to enum.");
    return;
  }
  UpdateHMILevel(app, level);
}
#endif  // SDL_REMOTE_CONTROL
}  //  namespace policy
