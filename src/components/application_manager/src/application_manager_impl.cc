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

#include <climits>
#include <string>
#include <fstream>
#include <utility>
#include <bson_object.h>

#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_command_factory.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_notification_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/hmi_capabilities_impl.h"
#include "application_manager/resumption/resume_ctrl_impl.h"
#include "application_manager/app_launch/app_launch_ctrl_impl.h"
#include "application_manager/app_launch/app_launch_data_db.h"
#include "application_manager/app_launch/app_launch_data_json.h"
#include "application_manager/helpers/application_helper.h"
#include "protocol_handler/protocol_handler.h"
#include "hmi_message_handler/hmi_message_handler.h"
#include "application_manager/command_holder_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "formatters/formatter_json_rpc.h"
#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "protocol/bson_object_keys.h"

#include "utils/threads/thread.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "utils/timer_task_impl.h"
#include "smart_objects/enum_schema_item.h"
#include "interfaces/HMI_API_schema.h"
#include "application_manager/application_impl.h"
#include "media_manager/media_manager.h"
#include "policy/usage_statistics/counter.h"
#include "utils/custom_string.h"
#include <time.h>

#ifdef SDL_REMOTE_CONTROL
#include "policy/usage_statistics/counter.h"
#include "functional_module/plugin_manager.h"
#include "application_manager/core_service.h"
#endif  // SDL_REMOTE_CONTROL

namespace {
int get_rand_from_range(uint32_t from = 0, int to = RAND_MAX) {
  return std::rand() % to + from;
}
}

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
    std::make_pair(std::string("WIFI"), hmi_apis::Common_TransportType::WIFI)};
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
  LOG4CXX_DEBUG(logger_,
                "Data to compare: device_id : " << device_id << " app mac: "
                                                << app->mac_address());

  return device_id == app->mac_address();
}

/**
 * @brief policy_app_id_comparator is predicate to compare policy application
 * ids
 * @param policy_app_id Policy id of application
 * @param app Application pointer
 * @return True if policy id of application matches to policy id passed
 */
bool policy_app_id_comparator(const std::string& policy_app_id,
                              ApplicationSharedPtr app) {
  DCHECK_OR_RETURN(app, false);
  return app->policy_app_id() == policy_app_id;
}

uint32_t ApplicationManagerImpl::corelation_id_ = 0;
const uint32_t ApplicationManagerImpl::max_corelation_id_ = UINT_MAX;

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

using namespace NsSmartDeviceLink::NsSmartObjects;

ApplicationManagerImpl::ApplicationManagerImpl(
    const ApplicationManagerSettings& am_settings,
    const policy::PolicySettings& policy_settings)
    : settings_(am_settings)
    , applications_list_lock_(true)
    , audio_pass_thru_active_(false)
    , is_distracting_driver_(false)
    , is_vr_session_strated_(false)
    , hmi_cooperating_(false)
    , is_all_apps_allowed_(true)
    , media_manager_(NULL)
    , hmi_handler_(NULL)
    , connection_handler_(NULL)
    , policy_handler_(new policy::PolicyHandler(policy_settings, *this))
    , protocol_handler_(NULL)
    , request_ctrl_(am_settings)
    , hmi_so_factory_(NULL)
    , mobile_so_factory_(NULL)
    , messages_from_mobile_("AM FromMobile", this)
    , messages_to_mobile_("AM ToMobile", this)
    , messages_from_hmi_("AM FromHMI", this)
    , messages_to_hmi_("AM ToHMI", this)
    , audio_pass_thru_messages_("AudioPassThru", this)
    , hmi_capabilities_(new HMICapabilitiesImpl(*this))
    , unregister_reason_(
          mobile_api::AppInterfaceUnregisteredReason::INVALID_ENUM)
    , resume_ctrl_(new resumption::ResumeCtrlImpl(*this))
    , navi_close_app_timeout_(am_settings.stop_streaming_timeout())
    , navi_end_stream_timeout_(am_settings.stop_streaming_timeout())
    , stopping_application_mng_lock_(true)
    , state_ctrl_(*this)
#ifdef TELEMETRY_MONITOR
    , metric_observer_(NULL)
#endif  // TELEMETRY_MONITOR
    , application_list_update_timer_(
          "AM ListUpdater",
          new TimerTaskImpl<ApplicationManagerImpl>(
              this, &ApplicationManagerImpl::OnApplicationListUpdateTimer))
    , tts_global_properties_timer_(
          "AM TTSGLPRTimer",
          new TimerTaskImpl<ApplicationManagerImpl>(
              this, &ApplicationManagerImpl::OnTimerSendTTSGlobalProperties))
    , is_low_voltage_(false)
    , apps_size_(0)
    , is_stopping_(false) {
  std::srand(std::time(0));
  AddPolicyObserver(this);

  dir_type_to_string_map_ = {{TYPE_STORAGE, "Storage"},
                             {TYPE_SYSTEM, "System"},
                             {TYPE_ICONS, "Icons"}};

  sync_primitives::AutoLock lock(timer_pool_lock_);
  TimerSPtr clearing_timer(utils::MakeShared<timer::Timer>(
      "ClearTimerPoolTimer",
      new TimerTaskImpl<ApplicationManagerImpl>(
          this, &ApplicationManagerImpl::ClearTimerPool)));
  const uint32_t timeout_ms = 10000u;
  clearing_timer->Start(timeout_ms, timer::kSingleShot);
  timer_pool_.push_back(clearing_timer);
  commands_holder_.reset(new CommandHolderImpl(*this));
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);

  is_stopping_ = true;
  SendOnSDLClose();
  media_manager_ = NULL;
  hmi_handler_ = NULL;
  connection_handler_ = NULL;
  if (hmi_so_factory_) {
    delete hmi_so_factory_;
    hmi_so_factory_ = NULL;
  }
  if (mobile_so_factory_) {
    delete mobile_so_factory_;
    mobile_so_factory_ = NULL;
  }
  protocol_handler_ = NULL;
  LOG4CXX_DEBUG(logger_, "Destroying Policy Handler");
  RemovePolicyObserver(this);

  sync_primitives::AutoLock lock(timer_pool_lock_);
  timer_pool_.clear();

  navi_app_to_stop_.clear();
  navi_app_to_end_stream_.clear();
}

DataAccessor<ApplicationSet> ApplicationManagerImpl::applications() const {
  DataAccessor<ApplicationSet> accessor(applications_, applications_list_lock_);
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

bool ActiveAppPredicate(const ApplicationSharedPtr app) {
  return app ? app->IsFullscreen() : false;
}

ApplicationSharedPtr ApplicationManagerImpl::active_application() const {
  // TODO(DK) : check driver distraction
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, ActiveAppPredicate);
}

bool LimitedAppPredicate(const ApplicationSharedPtr app) {
  return app ? app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED : false;
}

ApplicationSharedPtr ApplicationManagerImpl::get_limited_media_application()
    const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, LimitedAppPredicate);
}

bool LimitedNaviAppPredicate(const ApplicationSharedPtr app) {
  return app ? (app->is_navi() &&
                app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED)
             : false;
}

ApplicationSharedPtr ApplicationManagerImpl::get_limited_navi_application()
    const {
  DataAccessor<ApplicationSet> accessor = applications();
  return FindApp(accessor, LimitedNaviAppPredicate);
}

bool LimitedVoiceAppPredicate(const ApplicationSharedPtr app) {
  return app ? (app->is_voice_communication_supported() &&
                app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED)
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
                app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED)
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

std::vector<ApplicationSharedPtr> ApplicationManagerImpl::IviInfoUpdated(
    mobile_apis::VehicleDataType::eType vehicle_info, int value) {
  // Notify Policy Manager if available about info it's interested in,
  // i.e. odometer etc
  switch (vehicle_info) {
    case mobile_apis::VehicleDataType::VEHICLEDATA_ODOMETER:
      GetPolicyHandler().KmsChanged(value);
      break;
    default:
      break;
  }

  SubscribedToIVIPredicate finder(vehicle_info);
  DataAccessor<ApplicationSet> accessor = applications();
  return FindAllApps(accessor, finder);
}

void ApplicationManagerImpl::OnApplicationRegistered(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  sync_primitives::AutoLock lock(applications_list_lock_);
  const mobile_apis::HMILevel::eType default_level = GetDefaultHmiLevel(app);
  state_ctrl_.OnApplicationRegistered(app, default_level);

  // TODO(AOleynik): Is neccessary to be able to know that registration process
  // has been completed and default HMI level is set, otherwise policy will
  // block all the requests/notifications to mobile
  // APPLINK-20764 - introduce usage of internal events or re-implement
  event_engine::Event event(
      hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);

  smart_objects::SmartObject msg;
  msg[strings::params][strings::message_type] =
      hmi_apis::messageType::notification;
  msg[strings::params][strings::app_id] = app->app_id();

  event.set_smart_object(msg);
  event.raise(event_dispatcher());
}

void ApplicationManagerImpl::OnApplicationSwitched(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  commands_holder_->Resume(app, CommandHolder::CommandType::kMobileCommand);
  commands_holder_->Resume(app, CommandHolder::CommandType::kHmiCommand);
}

bool ApplicationManagerImpl::IsAppTypeExistsInFullOrLimited(
    ApplicationConstSharedPtr app) const {
  bool voice_state = app->is_voice_communication_supported();
  bool media_state = app->is_media_application();
  bool navi_state = app->is_navi();
  bool mobile_projection_state = app->mobile_projection_enabled();
  ApplicationSharedPtr active_app = active_application();
  // Check app in FULL level
  if (active_app.valid()) {
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
    if (get_limited_voice_application().valid() &&
        (get_limited_voice_application()->app_id() != app->app_id())) {
      return true;
    }
  }

  if (media_state) {
    if (get_limited_media_application().valid() &&
        (get_limited_media_application()->app_id() != app->app_id())) {
      return true;
    }
  }

  if (navi_state) {
    if (get_limited_navi_application().valid() &&
        (get_limited_navi_application()->app_id() != app->app_id())) {
      return true;
    }
  }

  if (mobile_projection_state) {
    if (get_limited_mobile_projection_application().valid() &&
        (get_limited_mobile_projection_application()->app_id() !=
         app->app_id())) {
      return true;
    }
  }

  return false;
}

ApplicationSharedPtr ApplicationManagerImpl::RegisterApplication(
    const utils::SharedPtr<smart_objects::SmartObject>&
        request_for_registration) {
  LOG4CXX_AUTO_TRACE(logger_);

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
    LOG4CXX_ERROR(logger_, "Failed to create application: no connection info.");
    utils::SharedPtr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::GENERIC_ERROR));
    ManageMobileCommand(response, commands::Command::ORIGIN_SDL);
    return ApplicationSharedPtr();
  }

  smart_objects::SmartObject& params = message[strings::msg_params];
  const std::string& policy_app_id = params[strings::app_id].asString();
  const custom_str::CustomString& app_name =
      message[strings::msg_params][strings::app_name].asCustomString();
  std::string device_mac;
  std::string connection_type;
  if (connection_handler().get_session_observer().GetDataOnDeviceID(
          device_id, NULL, NULL, &device_mac, &connection_type) == -1) {
    LOG4CXX_DEBUG(logger_, "Failed to extract device mac for id " << device_id);
  } else {
    LOG4CXX_DEBUG(logger_,
                  "Device mac for id" << device_id << " is " << device_mac);
  }

  LOG4CXX_DEBUG(logger_, "Restarting application list update timer");
  GetPolicyHandler().OnAppsSearchStarted();
  uint32_t timeout = get_settings().application_list_update_timeout();
  application_list_update_timer_.Start(timeout, timer::kSingleShot);

  if (!is_all_apps_allowed_) {
    LOG4CXX_WARN(logger_,
                 "RegisterApplication: access to app's disabled by user");
    utils::SharedPtr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::DISALLOWED));
    ManageMobileCommand(response, commands::Command::ORIGIN_SDL);
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
    utils::SharedPtr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
            connection_key,
            mobile_apis::FunctionID::RegisterAppInterfaceID,
            message[strings::params][strings::correlation_id].asUInt(),
            mobile_apis::Result::OUT_OF_MEMORY));
    ManageMobileCommand(response, commands::Command::ORIGIN_SDL);
    return ApplicationSharedPtr();
  }

  HmiStatePtr initial_state =
      CreateRegularState(utils::SharedPtr<Application>(application),
                         mobile_apis::HMILevel::INVALID_ENUM,
                         mobile_apis::AudioStreamingState::INVALID_ENUM,
                         mobile_apis::VideoStreamingState::INVALID_ENUM,
                         mobile_api::SystemContext::SYSCTXT_MAIN);

  application->SetInitialState(initial_state);

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
                               [strings::minor_version].asInt();
  version.min_supported_api_version = static_cast<APIVersion>(min_version);

  int32_t max_version = message[strings::msg_params][strings::sync_msg_version]
                               [strings::major_version].asInt();
  version.max_supported_api_version = static_cast<APIVersion>(max_version);
  application->set_version(version);

  protocol_handler::MajorProtocolVersion protocol_version =
      static_cast<protocol_handler::MajorProtocolVersion>(
          message[strings::params][strings::protocol_version].asInt());
  application->set_protocol_version(protocol_version);

  if (protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_UNKNOWN !=
      protocol_version) {
    connection_handler().BindProtocolVersionWithSession(
        connection_key, static_cast<uint8_t>(protocol_version));
  }
  if ((protocol_version ==
       protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3) &&
      (get_settings().heart_beat_timeout() != 0)) {
    connection_handler().StartSessionHeartBeat(connection_key);
  }

  // Keep HMI add id in case app is present in "waiting for registration" list
  apps_to_register_list_lock_.Acquire();
  AppsWaitRegistrationSet::iterator it = apps_to_register_.find(application);
  if (apps_to_register_.end() != it) {
    application->set_hmi_application_id((*it)->hmi_app_id());
    apps_to_register_.erase(application);
  }
  apps_to_register_list_lock_.Release();

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
  // Stops timer of saving data to resumption in order to
  // doesn't erase data from resumption storage.
  // Timer will be started after hmi level resumption.
  resume_controller().OnAppRegistrationStart(policy_app_id, device_mac);

  // Add application to registered app list and set appropriate mark.
  // Lock has to be released before adding app to policy DB to avoid possible
  // deadlock with simultaneous PTU processing
  applications_list_lock_.Acquire();
  application->MarkRegistered();
  applications_.insert(application);
  apps_size_ = applications_.size();
  applications_list_lock_.Release();

  return application;
}

