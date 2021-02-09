/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>  // for rand()

#include <bson_object.h>
#include <climits>
#include <fstream>
#include <string>
#include <utility>

#include "application_manager/app_launch/app_launch_ctrl_impl.h"
#include "application_manager/app_launch/app_launch_data_db.h"
#include "application_manager/app_launch/app_launch_data_json.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/command_holder_impl.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_notification_impl.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/hmi_capabilities_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/plugin_manager/rpc_plugin_manager_impl.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/rpc_handler_impl.h"
#include "application_manager/rpc_protection_manager_impl.h"
#include "application_manager/rpc_service_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/formatter_json_rpc.h"
#include "hmi_message_handler/hmi_message_handler.h"
#include "protocol/bson_object_keys.h"
#include "protocol_handler/protocol_handler.h"

#include <time.h>
#include <boost/filesystem.hpp>
#include "application_manager/application_impl.h"
#include "encryption/hashing.h"
#include "interfaces/HMI_API_schema.h"
#include "media_manager/media_manager.h"
#include "policy/usage_statistics/counter.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/custom_string.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "utils/threads/thread.h"
#include "utils/timer_task_impl.h"

namespace {
int get_rand_from_range(uint32_t from = 0, int to = RAND_MAX) {
  return std::rand() % to + from;
}
}  // namespace

namespace application_manager {

namespace {
DeviceTypes devicesType = {
    std::make_pair(std::string("USB_AOA"),
                   hmi_apis::Common_TransportType::USB_AOA),
    std::make_pair(std::string("USB_IOS"),
                   hmi_apis::Common_TransportType::USB_IOS),
    std::make_pair(std::string("BLUETOOTH"),
                   hmi_apis::Common_TransportType::BLUETOOTH),
    std::make_pair(std::string("BLUETOOTH_IOS"),
                   hmi_apis::Common_TransportType::BLUETOOTH),
    std::make_pair(std::string("WIFI"), hmi_apis::Common_TransportType::WIFI),
    std::make_pair(std::string("USB_IOS_HOST_MODE"),
                   hmi_apis::Common_TransportType::USB_IOS),
    std::make_pair(std::string("USB_IOS_DEVICE_MODE"),
                   hmi_apis::Common_TransportType::USB_IOS),
    std::make_pair(std::string("CARPLAY_WIRELESS_IOS"),
                   hmi_apis::Common_TransportType::WIFI),
    std::make_pair(std::string("CLOUD_WEBSOCKET"),
                   hmi_apis::Common_TransportType::CLOUD_WEBSOCKET),
    std::make_pair(std::string("WEBENGINE_WEBSOCKET"),
                   hmi_apis::Common_TransportType::WEBENGINE_WEBSOCKET)};
}

/**
 * @brief device_id_comparator is predicate to compare application device id
 * @param device_id Device id to compare with
 * @param app Application pointer
 * @return True if device id of application matches to device id passed
 */
bool device_id_comparator(const std::string& device_id,
                          ApplicationSharedPtr app) {
  DCHECK_OR_RETURN(app, false);
  SDL_LOG_DEBUG("Data to compare: device_id : " << device_id << " app mac: "
                                                << app->mac_address());

  return device_id == app->mac_address();
}

/**
 * @brief PolicyAppIdComparator is struct predicate to compare policy
 * application ids & device
 * @param device_handle of application
 * @param id of application
 * @return True if policy id & device_handle of application matches to policy id
 * & device_handle passed
 */
struct PolicyAppIdComparator {
  PolicyAppIdComparator(const connection_handler::DeviceHandle& device_handle,
                        const std::string& policy_app_id)
      : device_handle_(device_handle), policy_app_id_(policy_app_id) {}
  bool operator()(const ApplicationSharedPtr app) const {
    return app && app->device() == device_handle_ &&
           app->policy_app_id() == policy_app_id_;
  }

 private:
  const connection_handler::DeviceHandle& device_handle_;
  const std::string& policy_app_id_;
};

namespace formatters = ns_smart_device_link::ns_json_handler::formatters;
namespace jhs = ns_smart_device_link::ns_json_handler::strings;

using namespace ns_smart_device_link::ns_smart_objects;

ApplicationManagerImpl::ApplicationManagerImpl(
    const ApplicationManagerSettings& am_settings,
    const policy::PolicySettings& policy_settings)
    : settings_(am_settings)
    , applications_list_lock_ptr_(
          std::make_shared<sync_primitives::RecursiveLock>())
    , apps_to_register_list_lock_ptr_(std::make_shared<sync_primitives::Lock>())
    , reregister_wait_list_lock_ptr_(std::make_shared<sync_primitives::Lock>())
    , subscribed_to_hmi_way_points_(false)
    , audio_pass_thru_active_(false)
    , audio_pass_thru_app_id_(0)
    , driver_distraction_state_(hmi_apis::Common_DriverDistractionState::DD_OFF)
    , is_vr_session_strated_(false)
    , hmi_cooperating_(false)
    , is_all_apps_allowed_(true)
    , media_manager_(NULL)
    , hmi_handler_(NULL)
    , connection_handler_(NULL)
    , policy_handler_(new policy::PolicyHandler(policy_settings, *this))
    , protocol_handler_(NULL)
    , request_ctrl_(am_settings)
    , mobile_correlation_id_(0)
    , correlation_id_(0)
    , max_correlation_id_(UINT_MAX)
    , hmi_capabilities_(new HMICapabilitiesImpl(*this))
    , unregister_reason_(
          mobile_api::AppInterfaceUnregisteredReason::INVALID_ENUM)
    , resume_ctrl_(new resumption::ResumeCtrlImpl(*this))
    , navi_close_app_timeout_(am_settings.stop_streaming_timeout())
    , navi_end_stream_timeout_(am_settings.stop_streaming_timeout())
    , state_ctrl_(*this)
    , pending_device_map_lock_ptr_(
          std::make_shared<sync_primitives::RecursiveLock>())
    , application_list_update_timer_(
          "AM ListUpdater",
          new TimerTaskImpl<ApplicationManagerImpl>(
              this, &ApplicationManagerImpl::OnApplicationListUpdateTimer))
    , tts_global_properties_timer_(
          "AM TTSGLPRTimer",
          new TimerTaskImpl<ApplicationManagerImpl>(
              this, &ApplicationManagerImpl::OnTimerSendTTSGlobalProperties))
    , clear_pool_timer_("ClearPoolTimer",
                        new TimerTaskImpl<ApplicationManagerImpl>(
                            this, &ApplicationManagerImpl::ClearTimerPool))
    , is_low_voltage_(false)
    , apps_size_(0)
    , registered_during_timer_execution_(false)
    , is_stopping_(false) {
  std::srand(std::time(nullptr));
  AddPolicyObserver(this);

  dir_type_to_string_map_ = {{TYPE_STORAGE, "Storage"},
                             {TYPE_SYSTEM, "System"},
                             {TYPE_ICONS, "Icons"}};

  const uint32_t timeout_ms = 10000u;
  clear_pool_timer_.Start(timeout_ms, timer::kPeriodic);

  rpc_handler_.reset(new rpc_handler::RPCHandlerImpl(
      *this, hmi_so_factory(), mobile_so_factory()));
  commands_holder_.reset(new CommandHolderImpl(*this));
  std::shared_ptr<RPCProtectionManager> rpc_protection_manager =
      std::make_shared<RPCProtectionManagerImpl>(*policy_handler_);
  policy_handler_->add_listener(rpc_protection_manager.get());
  rpc_service_.reset(new rpc_service::RPCServiceImpl(*this,
                                                     request_ctrl_,
                                                     protocol_handler_,
                                                     hmi_handler_,
                                                     *commands_holder_,
                                                     rpc_protection_manager,
                                                     hmi_so_factory(),
                                                     mobile_so_factory()));
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  SDL_LOG_AUTO_TRACE();

  is_stopping_.store(true);
  SendOnSDLClose();
  media_manager_ = NULL;
  hmi_handler_ = NULL;
  connection_handler_ = NULL;
  protocol_handler_ = NULL;
  SDL_LOG_DEBUG("Destroying Policy Handler");
  RemovePolicyObserver(this);

  {
    sync_primitives::AutoLock lock(close_app_timer_pool_lock_);
    close_app_timer_pool_.clear();
  }

  {
    sync_primitives::AutoLock lock(end_stream_timer_pool_lock_);
    end_stream_timer_pool_.clear();
  }

  {
    sync_primitives::AutoLock lock(navi_app_to_stop_lock_);
    navi_app_to_stop_.clear();
  }

  navi_app_to_end_stream_.clear();

  secondary_transport_devices_cache_.clear();
}

DataAccessor<ApplicationSet> ApplicationManagerImpl::applications() const {
  DataAccessor<ApplicationSet> accessor(applications_,
                                        applications_list_lock_ptr_);
  return accessor;
}

DataAccessor<AppsWaitRegistrationSet>
ApplicationManagerImpl::pending_applications() const {
  DataAccessor<AppsWaitRegistrationSet> accessor(
      apps_to_register_, apps_to_register_list_lock_ptr_);
  return accessor;
}

DataAccessor<ReregisterWaitList>
ApplicationManagerImpl::reregister_applications() const {
  DataAccessor<ReregisterWaitList> accessor(reregister_wait_list_,
                                            reregister_wait_list_lock_ptr_);
  return accessor;
}

ApplicationSharedPtr ApplicationManagerImpl::application(
    uint32_t app_id) const {
  AppIdPredicate finder(app_id);
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, finder);
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_hmi_app(
    uint32_t hmi_app_id) const {
  HmiAppIdPredicate finder(hmi_app_id);
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, finder);
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_policy_id(
    const std::string& policy_app_id) const {
  PolicyAppIdPredicate finder(policy_app_id);
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, finder);
}

ApplicationSharedPtr ApplicationManagerImpl::pending_application_by_policy_id(
    const std::string& policy_app_id) const {
  PolicyAppIdPredicate finder(policy_app_id);
  DataAccessor<AppsWaitRegistrationSet> accessor = pending_applications();
  return FindPendingApp(accessor, finder);
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_name(
    const std::string& app_name) const {
  AppNamePredicate finder(app_name);
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, finder);
}

ApplicationSharedPtr
ApplicationManagerImpl::reregister_application_by_policy_id(
    const std::string& policy_app_id) const {
  PolicyAppIdPredicate finder(policy_app_id);
  DataAccessor<ReregisterWaitList> accessor = reregister_applications();
  return FindReregisterApp(accessor, finder);
}

bool ActiveAppPredicate(const ApplicationSharedPtr app) {
  return app ? app->IsFullscreen() : false;
}

ApplicationSharedPtr ApplicationManagerImpl::active_application() const {
  // TODO(DK) : check driver distraction
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, ActiveAppPredicate);
}

bool FullOrLimitedAppPredicate(const ApplicationSharedPtr app) {
  return app ? app->IsFullscreen() ||
                   app->hmi_level(
                       mobile_api::PredefinedWindows::DEFAULT_WINDOW) ==
                       mobile_api::HMILevel::HMI_LIMITED
             : false;
}

ApplicationSharedPtr ApplicationManagerImpl::get_full_or_limited_application()
    const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, FullOrLimitedAppPredicate);
}

bool LimitedMediaAppPredicate(const ApplicationSharedPtr app) {
  return app ? (app->is_media_application() &&
                app->hmi_level(mobile_api::PredefinedWindows::DEFAULT_WINDOW) ==
                    mobile_api::HMILevel::HMI_LIMITED)
             : false;
}

ApplicationSharedPtr ApplicationManagerImpl::get_limited_media_application()
    const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, LimitedMediaAppPredicate);
}

bool LimitedNaviAppPredicate(const ApplicationSharedPtr app) {
  return app ? (app->is_navi() &&
                app->hmi_level(mobile_api::PredefinedWindows::DEFAULT_WINDOW) ==
                    mobile_api::HMILevel::HMI_LIMITED)
             : false;
}

ApplicationSharedPtr ApplicationManagerImpl::get_limited_navi_application()
    const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, LimitedNaviAppPredicate);
}

bool LimitedVoiceAppPredicate(const ApplicationSharedPtr app) {
  return app ? (app->is_voice_communication_supported() &&
                app->hmi_level(mobile_api::PredefinedWindows::DEFAULT_WINDOW) ==
                    mobile_api::HMILevel::HMI_LIMITED)
             : false;
}

ApplicationSharedPtr ApplicationManagerImpl::get_limited_voice_application()
    const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, LimitedVoiceAppPredicate);
}

bool NaviAppPredicate(const ApplicationSharedPtr app) {
  return app ? app->is_navi() : false;
}

std::vector<ApplicationSharedPtr>
ApplicationManagerImpl::applications_with_navi() {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindAllApps(accessor, NaviAppPredicate);
}

bool LimitedMobileProjectionPredicate(const ApplicationSharedPtr app) {
  return app ? (app->mobile_projection_enabled() &&
                app->hmi_level(mobile_api::PredefinedWindows::DEFAULT_WINDOW) ==
                    mobile_api::HMILevel::HMI_LIMITED)
             : false;
}

ApplicationSharedPtr
ApplicationManagerImpl::get_limited_mobile_projection_application() const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, LimitedMobileProjectionPredicate);
}

bool MobileProjectionPredicate(const ApplicationSharedPtr app) {
  return app ? app->mobile_projection_enabled() : false;
}

std::vector<ApplicationSharedPtr>
ApplicationManagerImpl::applications_with_mobile_projection() {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindAllApps(accessor, MobileProjectionPredicate);
}

std::vector<ApplicationSharedPtr>
ApplicationManagerImpl::applications_by_button(uint32_t button) {
  SubscribedToButtonPredicate finder(
      static_cast<mobile_apis::ButtonName::eType>(button));
  DataAccessor<ApplicationSet> accessor = applications();
  return FindAllApps(accessor, finder);
}

std::vector<ApplicationSharedPtr> ApplicationManagerImpl::applications_by_name(
    const std::string& app_name) const {
  AppNamePredicate finder(app_name);
  DataAccessor<ApplicationSet> accessor = applications();
  return FindAllApps(accessor, finder);
}

struct IsApplication {
  IsApplication(connection_handler::DeviceHandle device_handle,
                const std::string& policy_app_id)
      : device_handle_(device_handle), policy_app_id_(policy_app_id) {}
  bool operator()(const ApplicationSharedPtr app) const {
    return app && app->device() == device_handle_ &&
           app->policy_app_id() == policy_app_id_;
  }

 private:
  connection_handler::DeviceHandle device_handle_;
  const std::string& policy_app_id_;
};
void ApplicationManagerImpl::IviInfoUpdated(const std::string& vehicle_info,
                                            int value) {
  // Notify Policy Manager if available about info it's interested in,
  // i.e. odometer etc
  if (strings::odometer == vehicle_info) {
    GetPolicyHandler().KmsChanged(value);
  }
}

void ApplicationManagerImpl::OnApplicationRegistered(ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(app);
  sync_primitives::AutoLock lock(applications_list_lock_ptr_);
  const mobile_apis::HMILevel::eType default_level = GetDefaultHmiLevel(app);
  state_ctrl_.OnApplicationRegistered(app, default_level);
}

void ApplicationManagerImpl::OnApplicationSwitched(ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  commands_holder_->Resume(app, CommandHolder::CommandType::kMobileCommand);
  commands_holder_->Resume(app, CommandHolder::CommandType::kHmiCommand);
}

bool ApplicationManagerImpl::IsAppTypeExistsInFullOrLimited(
    ApplicationConstSharedPtr app) const {
  SDL_LOG_AUTO_TRACE();
  bool voice_state = app->is_voice_communication_supported();
  bool media_state = app->is_media_application();
  bool navi_state = app->is_navi();
  bool mobile_projection_state = app->mobile_projection_enabled();
  ApplicationSharedPtr active_app = active_application();
  // Check app in FULL level
  if (active_app.use_count() != 0) {
    // If checking app hmi level FULL, we return false
    // because we couldn't have two applications with same HMIType in FULL and
    // LIMITED HMI level
    if (active_app->app_id() == app->app_id()) {
      return false;
    }

    if (voice_state && active_app->is_voice_communication_supported()) {
      return true;
    }

    if (media_state && active_app->is_media_application()) {
      return true;
    }

    if (navi_state && active_app->is_navi()) {
      return true;
    }

    if (mobile_projection_state && active_app->mobile_projection_enabled()) {
      return true;
    }
  }

  // Check LIMITED apps
  if (voice_state) {
    if ((get_limited_voice_application().use_count() != 0) &&
        (get_limited_voice_application()->app_id() != app->app_id())) {
      return true;
    }
  }

  if (media_state) {
    if ((get_limited_media_application().use_count() != 0) &&
        (get_limited_media_application()->app_id() != app->app_id())) {
      return true;
    }
  }

  if (navi_state) {
    if ((get_limited_navi_application().use_count() != 0) &&
        (get_limited_navi_application()->app_id() != app->app_id())) {
      return true;
    }
  }

  if (mobile_projection_state) {
    if ((get_limited_mobile_projection_application().use_count() != 0) &&
        (get_limited_mobile_projection_application()->app_id() !=
         app->app_id())) {
      return true;
    }
  }

  return false;
}

ApplicationSharedPtr ApplicationManagerImpl::RegisterApplication(
    const std::shared_ptr<smart_objects::SmartObject>&
        request_for_registration) {
  SDL_LOG_AUTO_TRACE();

  smart_objects::SmartObject& message = *request_for_registration;
  uint32_t connection_key =
      message[strings::params][strings::connection_key].asInt();

  // app_id is SDL "internal" ID
  // original app_id can be received via ApplicationImpl::mobile_app_id()
  uint32_t app_id = 0;
  std::list<int32_t> sessions_list;
  connection_handler::DeviceHandle device_id = 0;

  DCHECK_OR_RETURN(connection_handler_, ApplicationSharedPtr());
  if (connection_handler().get_session_observer().GetDataOnSessionKey(
          connection_key, &app_id, &sessions_list, &device_id) == -1) {
    SDL_LOG_ERROR("Failed to create application: no connection info.");
    std::shared_ptr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::GENERIC_ERROR));
    rpc_service_->ManageMobileCommand(response, commands::Command::SOURCE_SDL);
    return ApplicationSharedPtr();
  }

  smart_objects::SmartObject& params = message[strings::msg_params];
  const std::string& policy_app_id =
      GetCorrectMobileIDFromMessage(request_for_registration);
  const custom_str::CustomString& app_name =
      message[strings::msg_params][strings::app_name].asCustomString();
  std::string device_mac;
  std::string connection_type;
  if (connection_handler().get_session_observer().GetDataOnDeviceID(
          device_id, NULL, NULL, &device_mac, &connection_type) == -1) {
    SDL_LOG_DEBUG("Failed to extract device mac for id " << device_id);
  } else {
    SDL_LOG_DEBUG("Device mac for id " << device_id << " is " << device_mac);
  }

  SDL_LOG_DEBUG("Restarting application list update timer");
  GetPolicyHandler().OnAppsSearchStarted();
  uint32_t timeout = get_settings().application_list_update_timeout();
  application_list_update_timer_.Start(timeout, timer::kSingleShot);

  if (!is_all_apps_allowed_) {
    SDL_LOG_WARN("RegisterApplication: access to app's disabled by user");
    std::shared_ptr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::DISALLOWED));
    rpc_service_->ManageMobileCommand(response, commands::Command::SOURCE_SDL);
    return ApplicationSharedPtr();
  }

  ApplicationSharedPtr application(
      new ApplicationImpl(app_id,
                          policy_app_id,
                          device_mac,
                          device_id,
                          app_name,
                          GetPolicyHandler().GetStatisticManager(),
                          *this));

  if (!application) {
    std::shared_ptr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::OUT_OF_MEMORY));
    rpc_service_->ManageMobileCommand(response, commands::Command::SOURCE_SDL);
    return ApplicationSharedPtr();
  }

  HmiStatePtr initial_state =
      CreateRegularState(std::shared_ptr<Application>(application),
                         mobile_apis::WindowType::MAIN,
                         mobile_apis::HMILevel::INVALID_ENUM,
                         mobile_apis::AudioStreamingState::INVALID_ENUM,
                         mobile_apis::VideoStreamingState::INVALID_ENUM,
                         mobile_api::SystemContext::SYSCTXT_MAIN);

  application->SetInitialState(
      mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
      std::string(),  // should not be tracked for main window
      initial_state);

  application->set_folder_name(policy_app_id + "_" +
                               application->mac_address());
  // To load persistent files, app folder name must be known first, which is now
  // depends on device_id and mobile_app_id
  application->LoadPersistentFiles();

  application->set_grammar_id(GenerateGrammarID());
  mobile_api::Language::eType launguage_desired =
      static_cast<mobile_api::Language::eType>(
          params[strings::language_desired].asInt());
  application->set_language(launguage_desired);
  application->usage_report().RecordAppRegistrationVuiLanguage(
      launguage_desired);

  mobile_api::Language::eType hmi_display_language_desired =
      static_cast<mobile_api::Language::eType>(
          params[strings::hmi_display_language_desired].asInt());
  application->set_ui_language(hmi_display_language_desired);
  application->usage_report().RecordAppRegistrationGuiLanguage(
      hmi_display_language_desired);

  Version version;
  int32_t min_version = message[strings::msg_params][strings::sync_msg_version]
                               [strings::minor_version]
                                   .asInt();
  version.min_supported_api_version = static_cast<APIVersion>(min_version);

  int32_t max_version = message[strings::msg_params][strings::sync_msg_version]
                               [strings::major_version]
                                   .asInt();
  version.max_supported_api_version = static_cast<APIVersion>(max_version);
  application->set_version(version);

  protocol_handler::MajorProtocolVersion protocol_version =
      static_cast<protocol_handler::MajorProtocolVersion>(
          message[strings::params][strings::protocol_version].asInt());
  application->set_protocol_version(protocol_version);
  connection_handler_->BindProtocolVersionWithSession(connection_key,
                                                      protocol_version);

  // Keep HMI add id in case app is present in "waiting for registration" list
  apps_to_register_list_lock_ptr_->Acquire();
  PolicyAppIdPredicate finder(application->policy_app_id());
  ApplicationSet::iterator it =
      std::find_if(apps_to_register_.begin(), apps_to_register_.end(), finder);
  bool is_mismatched_cloud_app = false;

  if (apps_to_register_.end() == it) {
    DevicePredicate device_finder(application->device());
    it = std::find_if(
        apps_to_register_.begin(), apps_to_register_.end(), device_finder);

    bool found = apps_to_register_.end() != it;
    is_mismatched_cloud_app = found && (*it)->is_cloud_app() &&
                              policy_app_id != (*it)->policy_app_id();
  } else {
    application->set_hmi_application_id((*it)->hmi_app_id());

    // Set cloud app parameters
    application->set_cloud_app_endpoint((*it)->cloud_app_endpoint());
    application->set_cloud_app_certificate((*it)->cloud_app_certificate());
    application->set_auth_token((*it)->auth_token());
    application->set_cloud_app_transport_type(
        (*it)->cloud_app_transport_type());
    application->set_hybrid_app_preference((*it)->hybrid_app_preference());
    apps_to_register_.erase(it);
  }
  apps_to_register_list_lock_ptr_->Release();

  // Reject registration request if a cloud app registers with the incorrect
  // appID
  if (is_mismatched_cloud_app) {
    std::shared_ptr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::DISALLOWED));
    (*response)[strings::msg_params][strings::info] =
        "Cloud app registered with incorrect app id";
    rpc_service_->ManageMobileCommand(response, commands::Command::SOURCE_SDL);
    return ApplicationSharedPtr();
  }

  if (!application->hmi_app_id()) {
    const bool is_saved =
        resume_controller().IsApplicationSaved(policy_app_id, device_mac);
    application->set_hmi_application_id(
        is_saved
            ? resume_controller().GetHMIApplicationID(policy_app_id, device_mac)
            : GenerateNewHMIAppID());
  }

  if (params.keyExists(strings::app_info)) {
    const smart_objects::SmartObject& app_info = params[strings::app_info];
    const std::string& bundle_id = app_info[strings::bundle_id].asString();
    application->set_bundle_id(bundle_id);
  }

  const std::string app_icon_dir(settings_.app_icons_folder());
  const std::string full_icon_path(app_icon_dir + "/" + policy_app_id);
  if (file_system::FileExists(full_icon_path)) {
    application->set_app_icon_path(full_icon_path);
  }

  // Stops timer of saving data to resumption in order to
  // doesn't erase data from resumption storage.
  // Timer will be started after hmi level resumption.
  resume_controller().OnAppRegistrationStart(policy_app_id, device_mac);

  return application;
}

