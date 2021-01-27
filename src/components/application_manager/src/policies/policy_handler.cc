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

#include <dlfcn.h>
#include <unistd.h>
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/delegates/app_permission_delegate.h"
#include "application_manager/rpc_service.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/state_controller.h"
#include "application_manager/usage_statistics.h"
#include "connection_handler/connection_handler.h"
#include "interfaces/MOBILE_API.h"
#include "json/value.h"
#include "json/writer.h"
#include "policy/policy_manager_impl.h"
#include "policy/policy_types.h"
#include "policy/status.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/macro.h"
#include "utils/scope_guard.h"

#include "policy/policy_manager.h"
#include "utils/helpers.h"
#ifdef EXTERNAL_PROPRIETARY_MODE
#include "policy/ptu_retry_handler.h"
#endif  // EXTERNAL_PROPRIETARY_MODE

namespace policy {

using namespace application_manager;

SDL_CREATE_LOG_VARIABLE("PolicyHandler")

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
    {mobile_apis::RequestType::FOTA, "FOTA"},
    {mobile_apis::RequestType::OEM_SPECIFIC, "OEM_SPECIFIC"},
    {mobile_apis::RequestType::ICON_URL, "ICON_URL"}};

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
  policy::DeviceParams device_params;
  if (-1 == session_observer.GetDataOnDeviceID(
                device_handle,
                &device_params.device_name,
                NULL,
                &device_params.device_mac_address,
                &device_params.device_connection_type)) {
    SDL_LOG_ERROR("Failed to extract information for device " << device_handle);
  }
  device_params.device_handle = device_handle;
  return device_params;
}

struct HMILevelPredicate
    : public std::unary_function<ApplicationSharedPtr, bool> {
  explicit HMILevelPredicate(const mobile_api::HMILevel::eType level)
      : level_(level) {}

  bool operator()(const ApplicationSharedPtr app) const {
    return level_ == app->hmi_level(
                         mobile_apis::PredefinedWindows::DEFAULT_WINDOW)
               ? true
               : false;
  }

 private:
  mobile_api::HMILevel::eType level_;
};

constexpr char kLibraryNotLoadedMessage[] =
    "The shared library of policy is not loaded";

}  // namespace

#define POLICY_LIB_CHECK_OR_RETURN(policy_manager, return_value)   \
  {                                                                \
    sync_primitives::AutoReadLock lock(policy_manager_lock_);      \
    if (!policy_manager) {                                         \
      SDL_LOG_DEBUG("The shared library of policy is not loaded"); \
      return return_value;                                         \
    }                                                              \
  }

#define POLICY_LIB_CHECK_VOID(policy_manager)                      \
  {                                                                \
    sync_primitives::AutoReadLock lock(policy_manager_lock_);      \
    if (!policy_manager) {                                         \
      SDL_LOG_DEBUG("The shared library of policy is not loaded"); \
      return;                                                      \
    }                                                              \
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
          mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
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
    if (app.use_count() == 0) {
      SDL_LOG_WARN(
          "Invalid pointer to application was passed."
          "Skip current application.");
      return;
    }
    DeviceParams device_params = GetDeviceParams(
        app->device(),
        application_manager_.connection_handler().get_session_observer());
    const std::string app_id = app->policy_app_id();
    if (device_params.device_mac_address.empty()) {
      SDL_LOG_WARN("Couldn't find device, which hosts application " << app_id);
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
    if (app.use_count() == 0) {
      SDL_LOG_WARN(
          "Invalid pointer to application was passed."
          "Skip current application.");
      return;
    }
    DeviceParams device_params = GetDeviceParams(
        app->device(),
        application_manager_.connection_handler().get_session_observer());
    const std::string app_id = app->policy_app_id();
    if (device_params.device_mac_address.empty()) {
      SDL_LOG_WARN("Couldn't find device, which hosts application " << app_id);
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
    , last_activated_app_id_(0)
#ifndef EXTERNAL_PROPRIETARY_MODE
    , last_ptu_app_id_(0)
#endif  // EXTERNAL_PROPRIETARY_MODE
    , statistic_manager_impl_(std::make_shared<StatisticManagerImpl>(this))
    , settings_(settings)
    , application_manager_(application_manager)
    , last_registered_policy_app_id_(std::string()) {
}

PolicyHandler::~PolicyHandler() {}

PolicyEncryptionFlagGetterInterfaceSPtr
PolicyHandler::PolicyEncryptionFlagGetter() const {
  return LoadPolicyManager();
}

bool PolicyHandler::PolicyEnabled() const {
  return get_settings().enable_policy();
}

bool PolicyHandler::LoadPolicyLibrary() {
  SDL_LOG_AUTO_TRACE();

  auto create_policy_manager_instance = [this]() {
    if (!PolicyEnabled()) {
      SDL_LOG_WARN(
          "System is configured to work without policy "
          "functionality.");
      return std::shared_ptr<PolicyManager>();
    }

    ExchangePolicyManager(nullptr);
    sync_primitives::AutoWriteLock lock(policy_manager_lock_);
    void* const dl_policy_handle = dlopen(kLibrary.c_str(), RTLD_LAZY);

    if (!dl_policy_handle) {
      SDL_LOG_ERROR("An error occurs while calling dlopen");
      return std::shared_ptr<PolicyManager>();
    }

    typedef PolicyManager* (*CreateManager)(logger::Logger*);
    typedef void (*DeleteManager)(PolicyManager*);

    CreateManager create_manager = reinterpret_cast<CreateManager>(
        dlsym(dl_policy_handle, "CreateManager"));
    char* error_string = dlerror();
    if (NULL != error_string) {
      SDL_LOG_WARN(error_string);
      dlclose(dl_policy_handle);
      return std::shared_ptr<PolicyManager>();
    }

    DeleteManager delete_manager = reinterpret_cast<DeleteManager>(
        dlsym(dl_policy_handle, "DeleteManager"));

    error_string = dlerror();
    if (NULL != error_string) {
      SDL_LOG_WARN(error_string);
      dlclose(dl_policy_handle);
      return std::shared_ptr<PolicyManager>();
    }

    auto policy_destroyer = [delete_manager,
                             dl_policy_handle](PolicyManager* policy_manager) {
      SDL_LOG_DEBUG("Delete Policy Manager");
      delete_manager(policy_manager);
      dlclose(dl_policy_handle);
    };

    auto policy_manager = std::shared_ptr<PolicyManager>(
        create_manager(&logger::Logger::instance()), policy_destroyer);

    policy_manager->set_listener(this);

    return policy_manager;
  };

  auto policy_manager = create_policy_manager_instance();

  if (!policy_manager) {
    return false;
  }

  ExchangePolicyManager(policy_manager);
  event_observer_ = std::make_shared<PolicyEventObserver>(
      this, application_manager_.event_dispatcher());
  return true;
}

const PolicySettings& PolicyHandler::get_settings() const {
  return settings_;
}

bool PolicyHandler::InitPolicyTable() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false)

  std::string preloaded_file = get_settings().preloaded_pt_file();
  if (file_system::FileExists(preloaded_file)) {
    const bool pt_inited =
        policy_manager->InitPT(preloaded_file, &get_settings());
    OnPTInited();
    return pt_inited;
  }
  SDL_LOG_FATAL("The file which contains preloaded PT is not exist");
  return false;
}

void PolicyHandler::OnPTInited() {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock lock(listeners_lock_);

  std::for_each(listeners_.begin(),
                listeners_.end(),
                std::mem_fun(&PolicyHandlerObserver::OnPTInited));
}

void PolicyHandler::StopRetrySequence() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
#ifndef EXTERNAL_PROPRIETARY_MODE
  // Clear cached PTU app
  last_ptu_app_id_ = 0;
#endif  // EXTERNAL_PROPRIETARY_MODE
  policy_manager->StopRetrySequence();
}

bool PolicyHandler::ResetPolicyTable() {
  SDL_LOG_TRACE("Reset policy table.");
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false)
  std::string preloaded_file = get_settings().preloaded_pt_file();
  if (file_system::FileExists(preloaded_file)) {
    return policy_manager->ResetPT(preloaded_file);
  }
  SDL_LOG_WARN("The file which contains preloaded PT is not exist");
  return false;
}

bool PolicyHandler::ClearUserConsent() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false)
  return policy_manager->ResetUserConsent();
}

#ifndef EXTERNAL_PROPRIETARY_MODE
uint32_t PolicyHandler::ChoosePTUApplication(
    const PTUIterationType iteration_type) {
  SDL_LOG_AUTO_TRACE();

  // Return the previous app chosen if this is a retry for a PTU in progress
  if (iteration_type == PTUIterationType::RetryIteration &&
      last_ptu_app_id_ != 0) {
    ApplicationSharedPtr app =
        application_manager_.application(last_ptu_app_id_);
    if (app && app->IsRegistered()) {
      SDL_LOG_INFO("Previously chosen application exists, returning");
      return last_ptu_app_id_;
    }
  }

  last_ptu_app_id_ = GetAppIdForSending();
  return last_ptu_app_id_;
}

void PolicyHandler::CacheRetryInfo(const uint32_t app_id,
                                   const std::string url,
                                   const std::string snapshot_path) {
  last_ptu_app_id_ = app_id;
  retry_update_url_ = url;
  policy_snapshot_path_ = snapshot_path;
}
#endif  // EXTERNAL_PROPRIETARY_MODE

uint32_t PolicyHandler::GetAppIdForSending() const {
  SDL_LOG_AUTO_TRACE();

  // fix ApplicationSet access crash
  const ApplicationSet accessor = application_manager_.applications().GetData();

  HMILevelPredicate has_none_level(mobile_api::HMILevel::HMI_NONE);
  Applications apps_without_none_level;
  std::copy_if(accessor.begin(),
               accessor.end(),
               std::back_inserter(apps_without_none_level),
               std::not1(has_none_level));

  SDL_LOG_DEBUG("Number of apps with different from NONE level: "
                << apps_without_none_level.size());

  uint32_t app_id = ChooseRandomAppForPolicyUpdate(apps_without_none_level);

  if (app_id) {
    return app_id;
  }

  Applications apps_with_none_level;
  std::copy_if(accessor.begin(),
               accessor.end(),
               std::back_inserter(apps_with_none_level),
               has_none_level);

  SDL_LOG_DEBUG(
      "Number of apps with NONE level: " << apps_with_none_level.size());

  if (apps_with_none_level.empty()) {
    SDL_LOG_WARN("There is no registered application");
    return 0;
  }

  return ChooseRandomAppForPolicyUpdate(apps_with_none_level);
}