DEPRECATED bool ApplicationManagerImpl::RemoveAppDataFromHMI(
    ApplicationSharedPtr app) {
  return true;
}

DEPRECATED bool ApplicationManagerImpl::LoadAppDataToHMI(
    ApplicationSharedPtr app) {
  return true;
}

bool ApplicationManagerImpl::ActivateApplication(ApplicationSharedPtr app) {
  using namespace mobile_api;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN(app, false);

  // remove from resumption if app was activated by user
  resume_controller().OnAppActivated(app);
  HMILevel::eType hmi_level = HMILevel::HMI_FULL;
  AudioStreamingState::eType audio_state;
  VideoStreamingState::eType video_state;
  app->IsAudioApplication() ? audio_state = AudioStreamingState::AUDIBLE
                            : audio_state = AudioStreamingState::NOT_AUDIBLE;
  app->IsVideoApplication() ? video_state = VideoStreamingState::STREAMABLE
                            : video_state = VideoStreamingState::NOT_STREAMABLE;
  state_ctrl_.SetRegularState(app, hmi_level, audio_state, video_state, false);
  return true;
}

mobile_api::HMILevel::eType ApplicationManagerImpl::IsHmiLevelFullAllowed(
    ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application pointer invalid");
    NOTREACHED();
    return mobile_api::HMILevel::INVALID_ENUM;
  }
  bool is_audio_app = app->IsAudioApplication();
  bool does_audio_app_with_same_type_exist =
      IsAppTypeExistsInFullOrLimited(app);
  bool is_active_app_exist = active_application().valid();

  mobile_api::HMILevel::eType result = mobile_api::HMILevel::HMI_FULL;
  if (is_audio_app && does_audio_app_with_same_type_exist) {
    result = GetDefaultHmiLevel(app);
  } else if (is_active_app_exist && is_audio_app) {
    result = mobile_apis::HMILevel::HMI_LIMITED;
  } else if (is_active_app_exist && (!is_audio_app)) {
    result = GetDefaultHmiLevel(app);
  }
  LOG4CXX_ERROR(
      logger_,
      "is_audio_app : " << is_audio_app
                        << "; does_audio_app_with_same_type_exist : "
                        << does_audio_app_with_same_type_exist
                        << "; is_active_app_exist : " << is_active_app_exist
                        << "; result : " << result);
  return result;
}

void ApplicationManagerImpl::ConnectToDevice(const std::string& device_mac) {
  // TODO(VS): Call function from ConnectionHandler
  if (!connection_handler_) {
    LOG4CXX_WARN(logger_, "Connection handler is not set.");
    return;
  }

  connection_handler::DeviceHandle handle;
  if (!connection_handler().GetDeviceID(device_mac, &handle)) {
    LOG4CXX_ERROR(
        logger_,
        "Attempt to connect to invalid device with mac:" << device_mac);
    return;
  }
  connection_handler().ConnectToDevice(handle);
}

void ApplicationManagerImpl::OnHMIStartedCooperation() {
  LOG4CXX_AUTO_TRACE(logger_);
  hmi_cooperating_ = true;
  MessageHelper::SendGetSystemInfoRequest(*this);

  utils::SharedPtr<smart_objects::SmartObject> is_vr_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_IsReady,
                                        *this));
  ManageHMICommand(is_vr_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_tts_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::TTS_IsReady,
                                        *this));
  ManageHMICommand(is_tts_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_ui_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::UI_IsReady,
                                        *this));
  ManageHMICommand(is_ui_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_navi_ready(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::Navigation_IsReady, *this));
  ManageHMICommand(is_navi_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_ivi_ready(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_IsReady, *this));
  ManageHMICommand(is_ivi_ready);

#ifdef SDL_REMOTE_CONTROL
  utils::SharedPtr<smart_objects::SmartObject> is_rc_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::RC_IsReady,
                                        *this));
  ManageHMICommand(is_rc_ready);
#endif

  utils::SharedPtr<smart_objects::SmartObject> button_capabilities(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::Buttons_GetCapabilities, *this));
  ManageHMICommand(button_capabilities);

  utils::SharedPtr<smart_objects::SmartObject> mixing_audio_supported_request(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported,
          *this));
  ManageHMICommand(mixing_audio_supported_request);
  resume_controller().ResetLaunchTime();
}

uint32_t ApplicationManagerImpl::GetNextHMICorrelationID() {
  if (corelation_id_ < max_corelation_id_) {
    corelation_id_++;
  } else {
    corelation_id_ = 0;
  }

  return corelation_id_;
}

bool ApplicationManagerImpl::BeginAudioPassThrough() {
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  if (audio_pass_thru_active_) {
    return false;
  } else {
    audio_pass_thru_active_ = true;
    return true;
  }
}

bool ApplicationManagerImpl::EndAudioPassThrough() {
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  if (audio_pass_thru_active_) {
    audio_pass_thru_active_ = false;
    return true;
  } else {
    return false;
  }
}

void ApplicationManagerImpl::set_driver_distraction(const bool is_distracting) {
  is_distracting_driver_ = is_distracting;
}

DEPRECATED void ApplicationManagerImpl::set_vr_session_started(
    const bool state) {
  is_vr_session_strated_ = state;
}

void ApplicationManagerImpl::SetAllAppsAllowed(const bool allowed) {
  is_all_apps_allowed_ = allowed;
}

HmiStatePtr ApplicationManagerImpl::CreateRegularState(
    utils::SharedPtr<Application> app,
    mobile_apis::HMILevel::eType hmi_level,
    mobile_apis::AudioStreamingState::eType audio_state,
    mobile_apis::VideoStreamingState::eType video_state,
    mobile_apis::SystemContext::eType system_context) const {
  HmiStatePtr state(new HmiState(app, *this));
  state->set_hmi_level(hmi_level);
  state->set_audio_streaming_state(audio_state);
  state->set_video_streaming_state(video_state);
  state->set_system_context(system_context);
  return state;
}

HmiStatePtr ApplicationManagerImpl::CreateRegularState(
    uint32_t app_id,
    mobile_apis::HMILevel::eType hmi_level,
    mobile_apis::AudioStreamingState::eType audio_state,
    mobile_apis::SystemContext::eType system_context) const {
  HmiStatePtr state(new HmiState(app_id, *this));
  state->set_hmi_level(hmi_level);
  state->set_audio_streaming_state(audio_state);
  state->set_system_context(system_context);
  return state;
}

void ApplicationManagerImpl::StartAudioPassThruThread(int32_t session_key,
                                                      int32_t correlation_id,
                                                      int32_t max_duration,
                                                      int32_t sampling_rate,
                                                      int32_t bits_per_sample,
                                                      int32_t audio_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_INFO(logger_, "START MICROPHONE RECORDER");
  DCHECK_OR_RETURN_VOID(media_manager_);
  media_manager_->StartMicrophoneRecording(
      session_key, get_settings().recording_file_name(), max_duration);
}

void ApplicationManagerImpl::SendAudioPassThroughNotification(
    uint32_t session_key, std::vector<uint8_t>& binary_data) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!audio_pass_thru_active_) {
    LOG4CXX_ERROR(logger_,
                  "Trying to send PassThroughNotification"
                  " when PassThrough is not active");
    return;
  }

  impl::AudioData data;
  data.session_key = session_key;
  data.binary_data = binary_data;
  audio_pass_thru_messages_.PostMessage(data);
}

void ApplicationManagerImpl::StopAudioPassThru(int32_t application_key) {
  LOG4CXX_AUTO_TRACE(logger_);
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
    LOG4CXX_ERROR(logger_, "Failed to extract device name for id " << handle);
  } else {
    LOG4CXX_DEBUG(logger_, "\t\t\t\t\tDevice name is " << device_name);
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
    LOG4CXX_ERROR(logger_, "Unknown transport type " << transport_type);
  }

  return result;
}

void ApplicationManagerImpl::OnMessageReceived(
    const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  utils::SharedPtr<Message> outgoing_message = ConvertRawMsgToMessage(message);

  if (outgoing_message) {
    LOG4CXX_DEBUG(logger_, "Posting new Message");
    messages_from_mobile_.PostMessage(
        impl::MessageFromMobile(outgoing_message));
  }
}

void ApplicationManagerImpl::OnMobileMessageSent(
    const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
}

void ApplicationManagerImpl::OnMessageReceived(
    hmi_message_handler::MessageSharedPointer message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  messages_from_hmi_.PostMessage(impl::MessageFromHmi(message));
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
                                               apps_to_register_list_lock_);
}

bool ApplicationManagerImpl::IsAppsQueriedFrom(
    const connection_handler::DeviceHandle handle) const {
  sync_primitives::AutoLock lock(apps_to_register_list_lock_);
  AppsWaitRegistrationSet::iterator it = apps_to_register_.begin();
  AppsWaitRegistrationSet::const_iterator it_end = apps_to_register_.end();
  for (; it != it_end; ++it) {
    if (handle == (*it)->device()) {
      return true;
    }
  }
  return false;
}

StateController& ApplicationManagerImpl::state_controller() {
  return state_ctrl_;
}

const ApplicationManagerSettings& ApplicationManagerImpl::get_settings() const {
  return settings_;
}

void application_manager::ApplicationManagerImpl::MarkAppsGreyOut(
    const connection_handler::DeviceHandle handle, bool is_greyed_out) {
  sync_primitives::AutoLock lock(apps_to_register_list_lock_);
  AppsWaitRegistrationSet::iterator it = apps_to_register_.begin();
  AppsWaitRegistrationSet::const_iterator it_end = apps_to_register_.end();
  for (; it != it_end; ++it) {
    if (handle == (*it)->device()) {
      (*it)->set_greyed_out(is_greyed_out);
    }
  }
}
void ApplicationManagerImpl::OnErrorSending(
    hmi_message_handler::MessageSharedPointer message) {
  return;
}

void ApplicationManagerImpl::OnDeviceListUpdated(
    const connection_handler::DeviceMap& device_list) {
  LOG4CXX_AUTO_TRACE(logger_);

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
    LOG4CXX_WARN(logger_, "Failed to create sub-smart object.");
    return;
  }

  smart_objects::SmartObjectSPtr update_list = new smart_objects::SmartObject;
  smart_objects::SmartObject& so_to_send = *update_list;
  so_to_send[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
      hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList;
  so_to_send[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
      hmi_apis::messageType::request;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 3;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = 1;
  so_to_send[jhs::S_PARAMS][jhs::S_CORRELATION_ID] = GetNextHMICorrelationID();
  so_to_send[jhs::S_MSG_PARAMS] = *msg_params;
  ManageHMICommand(update_list);
}

void ApplicationManagerImpl::OnFindNewApplicationsRequest() {
  connection_handler().ConnectToAllDevices();
  LOG4CXX_DEBUG(logger_, "Starting application list update timer");
  uint32_t timeout = get_settings().application_list_update_timeout();
  application_list_update_timer_.Start(timeout, timer::kSingleShot);
  GetPolicyHandler().OnAppsSearchStarted();
}

void ApplicationManagerImpl::SendUpdateAppList() {
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObjectSPtr request = MessageHelper::CreateModuleInfoSO(
      FunctionID::BasicCommunication_UpdateAppList, *this);

  (*request)[strings::msg_params][strings::applications] =
      SmartObject(SmartType_Array);

  SmartObject& applications =
      (*request)[strings::msg_params][strings::applications];

  PrepareApplicationListSO(applications_, applications, *this);
  PrepareApplicationListSO(apps_to_register_, applications, *this);

  ManageHMICommand(request);
}

void ApplicationManagerImpl::RemoveDevice(
    const connection_handler::DeviceHandle& device_handle) {
  LOG4CXX_DEBUG(logger_, "device_handle " << device_handle);
}

void ApplicationManagerImpl::OnDeviceSwitchingStart(
    const connection_handler::Device& device_from,
    const connection_handler::Device& device_to) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    auto apps_data_accessor = applications();

    std::copy_if(apps_data_accessor.GetData().begin(),
                 apps_data_accessor.GetData().end(),
                 std::back_inserter(reregister_wait_list_),
                 std::bind1st(std::ptr_fun(&device_id_comparator),
                              device_from.mac_address()));
  }

  {
    // During sending of UpdateDeviceList this lock is acquired also so making
    // it scoped
    sync_primitives::AutoLock lock(reregister_wait_list_lock_);
    for (auto i = reregister_wait_list_.begin();
         reregister_wait_list_.end() != i;
         ++i) {
      auto app = *i;
      request_ctrl_.terminateAppRequests(app->app_id());
      resume_ctrl_->SaveApplication(app);
    }
  }

  policy_handler_->OnDeviceSwitching(device_from.mac_address(),
                                     device_to.mac_address());

  connection_handler::DeviceMap device_list;
  device_list.insert(std::make_pair(device_to.device_handle(), device_to));

  smart_objects::SmartObjectSPtr msg_params =
      MessageHelper::CreateDeviceListSO(device_list, GetPolicyHandler(), *this);
  if (!msg_params) {
    LOG4CXX_ERROR(logger_, "Can't create UpdateDeviceList notification");
    return;
  }

  auto update_list = utils::MakeShared<smart_objects::SmartObject>();
  smart_objects::SmartObject& so_to_send = *update_list;
  so_to_send[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
      hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList;
  so_to_send[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
      hmi_apis::messageType::request;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 2;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = 1;
  so_to_send[jhs::S_PARAMS][jhs::S_CORRELATION_ID] = GetNextHMICorrelationID();
  so_to_send[jhs::S_MSG_PARAMS] = *msg_params;
  ManageHMICommand(update_list);
}

void ApplicationManagerImpl::OnDeviceSwitchingFinish(
    const std::string& device_uid) {
  LOG4CXX_AUTO_TRACE(logger_);
  UNUSED(device_uid);
  sync_primitives::AutoLock lock(reregister_wait_list_lock_);

  const bool unexpected_disonnect = true;
  const bool is_resuming = true;
  for (auto app_it = reregister_wait_list_.begin();
       app_it != reregister_wait_list_.end();
       ++app_it) {
    auto app = *app_it;
    UnregisterApplication(app->app_id(),
                          mobile_apis::Result::INVALID_ENUM,
                          is_resuming,
                          unexpected_disonnect);
  }
  reregister_wait_list_.clear();
}

void ApplicationManagerImpl::SwitchApplication(ApplicationSharedPtr app,
                                               const uint32_t connection_key,
                                               const size_t device_id,
                                               const std::string& mac_address) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  sync_primitives::AutoLock lock(applications_list_lock_);
  DCHECK_OR_RETURN_VOID(1 == applications_.erase(app));

  LOG4CXX_DEBUG(logger_,
                "Changing app id to " << connection_key
                                      << ". Changing device id to "
                                      << device_id);

  SwitchApplicationParameters(app, connection_key, device_id, mac_address);

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
  LOG4CXX_AUTO_TRACE(logger_);
  HMILevel::eType default_hmi = HMILevel::HMI_NONE;

  if (GetPolicyHandler().PolicyEnabled()) {
    const std::string policy_app_id = application->policy_app_id();
    std::string default_hmi_string = "";
    if (GetPolicyHandler().GetDefaultHmi(policy_app_id, &default_hmi_string)) {
      if ("BACKGROUND" == default_hmi_string) {
        default_hmi = HMILevel::HMI_BACKGROUND;
      } else if ("FULL" == default_hmi_string) {
        default_hmi = HMILevel::HMI_FULL;
      } else if ("LIMITED" == default_hmi_string) {
        default_hmi = HMILevel::HMI_LIMITED;
      } else if ("NONE" == default_hmi_string) {
        default_hmi = HMILevel::HMI_NONE;
      } else {
        LOG4CXX_ERROR(
            logger_,
            "Unable to convert " + default_hmi_string + " to HMILevel");
      }
    } else {
      LOG4CXX_ERROR(logger_,
                    "Unable to get default hmi_level for " << policy_app_id);
    }
  }
  return default_hmi;
}

