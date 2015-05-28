/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_command_factory.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_notification_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/policies/policy_handler.h"
#include "hmi_message_handler/hmi_message_handler.h"
#include "connection_handler/connection_handler_impl.h"
#include "formatters/formatter_json_rpc.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/CFormatterJsonSDLRPCv1.hpp"
#include "config_profile/profile.h"
#include "utils/threads/thread.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "smart_objects/enum_schema_item.h"
#include "interfaces/HMI_API_schema.h"
#include "application_manager/application_impl.h"
#include "usage_statistics/counter.h"
#include <time.h>

namespace {
  int get_rand_from_range(uint32_t from = 0, int to = RAND_MAX) {
    return std::rand() % to + from;
  }
}

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

uint32_t ApplicationManagerImpl::corelation_id_ = 0;
const uint32_t ApplicationManagerImpl::max_corelation_id_ = UINT_MAX;

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

using namespace NsSmartDeviceLink::NsSmartObjects;

ApplicationManagerImpl::ApplicationManagerImpl()
  : applications_list_lock_(true),
    audio_pass_thru_active_(false),
    is_distracting_driver_(false),
    is_vr_session_strated_(false),
    hmi_cooperating_(false),
    is_all_apps_allowed_(true),
    media_manager_(NULL),
    hmi_handler_(NULL),
    connection_handler_(NULL),
    protocol_handler_(NULL),
    request_ctrl_(),
    hmi_so_factory_(NULL),
    mobile_so_factory_(NULL),
    messages_from_mobile_("AM FromMobile", this),
    messages_to_mobile_("AM ToMobile", this),
    messages_from_hmi_("AM FromHMI", this),
    messages_to_hmi_("AM ToHMI", this),
    audio_pass_thru_messages_("AudioPassThru", this),
    hmi_capabilities_(this),
    unregister_reason_(mobile_api::AppInterfaceUnregisteredReason::INVALID_ENUM),
    resume_ctrl_(this),
    end_services_timer("EndServiceTimer", this, &ApplicationManagerImpl::EndNaviServices),
    wait_end_service_timeout_(profile::Profile::instance()->stop_streaming_timeout()),
#ifdef TIME_TESTER
    metric_observer_(NULL),
#endif  // TIME_TESTER
    application_list_update_timer_(new ApplicationListUpdateTimer(this)),
    tts_global_properties_timer_("TTSGLPRTimer",
                                      this,
                                      &ApplicationManagerImpl::OnTimerSendTTSGlobalProperties,
                                      true),
    is_low_voltage_(false) {
    std::srand(std::time(0));
    AddPolicyObserver(this);
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  LOG4CXX_INFO(logger_, "Destructing ApplicationManager.");

  SendOnSDLClose();
  media_manager_ = NULL;
  hmi_handler_ = NULL;
  connection_handler_ = NULL;
  if(hmi_so_factory_) {
    delete hmi_so_factory_;
    hmi_so_factory_ = NULL;
  }
  if(mobile_so_factory_) {
    delete mobile_so_factory_;
    mobile_so_factory_ = NULL;
  }
  protocol_handler_ = NULL;
  media_manager_ = NULL;
  LOG4CXX_INFO(logger_, "Destroying Policy Handler");
  RemovePolicyObserver(this);
  policy::PolicyHandler::destroy();
}

bool ApplicationManagerImpl::Stop() {
  LOG4CXX_INFO(logger_, "Stop ApplicationManager.");
  application_list_update_timer_->stop();
  try {
    UnregisterAllApplications();
  } catch (...) {
    LOG4CXX_ERROR(logger_,
                  "An error occurred during unregistering applications.");
  }


  // for PASA customer policy backup should happen :AllApp(SUSPEND)
  LOG4CXX_INFO(logger_, "Unloading policy library.");
  policy::PolicyHandler::instance()->UnloadPolicyLibrary();

  return true;
}