void PolicyHandler::PushAppIdToPTUQueue(const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  sync_primitives::AutoLock lock(app_id_queue_lock_);
  const auto result = applications_ptu_queue_.insert(app_id);
  if (result.second) {
    policy_manager->UpdatePTUReadyAppsCount(applications_ptu_queue_.size());
  }
}

void PolicyHandler::PopAppIdFromPTUQueue() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  sync_primitives::AutoLock lock(app_id_queue_lock_);
  if (applications_ptu_queue_.size() > 0) {
    applications_ptu_queue_.erase(applications_ptu_queue_.begin());
    policy_manager->UpdatePTUReadyAppsCount(applications_ptu_queue_.size());
  }
}

#ifdef EXTERNAL_PROPRIETARY_MODE
PTURetryHandler& PolicyHandler::ptu_retry_handler() const {
  SDL_LOG_AUTO_TRACE();
  return *atomic_policy_manager_;
}

void PolicyHandler::OnAppPermissionConsent(
    const uint32_t connection_key,
    const PermissionConsent& permissions,
    const ExternalConsentStatus& external_consent_status) {
  SDL_LOG_AUTO_TRACE();
  AsyncRun(new AppPermissionDelegate(
      connection_key, permissions, external_consent_status, *this));
}
#else
void PolicyHandler::OnAppPermissionConsent(
    const uint32_t connection_key, const PermissionConsent& permissions) {
  SDL_LOG_AUTO_TRACE();
  AsyncRun(new AppPermissionDelegate(connection_key, permissions, *this));
}

#endif

void PolicyHandler::OnDeviceConsentChanged(const std::string& device_id,
                                           const bool is_allowed) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  connection_handler::DeviceHandle device_handle;
  if (!application_manager_.connection_handler().GetDeviceID(device_id,
                                                             &device_handle)) {
    SDL_LOG_ERROR("Unable to get device handle for device_id: " << device_id);
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
      if (policy_manager->IsPredataPolicy(policy_app_id) && !is_allowed) {
        continue;
      }

      policy_manager->ReactOnUserDevConsentForApp(
          device_handle, policy_app_id, is_allowed);

      policy_manager->SendNotificationOnPermissionsUpdated(device_id,
                                                           policy_app_id);

      if (policy_manager->IsPredataPolicy(policy_app_id) && !is_allowed) {
        SetHeartBeatTimeout(policy_app_id, (*it_app_list)->app_id());
      }
    }
  }
}

void PolicyHandler::SendOnAppPermissionsChanged(
    const AppPermissions& permissions,
    const std::string& device_id,
    const std::string& policy_app_id) const {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("PolicyHandler::SendOnAppPermissionsChanged for "
                << policy_app_id);
  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (app.use_count() == 0) {
    SDL_LOG_WARN("No app found for policy app id = " << policy_app_id);
    return;
  }
  MessageHelper::SendOnAppPermissionsChangedNotification(
      app->app_id(), permissions, application_manager_);
}

void PolicyHandler::SendOnAppPropertiesChangeNotification(
    const std::string& policy_app_id) const {
  SDL_LOG_AUTO_TRACE();
  const auto notification =
      MessageHelper::CreateOnAppPropertiesChangeNotification(
          policy_app_id, application_manager_);
  application_manager_.GetRPCService().ManageHMICommand(notification);
}

void PolicyHandler::OnPTExchangeNeeded() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->ForcePTExchange();
}

void PolicyHandler::GetAvailableApps(std::queue<std::string>& apps) {
  SDL_LOG_INFO("GetAvailable apps");
  const ApplicationSet& app_list =
      application_manager_.applications().GetData();
  ApplicationSetConstIt iter = app_list.begin();

  for (; app_list.end() != iter; ++iter) {
    SDL_LOG_INFO("one more app");
    apps.push((*iter)->policy_app_id());
  }
}

struct SmartObjectToInt {
  int operator()(const smart_objects::SmartObject& item) const {
    return item.asInt();
  }
};

StatusNotifier PolicyHandler::AddApplication(
    const std::string& device_id,
    const std::string& application_id,
    const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager,
                             std::make_shared<utils::CallNothing>());
  return policy_manager->AddApplication(device_id, application_id, hmi_types);
}

void PolicyHandler::AddDevice(const std::string& device_id,
                              const std::string& connection_type) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->AddDevice(device_id, connection_type);
}

void PolicyHandler::SetDeviceInfo(const std::string& device_id,
                                  const DeviceInfo& device_info) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->SetDeviceInfo(device_id, device_info);
}

#ifdef EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnAppPermissionConsentInternal(
    const uint32_t connection_key,
    const ExternalConsentStatus& external_consent_status,
    PermissionConsent& out_permissions) {
  SDL_LOG_AUTO_TRACE();
  const PolicyManager::NotificationMode mode =
      external_consent_status.empty() ? PolicyManager::kNotifyApplicationMode
                                      : PolicyManager::kSilentMode;
#else
void PolicyHandler::OnAppPermissionConsentInternal(
    const uint32_t connection_key, PermissionConsent& out_permissions) {
#endif
  const std::shared_ptr<PolicyManager> policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  if (connection_key) {
    ApplicationSharedPtr app = application_manager_.application(connection_key);

    if (app.use_count() != 0) {
      out_permissions.policy_app_id = app->policy_app_id();
      DeviceParams device_params = GetDeviceParams(
          app->device(),
          application_manager_.connection_handler().get_session_observer());

      out_permissions.device_id = device_params.device_mac_address;
    }

    if (!out_permissions.policy_app_id.empty()) {
#ifdef EXTERNAL_PROPRIETARY_MODE
      policy_manager->SetUserConsentForApp(out_permissions, mode);
#else
      policy_manager->SetUserConsentForApp(out_permissions);
#endif
    }
  } else if (!app_to_device_link_.empty()) {
    const ApplicationSet& accessor =
        application_manager_.applications().GetData();
    for (const auto& app : accessor) {
      // If list of apps sent to HMI for user consents is not the same as
      // current,
      // permissions should be set only for coincident to registered apps
      if (app.use_count() == 0) {
        SDL_LOG_WARN(
            "Invalid pointer to application was passed. "
            "Permissions setting skipped.");
        continue;
      }

      if (!app->IsRegistered()) {
        SDL_LOG_DEBUG("Application " << app->policy_app_id()
                                     << " is not marked as registered.");
        continue;
      }

      DeviceParams device_params = GetDeviceParams(
          app->device(),
          application_manager_.connection_handler().get_session_observer());

      if (device_params.device_mac_address != app->mac_address()) {
        SDL_LOG_WARN(
            "Device_id of application is changed. "
            "Permissions setting skipped.");
        continue;
      }

      out_permissions.policy_app_id = app->policy_app_id();
      out_permissions.device_id = app->mac_address();
#ifdef EXTERNAL_PROPRIETARY_MODE
      policy_manager->SetUserConsentForApp(out_permissions, mode);
#else
      policy_manager->SetUserConsentForApp(out_permissions);
#endif
    }
  } else {
    SDL_LOG_WARN(
        "There are no applications previously stored for "
        "setting common permissions.");
  }
#ifdef EXTERNAL_PROPRIETARY_MODE
  if (!policy_manager->SetExternalConsentStatus(external_consent_status)) {
    SDL_LOG_WARN("External User Consent Settings status has not been set!");
  }
#endif
}

void PolicyHandler::SetHeartBeatTimeout(const std::string& policy_app_id,
                                        const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();

  const std::shared_ptr<PolicyManager> policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  const uint32_t timeout = policy_manager->HeartBeatTimeout(policy_app_id);
  if (0 != timeout) {
    SDL_LOG_DEBUG("SetHeartBeatTimeout for " << app_id << " is " << timeout);
    application_manager_.connection_handler().SetHeartBeatTimeout(app_id,
                                                                  timeout);
  } else {
    SDL_LOG_DEBUG("SetHeartBeatTimeout for " << app_id << "  ignored");
  }
}

void policy::PolicyHandler::SetDaysAfterEpoch() {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  date_time::TimeDuration current_time = date_time::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  int days_after_epoch = date_time::getSecs(current_time) / kSecondsInDay;
  PTUpdatedAt(Counters::DAYS_AFTER_EPOCH, days_after_epoch);
}

#ifdef ENABLE_SECURITY
std::string PolicyHandler::RetrieveCertificate() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::string());
  return policy_manager->RetrieveCertificate();
}
#endif  // ENABLE_SECURITY

void PolicyHandler::OnGetUserFriendlyMessage(
    const std::vector<std::string>& message_codes,
    const std::string& language,
    uint32_t correlation_id) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

#ifdef EXTERNAL_PROPRIETARY_MODE
  const std::string active_hmi_language = application_manager::EnumToString(
      application_manager_.hmi_capabilities().active_ui_language());
  const std::vector<UserFriendlyMessage> result =
      policy_manager->GetUserFriendlyMessages(
          message_codes, language, active_hmi_language);
#else
  const std::vector<UserFriendlyMessage> result =
      policy_manager->GetUserFriendlyMessages(message_codes, language);
#endif  // EXTERNAL_PROPRIETARY_MODE
  // Send response to HMI with gathered data
  MessageHelper::SendGetUserFriendlyMessageResponse(
      result, correlation_id, application_manager_);
}

void PolicyHandler::OnSystemRequestReceived() const {
#ifdef EXTERNAL_PROPRIETARY_MODE
  ptu_retry_handler().OnSystemRequestReceived();
#endif
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->ResetTimeout();
}

void PolicyHandler::TriggerPTUOnStartupIfRequired() {
#ifdef PROPRIETARY_MODE
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->TriggerPTUOnStartupIfRequired();
#endif
}