uint32_t ApplicationManagerImpl::GenerateGrammarID() {
  return rand();
}

uint32_t ApplicationManagerImpl::GenerateNewHMIAppID() {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t hmi_app_id = get_rand_from_range(1);
  LOG4CXX_DEBUG(logger_, "GenerateNewHMIAppID value is: " << hmi_app_id);

  while (resume_controller().IsHMIApplicationIdExist(hmi_app_id)) {
    LOG4CXX_DEBUG(logger_, "HMI appID " << hmi_app_id << " is exists.");
    hmi_app_id = get_rand_from_range(1);
    LOG4CXX_DEBUG(logger_, "Trying new value: " << hmi_app_id);
  }

  return hmi_app_id;
}

void ApplicationManagerImpl::ReplaceMobileByHMIAppId(
    smart_objects::SmartObject& message) {
  MessageHelper::PrintSmartObject(message);
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application_ptr =
        application(message[strings::app_id].asUInt());
    if (application_ptr.valid()) {
      LOG4CXX_DEBUG(logger_,
                    "ReplaceMobileByHMIAppId from "
                        << message[strings::app_id].asInt() << " to "
                        << application_ptr->hmi_app_id());
      message[strings::app_id] = application_ptr->hmi_app_id();
    }
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for (; it != message_array->end(); ++it) {
          ReplaceMobileByHMIAppId(*it);
        }
        break;
      }
      case smart_objects::SmartType::SmartType_Map: {
        std::set<std::string> keys = message.enumerate();
        std::set<std::string>::const_iterator key = keys.begin();
        for (; key != keys.end(); ++key) {
          std::string k = *key;
          ReplaceMobileByHMIAppId(message[*key]);
        }
        break;
      }
      default: { break; }
    }
  }
}

void ApplicationManagerImpl::ReplaceHMIByMobileAppId(
    smart_objects::SmartObject& message) {
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
        application_by_hmi_app(message[strings::app_id].asUInt());

    if (application.valid()) {
      LOG4CXX_DEBUG(logger_,
                    "ReplaceHMIByMobileAppId from "
                        << message[strings::app_id].asInt() << " to "
                        << application->app_id());
      message[strings::app_id] = application->app_id();
    }
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for (; it != message_array->end(); ++it) {
          ReplaceHMIByMobileAppId(*it);
        }
        break;
      }
      case smart_objects::SmartType::SmartType_Map: {
        std::set<std::string> keys = message.enumerate();
        std::set<std::string>::const_iterator key = keys.begin();
        for (; key != keys.end(); ++key) {
          ReplaceHMIByMobileAppId(message[*key]);
        }
        break;
      }
      default: { break; }
    }
  }
}

bool ApplicationManagerImpl::StartNaviService(
    uint32_t app_id, protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  if (HMILevelAllowsStreaming(app_id, service_type)) {
    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it) {
      std::pair<NaviServiceStatusMap::iterator, bool> res =
          navi_service_status_.insert(
              std::pair<uint32_t, std::pair<bool, bool> >(
                  app_id, std::make_pair(false, false)));
      if (!res.second) {
        LOG4CXX_WARN(logger_, "Navi service refused");
        return false;
      }
      it = res.first;
    }
    // Fill NaviServices map. Set true to first value of pair if
    // we've started video service or to second value if we've
    // started audio service
    service_type == ServiceType::kMobileNav ? it->second.first = true
                                            : it->second.second = true;

    application(app_id)->StartStreaming(service_type);
    return true;
  } else {
    LOG4CXX_WARN(logger_, "Refused navi service by HMI level");
  }
  return false;
}

bool ApplicationManagerImpl::StartNaviService(
    uint32_t app_id,
    protocol_handler::ServiceType service_type,
    const BsonObject* params) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  if (HMILevelAllowsStreaming(app_id, service_type)) {
    {
      sync_primitives::AutoLock lock(navi_service_status_lock_);

      NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
      if (navi_service_status_.end() == it) {
        std::pair<NaviServiceStatusMap::iterator, bool> res =
            navi_service_status_.insert(
                std::pair<uint32_t, std::pair<bool, bool> >(
                    app_id, std::make_pair(false, false)));
        if (!res.second) {
          LOG4CXX_WARN(logger_, "Navi service refused");
          return false;
        }
        it = res.first;
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
        OnStreamingConfigured(app_id, service_type, false, rejected_params);
        return false;
      } else if (!converted_params.empty()) {
        LOG4CXX_INFO(logger_, "Sending video configuration params");
#ifdef DEBUG
        MessageHelper::PrintSmartObject(converted_params);
#endif
        bool request_sent =
            application(app_id)->SetVideoConfig(service_type, converted_params);
        if (request_sent) {
          return true;
        }
      }
    }
    // no configuration is needed, or SetVideoConfig is not sent
    std::vector<std::string> empty;
    OnStreamingConfigured(app_id, service_type, true, empty);
    return true;

  } else {
    LOG4CXX_WARN(logger_, "Refused navi service by HMI level");
  }
  std::vector<std::string> empty;
  OnStreamingConfigured(app_id, service_type, false, empty);
  return false;
}

void ApplicationManagerImpl::OnStreamingConfigured(
    uint32_t app_id,
    protocol_handler::ServiceType service_type,
    bool result,
    std::vector<std::string>& rejected_params) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  LOG4CXX_INFO(logger_,
               "OnStreamingConfigured called for service "
                   << service_type << ", result=" << result);

  if (result) {
    std::vector<std::string> empty;
    {
      sync_primitives::AutoLock lock(navi_service_status_lock_);

      NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
      if (navi_service_status_.end() == it) {
        LOG4CXX_WARN(logger_, "Application not found in navi status map");
        connection_handler().NotifyServiceStartedResult(app_id, false, empty);
        return;
      }

      // Fill NaviServices map. Set true to first value of pair if
      // we've started video service or to second value if we've
      // started audio service
      service_type == ServiceType::kMobileNav ? it->second.first = true
                                              : it->second.second = true;
    }

    application(app_id)->StartStreaming(service_type);
    connection_handler().NotifyServiceStartedResult(app_id, true, empty);
  } else {
    std::vector<std::string> converted_params =
        ConvertRejectedParamList(rejected_params);
    connection_handler().NotifyServiceStartedResult(
        app_id, false, converted_params);
  }
}

void ApplicationManagerImpl::StopNaviService(
    uint32_t app_id, protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it) {
      LOG4CXX_WARN(logger_,
                   "No Information about navi service " << service_type);
    } else {
      // Fill NaviServices map. Set false to first value of pair if
      // we've stopped video service or to second value if we've
      // stopped audio service
      service_type == ServiceType::kMobileNav ? it->second.first = false
                                              : it->second.second = false;
    }
  }

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return;
  }

  app->StopStreaming(service_type);
}

// Suppress warning for deprecated method used within another deprecated method
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
bool ApplicationManagerImpl::OnServiceStartedCallback(
    const connection_handler::DeviceHandle& device_handle,
    const int32_t& session_key,
    const protocol_handler::ServiceType& type) {
  using namespace helpers;
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "ServiceType = " << type << ". Session = " << std::hex
                                 << session_key);

  if (type == kRpc) {
    LOG4CXX_DEBUG(logger_, "RPC service is about to be started.");
    return true;
  }
  ApplicationSharedPtr app = application(session_key);
  if (!app) {
    LOG4CXX_WARN(logger_,
                 "The application with id:" << session_key
                                            << " doesn't exists.");
    return false;
  }

  if (Compare<ServiceType, EQ, ONE>(
          type, ServiceType::kMobileNav, ServiceType::kAudio)) {
    if (app->is_navi()) {
      return StartNaviService(session_key, type);
    } else {
      LOG4CXX_WARN(logger_, "Refuse not navi application");
    }
  } else {
    LOG4CXX_WARN(logger_, "Refuse unknown service");
  }
  return false;
}
#pragma GCC diagnostic pop

void ApplicationManagerImpl::OnServiceStartedCallback(
    const connection_handler::DeviceHandle& device_handle,
    const int32_t& session_key,
    const protocol_handler::ServiceType& type,
    const BsonObject* params) {
  using namespace helpers;
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "ServiceType = " << type << ". Session = " << std::hex
                                 << session_key);
  std::vector<std::string> empty;

  if (type == kRpc) {
    LOG4CXX_DEBUG(logger_, "RPC service is about to be started.");
    connection_handler().NotifyServiceStartedResult(session_key, true, empty);
    return;
  }
  ApplicationSharedPtr app = application(session_key);
  if (!app) {
    LOG4CXX_WARN(logger_,
                 "The application with id:" << session_key
                                            << " doesn't exists.");
    connection_handler().NotifyServiceStartedResult(session_key, false, empty);
    return;
  }

  if (Compare<ServiceType, EQ, ONE>(
          type, ServiceType::kMobileNav, ServiceType::kAudio)) {
    if (app->is_navi() || app->mobile_projection_enabled()) {
      if (!StartNaviService(session_key, type, params)) {
        LOG4CXX_WARN(logger_, "Starting Navigation service failed");
      }
      return;
    } else {
      LOG4CXX_WARN(logger_, "Refuse not navi/projection application");
    }
  } else {
    LOG4CXX_WARN(logger_, "Refuse unknown service");
  }
  connection_handler().NotifyServiceStartedResult(session_key, false, empty);
}