void ApplicationManagerImpl::FinalizeAppRegistration(
    ApplicationSharedPtr application, const uint32_t connection_key) {
  AddAppToRegisteredAppList(application);

  // Update cloud app information, in case any pending apps are unable to be
  // registered due to a mobile app taking precedence
  RefreshCloudAppInformation();

  // It is possible that secondary transport of this app has been already
  // established. Make sure that the information is reflected to application
  // instance.
  // Also, make sure that this is done *after* we updated applications_ list to
  // avoid timing issues.
  DeviceMap::iterator itr =
      secondary_transport_devices_cache_.find(connection_key);
  if (secondary_transport_devices_cache_.end() != itr) {
    connection_handler::DeviceHandle secondary_device_handle = itr->second;
    application->set_secondary_device(secondary_device_handle);
  }
}

bool ApplicationManagerImpl::ActivateApplication(ApplicationSharedPtr app) {
  using namespace mobile_api;
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(app, false);

  SDL_LOG_DEBUG("Activating application with id: " << app->app_id());

  // Remove from resumption if app was activated by user
  resume_controller().OnAppActivated(app);

  // Activate any app services published by the app
  GetAppServiceManager().OnAppActivated(app);

  // Activate main window in state controller
  state_ctrl_.ActivateDefaultWindow(app);

  return true;
}

mobile_api::HMILevel::eType ApplicationManagerImpl::IsHmiLevelFullAllowed(
    ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  if (!app) {
    SDL_LOG_ERROR("Application pointer invalid");
    NOTREACHED();
    return mobile_api::HMILevel::INVALID_ENUM;
  }
  const bool is_audio_app = app->IsAudioApplication();
  const bool does_audio_app_with_same_type_exist =
      IsAppTypeExistsInFullOrLimited(app);
  const bool is_active_app_exist = (active_application().use_count() != 0);

  mobile_api::HMILevel::eType result = mobile_api::HMILevel::HMI_FULL;
  if (is_audio_app && does_audio_app_with_same_type_exist) {
    result = GetDefaultHmiLevel(app);
  } else if (is_active_app_exist && is_audio_app) {
    result = mobile_apis::HMILevel::HMI_LIMITED;
  } else if (is_active_app_exist && (!is_audio_app)) {
    result = GetDefaultHmiLevel(app);
  }
  SDL_LOG_ERROR("is_audio_app : "
                << is_audio_app << "; does_audio_app_with_same_type_exist : "
                << does_audio_app_with_same_type_exist
                << "; is_active_app_exist : " << is_active_app_exist
                << "; result : " << result);
  return result;
}

void ApplicationManagerImpl::ConnectToDevice(const std::string& device_mac) {
  // TODO(VS): Call function from ConnectionHandler
  if (!connection_handler_) {
    SDL_LOG_WARN("Connection handler is not set.");
    return;
  }

  connection_handler::DeviceHandle handle;
  if (!connection_handler().GetDeviceID(device_mac, &handle)) {
    SDL_LOG_ERROR(
        "Attempt to connect to invalid device with mac:" << device_mac);
    return;
  }
  connection_handler().ConnectToDevice(handle);
}

void ApplicationManagerImpl::OnHMIReady() {
  SDL_LOG_AUTO_TRACE();

#ifdef WEBSOCKET_SERVER_TRANSPORT_SUPPORT
  connection_handler_->CreateWebEngineDevice();
#endif  // WEBSOCKET_SERVER_TRANSPORT_SUPPORT

  MessageHelper::SendGetSystemInfoRequest(*this);

  std::shared_ptr<smart_objects::SmartObject> is_navi_ready(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::Navigation_IsReady, *this));
  rpc_service_->ManageHMICommand(is_navi_ready);

  std::shared_ptr<smart_objects::SmartObject> mixing_audio_supported_request(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported,
          *this));
  rpc_service_->ManageHMICommand(mixing_audio_supported_request);
  resume_controller().ResetLaunchTime();

  RefreshCloudAppInformation();
  policy_handler_->TriggerPTUOnStartupIfRequired();
}

void ApplicationManagerImpl::RequestForInterfacesAvailability() {
  SDL_LOG_AUTO_TRACE();
  std::shared_ptr<smart_objects::SmartObject> is_ivi_ready(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_IsReady, *this));
  rpc_service_->ManageHMICommand(is_ivi_ready);

  std::shared_ptr<smart_objects::SmartObject> is_vr_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_IsReady,
                                        *this));
  rpc_service_->ManageHMICommand(is_vr_ready);

  std::shared_ptr<smart_objects::SmartObject> is_tts_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::TTS_IsReady,
                                        *this));
  rpc_service_->ManageHMICommand(is_tts_ready);

  std::shared_ptr<smart_objects::SmartObject> is_ui_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::UI_IsReady,
                                        *this));
  rpc_service_->ManageHMICommand(is_ui_ready);

  std::shared_ptr<smart_objects::SmartObject> is_rc_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::RC_IsReady,
                                        *this));
  rpc_service_->ManageHMICommand(is_rc_ready);

  if (hmi_capabilities_->IsRequestsRequiredForCapabilities(
          hmi_apis::FunctionID::Buttons_GetCapabilities)) {
    std::shared_ptr<smart_objects::SmartObject> button_capabilities(
        MessageHelper::CreateModuleInfoSO(
            hmi_apis::FunctionID::Buttons_GetCapabilities, *this));
    rpc_service_->ManageHMICommand(button_capabilities);
  }
}

std::string ApplicationManagerImpl::PolicyIDByIconUrl(const std::string url) {
  sync_primitives::AutoLock lock(app_icon_map_lock_ptr_);
  for (auto& x : app_icon_map_) {
    auto policy_id = x.first;
    std::string icon_url = GetPolicyHandler().GetIconUrl(policy_id);
    if (icon_url == url) {
      SDL_LOG_DEBUG("Matched icon url: " << url);
      x.second.pending_request = false;
      return policy_id;
    }
  }
  return std::string("");
}

void ApplicationManagerImpl::SetIconFileFromSystemRequest(
    const std::string policy_id) {
  app_icon_map_lock_ptr_.Acquire();
  auto app_icon_it = app_icon_map_.find(policy_id);
  if (app_icon_it != app_icon_map_.end()) {
    app_icon_map_.erase(app_icon_it);
  }
  app_icon_map_lock_ptr_.Release();

  // Find pending application and set icon path
  auto app = pending_application_by_policy_id(policy_id);
  if (!app) {
    return;
  }
  const std::string app_icon_dir(settings_.app_icons_folder());
  const std::string full_icon_path(app_icon_dir + "/" + policy_id);
  if (file_system::FileExists(full_icon_path)) {
    SDL_LOG_DEBUG("Set Icon Path: " << full_icon_path);
    AppFile file;
    file.is_persistent = true;
    file.is_download_complete = true;
    file.file_name = full_icon_path;

    std::string icon_url = GetPolicyHandler().GetIconUrl(policy_id);
    std::string extension = boost::filesystem::extension(icon_url);
    if (extension == "bmp" || extension == "BMP") {
      file.file_type = mobile_apis::FileType::GRAPHIC_BMP;
    } else if (extension == "JPEG" || extension == "jpeg" ||
               extension == "JPG" || extension == "jpg") {
      file.file_type = mobile_apis::FileType::GRAPHIC_JPEG;
    } else {
      file.file_type = mobile_apis::FileType::GRAPHIC_PNG;
    }

    app->AddFile(file);
    app->set_app_icon_path(full_icon_path);
  }
  SendUpdateAppList();
}

void ApplicationManagerImpl::DisconnectCloudApp(ApplicationSharedPtr app) {
#if !defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
  SDL_LOG_TRACE("Cloud app support is disabled. Exiting function");
  return;
#else
  std::string policy_app_id = app->policy_app_id();
  policy::AppProperties app_properties;
  GetPolicyHandler().GetAppProperties(policy_app_id, app_properties);
  if (app->IsRegistered() && app->is_cloud_app()) {
    SDL_LOG_DEBUG("Disabled app is registered, unregistering now");
    GetRPCService().ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app->app_id(),
            mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED),
        commands::Command::SOURCE_SDL);

    OnAppUnauthorized(app->app_id());
  }
  // Delete the cloud device
  connection_handler().RemoveCloudAppDevice(app->device());

  transport_manager::transport_adapter::CloudAppProperties properties{
      app_properties.endpoint,
      app_properties.certificate,
      app_properties.enabled,
      app_properties.auth_token,
      app_properties.transport_type,
      app_properties.hybrid_app_preference};
  // Create device in pending state
  SDL_LOG_DEBUG("Re-adding the cloud app device");
  connection_handler().AddCloudAppDevice(policy_app_id, properties);
#endif  // CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT
}

void ApplicationManagerImpl::RefreshCloudAppInformation() {
#if !defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
  SDL_LOG_TRACE("Cloud app support is disabled. Exiting function");
  return;
#else
  SDL_LOG_AUTO_TRACE();
  if (is_stopping()) {
    return;
  }
  std::vector<std::string> enabled_apps;
  GetPolicyHandler().GetEnabledCloudApps(enabled_apps);
  std::vector<std::string>::iterator enabled_it = enabled_apps.begin();
  std::vector<std::string>::iterator enabled_end = enabled_apps.end();

  // Store old device map and clear the current map
  pending_device_map_lock_ptr_->Acquire();
  app_icon_map_lock_ptr_.Acquire();
  std::map<std::string, std::string> old_device_map = pending_device_map_;
  pending_device_map_ = std::map<std::string, std::string>();
  // Create a device for each newly enabled cloud app
  policy::AppProperties app_properties;
  for (; enabled_it != enabled_end; ++enabled_it) {
    GetPolicyHandler().GetAppProperties(*enabled_it, app_properties);

    if (app_properties.endpoint.empty()) {
      continue;
    }

    mobile_apis::HybridAppPreference::eType hybrid_app_preference =
        mobile_apis::HybridAppPreference::INVALID_ENUM;
    smart_objects::EnumConversionHelper<
        mobile_apis::HybridAppPreference::eType>::
        StringToEnum(app_properties.hybrid_app_preference,
                     &hybrid_app_preference);

    auto policy_id = *enabled_it;
    policy::StringArray nicknames;
    policy::StringArray app_hmi_types;
    GetPolicyHandler().GetInitialAppData(policy_id, &nicknames, &app_hmi_types);

    if (nicknames.empty()) {
      SDL_LOG_ERROR("Cloud App missing nickname");
      continue;
    } else if (mobile_apis::HybridAppPreference::MOBILE ==
               hybrid_app_preference) {
      auto nickname_it = nicknames.begin();
      bool duplicate_found = false;
      for (; nickname_it != nicknames.end(); ++nickname_it) {
        auto apps = applications_by_name(*nickname_it);
        for (auto app : apps) {
          if (app.use_count() != 0 && !app->is_cloud_app()) {
            SDL_LOG_ERROR("Mobile app already registered for cloud app: "
                          << *nickname_it);
            duplicate_found = true;
            break;
          }
        }
      }

      if (duplicate_found) {
        continue;
      }
    }

    pending_device_map_.insert(std::pair<std::string, std::string>(
        app_properties.endpoint, policy_id));
    // Determine which endpoints were disabled by erasing all enabled apps from
    // the old device list
    auto old_device_it = old_device_map.find(app_properties.endpoint);
    if (old_device_it != old_device_map.end()) {
      old_device_map.erase(old_device_it);
    }

    transport_manager::transport_adapter::CloudAppProperties properties{
        app_properties.endpoint,
        app_properties.certificate,
        app_properties.enabled,
        app_properties.auth_token,
        app_properties.transport_type,
        app_properties.hybrid_app_preference};

    // If the device was disconnected, this will reinitialize the device
    connection_handler().AddCloudAppDevice(policy_id, properties);

    // Look for app icon url data and add to app_icon_url_map
    std::string url = GetPolicyHandler().GetIconUrl(policy_id);

    if (url.empty()) {
      SDL_LOG_DEBUG("No Icon Url for cloud app");
      continue;
    }

    auto app_icon_it = app_icon_map_.find(policy_id);
    if (app_icon_it != app_icon_map_.end()) {
      SDL_LOG_DEBUG("Cloud App Already Exists in Icon Map");
      continue;
    }

    const std::string app_icon_dir(settings_.app_icons_folder());
    const std::string full_icon_path(app_icon_dir + "/" + policy_id);
    if (!file_system::FileExists(full_icon_path)) {
      AppIconInfo icon_info(app_properties.endpoint, false);
      SDL_LOG_DEBUG(
          "Inserting cloud app into icon map: " << app_icon_map_.size());
      app_icon_map_.insert(
          std::pair<std::string, AppIconInfo>(policy_id, icon_info));
    }
  }
  app_icon_map_lock_ptr_.Release();
  pending_device_map_lock_ptr_->Release();

  int removed_app_count = 0;
  // Clear out devices for existing cloud apps that were disabled
  for (auto& device : old_device_map) {
    std::string policy_app_id = device.second;
    // First search for the disabled app within the registered apps
    ApplicationSharedPtr app = application_by_policy_id(policy_app_id);
    if (app.use_count() == 0) {
      sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
      // If the disabled app is not present in the registered app list, check
      // the apps awaiting registration
      PolicyAppIdPredicate finder(policy_app_id);
      ApplicationSet::iterator it = std::find_if(
          apps_to_register_.begin(), apps_to_register_.end(), finder);
      if (it == apps_to_register_.end()) {
        SDL_LOG_DEBUG("Unable to find app to remove (" << policy_app_id
                                                       << "), skipping");
        continue;
      }
      app = *it;
      apps_to_register_.erase(it);
    }
    // If the disabled app is registered, unregistered it before destroying the
    // device
    if (app->IsRegistered() && app->is_cloud_app()) {
      SDL_LOG_DEBUG("Disabled app is registered, unregistering now");
      GetRPCService().ManageMobileCommand(
          MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
              app->app_id(),
              mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED),
          commands::Command::SOURCE_SDL);

      OnAppUnauthorized(app->app_id());
    }
    // Delete the cloud device
    connection_handler().RemoveCloudAppDevice(app->device());
    removed_app_count++;
  }

  // Update app list if disabled apps were removed
  if (removed_app_count > 0) {
    SDL_LOG_DEBUG("Removed " << removed_app_count << " disabled apps");
    SendUpdateAppList();
  }
#endif  // CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT
}

void ApplicationManagerImpl::CreatePendingApplication(
    const transport_manager::ConnectionUID connection_id,
    const transport_manager::DeviceInfo& device_info,
    connection_handler::DeviceHandle device_id) {
  SDL_LOG_AUTO_TRACE();

  std::string name = device_info.name();
  pending_device_map_lock_ptr_->Acquire();
  auto it = pending_device_map_.find(name);
  if (it == pending_device_map_.end()) {
    pending_device_map_lock_ptr_->Release();
    return;
  }
  pending_device_map_lock_ptr_->Release();

  const std::string policy_app_id = it->second;

  policy::StringArray nicknames;
  policy::StringArray app_hmi_types;

  GetPolicyHandler().GetInitialAppData(
      policy_app_id, &nicknames, &app_hmi_types);

  if (nicknames.empty()) {
    SDL_LOG_ERROR("Cloud App missing nickname");
    return;
  }

  const std::string display_name = nicknames[0];

  ApplicationSharedPtr application(
      new ApplicationImpl(0,
                          policy_app_id,
                          device_info.mac_address(),
                          device_id,
                          custom_str::CustomString(display_name),
                          GetPolicyHandler().GetStatisticManager(),
                          *this));

  if (!application) {
    SDL_LOG_INFO("Could not create application");
    return;
  }

  const std::string app_icon_dir(settings_.app_icons_folder());
  const std::string full_icon_path(app_icon_dir + "/" + policy_app_id);
  if (file_system::FileExists(full_icon_path)) {
    application->set_app_icon_path(full_icon_path);
  }
  policy::AppProperties app_properties;
  GetPolicyHandler().GetAppProperties(policy_app_id, app_properties);

  mobile_apis::HybridAppPreference::eType hybrid_app_preference_enum;

  const bool convert_result = smart_objects::EnumConversionHelper<
      mobile_apis::HybridAppPreference::eType>::
      StringToEnum(app_properties.hybrid_app_preference,
                   &hybrid_app_preference_enum);

  if (!app_properties.hybrid_app_preference.empty() && !convert_result) {
    SDL_LOG_ERROR("Could not convert string to enum: "
                  << app_properties.hybrid_app_preference);
    return;
  }

  application->set_hmi_application_id(GenerateNewHMIAppID());
  application->set_cloud_app_endpoint(app_properties.endpoint);
  application->set_auth_token(app_properties.auth_token);
  application->set_cloud_app_transport_type(app_properties.transport_type);
  application->set_hybrid_app_preference(hybrid_app_preference_enum);
  application->set_cloud_app_certificate(app_properties.certificate);

  {
    sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
    SDL_LOG_DEBUG(
        "apps_to_register_ size before: " << apps_to_register_.size());
    apps_to_register_.insert(application);
    SDL_LOG_DEBUG("apps_to_register_ size after: " << apps_to_register_.size());
  }

  SendUpdateAppList();
}

void ApplicationManagerImpl::RemovePendingApplication(
    const std::string& policy_app_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
  PolicyAppIdPredicate finder(policy_app_id);
  auto app_it =
      std::find_if(apps_to_register_.begin(), apps_to_register_.end(), finder);

  if (apps_to_register_.end() == app_it) {
    SDL_LOG_WARN("Unable to find app to remove (" << policy_app_id
                                                  << "), skipping");
    return;
  }

  apps_to_register_.erase(app_it);
  SDL_LOG_DEBUG("Remove " << policy_app_id
                          << " from apps_to_register_. new size = "
                          << apps_to_register_.size());
}