ApplicationSharedPtr ApplicationManagerImpl::application(uint32_t app_id) const {
  AppIdPredicate finder(app_id);
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(finder);
  LOG4CXX_DEBUG(logger_, " app_id << " << app_id << "Found = " << app);
  return app;
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_hmi_app(
  uint32_t hmi_app_id) const {
  HmiAppIdPredicate finder(hmi_app_id);
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(finder);
  LOG4CXX_DEBUG(logger_, " hmi_app_id << " << hmi_app_id << "Found = " << app);
  return app;
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_policy_id(
  const std::string& policy_app_id) const {
  MobileAppIdPredicate finder(policy_app_id);
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(finder);
  LOG4CXX_DEBUG(logger_, " policy_app_id << " << policy_app_id << "Found = " << app);
  return app;
}

bool ActiveAppPredicate (const ApplicationSharedPtr app) {
  return app ? app->IsFullscreen() : false;
}

ApplicationSharedPtr ApplicationManagerImpl::active_application() const {
  // TODO(DK) : check driver distraction
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(ActiveAppPredicate);
  LOG4CXX_DEBUG(logger_, " Found = " << app);
  return app;
}

bool LimitedAppPredicate (const ApplicationSharedPtr app) {
  return app ? app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED :
               false;
}

ApplicationSharedPtr
ApplicationManagerImpl::get_limited_media_application() const {
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(LimitedAppPredicate);
  LOG4CXX_DEBUG(logger_, " Found = " << app);
  return app;
}

bool LimitedNaviAppPredicate (const ApplicationSharedPtr app) {
  return app ? (app->is_navi() &&
                app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED) :
               false;
}

ApplicationSharedPtr
ApplicationManagerImpl::get_limited_navi_application() const {
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(LimitedNaviAppPredicate);
  LOG4CXX_DEBUG(logger_, " Found = " << app);
  return app;
}

bool LimitedVoiceAppPredicate (const ApplicationSharedPtr app) {
  return app ? (app->is_voice_communication_supported() &&
                app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED) :
               false;
}

ApplicationSharedPtr
ApplicationManagerImpl::get_limited_voice_application() const {
  ApplicationListAccessor accessor;
  ApplicationSharedPtr app = accessor.Find(LimitedVoiceAppPredicate);
  LOG4CXX_DEBUG(logger_, " Found = " << app);
  return app;
}

bool NaviAppPredicate (const ApplicationSharedPtr app) {
  return app ? app->is_navi() : false;
}

std::vector<ApplicationSharedPtr> ApplicationManagerImpl::applications_with_navi() {
  ApplicationListAccessor accessor;
  std::vector<ApplicationSharedPtr> apps = accessor.FindAll(NaviAppPredicate);
  LOG4CXX_DEBUG(logger_, " Found count: " << apps.size());
  return apps;
}
std::vector<ApplicationSharedPtr> ApplicationManagerImpl::applications_by_button(
  uint32_t button) {
  SubscribedToButtonPredicate finder(
        static_cast<mobile_apis::ButtonName::eType>(button));
  ApplicationListAccessor accessor;
  std::vector<ApplicationSharedPtr> apps = accessor.FindAll(finder);
  LOG4CXX_DEBUG(logger_, " Found count: " << apps.size());
  return apps;
}

std::vector<ApplicationSharedPtr> ApplicationManagerImpl::IviInfoUpdated(
                                             VehicleDataType vehicle_info, int value) {

  // Notify Policy Manager if available about info it's interested in,
  // i.e. odometer etc
  switch (vehicle_info) {
    case ODOMETER:
      policy::PolicyHandler::instance()->KmsChanged(value);
      break;
    default:
      break;
  }

  SubscribedToIVIPredicate finder(
        static_cast<int32_t>(vehicle_info));
  ApplicationListAccessor accessor;
  std::vector<ApplicationSharedPtr> apps = accessor.FindAll(NaviAppPredicate);
  LOG4CXX_DEBUG(logger_, " vehicle_info << " << vehicle_info << "Found count: " << apps.size());
  return apps;
}

bool ApplicationManagerImpl::DoesAudioAppWithSameHMITypeExistInFullOrLimited(
    ApplicationSharedPtr app) const {
  bool voice_state = app->is_voice_communication_supported();
  bool media_state = app->is_media_application();
  bool navi_state = app->hmi_supports_navi_video_streaming();
  ApplicationSharedPtr active_app = active_application();
  // Check app in FULL level
  if (active_app.valid()) {
    // If checking app hmi level FULL, we return false
    // because we couldn't have two applications with same HMIType in FULL and LIMITED HMI level
    if (active_app->app_id() == app->app_id()) {
      return false;
    }

    if (voice_state && active_app->is_voice_communication_supported()) {
      return true;
    }

    if (media_state && active_app->is_media_application()) {
      return true;
    }

    if (navi_state && active_app->hmi_supports_navi_video_streaming()) {
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

  return false;
}


ApplicationSharedPtr ApplicationManagerImpl::RegisterApplication(
  const utils::SharedPtr<smart_objects::SmartObject>&
  request_for_registration) {

  LOG4CXX_DEBUG(logger_, "Restarting application list update timer");
  policy::PolicyHandler::instance()->OnAppsSearchStarted();
  uint32_t timeout = profile::Profile::instance()->application_list_update_timeout();
  application_list_update_timer_->start(timeout);

  smart_objects::SmartObject& message = *request_for_registration;
  uint32_t connection_key =
    message[strings::params][strings::connection_key].asInt();

  if (false == is_all_apps_allowed_) {
    LOG4CXX_INFO(logger_,
                 "RegisterApplication: access to app's disabled by user");
    utils::SharedPtr<smart_objects::SmartObject> response(
      MessageHelper::CreateNegativeResponse(
        connection_key, mobile_apis::FunctionID::RegisterAppInterfaceID,
        message[strings::params][strings::correlation_id].asUInt(),
        mobile_apis::Result::DISALLOWED));
    ManageMobileCommand(response);
    return ApplicationSharedPtr();
  }

  // app_id is SDL "internal" ID
  // original app_id can be gotten via ApplicationImpl::mobile_app_id()
  uint32_t app_id = 0;
  std::list<int32_t> sessions_list;
  uint32_t device_id = 0;

  if (connection_handler_) {
    connection_handler::ConnectionHandlerImpl* con_handler_impl =
      static_cast<connection_handler::ConnectionHandlerImpl*>(

        connection_handler_);
    if (con_handler_impl->GetDataOnSessionKey(connection_key, &app_id,
        &sessions_list, &device_id)
        == -1) {
      LOG4CXX_ERROR(logger_,
                    "Failed to create application: no connection info.");
      utils::SharedPtr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(
          connection_key, mobile_apis::FunctionID::RegisterAppInterfaceID,
          message[strings::params][strings::correlation_id].asUInt(),
          mobile_apis::Result::GENERIC_ERROR));
      ManageMobileCommand(response);
      return ApplicationSharedPtr();
    }
  }

  smart_objects::SmartObject& params = message[strings::msg_params];

  const std::string& mobile_app_id = params[strings::app_id].asString();
  const std::string& app_name =
    message[strings::msg_params][strings::app_name].asString();

  ApplicationSharedPtr application(
    new ApplicationImpl(app_id,
                        mobile_app_id, app_name,
                        policy::PolicyHandler::instance()->GetStatisticManager()));
  if (!application) {
    usage_statistics::AppCounter count_of_rejections_sync_out_of_memory(
      policy::PolicyHandler::instance()->GetStatisticManager(), mobile_app_id,
      usage_statistics::REJECTIONS_SYNC_OUT_OF_MEMORY);
    ++count_of_rejections_sync_out_of_memory;

    utils::SharedPtr<smart_objects::SmartObject> response(
      MessageHelper::CreateNegativeResponse(
        connection_key, mobile_apis::FunctionID::RegisterAppInterfaceID,
        message[strings::params][strings::correlation_id].asUInt(),
        mobile_apis::Result::OUT_OF_MEMORY));
    ManageMobileCommand(response);
    return ApplicationSharedPtr();
  }

  application->set_device(device_id);
  application->set_grammar_id(GenerateGrammarID());
  mobile_api::Language::eType launguage_desired =
    static_cast<mobile_api::Language::eType>(params[strings::language_desired]
        .asInt());
  application->set_language(launguage_desired);
  application->usage_report().RecordAppRegistrationVuiLanguage(
    launguage_desired);

  mobile_api::Language::eType hmi_display_language_desired =
    static_cast<mobile_api::Language::eType>(params[strings::hmi_display_language_desired]
        .asInt());
  application->set_ui_language(hmi_display_language_desired);
  application->usage_report().RecordAppRegistrationGuiLanguage(
    hmi_display_language_desired);

  Version version;
  int32_t min_version =
    message[strings::msg_params][strings::sync_msg_version]
    [strings::minor_version].asInt();
  version.min_supported_api_version = static_cast<APIVersion>(min_version);

  int32_t max_version =
    message[strings::msg_params][strings::sync_msg_version]
    [strings::major_version].asInt();
  version.max_supported_api_version = static_cast<APIVersion>(max_version);
  application->set_version(version);

  ProtocolVersion protocol_version = static_cast<ProtocolVersion>(
      message[strings::params][strings::protocol_version].asInt());
  application->set_protocol_version(protocol_version);

  if (connection_handler_) {
    if (ProtocolVersion::kUnknownProtocol != protocol_version) {
      connection_handler_->BindProtocolVersionWithSession(
          connection_key, static_cast<uint8_t>(protocol_version));
    }
    if (protocol_version >= ProtocolVersion::kV3 &&
            profile::Profile::instance()->heart_beat_timeout() > 0) {
      connection_handler_->StartSessionHeartBeat(connection_key);
    }
  }

  apps_to_register_list_lock_.Acquire();
  apps_to_register_.erase(application);
  apps_to_register_list_lock_.Release();
  ApplicationListAccessor app_list_accesor;
  application->MarkRegistered();
  app_list_accesor.Insert(application);

  return application;
}

bool ApplicationManagerImpl::RemoveAppDataFromHMI(ApplicationSharedPtr app) {
  return true;
}

bool ApplicationManagerImpl::LoadAppDataToHMI(ApplicationSharedPtr app) {
  return true;
}

bool ApplicationManagerImpl::ActivateApplication(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app) {
    LOG4CXX_ERROR(logger_, "Null-pointer application received.");
    NOTREACHED();
    return false;
  }

  if (app->IsFullscreen()) {
    LOG4CXX_WARN(logger_, "Application is already active.");
    return false;
  }

  using namespace mobile_api::HMILevel;

  bool is_new_app_media = app->is_media_application();
  ApplicationSharedPtr current_active_app = active_application();

  if (HMI_LIMITED != app->hmi_level()) {
    if (app->has_been_activated()) {
      MessageHelper::SendAppDataToHMI(app);
    }
  }

  if (current_active_app) {
    if (is_new_app_media && current_active_app->is_media_application()) {
      MakeAppNotAudible(current_active_app->app_id());
    } else {
      ChangeAppsHMILevel(current_active_app->app_id(),
                         current_active_app->IsAudioApplication() ? HMI_LIMITED :
                                                                    HMI_BACKGROUND);
    }

    MessageHelper::SendHMIStatusNotification(*current_active_app);
  }

  MakeAppFullScreen(app->app_id());

  if (is_new_app_media) {
    ApplicationSharedPtr limited_app = get_limited_media_application();
    if (limited_app ) {
      if (!limited_app->is_navi()) {
        MakeAppNotAudible(limited_app->app_id());
        MessageHelper::SendHMIStatusNotification(*limited_app);
      } else {
        app->set_audio_streaming_state(mobile_apis::AudioStreamingState::ATTENUATED);
        MessageHelper::SendHMIStatusNotification(*app);
      }
    }
  }

  if (app->is_voice_communication_supported() || app->is_navi()) {
    ApplicationSharedPtr limited_app = get_limited_voice_application();
    if (limited_app.valid()) {
      if (limited_app->is_media_application()) {
        limited_app->set_audio_streaming_state(
            mobile_api::AudioStreamingState::NOT_AUDIBLE);
      }
      ChangeAppsHMILevel(app->app_id(), HMI_BACKGROUND);
      MessageHelper::SendHMIStatusNotification(*limited_app);
    }
  }

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
      DoesAudioAppWithSameHMITypeExistInFullOrLimited(app);
  bool is_active_app_exist = active_application().valid();



  mobile_api::HMILevel::eType result = mobile_api::HMILevel::HMI_FULL;
  if (is_audio_app && does_audio_app_with_same_type_exist) {
    result = GetDefaultHmiLevel(app);
  } else if (is_active_app_exist && is_audio_app) {
    result = mobile_apis::HMILevel::HMI_LIMITED;
  } else if (is_active_app_exist && (!is_audio_app)) {
    result = GetDefaultHmiLevel(app);
  }
  LOG4CXX_ERROR(logger_, "is_audio_app : " << is_audio_app
                << "; does_audio_app_with_same_type_exist : " << does_audio_app_with_same_type_exist
                << "; is_active_app_exist : " << is_active_app_exist
                << "; result : " << result);
  return result;
}

void ApplicationManagerImpl::ConnectToDevice(uint32_t id) {
  // TODO(VS): Call function from ConnectionHandler
  if (!connection_handler_) {
    LOG4CXX_WARN(logger_, "Connection handler is not set.");
    return;
  }

  connection_handler_->ConnectToDevice(id);
}

void ApplicationManagerImpl::OnHMIStartedCooperation() {
  hmi_cooperating_ = true;
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnHMIStartedCooperation()");

  utils::SharedPtr<smart_objects::SmartObject> is_vr_ready(
      MessageHelper::CreateModuleInfoSO(
          static_cast<uint32_t>(hmi_apis::FunctionID::VR_IsReady)));
  ManageHMICommand(is_vr_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_tts_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::TTS_IsReady));
  ManageHMICommand(is_tts_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_ui_ready(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::UI_IsReady));
  ManageHMICommand(is_ui_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_navi_ready(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::Navigation_IsReady));
  ManageHMICommand(is_navi_ready);

  utils::SharedPtr<smart_objects::SmartObject> is_ivi_ready(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_IsReady));
  ManageHMICommand(is_ivi_ready);

  utils::SharedPtr<smart_objects::SmartObject> button_capabilities(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::Buttons_GetCapabilities));
  ManageHMICommand(button_capabilities);

  utils::SharedPtr<smart_objects::SmartObject> mixing_audio_supported_request(
      MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported));
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

bool ApplicationManagerImpl::begin_audio_pass_thru() {
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  if (audio_pass_thru_active_) {
    return false;
  } else {
    audio_pass_thru_active_ = true;
    return true;
  }
}

bool ApplicationManagerImpl::end_audio_pass_thru() {
  sync_primitives::AutoLock lock(audio_pass_thru_lock_);
  if (audio_pass_thru_active_) {
    audio_pass_thru_active_ = false;
    return true;
  } else {
    return false;
  }
}

void ApplicationManagerImpl::set_driver_distraction(bool is_distracting) {
  is_distracting_driver_ = is_distracting;
}

void ApplicationManagerImpl::set_vr_session_started(const bool& state) {
  is_vr_session_strated_ = state;
}

void ApplicationManagerImpl::set_all_apps_allowed(const bool& allowed) {
  is_all_apps_allowed_ = allowed;
}

void ApplicationManagerImpl::StartAudioPassThruThread(int32_t session_key,
    int32_t correlation_id, int32_t max_duration, int32_t sampling_rate,
    int32_t bits_per_sample, int32_t audio_type) {
  LOG4CXX_INFO(logger_, "START MICROPHONE RECORDER");
  if (NULL != media_manager_) {
    media_manager_->StartMicrophoneRecording(
      session_key,
      profile::Profile::instance()->recording_file_name(),
      max_duration);
  }
}

void ApplicationManagerImpl::SendAudioPassThroughNotification(
  uint32_t session_key,
  std::vector<uint8_t>& binary_data) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!audio_pass_thru_active_) {
    LOG4CXX_ERROR(logger_, "Trying to send PassThroughNotification"
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
  if (NULL != media_manager_) {
    media_manager_->StopMicrophoneRecording(application_key);
  }
}

std::string ApplicationManagerImpl::GetDeviceName(
  connection_handler::DeviceHandle handle) {
  DCHECK(connection_handler_ != 0);

  std::string device_name = "";
  connection_handler::ConnectionHandlerImpl* con_handler_impl =
    static_cast<connection_handler::ConnectionHandlerImpl*>(
      connection_handler_);
  if (con_handler_impl->GetDataOnDeviceID(handle, &device_name,
                                          NULL) == -1) {
    LOG4CXX_ERROR(logger_, "Failed to extract device name for id " << handle);
  } else {
    LOG4CXX_INFO(logger_, "\t\t\t\t\tDevice name is " << device_name);
  }

  return device_name;
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

void ApplicationManagerImpl::OnErrorSending(
  hmi_message_handler::MessageSharedPointer message) {
  return;
}

void ApplicationManagerImpl::OnDeviceListUpdated(
    const connection_handler::DeviceMap& device_list) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr msg_params = MessageHelper::CreateDeviceListSO(
        device_list);
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
  connection_handler_->ConnectToAllDevices();
  LOG4CXX_DEBUG(logger_, "Starting application list update timer");
  uint32_t timeout = profile::Profile::instance()->application_list_update_timeout();
  application_list_update_timer_->start(timeout);
  policy::PolicyHandler::instance()->OnAppsSearchStarted();
}

void ApplicationManagerImpl::SendUpdateAppList() {
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObjectSPtr request = MessageHelper::CreateModuleInfoSO(
        FunctionID::BasicCommunication_UpdateAppList);

  (*request)[strings::msg_params][strings::applications] = SmartObject(SmartType_Array);

  SmartObject& applications = (*request)[strings::msg_params][strings::applications];

  PrepareApplicationListSO(applications_, applications);
  PrepareApplicationListSO(apps_to_register_, applications);

  ManageHMICommand(request);
}

void ApplicationManagerImpl::RemoveDevice(
    const connection_handler::DeviceHandle& device_handle) {
  LOG4CXX_INFO(logger_, "device_handle " << device_handle);
}

bool ApplicationManagerImpl::IsAudioStreamingAllowed(uint32_t application_key) const {
  ApplicationSharedPtr app = application(application_key);

  using namespace mobile_apis::HMILevel;
  using namespace helpers;
  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return false;
  }

  return Compare<eType, EQ, ONE>(
        app->hmi_level(), HMI_FULL, HMI_LIMITED);
}