void ApplicationManagerImpl::OnServiceEndedCallback(
    const int32_t& session_key,
    const protocol_handler::ServiceType& type,
    const connection_handler::CloseSessionReason& close_reason) {
  using namespace helpers;
  using namespace protocol_handler;
  using namespace connection_handler;
  using namespace mobile_apis;

  LOG4CXX_DEBUG(logger_,
                "OnServiceEndedCallback for service "
                    << type << " with reason " << close_reason
                    << " in session 0x" << std::hex << session_key);

  auto app = application(static_cast<uint32_t>(session_key));
  if (!app) {
    return;
  }

  if (IsAppInReconnectMode(app->policy_app_id())) {
    LOG4CXX_DEBUG(logger_,
                  "Application is in reconnection list and won't be closed.");
    return;
  }

  if (type == kRpc) {
    LOG4CXX_INFO(logger_, "Remove application.");
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

        ManageMobileCommand(
            MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
                session_key, AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS),
            commands::Command::ORIGIN_SDL);
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

#ifdef ENABLE_SECURITY
bool ApplicationManagerImpl::OnHandshakeDone(
    uint32_t connection_key,
    security_manager::SSLContext::HandshakeResult result) {
  LOG4CXX_AUTO_TRACE(logger_);

  using security_manager::SSLContext;
  using namespace helpers;

  ApplicationSharedPtr app = application(connection_key);
  DCHECK_OR_RETURN(app, false);
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

void ApplicationManagerImpl::OnCertificateUpdateRequired() {
  LOG4CXX_AUTO_TRACE(logger_);
  GetPolicyHandler().OnPTExchangeNeeded();
}

bool ApplicationManagerImpl::GetPolicyCertificateData(std::string& data) const {
  LOG4CXX_AUTO_TRACE(logger_);
  data = GetPolicyHandler().RetrieveCertificate();
  return true;
}

security_manager::SSLContext::HandshakeContext
ApplicationManagerImpl::GetHandshakeContext(uint32_t key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using security_manager::SSLContext;
  ApplicationConstSharedPtr app = application(key);
  if (app) {
    return SSLContext::HandshakeContext(
        custom_str::CustomString(app->policy_app_id()), app->name());
  }
  return SSLContext::HandshakeContext();
}

bool ApplicationManagerImpl::CheckAppIsNavi(const uint32_t app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app = application(app_id);
  if (app) {
    return app->is_navi();
  }
  return false;
}
#endif  // ENABLE_SECURITY

void ApplicationManagerImpl::set_hmi_message_handler(
    hmi_message_handler::HMIMessageHandler* handler) {
  hmi_handler_ = handler;
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
}

void ApplicationManagerImpl::StartDevicesDiscovery() {
  connection_handler().get_device_discovery_starter().StartDevicesDiscovery();
}

void ApplicationManagerImpl::SendMessageToMobile(
    const commands::MessageSharedPtr message, bool final_message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  if (!protocol_handler_) {
    LOG4CXX_WARN(logger_, "No Protocol Handler set");
    return;
  }

  ApplicationSharedPtr app = application(
      (*message)[strings::params][strings::connection_key].asUInt());

  const bool is_result_code_exists =
      (*message)[strings::msg_params].keyExists(strings::result_code);

  if (!app) {
    LOG4CXX_ERROR(logger_, "No application associated with connection key");
    if (is_result_code_exists &&
        ((*message)[strings::msg_params][strings::result_code] ==
         NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_VERSION)) {
      (*message)[strings::params][strings::protocol_version] =
          protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1;
    } else {
      (*message)[strings::params][strings::protocol_version] =
          SupportedSDLVersion();
    }
  } else {
    (*message)[strings::params][strings::protocol_version] =
        app->protocol_version();
  }

  if (app && is_result_code_exists &&
      (*message)[strings::msg_params][strings::result_code] ==
          mobile_apis::Result::OUT_OF_MEMORY) {
    app->usage_report().RecordRejectionsSyncOutOfMemory();
  }

  mobile_so_factory().attachSchema(*message, false);
  LOG4CXX_DEBUG(
      logger_,
      "Attached schema to message, result if valid: " << message->isValid());

  // Messages to mobile are not yet prioritized so use default priority value
  utils::SharedPtr<Message> message_to_send(
      new Message(protocol_handler::MessagePriority::kDefault));
  if (!ConvertSOtoMessage((*message), (*message_to_send))) {
    LOG4CXX_WARN(logger_, "Can't send msg to Mobile: failed to create string");
    return;
  }

  smart_objects::SmartObject& msg_to_mobile = *message;
  // If correlation_id is not present, it is from-HMI message which should be
  // checked against policy permissions
  if (msg_to_mobile[strings::params].keyExists(strings::correlation_id)) {
    request_ctrl_.OnMobileResponse(
        msg_to_mobile[strings::params][strings::correlation_id].asUInt(),
        msg_to_mobile[strings::params][strings::connection_key].asUInt(),
        msg_to_mobile[strings::params][strings::function_id].asInt());
#ifdef SDL_REMOTE_CONTROL
    const mobile_apis::FunctionID::eType function_id =
        static_cast<mobile_apis::FunctionID::eType>(
            (*message)[strings::params][strings::function_id].asUInt());
    if (function_id == mobile_apis::FunctionID::RegisterAppInterfaceID &&
        (*message)[strings::msg_params][strings::success].asBool()) {
      LOG4CXX_INFO(logger_,
                   "Registered app "
                       << app->app_id() << " is "
                       << (plugin_manager_.IsAppForPlugins(app) ? "" : "not ")
                       << "for plugins.");
    }
#endif  // SDL_REMOTE_CONTROL
  } else if (app) {
    mobile_apis::FunctionID::eType function_id =
        static_cast<mobile_apis::FunctionID::eType>(
            (*message)[strings::params][strings::function_id].asUInt());
    RPCParams params;

    const smart_objects::SmartObject& s_map = (*message)[strings::msg_params];
    if (smart_objects::SmartType_Map == s_map.getType()) {
      smart_objects::SmartMap::iterator iter = s_map.map_begin();
      smart_objects::SmartMap::iterator iter_end = s_map.map_end();

      for (; iter != iter_end; ++iter) {
        if (true == iter->second.asBool()) {
          LOG4CXX_INFO(logger_, "Request's param: " << iter->first);
          params.insert(iter->first);
        }
      }
    }
    const std::string string_functionID =
        MessageHelper::StringifiedFunctionID(function_id);
    const mobile_apis::Result::eType check_result =
        CheckPolicyPermissions(app, string_functionID, params);
    if (mobile_apis::Result::SUCCESS != check_result) {
      LOG4CXX_WARN(logger_,
                   "Function \"" << string_functionID << "\" (#" << function_id
                                 << ") not allowed by policy");
      return;
    }

#ifdef EXTERNAL_PROPRIETARY_MODE
    if (function_id == mobile_apis::FunctionID::OnSystemRequestID) {
      mobile_apis::RequestType::eType request_type =
          static_cast<mobile_apis::RequestType::eType>(
              (*message)[strings::msg_params][strings::request_type].asUInt());
      if (mobile_apis::RequestType::PROPRIETARY == request_type ||
          mobile_apis::RequestType::HTTP == request_type) {
        GetPolicyHandler().OnUpdateRequestSentToMobile();
      }
    }
#endif  // EXTERNAL_PROPRIETARY_MODE
  }

  if (message_to_send->binary_data()) {
    LOG4CXX_DEBUG(
        logger_,
        "Binary data size: " << message_to_send->binary_data()->size());
  }
  messages_to_mobile_.PostMessage(
      impl::MessageToMobile(message_to_send, final_message));
}

void ApplicationManagerImpl::TerminateRequest(const uint32_t connection_key,
                                              const uint32_t corr_id,
                                              const int32_t function_id) {
  request_ctrl_.TerminateRequest(corr_id, connection_key, function_id, true);
}

bool ApplicationManagerImpl::ManageMobileCommand(
    const commands::MessageSharedPtr message,
    commands::Command::CommandOrigin origin) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    return false;
  }

  if (IsLowVoltage()) {
    LOG4CXX_WARN(logger_, "Low Voltage is active");
    return false;
  }
#ifdef DEBUG
  MessageHelper::PrintSmartObject(*message);
#endif

  LOG4CXX_DEBUG(logger_, "Trying to create message in mobile factory.");
  utils::SharedPtr<commands::Command> command(
      MobileCommandFactory::CreateCommand(message, origin, *this));

  if (!command) {
    LOG4CXX_WARN(logger_,
                 "RET  Failed to create mobile command from smart object");
    return false;
  }

  const uint32_t connection_key = static_cast<uint32_t>(
      (*message)[strings::params][strings::connection_key].asUInt());

  auto app_ptr = application(connection_key);
  if (app_ptr && IsAppInReconnectMode(app_ptr->policy_app_id())) {
    commands_holder_->Suspend(
        app_ptr, CommandHolder::CommandType::kMobileCommand, message);
    return true;
  }

  mobile_apis::FunctionID::eType function_id =
      static_cast<mobile_apis::FunctionID::eType>(
          (*message)[strings::params][strings::function_id].asInt());

  // Notifications from HMI have no such parameter
  uint32_t correlation_id =
      (*message)[strings::params].keyExists(strings::correlation_id)
          ? (*message)[strings::params][strings::correlation_id].asUInt()
          : 0;

  int32_t protocol_type =
      (*message)[strings::params][strings::protocol_type].asUInt();

  ApplicationSharedPtr app;
  int32_t message_type =
      (*message)[strings::params][strings::message_type].asInt();

  if (((mobile_apis::FunctionID::RegisterAppInterfaceID != function_id) &&
       (protocol_type == commands::CommandImpl::mobile_protocol_type_)) &&
      (mobile_apis::FunctionID::UnregisterAppInterfaceID != function_id)) {
    app = application(connection_key);
    if (!app) {
      LOG4CXX_ERROR(logger_, "RET APPLICATION_NOT_REGISTERED");
      smart_objects::SmartObjectSPtr response =
          MessageHelper::CreateNegativeResponse(
              connection_key,
              static_cast<int32_t>(function_id),
              correlation_id,
              static_cast<int32_t>(
                  mobile_apis::Result::APPLICATION_NOT_REGISTERED));

      SendMessageToMobile(response);
      return false;
    }

    // Message for "CheckPermission" must be with attached schema
    mobile_so_factory().attachSchema(*message, false);
  }

  if (message_type == mobile_apis::messageType::response) {
    if (command->Init()) {
      command->Run();
      command->CleanUp();
    }
    return true;
  }
  if (message_type == mobile_apis::messageType::notification) {
    request_ctrl_.addNotification(command);
    if (command->Init()) {
      command->Run();
      if (command->CleanUp()) {
        request_ctrl_.removeNotification(command.get());
      }
      // If CleanUp returned false notification should remove it self.
    }
    return true;
  }

  if (message_type == mobile_apis::messageType::request) {
    // commands will be launched from requesr_ctrl
    mobile_apis::HMILevel::eType app_hmi_level =
        mobile_apis::HMILevel::INVALID_ENUM;
    if (app) {
      app_hmi_level = app->hmi_level();
    }

    // commands will be launched from request_ctrl

    const request_controller::RequestController::TResult result =
        request_ctrl_.addMobileRequest(command, app_hmi_level);

    if (result == request_controller::RequestController::SUCCESS) {
      LOG4CXX_DEBUG(logger_, "Perform request");
    } else if (result == request_controller::RequestController::
                             TOO_MANY_PENDING_REQUESTS) {
      LOG4CXX_ERROR(logger_,
                    "RET  Unable top perform request: "
                        << "TOO_MANY_PENDING_REQUESTS");

      smart_objects::SmartObjectSPtr response =
          MessageHelper::CreateNegativeResponse(
              connection_key,
              static_cast<int32_t>(function_id),
              correlation_id,
              static_cast<int32_t>(
                  mobile_apis::Result::TOO_MANY_PENDING_REQUESTS));
      ApplicationSharedPtr app_ptr = application(connection_key);
      if (app_ptr) {
        app_ptr->usage_report().RecordRemovalsForBadBehavior();
      }

      SendMessageToMobile(response);
      return false;
    } else if (result ==
               request_controller::RequestController::TOO_MANY_REQUESTS) {
      LOG4CXX_ERROR(logger_,
                    "RET  Unable to perform request: "
                        << "TOO_MANY_REQUESTS");

      ManageMobileCommand(
          MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
              connection_key,
              mobile_api::AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS),
          commands::Command::ORIGIN_SDL);

      UnregisterApplication(connection_key,
                            mobile_apis::Result::TOO_MANY_PENDING_REQUESTS,
                            false);
      ApplicationSharedPtr app_ptr = application(connection_key);
      if (app_ptr) {
        app_ptr->usage_report().RecordRemovalsForBadBehavior();
      }
      return false;
    } else if (result == request_controller::RequestController::
                             NONE_HMI_LEVEL_MANY_REQUESTS) {
      LOG4CXX_ERROR(logger_,
                    "RET  Unable to perform request: "
                        << "REQUEST_WHILE_IN_NONE_HMI_LEVEL");

      ManageMobileCommand(
          MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
              connection_key,
              mobile_api::AppInterfaceUnregisteredReason::
                  REQUEST_WHILE_IN_NONE_HMI_LEVEL),
          commands::Command::ORIGIN_SDL);

      ApplicationSharedPtr app_ptr = application(connection_key);
      if (app_ptr) {
        app_ptr->usage_report().RecordRemovalsForBadBehavior();
      }
      UnregisterApplication(
          connection_key, mobile_apis::Result::INVALID_ENUM, false);
      return false;
    } else {
      LOG4CXX_ERROR(logger_, "RET  Unable to perform request: Unknown case");
      return false;
    }
    return true;
  }

  LOG4CXX_ERROR(logger_, "RET  UNKNOWN MESSAGE TYPE " << message_type);
  return false;
}

void ApplicationManagerImpl::RemoveHMIFakeParameters(
    application_manager::MessagePtr& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace NsSmartDeviceLink::NsSmartObjects;
  using namespace NsSmartDeviceLink::NsJSONHandler;
  SmartObject so;

  Formatters::FormatterJsonRpc::FromString<hmi_apis::FunctionID::eType,
                                           hmi_apis::messageType::eType>(
      message->json_message(), so);

  std::string formatted_message;
  namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
  hmi_apis::HMI_API factory;
  factory.attachSchema(so, true);
  Formatters::FormatterJsonRpc::ToString(so, formatted_message);
  message->set_json_message(formatted_message);
}

void ApplicationManagerImpl::SendMessageToHMI(
    const commands::MessageSharedPtr message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  if (!hmi_handler_) {
    LOG4CXX_WARN(logger_, "No HMI Handler set");
    return;
  }

  // SmartObject |message| has no way to declare priority for now
  utils::SharedPtr<Message> message_to_send(
      new Message(protocol_handler::MessagePriority::kDefault));
  if (!message_to_send) {
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

  hmi_so_factory().attachSchema(*message, false);
  LOG4CXX_INFO(
      logger_,
      "Attached schema to message, result if valid: " << message->isValid());

#ifdef HMI_DBUS_API
  message_to_send->set_smart_object(*message);
#else
  if (!ConvertSOtoMessage(*message, *message_to_send)) {
    LOG4CXX_WARN(logger_,
                 "Cannot send message to HMI: failed to create string");
    return;
  }
#endif  // HMI_DBUS_API

  messages_to_hmi_.PostMessage(impl::MessageToHmi(message_to_send));
}

bool ApplicationManagerImpl::ManageHMICommand(
    const commands::MessageSharedPtr message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    return false;
  }

  if (IsLowVoltage()) {
    LOG4CXX_WARN(logger_, "Low Voltage is active");
    return false;
  }

  MessageHelper::PrintSmartObject(*message);

  CommandSharedPtr command = HMICommandFactory::CreateCommand(message, *this);
  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create command from smart object");
    return false;
  }

  if ((*message).keyExists(strings::msg_params) &&
      (*message)[strings::msg_params].keyExists(strings::app_id)) {
    const auto connection_key =
        (*message)[strings::msg_params][strings::app_id].asUInt();

    auto app = application(static_cast<uint32_t>(connection_key));
    if (app && IsAppInReconnectMode(app->policy_app_id())) {
      commands_holder_->Suspend(
          app, CommandHolder::CommandType::kHmiCommand, message);
      return true;
    }
  }

  int32_t message_type =
      (*message)[strings::params][strings::message_type].asInt();

  if (kRequest == message_type) {
    LOG4CXX_DEBUG(logger_, "ManageHMICommand");
    request_ctrl_.addHMIRequest(command);
  }

  if (command->Init()) {
    command->Run();
    if (kResponse == message_type) {
      const uint32_t correlation_id =
          (*message)[strings::params][strings::correlation_id].asUInt();
      const int32_t function_id =
          (*message)[strings::params][strings::function_id].asInt();
      request_ctrl_.OnHMIResponse(correlation_id, function_id);
    }
    return true;
  }
  return false;
}