void ApplicationManagerImpl::CreatePendingLocalApplication(
    const std::string& policy_app_id) {
  policy::StringArray nicknames;
  policy::StringArray app_hmi_types;

  GetPolicyHandler().GetInitialAppData(
      policy_app_id, &nicknames, &app_hmi_types);

  if (nicknames.empty()) {
    SDL_LOG_ERROR("Cloud/Web App " << policy_app_id << " missing nickname");
    return;
  }

  const std::string display_name = nicknames[0];

  const auto web_engine_device = connection_handler_->GetWebEngineDeviceInfo();

  ApplicationSharedPtr application(
      new ApplicationImpl(0,
                          policy_app_id,
                          web_engine_device.mac_address(),
                          web_engine_device.device_handle(),
                          custom_str::CustomString(display_name),
                          GetPolicyHandler().GetStatisticManager(),
                          *this));

  const std::string app_icon_dir(settings_.app_icons_folder());
  const std::string full_icon_path(app_icon_dir + "/" + policy_app_id);
  if (file_system::FileExists(full_icon_path)) {
    application->set_app_icon_path(full_icon_path);
  }
  policy::AppProperties app_properties;
  GetPolicyHandler().GetAppProperties(policy_app_id, app_properties);

  mobile_apis::HybridAppPreference::eType hybrid_app_preference_enum;
  const bool convert_result = smart_objects::EnumConversionHelper<
      mobile_apis::HybridAppPreference::eType>::
      StringToEnum(app_properties.hybrid_app_preference,
                   &hybrid_app_preference_enum);

  if (!app_properties.hybrid_app_preference.empty() && !convert_result) {
    SDL_LOG_ERROR("Could not convert string to enum: "
                  << app_properties.hybrid_app_preference);
    return;
  }

  application->set_hmi_application_id(GenerateNewHMIAppID());
  application->set_cloud_app_endpoint(app_properties.endpoint);
  application->set_auth_token(app_properties.auth_token);
  application->set_cloud_app_transport_type(app_properties.transport_type);
  application->set_hybrid_app_preference(hybrid_app_preference_enum);
  application->set_cloud_app_certificate(app_properties.certificate);

  sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
  apps_to_register_.insert(application);
  SDL_LOG_DEBUG("Insert " << application->name().c_str()
                          << " to apps_to_register_. new size = "
                          << apps_to_register_.size());
}

void ApplicationManagerImpl::OnWebEngineDeviceCreated() {
  SDL_LOG_AUTO_TRACE();
  const auto enabled_local_apps = policy_handler_->GetEnabledLocalApps();

  if (enabled_local_apps.empty()) {
    SDL_LOG_DEBUG("No enabled local apps present");
    return;
  }

  for (auto policy_app_id : enabled_local_apps) {
    CreatePendingLocalApplication(policy_app_id);
  }
  SendUpdateAppList();
}

void ApplicationManagerImpl::SetPendingApplicationState(
    const transport_manager::ConnectionUID connection_id,
    const transport_manager::DeviceInfo& device_info) {
  std::string name = device_info.name();
  pending_device_map_lock_ptr_->Acquire();
  auto it = pending_device_map_.find(name);
  if (it == pending_device_map_.end()) {
    pending_device_map_lock_ptr_->Release();
    return;
  }
  pending_device_map_lock_ptr_->Release();

  const std::string policy_app_id = it->second;
  auto app = application_by_policy_id(policy_app_id);

  if (!app) {
    return;
  }
  SDL_LOG_DEBUG("Unregister application and move into apps_to_register");

  UnregisterApplication(
      app->app_id(), mobile_apis::Result::INVALID_ENUM, true, true);
  app->MarkUnregistered();

  sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
  apps_to_register_.insert(app);
}

void ApplicationManagerImpl::OnConnectionStatusUpdated() {
  SendUpdateAppList();
}

hmi_apis::Common_CloudConnectionStatus::eType
ApplicationManagerImpl::GetCloudAppConnectionStatus(
    ApplicationConstSharedPtr app) const {
#if !defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
  return hmi_apis::Common_CloudConnectionStatus::INVALID_ENUM;
#else
  transport_manager::ConnectionStatus status =
      connection_handler().GetConnectionStatus(app->device());
  switch (status) {
    case transport_manager::ConnectionStatus::CONNECTED:
      return hmi_apis::Common_CloudConnectionStatus::CONNECTED;
    case transport_manager::ConnectionStatus::RETRY:
      return hmi_apis::Common_CloudConnectionStatus::RETRY;
    case transport_manager::ConnectionStatus::PENDING:
    case transport_manager::ConnectionStatus::CLOSING:
      return hmi_apis::Common_CloudConnectionStatus::NOT_CONNECTED;
    default:
      return hmi_apis::Common_CloudConnectionStatus::INVALID_ENUM;
  }
#endif  // CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT
}

uint32_t ApplicationManagerImpl::GetNextMobileCorrelationID() {
  if (mobile_correlation_id_ < max_correlation_id_) {
    mobile_correlation_id_++;
  } else {
    mobile_correlation_id_ = 0;
  }

  return mobile_correlation_id_;
}

uint32_t ApplicationManagerImpl::GetNextHMICorrelationID() {
  if (correlation_id_ < max_correlation_id_) {
    correlation_id_++;
  } else {
    correlation_id_ = 0;
  }

  return correlation_id_;
}

bool ApplicationManagerImpl::BeginAudioPassThru(uint32_t app_id) {
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  if (audio_pass_thru_active_) {
    return false;
  } else {
    audio_pass_thru_active_ = true;
    audio_pass_thru_app_id_ = app_id;
    return true;
  }
}

bool ApplicationManagerImpl::EndAudioPassThru(uint32_t app_id) {
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  if (audio_pass_thru_active_ && audio_pass_thru_app_id_ == app_id) {
    audio_pass_thru_active_ = false;
    audio_pass_thru_app_id_ = 0;
    return true;
  } else {
    return false;
  }
}

hmi_apis::Common_DriverDistractionState::eType
ApplicationManagerImpl::driver_distraction_state() const {
  return driver_distraction_state_;
}

void ApplicationManagerImpl::set_driver_distraction_state(
    const hmi_apis::Common_DriverDistractionState::eType state) {
  driver_distraction_state_ = state;
}

void ApplicationManagerImpl::SetAllAppsAllowed(const bool allowed) {
  is_all_apps_allowed_ = allowed;
}

HmiStatePtr ApplicationManagerImpl::CreateRegularState(
    std::shared_ptr<Application> app,
    const mobile_apis::WindowType::eType window_type,
    const mobile_apis::HMILevel::eType hmi_level,
    const mobile_apis::AudioStreamingState::eType audio_state,
    const mobile_apis::VideoStreamingState::eType video_state,
    const mobile_apis::SystemContext::eType system_context) const {
  HmiStatePtr state(new HmiState(app, *this));
  state->set_window_type(window_type);
  state->set_hmi_level(hmi_level);
  state->set_audio_streaming_state(audio_state);
  state->set_video_streaming_state(video_state);
  state->set_system_context(system_context);
  return state;
}

void ApplicationManagerImpl::StartAudioPassThruThread(int32_t session_key,
                                                      int32_t correlation_id,
                                                      int32_t max_duration,
                                                      int32_t sampling_rate,
                                                      int32_t bits_per_sample,
                                                      int32_t audio_type) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_INFO("START MICROPHONE RECORDER");
  DCHECK_OR_RETURN_VOID(media_manager_);
  media_manager_->StartMicrophoneRecording(
      session_key,
      get_settings().recording_file_name(),
      max_duration,
      static_cast<mobile_apis::SamplingRate::eType>(sampling_rate),
      static_cast<mobile_apis::BitsPerSample::eType>(bits_per_sample),
      static_cast<mobile_apis::AudioType::eType>(audio_type));
}

void ApplicationManagerImpl::StopAudioPassThru(int32_t application_key) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  DCHECK_OR_RETURN_VOID(media_manager_);
  media_manager_->StopMicrophoneRecording(application_key);
}

std::string ApplicationManagerImpl::GetDeviceName(
    connection_handler::DeviceHandle handle) {
  DCHECK(connection_handler_);
  std::string device_name = "";
  if (connection_handler().get_session_observer().GetDataOnDeviceID(
          handle, &device_name, NULL, NULL, NULL) == -1) {
    SDL_LOG_ERROR("Failed to extract device name for id " << handle);
  } else {
    SDL_LOG_DEBUG("\t\t\t\t\tDevice name is " << device_name);
  }

  return device_name;
}

hmi_apis::Common_TransportType::eType
ApplicationManagerImpl::GetDeviceTransportType(
    const std::string& transport_type) {
  hmi_apis::Common_TransportType::eType result =
      hmi_apis::Common_TransportType::INVALID_ENUM;

  DeviceTypes::const_iterator it = devicesType.find(transport_type);
  if (it != devicesType.end()) {
    return devicesType[transport_type];
  } else {
    SDL_LOG_ERROR("Unknown transport type " << transport_type);
  }

  return result;
}

ApplicationConstSharedPtr ApplicationManagerImpl::WaitingApplicationByID(
    const uint32_t hmi_id) const {
  AppsWaitRegistrationSet app_list = AppsWaitingForRegistration().GetData();

  AppsWaitRegistrationSet::const_iterator it_end = app_list.end();

  HmiAppIdPredicate finder(hmi_id);
  ApplicationSharedPtr result;
  ApplicationSetConstIt it_app = std::find_if(app_list.begin(), it_end, finder);
  if (it_app != it_end) {
    result = *it_app;
  }
  return result;
}

DataAccessor<AppsWaitRegistrationSet>
ApplicationManagerImpl::AppsWaitingForRegistration() const {
  return DataAccessor<AppsWaitRegistrationSet>(apps_to_register_,
                                               apps_to_register_list_lock_ptr_);
}

bool ApplicationManagerImpl::IsAppsQueriedFrom(
    const connection_handler::DeviceHandle handle) const {
  sync_primitives::AutoLock lock(query_apps_devices_lock_);
  return query_apps_devices_.find(handle) != query_apps_devices_.end();
}

StateController& ApplicationManagerImpl::state_controller() {
  return state_ctrl_;
}

const ApplicationManagerSettings& ApplicationManagerImpl::get_settings() const {
  return settings_;
}

// Extract the app ID to use for policy based on the UseFullAppID .ini setting
std::string ApplicationManagerImpl::GetCorrectMobileIDFromMessage(
    const commands::MessageSharedPtr& message) const {
  // If core is expecting a fullAppID
  if (get_settings().use_full_app_id()) {
    // fullAppID is present and core is configured to use it
    if ((*message)[strings::msg_params].keyExists(strings::full_app_id)) {
      return (*message)[strings::msg_params][strings::full_app_id].asString();
    } else {
      SDL_LOG_DEBUG("UseFullAppID is on but only short ID given!");
    }
  }
  // If core isn't using full or no full given, use regular appID
  return (*message)[strings::msg_params][strings::app_id].asString();
}

void application_manager::ApplicationManagerImpl::MarkAppsGreyOut(
    const connection_handler::DeviceHandle handle, bool is_greyed_out) {
  sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
  AppsWaitRegistrationSet::iterator it = apps_to_register_.begin();
  AppsWaitRegistrationSet::const_iterator it_end = apps_to_register_.end();
  for (; it != it_end; ++it) {
    if (handle == (*it)->device()) {
      (*it)->set_greyed_out(is_greyed_out);
    }
  }
}

void ApplicationManagerImpl::OnDeviceListUpdated(
    const connection_handler::DeviceMap& device_list) {
  SDL_LOG_AUTO_TRACE();

  // add device to policy DB
  connection_handler::DeviceMap::const_iterator it = device_list.begin();
  for (; device_list.end() != it; ++it) {
    policy::DeviceParams dev_params;
    connection_handler().get_session_observer().GetDataOnDeviceID(
        it->second.device_handle(),
        &dev_params.device_name,
        NULL,
        &dev_params.device_mac_address,
        &dev_params.device_connection_type);

    policy::DeviceInfo device_info;
    device_info.AdoptDeviceType(dev_params.device_connection_type);

    GetPolicyHandler().AddDevice(dev_params.device_mac_address,
                                 device_info.connection_type);
    app_launch_ctrl().OnDeviceConnected(dev_params.device_mac_address);
  }

  smart_objects::SmartObjectSPtr msg_params =
      MessageHelper::CreateDeviceListSO(device_list, GetPolicyHandler(), *this);
  if (!msg_params) {
    SDL_LOG_WARN("Failed to create sub-smart object.");
    return;
  }

  smart_objects::SmartObjectSPtr update_list =
      std::make_shared<smart_objects::SmartObject>();
  smart_objects::SmartObject& so_to_send = *update_list;
  so_to_send[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
      hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList;
  so_to_send[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
      hmi_apis::messageType::request;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 3;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = 1;
  so_to_send[jhs::S_PARAMS][jhs::S_CORRELATION_ID] = GetNextHMICorrelationID();
  so_to_send[jhs::S_MSG_PARAMS] = *msg_params;
  rpc_service_->ManageHMICommand(update_list);
  RefreshCloudAppInformation();
}

void ApplicationManagerImpl::OnFindNewApplicationsRequest() {
  connection_handler().ConnectToAllDevices();
  SDL_LOG_DEBUG("Starting application list update timer");
  uint32_t timeout = get_settings().application_list_update_timeout();
  application_list_update_timer_.Start(timeout, timer::kSingleShot);
  GetPolicyHandler().OnAppsSearchStarted();
}

void ApplicationManagerImpl::OnQueryAppsRequest(
    const connection_handler::DeviceHandle device) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(query_apps_devices_lock_);
  query_apps_devices_.insert(device);
}

void ApplicationManagerImpl::SendUpdateAppList() {
  SDL_LOG_AUTO_TRACE();

  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObjectSPtr request = MessageHelper::CreateModuleInfoSO(
      FunctionID::BasicCommunication_UpdateAppList, *this);

  (*request)[strings::msg_params][strings::applications] =
      SmartObject(SmartType_Array);

  SmartObject& applications_so =
      (*request)[strings::msg_params][strings::applications];

  const auto applications_list = applications().GetData();
  PrepareApplicationListSO(applications_list, applications_so, *this);

  const auto pending_apps_list = AppsWaitingForRegistration().GetData();
  PrepareApplicationListSO(pending_apps_list, applications_so, *this);

  rpc_service_->ManageHMICommand(request);
}

void ApplicationManagerImpl::RemoveDevice(
    const connection_handler::DeviceHandle& device_handle) {
  SDL_LOG_DEBUG("device_handle " << device_handle);
  sync_primitives::AutoLock lock(query_apps_devices_lock_);
  query_apps_devices_.erase(device_handle);
}

void ApplicationManagerImpl::OnDeviceSwitchingStart(
    const connection_handler::Device& device_from,
    const connection_handler::Device& device_to) {
  SDL_LOG_AUTO_TRACE();
  ReregisterWaitList wait_list;
  {
    auto apps_data_accessor = applications();

    std::copy_if(apps_data_accessor.GetData().begin(),
                 apps_data_accessor.GetData().end(),
                 std::back_inserter(wait_list),
                 std::bind1st(std::ptr_fun(&device_id_comparator),
                              device_from.mac_address()));
  }

  {
    // During sending of UpdateDeviceList this lock is acquired also so making
    // it scoped
    sync_primitives::AutoLock lock(reregister_wait_list_lock_ptr_);
    std::copy(wait_list.begin(),
              wait_list.end(),
              std::back_inserter(reregister_wait_list_));
  }

  for (const auto& app : wait_list) {
    request_ctrl_.terminateAppRequests(app->app_id());
    resume_ctrl_->SaveApplication(app);
  }

  policy_handler_->OnDeviceSwitching(device_from.mac_address(),
                                     device_to.mac_address());
}

void ApplicationManagerImpl::OnDeviceSwitchingFinish(
    const std::string& device_uid) {
  SDL_LOG_AUTO_TRACE();
  UNUSED(device_uid);

  ReregisterWaitList wait_list;
  {
    sync_primitives::AutoLock lock(reregister_wait_list_lock_ptr_);
    wait_list.swap(reregister_wait_list_);
  }

  const bool unexpected_disonnect = true;
  const bool is_resuming = true;

  for (const auto& app : wait_list) {
    UnregisterApplication(app->app_id(),
                          mobile_apis::Result::INVALID_ENUM,
                          is_resuming,
                          unexpected_disonnect);
  }
}

void ApplicationManagerImpl::SwitchApplication(ApplicationSharedPtr app,
                                               const uint32_t connection_key,
                                               const size_t device_id,
                                               const std::string& mac_address) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(app);
  sync_primitives::AutoLock lock(applications_list_lock_ptr_);
  DCHECK_OR_RETURN_VOID(1 == applications_.erase(app));

  SDL_LOG_DEBUG("Changing app id to "
                << connection_key << ". Changing device id to " << device_id);

  bool is_subscribed_to_way_points = IsAppSubscribedForWayPoints(*app);
  if (is_subscribed_to_way_points) {
    UnsubscribeAppFromWayPoints(app, false);
  }
  SwitchApplicationParameters(app, connection_key, device_id, mac_address);
  if (is_subscribed_to_way_points) {
    SubscribeAppForWayPoints(app, false);
  }

  // Normally this is done during registration, however since switched apps are
  // not being registered again need to set protocol version on session.
  connection_handler().BindProtocolVersionWithSession(
      connection_key, static_cast<uint8_t>(app->protocol_version()));

  // Application need to be re-inserted in order to keep sorting in applications
  // container. Otherwise data loss on erasing is possible.
  applications_.insert(app);
}

mobile_apis::HMILevel::eType ApplicationManagerImpl::GetDefaultHmiLevel(
    ApplicationConstSharedPtr application) const {
  using namespace mobile_apis;
  SDL_LOG_AUTO_TRACE();
  HMILevel::eType default_hmi = HMILevel::HMI_NONE;

  if (GetPolicyHandler().PolicyEnabled()) {
    const std::string policy_app_id = application->policy_app_id();
    std::string default_hmi_string = "";
    if (GetPolicyHandler().GetDefaultHmi(
            application->mac_address(), policy_app_id, &default_hmi_string)) {
      if ("BACKGROUND" == default_hmi_string) {
        default_hmi = HMILevel::HMI_BACKGROUND;
      } else if ("FULL" == default_hmi_string) {
        default_hmi = HMILevel::HMI_FULL;
      } else if ("LIMITED" == default_hmi_string) {
        default_hmi = HMILevel::HMI_LIMITED;
      } else if ("NONE" == default_hmi_string) {
        default_hmi = HMILevel::HMI_NONE;
      } else {
        SDL_LOG_ERROR("Unable to convert " + default_hmi_string +
                      " to HMILevel");
      }
    } else {
      SDL_LOG_ERROR("Unable to get default hmi_level for " << policy_app_id);
    }
  }
  return default_hmi;
}

bool ApplicationManagerImpl::CheckResumptionRequiredTransportAvailable(
    ApplicationConstSharedPtr application) const {
  using namespace mobile_apis;
  SDL_LOG_AUTO_TRACE();

  const std::map<std::string, std::vector<std::string> >& transport_map =
      get_settings().transport_required_for_resumption_map();

  // retrieve transport type string used in .ini file
  const std::string transport_type =
      GetTransportTypeProfileString(application->device());
  const std::string secondary_transport_type =
      GetTransportTypeProfileString(application->secondary_device());

  const smart_objects::SmartObject* app_types_array = application->app_types();
  if (app_types_array == NULL || app_types_array->length() == 0) {
    // This app does not have any AppHMIType. In this case, check "EMPTY_APP"
    // entry
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        transport_map.find(std::string("EMPTY_APP"));
    if (it == transport_map.end()) {
      // if "EMPTY_APP" is not specified, resumption is always enabled
      return true;
    }
    const std::vector<std::string>& required_transport_list = it->second;

    for (std::vector<std::string>::const_iterator itr =
             required_transport_list.begin();
         itr != required_transport_list.end();
         ++itr) {
      if (transport_type == *itr || secondary_transport_type == *itr) {
        return true;
      }
    }
    return false;
  } else {
    // check all AppHMITypes that the app has
    for (size_t i = 0; i < app_types_array->length(); i++) {
      std::string app_type_string =
          EnumToString(static_cast<mobile_apis::AppHMIType::eType>(
              app_types_array->getElement(i).asUInt()));
      bool transport_is_found = false;

      std::map<std::string, std::vector<std::string> >::const_iterator it =
          transport_map.find(app_type_string);
      if (it == transport_map.end()) {
        // this AppHMIType is not listed in .ini file, so resumption is always
        // enabled
        continue;
      }

      const std::vector<std::string>& required_transport_list = it->second;
      for (std::vector<std::string>::const_iterator itr =
               required_transport_list.begin();
           itr != required_transport_list.end();
           ++itr) {
        if (transport_type == *itr || secondary_transport_type == *itr) {
          transport_is_found = true;
          break;
        }
      }

      // if neither primary or secondary transport type is included in the list,
      // then resumption will be disabled
      if (!transport_is_found) {
        return false;
      }
    }

    return true;
  }
}

uint32_t ApplicationManagerImpl::GenerateGrammarID() {
  return rand();
}

uint32_t ApplicationManagerImpl::GenerateNewHMIAppID() {
  SDL_LOG_AUTO_TRACE();
  uint32_t hmi_app_id = get_rand_from_range(1);
  SDL_LOG_DEBUG("GenerateNewHMIAppID value is: " << hmi_app_id);

  while (resume_controller().IsHMIApplicationIdExist(hmi_app_id)) {
    SDL_LOG_DEBUG("HMI appID " << hmi_app_id << " is exists.");
    hmi_app_id = get_rand_from_range(1);
    SDL_LOG_DEBUG("Trying new value: " << hmi_app_id);
  }

  return hmi_app_id;
}