void PolicyHandler::GetRegisteredLinks(
    std::map<std::string, std::string>& out_links) const {
  DataAccessor<ApplicationSet> accessor = application_manager_.applications();
  ApplicationSetConstIt it_app = accessor.GetData().begin();
  ApplicationSetConstIt it_app_end = accessor.GetData().end();

  LinksCollector linker(application_manager_, out_links);
  std::for_each(it_app, it_app_end, linker);
}

std::shared_ptr<PolicyManager> PolicyHandler::LoadPolicyManager() const {
  sync_primitives::AutoReadLock lock{policy_manager_lock_};
  return atomic_policy_manager_;
}

void PolicyHandler::ExchangePolicyManager(
    std::shared_ptr<PolicyManager> policy_manager) {
  sync_primitives::AutoWriteLock lock{policy_manager_lock_};
  atomic_policy_manager_.swap(policy_manager);
}

std::vector<policy::FunctionalGroupPermission>
PolicyHandler::CollectRegisteredAppsPermissions() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager,
                             std::vector<policy::FunctionalGroupPermission>());
  // If no specific app was passed, get permissions for all currently registered
  // applications
  sync_primitives::AutoLock lock(app_to_device_link_lock_);

  GetRegisteredLinks(app_to_device_link_);

  PermissionsConsolidator consolidator;
  std::vector<policy::FunctionalGroupPermission> group_permissions;
  std::map<std::string, std::string>::const_iterator it =
      app_to_device_link_.begin();
  for (; it != app_to_device_link_.end(); ++it) {
    policy_manager->GetUserConsentForApp(
        it->first, it->second, group_permissions);
    consolidator.Consolidate(group_permissions);
  }
  return consolidator.GetConsolidatedPermissions();
}

std::vector<FunctionalGroupPermission> PolicyHandler::CollectAppPermissions(
    const uint32_t connection_key) {
  std::vector<FunctionalGroupPermission> group_permissions;
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, group_permissions);

  // Single app only
  ApplicationSharedPtr app = application_manager_.application(connection_key);

  if (NULL == app.get() || app.use_count() == 0) {
    SDL_LOG_WARN("Connection key '"
                 << connection_key
                 << "' "
                    "not found within registered applications.");

    return group_permissions;
  }

  DeviceParams device_params = GetDeviceParams(
      app->device(),
      application_manager_.connection_handler().get_session_observer());

  if (device_params.device_mac_address.empty()) {
    SDL_LOG_WARN("Couldn't find device, which hosts application.");
    return group_permissions;
  }

  policy_manager->GetUserConsentForApp(device_params.device_mac_address,
                                       app->policy_app_id(),
                                       group_permissions);

  return group_permissions;
}

void PolicyHandler::OnGetListOfPermissions(const uint32_t connection_key,
                                           const uint32_t correlation_id) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  application_manager::ApplicationSharedPtr app =
      application_manager_.application(connection_key);
  const bool is_app_registered = NULL != app.get();
  const bool is_connection_key_valid = is_app_registered && connection_key;

  const std::vector<policy::FunctionalGroupPermission> permissions =
      is_connection_key_valid ? CollectAppPermissions(connection_key)
                              : CollectRegisteredAppsPermissions();

  if (permissions.empty() && is_connection_key_valid) {
    SDL_LOG_ERROR("No permissions found for application with connection key:"
                  << connection_key);
    return;
  }

  MessageHelper::SendGetListOfPermissionsResponse(
      permissions,
#ifdef EXTERNAL_PROPRIETARY_MODE
      policy_manager->GetExternalConsentStatus(),
#endif  // EXTERNAL_PROPRIETARY_MODE
      correlation_id,
      application_manager_);
}

void PolicyHandler::LinkAppsToDevice() {
  sync_primitives::AutoLock lock(app_to_device_link_lock_);
  LinkAppToDevice linker(app_to_device_link_, application_manager_);
  SDL_LOG_DEBUG("add links to app.  no specific app was passed");
  {
    const ApplicationSet& accessor =
        application_manager_.applications().GetData();
    if (accessor.empty()) {
      SDL_LOG_WARN("application_manager doesn't have any applications");
    } else {
      // Add all currently registered applications
      std::for_each(accessor.begin(), accessor.end(), linker);
    }
  }
}

bool PolicyHandler::IsAppSuitableForPolicyUpdate(
    const Applications::value_type value) const {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);

  if (!value->IsRegistered()) {
    SDL_LOG_DEBUG("Application " << value->app_id()
                                 << " is not marked as registered.");
    return false;
  }

  SDL_LOG_DEBUG("Application " << value->app_id()
                               << " marked as registered."
                                  "Checking its parameters.");

  DeviceParams device_params = GetDeviceParams(
      value->device(),
      application_manager_.connection_handler().get_session_observer());

  const bool is_device_allowed =
      (kDeviceAllowed == policy_manager->GetUserConsentForDevice(
                             device_params.device_mac_address));

  SDL_LOG_DEBUG("Is device " << device_params.device_mac_address << " allowed "
                             << std::boolalpha << is_device_allowed);
  return is_device_allowed;
}

uint32_t PolicyHandler::ChooseRandomAppForPolicyUpdate(
    Applications& app_list) const {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->OnDeviceSwitching(device_id_from, device_id_to);
}

void PolicyHandler::OnLockScreenDismissalStateChanged() {
  SDL_LOG_AUTO_TRACE();
  const auto accessor = application_manager_.applications();
  const auto apps = accessor.GetData();
  for (auto& app : apps) {
    application_manager_.SendDriverDistractionState(app);
  }
}

void PolicyHandler::OnGetStatusUpdate(const uint32_t correlation_id) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  MessageHelper::SendGetStatusUpdateResponse(
      policy_manager->GetPolicyTableStatus(),
      correlation_id,
      application_manager_);
}

void PolicyHandler::OnUpdateStatusChanged(const std::string& status) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->SaveUpdateStatusRequired(policy::kUpToDate != status);
  MessageHelper::SendOnStatusUpdate(status, application_manager_);
}

std::string PolicyHandler::OnCurrentDeviceIdUpdateRequired(
    const transport_manager::DeviceHandle& device_handle,
    const std::string& policy_app_id) {
  SDL_LOG_AUTO_TRACE();
  DeviceParams device_params = GetDeviceParams(
      device_handle,
      application_manager_.connection_handler().get_session_observer());

  ApplicationSharedPtr app = application_manager_.application(
      device_params.device_mac_address, policy_app_id);

  if (app.use_count() == 0) {
    SDL_LOG_WARN("Application with id '"
                 << policy_app_id
                 << "' "
                    "not found within registered applications.");
    return std::string();
  }

  return device_params.device_mac_address;
}

void PolicyHandler::OnSystemInfoChanged(const std::string& language) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->SetSystemLanguage(language);
}

void PolicyHandler::SetPreloadedPtFlag(const bool is_preloaded) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->SetPreloadedPtFlag(is_preloaded);
}

void PolicyHandler::OnGetSystemInfo(const std::string& ccpu_version,
                                    const std::string& wers_country_code,
                                    const std::string& language) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->SetSystemInfo(ccpu_version, wers_country_code, language);
}

std::string PolicyHandler::GetCCPUVersionFromPT() const {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::string());
  return policy_manager->GetCCPUVersionFromPT();
}

void PolicyHandler::OnVIIsReady() {
  SDL_LOG_AUTO_TRACE();
  const uint32_t correlation_id =
      application_manager_.GetNextHMICorrelationID();

  std::vector<std::string> params;
  params.push_back(strings::vin);

  MessageHelper::CreateGetVehicleDataRequest(
      correlation_id, params, application_manager_);
}

void PolicyHandler::OnVehicleDataUpdated(
    const smart_objects::SmartObject& message) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
#ifdef EXTERNAL_PROPRIETARY_MODE
  if (!message.keyExists(strings::msg_params)) {
    SDL_LOG_ERROR("Message does not contains mandatory section "
                  << strings::msg_params);
    return;
  }
  if (message[strings::msg_params].keyExists(strings::vin)) {
    policy_manager->SetVINValue(
        message[strings::msg_params][strings::vin].asString());
  }
#else
  SDL_LOG_DEBUG("This functionality is not available for not external policy");
#endif
}

void PolicyHandler::OnPendingPermissionChange(
    const std::string& device_id, const std::string& policy_app_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("PolicyHandler::OnPendingPermissionChange for "
                << policy_app_id);
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (app.use_count() == 0) {
    SDL_LOG_WARN("No app found for " << policy_app_id << " policy app id.");
    return;
  }

  AppPermissions permissions =
      policy_manager->GetAppPermissionsChanges(device_id, policy_app_id);

  const uint32_t app_id = app->app_id();

  if (permissions.appRevoked) {
    application_manager::MessageHelper::SendOnAppPermissionsChangedNotification(
        app_id, permissions, application_manager_);
    application_manager_.state_controller().SetRegularState(
        app,
        mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
        mobile_apis::HMILevel::HMI_NONE,
        mobile_apis::AudioStreamingState::NOT_AUDIBLE,
        mobile_apis::VideoStreamingState::NOT_STREAMABLE,
        true);
    policy_manager->RemovePendingPermissionChanges(policy_app_id);
    return;
  }

  mobile_apis::HMILevel::eType app_hmi_level =
      app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);

  switch (app_hmi_level) {
    case mobile_apis::HMILevel::eType::HMI_FULL:
    case mobile_apis::HMILevel::eType::HMI_LIMITED: {
      if (permissions.appPermissionsConsentNeeded) {
        MessageHelper::SendOnAppPermissionsChangedNotification(
            app->app_id(), permissions, application_manager_);
        // "Break" statement has to be here to continue processing in case of
        // there is another "true" flag in permissions struct
        break;
      }
    }
    case mobile_apis::HMILevel::eType::HMI_BACKGROUND: {
      if (permissions.isAppPermissionsRevoked) {
        MessageHelper::SendOnAppPermissionsChangedNotification(
            app->app_id(), permissions, application_manager_);
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
    application_manager_.GetRPCService().ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app->app_id(),
            mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED),
        commands::Command::SOURCE_SDL);

    application_manager_.OnAppUnauthorized(app->app_id());
  }

  if (permissions.requestTypeChanged || permissions.requestSubTypeChanged) {
    MessageHelper::SendOnAppPermissionsChangedNotification(
        app->app_id(), permissions, application_manager_);
  }

  policy_manager->RemovePendingPermissionChanges(policy_app_id);
}