bool ApplicationManagerImpl::Init(resumption::LastState& last_state,
                                  media_manager::MediaManager* media_manager) {
  LOG4CXX_TRACE(logger_, "Init application manager");
  const std::string app_storage_folder = get_settings().app_storage_folder();
  if (!InitDirectory(app_storage_folder, TYPE_STORAGE) ||
      !IsReadWriteAllowed(app_storage_folder, TYPE_STORAGE)) {
    return false;
  }
  if (!resume_controller().Init(last_state)) {
    LOG4CXX_ERROR(logger_, "Problem with initialization of resume controller");
    return false;
  }
  hmi_capabilities_->Init(&last_state);

  if (!(file_system::IsWritingAllowed(app_storage_folder) &&
        file_system::IsReadingAllowed(app_storage_folder))) {
    LOG4CXX_ERROR(logger_,
                  "Storage directory doesn't have read/write permissions");
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
      LOG4CXX_ERROR(logger_,
                    "Policy library is not loaded. Check LD_LIBRARY_PATH");
      return false;
    }
    LOG4CXX_INFO(logger_, "Policy library is loaded, now initing PT");
    if (!GetPolicyHandler().InitPolicyTable()) {
      LOG4CXX_ERROR(logger_, "Policy table is not initialized.");
      return false;
    }
  } else {
    LOG4CXX_WARN(logger_,
                 "System is configured to work without policy functionality.");
  }
  media_manager_ = media_manager;

  if (settings_.use_db_for_resumption()) {
    app_launch_dto_.reset(new app_launch::AppLaunchDataDB(settings_));
  } else {
    app_launch_dto_.reset(
        new app_launch::AppLaunchDataJson(settings_, last_state));
  }
  app_launch_ctrl_.reset(new app_launch::AppLaunchCtrlImpl(
      *app_launch_dto_.get(), *this, settings_));

#ifdef SDL_REMOTE_CONTROL
  if (!hmi_handler_) {
    LOG4CXX_ERROR(logger_, "HMI message handler was not initialized");
    return false;
  }
  plugin_manager_.SetServiceHandler(utils::MakeShared<CoreService>(*this));
  plugin_manager_.LoadPlugins(settings_.plugins_folder());
  plugin_manager_.OnServiceStateChanged(
      functional_modules::ServiceState::HMI_ADAPTER_INITIALIZED);
#endif  // SDL_REMOTE_CONTROL

  return true;
}

bool ApplicationManagerImpl::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  stopping_application_mng_lock_.Acquire();
  is_stopping_ = true;
  stopping_application_mng_lock_.Release();
  application_list_update_timer_.Stop();
  try {
    UnregisterAllApplications();
  } catch (...) {
    LOG4CXX_ERROR(logger_,
                  "An error occurred during unregistering applications.");
  }
  request_ctrl_.DestroyThreadpool();

  // for PASA customer policy backup should happen :AllApp(SUSPEND)
  LOG4CXX_DEBUG(logger_, "Unloading policy library.");
  GetPolicyHandler().UnloadPolicyLibrary();

  return true;
}

bool ApplicationManagerImpl::ConvertMessageToSO(
    const Message& message, smart_objects::SmartObject& output) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "\t\t\tMessage to convert: protocol "
                    << message.protocol_version() << "; json "
                    << message.json_message());

  switch (message.protocol_version()) {
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5:
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4:
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3:
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2: {
      const bool conversion_result =
          formatters::CFormatterJsonSDLRPCv2::fromString(
              message.json_message(),
              output,
              message.function_id(),
              message.type(),
              message.correlation_id());

      rpc::ValidationReport report("RPC");

      if (!conversion_result ||
          !mobile_so_factory().attachSchema(output, true) ||
          ((output.validate(&report) != smart_objects::Errors::OK))) {
        LOG4CXX_WARN(logger_,
                     "Failed to parse string to smart object :"
                         << message.json_message());
        utils::SharedPtr<smart_objects::SmartObject> response(
            MessageHelper::CreateNegativeResponse(
                message.connection_key(),
                message.function_id(),
                message.correlation_id(),
                mobile_apis::Result::INVALID_DATA));

        (*response)[strings::msg_params][strings::info] =
            rpc::PrettyFormat(report);
        ManageMobileCommand(response, commands::Command::ORIGIN_SDL);
        return false;
      }
      LOG4CXX_DEBUG(logger_,
                    "Convertion result for sdl object is true function_id "
                        << output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());

      output[strings::params][strings::connection_key] =
          message.connection_key();
      output[strings::params][strings::protocol_version] =
          message.protocol_version();
      if (message.binary_data()) {
        if (message.payload_size() < message.data_size()) {
          LOG4CXX_ERROR(logger_,
                        "Incomplete binary"
                            << " binary size should be  " << message.data_size()
                            << " payload data size is "
                            << message.payload_size());
          utils::SharedPtr<smart_objects::SmartObject> response(
              MessageHelper::CreateNegativeResponse(
                  message.connection_key(),
                  message.function_id(),
                  message.correlation_id(),
                  mobile_apis::Result::INVALID_DATA));
          ManageMobileCommand(response, commands::Command::ORIGIN_SDL);
          return false;
        }
        output[strings::params][strings::binary_data] =
            *(message.binary_data());
      }
      break;
    }
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_HMI: {
#ifdef ENABLE_LOG
      int32_t result =
#endif
          formatters::FormatterJsonRpc::FromString<
              hmi_apis::FunctionID::eType,
              hmi_apis::messageType::eType>(message.json_message(), output);
      LOG4CXX_DEBUG(logger_,
                    "Convertion result: "
                        << result << " function id "
                        << output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
      if (!hmi_so_factory().attachSchema(output, false)) {
        LOG4CXX_WARN(logger_, "Failed to attach schema to object.");
        return false;
      }

      rpc::ValidationReport report("RPC");

      if (output.validate(&report) != smart_objects::Errors::OK) {
        LOG4CXX_ERROR(logger_,
                      "Incorrect parameter from HMI"
                          << rpc::PrettyFormat(report));

        output.erase(strings::msg_params);
        output[strings::params][hmi_response::code] =
            hmi_apis::Common_Result::INVALID_DATA;
        output[strings::msg_params][strings::info] = rpc::PrettyFormat(report);
        return false;
      }
      break;
    }
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1: {
      static NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra v1_shema;

      if (message.function_id() == 0 || message.type() == kUnknownType) {
        LOG4CXX_ERROR(logger_, "Message received: UNSUPPORTED_VERSION");

        int32_t conversation_result =
            formatters::CFormatterJsonSDLRPCv1::fromString<
                NsSmartDeviceLinkRPC::V1::FunctionID::eType,
                NsSmartDeviceLinkRPC::V1::messageType::eType>(
                message.json_message(), output);

        if (formatters::CFormatterJsonSDLRPCv1::kSuccess ==
            conversation_result) {
          smart_objects::SmartObject params = smart_objects::SmartObject(
              smart_objects::SmartType::SmartType_Map);

          output[strings::params][strings::message_type] =
              NsSmartDeviceLinkRPC::V1::messageType::response;
          output[strings::params][strings::connection_key] =
              message.connection_key();

          output[strings::msg_params] = smart_objects::SmartObject(
              smart_objects::SmartType::SmartType_Map);
          output[strings::msg_params][strings::success] = false;
          output[strings::msg_params][strings::result_code] =
              NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_VERSION;

          smart_objects::SmartObjectSPtr msg_to_send =
              new smart_objects::SmartObject(output);
          v1_shema.attachSchema(*msg_to_send, false);
          SendMessageToMobile(msg_to_send);
          return false;
        }
      }
      break;
    }
    default:
      LOG4CXX_WARN(logger_,
                   "Application used unsupported protocol :"
                       << message.protocol_version() << ".");
      return false;
  }

  LOG4CXX_DEBUG(logger_, "Successfully parsed message into smart object");
  return true;
}