bool ApplicationManagerImpl::StartNaviService(
    uint32_t app_id,
    protocol_handler::ServiceType service_type,
    const BsonObject* params) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  if (HMIStateAllowsStreaming(app_id, service_type)) {
    {
      sync_primitives::AutoLock lock(navi_service_status_lock_);

      NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
      if (navi_service_status_.end() == it) {
        std::pair<NaviServiceStatusMap::iterator, bool> res =
            navi_service_status_.insert(
                std::pair<uint32_t, std::pair<bool, bool> >(
                    app_id, std::make_pair(false, false)));
        if (!res.second) {
          SDL_LOG_WARN("Navi service refused");
          return false;
        }
        it = res.first;
      }
    }

    {
      /* Fix: For NaviApp1 Switch to NaviApp2, App1's Endcallback() arrives
       later than App2's Startcallback(). Cause streaming issue on HMI.
      */
      auto accessor = applications();
      for (auto app : accessor.GetData()) {
        if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
          SDL_LOG_DEBUG("Continue, Not Navi App Id: " << app->app_id());
          continue;
        }
        SDL_LOG_DEBUG("Abort Stream Service of other NaviAppId: "
                      << app->app_id() << " Service_type: " << service_type);
        StopNaviService(app->app_id(), service_type);
      }
    }

    if (service_type == ServiceType::kMobileNav) {
      smart_objects::SmartObject converted_params(smart_objects::SmartType_Map);
      ConvertVideoParamsToSO(converted_params, params);
      std::vector<std::string> rejected_params;
      if (converted_params.keyExists(strings::codec) &&
          converted_params[strings::codec] ==
              hmi_apis::Common_VideoStreamingCodec::INVALID_ENUM) {
        rejected_params.push_back(strings::codec);
      }
      if (converted_params.keyExists(strings::protocol) &&
          converted_params[strings::protocol] ==
              hmi_apis::Common_VideoStreamingProtocol::INVALID_ENUM) {
        rejected_params.push_back(strings::protocol);
      }

      if (!rejected_params.empty()) {
        OnStreamingConfigurationFailed(app_id, rejected_params, std::string());
        return false;
      } else if (!converted_params.empty()) {
        SDL_LOG_INFO("Sending video configuration params");
        MessageHelper::PrintSmartObject(converted_params);
        bool request_sent =
            application(app_id)->SetVideoConfig(service_type, converted_params);
        if (request_sent) {
          return true;
        }
      }
    }
    // no configuration is needed, or SetVideoConfig is not sent
    OnStreamingConfigurationSuccessful(app_id, service_type);
    return true;
  }
  SDL_LOG_WARN("Refused navi service by HMI level");
  std::vector<std::string> empty;
  OnStreamingConfigurationFailed(
      app_id,
      empty,
      "Service type: " + std::to_string(service_type) +
          " disallowed with current HMI level");
  return false;
}

void ApplicationManagerImpl::OnStreamingConfigurationSuccessful(
    uint32_t app_id, protocol_handler::ServiceType service_type) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_INFO("Streaming configuration successful for service "
               << service_type);
  std::vector<std::string> empty;
  std::string reason;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it) {
      SDL_LOG_WARN("Application not found in navi status map");
      connection_handler().NotifyServiceStartedResult(
          app_id, false, empty, reason);
      return;
    }

    // Fill NaviServices map. Set true to first value of pair if
    // we've started video service or to second value if we've
    // started audio service
    service_type == protocol_handler::ServiceType::kMobileNav
        ? it->second.first = true
        : it->second.second = true;

    {
      sync_primitives::AutoLock lock(navi_app_to_stop_lock_);
      for (size_t i = 0; i < navi_app_to_stop_.size(); ++i) {
        if (app_id == navi_app_to_stop_[i]) {
          sync_primitives::AutoLock lock(close_app_timer_pool_lock_);
          close_app_timer_pool_.erase(close_app_timer_pool_.begin() + i);
          navi_app_to_stop_.erase(navi_app_to_stop_.begin() + i);
          break;
        }
      }
    }
  }

  application(app_id)->StartStreaming(service_type);
  connection_handler().NotifyServiceStartedResult(app_id, true, empty, reason);

  // Fix: For wifi Secondary
  // Should erase appid from deque of ForbidStreaming() push in the last time
  std::deque<uint32_t>::const_iterator iter = std::find(
      navi_app_to_end_stream_.begin(), navi_app_to_end_stream_.end(), app_id);
  if (navi_app_to_end_stream_.end() != iter) {
    navi_app_to_end_stream_.erase(iter);
  }
}

void ApplicationManagerImpl::OnStreamingConfigurationFailed(
    uint32_t app_id,
    std::vector<std::string>& rejected_params,
    const std::string& reason) {
  SDL_LOG_AUTO_TRACE();

  std::vector<std::string> converted_params =
      ConvertRejectedParamList(rejected_params);
  connection_handler().NotifyServiceStartedResult(
      app_id, false, converted_params, reason);
}

void ApplicationManagerImpl::StopNaviService(
    uint32_t app_id, protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it) {
      SDL_LOG_WARN("No Information about navi service " << service_type);
      // Fix: Need return for Not navi service at now
      return;
    } else {
      // Fix: Repeated tests are not executed after they have stopped for Navi
      if (false == it->second.first &&
          ServiceType::kMobileNav == service_type) {
        SDL_LOG_DEBUG("appId: " << app_id << "Navi had stopped");
        return;
      }

      // Fix: Repeated tests are not executed after they have stopped for Audio
      if (false == it->second.second && ServiceType::kAudio == service_type) {
        SDL_LOG_DEBUG("appId: " << app_id << "Audio had stopped");
        return;
      }
      // Fill NaviServices map. Set false to first value of pair if
      // we've stopped video service or to second value if we've
      // stopped audio service
      SDL_LOG_DEBUG("appId: " << app_id << " service_type: " << service_type
                              << " to stopped");
      service_type == ServiceType::kMobileNav ? it->second.first = false
                                              : it->second.second = false;
    }
    // Fix: For wifi Secondary
    // undisposed data active the VPMService restart again,
    // because Not set Allowstream flag
    ApplicationSharedPtr app = application(app_id);
    if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
      SDL_LOG_ERROR("Navi/Projection application not found");
      return;
    }
    if (service_type == ServiceType::kMobileNav) {
      app->set_video_streaming_allowed(false);
    }
    if (service_type == ServiceType::kAudio) {
      app->set_audio_streaming_allowed(false);
    }
    //  push_back for judge in ForbidStreaming(),
    StartEndStreamTimer(app_id);
  }

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    SDL_LOG_WARN("An application is not registered.");
    return;
  }

  app->StopStreaming(service_type);
}

void ApplicationManagerImpl::OnServiceStartedCallback(
    const connection_handler::DeviceHandle& device_handle,
    const int32_t& session_key,
    const protocol_handler::ServiceType& type,
    const BsonObject* params) {
  using namespace helpers;
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("ServiceType = " << type << ". Session = " << std::hex
                                 << session_key);
  std::vector<std::string> empty;
  std::string reason;

  if (kRpc == type) {
    SDL_LOG_DEBUG("RPC service is about to be started.");
    connection_handler().NotifyServiceStartedResult(
        session_key, true, empty, reason);
    return;
  }
  ApplicationSharedPtr app = application(session_key);
  if (!app) {
    SDL_LOG_WARN("The application with id:" << session_key
                                            << " doesn't exists.");
    return;
  }

  if (Compare<ServiceType, EQ, ONE>(
          type, ServiceType::kMobileNav, ServiceType::kAudio)) {
    if (app->is_navi() || app->mobile_projection_enabled()) {
      if (!StartNaviService(session_key, type, params)) {
        SDL_LOG_WARN("Starting Navigation service failed");
      }
      return;
    } else {
      SDL_LOG_WARN("Refuse not navi/projection application");
      reason = "Service type: " + std::to_string(type) +
               " disallowed with current HMI type";
    }
  } else {
    SDL_LOG_WARN("Refuse unknown service");
  }

  connection_handler().NotifyServiceStartedResult(
      session_key, false, empty, reason);
}

void ApplicationManagerImpl::OnServiceEndedCallback(
    const int32_t& session_key,
    const protocol_handler::ServiceType& type,
    const connection_handler::CloseSessionReason& close_reason) {
  using namespace helpers;
  using namespace protocol_handler;
  using namespace connection_handler;
  using namespace mobile_apis;

  SDL_LOG_DEBUG("OnServiceEndedCallback for service "
                << type << " with reason " << close_reason << " in session 0x"
                << std::hex << session_key);

  auto app = application(static_cast<uint32_t>(session_key));
  if (!app) {
    return;
  }

  if (IsAppInReconnectMode(app->device(), app->policy_app_id())) {
    SDL_LOG_DEBUG("Application is in reconnection list and won't be closed.");
    return;
  }

  if (type == kRpc) {
    SDL_LOG_INFO("Remove application.");
    /* In case it was unexpected disconnect or some special case
     (malformed message, flood) application will be removed
     and we will unregister application correctly, but in case it was
     closed by mobile and already unregistered we will be unable
     to find it in the list
    */

    Result::eType reason;
    bool is_resuming;
    bool is_unexpected_disconnect;
    switch (close_reason) {
      case CloseSessionReason::kFlood: {
        reason = Result::TOO_MANY_PENDING_REQUESTS;
        is_resuming = true;
        is_unexpected_disconnect = false;

        rpc_service_->ManageMobileCommand(
            MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
                session_key, AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS),
            commands::Command::SOURCE_SDL);
        break;
      }
      case CloseSessionReason::kMalformed: {
        reason = Result::INVALID_ENUM;
        is_resuming = false;
        is_unexpected_disconnect = false;
        break;
      }
      case CloseSessionReason::kUnauthorizedApp: {
        reason = Result::INVALID_ENUM;
        is_resuming = true;
        is_unexpected_disconnect = false;
        break;
      }
      case CloseSessionReason::kFinalMessage: {
        reason = Result::SUCCESS;
        is_resuming = false;
        is_unexpected_disconnect = false;
        break;
      }
      default: {
        reason = Result::INVALID_ENUM;
        is_resuming = true;
        is_unexpected_disconnect = true;
        break;
      }
    }
    UnregisterApplication(
        session_key, reason, is_resuming, is_unexpected_disconnect);
    return;
  }

  if (Compare<ServiceType, EQ, ONE>(
          type, ServiceType::kMobileNav, ServiceType::kAudio)) {
    StopNaviService(session_key, type);
  }
}

void ApplicationManagerImpl::ProcessServiceStatusUpdate(
    const uint32_t connection_key,
    hmi_apis::Common_ServiceType::eType service_type,
    hmi_apis::Common_ServiceEvent::eType service_event,
    utils::Optional<hmi_apis::Common_ServiceStatusUpdateReason::eType>
        service_update_reason) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_DEBUG("Processing status update with connection key: "
                << connection_key << " service type: " << service_type
                << " service_event " << service_event
                << " service_update_reason " << service_update_reason);

  const auto app = application(connection_key);

  const uint32_t app_id = app ? app->app_id() : 0u;

  auto reason = service_update_reason
                    ? *service_update_reason
                    : hmi_apis::Common_ServiceStatusUpdateReason::INVALID_ENUM;

  auto notification = MessageHelper::CreateOnServiceUpdateNotification(
      service_type, service_event, reason, app_id);

  rpc_service_->ManageHMICommand(notification);

  if (hmi_apis::Common_ServiceEvent::REQUEST_REJECTED == service_event &&
      HandleRejectedServiceStatus(app, service_type)) {
    state_ctrl_.SetRegularState(app,
                                mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
                                mobile_apis::HMILevel::HMI_NONE,
                                true);
  }
}

bool ApplicationManagerImpl::HandleRejectedServiceStatus(
    ApplicationSharedPtr app,
    const hmi_apis::Common_ServiceType::eType service_type) {
  SDL_LOG_AUTO_TRACE();
  if (!app) {
    SDL_LOG_WARN("Received invalid app");
    return false;
  }

  switch (service_type) {
    case hmi_apis::Common_ServiceType::VIDEO:
    case hmi_apis::Common_ServiceType::AUDIO: {
      {
        sync_primitives::AutoLock lock(navi_service_status_lock_);
        auto app_services = navi_service_status_.find(app->app_id());
        if (navi_service_status_.end() != app_services) {
          navi_service_status_.erase(app_services);
        }
      }
      SDL_LOG_DEBUG("The start of service"
                    << service_type << " for appID: " << app
                    << " is failed. Service info has been removed");
      return true;
    }
    case hmi_apis::Common_ServiceType::RPC: {
      connection_handler().SendEndService(app->app_id(),
                                          protocol_handler::ServiceType::kRpc);
      SDL_LOG_DEBUG("RPC service for appID: " << app << " is stopped");
      return true;
    }
    default:
      SDL_LOG_DEBUG("Unknown service type: " << service_type);
      return false;
  }
}

void ApplicationManagerImpl::OnSecondaryTransportStartedCallback(
    const connection_handler::DeviceHandle device_handle,
    const int32_t session_key) {
  SDL_LOG_AUTO_TRACE();

  if (device_handle == 0) {
    SDL_LOG_WARN("Invalid device handle passed for secondary transport of app "
                 << session_key);
    return;
  }

  secondary_transport_devices_cache_[session_key] = device_handle;

  {
    sync_primitives::AutoLock auto_lock(applications_list_lock_ptr_);
    ApplicationSharedPtr app = application(session_key);
    if (!app) {
      // It is possible that secondary transport is established prior to
      // RegisterAppInterface request being processed. In this case, we will
      // update the app's information during RegisterApplication().
      SDL_LOG_DEBUG("Application with id: " << session_key << " is not found");
      return;
    }
    app->set_secondary_device(device_handle);
  }

  // notify the event to HMI through BC.UpdateAppList request
  SendUpdateAppList();

  // if resumption has not been enabled, run it now
  resume_controller().RetryResumption(session_key);
}

void ApplicationManagerImpl::OnSecondaryTransportEndedCallback(
    const int32_t session_key) {
  SDL_LOG_AUTO_TRACE();

  DeviceMap::iterator it = secondary_transport_devices_cache_.find(session_key);
  if (it == secondary_transport_devices_cache_.end()) {
    SDL_LOG_WARN(
        "Unknown session_key specified while removing secondary transport: "
        << session_key);
  } else {
    secondary_transport_devices_cache_.erase(it);
  }

  {
    sync_primitives::AutoLock auto_lock(applications_list_lock_ptr_);
    ApplicationSharedPtr app = application(session_key);
    if (!app) {
      SDL_LOG_DEBUG("Application with id: " << session_key << " is not found");
      return;
    }

    connection_handler::DeviceHandle device_handle = app->secondary_device();
    if (device_handle == 0) {
      SDL_LOG_WARN("Secondary transport of app " << session_key
                                                 << " is not found");
      return;
    }

    app->set_secondary_device(0);
  }

  // notify the event to HMI through BC.UpdateAppList request
  SendUpdateAppList();
}

bool ApplicationManagerImpl::CheckAppIsNavi(const uint32_t app_id) const {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application(app_id);
  if (app) {
    return app->is_navi();
  }
  return false;
}

#ifdef ENABLE_SECURITY
bool ApplicationManagerImpl::OnHandshakeDone(
    uint32_t connection_key,
    security_manager::SSLContext::HandshakeResult result) {
  SDL_LOG_AUTO_TRACE();

  using security_manager::SSLContext;
  using namespace helpers;

  ApplicationSharedPtr app = application(connection_key);
  if (!app) {
    SDL_LOG_WARN("Application for connection key: " << connection_key
                                                    << " was not found");
    return false;
  }
  if (Compare<SSLContext::HandshakeResult, EQ, ONE>(
          result,
          SSLContext::Handshake_Result_CertExpired,
          SSLContext::Handshake_Result_CertNotSigned,
          SSLContext::Handshake_Result_AppIDMismatch,
          SSLContext::Handshake_Result_AppNameMismatch,
          SSLContext::Handshake_Result_NotYetValid)) {
    app->usage_report().RecordTLSError();
  }
  return false;
}
#ifdef ENABLE_SECURITY
bool ApplicationManagerImpl::OnPTUFailed() {
  SDL_LOG_AUTO_TRACE();
  return false;
}
#endif  // ENABLE_SECURITY

bool ApplicationManagerImpl::OnGetSystemTimeFailed() {
  SDL_LOG_AUTO_TRACE();
  return false;
}

void ApplicationManagerImpl::OnCertificateUpdateRequired() {
  SDL_LOG_AUTO_TRACE();
  GetPolicyHandler().OnPTExchangeNeeded();
}

bool ApplicationManagerImpl::GetPolicyCertificateData(std::string& data) const {
  SDL_LOG_AUTO_TRACE();
  data = GetPolicyHandler().RetrieveCertificate();
  return true;
}

security_manager::SSLContext::HandshakeContext
ApplicationManagerImpl::GetHandshakeContext(uint32_t key) const {
  SDL_LOG_AUTO_TRACE();
  using security_manager::SSLContext;
  ApplicationConstSharedPtr app = application(key);
  if (app) {
    return SSLContext::HandshakeContext(
        custom_str::CustomString(app->policy_app_id()), app->name());
  }
  return SSLContext::HandshakeContext();
}
#endif  // ENABLE_SECURITY

void ApplicationManagerImpl::set_hmi_message_handler(
    hmi_message_handler::HMIMessageHandler* handler) {
  hmi_handler_ = handler;
  rpc_service_->set_hmi_message_handler(handler);
}

void ApplicationManagerImpl::set_connection_handler(
    connection_handler::ConnectionHandler* handler) {
  connection_handler_ = handler;
}

connection_handler::ConnectionHandler&
ApplicationManagerImpl::connection_handler() const {
  return *connection_handler_;
}

protocol_handler::ProtocolHandler& ApplicationManagerImpl::protocol_handler()
    const {
  return *protocol_handler_;
}

void ApplicationManagerImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler* handler) {
  protocol_handler_ = handler;
  rpc_service_->set_protocol_handler(handler);
}

void ApplicationManagerImpl::StartDevicesDiscovery() {
  connection_handler().get_device_discovery_starter().StartDevicesDiscovery();
}

void ApplicationManagerImpl::TerminateRequest(const uint32_t connection_key,
                                              const uint32_t corr_id,
                                              const int32_t function_id) {
  request_ctrl_.TerminateRequest(corr_id, connection_key, function_id, true);
}