bool PolicyHandler::SendMessageToSDK(const BinaryMessage& pt_string,
                                     const std::string& url) {
  const uint32_t app_id = GetAppIdForSending();
  return SendMessageToSDK(pt_string, url, app_id);
}

bool PolicyHandler::SendMessageToSDK(const BinaryMessage& pt_string,
                                     const std::string& url,
                                     const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  POLICY_LIB_CHECK_OR_RETURN(atomic_policy_manager_, false);

  ApplicationSharedPtr app = application_manager_.application(app_id);

  if (!app) {
    SDL_LOG_WARN(
        "There is no registered application with "
        "connection key '"
        << app_id << "'");
    return false;
  }

  const std::string& mobile_app_id = app->policy_app_id();
  if (mobile_app_id.empty()) {
    SDL_LOG_WARN("Application with connection key '"
                 << app_id
                 << "'"
                    " has no application id.");
    return false;
  }

  SDL_LOG_DEBUG("Update url is " << url
                                 << " for application "
                                    "with connection key "
                                 << app_id);

  MessageHelper::SendPolicySnapshotNotification(
      app_id, pt_string, url, application_manager_);

  return true;
}

bool PolicyHandler::ReceiveMessageFromSDK(const std::string& file,
                                          const BinaryMessage& pt_string) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);

  const auto load_pt_result = policy_manager->LoadPT(file, pt_string);

  SDL_LOG_INFO("Load policy table result code: " << load_pt_result);

  const bool is_ptu_successful =
      load_pt_result == PolicyManager::PtProcessingResult::kSuccess;
  OnPTUFinished(is_ptu_successful);

  if (is_ptu_successful) {
    SDL_LOG_INFO("PTU was successful.");
    policy_manager->CleanupUnpairedDevices();
    SetDaysAfterEpoch();
    policy_manager->OnPTUFinished(load_pt_result);
#ifndef EXTERNAL_PROPRIETARY_MODE
    // Clean up retry information
    last_ptu_app_id_ = 0;
#endif  // EXTERNAL_PROPRIETARY_MODE

    uint32_t correlation_id = application_manager_.GetNextHMICorrelationID();
    event_observer_->subscribe_on_event(
        hmi_apis::FunctionID::VehicleInfo_GetVehicleData, correlation_id);
    std::vector<std::string> vehicle_data_args;
    vehicle_data_args.push_back(strings::odometer);
    MessageHelper::CreateGetVehicleDataRequest(
        correlation_id, vehicle_data_args, application_manager_);
  } else {
    SDL_LOG_WARN("Exchange wasn't successful");
    policy_manager->OnPTUFinished(load_pt_result);
  }

  return is_ptu_successful;
}

bool PolicyHandler::UnloadPolicyLibrary() {
  SDL_LOG_AUTO_TRACE();
  AsyncRunner::Stop();
  ExchangePolicyManager(nullptr);
  return !atomic_policy_manager_;
}

#ifdef EXTERNAL_PROPRIETARY_MODE
struct SDLAlowedNotification {
  SDLAlowedNotification(sync_primitives::RWLock& policy_manager_lock,
                        const connection_handler::DeviceHandle& device_id,
                        PolicyManager* policy_manager,
                        StateController& state_controller)
      : policy_manager_lock_(policy_manager_lock)
      , device_id_(device_id)
      , policy_manager_(policy_manager)
      , state_controller_(state_controller) {}

  void operator()(const ApplicationSharedPtr& app) {
    POLICY_LIB_CHECK_VOID(policy_manager_);
    if (app->device() == device_id_) {
      std::string hmi_level;
      mobile_apis::HMILevel::eType default_mobile_hmi;
      policy_manager_->GetDefaultHmi(
          app->mac_address(), app->policy_app_id(), &hmi_level);
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
      state_controller_.SetRegularState(
          app,
          mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
          default_mobile_hmi,
          true);
    }
  }

 private:
  sync_primitives::RWLock& policy_manager_lock_;
  connection_handler::DeviceHandle device_id_;
  PolicyManager* policy_manager_;
  StateController& state_controller_;
};
#endif  // EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::OnAllowSDLFunctionalityNotification(
    bool is_allowed, const std::string& device_mac) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
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
        SDL_LOG_WARN("Device with id " << device_id << " wasn't found.");
        continue;
      }
      policy_manager->SetUserConsentForDevice(device_id, is_allowed);
      connection_handler::DeviceHandle device_handle = 0;
      if (!connection_handler.GetDeviceID(device_id, &device_handle)) {
        SDL_LOG_WARN("Device handle with mac " << device_id
                                               << " wasn't found.");
      }

#ifdef EXTERNAL_PROPRIETARY_MODE
      ApplicationSet applications;
      {
        DataAccessor<ApplicationSet> accessor =
            application_manager_.applications();
        applications = accessor.GetData();
      }
      if (!is_allowed) {
        std::for_each(
            applications.begin(),
            applications.end(),
            DeactivateApplication(device_handle,
                                  application_manager_.state_controller()));
      } else {
        std::for_each(
            applications.begin(),
            applications.end(),
            SDLAlowedNotification(policy_manager_lock_,
                                  device_handle,
                                  policy_manager.get(),
                                  application_manager_.state_controller()));
      }
#endif  // EXTERNAL_PROPRIETARY_MODE
    }
  }

  // Case, when specific device was changed
  connection_handler::DeviceHandle device_handle = 0u;
  if (device_specific) {
    policy_manager->SetUserConsentForDevice(device_mac, is_allowed);
    if (!connection_handler.GetDeviceID(device_mac, &device_handle)) {
      SDL_LOG_WARN("Device hadle with mac " << device_mac << " wasn't found.");
    }
    DeviceHandles::iterator it = std::find(pending_device_handles_.begin(),
                                           pending_device_handles_.end(),
                                           device_handle);
    // If consent done from HMI menu
    if (it == pending_device_handles_.end()) {
      SDL_LOG_WARN("No pendining application activation");
      return;
    }

    pending_device_handles_.erase(it);
  }

#ifdef EXTERNAL_PROPRIETARY_MODE
  if (last_activated_app_id_) {
    ApplicationSharedPtr app =
        application_manager_.application(last_activated_app_id_);

    if (!app) {
      SDL_LOG_WARN("Application with id '"
                   << last_activated_app_id_
                   << "' not found among registered applications.");
      return;
    }

    bool is_allowed_by_policies = true;
    if (PolicyEnabled()) {
      is_allowed_by_policies =
          !policy_manager->IsApplicationRevoked(app->policy_app_id());
    }

    if (is_allowed && is_allowed_by_policies) {
      // Send HMI status notification to mobile
      // Put application in full
      AudioStreamingState::eType audio_state =
          app->IsAudioApplication() ? AudioStreamingState::AUDIBLE
                                    : AudioStreamingState::NOT_AUDIBLE;
      VideoStreamingState::eType video_state =
          app->IsVideoApplication() ? VideoStreamingState::STREAMABLE
                                    : VideoStreamingState::NOT_STREAMABLE;

      application_manager_.state_controller().SetRegularState(
          app,
          mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
          mobile_apis::HMILevel::HMI_FULL,
          audio_state,
          video_state,
          true);
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
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->IncrementIgnitionCycles();
}

void PolicyHandler::OnActivateApp(uint32_t connection_key,
                                  uint32_t correlation_id) {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key);
  if (app.use_count() == 0) {
    SDL_LOG_WARN("Activated App failed: no app found.");
    return;
  }
  std::string policy_app_id = app->policy_app_id();

  AppPermissions permissions(policy_app_id);

  const auto policy_manager = LoadPolicyManager();
  if (!policy_manager) {
    SDL_LOG_DEBUG("The shared library of policy is not loaded");
    if (!PolicyEnabled()) {
      permissions.isSDLAllowed = true;
    }
  } else {
    permissions = policy_manager->GetAppPermissionsChanges(app->mac_address(),
                                                           policy_app_id);
#ifdef EXTERNAL_PROPRIETARY_MODE
    UsageStatistics& usage = app->usage_report();

    usage.RecordAppUserSelection();
    DeviceParams device_params = GetDeviceParams(
        app->device(),
        application_manager_.connection_handler().get_session_observer());
    permissions.deviceInfo = device_params;

    DeviceConsent consent = policy_manager->GetUserConsentForDevice(
        permissions.deviceInfo.device_mac_address);
    permissions.isSDLAllowed = kDeviceAllowed == consent;

    // According to the SDLAQ-CRS-2794, p.9
    // 'priority' should be omitted in case when device
    // is not allowed.
    if (!permissions.isSDLAllowed) {
      permissions.priority.clear();
      if (!permissions.appRevoked) {
        last_activated_app_id_ = connection_key;
      }
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
    policy_manager->RemovePendingPermissionChanges(policy_app_id);
  }
  // If application is revoked it should not be activated
  // In this case we need to activate application
  if (false == permissions.appRevoked && true == permissions.isSDLAllowed) {
    SDL_LOG_INFO("Application will be activated");
    if (application_manager_.ActivateApplication(app)) {
      last_activated_app_id_ = 0;
    }
  } else {
    SDL_LOG_WARN("Application should not be activated");
  }

  MessageHelper::SendSDLActivateAppResponse(
      permissions, correlation_id, application_manager_);
}

void PolicyHandler::KmsChanged(int kilometers) {
  SDL_LOG_DEBUG("PolicyHandler::KmsChanged " << kilometers << " kilometers");
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->KmsChanged(kilometers);
}

void PolicyHandler::PTExchangeAtUserRequest(uint32_t correlation_id) {
  SDL_LOG_TRACE("PT exchange at user request");
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  std::string update_status = policy_manager->ForcePTExchangeAtUserRequest();
  MessageHelper::SendUpdateSDLResponse(
      update_status, correlation_id, application_manager_);
}

void PolicyHandler::OnPermissionsUpdated(const std::string& device_id,
                                         const std::string& policy_app_id,
                                         const Permissions& permissions,
                                         const HMILevel& default_hmi) {
  SDL_LOG_AUTO_TRACE();
  OnPermissionsUpdated(device_id, policy_app_id, permissions);

  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (app.use_count() == 0) {
    SDL_LOG_WARN(
        "Connection_key not found for application_id: " << policy_app_id);
    return;
  }

  // The application currently not running (i.e. in NONE) should change HMI
  // level to default
  mobile_apis::HMILevel::eType current_hmi_level =
      app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  auto hmi_level = StringToEnum<mobile_apis::HMILevel::eType>(default_hmi);

  if (mobile_apis::HMILevel::INVALID_ENUM == hmi_level) {
    SDL_LOG_WARN("Couldn't convert default hmi level " << default_hmi
                                                       << " to enum.");
    return;
  }
  if (current_hmi_level == hmi_level) {
    SDL_LOG_DEBUG("Application already in default hmi state.");
    return;
  }
  switch (current_hmi_level) {
    case mobile_apis::HMILevel::HMI_NONE: {
      SDL_LOG_INFO("Changing hmi level of application "
                   << policy_app_id << " to default hmi level " << default_hmi);

      if (hmi_level == mobile_apis::HMILevel::HMI_FULL) {
        application_manager_.state_controller().SetRegularState(
            app,
            mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
            hmi_level,
            true);
      } else {
        application_manager_.state_controller().SetRegularState(
            app,
            mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
            hmi_level,
            false);
      }
      break;
    }
    default:
      SDL_LOG_WARN("Application " << policy_app_id
                                  << " is running."
                                     "HMI level won't be changed.");
      break;
  }
}

void PolicyHandler::OnPermissionsUpdated(const std::string& device_id,
                                         const std::string& policy_app_id,
                                         const Permissions& permissions) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (app.use_count() == 0) {
    SDL_LOG_WARN(
        "Connection_key not found for application_id: " << policy_app_id);
    return;
  }

  const auto require_encryption =
      policy_manager->GetAppEncryptionRequired(policy_app_id);

  MessageHelper::SendOnPermissionsChangeNotification(
      app->app_id(), permissions, application_manager_, require_encryption);

  SDL_LOG_DEBUG("Notification sent for application_id: "
                << policy_app_id << " and connection_key " << app->app_id());
}