bool ApplicationManagerImpl::ConvertSOtoMessage(
    const smart_objects::SmartObject& message, Message& output) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (smart_objects::SmartType_Null == message.getType() ||
      smart_objects::SmartType_Invalid == message.getType()) {
    LOG4CXX_WARN(logger_, "Invalid smart object received.");
    return false;
  }

  LOG4CXX_DEBUG(
      logger_,
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
        if (!formatters::CFormatterJsonSDLRPCv1::toString(message,
                                                          output_string)) {
          LOG4CXX_WARN(logger_, "Failed to serialize smart object");
          return false;
        }
        output.set_protocol_version(
            protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1);
      } else {
        if (!formatters::CFormatterJsonSDLRPCv2::toString(message,
                                                          output_string)) {
          LOG4CXX_WARN(logger_, "Failed to serialize smart object");
          return false;
        }
        output.set_protocol_version(
            static_cast<protocol_handler::MajorProtocolVersion>(
                protocol_version));
      }

      break;
    }
    case 1: {
      if (!formatters::FormatterJsonRpc::ToString(message, output_string)) {
        LOG4CXX_WARN(logger_, "Failed to serialize smart object");
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

  LOG4CXX_DEBUG(logger_, "Convertion result: " << output_string);

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

  LOG4CXX_DEBUG(logger_, "Successfully parsed smart object into message");
  return true;
}

MessageValidationResult ApplicationManagerImpl::ValidateMessageBySchema(
    const Message& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject so;
  using namespace protocol_handler;
  switch (message.protocol_version()) {
    case MajorProtocolVersion::PROTOCOL_VERSION_5:
    case MajorProtocolVersion::PROTOCOL_VERSION_4:
    case MajorProtocolVersion::PROTOCOL_VERSION_3:
    case MajorProtocolVersion::PROTOCOL_VERSION_2: {
      const bool conversion_result =
          formatters::CFormatterJsonSDLRPCv2::fromString(
              message.json_message(),
              so,
              message.function_id(),
              message.type(),
              message.correlation_id());
      if (!conversion_result) {
        return INVALID_JSON;
      }

      if (!mobile_so_factory().attachSchema(so, true)) {
        return INVALID_METADATA;
      }
      rpc::ValidationReport report("RPC");
      if (so.validate(&report) != smart_objects::Errors::OK) {
        LOG4CXX_WARN(logger_,
                     "validate() failed for Mobile message - "
                         << rpc::PrettyFormat(report));
        return SCHEMA_MISMATCH;
      }
      break;
    }
    case MajorProtocolVersion::PROTOCOL_VERSION_HMI: {
      const int32_t conversion_result = formatters::FormatterJsonRpc::
          FromString<hmi_apis::FunctionID::eType, hmi_apis::messageType::eType>(
              message.json_message(), so);
      if (0 != conversion_result) {
        LOG4CXX_WARN(logger_,
                     "Failed to parse json from HMI: " << conversion_result);
        return INVALID_JSON;
      }

      if (!hmi_so_factory().attachSchema(so, true)) {
        return INVALID_METADATA;
      }

      rpc::ValidationReport report("RPC");
      if (so.validate(&report) != smart_objects::Errors::OK) {
        LOG4CXX_WARN(logger_,
                     "validate() failed for HMI message - "
                         << rpc::PrettyFormat(report));
        return SCHEMA_MISMATCH;
      }
      break;
    }
    default: { return UNSUPPORTED_PROTOCOL; }
  }
  return SUCCESS;
}

utils::SharedPtr<Message> ApplicationManagerImpl::ConvertRawMsgToMessage(
    const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(message);
  utils::SharedPtr<Message> outgoing_message;

  LOG4CXX_DEBUG(logger_, "Service type." << message->service_type());
  if (message->service_type() != protocol_handler::kRpc &&
      message->service_type() != protocol_handler::kBulk) {
    // skip this message, not under handling of ApplicationManager
    LOG4CXX_TRACE(logger_, "Skipping message; not the under AM handling.");
    return outgoing_message;
  }

  Message* convertion_result =
      MobileMessageHandler::HandleIncomingMessageProtocol(message);

  if (convertion_result) {
    outgoing_message = convertion_result;
  } else {
    LOG4CXX_ERROR(logger_, "Received invalid message");
  }
  return outgoing_message;
}

void ApplicationManagerImpl::ProcessMessageFromMobile(
    const utils::SharedPtr<Message> message) {
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef TELEMETRY_MONITOR
  AMTelemetryObserver::MessageMetricSharedPtr metric(
      new AMTelemetryObserver::MessageMetric());
  metric->begin = date_time::DateTime::getCurrentTime();
#endif  // TELEMETRY_MONITOR
  smart_objects::SmartObjectSPtr so_from_mobile =
      utils::MakeShared<smart_objects::SmartObject>();

  DCHECK_OR_RETURN_VOID(so_from_mobile);
  if (!so_from_mobile) {
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

  if (!ConvertMessageToSO(*message, *so_from_mobile)) {
    LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
    return;
  }
#ifdef TELEMETRY_MONITOR
  metric->message = so_from_mobile;
#endif  // TELEMETRY_MONITOR

  if (!ManageMobileCommand(so_from_mobile, commands::Command::ORIGIN_MOBILE)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
#ifdef TELEMETRY_MONITOR
  metric->end = date_time::DateTime::getCurrentTime();
  if (metric_observer_) {
    metric_observer_->OnMessage(metric);
  }
#endif  // TELEMETRY_MONITOR
}

void ApplicationManagerImpl::ProcessMessageFromHMI(
    const utils::SharedPtr<Message> message) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr smart_object(new smart_objects::SmartObject);

  if (!smart_object) {
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

#ifdef HMI_DBUS_API
  *smart_object = message->smart_object();
#else
  if (!ConvertMessageToSO(*message, *smart_object)) {
    if (application_manager::MessageType::kResponse ==
        (*smart_object)[strings::params][strings::message_type].asInt()) {
      (*smart_object).erase(strings::msg_params);
      (*smart_object)[strings::params][hmi_response::code] =
          hmi_apis::Common_Result::INVALID_DATA;
      (*smart_object)[strings::msg_params][strings::info] =
          std::string("Received invalid data on HMI response");
    } else {
      LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
      return;
    }
  }
#endif  // HMI_DBUS_API

  LOG4CXX_DEBUG(logger_, "Converted message, trying to create hmi command");
  if (!ManageHMICommand(smart_object)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
}

hmi_apis::HMI_API& ApplicationManagerImpl::hmi_so_factory() {
  if (!hmi_so_factory_) {
    hmi_so_factory_ = new hmi_apis::HMI_API;
    if (!hmi_so_factory_) {
      LOG4CXX_ERROR(logger_, "Out of memory");
      NOTREACHED();
    }
  }
  return *hmi_so_factory_;
}

mobile_apis::MOBILE_API& ApplicationManagerImpl::mobile_so_factory() {
  if (!mobile_so_factory_) {
    mobile_so_factory_ = new mobile_apis::MOBILE_API;
    if (!mobile_so_factory_) {
      LOG4CXX_ERROR(logger_, "Out of memory.");
      NOTREACHED();
    }
  }
  return *mobile_so_factory_;
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
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app_data.keyExists(json::languages)) {
    LOG4CXX_WARN(logger_, "\"languages\" not exists");
    return;
  }

  const HMICapabilities& hmi_cap = hmi_capabilities();
  std::string cur_vr_lang(
      MessageHelper::CommonLanguageToString(hmi_cap.active_vr_language()));
  const SmartObject& languages = app_data[json::languages];

  std::transform(
      cur_vr_lang.begin(), cur_vr_lang.end(), cur_vr_lang.begin(), ::toupper);

  ssize_t default_idx = -1;
  ssize_t specific_idx = -1;

  const size_t size = languages.length();
  for (size_t idx = 0; idx < size; ++idx) {
    if (languages[idx].keyExists(cur_vr_lang)) {
      LOG4CXX_DEBUG(logger_, "Found active HMI language " << cur_vr_lang);
      specific_idx = idx;
    } else if (languages[idx].keyExists(json::default_)) {
      LOG4CXX_DEBUG(logger_, "Found default language");
      default_idx = idx;
    }
  }

  if ((-1 == specific_idx) && (-1 == default_idx)) {
    LOG4CXX_DEBUG(logger_, "No suitable language found");
    return;
  }

  if (app_data[json::languages][specific_idx][cur_vr_lang].keyExists(
          json::ttsName)) {
    LOG4CXX_DEBUG(logger_, "Get ttsName from " << cur_vr_lang << " language");
    ttsName =
        app_data[json::languages][specific_idx][cur_vr_lang][json::ttsName];
  } else {
    LOG4CXX_DEBUG(logger_,
                  "No data for ttsName for " << cur_vr_lang << " language");
  }

  if (app_data[json::languages][specific_idx][cur_vr_lang].keyExists(
          json::vrSynonyms)) {
    LOG4CXX_DEBUG(logger_,
                  "Get vrSynonyms from " << cur_vr_lang << " language");
    vrSynonym =
        app_data[json::languages][specific_idx][cur_vr_lang][json::vrSynonyms];
  } else {
    LOG4CXX_DEBUG(logger_,
                  "No data for vrSynonyms for " << cur_vr_lang << " language");
  }
}

void ApplicationManagerImpl::CreateApplications(SmartArray& obj_array,
                                                const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace policy;

  const std::size_t arr_size(obj_array.size());
  for (std::size_t idx = 0; idx < arr_size; ++idx) {
    const SmartObject& app_data = obj_array[idx];

    if (!(app_data.keyExists(json::name) && app_data.keyExists(json::appId))) {
      LOG4CXX_DEBUG(logger_, "The entry in query apps json is not valid");
      continue;
    }

    const std::string policy_app_id(app_data[json::appId].asString());
    ApplicationSharedPtr registered_app =
        application_by_policy_id(policy_app_id);
    if (registered_app) {
      LOG4CXX_DEBUG(logger_,
                    "Application with the same id: "
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

    connection_handler::DeviceHandle device_id = 0;

    if (-1 ==
        connection_handler().get_session_observer().GetDataOnSessionKey(
            connection_key, NULL, NULL, &device_id)) {
      LOG4CXX_ERROR(logger_,
                    "Failed to create application: no connection info.");
      continue;
    }

    std::string device_mac;
    connection_handler().get_session_observer().GetDataOnDeviceID(
        device_id, NULL, NULL, &device_mac, NULL);

    const uint32_t hmi_app_id =
        resume_controller().IsApplicationSaved(policy_app_id, device_mac)
            ? resume_controller().GetHMIApplicationID(policy_app_id, device_mac)
            : GenerateNewHMIAppID();

    // AppId = 0 because this is query_app(provided by hmi for download, but not
    // yet registered)
    ApplicationSharedPtr app(
        new ApplicationImpl(0,
                            policy_app_id,
                            device_mac,
                            device_id,
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

    sync_primitives::AutoLock lock(apps_to_register_list_lock_);
    LOG4CXX_DEBUG(
        logger_, "apps_to_register_ size before: " << apps_to_register_.size());
    apps_to_register_.insert(app);
    LOG4CXX_DEBUG(logger_,
                  "apps_to_register_ size after: " << apps_to_register_.size());
  }
}

void ApplicationManagerImpl::ProcessQueryApp(
    const smart_objects::SmartObject& sm_object,
    const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace policy;

  if (!sm_object.keyExists(json::response)) {
    LOG4CXX_DEBUG(logger_, "The response key is not exists.");
    return;
  }

  SmartArray* obj_array = sm_object[json::response].asArray();
  if (NULL != obj_array) {
    CreateApplications(*obj_array, connection_key);
    SendUpdateAppList();

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
  metric_observer_ = observer;
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
  LOG4CXX_AUTO_TRACE(logger_);
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
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_TRACE(logger_, "reason = " << reason);
  unregister_reason_ = reason;
}

void ApplicationManagerImpl::HeadUnitReset(
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  LOG4CXX_AUTO_TRACE(logger_);
  stopping_application_mng_lock_.Acquire();
  is_stopping_ = true;
  stopping_application_mng_lock_.Release();
  switch (reason) {
    case mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET: {
      LOG4CXX_TRACE(logger_, "Performing MASTER_RESET");
      UnregisterAllApplications();
      GetPolicyHandler().ResetPolicyTable();
      GetPolicyHandler().UnloadPolicyLibrary();

      resume_controller().StopSavePersistentDataTimer();

      const std::string storage_folder = get_settings().app_storage_folder();
      file_system::RemoveDirectory(storage_folder, true);
      ClearAppsPersistentData();
      break;
    }
    case mobile_api::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS: {
      LOG4CXX_TRACE(logger_, "Performing FACTORY_DEFAULTS");
      GetPolicyHandler().ClearUserConsent();

      resume_controller().StopSavePersistentDataTimer();

      ClearAppsPersistentData();
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Bad AppInterfaceUnregisteredReason");
      return;
    }
  }
}

void ApplicationManagerImpl::ClearAppsPersistentData() {
  LOG4CXX_AUTO_TRACE(logger_);
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
    LOG4CXX_TRACE(logger_, "Removing : " << item_to_remove);
    if (file_system::IsDirectory(item_to_remove)) {
      LOG4CXX_TRACE(logger_,
                    "Removal result : " << file_system::RemoveDirectory(
                        item_to_remove, true));
    } else {
      LOG4CXX_TRACE(
          logger_,
          "Removal result : " << file_system::DeleteFile(item_to_remove));
    }
  }

  const std::string apps_icons_folder = get_settings().app_icons_folder();
  if (storage_folder != apps_icons_folder) {
    file_system::RemoveDirectory(apps_icons_folder, true);
  }
}

void ApplicationManagerImpl::SendOnSDLClose() {
  LOG4CXX_AUTO_TRACE(logger_);

  // must be sent to PASA HMI on shutdown synchronously
  smart_objects::SmartObjectSPtr msg =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*msg)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSDLClose;
  (*msg)[strings::params][strings::message_type] = MessageType::kNotification;
  (*msg)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
  (*msg)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;

  if (!msg) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  // SmartObject |message| has no way to declare priority for now
  utils::SharedPtr<Message> message_to_send(
      new Message(protocol_handler::MessagePriority::kDefault));

  hmi_so_factory().attachSchema(*msg, false);
  LOG4CXX_DEBUG(
      logger_,
      "Attached schema to message, result if valid: " << msg->isValid());

#ifdef HMI_DBUS_API
  message_to_send->set_smart_object(*msg);
#else
  if (!ConvertSOtoMessage(*msg, *message_to_send)) {
    LOG4CXX_WARN(logger_,
                 "Cannot send message to HMI: failed to create string");
    return;
  }
#endif  // HMI_DBUS_API

  if (!hmi_handler_) {
    LOG4CXX_WARN(logger_, "No HMI Handler set");
    return;
  }

  hmi_handler_->SendMessageToHMI(message_to_send);
}

void ApplicationManagerImpl::UnregisterAllApplications() {
  LOG4CXX_DEBUG(logger_, "Unregister reason  " << unregister_reason_);

  hmi_cooperating_ = false;
  bool is_ignition_off = false;
  using namespace mobile_api::AppInterfaceUnregisteredReason;
  using namespace helpers;

  is_ignition_off =
      Compare<eType, EQ, ONE>(unregister_reason_, IGNITION_OFF, INVALID_ENUM);

  bool is_unexpected_disconnect = Compare<eType, NEQ, ALL>(
      unregister_reason_, IGNITION_OFF, MASTER_RESET, FACTORY_DEFAULTS);

  ClearTTSGlobalPropertiesList();

  {  // A local scope to limit accessor's lifetime and release app list lock.
    DataAccessor<ApplicationSet> accessor = applications();
    ApplicationSetConstIt it = accessor.GetData().begin();
    while (it != accessor.GetData().end()) {
      ApplicationSharedPtr app_to_remove = *it;
      ManageMobileCommand(
          MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
              app_to_remove->app_id(), unregister_reason_),
          commands::Command::ORIGIN_SDL);
      UnregisterApplication(app_to_remove->app_id(),
                            mobile_apis::Result::INVALID_ENUM,
                            is_ignition_off,
                            is_unexpected_disconnect);
      connection_handler().CloseSession(app_to_remove->app_id(),
                                        connection_handler::kCommon);
      it = accessor.GetData().begin();
    }
  }
  if (is_ignition_off) {
    resume_controller().OnIgnitionOff();
  }
  request_ctrl_.terminateAllHMIRequests();
}

void ApplicationManagerImpl::RemoveAppsWaitingForRegistration(
    const connection_handler::DeviceHandle handle) {
  DevicePredicate device_finder(handle);
  apps_to_register_list_lock_.Acquire();
  AppsWaitRegistrationSet::iterator it_app = std::find_if(
      apps_to_register_.begin(), apps_to_register_.end(), device_finder);

  while (apps_to_register_.end() != it_app) {
    LOG4CXX_DEBUG(logger_,
                  "Waiting app: " << (*it_app)->name().c_str()
                                  << " is removed.");
    apps_to_register_.erase(it_app);
    it_app = std::find_if(
        apps_to_register_.begin(), apps_to_register_.end(), device_finder);
  }

  apps_to_register_list_lock_.Release();
}

void ApplicationManagerImpl::UnregisterApplication(
    const uint32_t& app_id,
    mobile_apis::Result::eType reason,
    bool is_resuming,
    bool is_unexpected_disconnect) {
  LOG4CXX_DEBUG(logger_,
                "app_id = " << app_id << "; reason = " << reason
                            << "; is_resuming = " << is_resuming
                            << "; is_unexpected_disconnect = "
                            << is_unexpected_disconnect);
  size_t subscribed_for_way_points_app_count = 0;

  // SDL sends UnsubscribeWayPoints only for last application
  {
    sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
    subscribed_for_way_points_app_count =
        subscribed_way_points_apps_list_.size();
  }
  if (1 == subscribed_for_way_points_app_count) {
    LOG4CXX_ERROR(logger_, "Send UnsubscribeWayPoints");
    MessageHelper::SendUnsubscribedWayPoints(*this);
  }

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
          LOG4CXX_DEBUG(
              logger_,
              "INSERT: " << GetHashedAppID(app_id, app_ptr->policy_app_id()));
          forbidden_applications.insert(
              GetHashedAppID(app_id, app_ptr->policy_app_id()));
        }
      }
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown unregister reason " << reason);
      break;
    }
  }
  ApplicationSharedPtr app_to_remove;
  connection_handler::DeviceHandle handle = 0;
  {
    sync_primitives::AutoLock lock(applications_list_lock_);
    auto it_app = applications_.begin();
    while (applications_.end() != it_app) {
      if (app_id == (*it_app)->app_id()) {
        app_to_remove = *it_app;
        applications_.erase(it_app++);
      } else {
        ++it_app;
      }
    }
    if (!app_to_remove) {
      LOG4CXX_ERROR(logger_, "Cant find application with app_id = " << app_id);

      // Just to terminate RAI in case of connection is dropped (rare case)
      // App won't be unregistered since RAI has not been started yet
      LOG4CXX_DEBUG(logger_, "Trying to terminate possible RAI request.");
      request_ctrl_.terminateAppRequests(app_id);

      return;
    }
    if (is_resuming) {
      resume_controller().SaveApplication(app_to_remove);
    } else {
      resume_controller().RemoveApplicationFromSaved(app_to_remove);
    }
    (hmi_capabilities_->get_hmi_language_handler())
        .OnUnregisterApplication(app_id);
    AppV4DevicePredicate finder(handle);
    ApplicationSharedPtr app = FindApp(applications(), finder);
    if (!app) {
      LOG4CXX_DEBUG(
          logger_, "There is no more SDL4 apps with device handle: " << handle);

      RemoveAppsWaitingForRegistration(handle);
      SendUpdateAppList();
    }
  }

  commands_holder_->Clear(app_to_remove);

  if (audio_pass_thru_active_) {
    // May be better to put this code in MessageHelper?
    EndAudioPassThrough();
    StopAudioPassThru(app_id);
    MessageHelper::SendStopAudioPathThru(*this);
  }

#ifdef SDL_REMOTE_CONTROL
  plugin_manager_.OnApplicationEvent(
      functional_modules::ApplicationEvent::kApplicationUnregistered,
      app_to_remove);
#endif

  MessageHelper::SendOnAppUnregNotificationToHMI(
      app_to_remove, is_unexpected_disconnect, *this);
  request_ctrl_.terminateAppRequests(app_id);
  return;
}

void ApplicationManagerImpl::OnAppUnauthorized(const uint32_t& app_id) {
  connection_handler().CloseSession(app_id,
                                    connection_handler::kUnauthorizedApp);
}

void ApplicationManagerImpl::Handle(const impl::MessageFromMobile message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }
  sync_primitives::AutoLock lock(stopping_application_mng_lock_);
  if (is_stopping_) {
    LOG4CXX_INFO(logger_, "Application manager is stopping");
    return;
  }
#ifdef SDL_REMOTE_CONTROL
  if (plugin_manager_.IsMessageForPlugin(message)) {
    if (functional_modules::ProcessResult::PROCESSED ==
        plugin_manager_.ProcessMessage(message)) {
      LOG4CXX_INFO(logger_, "Message is processed by plugin.");
      return;
    }
  }
#endif
  ProcessMessageFromMobile(message);
}