void ApplicationManagerImpl::RemoveHMIFakeParameters(
    application_manager::commands::MessageSharedPtr& message,
    const hmi_apis::FunctionID::eType& function_id) {
  SDL_LOG_AUTO_TRACE();
  hmi_apis::HMI_API factory;
  if (!(*message)[jhs::S_PARAMS].keyExists(jhs::S_FUNCTION_ID)) {
    SDL_LOG_ERROR("RemoveHMIFakeParameters message missing function id");
    return;
  }
  mobile_apis::FunctionID::eType mobile_function_id =
      static_cast<mobile_apis::FunctionID::eType>(
          (*message)[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
  (*message)[jhs::S_PARAMS][jhs::S_FUNCTION_ID] = function_id;
  factory.attachSchema(*message, true);
  (*message)[jhs::S_PARAMS][jhs::S_FUNCTION_ID] = mobile_function_id;
}

bool ApplicationManagerImpl::Init(resumption::LastState&,
                                  media_manager::MediaManager*) {
  return false;
}

bool ApplicationManagerImpl::Init(
    resumption::LastStateWrapperPtr last_state_wrapper,
    media_manager::MediaManager* media_manager) {
  SDL_LOG_TRACE("Init application manager");
  plugin_manager_.reset(
      new plugin_manager::RPCPluginManagerImpl(*this,
                                               *rpc_service_,
                                               *hmi_capabilities_,
                                               *policy_handler_,
                                               last_state_wrapper));
  if (!plugin_manager_->LoadPlugins(get_settings().plugins_folder())) {
    SDL_LOG_ERROR("Plugins are not loaded");
    return false;
  }
  const std::string app_storage_folder = get_settings().app_storage_folder();
  if (!InitDirectory(app_storage_folder, TYPE_STORAGE) ||
      !IsReadWriteAllowed(app_storage_folder, TYPE_STORAGE)) {
    return false;
  }
  if (!resume_controller().Init(last_state_wrapper)) {
    SDL_LOG_ERROR("Problem with initialization of resume controller");
    return false;
  }
  hmi_capabilities_->Init(last_state_wrapper);

  if (!(file_system::IsWritingAllowed(app_storage_folder) &&
        file_system::IsReadingAllowed(app_storage_folder))) {
    SDL_LOG_ERROR("Storage directory doesn't have read/write permissions");
    return false;
  }

  const std::string system_files_path = get_settings().system_files_path();
  if (!InitDirectory(system_files_path, TYPE_SYSTEM) ||
      !IsReadWriteAllowed(system_files_path, TYPE_SYSTEM)) {
    return false;
  }
  const std::string app_icons_folder = get_settings().app_icons_folder();
  if (!InitDirectory(app_icons_folder, TYPE_ICONS)) {
    return false;
  }
  // In case there is no R/W permissions for this location, SDL just has to
  // log this and proceed
  IsReadWriteAllowed(app_icons_folder, TYPE_ICONS);
  if (GetPolicyHandler().PolicyEnabled()) {
    if (!GetPolicyHandler().LoadPolicyLibrary()) {
      SDL_LOG_ERROR("Policy library is not loaded. Check LD_LIBRARY_PATH");
      return false;
    }
    SDL_LOG_INFO("Policy library is loaded, now initing PT");
    if (!GetPolicyHandler().InitPolicyTable()) {
      SDL_LOG_ERROR("Policy table is not initialized.");
      return false;
    }
  } else {
    SDL_LOG_WARN("System is configured to work without policy functionality.");
  }
  media_manager_ = media_manager;

  if (settings_.use_db_for_resumption()) {
    app_launch_dto_.reset(new app_launch::AppLaunchDataDB(settings_));
  } else {
    app_launch_dto_.reset(
        new app_launch::AppLaunchDataJson(settings_, last_state_wrapper));
  }
  app_launch_ctrl_.reset(new app_launch::AppLaunchCtrlImpl(
      *app_launch_dto_.get(), *this, settings_));

  app_service_manager_.reset(
      new application_manager::AppServiceManager(*this, last_state_wrapper));

  auto on_app_policy_updated = [](plugin_manager::RPCPlugin& plugin) {
    plugin.OnPolicyEvent(plugin_manager::kApplicationPolicyUpdated);
  };

  plugin_manager_->ForEachPlugin(on_app_policy_updated);

  return true;
}

bool ApplicationManagerImpl::Stop() {
  SDL_LOG_AUTO_TRACE();
  is_stopping_.store(true);
  application_list_update_timer_.Stop();
  try {
    if (unregister_reason_ ==
        mobile_api::AppInterfaceUnregisteredReason::INVALID_ENUM) {
      SetUnregisterAllApplicationsReason(
          mobile_api::AppInterfaceUnregisteredReason::IGNITION_OFF);
    }

    UnregisterAllApplications();
  } catch (...) {
    SDL_LOG_ERROR("An error occurred during unregistering applications.");
  }
  request_ctrl_.DestroyThreadpool();

  // for PASA customer policy backup should happen :AllApp(SUSPEND)
  SDL_LOG_DEBUG("Unloading policy library.");
  GetPolicyHandler().UnloadPolicyLibrary();

  rpc_service_->Stop();

  return true;
}

bool ApplicationManagerImpl::ConvertSOtoMessage(
    const smart_objects::SmartObject& message,
    Message& output,
    const bool allow_unknown_parameters) {
  SDL_LOG_AUTO_TRACE();

  if (smart_objects::SmartType_Null == message.getType() ||
      smart_objects::SmartType_Invalid == message.getType()) {
    SDL_LOG_WARN("Invalid smart object received.");
    return false;
  }

  SDL_LOG_DEBUG(
      "Message with protocol: " << message.getElement(jhs::S_PARAMS)
                                       .getElement(jhs::S_PROTOCOL_TYPE)
                                       .asInt());

  std::string output_string;
  const int64_t protocol_type = message.getElement(jhs::S_PARAMS)
                                    .getElement(jhs::S_PROTOCOL_TYPE)
                                    .asInt();
  const int64_t protocol_version = message.getElement(jhs::S_PARAMS)
                                       .getElement(jhs::S_PROTOCOL_VERSION)
                                       .asInt();
  switch (protocol_type) {
    case 0: {
      if (protocol_version == 1) {
        if (!formatters::CFormatterJsonSDLRPCv1::toString(
                message, output_string, !allow_unknown_parameters)) {
          SDL_LOG_WARN("Failed to serialize smart object");
          return false;
        }
        output.set_protocol_version(
            protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1);
      } else {
        if (!formatters::CFormatterJsonSDLRPCv2::toString(
                message, output_string, !allow_unknown_parameters)) {
          SDL_LOG_WARN("Failed to serialize smart object");
          return false;
        }
        output.set_protocol_version(
            static_cast<protocol_handler::MajorProtocolVersion>(
                protocol_version));
      }

      break;
    }
    case 1: {
      if (!formatters::FormatterJsonRpc::ToString(
              message, output_string, !allow_unknown_parameters)) {
        SDL_LOG_WARN("Failed to serialize smart object");
        return false;
      }
      output.set_protocol_version(
          protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_HMI);
      break;
    }
    default:
      NOTREACHED();
      return false;
  }

  SDL_LOG_DEBUG("Convertion result: " << output_string);

  output.set_connection_key(message.getElement(jhs::S_PARAMS)
                                .getElement(strings::connection_key)
                                .asInt());

  output.set_function_id(
      message.getElement(jhs::S_PARAMS).getElement(jhs::S_FUNCTION_ID).asInt());

  output.set_correlation_id(message.getElement(jhs::S_PARAMS)
                                .getElement(jhs::S_CORRELATION_ID)
                                .asInt());
  output.set_message_type(
      static_cast<MessageType>(message.getElement(jhs::S_PARAMS)
                                   .getElement(jhs::S_MESSAGE_TYPE)
                                   .asInt()));

  // Currently formatter creates JSON = 3 bytes for empty SmartObject.
  // workaround for notification. JSON must be empty
  if (mobile_apis::FunctionID::OnAudioPassThruID !=
      message.getElement(jhs::S_PARAMS)
          .getElement(strings::function_id)
          .asInt()) {
    output.set_json_message(output_string);
  }

  if (message.getElement(jhs::S_PARAMS).keyExists(strings::binary_data)) {
    const application_manager::BinaryData binaryData(
        message.getElement(jhs::S_PARAMS)
            .getElement(strings::binary_data)
            .asBinary());

    output.set_binary_data(&binaryData);
  }

  SDL_LOG_DEBUG("Successfully parsed smart object into message");
  return true;
}

hmi_apis::HMI_API& ApplicationManagerImpl::hmi_so_factory() {
  return hmi_so_factory_;
}

mobile_apis::MOBILE_API& ApplicationManagerImpl::mobile_so_factory() {
  return mobile_so_factory_;
}

ns_smart_device_link_rpc::V1::v4_protocol_v1_2_no_extra&
ApplicationManagerImpl::mobile_v4_protocol_so_factory() {
  return mobile_v4_protocol_so_factory_;
}

HMICapabilities& ApplicationManagerImpl::hmi_capabilities() {
  return *hmi_capabilities_;
}

const HMICapabilities& ApplicationManagerImpl::hmi_capabilities() const {
  return *hmi_capabilities_;
}

void ApplicationManagerImpl::PullLanguagesInfo(const SmartObject& app_data,
                                               SmartObject& ttsName,
                                               SmartObject& vrSynonym) {
  SDL_LOG_AUTO_TRACE();
  if (!app_data.keyExists(json::languages)) {
    SDL_LOG_WARN("\"languages\" not exists");
    return;
  }

  const HMICapabilities& hmi_cap = hmi_capabilities();
  std::string cur_vr_lang(EnumToString(hmi_cap.active_vr_language()));
  const SmartObject& languages = app_data[json::languages];

  std::transform(
      cur_vr_lang.begin(), cur_vr_lang.end(), cur_vr_lang.begin(), ::toupper);

  ssize_t default_idx = -1;
  ssize_t specific_idx = -1;

  const size_t size = languages.length();
  for (size_t idx = 0; idx < size; ++idx) {
    if (languages[idx].keyExists(cur_vr_lang)) {
      SDL_LOG_DEBUG("Found active HMI language " << cur_vr_lang);
      specific_idx = idx;
    } else if (languages[idx].keyExists(json::default_)) {
      SDL_LOG_DEBUG("Found default language");
      default_idx = idx;
    }
  }

  if ((-1 == specific_idx) && (-1 == default_idx)) {
    SDL_LOG_DEBUG("No suitable language found");
    return;
  }

  if (app_data[json::languages][specific_idx][cur_vr_lang].keyExists(
          json::ttsName)) {
    SDL_LOG_DEBUG("Get ttsName from " << cur_vr_lang << " language");
    ttsName =
        app_data[json::languages][specific_idx][cur_vr_lang][json::ttsName];
  } else {
    SDL_LOG_DEBUG("No data for ttsName for " << cur_vr_lang << " language");
  }

  if (app_data[json::languages][specific_idx][cur_vr_lang].keyExists(
          json::vrSynonyms)) {
    SDL_LOG_DEBUG("Get vrSynonyms from " << cur_vr_lang << " language");
    vrSynonym =
        app_data[json::languages][specific_idx][cur_vr_lang][json::vrSynonyms];
  } else {
    SDL_LOG_DEBUG("No data for vrSynonyms for " << cur_vr_lang << " language");
  }
}

void ApplicationManagerImpl::CreateApplications(SmartArray& obj_array,
                                                const uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  using namespace policy;

  const std::size_t arr_size(obj_array.size());
  for (std::size_t idx = 0; idx < arr_size; ++idx) {
    const SmartObject& app_data = obj_array[idx];

    if (!(app_data.keyExists(json::name) && app_data.keyExists(json::appId))) {
      SDL_LOG_DEBUG("The entry in query apps json is not valid");
      continue;
    }

    const std::string policy_app_id(app_data[json::appId].asString());

    connection_handler::DeviceHandle device_handle;
    if (-1 == connection_handler().get_session_observer().GetDataOnSessionKey(
                  connection_key, nullptr, nullptr, &device_handle)) {
      SDL_LOG_ERROR("Failed to create application: no connection info.");
      continue;
    }

    std::string device_id;
    if (-1 == connection_handler().get_session_observer().GetDataOnDeviceID(
                  device_handle, nullptr, nullptr, &device_id)) {
      SDL_LOG_ERROR("Failed to create application: no connection info.");
      continue;
    }

    ApplicationSharedPtr registered_app = application(device_id, policy_app_id);
    if (registered_app) {
      SDL_LOG_DEBUG("Application with the same id: "
                    << policy_app_id << " is registered already.");
      continue;
    }

    std::string url_scheme;
    std::string package_name;
    std::string os_type;
    SmartObject vrSynonym;
    SmartObject ttsName;

    const custom_str::CustomString appName(
        app_data[json::name].asCustomString());

    if (app_data.keyExists(json::ios)) {
      os_type = json::ios;
      url_scheme = app_data[os_type][json::urlScheme].asString();
    } else if (app_data.keyExists(json::android)) {
      os_type = json::android;
      package_name = app_data[os_type][json::packageName].asString();
    }

    PullLanguagesInfo(app_data[os_type], ttsName, vrSynonym);

    if (ttsName.empty()) {
      ttsName = SmartObject(SmartType_Array);
      ttsName[0] = appName;
    }
    if (vrSynonym.empty()) {
      vrSynonym = SmartObject(SmartType_Array);
      vrSynonym[0] = appName;
    }

    const std::string app_icon_dir(settings_.app_icons_folder());
    const std::string full_icon_path(app_icon_dir + "/" + policy_app_id);

    const uint32_t hmi_app_id =
        resume_controller().IsApplicationSaved(policy_app_id, device_id)
            ? resume_controller().GetHMIApplicationID(policy_app_id, device_id)
            : GenerateNewHMIAppID();

    // AppId = 0 because this is query_app(provided by hmi for download, but not
    // yet registered)
    ApplicationSharedPtr app(
        new ApplicationImpl(0,
                            policy_app_id,
                            device_id,
                            device_handle,
                            appName,
                            GetPolicyHandler().GetStatisticManager(),
                            *this));
    DCHECK_OR_RETURN_VOID(app);
    app->SetShemaUrl(url_scheme);
    app->SetPackageName(package_name);
    app->set_app_icon_path(full_icon_path);
    app->set_hmi_application_id(hmi_app_id);

    app->set_vr_synonyms(vrSynonym);
    app->set_tts_name(ttsName);

    sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
    SDL_LOG_DEBUG(
        "apps_to_register_ size before: " << apps_to_register_.size());
    apps_to_register_.insert(app);
    SDL_LOG_DEBUG("apps_to_register_ size after: " << apps_to_register_.size());
  }
}

void ApplicationManagerImpl::ProcessQueryApp(
    const smart_objects::SmartObject& sm_object,
    const uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  using namespace policy;

  if (!sm_object.keyExists(json::response)) {
    SDL_LOG_DEBUG("The response key is not exists.");
    return;
  }

  SmartArray* obj_array = sm_object[json::response].asArray();
  if (NULL != obj_array) {
    CreateApplications(*obj_array, connection_key);
    SendUpdateAppList();

    sync_primitives::AutoLock lock(apps_to_register_list_lock_ptr_);
    AppsWaitRegistrationSet::const_iterator it = apps_to_register_.begin();
    for (; it != apps_to_register_.end(); ++it) {
      const std::string full_icon_path((*it)->app_icon_path());
      if (file_system::FileExists(full_icon_path)) {
        MessageHelper::SendSetAppIcon(
            (*it)->hmi_app_id(), full_icon_path, *this);
      }
    }
  }
}

bool ApplicationManagerImpl::is_attenuated_supported() const {
  return hmi_capabilities().attenuated_supported() &&
         get_settings().is_mixing_audio_supported();
}

#ifdef TELEMETRY_MONITOR
void ApplicationManagerImpl::SetTelemetryObserver(
    AMTelemetryObserver* observer) {
  rpc_handler_->SetTelemetryObserver(observer);
}
#endif  // TELEMETRY_MONITOR

void ApplicationManagerImpl::addNotification(const CommandSharedPtr ptr) {
  request_ctrl_.addNotification(ptr);
}

void ApplicationManagerImpl::removeNotification(
    const commands::Command* notification) {
  request_ctrl_.removeNotification(notification);
}

void ApplicationManagerImpl::updateRequestTimeout(
    uint32_t connection_key,
    uint32_t mobile_correlation_id,
    uint32_t new_timeout_value) {
  SDL_LOG_AUTO_TRACE();
  request_ctrl_.updateRequestTimeout(
      connection_key, mobile_correlation_id, new_timeout_value);
}

void ApplicationManagerImpl::IncreaseForwardedRequestTimeout(
    uint32_t connection_key, uint32_t mobile_correlation_id) {
  SDL_LOG_DEBUG("Increasing Request Timeout by "
                << get_settings().rpc_pass_through_timeout());
  uint32_t new_timeout_value = get_settings().default_timeout() +
                               get_settings().rpc_pass_through_timeout();
  request_ctrl_.updateRequestTimeout(
      connection_key, mobile_correlation_id, new_timeout_value);
}

uint32_t ApplicationManagerImpl::application_id(const int32_t correlation_id) {
  // ykazakov: there is no erase for const iterator for QNX
  std::map<const int32_t, const uint32_t>::iterator it =
      appID_list_.find(correlation_id);
  if (appID_list_.end() != it) {
    const uint32_t app_id = it->second;
    appID_list_.erase(it);
    return app_id;
  } else {
    return 0;
  }
}

void ApplicationManagerImpl::set_application_id(const int32_t correlation_id,
                                                const uint32_t app_id) {
  appID_list_.insert(
      std::pair<const int32_t, const uint32_t>(correlation_id, app_id));
}

uint32_t ApplicationManagerImpl::get_current_audio_source() const {
  return current_audio_source_;
}

void ApplicationManagerImpl::set_current_audio_source(const uint32_t source) {
  current_audio_source_ = source;
}

void ApplicationManagerImpl::AddPolicyObserver(
    policy::PolicyHandlerObserver* listener) {
  GetPolicyHandler().add_listener(listener);
}

void ApplicationManagerImpl::RemovePolicyObserver(
    policy::PolicyHandlerObserver* listener) {
  GetPolicyHandler().remove_listener(listener);
}

void ApplicationManagerImpl::SetUnregisterAllApplicationsReason(
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_TRACE("reason = " << reason);
  unregister_reason_ = reason;
}

void ApplicationManagerImpl::HeadUnitReset(
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  SDL_LOG_AUTO_TRACE();
  is_stopping_.store(true);
  switch (reason) {
    case mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET: {
      SDL_LOG_TRACE("Performing MASTER_RESET");
      UnregisterAllApplications();
      GetPolicyHandler().ResetPolicyTable();
      GetPolicyHandler().UnloadPolicyLibrary();

      resume_controller().StopSavePersistentDataTimer();
      if (!hmi_capabilities_->DeleteCachedCapabilitiesFile()) {
        SDL_LOG_ERROR("Failed to remove HMI capabilities cache file");
      }
      const std::string storage_folder = get_settings().app_storage_folder();
      file_system::RemoveDirectory(storage_folder, true);
      ClearAppsPersistentData();
      break;
    }
    case mobile_api::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS: {
      SDL_LOG_TRACE("Performing FACTORY_DEFAULTS");
      GetPolicyHandler().ClearUserConsent();

      resume_controller().StopSavePersistentDataTimer();
      if (!hmi_capabilities_->DeleteCachedCapabilitiesFile()) {
        SDL_LOG_ERROR("Failed to remove HMI capabilities cache file");
      }
      ClearAppsPersistentData();
      break;
    }
    default: {
      SDL_LOG_ERROR("Bad AppInterfaceUnregisteredReason");
      return;
    }
  }
}

void ApplicationManagerImpl::ClearAppsPersistentData() {
  SDL_LOG_AUTO_TRACE();
  typedef std::vector<std::string> FilesList;
  const std::string apps_info_storage_file = get_settings().app_info_storage();
  file_system::DeleteFile(apps_info_storage_file);

  const std::string storage_folder = get_settings().app_storage_folder();

  FilesList files = file_system::ListFiles(storage_folder);
  FilesList::iterator element_to_skip =
      std::find(files.begin(), files.end(), "policy.sqlite");
  if (element_to_skip != files.end()) {
    files.erase(element_to_skip);
  }

  FilesList::iterator it = files.begin();
  for (; it != files.end(); ++it) {
    const std::string path_to_item = storage_folder + "/";
    const std::string item_to_remove = path_to_item + (*it);
    SDL_LOG_TRACE("Removing : " << item_to_remove);
    if (file_system::IsDirectory(item_to_remove)) {
      SDL_LOG_TRACE("Removal result : " << file_system::RemoveDirectory(
                        item_to_remove, true));
    } else {
      SDL_LOG_TRACE(
          "Removal result : " << file_system::DeleteFile(item_to_remove));
    }
  }

  const std::string apps_icons_folder = get_settings().app_icons_folder();
  if (storage_folder != apps_icons_folder) {
    file_system::RemoveDirectory(apps_icons_folder, true);
  }
}

void ApplicationManagerImpl::SendOnSDLClose() {
  SDL_LOG_AUTO_TRACE();
  if (IsLowVoltage()) {
    SDL_LOG_TRACE("SDL is in Low Voltage State");
    return;
  }
  // must be sent to PASA HMI on shutdown synchronously
  smart_objects::SmartObjectSPtr msg =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  (*msg)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSDLClose;
  (*msg)[strings::params][strings::message_type] = MessageType::kNotification;
  (*msg)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
  (*msg)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;

  if (!msg) {
    SDL_LOG_WARN("Null-pointer message received.");
    NOTREACHED();
    return;
  }

  // SmartObject |message| has no way to declare priority for now
  std::shared_ptr<Message> message_to_send(
      new Message(protocol_handler::MessagePriority::kDefault));

  hmi_so_factory().attachSchema(*msg, false);
  SDL_LOG_DEBUG(
      "Attached schema to message, result if valid: " << msg->isValid());

  if (!ConvertSOtoMessage(*msg, *message_to_send)) {
    SDL_LOG_WARN("Cannot send message to HMI: failed to create string");
    return;
  }

  if (!hmi_handler_) {
    SDL_LOG_WARN("No HMI Handler set");
    return;
  }

  hmi_handler_->SendMessageToHMI(message_to_send);
}

void ApplicationManagerImpl::UnregisterAllApplications() {
  SDL_LOG_DEBUG("Unregister reason  " << unregister_reason_);

  SetHMICooperating(false);

  using namespace mobile_api::AppInterfaceUnregisteredReason;
  using namespace helpers;

  bool is_ignition_off =
      Compare<eType, EQ, ONE>(unregister_reason_, IGNITION_OFF, INVALID_ENUM);

  bool is_unexpected_disconnect = Compare<eType, NEQ, ALL>(
      unregister_reason_, IGNITION_OFF, MASTER_RESET, FACTORY_DEFAULTS);

  ClearTTSGlobalPropertiesList();

  {  // A local scope to limit accessor's lifetime and release app list lock.
    DataAccessor<ApplicationSet> accessor = applications();
    ApplicationSetConstIt it = accessor.GetData().begin();
    while (it != accessor.GetData().end()) {
      ApplicationSharedPtr app_to_remove = *it;
      rpc_service_->ManageMobileCommand(
          MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
              app_to_remove->app_id(), unregister_reason_),
          commands::Command::SOURCE_SDL);
      UnregisterApplication(app_to_remove->app_id(),
                            mobile_apis::Result::INVALID_ENUM,
                            is_ignition_off,
                            is_unexpected_disconnect);
      connection_handler().CloseSession(app_to_remove->app_id(),
                                        connection_handler::kCommon);
      it = accessor.GetData().begin();
    }
  }

  bool send_pending_update_app_list = false;
  {
    sync_primitives::AutoLock auto_lock(apps_to_register_list_lock_ptr_);
    if (!apps_to_register_.empty()) {
      send_pending_update_app_list = true;
      apps_to_register_.clear();
    }
  }

  // Only send update app list if pending apps were removed.
  if (send_pending_update_app_list) {
    SendUpdateAppList();
  }

  if (is_ignition_off) {
    resume_controller().OnIgnitionOff();
  }
  request_ctrl_.terminateAllHMIRequests();
}

void ApplicationManagerImpl::RemoveAppsWaitingForRegistration(
    const connection_handler::DeviceHandle handle) {
  SDL_LOG_AUTO_TRACE();
  DevicePredicate device_finder(handle);
  apps_to_register_list_lock_ptr_->Acquire();
  std::vector<ApplicationSharedPtr> apps_to_remove;
  std::copy_if(apps_to_register_.begin(),
               apps_to_register_.end(),
               std::back_inserter(apps_to_remove),
               device_finder);

  const auto enabled_local_apps = policy_handler_->GetEnabledLocalApps();
  for (auto app : apps_to_remove) {
    const bool is_app_enabled =
        helpers::in_range(enabled_local_apps, app->policy_app_id());
    if (!is_app_enabled) {
      SDL_LOG_DEBUG("Waiting app: " << app->name().c_str() << " is removed.");
      apps_to_register_.erase(app);
    }
  }

  apps_to_register_list_lock_ptr_->Release();
}

void ApplicationManagerImpl::UnregisterApplication(
    const uint32_t& app_id,
    mobile_apis::Result::eType reason,
    bool is_resuming,
    bool is_unexpected_disconnect) {
  SDL_LOG_DEBUG("app_id = " << app_id << "; reason = " << reason
                            << "; is_resuming = " << is_resuming
                            << "; is_unexpected_disconnect = "
                            << is_unexpected_disconnect);

  GetAppServiceManager().UnpublishServices(app_id);

  EndNaviServices(app_id);

  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() != it) {
      navi_service_status_.erase(it);
    }
  }

  // remove appID from tts_global_properties_app_list_
  RemoveAppFromTTSGlobalPropertiesList(app_id);

  switch (reason) {
    case mobile_apis::Result::SUCCESS:
      break;
    case mobile_apis::Result::DISALLOWED:
      break;
    case mobile_apis::Result::USER_DISALLOWED:
      break;
    case mobile_apis::Result::INVALID_CERT:
      break;
    case mobile_apis::Result::EXPIRED_CERT:
      break;
    case mobile_apis::Result::TOO_MANY_PENDING_REQUESTS: {
      ApplicationSharedPtr app_ptr = application(app_id);
      if (app_ptr) {
        app_ptr->usage_report().RecordRemovalsForBadBehavior();
        if (reason == mobile_apis::Result::TOO_MANY_PENDING_REQUESTS) {
          SDL_LOG_DEBUG(
              "INSERT: " << GetHashedAppID(app_id, app_ptr->policy_app_id()));
          forbidden_applications.insert(
              GetHashedAppID(app_id, app_ptr->policy_app_id()));
        }
      }
      break;
    }
    default: {
      SDL_LOG_ERROR("Unknown unregister reason " << reason);
      break;
    }
  }
  ApplicationSharedPtr app_to_remove;
  connection_handler::DeviceHandle handle = 0;
  {
    sync_primitives::AutoLock lock(applications_list_lock_ptr_);
    auto it_app = applications_.begin();
    while (applications_.end() != it_app) {
      if (app_id == (*it_app)->app_id()) {
        app_to_remove = *it_app;
        applications_.erase(it_app++);
        break;
      } else {
        ++it_app;
      }
    }
    if (!app_to_remove) {
      SDL_LOG_ERROR("Cant find application with app_id = " << app_id);

      // Just to terminate RAI in case of connection is dropped (rare case)
      // App won't be unregistered since RAI has not been started yet
      SDL_LOG_DEBUG("Trying to terminate possible RAI request.");
      request_ctrl_.terminateAppRequests(app_id);

      return;
    }

    if (is_resuming) {
      resume_controller().SaveApplication(app_to_remove);
    } else {
      resume_controller().RemoveApplicationFromSaved(app_to_remove);
    }

    if (IsAppSubscribedForWayPoints(app_id)) {
      UnsubscribeAppFromWayPoints(app_id, true);
      if (!IsAnyAppSubscribedForWayPoints()) {
        SDL_LOG_DEBUG("Send UnsubscribeWayPoints");
        auto request = MessageHelper::CreateUnsubscribeWayPointsRequest(
            GetNextHMICorrelationID());
        rpc_service_->ManageHMICommand(request);
      }
    }

    (hmi_capabilities_->get_hmi_language_handler())
        .OnUnregisterApplication(app_id);

    if (connection_handler().GetDeviceID(app_to_remove->mac_address(),
                                         &handle)) {
      AppV4DevicePredicate finder(handle);
      ApplicationSharedPtr app = FindApp(applications(), finder);
      if (!app) {
        SDL_LOG_DEBUG(
            "There is no more SDL4 apps with device handle: " << handle);

        RemoveAppsWaitingForRegistration(handle);
      }
    }

    MessageHelper::SendOnAppUnregNotificationToHMI(
        app_to_remove, is_unexpected_disconnect, *this);
    commands_holder_->Clear(app_to_remove);

    const auto enabled_local_apps = policy_handler_->GetEnabledLocalApps();
    if (helpers::in_range(enabled_local_apps, app_to_remove->policy_app_id())) {
      SDL_LOG_DEBUG(
          "Enabled local app has been unregistered. Re-create "
          "pending application");
      CreatePendingLocalApplication(app_to_remove->policy_app_id());
    }

    RefreshCloudAppInformation();
    SendUpdateAppList();
  }

  if (EndAudioPassThru(app_id)) {
    // May be better to put this code in MessageHelper?
    StopAudioPassThru(app_id);
    MessageHelper::SendStopAudioPathThru(*this);
  }
  auto on_app_unregistered =
      [app_to_remove](plugin_manager::RPCPlugin& plugin) {
        plugin.OnApplicationEvent(plugin_manager::kApplicationUnregistered,
                                  app_to_remove);
      };

  plugin_manager_->ForEachPlugin(on_app_unregistered);
  request_ctrl_.terminateAppRequests(app_id);

  const bool is_applications_list_empty = applications().GetData().empty();
  if (is_applications_list_empty) {
    policy_handler_->StopRetrySequence();
  }
  return;
}