void PolicyHandler::OnPTUTimeOut() {
  PopAppIdFromPTUQueue();
#ifndef EXTERNAL_PROPRIETARY_MODE
  application_manager_.protocol_handler().ProcessFailedPTU();
#endif
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
    SDL_LOG_ERROR("Failed to write snapshot file to " << snap_path);
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
        timeout_exchange_ms / date_time::MILLISECONDS_IN_SECOND;
    MessageHelper::SendPolicyUpdate(policy_snapshot_full_path,
                                    timeout_exchange_s,
                                    retry_delay_seconds,
                                    application_manager_);
  }
}
#else  // EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnSnapshotCreated(const BinaryMessage& pt_string,
                                      const PTUIterationType iteration_type) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
#ifdef PROPRIETARY_MODE
  if (PTUIterationType::RetryIteration == iteration_type) {
    uint32_t app_id_for_sending = 0;
    const std::string& url =
        GetNextUpdateUrl(PTUIterationType::RetryIteration, app_id_for_sending);
    if (0 != url.length() && !policy_snapshot_path_.empty()) {
      MessageHelper::SendPolicySnapshotNotification(
          app_id_for_sending, policy_snapshot_path_, url, application_manager_);
    }
  } else {
    std::string policy_snapshot_full_path;
    if (!SaveSnapshot(pt_string, policy_snapshot_full_path)) {
      SDL_LOG_ERROR("Snapshot processing skipped.");
      return;
    }

    MessageHelper::SendPolicyUpdate(
        policy_snapshot_full_path,
        TimeoutExchangeSec(),
        policy_manager->RetrySequenceDelaysSeconds(),
        application_manager_);
  }
#else   // PROPRIETARY_MODE
  SDL_LOG_INFO("HTTP policy");

  uint32_t app_id_for_sending = 0;
  const std::string& url = GetNextUpdateUrl(iteration_type, app_id_for_sending);
  if (0 != url.length()) {
    SendMessageToSDK(pt_string, url, app_id_for_sending);
  }
#endif  // PROPRIETARY_MODE
}

std::string PolicyHandler::GetNextUpdateUrl(
    const PTUIterationType iteration_type, uint32_t& app_id) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::string());
  app_id = ChoosePTUApplication(iteration_type);

  if (0 == app_id) {
    return std::string();
  }

  // Use cached URL for retries if it was provided by the HMI
  if (PTUIterationType::RetryIteration == iteration_type &&
      !retry_update_url_.empty()) {
    return retry_update_url_;
  }

  EndpointUrls endpoint_urls;
  policy_manager->GetUpdateUrls("0x07", endpoint_urls);

  if (endpoint_urls.empty()) {
    SDL_LOG_ERROR("Service URLs are empty!");
    return std::string();
  }

  auto get_ptu_app = [this](AppIdURL app_url, uint32_t& app_id) {
    if (app_url.first == 0 && app_url.second == 0) {
      // We've looped past the end of the list, choose new application
      app_id = ChoosePTUApplication(PTUIterationType::DefaultIteration);
      if (0 == app_id) {
        return ApplicationSharedPtr();
      }
    }
    return application_manager_.application(app_id);
  };

  AppIdURL app_url = policy_manager->GetNextUpdateUrl(endpoint_urls);
  ApplicationSharedPtr app = get_ptu_app(app_url, app_id);
  if (!app) {
    SDL_LOG_ERROR("No available applications for PTU!");
    return std::string();
  }
  EndpointData& data = endpoint_urls[app_url.first];
  while (!IsUrlAppIdValid(app->policy_app_id(), data)) {
    app_url = policy_manager->GetNextUpdateUrl(endpoint_urls);
    app = get_ptu_app(app_url, app_id);
    if (!app) {
      SDL_LOG_ERROR("No available applications for PTU!");
      return std::string();
    }
    data = endpoint_urls[app_url.first];
  }
  const std::string& url = data.url[app_url.second];
  return url;
}
#endif  // EXTERNAL_PROPRIETARY_MODE

bool PolicyHandler::GetPriority(const std::string& policy_app_id,
                                std::string* priority) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->GetPriority(policy_app_id, priority);
}

void PolicyHandler::CheckPermissions(
    const application_manager::ApplicationSharedPtr app,
    const application_manager::WindowID window_id,
    const PTString& rpc,
    const RPCParams& rpc_params,
    CheckPermissionResult& result) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  const std::string hmi_level =
      application_manager::EnumToString(app->hmi_level(window_id));
  if (hmi_level.empty()) {
    SDL_LOG_WARN("HMI level for " << app->policy_app_id() << " is invalid, rpc "
                                  << rpc << " is not allowed.");
    result.hmi_level_permitted = policy::kRpcDisallowed;
    return;
  }
  const std::string device_id = MessageHelper::GetDeviceMacAddressForHandle(
      app->device(), application_manager_);
  SDL_LOG_INFO("Checking permissions for  " << app->policy_app_id() << " in "
                                            << hmi_level << " on device "
                                            << device_id << " rpc " << rpc);
  policy_manager->CheckPermissions(
      device_id, app->policy_app_id(), hmi_level, rpc, rpc_params, result);
}

uint32_t PolicyHandler::GetNotificationsNumber(const std::string& priority,
                                               const bool is_subtle) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, 0);
  return policy_manager->GetNotificationsNumber(priority, is_subtle);
}

DeviceConsent PolicyHandler::GetUserConsentForDevice(
    const std::string& device_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, kDeviceDisallowed);
  return policy_manager->GetUserConsentForDevice(device_id);
}

Json::Value PolicyHandler::GetPolicyTableData() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, Json::Value());
  return policy_manager->GetPolicyTableData();
}

bool PolicyHandler::GetDefaultHmi(const std::string& device_id,
                                  const std::string& policy_app_id,
                                  std::string* default_hmi) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->GetDefaultHmi(device_id, policy_app_id, default_hmi);
}

bool PolicyHandler::GetInitialAppData(const std::string& application_id,
                                      StringArray* nicknames,
                                      StringArray* app_hmi_types) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->GetInitialAppData(
      application_id, nicknames, app_hmi_types);
}

void PolicyHandler::GetUpdateUrls(const std::string& service_type,
                                  EndpointUrls& out_end_points) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->GetUpdateUrls(service_type, out_end_points);
}

void PolicyHandler::GetUpdateUrls(const uint32_t service_type,
                                  EndpointUrls& out_end_points) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->GetUpdateUrls(service_type, out_end_points);
}

std::string PolicyHandler::GetLockScreenIconUrl(
    const std::string& policy_app_id) const {
  const std::string default_url;
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, default_url);

  EndpointUrls endpoints;
  policy_manager->GetUpdateUrls("lock_screen_icon_url", endpoints);

  auto it_specific =
      std::find_if(endpoints.begin(),
                   endpoints.end(),
                   [&policy_app_id](const EndpointData& endpoint) {
                     return endpoint.app_id == policy_app_id;
                   });

  if (endpoints.end() != it_specific && !it_specific->url.empty()) {
    SDL_LOG_DEBUG("Specific app URL will be used for " << policy_app_id);
    return it_specific->url.front();
  }

  auto it_default = std::find_if(
      endpoints.begin(), endpoints.end(), [](const EndpointData& endpoint) {
        return endpoint.app_id == kDefaultId;
      });

  if (endpoints.end() != it_default && !it_default->url.empty()) {
    SDL_LOG_DEBUG("Default URL will be used for " << policy_app_id);
    return it_default->url.front();
  }

  SDL_LOG_ERROR("Can't find URL for " << policy_app_id);
  return std::string();
}

std::string PolicyHandler::GetIconUrl(const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::string());
  return policy_manager->GetIconUrl(policy_app_id);
}

uint32_t PolicyHandler::NextRetryTimeout() {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, 0);
  SDL_LOG_AUTO_TRACE();
  return policy_manager->NextRetryTimeout();
}