void ApplicationManagerImpl::Handle(const impl::MessageToMobile message) {
  if (!protocol_handler_) {
    LOG4CXX_WARN(logger_,
                 "Protocol Handler is not set; cannot send message to mobile.");
    return;
  }

  utils::SharedPtr<protocol_handler::RawMessage> rawMessage =
      MobileMessageHandler::HandleOutgoingMessageProtocol(message);

  if (!rawMessage) {
    LOG4CXX_ERROR(logger_, "Failed to create raw message.");
    return;
  }

  bool is_final = message.is_final;
  bool close_session = false;
  if (is_final) {
    if (1 < connection_handler().GetConnectionSessionsCount(
                message->connection_key())) {
      is_final = false;
      close_session = true;
    }
  }

  protocol_handler_->SendMessageToMobileApp(rawMessage, is_final);
  LOG4CXX_INFO(logger_, "Message for mobile given away");

  if (close_session) {
    connection_handler().CloseSession(message->connection_key(),
                                      connection_handler::kCommon);
  }
}

void ApplicationManagerImpl::Handle(const impl::MessageFromHmi message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }

#ifdef SDL_REMOTE_CONTROL
  if (plugin_manager_.IsHMIMessageForPlugin(message)) {
    functional_modules::ProcessResult result =
        plugin_manager_.ProcessHMIMessage(message);
    if (functional_modules::ProcessResult::PROCESSED == result ||
        functional_modules::ProcessResult::FAILED == result) {
      LOG4CXX_INFO(logger_, "Message is processed by plugin.");
      return;
    }
  }
#endif

  ProcessMessageFromHMI(message);
}

void ApplicationManagerImpl::Handle(const impl::MessageToHmi message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!hmi_handler_) {
    LOG4CXX_ERROR(logger_, "Observer is not set for HMIMessageHandler");
    return;
  }

  hmi_handler_->SendMessageToHMI(message);
  LOG4CXX_INFO(logger_, "Message for HMI given away");
}

void ApplicationManagerImpl::Handle(const impl::AudioData message) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr on_audio_pass =
      new smart_objects::SmartObject();

  if (!on_audio_pass) {
    LOG4CXX_ERROR(logger_, "OnAudioPassThru NULL pointer");
    return;
  }

  LOG4CXX_DEBUG(logger_, "Fill smart object");

  (*on_audio_pass)[strings::params][strings::message_type] =
      application_manager::MessageType::kNotification;

  (*on_audio_pass)[strings::params][strings::connection_key] =
      static_cast<int32_t>(message.session_key);
  (*on_audio_pass)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnAudioPassThruID;

  LOG4CXX_DEBUG(logger_, "Fill binary data");
  // binary data
  (*on_audio_pass)[strings::params][strings::binary_data] =
      smart_objects::SmartObject(message.binary_data);

  LOG4CXX_DEBUG(logger_, "After fill binary data");
  LOG4CXX_DEBUG(logger_, "Send data");
  CommandSharedPtr command(MobileCommandFactory::CreateCommand(
      on_audio_pass, commands::Command::ORIGIN_SDL, *this));
  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create mobile command from smart object");
    return;
  }

  if (command->Init()) {
    command->Run();
    command->CleanUp();
  }
}

mobile_apis::Result::eType ApplicationManagerImpl::CheckPolicyPermissions(
    const ApplicationSharedPtr app,
    const std::string& function_id,
    const RPCParams& rpc_params,
    CommandParametersPermissions* params_permissions) {
  LOG4CXX_INFO(logger_, "CheckPolicyPermissions");
  // TODO(AOleynik): Remove check of policy_enable, when this flag will be
  // unused in config file
  if (!GetPolicyHandler().PolicyEnabled()) {
    return mobile_apis::Result::SUCCESS;
  }

  DCHECK(app);
  policy::CheckPermissionResult result;
  GetPolicyHandler().CheckPermissions(app, function_id, rpc_params, result);

  if (NULL != params_permissions) {
    params_permissions->allowed_params = result.list_of_allowed_params;
    params_permissions->disallowed_params = result.list_of_disallowed_params;
    params_permissions->undefined_params = result.list_of_undefined_params;
  }

  if (app->hmi_level() == mobile_apis::HMILevel::HMI_NONE &&
      function_id != MessageHelper::StringifiedFunctionID(
                         mobile_apis::FunctionID::UnregisterAppInterfaceID)) {
    if (result.hmi_level_permitted != policy::kRpcAllowed) {
      app->usage_report().RecordRpcSentInHMINone();
    }
  }

#ifdef ENABLE_LOG
  const std::string log_msg =
      "Application: " + app->policy_app_id() + ", RPC: " + function_id +
      ", HMI status: " + MessageHelper::StringifiedHMILevel(app->hmi_level());
#endif  // ENABLE_LOG
  if (result.hmi_level_permitted != policy::kRpcAllowed) {
    LOG4CXX_WARN(logger_, "Request is blocked by policies. " << log_msg);

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
  LOG4CXX_DEBUG(logger_, "Request is allowed by policies. " << log_msg);
  return mobile_api::Result::SUCCESS;
}

void ApplicationManagerImpl::OnLowVoltage() {
  LOG4CXX_AUTO_TRACE(logger_);
  is_low_voltage_ = true;
  request_ctrl_.OnLowVoltage();
}

bool ApplicationManagerImpl::IsLowVoltage() {
  LOG4CXX_TRACE(logger_, "result: " << is_low_voltage_);
  return is_low_voltage_;
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

bool ApplicationManagerImpl::HMILevelAllowsStreaming(
    uint32_t app_id, protocol_handler::ServiceType service_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis::HMILevel;
  using namespace helpers;

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return false;
  }
  return Compare<eType, EQ, ONE>(app->hmi_level(), HMI_FULL, HMI_LIMITED);
}

bool ApplicationManagerImpl::CanAppStream(
    uint32_t app_id, protocol_handler::ServiceType service_type) const {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return false;
  }

  bool is_allowed = false;
  if (ServiceType::kMobileNav == service_type) {
    is_allowed = app->video_streaming_allowed();
  } else if (ServiceType::kAudio == service_type) {
    is_allowed = app->audio_streaming_allowed();
  } else {
    LOG4CXX_WARN(logger_, "Unsupported service_type " << service_type);
  }

  return HMILevelAllowsStreaming(app_id, service_type) && is_allowed;
}

void ApplicationManagerImpl::ForbidStreaming(uint32_t app_id) {
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;

  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    LOG4CXX_DEBUG(
        logger_,
        "There is no navi or projection application with id: " << app_id);
    return;
  }

  if (navi_app_to_stop_.end() != std::find(navi_app_to_stop_.begin(),
                                           navi_app_to_stop_.end(),
                                           app_id) ||
      navi_app_to_end_stream_.end() !=
          std::find(navi_app_to_end_stream_.begin(),
                    navi_app_to_end_stream_.end(),
                    app_id)) {
    return;
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
    ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app_id, PROTOCOL_VIOLATION),
        commands::Command::ORIGIN_SDL);
    UnregisterApplication(app_id, ABORTED);
    return;
  }
  EndNaviServices(app_id);
}

void ApplicationManagerImpl::OnAppStreaming(
    uint32_t app_id, protocol_handler::ServiceType service_type, bool state) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    LOG4CXX_DEBUG(
        logger_,
        " There is no navi or projection application with id: " << app_id);
    return;
  }
  DCHECK_OR_RETURN_VOID(media_manager_);

  if (state) {
    state_ctrl_.OnNaviStreamingStarted();
    media_manager_->StartStreaming(app_id, service_type);
  } else {
    media_manager_->StopStreaming(app_id, service_type);
    state_ctrl_.OnNaviStreamingStopped();
  }
}

void ApplicationManagerImpl::EndNaviServices(uint32_t app_id) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    LOG4CXX_DEBUG(
        logger_,
        "There is no navi or projection application with id: " << app_id);
    return;
  }

  bool end_video = false;
  bool end_audio = false;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() == it) {
      LOG4CXX_ERROR(logger_, "No info about navi servicies for app");
      return;
    }
    end_video = it->second.first;
    end_audio = it->second.second;
  }

  if (connection_handler_) {
    if (end_video) {
      LOG4CXX_DEBUG(logger_, "Going to end video service");
      connection_handler().SendEndService(app_id, ServiceType::kMobileNav);
      app->StopStreamingForce(ServiceType::kMobileNav);
    }
    if (end_audio) {
      LOG4CXX_DEBUG(logger_, "Going to end audio service");
      connection_handler().SendEndService(app_id, ServiceType::kAudio);
      app->StopStreamingForce(ServiceType::kAudio);
    }
    DisallowStreaming(app_id);

    navi_app_to_stop_.push_back(app_id);

    TimerSPtr close_timer(utils::MakeShared<timer::Timer>(
        "CloseNaviAppTimer",
        new TimerTaskImpl<ApplicationManagerImpl>(
            this, &ApplicationManagerImpl::CloseNaviApp)));
    close_timer->Start(navi_close_app_timeout_, timer::kSingleShot);

    sync_primitives::AutoLock lock(timer_pool_lock_);
    timer_pool_.push_back(close_timer);
  }
}

void ApplicationManagerImpl::OnHMILevelChanged(
    uint32_t app_id,
    mobile_apis::HMILevel::eType from,
    mobile_apis::HMILevel::eType to) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis::HMILevel;
  using namespace helpers;

  if (from == to) {
    LOG4CXX_TRACE(logger_, "HMILevel from = to");
    return;
  }

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    LOG4CXX_ERROR(logger_, "Navi/Projection application not found");
    return;
  }

  if (to == HMI_FULL || to == HMI_LIMITED) {
    LOG4CXX_TRACE(logger_, "HMILevel to FULL or LIMITED");
    if (from == HMI_BACKGROUND) {
      LOG4CXX_TRACE(logger_, "HMILevel from BACKGROUND");
      AllowStreaming(app_id);
    }
  } else if (to == HMI_BACKGROUND) {
    LOG4CXX_TRACE(logger_, "HMILevel to BACKGROUND");
    if (from == HMI_FULL || from == HMI_LIMITED) {
      LOG4CXX_TRACE(logger_, "HMILevel from FULL or LIMITED");
      navi_app_to_end_stream_.push_back(app_id);
      TimerSPtr end_stream_timer(utils::MakeShared<timer::Timer>(
          "AppShouldFinishStreaming",
          new TimerTaskImpl<ApplicationManagerImpl>(
              this, &ApplicationManagerImpl::EndNaviStreaming)));
      end_stream_timer->Start(navi_end_stream_timeout_, timer::kPeriodic);

      sync_primitives::AutoLock lock(timer_pool_lock_);
      timer_pool_.push_back(end_stream_timer);
    }
  } else if (to == HMI_NONE) {
    LOG4CXX_TRACE(logger_, "HMILevel to NONE");
    if (from == HMI_FULL || from == HMI_LIMITED || from == HMI_BACKGROUND) {
      EndNaviServices(app_id);
    }
  }
}

void ApplicationManagerImpl::SendHMIStatusNotification(
    const utils::SharedPtr<Application> app) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
      static_cast<int32_t>(mobile_api::FunctionID::OnHMIStatusID);

  message[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);

  message[strings::params][strings::connection_key] =
      static_cast<int32_t>(app->app_id());

  message[strings::msg_params][strings::hmi_level] =
      static_cast<int32_t>(app->hmi_level());

  message[strings::msg_params][strings::audio_streaming_state] =
      static_cast<int32_t>(app->audio_streaming_state());

  message[strings::msg_params][strings::video_streaming_state] =
      static_cast<int32_t>(app->video_streaming_state());

  message[strings::msg_params][strings::system_context] =
      static_cast<int32_t>(app->system_context());

  ManageMobileCommand(notification, commands::Command::ORIGIN_SDL);
}

void ApplicationManagerImpl::ClearTimerPool() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<TimerSPtr> new_timer_pool;

  sync_primitives::AutoLock lock(timer_pool_lock_);
  new_timer_pool.push_back(timer_pool_[0]);

  for (size_t i = 1; i < timer_pool_.size(); ++i) {
    if (timer_pool_[i]->is_running()) {
      new_timer_pool.push_back(timer_pool_[i]);
    }
  }

  timer_pool_.swap(new_timer_pool);
  new_timer_pool.clear();
}

void ApplicationManagerImpl::CloseNaviApp() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;
  DCHECK_OR_RETURN_VOID(!navi_app_to_stop_.empty());
  uint32_t app_id = navi_app_to_stop_.front();
  navi_app_to_stop_.pop_front();

  bool unregister = false;
  {
    sync_primitives::AutoLock lock(navi_service_status_lock_);

    NaviServiceStatusMap::iterator it = navi_service_status_.find(app_id);
    if (navi_service_status_.end() != it) {
      if (it->second.first || it->second.second) {
        unregister = true;
      }
    }
  }
  if (unregister) {
    LOG4CXX_INFO(logger_,
                 "App haven't answered for EndService. Unregister it.");
    ManageMobileCommand(
        MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
            app_id, PROTOCOL_VIOLATION),
        commands::Command::ORIGIN_SDL);
    UnregisterApplication(app_id, ABORTED);
  }
}

void ApplicationManagerImpl::EndNaviStreaming() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;

  if (!navi_app_to_end_stream_.empty()) {
    const uint32_t app_id = navi_app_to_end_stream_.front();
    navi_app_to_end_stream_.pop_front();

    if (navi_app_to_stop_.end() ==
        std::find(navi_app_to_stop_.begin(), navi_app_to_stop_.end(), app_id)) {
      DisallowStreaming(app_id);
    }
  }
}

void ApplicationManagerImpl::DisallowStreaming(uint32_t app_id) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    LOG4CXX_ERROR(logger_, "Navi/Projection application not found");
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
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!app || (!app->is_navi() && !app->mobile_projection_enabled())) {
    LOG4CXX_ERROR(logger_, "Navi/Projection application not found");
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
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(reregister_wait_list_lock_);
  return reregister_wait_list_.end() !=
         std::find_if(reregister_wait_list_.begin(),
                      reregister_wait_list_.end(),
                      std::bind1st(std::ptr_fun(&policy_app_id_comparator),
                                   policy_app_id));
}

policy::DeviceConsent ApplicationManagerImpl::GetUserConsentForDevice(
    const std::string& device_id) const {
  return GetPolicyHandler().GetUserConsentForDevice(device_id);
}

void ApplicationManagerImpl::OnWakeUp() {
  LOG4CXX_AUTO_TRACE(logger_);
  is_low_voltage_ = false;
  request_ctrl_.OnWakeUp();
}