bool ApplicationManagerImpl::IsVideoStreamingAllowed(uint32_t application_key) const {
  ApplicationSharedPtr app = application(application_key);
  using namespace mobile_apis::HMILevel;
  using namespace helpers;

  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "HMILevel: " << app->hmi_level());
  return Compare<eType, EQ, ONE>(app->hmi_level(), HMI_FULL, HMI_LIMITED);
}

mobile_apis::HMILevel::eType ApplicationManagerImpl::GetDefaultHmiLevel(
    ApplicationSharedPtr application) const {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);
  HMILevel::eType default_hmi = HMILevel::HMI_NONE;

  if (policy::PolicyHandler::instance()->PolicyEnabled()) {
    const std::string policy_app_id = application->mobile_app_id();
    std::string default_hmi_string = "";
    if (policy::PolicyHandler::instance()->GetDefaultHmi(
          policy_app_id, &default_hmi_string)) {
      if ("BACKGROUND" == default_hmi_string) {
        default_hmi = HMILevel::HMI_BACKGROUND;
      } else if ("FULL" == default_hmi_string) {
        default_hmi = HMILevel::HMI_FULL;
      } else if ("LIMITED" == default_hmi_string) {
        default_hmi = HMILevel::HMI_LIMITED;
      } else if ("NONE" == default_hmi_string) {
        default_hmi = HMILevel::HMI_NONE;
      } else {
        LOG4CXX_ERROR(logger_, "Unable to convert " + default_hmi_string + " to HMILevel");
      }
    } else {
      LOG4CXX_ERROR(logger_, "Unable to get default hmi_level for "
                    << policy_app_id);
    }
  }
  return default_hmi;
}

uint32_t ApplicationManagerImpl::GenerateGrammarID() {
  return rand();
}

uint32_t ApplicationManagerImpl::GenerateNewHMIAppID() {
  LOG4CXX_TRACE(logger_, "ENTER");
  uint32_t hmi_app_id = get_rand_from_range(1);
  LOG4CXX_DEBUG(logger_, "GenerateNewHMIAppID value is: " << hmi_app_id);

  while (resume_ctrl_.IsHMIApplicationIdExist(hmi_app_id)) {
    LOG4CXX_DEBUG(logger_, "HMI appID " << hmi_app_id << " is exists.");
    hmi_app_id = get_rand_from_range(1);
    LOG4CXX_DEBUG(logger_, "Trying new value: " << hmi_app_id);
  }

  LOG4CXX_TRACE(logger_, "EXIT");
  return hmi_app_id;
}

void ApplicationManagerImpl::ReplaceMobileByHMIAppId(
    smart_objects::SmartObject& message) {
  MessageHelper::PrintSmartObject(message);
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
        ApplicationManagerImpl::instance()->application(
          message[strings::app_id].asUInt());
    if (application.valid()) {
      LOG4CXX_INFO(logger_, "ReplaceMobileByHMIAppId from " << message[strings::app_id].asInt()
                   << " to " << application->hmi_app_id());
      message[strings::app_id] = application->hmi_app_id();
    }
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for(; it != message_array->end(); ++it) {
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
      default: {
        break;
      }
    }
  }
}

void ApplicationManagerImpl::ReplaceHMIByMobileAppId(
  smart_objects::SmartObject& message) {
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application_by_hmi_app(
        message[strings::app_id].asUInt());

    if (application.valid()) {
      LOG4CXX_INFO(logger_, "ReplaceHMIByMobileAppId from " << message[strings::app_id].asInt()
                   << " to " << application->app_id());
      message[strings::app_id] = application->app_id();
    }
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for(; it != message_array->end(); ++it) {
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
      default: {
        break;
      }
    }
  }
}

bool ApplicationManagerImpl::ProcessNaviService(protocol_handler::ServiceType type,
                                                uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!media_manager_) {
    LOG4CXX_DEBUG(logger_, "The media manager is not initialized.");
    return false;
  }

  bool result = false;
  switch (type) {
    case protocol_handler::kMobileNav:
      LOG4CXX_DEBUG(logger_, "Video service is about to be started.");
      if (IsVideoStreamingAllowed(connection_key)) {
        media_manager_->StartVideoStreaming(connection_key);
        result = true;
      }
      break;
    case protocol_handler::kAudio:
      LOG4CXX_DEBUG(logger_, "Audio service is about to be started.");
      if (IsAudioStreamingAllowed(connection_key)) {
        media_manager_->StartAudioStreaming(connection_key);
        result = true;
      }
      break;
    default:
      LOG4CXX_DEBUG(logger_, "Unknown type of service to be started.");
      break;
  }

  service_status_[type] = std::make_pair(result, false);
  return result;
}

bool ApplicationManagerImpl::OnServiceStartedCallback(
  const connection_handler::DeviceHandle& device_handle,
  const int32_t& session_key,
  const protocol_handler::ServiceType& type) {
  using namespace protocol_handler;
  using namespace helpers;

  LOG4CXX_INFO(logger_,
               "OnServiceStartedCallback " << type << " in session " << session_key);
  if (type == kRpc) {
    LOG4CXX_INFO(logger_, "RPC service is about to be started.");
    return true;
  }
  ApplicationSharedPtr app = application(session_key);
  if (!app) {
    LOG4CXX_DEBUG(logger_, "The application with id:" << session_key <<
                  " doesn't exists.");
    return false;
  }
  bool result = false;
  if (Compare<ServiceType, EQ, ONE>(type, kMobileNav, kAudio)) {
    if (app->is_navi()) {
      result = ProcessNaviService(type, session_key);
      app->set_streaming_allowed(result);
    }
  }
  return result;
}

void ApplicationManagerImpl::OnServiceEndedCallback(const int32_t& session_key,
    const protocol_handler::ServiceType& type) {
  using namespace protocol_handler;
  LOG4CXX_INFO_EXT(
    logger_,
    "OnServiceEndedCallback " << type  << " in session " << session_key);

  if (type == kRpc) {
    LOG4CXX_INFO(logger_, "Remove application.");
    /* in case it was unexpected disconnect application will be removed
     and we will notify HMI that it was unexpected disconnect,
     but in case it was closed by mobile we will be unable to find it in the list
    */
    UnregisterApplication(session_key, mobile_apis::Result::INVALID_ENUM,
                          true, true);
    return;
  }

  if (media_manager_) {
    switch (type) {
      case protocol_handler::kMobileNav: {
        LOG4CXX_INFO(logger_, "Stop video streaming.");
        media_manager_->StopVideoStreaming(session_key);
        break;
      }
      case protocol_handler::kAudio: {
        LOG4CXX_INFO(logger_, "Stop audio service.");
        media_manager_->StopAudioStreaming(session_key);
        break;
      }
      default:
        LOG4CXX_WARN(logger_, "Unknown type of service to be ended." << type);
        break;
    }
    service_status_[type].second = true;
    LOG4CXX_DEBUG(logger_, "Ack status: " << service_status_[type].first <<" : "
                  << service_status_[type].second);
  }
}

void ApplicationManagerImpl::OnApplicationFloodCallBack(const uint32_t &connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Unregister flooding application " << connection_key);

  MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
      connection_key,
      mobile_apis::AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS);

  const bool resuming = true;
  const bool unexpected_disconnect = false;
  UnregisterApplication(connection_key, mobile_apis::Result::TOO_MANY_PENDING_REQUESTS,
                        resuming, unexpected_disconnect);
  // TODO(EZamakhov): increment "removals_for_bad_behaviour" field in policy table
}

void ApplicationManagerImpl::OnMalformedMessageCallback(const uint32_t &connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Unregister malformed messaging application " << connection_key);

  MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
      connection_key,
      mobile_apis::AppInterfaceUnregisteredReason::PROTOCOL_VIOLATION);
}

void ApplicationManagerImpl::set_hmi_message_handler(
  hmi_message_handler::HMIMessageHandler* handler) {
  hmi_handler_ = handler;
}

void ApplicationManagerImpl::set_connection_handler(
  connection_handler::ConnectionHandler* handler) {
  connection_handler_ = handler;
}

connection_handler::ConnectionHandler* ApplicationManagerImpl::connection_handler() {
  return connection_handler_;
}

void ApplicationManagerImpl::set_protocol_handler(
  protocol_handler::ProtocolHandler* handler) {
  protocol_handler_ = handler;
}