uint32_t PolicyHandler::TimeoutExchangeSec() const {
  return TimeoutExchangeMSec() / date_time::MILLISECONDS_IN_SECOND;
}

uint32_t PolicyHandler::TimeoutExchangeMSec() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, 0);
  return policy_manager->TimeoutExchangeMSec();
}

void PolicyHandler::OnExceededTimeout() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  std::for_each(listeners_.begin(),
                listeners_.end(),
                std::mem_fn(&PolicyHandlerObserver::OnPTUTimeoutExceeded));

  policy_manager->OnExceededTimeout();
}

const boost::optional<bool> PolicyHandler::LockScreenDismissalEnabledState()
    const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, boost::optional<bool>());
  return policy_manager->LockScreenDismissalEnabledState();
}

const boost::optional<std::string>
PolicyHandler::LockScreenDismissalWarningMessage(
    const std::string& language) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, boost::optional<std::string>());
  return policy_manager->LockScreenDismissalWarningMessage(language);
}

void PolicyHandler::PTUpdatedAt(Counters counter, int value) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->PTUpdatedAt(counter, value);
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

std::shared_ptr<usage_statistics::StatisticsManager>
PolicyHandler::GetStatisticManager() const {
  return statistic_manager_impl_;
}

void PolicyHandler::AddStatisticsInfo(int type) {
  POLICY_LIB_CHECK_VOID(atomic_policy_manager_);
  switch (static_cast<hmi_apis::Common_StatisticsType::eType>(type)) {
    case hmi_apis::Common_StatisticsType::iAPP_BUFFER_FULL: {
      usage_statistics::GlobalCounter count_of_iap_buffer_full(
          GetStatisticManager(), usage_statistics::IAP_BUFFER_FULL);
      ++count_of_iap_buffer_full;
      break;
    }
    default: { SDL_LOG_WARN("Type of statistics is unknown"); }
  }
}

void PolicyHandler::OnSystemError(int code) {
  POLICY_LIB_CHECK_VOID(atomic_policy_manager_);
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
    default: { SDL_LOG_WARN("System error is unknown"); }
  }
}

custom_str::CustomString PolicyHandler::GetAppName(
    const std::string& policy_app_id) {
  ApplicationSharedPtr app =
      application_manager_.application_by_policy_id(policy_app_id);

  if (app.use_count() == 0) {
    SDL_LOG_WARN(
        "Connection_key not found for application_id: " << policy_app_id);
    return custom_str::CustomString("");
  }
  return app->name();
}

void PolicyHandler::OnUpdateHMIAppType(
    std::map<std::string, StringArray> app_hmi_types) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    (*it)->OnUpdateHMIAppType(app_hmi_types);
  }
}
#ifdef EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::OnCertificateUpdated(const std::string& certificate_data) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_DEBUG("Saving encrypted certificate data: \"" << certificate_data
                                                        << '"');
  if (!is_written) {
    SDL_LOG_ERROR("Unable to save encrypted certificate to file " << file_name);
    return;
  }
  SDL_LOG_DEBUG("Saved encrypted certificate data" << file_name);

  MessageHelper::SendDecryptCertificateToHMI(file_name, application_manager_);
}

void PolicyHandler::OnEmptyCertificateArrived() const {
  SDL_LOG_DEBUG("Empty certificate arrived");
  const std::string empty_certificate;
  sync_primitives::AutoLock lock(listeners_lock_);
  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(std::mem_fun(&PolicyHandlerObserver::OnCertificateUpdated),
                   empty_certificate));
}

void PolicyHandler::OnCertificateDecrypted(bool is_succeeded) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  const std::string file_name =
      file_system::GetAbsolutePath(get_settings().app_storage_folder()) + +"/" +
      kCerficateFileName;

  SDL_LOG_DEBUG("Loading certificate data from file " << file_name);

  utils::ScopeGuard file_deleter =
      utils::MakeGuard(file_system::DeleteFile, file_name);
  UNUSED(file_deleter);

  if (!is_succeeded) {
    SDL_LOG_ERROR("Couldn't delete file " << file_name);
    ProcessCertDecryptFailed();
    return;
  }

  std::string certificate_data;
  if (!file_system::ReadFile(file_name, certificate_data)) {
    SDL_LOG_ERROR("Unable to read certificate from file " << file_name);
    return;
  }
  SDL_LOG_DEBUG("Loaded decrypted certificate data: \"" << certificate_data
                                                        << '"');

  policy_manager->SetDecryptedCertificate(certificate_data);

  sync_primitives::AutoLock lock(listeners_lock_);
  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(std::mem_fun(&PolicyHandlerObserver::OnCertificateUpdated),
                   certificate_data));
}

void PolicyHandler::ProcessCertDecryptFailed() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(listeners_lock_);

  std::for_each(
      listeners_.begin(),
      listeners_.end(),
      std::bind2nd(std::mem_fn(&PolicyHandlerObserver::OnCertDecryptFinished),
                   false));
}

#else   // EXTERNAL_PROPRIETARY_MODE
void PolicyHandler::OnCertificateUpdated(const std::string& certificate_data) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    PolicyHandlerObserver* observer = *it;
    observer->OnCertificateUpdated(certificate_data);
  }
}
#endif  // EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::OnAuthTokenUpdated(const std::string& policy_app_id,
                                       const std::string& auth_token) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(listeners_lock_);
  HandlersCollection::const_iterator it = listeners_.begin();
  for (; it != listeners_.end(); ++it) {
    PolicyHandlerObserver* observer = *it;
    observer->OnAuthTokenUpdated(policy_app_id, auth_token);
  }
}

void PolicyHandler::OnPTUFinished(const bool ptu_result) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(listeners_lock_);

  PopAppIdFromPTUQueue();

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
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  policy_manager->MarkUnpairedDevice(device_id);
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
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);

  return policy_manager->IsApplicationRevoked(app_id);
}

void PolicyHandler::OnUpdateRequestSentToMobile() {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->OnUpdateStarted();
}

bool PolicyHandler::CheckKeepContext(const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->CanAppKeepContext(policy_app_id);
}

bool PolicyHandler::CheckStealFocus(const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->CanAppStealFocus(policy_app_id);
}

bool PolicyHandler::CheckSystemAction(
    mobile_apis::SystemAction::eType system_action,
    const std::string& policy_app_id) const {
  using namespace mobile_apis;
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_DEBUG("Unknown system action");
  return false;
}

std::vector<std::string> PolicyHandler::GetApplicationPolicyIDs() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::vector<std::string>());
  const auto all_policy_ids = policy_manager->GetApplicationPolicyIDs();
  std::vector<std::string> policy_app_ids;

  std::copy_if(
      all_policy_ids.begin(),
      all_policy_ids.end(),
      std::back_inserter(policy_app_ids),
      [](std::string id) {
        return helpers::Compare<std::string, helpers::NEQ, helpers::ALL>(
            id, kDefaultId, kPreDataConsentId, kDeviceId);
      });

  return policy_app_ids;
}

void PolicyHandler::GetEnabledCloudApps(
    std::vector<std::string>& enabled_apps) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->GetEnabledCloudApps(enabled_apps);
}

bool PolicyHandler::GetAppProperties(const std::string& policy_app_id,
                                     AppProperties& out_app_properties) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->GetAppProperties(policy_app_id, out_app_properties);
}

std::vector<std::string> PolicyHandler::GetEnabledLocalApps() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::vector<std::string>());
  return policy_manager->GetEnabledLocalApps();
}

const bool PolicyHandler::CheckCloudAppEnabled(
    const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  AppProperties out_app_properties;
  policy_manager->GetAppProperties(policy_app_id, out_app_properties);
  return out_app_properties.enabled;
}

PolicyHandler::AppPropertiesState PolicyHandler::GetAppPropertiesStatus(
    const smart_objects::SmartObject& properties,
    const std::string& app_id) const {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, AppPropertiesState::NO_CHANGES);

  AppProperties app_properties;
  policy_manager->GetAppProperties(app_id, app_properties);

  policy::StringArray nicknames;
  policy::StringArray app_hmi_types;
  policy_manager->GetInitialAppData(app_id, &nicknames, &app_hmi_types);

  if (properties.keyExists(strings::enabled) &&
      app_properties.enabled != properties[strings::enabled].asBool()) {
    SDL_LOG_DEBUG("\"enabled\" was changed from: "
                  << app_properties.enabled
                  << " to: " << properties[strings::enabled].asBool());
    return AppPropertiesState::ENABLED_FLAG_SWITCH;
  }
  if (properties.keyExists(strings::auth_token) &&
      app_properties.auth_token != properties[strings::auth_token].asString()) {
    SDL_LOG_DEBUG("\"auth_token\" was changed from: "
                  << app_properties.auth_token
                  << " to: " << properties[strings::auth_token].asString());
    return AppPropertiesState::AUTH_TOKEN_CHANGED;
  }
  if (properties.keyExists(strings::transport_type) &&
      app_properties.transport_type !=
          properties[strings::transport_type].asString()) {
    SDL_LOG_DEBUG("\"transport_type\" was changed from: "
                  << app_properties.transport_type
                  << " to: " << properties[strings::transport_type].asString());
    return AppPropertiesState::TRANSPORT_TYPE_CHANGED;
  }

  if (properties.keyExists(strings::cloud_transport_type) &&
      app_properties.transport_type !=
          properties[strings::cloud_transport_type].asString()) {
    SDL_LOG_DEBUG("\"transport_type\" was changed from: "
                  << app_properties.transport_type << " to: "
                  << properties[strings::cloud_transport_type].asString());
    return AppPropertiesState::TRANSPORT_TYPE_CHANGED;
  }

  if (properties.keyExists(strings::endpoint) &&
      app_properties.endpoint != properties[strings::endpoint].asString()) {
    SDL_LOG_DEBUG("\"endpoint\" was changed from: "
                  << app_properties.endpoint
                  << " to: " << properties[strings::endpoint].asString());
    return AppPropertiesState::ENDPOINT_CHANGED;
  }
  if (properties.keyExists(strings::nicknames)) {
    const smart_objects::SmartArray* nicknames_array =
        properties[strings::nicknames].asArray();

    if (nicknames_array->size() != nicknames.size()) {
      return AppPropertiesState::NICKNAMES_CHANGED;
    }

    smart_objects::SmartArray::const_iterator it_begin =
        nicknames_array->begin();
    smart_objects::SmartArray::const_iterator it_end = nicknames_array->end();
    for (; it_begin != it_end; ++it_begin) {
      const auto result =
          std::find(nicknames.begin(), nicknames.end(), (*it_begin).asString());
      if (nicknames.end() == result) {
        SDL_LOG_DEBUG("\"nicknames\" were changed, new value: "
                      << (*it_begin).asString());
        return AppPropertiesState::NICKNAMES_CHANGED;
      }
    }
  }
  if (properties.keyExists(strings::hybrid_app_preference)) {
    auto value = static_cast<mobile_apis::HybridAppPreference::eType>(
        properties[strings::hybrid_app_preference].asUInt());
    std::string hybrid_app_preference_str;
    smart_objects::EnumConversionHelper<
        mobile_apis::HybridAppPreference::eType>::
        EnumToString(value, &hybrid_app_preference_str);
    if (app_properties.hybrid_app_preference != hybrid_app_preference_str) {
      SDL_LOG_DEBUG("\"hybrid_app_preference\" was changed from: "
                    << app_properties.hybrid_app_preference << " to: "
                    << properties[strings::hybrid_app_preference].asString());
      return AppPropertiesState::HYBRYD_APP_PROPERTIES_CHANGED;
    }
  }
  return AppPropertiesState::NO_CHANGES;
}