mobile_apis::Result::eType ApplicationManagerImpl::SaveBinary(
    const std::vector<uint8_t>& binary_data,
    const std::string& file_path,
    const std::string& file_name,
    const int64_t offset) {
  LOG4CXX_DEBUG(logger_,
                "SaveBinaryWithOffset  binary_size = "
                    << binary_data.size() << " offset = " << offset);

  if (binary_data.size() > file_system::GetAvailableDiskSpace(file_path)) {
    LOG4CXX_ERROR(logger_, "Out of free disc space.");
    return mobile_apis::Result::OUT_OF_MEMORY;
  }

  const std::string full_file_path = file_path + "/" + file_name;
  const int64_t file_size = file_system::FileSize(full_file_path);
  std::ofstream* file_stream;
  if (offset != 0) {
    if (file_size != offset) {
      LOG4CXX_DEBUG(logger_,
                    "ApplicationManagerImpl::SaveBinaryWithOffset offset"
                        << " does'n match existing file size");
      return mobile_apis::Result::INVALID_DATA;
    }
    file_stream = file_system::Open(full_file_path, std::ios_base::app);
  } else {
    LOG4CXX_DEBUG(
        logger_,
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
  LOG4CXX_INFO(logger_, "Successfully write data to file");
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

void ApplicationManagerImpl::OnApplicationListUpdateTimer() {
  LOG4CXX_DEBUG(logger_, "Application list update timer finished");

  apps_to_register_list_lock_.Acquire();
  const bool trigger_ptu = apps_size_ != applications_.size();
  apps_to_register_list_lock_.Release();
  SendUpdateAppList();
  GetPolicyHandler().OnAppsSearchCompleted(trigger_ptu);
}

void ApplicationManagerImpl::OnTimerSendTTSGlobalProperties() {
  std::vector<uint32_t> app_list;
  {
    sync_primitives::AutoLock lock(tts_global_properties_app_list_lock_);
    std::map<uint32_t, TimevalStruct>::iterator it =
        tts_global_properties_app_list_.begin();
    std::map<uint32_t, TimevalStruct>::iterator it_end =
        tts_global_properties_app_list_.end();
    date_time::TimeCompare time_comp;
    for (; it != it_end; ++it) {
      time_comp = date_time::DateTime::compareTime(
          date_time::DateTime::getCurrentTime(), it->second);
      if (date_time::GREATER == time_comp || date_time::EQUAL == time_comp) {
        app_list.push_back(it->first);
      }
    }
  }
  if (!app_list.empty()) {
    for (uint32_t i = 0; i < app_list.size(); ++i) {
      LOG4CXX_INFO(logger_,
                   "Send TTS GlobalProperties to HMI with default helpPrompt");
      MessageHelper::SendTTSGlobalProperties(
          application(app_list[i]), true, *this);
      RemoveAppFromTTSGlobalPropertiesList(app_list[i]);
    }
  }
}

void ApplicationManagerImpl::AddAppToTTSGlobalPropertiesList(
    const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint16_t timeout = get_settings().tts_global_properties_timeout();
  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  current_time.tv_sec += timeout;
  // please avoid AutoLock usage to avoid deadlock
  tts_global_properties_app_list_lock_.Acquire();
  if (tts_global_properties_app_list_.end() ==
      tts_global_properties_app_list_.find(app_id)) {
    tts_global_properties_app_list_[app_id] = current_time;
  }
  // if add first item need to start timer on one second
  if (1 == tts_global_properties_app_list_.size()) {
    LOG4CXX_INFO(logger_, "Start tts_global_properties_timer_");
    tts_global_properties_app_list_lock_.Release();
    const uint32_t timeout_ms = 1000;
    tts_global_properties_timer_.Start(timeout_ms, timer::kSingleShot);
    return;
  }
  tts_global_properties_app_list_lock_.Release();
}

void ApplicationManagerImpl::RemoveAppFromTTSGlobalPropertiesList(
    const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  // please avoid AutoLock usage to avoid deadlock
  tts_global_properties_app_list_lock_.Acquire();
  std::map<uint32_t, TimevalStruct>::iterator it =
      tts_global_properties_app_list_.find(app_id);
  if (tts_global_properties_app_list_.end() != it) {
    tts_global_properties_app_list_.erase(it);
    if (tts_global_properties_app_list_.empty()) {
      LOG4CXX_DEBUG(logger_, "Stop tts_global_properties_timer_");
      // if container is empty need to stop timer
      tts_global_properties_app_list_lock_.Release();
      tts_global_properties_timer_.Stop();
      return;
    }
  }
  tts_global_properties_app_list_lock_.Release();
}

mobile_apis::AppHMIType::eType ApplicationManagerImpl::StringToAppHMIType(
    std::string str) {
  LOG4CXX_AUTO_TRACE(logger_);
  if ("DEFAULT" == str) {
    return mobile_apis::AppHMIType::DEFAULT;
  } else if ("COMMUNICATION" == str) {
    return mobile_apis::AppHMIType::COMMUNICATION;
  } else if ("MEDIA" == str) {
    return mobile_apis::AppHMIType::MEDIA;
  } else if ("MESSAGING" == str) {
    return mobile_apis::AppHMIType::MESSAGING;
  } else if ("NAVIGATION" == str) {
    return mobile_apis::AppHMIType::NAVIGATION;
  } else if ("PROJECTION" == str) {
    return mobile_apis::AppHMIType::PROJECTION;
  } else if ("INFORMATION" == str) {
    return mobile_apis::AppHMIType::INFORMATION;
  } else if ("SOCIAL" == str) {
    return mobile_apis::AppHMIType::SOCIAL;
  } else if ("BACKGROUND_PROCESS" == str) {
    return mobile_apis::AppHMIType::BACKGROUND_PROCESS;
  } else if ("TESTING" == str) {
    return mobile_apis::AppHMIType::TESTING;
  } else if ("SYSTEM" == str) {
    return mobile_apis::AppHMIType::SYSTEM;
  } else {
    return mobile_apis::AppHMIType::INVALID_ENUM;
  }
}

bool ApplicationManagerImpl::CompareAppHMIType(
    const smart_objects::SmartObject& from_policy,
    const smart_objects::SmartObject& from_application) {
  LOG4CXX_AUTO_TRACE(logger_);
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
  LOG4CXX_AUTO_TRACE(logger_);

  std::map<std::string, std::vector<std::string> >::iterator
      it_app_hmi_types_from_policy;
  std::vector<std::string> hmi_types_from_policy;
  smart_objects::SmartObject transform_app_hmi_types(
      smart_objects::SmartType_Array);
  bool flag_diffirence_app_hmi_type = false;
  DataAccessor<ApplicationSet> accessor(applications());
  for (ApplicationSetIt it = accessor.GetData().begin();
       it != accessor.GetData().end();
       ++it) {
    it_app_hmi_types_from_policy = app_hmi_types.find(((*it)->policy_app_id()));

    if (it_app_hmi_types_from_policy != app_hmi_types.end() &&
        ((it_app_hmi_types_from_policy->second).size())) {
      flag_diffirence_app_hmi_type = false;
      hmi_types_from_policy = (it_app_hmi_types_from_policy->second);

      if (transform_app_hmi_types.length()) {
        transform_app_hmi_types =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
      }

      for (uint32_t i = 0; i < hmi_types_from_policy.size(); ++i) {
        transform_app_hmi_types[i] =
            StringToAppHMIType(hmi_types_from_policy[i]);
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
        if ((*it)->hmi_level() == mobile_api::HMILevel::HMI_BACKGROUND) {
          MessageHelper::SendUIChangeRegistrationRequestToHMI(*it, *this);
        } else if (((*it)->hmi_level() == mobile_api::HMILevel::HMI_FULL) ||
                   ((*it)->hmi_level() == mobile_api::HMILevel::HMI_LIMITED)) {
          MessageHelper::SendUIChangeRegistrationRequestToHMI(*it, *this);
          state_controller().SetRegularState(
              *it, mobile_apis::HMILevel::HMI_BACKGROUND, true);
        }
      }
    }
  }
}

void ApplicationManagerImpl::EraseAppFromReconnectionList(
    const ApplicationSharedPtr& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app) {
    LOG4CXX_WARN(logger_, "Application is not valid.");
    return;
  }

  const auto policy_app_id = app->policy_app_id();
  sync_primitives::AutoLock lock(reregister_wait_list_lock_);
  auto app_it = std::find_if(
      reregister_wait_list_.begin(),
      reregister_wait_list_.end(),
      std::bind1st(std::ptr_fun(&policy_app_id_comparator), policy_app_id));
  if (reregister_wait_list_.end() != app_it) {
    reregister_wait_list_.erase(app_it);
  }
}

void ApplicationManagerImpl::ProcessReconnection(
    ApplicationSharedPtr application, const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
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
#ifdef SDL_REMOTE_CONTROL
  if (!ptu_result) {
    return;
  }
  plugin_manager_.OnPolicyEvent(
      functional_modules::PolicyEvent::kApplicationPolicyUpdated);
#endif  // SDL_REMOTE_CONTROL
}

protocol_handler::MajorProtocolVersion
ApplicationManagerImpl::SupportedSDLVersion() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return static_cast<protocol_handler::MajorProtocolVersion>(
      get_settings().max_supported_protocol_version());
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
    LOG4CXX_WARN(logger_, directory_type << " directory doesn't exist.");
    // if storage directory doesn't exist try to create it
    if (!file_system::CreateDirectoryRecursively(path)) {
      LOG4CXX_ERROR(logger_,
                    "Unable to create " << directory_type << " directory "
                                        << path);
      return false;
    }
    LOG4CXX_DEBUG(logger_,
                  directory_type << " directory has been created: " << path);
  }

  return true;
}

bool ApplicationManagerImpl::IsReadWriteAllowed(const std::string& path,
                                                DirectoryType type) const {
  const std::string directory_type = DirectoryTypeToString(type);
  if (!(file_system::IsWritingAllowed(path) &&
        file_system::IsReadingAllowed(path))) {
    LOG4CXX_ERROR(logger_,
                  directory_type
                      << " directory doesn't have read/write permissions.");
    return false;
  }

  LOG4CXX_DEBUG(logger_,
                directory_type << " directory has read/write permissions.");

  return true;
}

void ApplicationManagerImpl::ClearTTSGlobalPropertiesList() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(tts_global_properties_app_list_lock_);
  tts_global_properties_app_list_.clear();
}

bool ApplicationManagerImpl::IsAppSubscribedForWayPoints(
    const uint32_t app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  if (subscribed_way_points_apps_list_.find(app_id) ==
      subscribed_way_points_apps_list_.end()) {
    return false;
  }
  return true;
}

void ApplicationManagerImpl::SubscribeAppForWayPoints(const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  subscribed_way_points_apps_list_.insert(app_id);
}

void ApplicationManagerImpl::UnsubscribeAppFromWayPoints(
    const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  subscribed_way_points_apps_list_.erase(app_id);
}

bool ApplicationManagerImpl::IsAppSubscribedForWayPoints(
    ApplicationSharedPtr app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  LOG4CXX_DEBUG(logger_,
                "There are applications subscribed: "
                    << subscribed_way_points_apps_list_.size());
  if (subscribed_way_points_apps_list_.find(app) ==
      subscribed_way_points_apps_list_.end()) {
    return false;
  }
  return true;
}

void ApplicationManagerImpl::SubscribeAppForWayPoints(
    ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  subscribed_way_points_apps_list_.insert(app);
  LOG4CXX_DEBUG(logger_,
                "There are applications subscribed: "
                    << subscribed_way_points_apps_list_.size());
}

void ApplicationManagerImpl::UnsubscribeAppFromWayPoints(
    ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  subscribed_way_points_apps_list_.erase(app);
  LOG4CXX_DEBUG(logger_,
                "There are applications subscribed: "
                    << subscribed_way_points_apps_list_.size());
}

bool ApplicationManagerImpl::IsAnyAppSubscribedForWayPoints() const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  LOG4CXX_DEBUG(logger_,
                "There are applications subscribed: "
                    << subscribed_way_points_apps_list_.size());
  return !subscribed_way_points_apps_list_.empty();
}

const std::set<int32_t> ApplicationManagerImpl::GetAppsSubscribedForWayPoints()
    const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(subscribed_way_points_apps_lock_);
  return subscribed_way_points_apps_list_;
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

#ifdef BUILD_TESTS
void ApplicationManagerImpl::AddMockApplication(ApplicationSharedPtr mock_app) {
  applications_list_lock_.Acquire();
  applications_.insert(mock_app);
  apps_size_ = applications_.size();
  applications_list_lock_.Release();
}
#endif  // BUILD_TESTS
#ifdef SDL_REMOTE_CONTROL
struct MobileAppIdPredicate {
  std::string policy_app_id_;
  MobileAppIdPredicate(const std::string& policy_app_id)
      : policy_app_id_(policy_app_id) {}
  bool operator()(const ApplicationSharedPtr app) const {
    return app ? policy_app_id_ == app->policy_app_id() : false;
  }
};

struct TakeDeviceHandle {
 public:
  TakeDeviceHandle(const ApplicationManager& app_mngr) : app_mngr_(app_mngr) {}
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
    LOG4CXX_DEBUG(logger_, "No such device : " << device_id);
    return ApplicationSharedPtr();
  }

  DataAccessor<ApplicationSet> accessor = applications();
  ApplicationSharedPtr app =
      FindApp(accessor, IsApplication(device_handle, policy_app_id));

  LOG4CXX_DEBUG(logger_,
                " policy_app_id << " << policy_app_id << "Found = " << app);
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
  using namespace mobile_apis::HMILevel;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "AppID to change: " << app_id << " -> " << level);
  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_, "There is no app with id: " << app_id);
    return;
  }
  eType old_level = app->hmi_level();
  if (old_level != level) {
    app->set_hmi_level(level);
    OnHMILevelChanged(app_id, old_level, level);

    plugin_manager_.OnAppHMILevelChanged(app, old_level);
  } else {
    LOG4CXX_WARN(logger_, "Redudant changing HMI level : " << level);
  }
}

void ApplicationManagerImpl::SendPostMessageToMobile(
    const MessagePtr& message) {
  messages_to_mobile_.PostMessage(impl::MessageToMobile(message, false));
}

void ApplicationManagerImpl::SendPostMessageToHMI(const MessagePtr& message) {
  messages_to_hmi_.PostMessage(impl::MessageToHmi(message));
}

#endif  // SDL_REMOTE_CONTROL

}  // namespace application_manager