void ApplicationManagerImpl::StartDevicesDiscovery() {
  connection_handler::ConnectionHandlerImpl::instance()->
  StartDevicesDiscovery();
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

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_,
                      "No application associated with connection key");
    if ((*message)[strings::msg_params].keyExists(strings::result_code) &&
        ((*message)[strings::msg_params][strings::result_code] ==
         NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_VERSION)) {
      (*message)[strings::params][strings::protocol_version] =
        ProtocolVersion::kV1;
    } else {
      (*message)[strings::params][strings::protocol_version] =
        SupportedSDLVersion();
    }
  } else {
    (*message)[strings::params][strings::protocol_version] =
      app->protocol_version();
  }

  mobile_so_factory().attachSchema(*message);
  LOG4CXX_INFO(
    logger_,
    "Attached schema to message, result if valid: " << message->isValid());

  // Messages to mobile are not yet prioritized so use default priority value
  utils::SharedPtr<Message> message_to_send(new Message(
        protocol_handler::MessagePriority::kDefault));
  if (!ConvertSOtoMessage((*message), (*message_to_send))) {
    LOG4CXX_WARN(logger_, "Can't send msg to Mobile: failed to create string");
    return;
  }

  smart_objects::SmartObject& msg_to_mobile = *message;
  // If correlation_id is not present, it is from-HMI message which should be
  // checked against policy permissions
  if (msg_to_mobile[strings::params].keyExists(strings::correlation_id)) {
    request_ctrl_.terminateMobileRequest(
      msg_to_mobile[strings::params][strings::correlation_id].asInt(),
      msg_to_mobile[strings::params][strings::connection_key].asInt());
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
          params.push_back(iter->first);
        }
      }
    }
    const mobile_apis::Result::eType check_result =
        CheckPolicyPermissions( app->mobile_app_id(),
                                app->hmi_level(), function_id, params);
    if (mobile_apis::Result::SUCCESS != check_result) {
      const std::string string_functionID =
          MessageHelper::StringifiedFunctionID(function_id);
      LOG4CXX_WARN(logger_, "Function \"" << string_functionID << "\" (#"
                   << function_id << ") not allowed by policy");
      return;
    }

    if (function_id == mobile_apis::FunctionID::OnSystemRequestID) {
      mobile_apis::RequestType::eType request_type =
          static_cast<mobile_apis::RequestType::eType>(
          (*message)[strings::msg_params][strings::request_type].asUInt());
      if (mobile_apis::RequestType::PROPRIETARY == request_type ||
          mobile_apis::RequestType::HTTP == request_type) {
        policy::PolicyHandler::instance()->OnUpdateRequestSentToMobile();
      }
    }
  }

  messages_to_mobile_.PostMessage(impl::MessageToMobile(message_to_send,
                                  final_message));
}

bool ApplicationManagerImpl::ManageMobileCommand(
    const commands::MessageSharedPtr message,
    commands::Command::CommandOrigin origin) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_WARN(logger_, "RET Null-pointer message received.");
    return false;
  }

  if (IsLowVoltage()) {
    LOG4CXX_WARN(logger_, "Low Voltage is active");
    return false;
  }
#ifdef DEBUG
  MessageHelper::PrintSmartObject(*message);
#endif

  LOG4CXX_INFO(logger_, "Trying to create message in mobile factory.");
  utils::SharedPtr<commands::Command> command(
          MobileCommandFactory::CreateCommand(message, origin));

  if (!command) {
    LOG4CXX_WARN(logger_, "RET  Failed to create mobile command from smart object");
    return false;
  }

  mobile_apis::FunctionID::eType function_id =
    static_cast<mobile_apis::FunctionID::eType>(
      (*message)[strings::params][strings::function_id].asInt());

  // Notifications from HMI have no such parameter
  uint32_t correlation_id =
    (*message)[strings::params].keyExists(strings::correlation_id)
    ? (*message)[strings::params][strings::correlation_id].asUInt()
    : 0;

  uint32_t connection_key =
    (*message)[strings::params][strings::connection_key].asUInt();

  int32_t protocol_type =
    (*message)[strings::params][strings::protocol_type].asUInt();

  ApplicationSharedPtr app;
  int32_t message_type = (*message)[strings::params][strings::message_type].asInt();

  if (((mobile_apis::FunctionID::RegisterAppInterfaceID != function_id) &&
       (protocol_type == commands::CommandImpl::mobile_protocol_type_)) &&
      (mobile_apis::FunctionID::UnregisterAppInterfaceID != function_id)) {
    app = ApplicationManagerImpl::instance()->application(connection_key);
    if (!app) {
      LOG4CXX_ERROR_EXT(logger_, "RET APPLICATION_NOT_REGISTERED");
      smart_objects::SmartObjectSPtr response =
          MessageHelper::CreateNegativeResponse(connection_key,
                                                static_cast<int32_t>(function_id),
                                                correlation_id,
                                                static_cast<int32_t>(mobile_apis::Result::APPLICATION_NOT_REGISTERED));

      SendMessageToMobile(response);
      return false;
    }

    // Message for "CheckPermission" must be with attached schema
    mobile_so_factory().attachSchema(*message);
  }

  if (message_type ==
      mobile_apis::messageType::response) {
    if (command->Init()) {
      command->Run();
      command->CleanUp();
    }
    return true;
  }
  if (message_type ==
      mobile_apis::messageType::notification) {
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

  if (message_type ==
      mobile_apis::messageType::request) {

    // commands will be launched from requesr_ctrl
    mobile_apis::HMILevel::eType app_hmi_level = mobile_apis::HMILevel::INVALID_ENUM;
    if (app) {
      app_hmi_level = app->hmi_level();
    }

    // commands will be launched from request_ctrl

    const request_controller::RequestController::TResult result =
      request_ctrl_.addMobileRequest(command, app_hmi_level);

    if (result == request_controller::RequestController::SUCCESS) {
      LOG4CXX_INFO(logger_, "Perform request");
    } else if (result ==
               request_controller::RequestController::
               TOO_MANY_PENDING_REQUESTS) {
      LOG4CXX_ERROR_EXT(logger_, "RET  Unable top perform request: " <<
                        "TOO_MANY_PENDING_REQUESTS");

      smart_objects::SmartObjectSPtr response =
          MessageHelper::CreateNegativeResponse(connection_key,
                                                static_cast<int32_t>(function_id),
                                                correlation_id,
                                                static_cast<int32_t>(mobile_apis::Result::TOO_MANY_PENDING_REQUESTS));

      SendMessageToMobile(response);
      return false;
    } else if (result ==
               request_controller::RequestController::TOO_MANY_REQUESTS) {
      LOG4CXX_ERROR_EXT(logger_, "RET  Unable to perform request: " <<
                        "TOO_MANY_REQUESTS");

      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
        connection_key,
        mobile_api::AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS);

      UnregisterApplication(connection_key,
                            mobile_apis::Result::TOO_MANY_PENDING_REQUESTS,
                            false);
      return false;
    } else if (result ==
               request_controller::RequestController::
               NONE_HMI_LEVEL_MANY_REQUESTS) {
      LOG4CXX_ERROR_EXT(logger_, "RET  Unable to perform request: " <<
                        "REQUEST_WHILE_IN_NONE_HMI_LEVEL");

      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
        connection_key, mobile_api::AppInterfaceUnregisteredReason::
        REQUEST_WHILE_IN_NONE_HMI_LEVEL);

      ApplicationSharedPtr app_ptr = application(connection_key);
      if(app_ptr) {
        app_ptr->usage_report().RecordRemovalsForBadBehavior();
      }
      UnregisterApplication(connection_key, mobile_apis::Result::INVALID_ENUM,
                            false);
      return false;
    } else {
      LOG4CXX_ERROR_EXT(logger_, "RET  Unable to perform request: Unknown case");
      return false;
    }
    return true;
  }

  LOG4CXX_ERROR(logger_, "RET  UNKNOWN MESSAGE TYPE " << message_type);
  return false;
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

  hmi_so_factory().attachSchema(*message);
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

  CommandSharedPtr command = HMICommandFactory::CreateCommand(message);
  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create command from smart object");
    return false;
  }

  int32_t message_type = (*(message.get()))[strings::params][strings::message_type].asInt();

  if (kRequest == message_type) {
    LOG4CXX_DEBUG(logger_, "ManageHMICommand");
    request_ctrl_.addHMIRequest(command);
  }

  if (command->Init()) {
    command->Run();
      if (kResponse == message_type) {
        int32_t correlation_id = (*(message.get()))[strings::params][strings::correlation_id].asInt();
        request_ctrl_.terminateHMIRequest(correlation_id);
      }
      return true;
  }
  return false;
}

bool ApplicationManagerImpl::Init() {
  LOG4CXX_TRACE(logger_, "Init application manager");
  const std::string app_storage_folder =
      profile::Profile::instance()->app_storage_folder();
  if (!file_system::DirectoryExists(app_storage_folder)) {
    LOG4CXX_WARN(logger_, "Storage directory doesn't exist");
    // if storage directory doesn't exist try to create it
    if (!file_system::CreateDirectoryRecursively(app_storage_folder)) {
      LOG4CXX_ERROR(logger_, "Unable to create Storage directory "
                    << app_storage_folder);
      return false;
    }
  }
  if (!(file_system::IsWritingAllowed(app_storage_folder) &&
        file_system::IsReadingAllowed(app_storage_folder))) {
    LOG4CXX_ERROR(logger_,
                  "Storage directory doesn't have read/write permissions");
    return false;
  }

  const std::string system_files_path =
      profile::Profile::instance()->system_files_path();
  if (!file_system::DirectoryExists(system_files_path)) {
    LOG4CXX_WARN(logger_, "System files directory doesn't exist");
    // if system directory doesn't exist try to create it
    if (!file_system::CreateDirectoryRecursively(system_files_path)) {
      LOG4CXX_ERROR(logger_, "Unable to create System directory "
                    << system_files_path);
      return false;
    }
  }
  if (!(file_system::IsWritingAllowed(system_files_path) &&
        file_system::IsReadingAllowed(system_files_path))) {
    LOG4CXX_ERROR(logger_,
                  "System directory doesn't have read/write permissions");
    return false;
  }
  if (policy::PolicyHandler::instance()->PolicyEnabled()) {
    if(!policy::PolicyHandler::instance()->LoadPolicyLibrary()) {
      LOG4CXX_ERROR(logger_, "Policy library is not loaded. Check LD_LIBRARY_PATH");
      return false;
    }
    LOG4CXX_INFO(logger_, "Policy library is loaded, now initing PT");
    if (!policy::PolicyHandler::instance()->InitPolicyTable()) {
      LOG4CXX_ERROR(logger_, "Policy table is not initialized.");
      return false;
    }
  } else {
    LOG4CXX_WARN(logger_, "System is configured to work without policy functionality.");
  }
  media_manager_ = media_manager::MediaManagerImpl::instance();
  return true;
}