bool PolicyHandler::IsNewApplication(const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->IsNewApplication(policy_app_id);
}

void PolicyHandler::OnSetAppProperties(
    const smart_objects::SmartObject& properties) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);

  const auto policy_app_id(properties[strings::policy_app_id].asString());
  policy_manager->InitCloudApp(policy_app_id);

  bool auth_token_update = false;
  if (properties.keyExists(strings::enabled)) {
    const bool enabled = properties[strings::enabled].asBool();
    policy_manager->SetCloudAppEnabled(policy_app_id, enabled);
  }
  if (properties.keyExists(strings::auth_token)) {
    const std::string auth_token = properties[strings::auth_token].asString();
    policy_manager->SetAppAuthToken(policy_app_id, auth_token);
    auth_token_update = true;
  }
  if (properties.keyExists(strings::transport_type)) {
    policy_manager->SetAppCloudTransportType(
        policy_app_id, properties[strings::transport_type].asString());
  }
  if (properties.keyExists(strings::endpoint)) {
    policy_manager->SetAppEndpoint(policy_app_id,
                                   properties[strings::endpoint].asString());
  }
  if (properties.keyExists(strings::nicknames)) {
    StringArray nicknames;
    const smart_objects::SmartObject& nicknames_array =
        properties[strings::nicknames];
    for (size_t i = 0; i < nicknames_array.length(); ++i) {
      nicknames.push_back(nicknames_array[i].asString());
    }
    policy_manager->SetAppNicknames(policy_app_id, nicknames);
  }
  if (properties.keyExists(strings::hybrid_app_preference)) {
    std::string hybrid_app_preference;

    auto value = static_cast<mobile_apis::HybridAppPreference::eType>(
        properties[strings::hybrid_app_preference].asUInt());
    smart_objects::EnumConversionHelper<
        mobile_apis::HybridAppPreference::eType>::
        EnumToString(value, &hybrid_app_preference);
    policy_manager->SetHybridAppPreference(policy_app_id,
                                           hybrid_app_preference);
  }

  if (auth_token_update) {
    AppProperties app_properties;
    if (policy_manager->GetAppProperties(policy_app_id, app_properties)) {
      OnAuthTokenUpdated(policy_app_id, app_properties.auth_token);
    }
  }
}

void PolicyHandler::OnLocalAppAdded() {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->OnLocalAppAdded();
}

void PolicyHandler::OnSetCloudAppProperties(
    const smart_objects::SmartObject& message) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  if (!message.keyExists(strings::msg_params)) {
    SDL_LOG_ERROR("Message does not contain mandatory section "
                  << strings::msg_params);
    return;
  }
  if (!message[strings::msg_params].keyExists(strings::properties)) {
    SDL_LOG_ERROR("Message does not contain app properties "
                  << strings::msg_params);
    return;
  }

  const smart_objects::SmartObject& properties =
      message[strings::msg_params][strings::properties];
  if (!properties.keyExists(strings::app_id)) {
    SDL_LOG_ERROR("Message does not contain mandatory parameter "
                  << strings::app_id);
    return;
  }
  std::string policy_app_id(properties[strings::app_id].asString());

  policy_manager->InitCloudApp(policy_app_id);

  bool auth_token_update = false;
  if (properties.keyExists(strings::auth_token)) {
    std::string auth_token = properties[strings::auth_token].asString();
    policy_manager->SetAppAuthToken(policy_app_id, auth_token);
    auth_token_update = true;
  }
  if (properties.keyExists(strings::cloud_transport_type)) {
    policy_manager->SetAppCloudTransportType(
        policy_app_id, properties[strings::cloud_transport_type].asString());
  }
  if (properties.keyExists(strings::endpoint)) {
    policy_manager->SetAppEndpoint(policy_app_id,
                                   properties[strings::endpoint].asString());
  }
  if (properties.keyExists(strings::nicknames)) {
    StringArray nicknames;
    const smart_objects::SmartObject& nicknames_array =
        properties[strings::nicknames];
    for (size_t i = 0; i < nicknames_array.length(); ++i) {
      nicknames.push_back(nicknames_array[i].asString());
    }
    policy_manager->SetAppNicknames(policy_app_id, nicknames);
  }
  if (properties.keyExists(strings::hybrid_app_preference)) {
    std::string hybrid_app_preference;

    auto value = static_cast<mobile_apis::HybridAppPreference::eType>(
        properties[strings::hybrid_app_preference].asUInt());
    smart_objects::EnumConversionHelper<
        mobile_apis::HybridAppPreference::eType>::
        EnumToString(value, &hybrid_app_preference);
    policy_manager->SetHybridAppPreference(policy_app_id,
                                           hybrid_app_preference);
  }
  if (properties.keyExists(strings::enabled)) {
    bool enabled = properties[strings::enabled].asBool();
    policy_manager->SetCloudAppEnabled(policy_app_id, enabled);
    if (!auth_token_update) {
      auth_token_update = enabled;
    }
    application_manager_.RefreshCloudAppInformation();
  }

  if (auth_token_update) {
    AppProperties app_properties;

    policy_manager->GetAppProperties(policy_app_id, app_properties);
    OnAuthTokenUpdated(policy_app_id, app_properties.auth_token);
  }
}

void PolicyHandler::GetAppServiceParameters(
    const std::string& policy_app_id,
    policy_table::AppServiceParameters* app_service_parameters) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->GetAppServiceParameters(policy_app_id,
                                          app_service_parameters);
}

bool PolicyHandler::CheckAppServiceParameters(
    const std::string& policy_app_id,
    const std::string& requested_service_name,
    const std::string& requested_service_type,
    smart_objects::SmartArray* requested_handled_rpcs) const {
  std::vector<int32_t> handled_rpcs = {};

  policy_table::AppServiceParameters app_service_parameters =
      policy_table::AppServiceParameters();
  this->GetAppServiceParameters(policy_app_id, &app_service_parameters);

  if (app_service_parameters.find(requested_service_type) ==
      app_service_parameters.end()) {
    SDL_LOG_DEBUG("Disallowed service type: " << requested_service_type);
    return false;
  }

  auto service_names =
      *(app_service_parameters[requested_service_type].service_names);
  if (!service_names.is_initialized()) {
    SDL_LOG_DEBUG("Pt Service Name is Null, All service names accepted");
  } else if (!requested_service_name.empty()) {
    auto find_name_result =
        std::find(service_names.begin(),
                  service_names.end(),
                  rpc::String<0, 255>(requested_service_name));
    if (find_name_result == service_names.end()) {
      SDL_LOG_DEBUG("Disallowed service name: " << requested_service_name);
      return false;
    }
  }

  if (requested_handled_rpcs) {
    auto temp_rpcs =
        app_service_parameters[requested_service_type].handled_rpcs;
    for (auto handled_it = temp_rpcs.begin(); handled_it != temp_rpcs.end();
         ++handled_it) {
      handled_rpcs.push_back(handled_it->function_id);
    }

    for (auto requested_it = requested_handled_rpcs->begin();
         requested_it != requested_handled_rpcs->end();
         ++requested_it) {
      auto find_result = std::find(
          handled_rpcs.begin(), handled_rpcs.end(), requested_it->asInt());
      if (find_result == handled_rpcs.end()) {
        SDL_LOG_DEBUG("Disallowed by handled rpc: " << requested_it->asInt());
        return false;
      }
    }
  }
  return true;
}

bool PolicyHandler::UnknownRPCPassthroughAllowed(
    const std::string& policy_app_id) const {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->UnknownRPCPassthroughAllowed(policy_app_id);
}

uint32_t PolicyHandler::HeartBeatTimeout(const std::string& app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, 0);
  return policy_manager->HeartBeatTimeout(app_id);
}

const std::string PolicyHandler::RemoteAppsUrl() const {
  const std::string default_url;
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, default_url);
  EndpointUrls endpoints;
  policy_manager->GetUpdateUrls("queryAppsUrl", endpoints);
  if (endpoints.empty() || endpoints[0].url.empty()) {
    return default_url;
  }

  return endpoints[0].url[0];
}

void PolicyHandler::OnAppsSearchStarted() {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->OnAppsSearchStarted();
}

void PolicyHandler::OnAppsSearchCompleted(const bool trigger_ptu) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->OnAppsSearchCompleted(trigger_ptu);
}

void PolicyHandler::OnAddedNewApplicationToAppList(
    const uint32_t new_app_id, const std::string& policy_id) {
  SDL_LOG_AUTO_TRACE();
  if (policy_id == last_registered_policy_app_id_) {
    return;
  }
  last_registered_policy_app_id_ = policy_id;
  PushAppIdToPTUQueue(new_app_id);
}