void ApplicationManagerImpl::OnAppUnauthorized(const uint32_t& app_id) {
  connection_handler().CloseSession(app_id,
                                    connection_handler::kUnauthorizedApp);
}

mobile_apis::Result::eType ApplicationManagerImpl::CheckPolicyPermissions(
    const ApplicationSharedPtr app,
    const WindowID window_id,
    const std::string& function_id,
    const RPCParams& rpc_params,
    CommandParametersPermissions* params_permissions) {
  SDL_LOG_AUTO_TRACE();
  // TODO(AOleynik): Remove check of policy_enable, when this flag will be
  // unused in config file
  if (!GetPolicyHandler().PolicyEnabled()) {
    return mobile_apis::Result::SUCCESS;
  }

  DCHECK(app);
  policy::CheckPermissionResult result;
  GetPolicyHandler().CheckPermissions(
      app, window_id, function_id, rpc_params, result);

  if (NULL != params_permissions) {
    params_permissions->allowed_params = result.list_of_allowed_params;
    params_permissions->disallowed_params = result.list_of_disallowed_params;
    params_permissions->undefined_params = result.list_of_undefined_params;
  }

  // Record statistics for default window only
  if (app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW) ==
          mobile_apis::HMILevel::HMI_NONE &&
      function_id != MessageHelper::StringifiedFunctionID(
                         mobile_apis::FunctionID::UnregisterAppInterfaceID)) {
    if (result.hmi_level_permitted != policy::kRpcAllowed) {
      app->usage_report().RecordRpcSentInHMINone();
    }
  }

#ifdef ENABLE_LOG
  const std::string log_msg =
      "Application: " + app->policy_app_id() + ", RPC: " + function_id +
      ", window_id: " + std::to_string(window_id) +
      ", HMI status: " + EnumToString(app->hmi_level(window_id));
#endif  // ENABLE_LOG
  if (result.hmi_level_permitted != policy::kRpcAllowed) {
    SDL_LOG_WARN("Request is blocked by policies. " << log_msg);

    app->usage_report().RecordPolicyRejectedRpcCall();

    switch (result.hmi_level_permitted) {
      case policy::kRpcDisallowed:
        return mobile_apis::Result::DISALLOWED;
      case policy::kRpcUserDisallowed:
        return mobile_apis::Result::USER_DISALLOWED;
      default:
        return mobile_apis::Result::INVALID_ENUM;
    }
  }
  SDL_LOG_DEBUG("Request is allowed by policies. " << log_msg);
  return mobile_api::Result::SUCCESS;
}

bool ApplicationManagerImpl::is_stopping() const {
  return is_stopping_;
}

bool ApplicationManagerImpl::is_audio_pass_thru_active() const {
  return audio_pass_thru_active_;
}

void ApplicationManagerImpl::OnLowVoltage() {
  SDL_LOG_AUTO_TRACE();
  is_low_voltage_ = true;
  resume_ctrl_->SaveLowVoltageTime();
  resume_ctrl_->StopSavePersistentDataTimer();
  request_ctrl_.OnLowVoltage();
}

bool ApplicationManagerImpl::IsLowVoltage() const {
  SDL_LOG_TRACE("Result: " << is_low_voltage_);
  return is_low_voltage_;
}

void ApplicationManagerImpl::OnWakeUp() {
  SDL_LOG_AUTO_TRACE();
  resume_ctrl_->SaveWakeUpTime();
  resume_ctrl_->StartSavePersistentDataTimer();
  request_ctrl_.OnWakeUp();
  is_low_voltage_ = false;
}

std::string ApplicationManagerImpl::GetHashedAppID(
    uint32_t connection_key, const std::string& mobile_app_id) const {
  connection_handler::DeviceHandle device_id = 0;
  connection_handler().get_session_observer().GetDataOnSessionKey(
      connection_key, 0, NULL, &device_id);
  std::string device_name;
  connection_handler().get_session_observer().GetDataOnDeviceID(
      device_id, &device_name, NULL, NULL, NULL);

  return mobile_app_id + device_name;
}

bool ApplicationManagerImpl::HMIStateAllowsStreaming(
    uint32_t app_id, protocol_handler::ServiceType service_type) const {
  SDL_LOG_AUTO_TRACE();
  using namespace mobile_apis::HMILevel;
  using namespace mobile_apis::PredefinedWindows;
  using namespace mobile_apis::VideoStreamingState;
  using namespace helpers;

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    SDL_LOG_WARN("An application is not registered.");
    return false;
  }

  const auto hmi_state = app->CurrentHmiState(DEFAULT_WINDOW);
  const bool allow_streaming_by_hmi_level =
      Compare<mobile_apis::HMILevel::eType, EQ, ONE>(
          hmi_state->hmi_level(), HMI_FULL, HMI_LIMITED);
  const bool allow_streaming_by_streaming_state =
      hmi_state->video_streaming_state() == STREAMABLE;

  return allow_streaming_by_hmi_level && allow_streaming_by_streaming_state;
}

bool ApplicationManagerImpl::CanAppStream(
    uint32_t app_id, protocol_handler::ServiceType service_type) const {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    SDL_LOG_WARN("An application is not registered.");
    return false;
  }

  bool is_allowed = false;
  if (ServiceType::kMobileNav == service_type) {
    is_allowed = app->video_streaming_allowed();
  } else if (ServiceType::kAudio == service_type) {
    is_allowed = app->audio_streaming_allowed();
  } else {
    SDL_LOG_WARN("Unsupported service_type " << service_type);
  }

  return HMIStateAllowsStreaming(app_id, service_type) && is_allowed;
}

void ApplicationManagerImpl::ForbidStreaming(uint32_t app_id) {
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;

  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_DEBUG(
        "There is no navi or projection application with id: " << app_id);
    return;
  }

  {
    sync_primitives::AutoLock lock(navi_app_to_stop_lock_);
    if (navi_app_to_stop_.end() != std::find(navi_app_to_stop_.begin(),
                                             navi_app_to_stop_.end(),
                                             app_id) ||
        navi_app_to_end_stream_.end() !=
            std::find(navi_app_to_end_stream_.begin(),
                      navi_app_to_end_stream_.end(),
                      app_id)) {
      return;
    }
  }

  bool unregister = false;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it ||
        (!it->second.first && !it->second.second)) {
      unregister = true;
    }
  }
  if (unregister) {
    rpc_service_->ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app_id, PROTOCOL_VIOLATION),
        commands::Command::SOURCE_SDL);
    UnregisterApplication(app_id, ABORTED);
    return;
  }
  EndNaviServices(app_id);
}

void ApplicationManagerImpl::ForbidStreaming(
    uint32_t app_id, protocol_handler::ServiceType service_type) {
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;
  using namespace protocol_handler;

  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_DEBUG(
        "There is no navi or projection application with id: " << app_id);
    return;
  }

  {
    sync_primitives::AutoLock lock(navi_app_to_stop_lock_);

    if (navi_app_to_stop_.end() != std::find(navi_app_to_stop_.begin(),
                                             navi_app_to_stop_.end(),
                                             app_id) ||
        navi_app_to_end_stream_.end() !=
            std::find(navi_app_to_end_stream_.begin(),
                      navi_app_to_end_stream_.end(),
                      app_id)) {
      return;
    }
  }

  bool unregister = false;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it ||
        (!it->second.first && !it->second.second)) {
      unregister = true;
    }
  }
  if (unregister) {
    rpc_service_->ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app_id, PROTOCOL_VIOLATION),
        commands::Command::SOURCE_SDL);
    UnregisterApplication(app_id, ABORTED);
    return;
  }

  if (ServiceType::kMobileNav == service_type &&
      app->video_streaming_allowed()) {
    SDL_LOG_DEBUG("Video streaming is still allowed");
    return;
  }

  if (ServiceType::kAudio == service_type && app->audio_streaming_allowed()) {
    SDL_LOG_DEBUG("Audio streaming is still allowed");
    return;
  }

  EndNaviServices(app_id);
}

void ApplicationManagerImpl::OnAppStreaming(
    uint32_t app_id,
    protocol_handler::ServiceType service_type,
    const Application::StreamingState new_state) {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_DEBUG(
        " There is no navi or projection application with id: " << app_id);
    return;
  }
  DCHECK_OR_RETURN_VOID(media_manager_);

  SDL_LOG_DEBUG("New state for service " << static_cast<int32_t>(service_type)
                                         << " is "
                                         << static_cast<int32_t>(new_state));
  switch (new_state) {
    case Application::StreamingState::kStopped: {
      // Stop activity in media_manager_ when service is stopped
      // State controller has been already notified by kSuspended event
      // received before
      media_manager_->StopStreaming(app_id, service_type);
      break;
    }

    case Application::StreamingState::kStarted: {
      // Apply temporary streaming state and start activity in media_manager_
      state_ctrl_.OnVideoStreamingStarted(app);
      media_manager_->StartStreaming(app_id, service_type);
      break;
    }

    case Application::StreamingState::kSuspended: {
      // Don't stop activity in media_manager_ in that case
      // Just cancel the temporary streaming state
      state_ctrl_.OnVideoStreamingStopped(app);
      break;
    }
  }
}

void ApplicationManagerImpl::EndNaviServices(uint32_t app_id) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_DEBUG(
        "There is no navi or projection application with id: " << app_id);
    return;
  }

  bool end_video = false;
  bool end_audio = false;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it) {
      SDL_LOG_ERROR("No info about navi servicies for app");
      return;
    }
    end_video = it->second.first;
    end_audio = it->second.second;
  }

  if (connection_handler_) {
    if (end_video) {
      SDL_LOG_DEBUG("Going to end video service");
      connection_handler().SendEndService(app_id, ServiceType::kMobileNav);
      app->StopStreamingForce(ServiceType::kMobileNav);
    }
    if (end_audio) {
      SDL_LOG_DEBUG("Going to end audio service");
      connection_handler().SendEndService(app_id, ServiceType::kAudio);
      app->StopStreamingForce(ServiceType::kAudio);
    }

    DisallowStreaming(app_id);

    {
      sync_primitives::AutoLock lock(navi_app_to_stop_lock_);
      navi_app_to_stop_.push_back(app_id);
    }

    TimerSPtr close_timer(std::make_shared<timer::Timer>(
        "CloseNaviAppTimer",
        new TimerTaskImpl<ApplicationManagerImpl>(
            this, &ApplicationManagerImpl::CloseNaviApp)));
    close_timer->Start(navi_close_app_timeout_, timer::kSingleShot);

    sync_primitives::AutoLock lock(close_app_timer_pool_lock_);
    close_app_timer_pool_.push_back(close_timer);
  }
}

void ApplicationManagerImpl::OnHMIStateChanged(const uint32_t app_id,
                                               const HmiStatePtr from,
                                               const HmiStatePtr to) {
  SDL_LOG_AUTO_TRACE();
  ProcessPostponedMessages(app_id);
  ProcessApp(app_id, from, to);
}

void ApplicationManagerImpl::ProcessPostponedMessages(const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    SDL_LOG_WARN("The app with id: " << app_id << " does not exist");
    return;
  }
  MobileMessageQueue messages;
  app->SwapMobileMessageQueue(messages);
  auto push_allowed_messages = [this,
                                &app](smart_objects::SmartObjectSPtr message) {
    const std::string function_id = MessageHelper::StringifiedFunctionID(
        static_cast<mobile_apis::FunctionID::eType>(
            (*message)[strings::params][strings::function_id].asUInt()));
    const WindowID window_id = MessageHelper::ExtractWindowIdFromSmartObject(
        (*message)[strings::msg_params]);
    const RPCParams params;
    const mobile_apis::Result::eType check_result =
        CheckPolicyPermissions(app, window_id, function_id, params);
    if (mobile_api::Result::SUCCESS == check_result) {
      rpc_service_->ManageMobileCommand(message, commands::Command::SOURCE_SDL);
    } else {
      app->PushMobileMessage(message);
    }
  };
  std::for_each(messages.begin(), messages.end(), push_allowed_messages);
}

void ApplicationManagerImpl::ProcessOnDataStreamingNotification(
    const protocol_handler::ServiceType service_type,
    const uint32_t app_id,
    const bool streaming_data_available) {
  SDL_LOG_AUTO_TRACE();

  bool should_send_notification = false;

  {
    sync_primitives::AutoLock lock(streaming_services_lock_);
    auto& active_services = streaming_application_services_[service_type];
    should_send_notification = active_services.empty();
    if (streaming_data_available) {
      active_services.insert(app_id);
      SDL_LOG_DEBUG("Streaming session with id "
                    << app_id << " for service "
                    << static_cast<uint32_t>(service_type)
                    << " was added. Currently streaming sessions count: "
                    << active_services.size());
    } else {
      active_services.erase(app_id);
      should_send_notification =
          !should_send_notification && active_services.empty();

      SDL_LOG_DEBUG("Streaming session with id "
                    << app_id << " for service "
                    << static_cast<uint32_t>(service_type)
                    << " was removed. Currently streaming sessions count: "
                    << active_services.size());
    }
  }

  if (should_send_notification) {
    MessageHelper::SendOnDataStreaming(
        service_type, streaming_data_available, *this);
  }
}

void ApplicationManagerImpl::ProcessApp(const uint32_t app_id,
                                        const HmiStatePtr from,
                                        const HmiStatePtr to) {
  using namespace mobile_apis::HMILevel;
  using namespace mobile_apis::VideoStreamingState;
  using namespace helpers;

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_ERROR("Navi/Projection application not found");
    return;
  }

  const auto hmi_level_from = from->hmi_level();
  const auto hmi_level_to = to->hmi_level();
  const auto streaming_state_from = from->video_streaming_state();
  const auto streaming_state_to = to->video_streaming_state();

  if (hmi_level_from == hmi_level_to &&
      streaming_state_from == streaming_state_to) {
    SDL_LOG_TRACE("HMILevel && streaming state were not changed");
    return;
  }

  auto full_or_limited_hmi_level = [](mobile_apis::HMILevel::eType hmi_level) {
    return Compare<mobile_apis::HMILevel::eType, EQ, ONE>(
        hmi_level, HMI_FULL, HMI_LIMITED);
  };

  const bool allow_streaming_by_streaming_state =
      NOT_STREAMABLE == streaming_state_from &&
      STREAMABLE == streaming_state_to;
  const bool allow_streaming_by_hmi_level =
      HMI_BACKGROUND == hmi_level_from &&
      full_or_limited_hmi_level(hmi_level_to) &&
      STREAMABLE == streaming_state_to;
  if (allow_streaming_by_streaming_state || allow_streaming_by_hmi_level) {
    SDL_LOG_TRACE("Allow streaming by streaming state: "
                  << std::boolalpha << allow_streaming_by_streaming_state
                  << "; by hmi level: " << std::boolalpha
                  << allow_streaming_by_hmi_level);
    AllowStreaming(app_id);
    return;
  }

  const bool end_streaming_by_streaming_state =
      STREAMABLE == streaming_state_from &&
      NOT_STREAMABLE == streaming_state_to &&
      full_or_limited_hmi_level(hmi_level_to);
  const bool start_timer_by_hmi_level =
      full_or_limited_hmi_level(hmi_level_from) &&
      HMI_BACKGROUND == hmi_level_to;
  if (end_streaming_by_streaming_state || start_timer_by_hmi_level) {
    SDL_LOG_TRACE("Start EndStream timer by streaming state: "
                  << std::boolalpha << end_streaming_by_streaming_state
                  << "; by hmi level: " << std::boolalpha
                  << start_timer_by_hmi_level);
    StartEndStreamTimer(app_id);
    return;
  }

  const bool end_streaming_by_hmi_level =
      full_or_limited_hmi_level(hmi_level_from) && HMI_NONE == hmi_level_to;
  if (end_streaming_by_hmi_level) {
    SDL_LOG_TRACE("End streaming services by hmi level: "
                  << std::boolalpha << end_streaming_by_hmi_level);
    EndNaviServices(app_id);
    return;
  }

  SDL_LOG_TRACE("No actions required for app " << app_id);
}