bool ApplicationManagerImpl::ConvertMessageToSO(
  const Message& message, smart_objects::SmartObject& output) {
  LOG4CXX_INFO(
    logger_,
    "\t\t\tMessage to convert: protocol " << message.protocol_version()
    << "; json " << message.json_message());

  switch (message.protocol_version()) {
    case ProtocolVersion::kV4:
    case ProtocolVersion::kV3:
    case ProtocolVersion::kV2: {
        const bool conversion_result =
            formatters::CFormatterJsonSDLRPCv2::fromString(
            message.json_message(), output, message.function_id(),
            message.type(), message.correlation_id());
        if (!conversion_result
            || !mobile_so_factory().attachSchema(output)
            || ((output.validate() != smart_objects::Errors::OK)) ) {
          LOG4CXX_WARN(logger_, "Failed to parse string to smart object :"
                       << message.json_message());
          utils::SharedPtr<smart_objects::SmartObject> response(
                MessageHelper::CreateNegativeResponse(
                  message.connection_key(), message.function_id(),
                  message.correlation_id(), mobile_apis::Result::INVALID_DATA));
          ManageMobileCommand(response);
          return false;
        }
      LOG4CXX_INFO(
        logger_,
        "Convertion result for sdl object is true" << " function_id "
        << output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
      output[strings::params][strings::connection_key] =
        message.connection_key();
      output[strings::params][strings::protocol_version] =
        message.protocol_version();
      if (message.binary_data()) {
        if (message.payload_size() < message.data_size()) {
          LOG4CXX_ERROR(logger_, "Incomplete binary" <<
                                " binary size should be  " << message.data_size() <<
                                " payload data size is " << message.payload_size());
          utils::SharedPtr<smart_objects::SmartObject> response(
                            MessageHelper::CreateNegativeResponse(
                            message.connection_key(), message.function_id(),
                            message.correlation_id(), mobile_apis::Result::INVALID_DATA));
          ManageMobileCommand(response);
          return false;
        }
        output[strings::params][strings::binary_data] =
          *(message.binary_data());
      }
      break;
    }
    case ProtocolVersion::kHMI: {
#ifdef ENABLE_LOG
      int32_t result =
#endif
      formatters::FormatterJsonRpc::FromString <
                       hmi_apis::FunctionID::eType, hmi_apis::messageType::eType > (
                         message.json_message(), output);
      LOG4CXX_INFO(
        logger_,
        "Convertion result: " << result << " function id "
        << output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
      if (!hmi_so_factory().attachSchema(output)) {
        LOG4CXX_WARN(logger_, "Failed to attach schema to object.");
        return false;
      }
      if (output.validate() != smart_objects::Errors::OK) {
        LOG4CXX_ERROR(logger_, "Incorrect parameter from HMI");

        if (application_manager::MessageType::kNotification ==
            output[strings::params][strings::message_type].asInt()) {
          LOG4CXX_ERROR(logger_, "Ignore wrong HMI notification");
          return false;
        }

        if (application_manager::MessageType::kRequest ==
            output[strings::params][strings::message_type].asInt()) {
          LOG4CXX_ERROR(logger_, "Ignore wrong HMI request");
          return false;
        }

        output.erase(strings::msg_params);
        output[strings::params][hmi_response::code] =
          hmi_apis::Common_Result::INVALID_DATA;
        output[strings::msg_params][strings::info] =
          std::string("Received invalid data on HMI response");
      }
      break;
    }
    case ProtocolVersion::kV1: {
      static NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra v1_shema;

      if (message.function_id() == 0 || message.type() == kUnknownType) {
        LOG4CXX_ERROR(logger_, "Message received: UNSUPPORTED_VERSION");

        int32_t conversation_result =
          formatters::CFormatterJsonSDLRPCv1::fromString <
          NsSmartDeviceLinkRPC::V1::FunctionID::eType,
          NsSmartDeviceLinkRPC::V1::messageType::eType > (
            message.json_message(), output);

        if (formatters::CFormatterJsonSDLRPCv1::kSuccess
            == conversation_result) {

          smart_objects::SmartObject params = smart_objects::SmartObject(smart_objects::SmartType::SmartType_Map);

          output[strings::params][strings::message_type] =
            NsSmartDeviceLinkRPC::V1::messageType::response;
          output[strings::params][strings::connection_key] = message.connection_key();

          output[strings::msg_params] =
            smart_objects::SmartObject(smart_objects::SmartType::SmartType_Map);
          output[strings::msg_params][strings::success] = false;
          output[strings::msg_params][strings::result_code] =
            NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_VERSION;

          smart_objects::SmartObjectSPtr msg_to_send = new smart_objects::SmartObject(output);
          v1_shema.attachSchema(*msg_to_send);
          SendMessageToMobile(msg_to_send);
          return false;
        }
      }

      break;
    }
    default:
      // TODO(PV):
      //  removed NOTREACHED() because some app can still have vesion 1.
      LOG4CXX_WARN(
        logger_,
        "Application used unsupported protocol :" << message.protocol_version()
        << ".");
      return false;
  }

  LOG4CXX_INFO(logger_, "Successfully parsed message into smart object");
  return true;
}

bool ApplicationManagerImpl::ConvertSOtoMessage(
  const smart_objects::SmartObject& message, Message& output) {
  LOG4CXX_INFO(logger_, "Message to convert");

  if (smart_objects::SmartType_Null == message.getType()
      || smart_objects::SmartType_Invalid == message.getType()) {
    LOG4CXX_WARN(logger_, "Invalid smart object received.");
    return false;
  }

  LOG4CXX_INFO(
    logger_,
    "Message with protocol: "
    << message.getElement(jhs::S_PARAMS).getElement(jhs::S_PROTOCOL_TYPE)
    .asInt());

  std::string output_string;
  switch (message.getElement(jhs::S_PARAMS).getElement(jhs::S_PROTOCOL_TYPE)
          .asInt()) {
    case 0: {
      if (message.getElement(jhs::S_PARAMS).getElement(jhs::S_PROTOCOL_VERSION).asInt() == 1) {
        if (!formatters::CFormatterJsonSDLRPCv1::toString(message,
            output_string)) {
          LOG4CXX_WARN(logger_, "Failed to serialize smart object");
          return false;
        }
        output.set_protocol_version(application_manager::kV1);
      } else {
        if (!formatters::CFormatterJsonSDLRPCv2::toString(message,
            output_string)) {
          LOG4CXX_WARN(logger_, "Failed to serialize smart object");
          return false;
        }
        output.set_protocol_version(
          static_cast<ProtocolVersion>(
            message.getElement(jhs::S_PARAMS).getElement(
              jhs::S_PROTOCOL_VERSION).asUInt()));
      }

      break;
    }
    case 1: {
      if (!formatters::FormatterJsonRpc::ToString(message, output_string)) {
        LOG4CXX_WARN(logger_, "Failed to serialize smart object");
        return false;
      }
      output.set_protocol_version(application_manager::kHMI);
      break;
    }
    default:
      NOTREACHED();
      return false;
  }

  LOG4CXX_INFO(logger_, "Convertion result: " << output_string);

  output.set_connection_key(
    message.getElement(jhs::S_PARAMS).getElement(strings::connection_key)
    .asInt());

  output.set_function_id(
    message.getElement(jhs::S_PARAMS).getElement(jhs::S_FUNCTION_ID).asInt());

  output.set_correlation_id(
    message.getElement(jhs::S_PARAMS).getElement(jhs::S_CORRELATION_ID)
    .asInt());
  output.set_message_type(
    static_cast<MessageType>(message.getElement(jhs::S_PARAMS).getElement(
                               jhs::S_MESSAGE_TYPE).asInt()));

  // Currently formatter creates JSON = 3 bytes for empty SmartObject.
  // workaround for notification. JSON must be empty
  if (mobile_apis::FunctionID::OnAudioPassThruID
      != message.getElement(jhs::S_PARAMS).getElement(strings::function_id)
      .asInt()) {
    output.set_json_message(output_string);
  }

  if (message.getElement(jhs::S_PARAMS).keyExists(strings::binary_data)) {
    application_manager::BinaryData* binaryData =
      new application_manager::BinaryData(
      message.getElement(jhs::S_PARAMS).getElement(strings::binary_data)
      .asBinary());

    if (NULL == binaryData) {
      LOG4CXX_ERROR(logger_, "Null pointer");
      return false;
    }
    output.set_binary_data(binaryData);
  }

  LOG4CXX_INFO(logger_, "Successfully parsed smart object into message");
  return true;
}

utils::SharedPtr<Message> ApplicationManagerImpl::ConvertRawMsgToMessage(
  const ::protocol_handler::RawMessagePtr message) {
  DCHECK(message);
  utils::SharedPtr<Message> outgoing_message;

  LOG4CXX_INFO(logger_, "Service type." << message->service_type());

  if (message->service_type() != protocol_handler::kRpc
      &&
      message->service_type() != protocol_handler::kBulk) {
    // skip this message, not under handling of ApplicationManager
    LOG4CXX_TRACE(logger_, "Skipping message; not the under AM handling.");
    return outgoing_message;
  }

  Message* convertion_result = MobileMessageHandler::HandleIncomingMessageProtocol(message);

  if (convertion_result) {
    outgoing_message = convertion_result;
  } else {
    LOG4CXX_ERROR(logger_, "Received invalid message");
  }
  return outgoing_message;
}

void ApplicationManagerImpl::ProcessMessageFromMobile(
  const utils::SharedPtr<Message> message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::ProcessMessageFromMobile()");
#ifdef TIME_TESTER
  AMMetricObserver::MessageMetricSharedPtr metric(new AMMetricObserver::MessageMetric());
  metric->begin = date_time::DateTime::getCurrentTime();
#endif  // TIME_TESTER
  smart_objects::SmartObjectSPtr so_from_mobile(new smart_objects::SmartObject);

  if (!so_from_mobile) {
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

  if (!ConvertMessageToSO(*message, *so_from_mobile)) {
    LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
    return;
  }
#ifdef TIME_TESTER
  metric->message = so_from_mobile;
#endif  // TIME_TESTER

  if (!ManageMobileCommand(so_from_mobile,
                           commands::Command::ORIGIN_MOBILE)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
#ifdef TIME_TESTER
  metric->end = date_time::DateTime::getCurrentTime();
  if (metric_observer_) {
    metric_observer_->OnMessage(metric);
  }
#endif  // TIME_TESTER
}

void ApplicationManagerImpl::ProcessMessageFromHMI(
  const utils::SharedPtr<Message> message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::ProcessMessageFromHMI()");
  smart_objects::SmartObjectSPtr smart_object(new smart_objects::SmartObject);

  if (!smart_object) {
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

#ifdef HMI_DBUS_API
  *smart_object = message->smart_object();
#else
  if (!ConvertMessageToSO(*message, *smart_object)) {
    LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
    return;
  }
#endif  // HMI_DBUS_API

  LOG4CXX_INFO(logger_, "Converted message, trying to create hmi command");
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
  return hmi_capabilities_;
}

void ApplicationManagerImpl::CreateApplications(SmartArray& obj_array) {

  using namespace policy;

  const std::size_t arr_size(obj_array.size());
  for (std::size_t idx = 0; idx < arr_size; ++idx) {

    const SmartObject& app_data = obj_array[idx];
    if (app_data.isValid()) {
      const std::string url_schema(app_data[strings::urlSchema].asString());
      const std::string package_name(app_data[strings::packageName].asString());
      const std::string mobile_app_id(app_data[strings::app_id].asString());
      const std::string appName(app_data[strings::app_name].asString());

      const uint32_t hmi_app_id(GenerateNewHMIAppID());

      ApplicationSharedPtr app(
            new ApplicationImpl(0,
                                mobile_app_id,
                                appName,
                                PolicyHandler::instance()->GetStatisticManager()));
      if (app) {
        app->SetShemaUrl(url_schema);
        app->SetPackageName(package_name);
        app->set_hmi_application_id(hmi_app_id);

        sync_primitives::AutoLock lock(apps_to_register_list_lock_);
        apps_to_register_.insert(app);
      }
    }
  }
}

void ApplicationManagerImpl::ProcessQueryApp(
    const smart_objects::SmartObject& sm_object) {
  using namespace policy;
  using namespace profile;

  if (sm_object.keyExists(strings::application)) {
    SmartArray* obj_array = sm_object[strings::application].asArray();
    if (NULL != obj_array) {
      const std::string app_icon_dir(Profile::instance()->app_icons_folder());
      CreateApplications(*obj_array);
      SendUpdateAppList();

      AppsWaitRegistrationSet::const_iterator it = apps_to_register_.begin();
      for (; it != apps_to_register_.end(); ++it) {

        const std::string full_icon_path(app_icon_dir + "/" + (*it)->mobile_app_id());
        if (file_system::FileExists(full_icon_path)) {
          MessageHelper::SendSetAppIcon((*it)->hmi_app_id(), full_icon_path);
        }
      }
    }
  }
}

#ifdef TIME_TESTER
void ApplicationManagerImpl::SetTimeMetricObserver(AMMetricObserver* observer) {
  metric_observer_ = observer;
}
#endif  // TIME_TESTER

void ApplicationManagerImpl::addNotification(const CommandSharedPtr ptr) {
  request_ctrl_.addNotification(ptr);
}

void ApplicationManagerImpl::removeNotification(const commands::Command* notification) {
  request_ctrl_.removeNotification(notification);
}

void ApplicationManagerImpl::updateRequestTimeout(uint32_t connection_key,
    uint32_t mobile_correlation_id,
    uint32_t new_timeout_value) {
  LOG4CXX_AUTO_TRACE(logger_);
  request_ctrl_.updateRequestTimeout(connection_key, mobile_correlation_id,
                                     new_timeout_value);
}

const uint32_t ApplicationManagerImpl::application_id
(const int32_t correlation_id) {
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
  appID_list_.insert(std::pair<const int32_t, const uint32_t>
                     (correlation_id, app_id));
}

void ApplicationManagerImpl::AddPolicyObserver( policy::PolicyHandlerObserver* listener) {
  policy::PolicyHandler::instance()->add_listener(listener);
}

void ApplicationManagerImpl::RemovePolicyObserver(policy::PolicyHandlerObserver* listener) {
  policy::PolicyHandler::instance()->remove_listener(listener);
}

void ApplicationManagerImpl::SetUnregisterAllApplicationsReason(
  mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  LOG4CXX_TRACE(logger_, "reason = " << reason);
  unregister_reason_ = reason;
}

void ApplicationManagerImpl::HeadUnitReset(
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  switch (reason) {
    case mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET: {
      UnregisterAllApplications();
      policy::PolicyHandler::instance()->ResetPolicyTable();
      policy::PolicyHandler::instance()->UnloadPolicyLibrary();

      resume_controller().StopSavePersistentDataTimer();
      file_system::remove_directory_content(profile::Profile::instance()->app_storage_folder());
      break;
    }
    case mobile_api::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS: {
      policy::PolicyHandler::instance()->ClearUserConsent();
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Bad AppInterfaceUnregisteredReason");
      return;
    }
  }
}


void ApplicationManagerImpl::SendOnSDLClose() {
  LOG4CXX_AUTO_TRACE(logger_);

  // must be sent to PASA HMI on shutdown synchronously
  smart_objects::SmartObjectSPtr msg = new smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  (*msg)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_OnSDLClose;
  (*msg)[strings::params][strings::message_type] =
      MessageType::kNotification;
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

  hmi_so_factory().attachSchema(*msg);
  LOG4CXX_INFO(
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

  bool is_unexpected_disconnect =
      Compare<eType, NEQ, ALL>(unregister_reason_,
                               IGNITION_OFF, MASTER_RESET, FACTORY_DEFAULTS);
  ApplicationListAccessor accessor;
  ApplictionSetConstIt it = accessor.begin();
  while (it != accessor.end()) {
    ApplicationSharedPtr app_to_remove = *it;

    UnregisterApplication(app_to_remove->app_id(),
                          mobile_apis::Result::INVALID_ENUM, is_ignition_off,
                          is_unexpected_disconnect);
    connection_handler_->CloseSession(app_to_remove->app_id(),
                                      connection_handler::kCommon);
    it = accessor.begin();
  }
  if (is_ignition_off) {
    resume_controller().Suspend();
  }
  request_ctrl_.terminateAllHMIRequests();
}

void ApplicationManagerImpl::UnregisterApplication(
  const uint32_t& app_id, mobile_apis::Result::eType reason,
  bool is_resuming, bool is_unexpected_disconnect) {
  LOG4CXX_INFO(logger_, "app_id = " << app_id
               << "; reason = " << reason
               << "; is_resuming = " << is_resuming
               << "; is_unexpected_disconnect = " << is_unexpected_disconnect);
  //remove appID from tts_global_properties_app_list_
    MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
          app_id, unregister_reason_);

  RemoveAppFromTTSGlobalPropertiesList(app_id);

  switch (reason) {
    case mobile_apis::Result::SUCCESS:break;
    case mobile_apis::Result::DISALLOWED: break;
    case mobile_apis::Result::USER_DISALLOWED:break;
    case mobile_apis::Result::INVALID_CERT: break;
    case mobile_apis::Result::EXPIRED_CERT: break;
    case mobile_apis::Result::TOO_MANY_PENDING_REQUESTS: {
        ApplicationSharedPtr app_ptr = application(app_id);
        if(app_ptr) {
          app_ptr->usage_report().RecordRemovalsForBadBehavior();
        }
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown unregister reason " << reason);
      break;
    }
  }

  ApplicationSharedPtr app_to_remove;
  {
    ApplicationListAccessor accessor;
    ApplictionSetConstIt it = accessor.begin();
    for (; it != accessor.end(); ++it) {
      if ((*it)->app_id() == app_id) {
        app_to_remove = *it;
        break;
      }
    }
    if (!app_to_remove) {
      LOG4CXX_ERROR(logger_, "Cant find application with app_id = " << app_id);
      return;
    }
    accessor.Erase(app_to_remove);
  }
  if (is_resuming) {
      resume_ctrl_.SaveApplication(app_to_remove);
  } else {
    resume_ctrl_.RemoveApplicationFromSaved(app_to_remove->mobile_app_id());
  }

  if (audio_pass_thru_active_) {
    // May be better to put this code in MessageHelper?
    end_audio_pass_thru();
    StopAudioPassThru(app_id);
    MessageHelper::SendStopAudioPathThru();
  }
  MessageHelper::SendOnAppUnregNotificationToHMI(app_to_remove,
                                                 is_unexpected_disconnect);

  request_ctrl_.terminateAppRequests(app_id);
  return;
}


void ApplicationManagerImpl::OnAppUnauthorized(const uint32_t& app_id) {
  connection_handler_->CloseSession(app_id, connection_handler::kCommon);
}

void ApplicationManagerImpl::Handle(const impl::MessageFromMobile message) {
  LOG4CXX_INFO(logger_, "Received message from Mobile side");

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }
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
    if (1 < connection_handler_->GetConnectionSessionsCount(message->connection_key())) {
      is_final = false;
      close_session = true;
    }
  }

  protocol_handler_->SendMessageToMobileApp(rawMessage, is_final);
  LOG4CXX_INFO(logger_, "Message for mobile given away");

  if (close_session) {
    connection_handler_->CloseSession(message->connection_key(),
                                      connection_handler::kCommon);
  }
}

void ApplicationManagerImpl::Handle(const impl::MessageFromHmi message) {
  LOG4CXX_INFO(logger_, "Received message from hmi");

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }

  ProcessMessageFromHMI(message);
}

void ApplicationManagerImpl::Handle(const impl::MessageToHmi message) {
  LOG4CXX_INFO(logger_, "Received message to hmi");
  if (!hmi_handler_) {
    LOG4CXX_ERROR(logger_, "Observer is not set for HMIMessageHandler");
    return;
  }

  hmi_handler_->SendMessageToHMI(message);
  LOG4CXX_INFO(logger_, "Message to hmi given away.");
}

void ApplicationManagerImpl::Handle(const impl::AudioData message) {
  LOG4CXX_INFO(logger_, "Send AudioPassThru notification");
  smart_objects::SmartObjectSPtr on_audio_pass = new smart_objects::SmartObject();

  if (!on_audio_pass) {
    LOG4CXX_ERROR_EXT(logger_, "OnAudioPassThru NULL pointer");
    return;
  }

  LOG4CXX_INFO_EXT(logger_, "Fill smart object");

  (*on_audio_pass)[strings::params][strings::message_type] =
      application_manager::MessageType::kNotification;

  (*on_audio_pass)[strings::params][strings::connection_key] =
      static_cast<int32_t>(message.session_key);
  (*on_audio_pass)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnAudioPassThruID;

  LOG4CXX_INFO_EXT(logger_, "Fill binary data");
  // binary data
  (*on_audio_pass)[strings::params][strings::binary_data] =
      smart_objects::SmartObject(message.binary_data);

   LOG4CXX_INFO_EXT(logger_, "After fill binary data");

   LOG4CXX_INFO_EXT(logger_, "Send data");
   CommandSharedPtr command (
       MobileCommandFactory::CreateCommand(on_audio_pass,
                                           commands::Command::ORIGIN_SDL));
   command->Init();
   command->Run();
   command->CleanUp();
}

mobile_apis::Result::eType ApplicationManagerImpl::CheckPolicyPermissions(
    const std::string& policy_app_id,
    mobile_apis::HMILevel::eType hmi_level,
    mobile_apis::FunctionID::eType function_id,
    const RPCParams& rpc_params,
    CommandParametersPermissions* params_permissions) {
  LOG4CXX_INFO(logger_, "CheckPolicyPermissions");
  // TODO(AOleynik): Remove check of policy_enable, when this flag will be
  // unused in config file
  if (!policy::PolicyHandler::instance()->PolicyEnabled()) {
    return mobile_apis::Result::SUCCESS;
  }

  const std::string stringified_functionID =
      MessageHelper::StringifiedFunctionID(function_id);
  const std::string stringified_hmi_level =
      MessageHelper::StringifiedHMILevel(hmi_level);
  LOG4CXX_INFO(
    logger_,
    "Checking permissions for  " << policy_app_id  <<
    " in " << stringified_hmi_level <<
    " rpc " << stringified_functionID);
    policy::CheckPermissionResult result;
    policy::PolicyHandler::instance()->CheckPermissions(
          policy_app_id,
          stringified_hmi_level,
          stringified_functionID,
          rpc_params,
          result);

  if (NULL != params_permissions) {
      params_permissions->allowed_params = result.list_of_allowed_params;
      params_permissions->disallowed_params = result.list_of_disallowed_params;
      params_permissions->undefined_params = result.list_of_undefined_params;
  }

  if (hmi_level == mobile_apis::HMILevel::HMI_NONE
      && function_id != mobile_apis::FunctionID::UnregisterAppInterfaceID) {
    ApplicationSharedPtr app = application_by_policy_id(policy_app_id);
    if (!app) {
      LOG4CXX_ERROR(logger_, "No application for policy id " << policy_app_id);
      return mobile_apis::Result::GENERIC_ERROR;
    }
    if (result.hmi_level_permitted != policy::kRpcAllowed) {
      app->usage_report().RecordRpcSentInHMINone();
    }
  }

  const std::string log_msg = "Application: "+ policy_app_id+
                              ", RPC: "+stringified_functionID+
                              ", HMI status: "+stringified_hmi_level;

  if (result.hmi_level_permitted != policy::kRpcAllowed) {
    LOG4CXX_WARN(logger_, "Request is blocked by policies. " << log_msg );

    application_by_policy_id(policy_app_id)->
        usage_report().RecordPolicyRejectedRpcCall();

    switch (result.hmi_level_permitted) {
      case policy::kRpcDisallowed:
        return mobile_apis::Result::DISALLOWED;
      case policy::kRpcUserDisallowed:
        return mobile_apis::Result::USER_DISALLOWED;
      default:
        return mobile_apis::Result::INVALID_ENUM;
    }
  }
  LOG4CXX_INFO(logger_, "Request is allowed by policies. "+log_msg);
  return mobile_api::Result::SUCCESS;
}


void  ApplicationManagerImpl::OnLowVoltage() {
    LOG4CXX_AUTO_TRACE(logger_);
    is_low_voltage_ = true;
    request_ctrl_.OnLowVoltage();
}

bool ApplicationManagerImpl::IsLowVoltage() {
  LOG4CXX_TRACE(logger_, "result: " << is_low_voltage_);
  return is_low_voltage_;
}

void ApplicationManagerImpl::NaviAppStreamStatus(bool stream_active) {
  ApplicationSharedPtr active_app = active_application();
  using namespace mobile_apis;
  if(active_app && active_app->is_media_application()) {
    LOG4CXX_DEBUG(logger_, "Stream status: " << active_app->app_id());

    active_app->set_audio_streaming_state(stream_active ?
                                            AudioStreamingState::ATTENUATED :
                                            AudioStreamingState::AUDIBLE);
    MessageHelper::SendHMIStatusNotification(*active_app);
  }
}

void ApplicationManagerImpl::ForbidStreaming(uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace protocol_handler;
  ApplicationSharedPtr app = application(app_id);
  if (!(app && app->is_navi())) {
    LOG4CXX_DEBUG(logger_, " There is no application with id: " << app_id);
    return;
  }

  if (connection_handler_) {
    const bool send_end_service = true;
    const bool ack_received = false;
    if (app->hmi_supports_navi_video_streaming()) {
      LOG4CXX_DEBUG(logger_, "Going to end video service");
      connection_handler_->SendEndService(navi_app_to_stop_, kMobileNav);
      service_status_[kMobileNav] = std::make_pair(send_end_service, ack_received);
    }
    if (app->hmi_supports_navi_audio_streaming()) {
      LOG4CXX_DEBUG(logger_, "Going to end audio service");
      connection_handler_->SendEndService(navi_app_to_stop_, kAudio);
      service_status_[kAudio] = std::make_pair(send_end_service, ack_received);
    }
  }
  // this timer will check if appropriate acks from mobile were received.
  // in case no acks, the application will be unregistered.
  end_services_timer.start(wait_end_service_timeout_, this, &ApplicationManagerImpl::CloseNaviApp);
  bool const allow_streaming = false;
  ChangeStreamStatus(app_id, allow_streaming);
}

bool ApplicationManagerImpl::CanAppStream(uint32_t app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application(app_id);
  if (!(app && app->is_navi())) {
    LOG4CXX_DEBUG(logger_, " There is no application with id: " << app_id);
    return false;
  }

  return app->is_streaming_allowed();
}

void ApplicationManagerImpl::ChangeStreamStatus(uint32_t app_id, bool can_stream) {
  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_DEBUG(logger_, " There is no application with id: " << app_id);
    return;
  }

  // Change streaming status only in case incoming value is different.
  if (can_stream != app->streaming()) {
    NaviAppStreamStatus(can_stream);
    app->set_streaming(can_stream);
  }
}

void ApplicationManagerImpl::StreamingEnded(uint32_t app_id) {
  LOG4CXX_DEBUG(logger_, "Streaming has been stoped.");
  ChangeStreamStatus(app_id, false);
}

void ApplicationManagerImpl::OnHMILevelChanged(uint32_t app_id,
                                               mobile_apis::HMILevel::eType from,
                                               mobile_apis::HMILevel::eType to) {
  using namespace mobile_apis::HMILevel;
  using namespace helpers;

  ApplicationSharedPtr app = application(app_id);
  if (!(app && app->is_navi())) {
    return;
  }

  if (Compare<eType, EQ, ONE>(from, HMI_FULL, HMI_LIMITED)) {
    navi_app_to_stop_ = app_id;
    NaviAppChangeLevel(to);
  } else if (Compare<eType, EQ, ONE>(to, HMI_FULL, HMI_LIMITED)) {
    LOG4CXX_DEBUG(logger_, "Restore streaming ability");
    app->set_streaming_allowed(true);
  }
}

void ApplicationManagerImpl::EndNaviServices() {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app = application(navi_app_to_stop_);
  if (!app) {
    LOG4CXX_DEBUG(logger_, "The application doesn't exists anymore.");
    return;
  }
  app->set_streaming_allowed(false);
}

void ApplicationManagerImpl::CloseNaviApp() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis::AppInterfaceUnregisteredReason;
  using namespace mobile_apis::Result;
  using namespace protocol_handler;
  const bool is_ack_received = AckReceived(kAudio) && AckReceived(kMobileNav);
  if (!is_ack_received) {
    SetUnregisterAllApplicationsReason(PROTOCOL_VIOLATION);
    UnregisterApplication(navi_app_to_stop_, ABORTED);
  }
}