void PolicyHandler::OnAppRegisteredOnMobile(const std::string& device_id,
                                            const std::string& application_id) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->OnAppRegisteredOnMobile(device_id, application_id);
}

RequestType::State PolicyHandler::GetAppRequestTypeState(
    const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, RequestType::State::UNAVAILABLE);
  return policy_manager->GetAppRequestTypesState(policy_app_id);
}

RequestSubType::State PolicyHandler::GetAppRequestSubTypeState(
    const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager,
                             RequestSubType::State::UNAVAILABLE);
  return policy_manager->GetAppRequestSubTypesState(policy_app_id);
}

bool PolicyHandler::IsRequestTypeAllowed(
    const transport_manager::DeviceHandle& device_id,
    const std::string& policy_app_id,
    mobile_apis::RequestType::eType type) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  using namespace mobile_apis;

  const std::string stringified_type = RequestTypeToString(type);
  if (stringified_type.empty()) {
    SDL_LOG_ERROR("Unknown request type.");
    return false;
  }

  const RequestType::State request_type_state =
      policy_manager->GetAppRequestTypesState(policy_app_id);

  switch (request_type_state) {
    case RequestType::State::EMPTY: {
      // If empty array of request types is assigned to app - any is allowed
      SDL_LOG_TRACE("Any Request Type is allowed by policies.");
      return true;
    }
    case RequestType::State::OMITTED: {
      // If RequestType parameter omitted for app - any is disallowed
      SDL_LOG_TRACE("All Request Types are disallowed by policies.");
      return false;
    }
    case RequestType::State::AVAILABLE: {
      // If any of request types is available for current application - get them
      const auto request_types =
#ifdef EXTERNAL_PROPRIETARY_MODE
          policy_manager->GetAppRequestTypes(device_id, policy_app_id);
#else
          policy_manager->GetAppRequestTypes(policy_app_id);
#endif
      return helpers::in_range(request_types, stringified_type);
    }
    default:
      return false;
  }
}

bool PolicyHandler::IsRequestSubTypeAllowed(
    const std::string& policy_app_id,
    const std::string& request_subtype) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  using namespace mobile_apis;

  if (request_subtype.empty()) {
    SDL_LOG_ERROR("Request subtype to check is empty.");
    return false;
  }

  const RequestSubType::State request_subtype_state =
      policy_manager->GetAppRequestSubTypesState(policy_app_id);
  switch (request_subtype_state) {
    case RequestSubType::State::EMPTY: {
      // If empty array of request subtypes is assigned to app - any is allowed
      SDL_LOG_TRACE("Any Request SubType is allowed by policies.");
      return true;
    }
    case RequestSubType::State::OMITTED: {
      // If RequestSubType parameter omitted for app - any is disallowed
      SDL_LOG_TRACE("All Request SubTypes are disallowed by policies.");
      return false;
    }
    case RequestSubType::State::AVAILABLE: {
      // If any of request subtypes is available for current application
      // get them all
      const auto request_subtypes =
          policy_manager->GetAppRequestSubTypes(policy_app_id);
      return helpers::in_range(request_subtypes, request_subtype);
    }
    default:
      return false;
  }
}

const std::vector<std::string> PolicyHandler::GetAppRequestTypes(
    const transport_manager::DeviceHandle& device_handle,
    const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::vector<std::string>());
#ifdef EXTERNAL_PROPRIETARY_MODE
  return policy_manager->GetAppRequestTypes(device_handle, policy_app_id);
#else
  return policy_manager->GetAppRequestTypes(policy_app_id);
#endif
}

const std::vector<std::string> PolicyHandler::GetAppRequestSubTypes(
    const std::string& policy_app_id) const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, std::vector<std::string>());
  return policy_manager->GetAppRequestSubTypes(policy_app_id);
}

const std::vector<policy_table::VehicleDataItem>
policy::PolicyHandler::GetVehicleDataItems() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager,
                             std::vector<policy_table::VehicleDataItem>());
  return policy_manager->GetVehicleDataItems();
}

std::vector<rpc::policy_table_interface_base::VehicleDataItem>
policy::PolicyHandler::GetRemovedVehicleDataItems() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager,
                             std::vector<policy_table::VehicleDataItem>());
  return policy_manager->GetRemovedVehicleDataItems();
}

#ifdef EXTERNAL_PROPRIETARY_MODE
const MetaInfo PolicyHandler::GetMetaInfo() const {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, MetaInfo());
  return policy_manager->GetMetaInfo();
}
#endif  // EXTERNAL_PROPRIETARY_MODE

void PolicyHandler::Increment(usage_statistics::GlobalCounterId type) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->Increment(type);
}

void PolicyHandler::Increment(const std::string& app_id,
                              usage_statistics::AppCounterId type) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->Increment(app_id, type);
}

void PolicyHandler::Set(const std::string& app_id,
                        usage_statistics::AppInfoId type,
                        const std::string& value) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->Set(app_id, type, value);
}

void PolicyHandler::Add(const std::string& app_id,
                        usage_statistics::AppStopwatchId type,
                        int32_t timespan_seconds) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->Add(app_id, type, timespan_seconds);
}

bool PolicyHandler::IsUrlAppIdValid(const std::string app_id,
                                    const EndpointData& app_data) const {
  if (policy::kDefaultId == app_data.app_id) {
    return true;
  }

  const std::vector<std::string> app_urls = app_data.url;
  if (app_urls.empty() || app_id != app_data.app_id) {
    return false;
  }

  const auto devices_ids = GetDevicesIds(app_id);
  SDL_LOG_TRACE("Count devices: " << devices_ids.size()
                                  << " for app_id: " << app_id);
  for (const auto& device_id : devices_ids) {
    const ApplicationSharedPtr app =
        application_manager_.application(device_id, app_id);
    if (app && (app->IsRegistered())) {
      return true;
    }
  }

  return false;
}

std::vector<std::string> PolicyHandler::GetDevicesIds(
    const std::string& policy_app_id) const {
  return application_manager_.devices(policy_app_id);
}

void PolicyHandler::UpdateHMILevel(ApplicationSharedPtr app,
                                   mobile_apis::HMILevel::eType level) {
  SDL_LOG_AUTO_TRACE();
  POLICY_LIB_CHECK_VOID(app);
  if (app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW) ==
      mobile_apis::HMILevel::HMI_NONE) {
    // If default is FULL, send request to HMI. Notification to mobile will be
    // sent on response receiving.
    if (mobile_apis::HMILevel::HMI_FULL == level) {
      MessageHelper::SendActivateAppToHMI(app->app_id(), application_manager_);
    } else {
      SDL_LOG_INFO("Changing hmi level of application "
                   << app->app_id() << " to default hmi level " << level);
      // Set application hmi level
      application_manager_.ChangeAppsHMILevel(app->app_id(), level);
      // If hmi Level is full, it will be seted after ActivateApp response
      auto notification = MessageHelper::CreateHMIStatusNotification(
          app, mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
      application_manager_.GetRPCService().ManageMobileCommand(
          notification, commands::Command::SOURCE_SDL);
    }
  }
}

bool PolicyHandler::CheckModule(const PTString& app_id,
                                const PTString& module) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->CheckModule(app_id, module);
}

void PolicyHandler::OnRemoteAppPermissionsChanged(
    const std::string& device_id, const std::string& application_id) {
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  policy_manager->SendAppPermissionsChanged(device_id, application_id);
}

void PolicyHandler::OnUpdateHMIStatus(const std::string& device_id,
                                      const std::string& policy_app_id,
                                      const std::string& hmi_level) {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (!app) {
    SDL_LOG_WARN("Could not find application: " << device_id << " - "
                                                << policy_app_id);
    return;
  }
  auto level = StringToEnum<mobile_apis::HMILevel::eType>(hmi_level);
  if (mobile_apis::HMILevel::INVALID_ENUM == level) {
    SDL_LOG_WARN("Couldn't convert default hmi level " << hmi_level
                                                       << " to enum.");
    return;
  }

  SDL_LOG_INFO("Changing hmi level of application "
               << app->app_id() << " to default hmi level " << level);
  // Set application hmi level
  application_manager_.ChangeAppsHMILevel(app->app_id(), level);
  auto notification = MessageHelper::CreateHMIStatusNotification(
      app, mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  application_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::SOURCE_SDL);
}

bool PolicyHandler::GetModuleTypes(const std::string& policy_app_id,
                                   std::vector<std::string>* modules) const {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  return policy_manager->GetModuleTypes(policy_app_id, modules);
}

void PolicyHandler::SetDefaultHmiTypes(
    const transport_manager::DeviceHandle& device_handle,
    const std::string& application_id,
    const smart_objects::SmartObject* app_types) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_VOID(policy_manager);
  std::vector<int> hmi_types;
  if (app_types && app_types->asArray()) {
    smart_objects::SmartArray* hmi_list = app_types->asArray();
    std::transform(hmi_list->begin(),
                   hmi_list->end(),
                   std::back_inserter(hmi_types),
                   SmartObjectToInt());
  }
  policy_manager->SetDefaultHmiTypes(device_handle, application_id, hmi_types);
}

bool PolicyHandler::CheckHMIType(const std::string& application_id,
                                 mobile_apis::AppHMIType::eType hmi,
                                 const smart_objects::SmartObject* app_types) {
  SDL_LOG_AUTO_TRACE();
  const auto policy_manager = LoadPolicyManager();
  POLICY_LIB_CHECK_OR_RETURN(policy_manager, false);
  std::vector<int> policy_hmi_types;
  bool ret = policy_manager->GetHMITypes(application_id, &policy_hmi_types);

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
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app =
      application_manager_.application(device_id, policy_app_id);
  if (!app) {
    SDL_LOG_WARN("Could not find application: " << device_id << " - "
                                                << policy_app_id);
    return;
  }
  auto level = StringToEnum<mobile_apis::HMILevel::eType>(hmi_level);
  if (mobile_apis::HMILevel::INVALID_ENUM == level) {
    SDL_LOG_WARN("Couldn't convert default hmi level " << hmi_level
                                                       << " to enum.");
    return;
  }
  UpdateHMILevel(app, level);
}
}  //  namespace policy