bool ApplicationManagerImpl::ResetHelpPromt(ApplicationSharedPtr app) const {
  if (!app) {
    SDL_LOG_ERROR("Null pointer");
    return false;
  }
  const std::vector<std::string>& help_prompt = get_settings().help_prompt();

  smart_objects::SmartObject so_help_prompt =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  for (size_t i = 0; i < help_prompt.size(); ++i) {
    smart_objects::SmartObject help_prompt_item =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    help_prompt_item[strings::text] = help_prompt[i];
    help_prompt_item[strings::type] =
        hmi_apis::Common_SpeechCapabilities::SC_TEXT;
    so_help_prompt[i] = help_prompt_item;
  }

  app->set_help_prompt(so_help_prompt);
  return true;
}

bool ApplicationManagerImpl::ResetTimeoutPromt(ApplicationSharedPtr app) const {
  if (!app) {
    SDL_LOG_ERROR("Null pointer");
    return false;
  }

  const std::vector<std::string>& time_out_promt =
      get_settings().time_out_promt();

  smart_objects::SmartObject so_time_out_promt =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  for (uint32_t i = 0; i < time_out_promt.size(); ++i) {
    smart_objects::SmartObject timeoutPrompt =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    timeoutPrompt[strings::text] = time_out_promt[i];
    timeoutPrompt[strings::type] = hmi_apis::Common_SpeechCapabilities::SC_TEXT;
    so_time_out_promt[i] = timeoutPrompt;
  }

  app->set_timeout_prompt(so_time_out_promt);

  return true;
}

bool ApplicationManagerImpl::ResetVrHelpTitleItems(
    ApplicationSharedPtr app) const {
  if (!app) {
    SDL_LOG_ERROR("Null pointer");
    return false;
  }

  const std::string& vr_help_title = get_settings().vr_help_title();
  smart_objects::SmartObject so_vr_help_title(vr_help_title);

  app->reset_vr_help_title();
  app->reset_vr_help();
  app->set_vr_help_title(so_vr_help_title);

  return true;
}

void ApplicationManagerImpl::StartEndStreamTimer(const uint32_t app_id) {
  SDL_LOG_DEBUG("Start end stream timer for app " << app_id);
  navi_app_to_end_stream_.push_back(app_id);
  TimerSPtr end_stream_timer(std::make_shared<timer::Timer>(
      "DisallowAppStreamTimer",
      new TimerTaskImpl<ApplicationManagerImpl>(
          this, &ApplicationManagerImpl::EndNaviStreaming)));
  end_stream_timer->Start(navi_end_stream_timeout_, timer::kSingleShot);

  sync_primitives::AutoLock lock(end_stream_timer_pool_lock_);
  end_stream_timer_pool_.push_back(end_stream_timer);
}

void ApplicationManagerImpl::ClearTimerPool() {
  SDL_LOG_AUTO_TRACE();
  {
    sync_primitives::AutoLock lock(close_app_timer_pool_lock_);

    close_app_timer_pool_.erase(
        std::remove_if(close_app_timer_pool_.begin(),
                       close_app_timer_pool_.end(),
                       [](TimerSPtr timer) { return !timer->is_running(); }),
        close_app_timer_pool_.end());
  }

  {
    sync_primitives::AutoLock lock(end_stream_timer_pool_lock_);
    end_stream_timer_pool_.erase(
        std::remove_if(end_stream_timer_pool_.begin(),
                       end_stream_timer_pool_.end(),
                       [](TimerSPtr timer) { return !timer->is_running(); }),
        end_stream_timer_pool_.end());
  }
}

void ApplicationManagerImpl::CloseNaviApp() {
  SDL_LOG_AUTO_TRACE();
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;
  uint32_t app_id;

  {
    sync_primitives::AutoLock lock(navi_app_to_stop_lock_);
    DCHECK_OR_RETURN_VOID(!navi_app_to_stop_.empty());
    app_id = navi_app_to_stop_.front();
    navi_app_to_stop_.pop_front();
  }

  bool unregister = false;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() != it) {
      if (it->second.first || it->second.second) {
        unregister = true;
        navi_service_status_.erase(it);
      }
    }
  }
  if (unregister) {
    SDL_LOG_INFO("App haven't answered for EndService. Unregister it.");
    rpc_service_->ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app_id, PROTOCOL_VIOLATION),
        commands::Command::SOURCE_SDL);
    UnregisterApplication(app_id, ABORTED);
  }
}

void ApplicationManagerImpl::EndNaviStreaming() {
  SDL_LOG_AUTO_TRACE();
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;

  if (!navi_app_to_end_stream_.empty()) {
    const uint32_t app_id = navi_app_to_end_stream_.front();
    navi_app_to_end_stream_.pop_front();

    {
      sync_primitives::AutoLock lock(navi_app_to_stop_lock_);
      if (navi_app_to_stop_.end() == std::find(navi_app_to_stop_.begin(),
                                               navi_app_to_stop_.end(),
                                               app_id)) {
        DisallowStreaming(app_id);
      }
    }
  }
}

void ApplicationManagerImpl::DisallowStreaming(uint32_t app_id) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_ERROR("Navi/Projection application not found");
    return;
  }

  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() != it) {
      if (it->second.first) {
        app->set_video_streaming_allowed(false);
      }
      if (it->second.second) {
        app->set_audio_streaming_allowed(false);
      }
    }
  }
}

void ApplicationManagerImpl::AllowStreaming(uint32_t app_id) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    SDL_LOG_ERROR("Navi/Projection application not found");
    return;
  }

  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() != it) {
      if (it->second.first) {
        app->set_video_streaming_allowed(true);
      }
      if (it->second.second) {
        app->set_audio_streaming_allowed(true);
      }
    }
  }
}

bool ApplicationManagerImpl::IsApplicationForbidden(
    uint32_t connection_key, const std::string& mobile_app_id) const {
  const std::string name = GetHashedAppID(connection_key, mobile_app_id);
  return forbidden_applications.find(name) != forbidden_applications.end();
}

bool ApplicationManagerImpl::IsAppInReconnectMode(
    const connection_handler::DeviceHandle& device_id,
    const std::string& policy_app_id) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(reregister_wait_list_lock_ptr_);
  return reregister_wait_list_.end() !=
         std::find_if(reregister_wait_list_.begin(),
                      reregister_wait_list_.end(),
                      PolicyAppIdComparator(device_id, policy_app_id));
}

policy::DeviceConsent ApplicationManagerImpl::GetUserConsentForDevice(
    const std::string& device_id) const {
  return GetPolicyHandler().GetUserConsentForDevice(device_id);
}

mobile_apis::Result::eType ApplicationManagerImpl::SaveBinary(
    const std::vector<uint8_t>& binary_data,
    const std::string& file_path,
    const std::string& file_name,
    const uint64_t offset) {
  SDL_LOG_DEBUG("SaveBinaryWithOffset  binary_size = "
                << binary_data.size() << " offset = " << offset);

  if (binary_data.size() > file_system::GetAvailableDiskSpace(file_path)) {
    SDL_LOG_ERROR("Out of free disc space.");
    return mobile_apis::Result::OUT_OF_MEMORY;
  }

  const std::string full_file_path = file_path + "/" + file_name;
  const uint64_t file_size = file_system::FileSize(full_file_path);
  std::ofstream* file_stream;
  if (offset != 0) {
    if (file_size != offset) {
      SDL_LOG_DEBUG("ApplicationManagerImpl::SaveBinaryWithOffset offset"
                    << " does'n match existing file size");
      return mobile_apis::Result::INVALID_DATA;
    }
    file_stream = file_system::Open(full_file_path, std::ios_base::app);
  } else {
    SDL_LOG_DEBUG(
        "ApplicationManagerImpl::SaveBinaryWithOffset offset is 0, rewrite");
    // if offset == 0: rewrite file
    file_stream = file_system::Open(full_file_path, std::ios_base::out);
  }

  if (!file_system::Write(
          file_stream, binary_data.data(), binary_data.size())) {
    file_system::Close(file_stream);
    delete file_stream;
    file_stream = NULL;
    return mobile_apis::Result::GENERIC_ERROR;
  }

  file_system::Close(file_stream);
  delete file_stream;
  file_stream = NULL;
  SDL_LOG_INFO("Successfully write data to file");
  return mobile_apis::Result::SUCCESS;
}

uint32_t ApplicationManagerImpl::GetAvailableSpaceForApp(
    const std::string& folder_name) {
  const uint32_t app_quota = settings_.app_dir_quota();
  std::string app_storage_path = settings_.app_storage_folder();

  app_storage_path += "/";
  app_storage_path += folder_name;

  if (file_system::DirectoryExists(app_storage_path)) {
    size_t size_of_directory = file_system::DirectorySize(app_storage_path);
    if (app_quota < size_of_directory) {
      return 0;
    }

    uint32_t current_app_quota = app_quota - size_of_directory;
    uint32_t available_disk_space =
        file_system::GetAvailableDiskSpace(app_storage_path);

    if (current_app_quota > available_disk_space) {
      return available_disk_space;
    } else {
      return current_app_quota;
    }
  } else {
    return app_quota;
  }
}

bool ApplicationManagerImpl::IsHMICooperating() const {
  return hmi_cooperating_;
}

void ApplicationManagerImpl::SetHMICooperating(const bool hmi_cooperating) {
  hmi_cooperating_ = hmi_cooperating;
}

void ApplicationManagerImpl::OnApplicationListUpdateTimer() {
  SDL_LOG_DEBUG("Application list update timer finished");
  const bool is_new_app_registered = registered_during_timer_execution_;
  registered_during_timer_execution_ = false;

  const bool trigger_ptu = apps_size_ != applications().GetData().size();

  if (is_new_app_registered) {
    SendUpdateAppList();
  }

  GetPolicyHandler().OnAppsSearchCompleted(trigger_ptu);
}

void ApplicationManagerImpl::OnTimerSendTTSGlobalProperties() {
  std::vector<uint32_t> app_list;
  {
    sync_primitives::AutoLock lock(tts_global_properties_app_list_lock_);
    std::map<uint32_t, date_time::TimeDuration>::iterator it =
        tts_global_properties_app_list_.begin();
    std::map<uint32_t, date_time::TimeDuration>::iterator it_end =
        tts_global_properties_app_list_.end();
    date_time::TimeCompare time_comp;
    for (; it != it_end; ++it) {
      time_comp =
          date_time::compareTime(date_time::getCurrentTime(), it->second);
      if (date_time::GREATER == time_comp || date_time::EQUAL == time_comp) {
        app_list.push_back(it->first);
      }
    }
  }
  if (!app_list.empty()) {
    for (uint32_t i = 0; i < app_list.size(); ++i) {
      SDL_LOG_INFO("Send TTS GlobalProperties to HMI with default helpPrompt");
      MessageHelper::SendTTSGlobalProperties(
          application(app_list[i]), true, *this);
      RemoveAppFromTTSGlobalPropertiesList(app_list[i]);
    }
  }
}

void ApplicationManagerImpl::AddAppToTTSGlobalPropertiesList(
    const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  uint16_t timeout = get_settings().tts_global_properties_timeout();
  date_time::TimeDuration current_time = date_time::getCurrentTime();
  current_time += date_time::seconds(timeout);
  // please avoid AutoLock usage to avoid deadlock
  tts_global_properties_app_list_lock_.Acquire();
  if (tts_global_properties_app_list_.end() ==
      tts_global_properties_app_list_.find(app_id)) {
    tts_global_properties_app_list_[app_id] = current_time;
  }
  // if add first item need to start timer on one second
  if (1 == tts_global_properties_app_list_.size()) {
    SDL_LOG_INFO("Start tts_global_properties_timer_");
    tts_global_properties_app_list_lock_.Release();
    const uint32_t timeout_ms = 1000;
    tts_global_properties_timer_.Start(timeout_ms, timer::kSingleShot);
    return;
  }
  tts_global_properties_app_list_lock_.Release();
}

void ApplicationManagerImpl::RemoveAppFromTTSGlobalPropertiesList(
    const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  // please avoid AutoLock usage to avoid deadlock
  tts_global_properties_app_list_lock_.Acquire();
  std::map<uint32_t, date_time::TimeDuration>::iterator it =
      tts_global_properties_app_list_.find(app_id);
  if (tts_global_properties_app_list_.end() != it) {
    tts_global_properties_app_list_.erase(it);
    if (tts_global_properties_app_list_.empty()) {
      SDL_LOG_DEBUG("Stop tts_global_properties_timer_");
      // if container is empty need to stop timer
      tts_global_properties_app_list_lock_.Release();
      tts_global_properties_timer_.Stop();
      return;
    }
  }
  tts_global_properties_app_list_lock_.Release();
}

ResetGlobalPropertiesResult ApplicationManagerImpl::ResetGlobalProperties(
    const smart_objects::SmartObject& global_properties_ids,
    const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr application =
      ApplicationManagerImpl::application(app_id);
  // if application waits for sending ttsGlobalProperties need to remove this
  // application from tts_global_properties_app_list_
  SDL_LOG_DEBUG("RemoveAppFromTTSGlobalPropertiesList");
  RemoveAppFromTTSGlobalPropertiesList(app_id);

  ResetGlobalPropertiesResult result;

  for (size_t i = 0; i < global_properties_ids.length(); ++i) {
    mobile_apis::GlobalProperty::eType global_property =
        static_cast<mobile_apis::GlobalProperty::eType>(
            global_properties_ids[i].asInt());
    switch (global_property) {
      case mobile_apis::GlobalProperty::HELPPROMPT: {
        result.help_prompt = ResetHelpPromt(application);
        break;
      }
      case mobile_apis::GlobalProperty::TIMEOUTPROMPT: {
        result.timeout_prompt = ResetTimeoutPromt(application);
        break;
      }
      case mobile_apis::GlobalProperty::VRHELPTITLE:
      case mobile_apis::GlobalProperty::VRHELPITEMS: {
        if (!result.vr_has_been_reset) {
          result.vr_has_been_reset = true;
          result.vr_help_title_items = ResetVrHelpTitleItems(application);
        }
        break;
      }
      case mobile_apis::GlobalProperty::MENUNAME: {
        result.menu_name = true;
        break;
      }
      case mobile_apis::GlobalProperty::MENUICON: {
        result.menu_icon = true;
        break;
      }
      case mobile_apis::GlobalProperty::KEYBOARDPROPERTIES: {
        result.keyboard_properties = true;
        break;
      }
      default: {
        SDL_LOG_TRACE("Unknown global property: " << global_property);
        break;
      }
    }
  }

  return result;
}

ResetGlobalPropertiesResult
ApplicationManagerImpl::ResetAllApplicationGlobalProperties(
    const uint32_t app_id) {
  const smart_objects::SmartObjectSPtr application_gl_props =
      CreateAllAppGlobalPropsIDList(app_id);
  return ResetGlobalProperties(*application_gl_props, app_id);
}

const smart_objects::SmartObjectSPtr
ApplicationManagerImpl::CreateAllAppGlobalPropsIDList(
    const uint32_t app_id) const {
  auto global_properties = std::make_shared<smart_objects::SmartObject>(
      smart_objects::SmartType_Array);
  using namespace mobile_apis;

  ApplicationConstSharedPtr application =
      ApplicationManagerImpl::application(app_id);
  int32_t i = 0;

  if (application->help_prompt()) {
    (*global_properties)[i++] = GlobalProperty::HELPPROMPT;
  }
  if (application->timeout_prompt()) {
    (*global_properties)[i++] = GlobalProperty::TIMEOUTPROMPT;
  }
  if (application->vr_help_title()) {
    (*global_properties)[i++] = GlobalProperty::VRHELPTITLE;
  }
  if (application->vr_help()) {
    (*global_properties)[i++] = GlobalProperty::VRHELPITEMS;
  }
  if (application->menu_title()) {
    (*global_properties)[i++] = GlobalProperty::MENUNAME;
  }
  if (application->menu_icon()) {
    (*global_properties)[i++] = GlobalProperty::MENUICON;
  }
  if (application->keyboard_props()) {
    (*global_properties)[i++] = GlobalProperty::KEYBOARDPROPERTIES;
  }

  return global_properties;
}

bool ApplicationManagerImpl::CompareAppHMIType(
    const smart_objects::SmartObject& from_policy,
    const smart_objects::SmartObject& from_application) {
  SDL_LOG_AUTO_TRACE();
  bool equal = false;
  uint32_t lenght_policy_app_types = from_policy.length();
  uint32_t lenght_application_app_types = from_application.length();

  for (uint32_t i = 0; i < lenght_application_app_types; ++i) {
    for (uint32_t k = 0; k < lenght_policy_app_types; ++k) {
      if (from_application[i] == from_policy[k]) {
        equal = true;
        break;
      }
    }
    if (!equal) {
      return false;
    }
    equal = false;
  }
  return true;
}

void ApplicationManagerImpl::OnUpdateHMIAppType(
    std::map<std::string, std::vector<std::string> > app_hmi_types) {
  SDL_LOG_AUTO_TRACE();

  std::map<std::string, std::vector<std::string> >::iterator
      it_app_hmi_types_from_policy;
  std::vector<std::string> hmi_types_from_policy;
  smart_objects::SmartObject transform_app_hmi_types(
      smart_objects::SmartType_Array);
  bool flag_diffirence_app_hmi_type;
  DataAccessor<ApplicationSet> accessor(applications());
  for (ApplicationSetIt it = accessor.GetData().begin();
       it != accessor.GetData().end();
       ++it) {
    it_app_hmi_types_from_policy = app_hmi_types.find(((*it)->policy_app_id()));

    if (it_app_hmi_types_from_policy != app_hmi_types.end() &&
        ((it_app_hmi_types_from_policy->second).size())) {
      hmi_types_from_policy = (it_app_hmi_types_from_policy->second);

      if (transform_app_hmi_types.length()) {
        transform_app_hmi_types =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
      }

      for (uint32_t i = 0; i < hmi_types_from_policy.size(); ++i) {
        transform_app_hmi_types[i] =
            StringToEnum<mobile_api::AppHMIType::eType>(
                hmi_types_from_policy[i]);
      }

      ApplicationConstSharedPtr app = *it;
      const smart_objects::SmartObject* save_application_hmi_type =
          app->app_types();

      if (save_application_hmi_type == NULL ||
          ((*save_application_hmi_type).length() !=
           transform_app_hmi_types.length())) {
        flag_diffirence_app_hmi_type = true;
      } else {
        flag_diffirence_app_hmi_type = !(CompareAppHMIType(
            transform_app_hmi_types, *save_application_hmi_type));
      }

      if (flag_diffirence_app_hmi_type) {
        (*it)->set_app_types(transform_app_hmi_types);
        (*it)->ChangeSupportingAppHMIType();
        const mobile_apis::HMILevel::eType app_hmi_level =
            (*it)->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
        if (app_hmi_level == mobile_api::HMILevel::HMI_BACKGROUND) {
          MessageHelper::SendUIChangeRegistrationRequestToHMI(*it, *this);
        } else if ((app_hmi_level == mobile_api::HMILevel::HMI_FULL) ||
                   (app_hmi_level == mobile_api::HMILevel::HMI_LIMITED)) {
          MessageHelper::SendUIChangeRegistrationRequestToHMI(*it, *this);
          state_controller().SetRegularState(
              *it,
              mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
              mobile_apis::HMILevel::HMI_BACKGROUND,
              true);
        }
      }
    }
  }
}

void ApplicationManagerImpl::EraseAppFromReconnectionList(
    const ApplicationSharedPtr& app) {
  SDL_LOG_AUTO_TRACE();
  if (!app) {
    SDL_LOG_WARN("Application is not valid.");
    return;
  }

  const auto policy_app_id = app->policy_app_id();
  sync_primitives::AutoLock lock(reregister_wait_list_lock_ptr_);
  auto app_it =
      std::find_if(reregister_wait_list_.begin(),
                   reregister_wait_list_.end(),
                   PolicyAppIdComparator(app->device(), policy_app_id));
  if (reregister_wait_list_.end() != app_it) {
    reregister_wait_list_.erase(app_it);
  }
}

void ApplicationManagerImpl::ProcessReconnection(
    ApplicationSharedPtr application, const uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(application);

  connection_handler::DeviceHandle new_device_id = 0;
  connection_handler().get_session_observer().GetDataOnSessionKey(
      connection_key, NULL, NULL, &new_device_id);
  DCHECK_OR_RETURN_VOID(new_device_id);

  std::string device_mac;
  std::string connection_type;
  connection_handler().get_session_observer().GetDataOnDeviceID(
      new_device_id, NULL, NULL, &device_mac, &connection_type);

  EraseAppFromReconnectionList(application);

  SwitchApplication(application, connection_key, new_device_id, device_mac);

  // Update connection type for existed device.
  GetPolicyHandler().AddDevice(device_mac, connection_type);
}