bool ApplicationManagerImpl::AckReceived(protocol_handler::ServiceType type) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace protocol_handler;

  const bool sent = service_status_[type].first;
  const bool received = service_status_[type].second;

  LOG4CXX_DEBUG(logger_, "Ack for services type " << type
                << " is send: " << sent
                << " is received: " << received);

  return sent == received;
}

void ApplicationManagerImpl::NaviAppChangeLevel(mobile_apis::HMILevel::eType new_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  if (new_level == HMILevel::HMI_BACKGROUND) {
    end_services_timer.start(wait_end_service_timeout_, this, &ApplicationManagerImpl::EndNaviServices);
  } else if (new_level == HMILevel::HMI_NONE) {
    EndNaviServices();
    LOG4CXX_DEBUG(logger_, "Send end services start close app timer");
    end_services_timer.start(wait_end_service_timeout_, this, &ApplicationManagerImpl::CloseNaviApp);
  } else {
    LOG4CXX_DEBUG(logger_, "There is no defined behavior for hmi " <<
                  "levels that are differen from NONE or BACKGROUND");
  }
}

void ApplicationManagerImpl::OnWakeUp() {
    LOG4CXX_AUTO_TRACE(logger_);
    is_low_voltage_ = false;
    request_ctrl_.OnWakeUp();
}

void ApplicationManagerImpl::Mute(VRTTSSessionChanging changing_state) {
  mobile_apis::AudioStreamingState::eType state =
      mobile_apis::AudioStreamingState::NOT_AUDIBLE;

  // ATTENUATED state applicable only for TTS
  if ((kTTSSessionChanging == changing_state) &&
       hmi_capabilities_.attenuated_supported()) {
    state = mobile_apis::AudioStreamingState::ATTENUATED;
  }

  ApplicationManagerImpl::ApplicationListAccessor accessor;

  ApplicationManagerImpl::ApplictionSetConstIt it =
      accessor.begin();
  ApplicationManagerImpl::ApplictionSetConstIt
      itEnd = accessor.end();
  for (; it != itEnd; ++it) {
    if ((*it).valid()) {
      if ((*it)->is_media_application()) {
        if (kTTSSessionChanging == changing_state) {
          (*it)->set_tts_speak_state(true);
        }
        if ((*it)->audio_streaming_state() != state &&
            (mobile_api::HMILevel::HMI_NONE != (*it)->hmi_level()) &&
            (mobile_api::HMILevel::HMI_BACKGROUND != (*it)->hmi_level())) {
          (*it)->set_audio_streaming_state(state);
          MessageHelper::SendHMIStatusNotification(*(*it));
        }
      }
    }
  }
}

void ApplicationManagerImpl::Unmute(VRTTSSessionChanging changing_state) {

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  ApplicationManagerImpl::ApplictionSetConstIt it = accessor.begin();
  ApplicationManagerImpl::ApplictionSetConstIt itEnd = accessor.end();

  for (; it != itEnd; ++it) {
    if ((*it).valid()) {
      if ((*it)->is_media_application()) {
        if (kTTSSessionChanging == changing_state) {
          (*it)->set_tts_speak_state(false);
        }
        if ((!(vr_session_started())) &&
            (!((*it)->tts_speak_state())) &&
            ((*it)->audio_streaming_state() !=
             mobile_apis::AudioStreamingState::AUDIBLE) &&
            (mobile_api::HMILevel::HMI_NONE != (*it)->hmi_level()) &&
            (mobile_api::HMILevel::HMI_BACKGROUND != (*it)->hmi_level())) {
          (*it)->set_audio_streaming_state(
              mobile_apis::AudioStreamingState::AUDIBLE);
          MessageHelper::SendHMIStatusNotification(*(*it));
        }
      }
    }
  }
}