void ApplicationManagerImpl::OnPTUFinished(const bool ptu_result) {
  SDL_LOG_AUTO_TRACE();
  if (!ptu_result) {
    protocol_handler_->ProcessFailedPTU();
    return;
  }

  RefreshCloudAppInformation();

  auto app_id = policy_handler_->GetAppIdForSending();
  auto app = application(app_id);
  if (app) {
    SendGetIconUrlNotifications(app->app_id(), app);
  }

  auto on_app_policy_updated = [](plugin_manager::RPCPlugin& plugin) {
    plugin.OnPolicyEvent(plugin_manager::kApplicationPolicyUpdated);
  };

  plugin_manager_->ForEachPlugin(on_app_policy_updated);
}

#if defined(EXTERNAL_PROPRIETARY_MODE) && defined(ENABLE_SECURITY)
void ApplicationManagerImpl::OnCertDecryptFinished(const bool decrypt_result) {
  SDL_LOG_AUTO_TRACE();
  if (!decrypt_result) {
    protocol_handler_->ProcessFailedCertDecrypt();
  }
}

bool ApplicationManagerImpl::OnCertDecryptFailed() {
  SDL_LOG_AUTO_TRACE();
  return false;
}
#endif

void ApplicationManagerImpl::OnPTUTimeoutExceeded() {
  SDL_LOG_AUTO_TRACE();
  protocol_handler_->ProcessFailedPTU();
}

void ApplicationManagerImpl::SendDriverDistractionState(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  if (hmi_apis::Common_DriverDistractionState::INVALID_ENUM ==
      driver_distraction_state()) {
    SDL_LOG_WARN("DriverDistractionState is INVALID_ENUM");
    return;
  }

  auto create_notification = [application, this]() {
    auto notification = std::make_shared<smart_objects::SmartObject>();
    auto& msg_params = (*notification)[strings::msg_params];
    auto& params = (*notification)[strings::params];

    params[strings::message_type] =
        static_cast<int32_t>(application_manager::MessageType::kNotification);
    params[strings::function_id] =
        static_cast<int32_t>(mobile_apis::FunctionID::OnDriverDistractionID);
    msg_params[mobile_notification::state] = driver_distraction_state();
    const auto lock_screen_dismissal =
        policy_handler_->LockScreenDismissalEnabledState();

    if (lock_screen_dismissal &&
        hmi_apis::Common_DriverDistractionState::DD_ON ==
            driver_distraction_state()) {
      bool dismissal_enabled = *lock_screen_dismissal;
      if (dismissal_enabled) {
        const auto language = EnumToString(application->ui_language());

        const auto warning_message =
            policy_handler_->LockScreenDismissalWarningMessage(language);
        // Only allow lock screen dismissal if a warning message is available
        if (warning_message && !warning_message->empty()) {
          msg_params[mobile_notification::lock_screen_dismissal_warning] =
              *warning_message;
        } else {
          dismissal_enabled = false;
        }
      }
      msg_params[mobile_notification::lock_screen_dismissal_enabled] =
          dismissal_enabled;
    }

    params[strings::connection_key] = application->app_id();
    return notification;
  };

  const RPCParams params;
  const std::string function_id = MessageHelper::StringifiedFunctionID(
      mobile_api::FunctionID::OnDriverDistractionID);
  const mobile_apis::Result::eType check_result =
      CheckPolicyPermissions(application,
                             mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
                             function_id,
                             params);
  if (mobile_api::Result::SUCCESS == check_result) {
    rpc_service_->ManageMobileCommand(create_notification(),
                                      commands::Command::SOURCE_SDL);
  } else {
    MobileMessageQueue messages;
    application->SwapMobileMessageQueue(messages);
    messages.erase(
        std::remove_if(
            messages.begin(),
            messages.end(),
            [](smart_objects::SmartObjectSPtr message) {
              return (*message)[strings::params][strings::function_id]
                         .asUInt() ==
                     mobile_apis::FunctionID::OnDriverDistractionID;
            }),
        messages.end());
    application->SwapMobileMessageQueue(messages);
    application->PushMobileMessage(create_notification());
  }
}

void ApplicationManagerImpl::SendGetIconUrlNotifications(
    const uint32_t connection_key, ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  std::vector<std::string> enabled_apps;
  GetPolicyHandler().GetEnabledCloudApps(enabled_apps);
  std::vector<std::string>::iterator enabled_it = enabled_apps.begin();
  std::vector<std::string>::iterator enabled_end = enabled_apps.end();
  sync_primitives::AutoLock lock(app_icon_map_lock_ptr_);
  for (; enabled_it != enabled_end; ++enabled_it) {
    auto app_icon_it = app_icon_map_.find(*enabled_it);
    if (app_icon_it == app_icon_map_.end()) {
      SDL_LOG_WARN("Could not find cloud app in icon map");
      continue;
    }

    bool pending_request = app_icon_it->second.pending_request;

    if (pending_request) {
      SDL_LOG_DEBUG("Cloud app has already sent request");
      continue;
    }

    std::string url = GetPolicyHandler().GetIconUrl(*enabled_it);

    if (url.empty()) {
      SDL_LOG_DEBUG("No Icon Url for cloud app");
      continue;
    }

    SDL_LOG_DEBUG("Creating Get Icon Request");

    smart_objects::SmartObjectSPtr message =
        std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Map);
    (*message)[strings::params][strings::function_id] =
        mobile_apis::FunctionID::OnSystemRequestID;
    (*message)[strings::params][strings::connection_key] = connection_key;
    (*message)[strings::params][strings::message_type] =
        mobile_apis::messageType::notification;
    (*message)[strings::params][strings::protocol_version] =
        application->protocol_version();
    (*message)[strings::msg_params][strings::request_type] =
        mobile_apis::RequestType::ICON_URL;
    (*message)[strings::msg_params][strings::url] = url;

    app_icon_it->second.pending_request = true;

    rpc_service_->ManageMobileCommand(message, commands::Command::SOURCE_SDL);
  }
}

protocol_handler::MajorProtocolVersion
ApplicationManagerImpl::SupportedSDLVersion() const {
  SDL_LOG_AUTO_TRACE();
  return static_cast<protocol_handler::MajorProtocolVersion>(
      get_settings().max_supported_protocol_version());
}

void ApplicationManagerImpl::AddAppToRegisteredAppList(
    const ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(application);
  sync_primitives::AutoLock lock(applications_list_lock_ptr_);

  // Add application to registered app list and set appropriate mark.
  application->MarkRegistered();
  applications_.insert(application);
  SDL_LOG_DEBUG("App with app_id: "
                << application->app_id()
                << " has been added to registered applications list");
  if (application_list_update_timer_.is_running() &&
      !registered_during_timer_execution_) {
    GetPolicyHandler().OnAddedNewApplicationToAppList(
        application->app_id(), application->policy_app_id());
    registered_during_timer_execution_ = true;
  }
  apps_size_ = static_cast<uint32_t>(applications_.size());
}

void ApplicationManagerImpl::ApplyFunctorForEachPlugin(
    std::function<void(plugin_manager::RPCPlugin&)> functor) {
  SDL_LOG_AUTO_TRACE();
  plugin_manager_->ForEachPlugin(functor);
}

event_engine::EventDispatcher& ApplicationManagerImpl::event_dispatcher() {
  return event_dispatcher_;
}

app_launch::AppLaunchCtrl& ApplicationManagerImpl::app_launch_ctrl() {
  return *app_launch_ctrl_;
}

const std::string ApplicationManagerImpl::DirectoryTypeToString(
    ApplicationManagerImpl::DirectoryType type) const {
  DirectoryTypeMap::const_iterator it = dir_type_to_string_map_.find(type);
  if (it != dir_type_to_string_map_.end()) {
    return it->second;
  }
  return "Unknown";
}

bool ApplicationManagerImpl::InitDirectory(
    const std::string& path, ApplicationManagerImpl::DirectoryType type) const {
  const std::string directory_type = DirectoryTypeToString(type);
  if (!file_system::DirectoryExists(path)) {
    SDL_LOG_WARN(directory_type << " directory doesn't exist.");
    // if storage directory doesn't exist try to create it
    if (!file_system::CreateDirectoryRecursively(path)) {
      SDL_LOG_ERROR("Unable to create " << directory_type << " directory "
                                        << path);
      return false;
    }
    SDL_LOG_DEBUG(directory_type << " directory has been created: " << path);
  }

  return true;
}

bool ApplicationManagerImpl::IsReadWriteAllowed(const std::string& path,
                                                DirectoryType type) const {
  const std::string directory_type = DirectoryTypeToString(type);
  if (!(file_system::IsWritingAllowed(path) &&
        file_system::IsReadingAllowed(path))) {
    SDL_LOG_ERROR(directory_type
                  << " directory doesn't have read/write permissions.");
    return false;
  }

  SDL_LOG_DEBUG(directory_type << " directory has read/write permissions.");

  return true;
}

void ApplicationManagerImpl::ClearTTSGlobalPropertiesList() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(tts_global_properties_app_list_lock_);
  tts_global_properties_app_list_.clear();
}

bool ApplicationManagerImpl::IsAppSubscribedForWayPoints(
    uint32_t app_id) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  SDL_LOG_DEBUG("There are applications subscribed: "
                << subscribed_way_points_apps_list_.size());
  if (subscribed_way_points_apps_list_.find(app_id) ==
      subscribed_way_points_apps_list_.end()) {
    return false;
  }
  return true;
}

bool ApplicationManagerImpl::IsAppSubscribedForWayPoints(
    Application& app) const {
  return IsAppSubscribedForWayPoints(app.app_id());
}

void ApplicationManagerImpl::SubscribeAppForWayPoints(uint32_t app_id,
                                                      bool response_from_hmi) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  SDL_LOG_DEBUG("Subscribing " << app_id);
  subscribed_way_points_apps_list_.insert(app_id);
  if (response_from_hmi) {
    subscribed_to_hmi_way_points_ = true;
  }
  SDL_LOG_DEBUG("There are applications subscribed: "
                << subscribed_way_points_apps_list_.size());
  if (GetAppServiceManager().FindWayPointsHandler() != nullptr) {
    auto service = GetAppServiceManager().ActiveServiceForType(
        EnumToString(mobile_apis::AppServiceType::NAVIGATION));
    auto it = mobile_way_points_data_.find(service->connection_key);
    if (mobile_way_points_data_.end() == it) {
      SDL_LOG_DEBUG("No waypoint data provided by app service provider yet");
      return;
    }
    smart_objects::SmartObjectSPtr way_point_notification =
        std::make_shared<smart_objects::SmartObject>(it->second);
    (*way_point_notification)[strings::params][strings::connection_key] =
        app_id;
    GetRPCService().SendMessageToMobile(way_point_notification);
  } else if (hmi_way_points_data_) {
    smart_objects::SmartObjectSPtr way_point_notification =
        std::make_shared<smart_objects::SmartObject>(*hmi_way_points_data_);
    (*way_point_notification)[strings::params][strings::connection_key] =
        app_id;
    GetRPCService().SendMessageToMobile(way_point_notification);
  }
}

void ApplicationManagerImpl::SubscribeAppForWayPoints(ApplicationSharedPtr app,
                                                      bool response_from_hmi) {
  SubscribeAppForWayPoints(app->app_id(), response_from_hmi);
}

void ApplicationManagerImpl::UnsubscribeAppFromWayPoints(
    uint32_t app_id, bool response_from_hmi) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  SDL_LOG_DEBUG("Unsubscribing " << app_id);
  subscribed_way_points_apps_list_.erase(app_id);
  if (response_from_hmi) {
    subscribed_to_hmi_way_points_ = false;
  }
  SDL_LOG_DEBUG("There are applications subscribed: "
                << subscribed_way_points_apps_list_.size());
  if (subscribed_way_points_apps_list_.empty()) {
    hmi_way_points_data_.reset();
    mobile_way_points_data_.clear();
  }
}

void ApplicationManagerImpl::UnsubscribeAppFromWayPoints(
    ApplicationSharedPtr app, bool response_from_hmi) {
  UnsubscribeAppFromWayPoints(app->app_id(), response_from_hmi);
}

bool ApplicationManagerImpl::IsSubscribedToHMIWayPoints() const {
  SDL_LOG_AUTO_TRACE();
  return subscribed_to_hmi_way_points_;
}

bool ApplicationManagerImpl::IsAnyAppSubscribedForWayPoints() const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  SDL_LOG_DEBUG("There are applications subscribed: "
                << subscribed_way_points_apps_list_.size());
  return !subscribed_way_points_apps_list_.empty();
}

void ApplicationManagerImpl::SaveWayPointsMessage(
    std::shared_ptr<smart_objects::SmartObject> way_points_message,
    uint32_t app_id) {
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  // Notification from HMI
  if (0 == app_id) {
    hmi_way_points_data_ = way_points_message;
  }
  // Notification from app service provider
  else {
    mobile_way_points_data_[app_id] = *way_points_message;
  }
}

const std::set<uint32_t> ApplicationManagerImpl::GetAppsSubscribedForWayPoints()
    const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  return subscribed_way_points_apps_list_;
}

// retrieve transport type string used in .ini file, e.g. "TCP_WIFI"
const std::string ApplicationManagerImpl::GetTransportTypeProfileString(
    connection_handler::DeviceHandle device_handle) const {
  SDL_LOG_AUTO_TRACE();

  return connection_handler()
      .get_session_observer()
      .TransportTypeProfileStringFromDeviceHandle(device_handle);
}

static hmi_apis::Common_VideoStreamingProtocol::eType ConvertVideoProtocol(
    const char* str) {
  if (strcmp(str, "RAW") == 0) {
    return hmi_apis::Common_VideoStreamingProtocol::RAW;
  } else if (strcmp(str, "RTP") == 0) {
    return hmi_apis::Common_VideoStreamingProtocol::RTP;
  } else if (strcmp(str, "RTSP") == 0) {
    return hmi_apis::Common_VideoStreamingProtocol::RTSP;
  } else if (strcmp(str, "RTMP") == 0) {
    return hmi_apis::Common_VideoStreamingProtocol::RTMP;
  } else if (strcmp(str, "WEBM") == 0) {
    return hmi_apis::Common_VideoStreamingProtocol::WEBM;
  }
  return hmi_apis::Common_VideoStreamingProtocol::INVALID_ENUM;
}

static hmi_apis::Common_VideoStreamingCodec::eType ConvertVideoCodec(
    const char* str) {
  if (strcmp(str, "H264") == 0) {
    return hmi_apis::Common_VideoStreamingCodec::H264;
  } else if (strcmp(str, "H265") == 0) {
    return hmi_apis::Common_VideoStreamingCodec::H265;
  } else if (strcmp(str, "Theora") == 0) {
    return hmi_apis::Common_VideoStreamingCodec::Theora;
  } else if (strcmp(str, "VP8") == 0) {
    return hmi_apis::Common_VideoStreamingCodec::VP8;
  } else if (strcmp(str, "VP9") == 0) {
    return hmi_apis::Common_VideoStreamingCodec::VP9;
  }
  return hmi_apis::Common_VideoStreamingCodec::INVALID_ENUM;
}

// static
void ApplicationManagerImpl::ConvertVideoParamsToSO(
    smart_objects::SmartObject& output, const BsonObject* input) {
  if (input == NULL) {
    return;
  }
  BsonObject* obj = const_cast<BsonObject*>(input);

  const char* protocol =
      bson_object_get_string(obj, protocol_handler::strings::video_protocol);
  if (protocol != NULL) {
    output[strings::protocol] = ConvertVideoProtocol(protocol);
  }
  const char* codec =
      bson_object_get_string(obj, protocol_handler::strings::video_codec);
  if (codec != NULL) {
    output[strings::codec] = ConvertVideoCodec(codec);
  }
  BsonElement* element =
      bson_object_get(obj, protocol_handler::strings::height);
  if (element != NULL && element->type == TYPE_INT32) {
    output[strings::height] =
        bson_object_get_int32(obj, protocol_handler::strings::height);
  }
  element = bson_object_get(obj, protocol_handler::strings::width);
  if (element != NULL && element->type == TYPE_INT32) {
    output[strings::width] =
        bson_object_get_int32(obj, protocol_handler::strings::width);
  }
}

// static
std::vector<std::string> ApplicationManagerImpl::ConvertRejectedParamList(
    const std::vector<std::string>& input) {
  std::vector<std::string> output;
  for (std::vector<std::string>::const_iterator it = input.begin();
       it != input.end();
       ++it) {
    if (*it == strings::protocol) {
      output.push_back(protocol_handler::strings::video_protocol);
    } else if (*it == strings::codec) {
      output.push_back(protocol_handler::strings::video_codec);
    } else if (*it == strings::height) {
      output.push_back(protocol_handler::strings::height);
    } else if (*it == strings::width) {
      output.push_back(protocol_handler::strings::width);
    }
    // ignore unknown parameters
  }
  return output;
}

bool ApplicationManagerImpl::IsSOStructValid(
    const hmi_apis::StructIdentifiers::eType struct_id,
    const smart_objects::SmartObject& display_capabilities) {
  smart_objects::SmartObject display_capabilities_so = display_capabilities;
  if (hmi_so_factory().AttachSchema(struct_id, display_capabilities_so)) {
    return display_capabilities_so.isValid();
  }

  SDL_LOG_ERROR("Could not find struct id: " << struct_id);
  return false;
}

bool ApplicationManagerImpl::UnsubscribeAppFromSoftButtons(
    const commands::MessageSharedPtr response) {
  using namespace mobile_apis;

  const uint32_t connection_key =
      (*response)[strings::params][strings::connection_key].asUInt();
  const auto function_id = static_cast<FunctionID::eType>(
      (*response)[strings::params][strings::function_id].asInt());

  ApplicationSharedPtr app = application(connection_key);
  DCHECK_OR_RETURN(app, false);
  app->UnsubscribeFromSoftButtons(function_id);
  SDL_LOG_DEBUG("Application has unsubscribed from softbuttons. FunctionID: "
                << function_id << ", app_id:" << app->app_id());
  return true;
}

#ifdef BUILD_TESTS
void ApplicationManagerImpl::AddMockApplication(ApplicationSharedPtr mock_app) {
  applications_list_lock_ptr_->Acquire();
  applications_.insert(mock_app);
  apps_size_ = applications_.size();
  applications_list_lock_ptr_->Release();
}

void ApplicationManagerImpl::AddMockPendingApplication(
    ApplicationSharedPtr mock_app) {
  apps_to_register_list_lock_ptr_->Acquire();
  apps_to_register_.insert(mock_app);
  apps_to_register_list_lock_ptr_->Release();
}

void ApplicationManagerImpl::SetMockMediaManager(
    media_manager::MediaManager* mock_media_manager) {
  media_manager_ = mock_media_manager;
}
#endif  // BUILD_TESTS
struct MobileAppIdPredicate {
  std::string policy_app_id_;
  explicit MobileAppIdPredicate(const std::string& policy_app_id)
      : policy_app_id_(policy_app_id) {}
  bool operator()(const ApplicationSharedPtr app) const {
    return app ? policy_app_id_ == app->policy_app_id() : false;
  }
};

struct TakeDeviceHandle {
 public:
  explicit TakeDeviceHandle(const ApplicationManager& app_mngr)
      : app_mngr_(app_mngr) {}
  std::string operator()(ApplicationSharedPtr& app) {
    DCHECK_OR_RETURN(app, "");
    return MessageHelper::GetDeviceMacAddressForHandle(app->device(),
                                                       app_mngr_);
  }

 private:
  const ApplicationManager& app_mngr_;
};

ApplicationSharedPtr ApplicationManagerImpl::application(
    const std::string& device_id, const std::string& policy_app_id) const {
  connection_handler::DeviceHandle device_handle;
  if (!connection_handler().GetDeviceID(device_id, &device_handle)) {
    SDL_LOG_DEBUG("No such device : " << device_id);
    return ApplicationSharedPtr();
  }

  DataAccessor<ApplicationSet> accessor = applications();
  ApplicationSharedPtr app =
      FindApp(accessor, IsApplication(device_handle, policy_app_id));

  SDL_LOG_DEBUG(" policy_app_id << " << policy_app_id << "Found = " << app);
  return app;
}

std::vector<std::string> ApplicationManagerImpl::devices(
    const std::string& policy_app_id) const {
  MobileAppIdPredicate matcher(policy_app_id);
  AppSharedPtrs apps = FindAllApps(applications(), matcher);
  std::vector<std::string> devices;
  std::transform(apps.begin(),
                 apps.end(),
                 std::back_inserter(devices),
                 TakeDeviceHandle(*this));
  return devices;
}

void ApplicationManagerImpl::ChangeAppsHMILevel(
    uint32_t app_id, mobile_apis::HMILevel::eType level) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("AppID to change: " << app_id << " -> " << level);
  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    SDL_LOG_ERROR("There is no app with id: " << app_id);
    return;
  }

  const auto old_hmi_state =
      app->CurrentHmiState(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  if (old_hmi_state->hmi_level() != level) {
    app->set_hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW, level);
    const auto new_hmi_state =
        CreateRegularState(app,
                           mobile_apis::WindowType::MAIN,
                           level,
                           old_hmi_state->audio_streaming_state(),
                           old_hmi_state->video_streaming_state(),
                           old_hmi_state->system_context());
    OnHMIStateChanged(app_id, old_hmi_state, new_hmi_state);
  } else {
    SDL_LOG_WARN("Redundant changing HMI level: " << level);
  }
}

}  // namespace application_manager