mobile_apis::Result::eType ApplicationManagerImpl::SaveBinary(
  const std::vector<uint8_t>& binary_data, const std::string& file_path,
  const std::string& file_name, const int64_t offset) {
  LOG4CXX_INFO(logger_,
               "SaveBinaryWithOffset  binary_size = " << binary_data.size()
               << " offset = " << offset);

  if (binary_data.size() > file_system::GetAvailableDiskSpace(file_path)) {
    LOG4CXX_ERROR(logger_, "Out of free disc space.");
    return mobile_apis::Result::OUT_OF_MEMORY;
  }

  const std::string full_file_path = file_path + "/" + file_name;
  int64_t file_size = file_system::FileSize(full_file_path);
  std::ofstream* file_stream;
  if (offset != 0) {
    if (file_size != offset) {
      LOG4CXX_INFO(logger_,
                   "ApplicationManagerImpl::SaveBinaryWithOffset offset"
                   << " does'n match existing file size");
      return mobile_apis::Result::INVALID_DATA;
    }
    file_stream = file_system::Open(full_file_path, std::ios_base::app);
  } else {
    LOG4CXX_INFO(
      logger_,
      "ApplicationManagerImpl::SaveBinaryWithOffset offset is 0, rewrite");
    // if offset == 0: rewrite file
    file_stream = file_system::Open(full_file_path, std::ios_base::out);
  }

  if (!file_system::Write(file_stream, binary_data.data(),
                          binary_data.size())) {
    file_system::Close(file_stream);
    return mobile_apis::Result::GENERIC_ERROR;
  }

  file_system::Close(file_stream);
  LOG4CXX_INFO(logger_, "Successfully write data to file");
  return mobile_apis::Result::SUCCESS;
}

uint32_t ApplicationManagerImpl::GetAvailableSpaceForApp(
  const std::string& folder_name) {
  const uint32_t app_quota = profile::Profile::instance()->app_dir_quota();
  std::string app_storage_path =
    profile::Profile::instance()->app_storage_folder();

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
  SendUpdateAppList();
  policy::PolicyHandler::instance()->OnAppsSearchCompleted();
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
      LOG4CXX_INFO(logger_, "Send TTS GlobalProperties to HMI with default helpPrompt");
      MessageHelper::SendTTSGlobalProperties(application(app_list[i]), true);
      RemoveAppFromTTSGlobalPropertiesList(app_list[i]);
    }
  }
}

void ApplicationManagerImpl::AddAppToTTSGlobalPropertiesList(
    const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint16_t timeout = profile::Profile::instance()->tts_global_properties_timeout();
  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  current_time.tv_sec += timeout;
  // please avoid AutoLock usage to avoid deadlock
  tts_global_properties_app_list_lock_.Acquire();
  if (tts_global_properties_app_list_.end() ==
      tts_global_properties_app_list_.find(app_id)) {
    tts_global_properties_app_list_[app_id] = current_time;
  }
  //if add first item need to start timer on one second
  if (1 == tts_global_properties_app_list_.size()) {
    LOG4CXX_INFO(logger_, "Start tts_global_properties_timer_");
    tts_global_properties_app_list_lock_.Release();
    tts_global_properties_timer_.start(1);
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
      LOG4CXX_INFO(logger_, "Stop tts_global_properties_timer_");
      // if container is empty need to stop timer
      tts_global_properties_app_list_lock_.Release();
      tts_global_properties_timer_.pause();
      return;
    }
  }
  tts_global_properties_app_list_lock_.Release();
}

void ApplicationManagerImpl::CreatePhoneCallAppList() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationManagerImpl::ApplicationListAccessor accessor;

  ApplicationManagerImpl::ApplictionSetIt it = accessor.begin();
  ApplicationManagerImpl::ApplictionSetIt itEnd = accessor.end();

  using namespace mobile_apis::HMILevel;
  using namespace helpers;
  for (; it != itEnd; ++it) {
    if (Compare<eType, EQ, ONE>((*it)->hmi_level(), HMI_FULL, HMI_LIMITED)) {

      // back up app state
      on_phone_call_app_list_.insert(std::pair<uint32_t, AppState>(
          (*it)->app_id(), AppState((*it)->hmi_level(),
                                    (*it)->audio_streaming_state(),
                                    (*it)->system_context())));

      ChangeAppsHMILevel((*it)->app_id() , (*it)->is_navi() ? HMI_LIMITED : HMI_BACKGROUND);

      // app state during phone call
      (*it)->set_audio_streaming_state(mobile_api::AudioStreamingState::NOT_AUDIBLE);
      (*it)->set_system_context(mobile_api::SystemContext::SYSCTXT_MAIN);
      MessageHelper::SendHMIStatusNotification(*(*it));
    }
  }
}

void ApplicationManagerImpl::ResetPhoneCallAppList() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::map<uint32_t, AppState>::iterator it =
      on_phone_call_app_list_.begin();
  std::map<uint32_t, AppState>::iterator it_end =
      on_phone_call_app_list_.end();
  for (; it != it_end; ++it) {
    ApplicationSharedPtr app = application(it->first);
    if (app) {
      ChangeAppsHMILevel(app->app_id(), it->second.hmi_level);

      app->set_audio_streaming_state(it->second.audio_streaming_state);
      app->set_system_context(it->second.system_context);
      MessageHelper::SendHMIStatusNotification(*app);
    }
  }

  on_phone_call_app_list_.clear();
}

void ApplicationManagerImpl::ChangeAppsHMILevel(uint32_t app_id,
                                                mobile_apis::HMILevel::eType level) {
  using namespace mobile_apis::HMILevel;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "AppID to change: " << app_id << " -> "
                << level);
  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_DEBUG(logger_, "There is no app with id: " << app_id);
    return;
  }
  eType old_level = app->hmi_level();
  if (old_level != level) {
    app->set_hmi_level(level);
    OnHMILevelChanged(app_id, old_level, level);
  } else {
    LOG4CXX_WARN(logger_, "Redudant changing HMI level : " << level);
  }

}

void ApplicationManagerImpl::MakeAppNotAudible(uint32_t app_id) {
  using namespace mobile_apis;
  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_DEBUG(logger_, "There is no app with id: " << app_id);
    return;
  }
  ChangeAppsHMILevel(app_id, HMILevel::HMI_BACKGROUND);
  app->set_audio_streaming_state(AudioStreamingState::NOT_AUDIBLE);
}

bool ApplicationManagerImpl::MakeAppFullScreen(uint32_t app_id) {
  using namespace mobile_apis;
  ApplicationSharedPtr app = application(app_id);
  if (!app) {
    LOG4CXX_DEBUG(logger_, "There is no app with id: " << app_id);
    return false;
  }

  ChangeAppsHMILevel(app_id, HMILevel::HMI_FULL);
  if (app->is_media_application() || app->is_navi()) {
    app->set_audio_streaming_state(AudioStreamingState::AUDIBLE);
  }
  app->set_system_context(SystemContext::SYSCTXT_MAIN);

  if(!app->has_been_activated()) {
    app->set_activated(true);
  }

  return true;
}


mobile_apis::AppHMIType::eType ApplicationManagerImpl::StringToAppHMIType(std::string str) {
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

bool ApplicationManagerImpl::CompareAppHMIType (const smart_objects::SmartObject& from_policy,
                                                const smart_objects::SmartObject& from_application) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool equal = false;
  uint32_t lenght_policy_app_types = from_policy.length();
  uint32_t lenght_application_app_types = from_application.length();

  for(uint32_t i = 0; i < lenght_application_app_types; ++i) {
    for(uint32_t k = 0; k < lenght_policy_app_types; ++k) {
      if (from_application[i] == from_policy[k]) {
        equal = true;
        break;
      }
    }
    if(!equal) {
      return false;
    }
    equal = false;
  }
  return true;
}

void ApplicationManagerImpl::OnUpdateHMIAppType(
    std::map<std::string, std::vector<std::string> > app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);

  std::map<std::string, std::vector<std::string> >::iterator it_app_hmi_types_from_policy;
  std::vector<std::string> hmi_types_from_policy;
  smart_objects::SmartObject transform_app_hmi_types(smart_objects::SmartType_Array);
  bool flag_diffirence_app_hmi_type = false;
  ApplicationListAccessor accessor;
  for (ApplictionSetIt it = accessor.begin();
      it != accessor.end(); ++it) {

    it_app_hmi_types_from_policy =
        app_hmi_types.find(((*it)->mobile_app_id()));

    if (it_app_hmi_types_from_policy != app_hmi_types.end() &&
        ((it_app_hmi_types_from_policy->second).size())) {
      flag_diffirence_app_hmi_type = false;
      hmi_types_from_policy = (it_app_hmi_types_from_policy->second);

      if(transform_app_hmi_types.length()) {
        transform_app_hmi_types =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
      }

      for(uint32_t i = 0; i < hmi_types_from_policy.size(); ++i) {
        transform_app_hmi_types[i] = StringToAppHMIType(hmi_types_from_policy[i]);
      }

      ApplicationConstSharedPtr app = *it;
      const smart_objects::SmartObject* save_application_hmi_type = app->app_types();

      if (save_application_hmi_type == NULL ||
          ((*save_application_hmi_type).length() != transform_app_hmi_types.length())) {
        flag_diffirence_app_hmi_type = true;
      } else {
        flag_diffirence_app_hmi_type = !(CompareAppHMIType(transform_app_hmi_types,
                                                        *save_application_hmi_type));
      }

      if (flag_diffirence_app_hmi_type) {
        (*it)->set_app_types(transform_app_hmi_types);
        (*it)->ChangeSupportingAppHMIType();
        if ((*it)->hmi_level() == mobile_api::HMILevel::HMI_BACKGROUND) {

          MessageHelper::SendUIChangeRegistrationRequestToHMI(*it);
        } else if (((*it)->hmi_level() == mobile_api::HMILevel::HMI_FULL) ||
            ((*it)->hmi_level() == mobile_api::HMILevel::HMI_LIMITED)) {

          MessageHelper::SendActivateAppToHMI((*it)->app_id(),
                                              hmi_apis::Common_HMILevel::BACKGROUND,
                                              false);
          MessageHelper::SendUIChangeRegistrationRequestToHMI(*it);
          ChangeAppsHMILevel((*it)->app_id(), mobile_api::HMILevel::HMI_BACKGROUND);
          MessageHelper::SendHMIStatusNotification(*(*it));
        }
      }
    }
  }
}

ProtocolVersion ApplicationManagerImpl::SupportedSDLVersion() const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool heart_beat_support =
    profile::Profile::instance()->heart_beat_timeout();
  bool sdl4_support = profile::Profile::instance()->enable_protocol_4();

  if (sdl4_support) {
    LOG4CXX_DEBUG(logger_, "SDL Supported protocol version "<<ProtocolVersion::kV4);
    return ProtocolVersion::kV4;
  }
  if (heart_beat_support) {
    LOG4CXX_DEBUG(logger_, "SDL Supported protocol version "<<ProtocolVersion::kV3);
    return ProtocolVersion::kV3;
  }

  LOG4CXX_DEBUG(logger_, "SDL Supported protocol version "<<ProtocolVersion::kV2);
  return ProtocolVersion::kV2;
}


ApplicationManagerImpl::ApplicationListAccessor::~ApplicationListAccessor() {
}

}  // namespace application_manager
