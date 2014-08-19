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
#include "application_manager/application_impl.h"
#include "usage_statistics/counter.h"
#include <time.h>

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

uint32_t ApplicationManagerImpl::corelation_id_ = 0;
const uint32_t ApplicationManagerImpl::max_corelation_id_ = UINT_MAX;

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

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
    policy_manager_(policy::PolicyHandler::instance()->LoadPolicyLibrary()),
    protocol_handler_(NULL),
    request_ctrl_(),
    hmi_so_factory_(NULL),
    mobile_so_factory_(NULL),
    messages_from_mobile_("application_manager::FromMobileThreadImpl", this),
    messages_to_mobile_("application_manager::ToMobileThreadImpl", this),
    messages_from_hmi_("application_manager::FromHMHThreadImpl", this),
    messages_to_hmi_("application_manager::ToHMHThreadImpl", this),
    hmi_capabilities_(this),
    unregister_reason_(mobile_api::AppInterfaceUnregisteredReason::IGNITION_OFF),
    resume_ctrl_(this),
#ifdef TIME_TESTER
    metric_observer_(NULL),
#endif  // TIME_TESTER
    application_list_update_timer_(new ApplicationListUpdateTimer(this))
{
}

bool ApplicationManagerImpl::InitThread(threads::Thread* thread) {
  if (!thread) {
    LOG4CXX_ERROR(logger_, "Failed to allocate memory for thread object");
    return false;
  }
  LOG4CXX_INFO(
    logger_,
    "Starting thread with stack size "
    << profile::Profile::instance()->thread_min_stack_size());
  if (!thread->start()) {
    /*startWithOptions(
     threads::ThreadOptions(
     profile::Profile::instance()->thread_min_stack_size()))*/
    LOG4CXX_ERROR(logger_, "Failed to start thread");
    return false;
  }
  return true;
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  LOG4CXX_INFO(logger_, "Destructing ApplicationManager.");

  if (policy_manager_) {
    LOG4CXX_INFO(logger_, "Unloading policy library.");
    policy::PolicyHandler::instance()->UnloadPolicyLibrary();
  }

  SendOnSDLClose();

  policy_manager_ = NULL;
  media_manager_ = NULL;
  hmi_handler_ = NULL;
  connection_handler_ = NULL;
  if (hmi_so_factory_) {
    delete hmi_so_factory_;
  }
  hmi_so_factory_ = NULL;
  if (mobile_so_factory_) {
    delete mobile_so_factory_;
  }
  mobile_so_factory_ = NULL;
  protocol_handler_ = NULL;
  media_manager_ = NULL;
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

  return true;
}

ApplicationSharedPtr ApplicationManagerImpl::application(uint32_t app_id) const {
  sync_primitives::AutoLock lock(applications_list_lock_);

  std::set<ApplicationSharedPtr>::const_iterator it =
    application_list_.begin();
  for (; it != application_list_.end(); ++it) {
    if ((*it)->app_id() == app_id) {
      return (*it);
    }
  }
  return ApplicationSharedPtr();
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_hmi_app(
  uint32_t hmi_app_id) const {
  sync_primitives::AutoLock lock(applications_list_lock_);

  std::set<ApplicationSharedPtr>::const_iterator it =
    application_list_.begin();
  for (; it != application_list_.end(); ++it) {
    if ((*it)->hmi_app_id() == hmi_app_id) {
      return (*it);
    }
  }
  return ApplicationSharedPtr();
}

ApplicationSharedPtr ApplicationManagerImpl::application_by_policy_id(
  const std::string& policy_app_id) const {
  sync_primitives::AutoLock lock(applications_list_lock_);

  std::vector<ApplicationSharedPtr> result;
  for (std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if (policy_app_id.compare((*it)->mobile_app_id()->asString()) == 0) {
      return *it;
    }
  }
  return ApplicationSharedPtr();
}

ApplicationSharedPtr ApplicationManagerImpl::active_application() const {
  // TODO(DK) : check driver distraction
  for (std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->IsFullscreen()) {
      return *it;
    }
  }
  return ApplicationSharedPtr();
}

std::vector<ApplicationSharedPtr> ApplicationManagerImpl::applications_by_button(
  uint32_t button) {
  std::vector<ApplicationSharedPtr> result;
  for (std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
       application_list_.end() != it; ++it) {
    if ((*it)->IsSubscribedToButton(
          static_cast<mobile_apis::ButtonName::eType>(button))) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<utils::SharedPtr<Application>> ApplicationManagerImpl::IviInfoUpdated(
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

  std::vector<utils::SharedPtr<application_manager::Application>> result;
  for (std::set<utils::SharedPtr<application_manager::Application>>::iterator it = application_list_.begin();
       application_list_.end() != it; ++it) {
    if ((*it)->IsSubscribedToIVI(static_cast<uint32_t>(vehicle_info))) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<ApplicationSharedPtr> ApplicationManagerImpl::applications_with_navi() {
  std::vector<ApplicationSharedPtr> result;
  for (std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->allowed_support_navigation()) {
      result.push_back(*it);
    }
  }
  return result;
}

ApplicationSharedPtr ApplicationManagerImpl::RegisterApplication(
  const utils::SharedPtr<smart_objects::SmartObject>&
  request_for_registration) {

  LOG4CXX_DEBUG(logger_, "Restarting application list update timer");
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
    new ApplicationImpl(app_id, mobile_app_id, app_name, policy_manager_));
  if (!application) {
    usage_statistics::AppCounter count_of_rejections_sync_out_of_memory(
      policy_manager_, mobile_app_id,
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

  /*if (min_version < APIVersion::kAPIV2) {
    LOG4CXX_ERROR(logger_, "UNSUPPORTED_VERSION");
    utils::SharedPtr<smart_objects::SmartObject> response(
      MessageHelper::CreateNegativeResponse(
        connection_key, mobile_apis::FunctionID::RegisterAppInterfaceID,
        message[strings::params][strings::correlation_id],
        mobile_apis::Result::UNSUPPORTED_VERSION));
    ManageMobileCommand(response);
    delete application;
    return NULL;
  }*/
  version.min_supported_api_version = static_cast<APIVersion>(min_version);

  int32_t max_version =
    message[strings::msg_params][strings::sync_msg_version]
    [strings::major_version].asInt();

  /*if (max_version > APIVersion::kAPIV2) {
    LOG4CXX_ERROR(logger_, "UNSUPPORTED_VERSION");
    utils::SharedPtr<smart_objects::SmartObject> response(
      MessageHelper::CreateNegativeResponse(
        connection_key, mobile_apis::FunctionID::RegisterAppInterfaceID,
        message[strings::params][strings::correlation_id],
        mobile_apis::Result::UNSUPPORTED_VERSION));
    ManageMobileCommand(response);
    delete application;
    return NULL;
  }*/
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
    if (ProtocolVersion::kV3 == protocol_version) {
      connection_handler_->StartSessionHeartBeat(connection_key);
    }
  }

  sync_primitives::AutoLock lock(applications_list_lock_);

  application_list_.insert(application);

  return application;
}

bool ApplicationManagerImpl::RemoveAppDataFromHMI(ApplicationSharedPtr app) {
  return true;
}

bool ApplicationManagerImpl::LoadAppDataToHMI(ApplicationSharedPtr app) {
  return true;
}

bool ApplicationManagerImpl::ActivateApplication(ApplicationSharedPtr app) {
  if (!app) {
    LOG4CXX_ERROR(logger_, "Null-pointer application received.");
    NOTREACHED();
    return false;
  }

  bool is_new_app_media = app->is_media_application();

  if (app->IsFullscreen()) {
    LOG4CXX_WARN(logger_, "Application is already active.");
    return false;
  }
  if (mobile_api::HMILevel::eType::HMI_LIMITED !=
      app->hmi_level()) {
    if (app->has_been_activated()) {
      MessageHelper::SendAppDataToHMI(app);
    }
  }
  app->MakeFullscreen();
  for (std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    ApplicationSharedPtr curr_app = *it;
    if (app->app_id() == curr_app->app_id()) {
      continue;
    } else {
      if (is_new_app_media) {
        if (curr_app->IsAudible()) {
          curr_app->MakeNotAudible();
          MessageHelper::SendHMIStatusNotification(*curr_app);
        }
      }
      if (curr_app->IsFullscreen()) {
        if (true == curr_app->is_media_application()) {
          PutApplicationInLimited(app);
        } else {
          app->set_hmi_level(mobile_api::HMILevel::HMI_BACKGROUND);
        }
        MessageHelper::ResetGlobalproperties(curr_app);
        MessageHelper::SendHMIStatusNotification(*curr_app);
      }
    }
  }
  return true;
}

mobile_apis::HMILevel::eType ApplicationManagerImpl::PutApplicationInLimited(
  ApplicationSharedPtr app) {
  DCHECK(app.get())

  mobile_api::HMILevel::eType result = mobile_api::HMILevel::HMI_LIMITED;

  for (std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    ApplicationSharedPtr curr_app = *it;
    if (app->app_id() == curr_app->app_id()) {
      continue;
    }

    if (curr_app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED) {
      result = mobile_api::HMILevel::HMI_BACKGROUND;
      break;
    }
    if (curr_app->hmi_level() == mobile_api::HMILevel::HMI_FULL) {
      if (curr_app->is_media_application()) {
        result = mobile_api::HMILevel::HMI_BACKGROUND;
        break;
      } else {
        result = mobile_api::HMILevel::HMI_LIMITED;
      }
    }

  }
  app->set_hmi_level(result);
  return result;
}

mobile_api::HMILevel::eType ApplicationManagerImpl::PutApplicationInFull(
  ApplicationSharedPtr app) {
  DCHECK(app.get())

  bool is_new_app_media = app->is_media_application();
  mobile_api::HMILevel::eType result = mobile_api::HMILevel::HMI_FULL;

  std::set<ApplicationSharedPtr>::iterator it = application_list_.begin();
  for (; application_list_.end() != it; ++it) {
    ApplicationSharedPtr curr_app = *it;
    if (app->app_id() == curr_app->app_id()) {
      continue;
    }

    if (is_new_app_media) {
      if (curr_app->hmi_level() == mobile_api::HMILevel::HMI_FULL) {
        if (curr_app->is_media_application()) {
          result = mobile_api::HMILevel::HMI_BACKGROUND;
          break;
        } else {
          result = mobile_api::HMILevel::HMI_LIMITED;
        }
      }
      if (curr_app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED) {
        result = mobile_api::HMILevel::HMI_BACKGROUND;
        break;
      }
    } else {
      if (curr_app->hmi_level() == mobile_api::HMILevel::HMI_FULL) {
        result = mobile_api::HMILevel::HMI_BACKGROUND;
        break;
      }
      if (curr_app->hmi_level() == mobile_api::HMILevel::HMI_LIMITED) {
        result = mobile_api::HMILevel::HMI_FULL;
      }
    }
  }

  if (result == mobile_api::HMILevel::HMI_FULL) {
    app->set_hmi_level(result);
    MessageHelper::SendActivateAppToHMI(app->app_id());
  }
  return result;
}

void ApplicationManagerImpl::DeactivateApplication(ApplicationSharedPtr app) {
  MessageHelper::ResetGlobalproperties(app);
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
#ifdef CUSTOMER_PASA
  //Line start of transportManager component was left for panasonic
  if (!connection_handler_) {
    LOG4CXX_WARN(logger_, "Connection handler is not set.");
  } else {
    connection_handler_->StartTransportManager();
  }
#endif // CUSTOMER_PASA
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
  std::vector<uint8_t> binaryData) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!audio_pass_thru_active_) {
    LOG4CXX_ERROR(logger_, "Trying to send PassThroughNotification"
                  " when PassThrough is not active");
    return;
  }
  smart_objects::SmartObject* on_audio_pass = NULL;
  on_audio_pass = new smart_objects::SmartObject();

  if (NULL == on_audio_pass) {
    LOG4CXX_ERROR_EXT(logger_, "OnAudioPassThru NULL pointer");

    return;
  }

  LOG4CXX_INFO_EXT(logger_, "Fill smart object");

  (*on_audio_pass)[strings::params][strings::message_type] =
    application_manager::MessageType::kNotification;

  (*on_audio_pass)[strings::params][strings::connection_key] =
    static_cast<int32_t>(session_key);
  (*on_audio_pass)[strings::params][strings::function_id] =
    mobile_apis::FunctionID::OnAudioPassThruID;

  LOG4CXX_INFO_EXT(logger_, "Fill binary data");
  // binary data
  (*on_audio_pass)[strings::params][strings::binary_data] =
    smart_objects::SmartObject(binaryData);

  LOG4CXX_INFO_EXT(logger_, "After fill binary data");

  LOG4CXX_INFO_EXT(logger_, "Send data");
  CommandSharedPtr command =
    MobileCommandFactory::CreateCommand(&(*on_audio_pass));
  command->Init();
  command->Run();
  command->CleanUp();
}

void ApplicationManagerImpl::StopAudioPassThru(int32_t application_key) {
  LOG4CXX_TRACE_ENTER(logger_);
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
  const RawMessagePtr message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnMessageReceived");

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
  const RawMessagePtr message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnMobileMessageSent");
}

void ApplicationManagerImpl::OnMessageReceived(
  hmi_message_handler::MessageSharedPointer message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnMessageReceived");

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
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnDeviceListUpdated");

  smart_objects::SmartObject* update_list = new smart_objects::SmartObject;
  smart_objects::SmartObject& so_to_send = *update_list;
  so_to_send[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
    hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList;
  so_to_send[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
    hmi_apis::messageType::request;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 3;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = 1;
  so_to_send[jhs::S_PARAMS][jhs::S_CORRELATION_ID] = GetNextHMICorrelationID();
  smart_objects::SmartObject* msg_params = MessageHelper::CreateDeviceListSO(
        device_list);
  if (!msg_params) {
    LOG4CXX_WARN(logger_, "Failed to create sub-smart object.");
    delete update_list;
    return;
  }
  so_to_send[jhs::S_MSG_PARAMS] = *msg_params;
  ManageHMICommand(update_list);
}

void ApplicationManagerImpl::OnFindNewApplicationsRequest() {
  connection_handler_->ConnectToAllDevices();
  LOG4CXX_DEBUG(logger_, "Starting application list update timer");
  uint32_t timeout = profile::Profile::instance()->application_list_update_timeout();
  application_list_update_timer_->start(timeout);
}

void ApplicationManagerImpl::SendUpdateAppList(const std::list<uint32_t>& applications_ids) {
  LOG4CXX_TRACE(logger_, "SendUpdateAppList");

  LOG4CXX_DEBUG(logger_, applications_ids.size() << " applications.");

  smart_objects::SmartObject* request = MessageHelper::CreateModuleInfoSO(
                                          hmi_apis::FunctionID::BasicCommunication_UpdateAppList);
  (*request)[strings::msg_params][strings::applications] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObject& applications =
      (*request)[strings::msg_params][strings::applications];

  uint32_t app_count = 0;
  for (std::list<uint32_t>::const_iterator it = applications_ids.begin();
       it != applications_ids.end(); ++it) {
    ApplicationSharedPtr app = application(*it);

    if (!app.valid()) {
      LOG4CXX_ERROR(logger_, "Application not found , id = " << *it);
      continue;
    }

    smart_objects::SmartObject hmi_application(smart_objects::SmartType_Map);;
    if (!MessageHelper::CreateHMIApplicationStruct(app, hmi_application)) {
      LOG4CXX_ERROR(logger_, "Can't CreateHMIApplicationStruct ', id = " << *it);
      continue;
    }
    applications[app_count++] = hmi_application;
  }
  if (app_count <= 0) {
    LOG4CXX_WARN(logger_, "Empty applications list");
  }
  ManageHMICommand(request);
}

void ApplicationManagerImpl::RemoveDevice(
    const connection_handler::DeviceHandle& device_handle) {
  LOG4CXX_INFO(logger_, "device_handle " << device_handle);
}

bool ApplicationManagerImpl::IsAudioStreamingAllowed(uint32_t connection_key) const {
  ApplicationSharedPtr app = application(connection_key);

  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return false;
  }

  const mobile_api::HMILevel::eType& hmi_level = app->hmi_level();

  if (mobile_api::HMILevel::HMI_FULL == hmi_level ||
      mobile_api::HMILevel::HMI_LIMITED == hmi_level) {
    return true;
  }

  return false;
}

bool ApplicationManagerImpl::IsVideoStreamingAllowed(uint32_t connection_key) const {
  ApplicationSharedPtr app = application(connection_key);

  if (!app) {
    LOG4CXX_WARN(logger_, "An application is not registered.");
    return false;
  }

  const mobile_api::HMILevel::eType& hmi_level = app->hmi_level();

  if (mobile_api::HMILevel::HMI_FULL == hmi_level &&
      app->hmi_supports_navi_streaming()) {
    return true;
  }

  return false;
}

uint32_t ApplicationManagerImpl::GenerateGrammarID() {
  return rand();
}

uint32_t ApplicationManagerImpl::GenerateNewHMIAppID() {
  uint32_t hmi_app_id = rand();
  while (resume_ctrl_.IsHMIApplicationIdExist(hmi_app_id)) {
    hmi_app_id = rand();
  }

  return hmi_app_id;
}

void ApplicationManagerImpl::ReplaceMobileByHMIAppId(
    smart_objects::SmartObject& message) {
  MessageHelper::PrintSmartObject(message);
  flush(std::cout);
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

bool ApplicationManagerImpl::OnServiceStartedCallback(
  const connection_handler::DeviceHandle& device_handle,
  const int32_t& session_key,
  const protocol_handler::ServiceType& type) {
  LOG4CXX_INFO(logger_,
               "OnServiceStartedCallback " << type << " in session " << session_key);
  ApplicationSharedPtr app = application(session_key);

  switch (type) {
    case protocol_handler::kRpc: {
      LOG4CXX_INFO(logger_, "RPC service is about to be started.");
      break;
    }
    case protocol_handler::kMobileNav: {
      LOG4CXX_INFO(logger_, "Video service is about to be started.");
      if (media_manager_) {
        if (!app) {
          LOG4CXX_ERROR_EXT(logger_, "An application is not registered.");
          return false;
        }
        if (app->allowed_support_navigation()) {
          media_manager_->StartVideoStreaming(session_key);
        } else {
          return false;
        }
      }
      break;
    }
    case protocol_handler::kAudio: {
      LOG4CXX_INFO(logger_, "Audio service is about to be started.");
      if (media_manager_) {
        if (!app) {
          LOG4CXX_ERROR_EXT(logger_, "An application is not registered.");
          return false;
        }
        if (app->allowed_support_navigation()) {
          media_manager_->StartAudioStreaming(session_key);
        } else {
          return false;
        }
      }
      break;
    }
    default: {
      LOG4CXX_WARN(logger_, "Unknown type of service to be started.");
      break;
    }
  }

  return true;
}

void ApplicationManagerImpl::OnServiceEndedCallback(const int32_t& session_key,
    const protocol_handler::ServiceType& type) {
  LOG4CXX_INFO_EXT(
    logger_,
    "OnServiceEndedCallback " << type  << " in session " << session_key);

  switch (type) {
    case protocol_handler::kRpc: {
      LOG4CXX_INFO(logger_, "Remove application.");
      /* in case it was unexpected disconnect application will be removed
       and we will notify HMI that it was unexpected disconnect,
       but in case it was closed by mobile we will be unable to find it in the list
      */
      UnregisterApplication(session_key, mobile_apis::Result::INVALID_ENUM,
                            true, true);
      break;
    }
    case protocol_handler::kMobileNav: {
      LOG4CXX_INFO(logger_, "Stop video streaming.");
      if (media_manager_) {
        media_manager_->StopVideoStreaming(session_key);
      }
      break;
    }
    case protocol_handler::kAudio: {
      LOG4CXX_INFO(logger_, "Stop audio service.");
      if (media_manager_) {
        media_manager_->StopAudioStreaming(session_key);
      }
      break;
    }
    default:
      LOG4CXX_WARN(logger_, "Unknown type of service to be ended.");
      break;
  }
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
  const utils::SharedPtr<smart_objects::SmartObject>& message,
  bool final_message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::SendMessageToMobile");

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
        ProtocolVersion::kV3;
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
    request_ctrl_.terminateRequest(
      msg_to_mobile[strings::params][strings::correlation_id].asInt());
  } else if (app) {
    mobile_apis::FunctionID::eType function_id =
        static_cast<mobile_apis::FunctionID::eType>(
        (*message)[strings::params][strings::function_id].asUInt());
    mobile_apis::Result::eType check_result = CheckPolicyPermissions(
                                                app->mobile_app_id()->asString(),
                                                app->hmi_level(),
                                                function_id);
    if (mobile_apis::Result::SUCCESS != check_result) {
      return;
    }
  }

  messages_to_mobile_.PostMessage(impl::MessageToMobile(message_to_send,
                                  final_message));
}

bool ApplicationManagerImpl::ManageMobileCommand(
  const utils::SharedPtr<smart_objects::SmartObject>& message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::ManageMobileCommand");

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    NOTREACHED()
    return false;
  }

#ifdef DEBUG
  MessageHelper::PrintSmartObject(*message);
#endif

  LOG4CXX_INFO(logger_, "Trying to create message in mobile factory.");
  CommandSharedPtr command = MobileCommandFactory::CreateCommand(message);

  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create mobile command from smart object");
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

  if (((mobile_apis::FunctionID::RegisterAppInterfaceID != function_id) &&
       (protocol_type == commands::CommandImpl::mobile_protocol_type_)) &&
      (mobile_apis::FunctionID::UnregisterAppInterfaceID != function_id)) {
    app = ApplicationManagerImpl::instance()->application(connection_key);
    if (!app) {
      LOG4CXX_ERROR_EXT(logger_, "APPLICATION_NOT_REGISTERED");
      smart_objects::SmartObject* response =
        MessageHelper::CreateNegativeResponse(
          connection_key,
          static_cast<int32_t>(function_id),
          correlation_id,
          static_cast<int32_t>(mobile_apis::Result::APPLICATION_NOT_REGISTERED));

      SendMessageToMobile(response);
      return false;
    }

    // Message for "CheckPermission" must be with attached schema
    mobile_so_factory().attachSchema(*message);
  }

  if ((*message)[strings::params][strings::message_type].asInt() ==
      mobile_apis::messageType::request) {
    // get application hmi level
    mobile_api::HMILevel::eType app_hmi_level =
      mobile_api::HMILevel::INVALID_ENUM;
    if (app) {
      app_hmi_level = app->hmi_level();
    }

    // commands will be launched from requesr_ctrl
    request_controller::RequestController::TResult result =
      request_ctrl_.addRequest(command, app_hmi_level);

    if (result == request_controller::RequestController::SUCCESS) {
      LOG4CXX_INFO(logger_, "Perform request");
    } else if (result ==
               request_controller::RequestController::
               TOO_MANY_PENDING_REQUESTS) {
      LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: " <<
                        "TOO_MANY_PENDING_REQUESTS");

      smart_objects::SmartObject* response =
        MessageHelper::CreateNegativeResponse(
          connection_key,
          static_cast<int32_t>(function_id),
          correlation_id,
          static_cast<int32_t>(mobile_apis::Result::TOO_MANY_PENDING_REQUESTS));

      SendMessageToMobile(response);
      return false;
    } else if (result ==
               request_controller::RequestController::TOO_MANY_REQUESTS) {
      LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: " <<
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
      LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: " <<
                        "REQUEST_WHILE_IN_NONE_HMI_LEVEL");

      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
        connection_key, mobile_api::AppInterfaceUnregisteredReason::
        REQUEST_WHILE_IN_NONE_HMI_LEVEL);

      application(connection_key)->usage_report().RecordRemovalsForBadBehavior();
      UnregisterApplication(connection_key, mobile_apis::Result::INVALID_ENUM,
                            false);
      return false;
    } else {
      LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: Unknown case");
      return false;
    }
  } else {
    // run all other types of command
    if (command->Init()) {
      command->Run();
      command->CleanUp();
    }
  }

  return true;
}

void ApplicationManagerImpl::SendMessageToHMI(
  const utils::SharedPtr<smart_objects::SmartObject>& message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::SendMessageToHMI");

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
  const utils::SharedPtr<smart_objects::SmartObject>& message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::ManageHMICommand");

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    NOTREACHED();
    return false;
  }

#ifdef DEBUG
  MessageHelper::PrintSmartObject(*message);
#endif

  CommandSharedPtr command = HMICommandFactory::CreateCommand(message);

  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create command from smart object");
    return false;
  }

  if (command->Init()) {
    command->Run();
    if (command->CleanUp()) {
      return true;
    }
  }
  return false;
}

bool ApplicationManagerImpl::Init() {
  LOG4CXX_TRACE(logger_, "Init application manager");
  bool init_result = true;
  do {
    if (policy_manager_) {
      LOG4CXX_INFO(logger_, "Policy library is loaded, now initing PT");
      if (!policy::PolicyHandler::instance()->InitPolicyTable()) {
        init_result = false;
        break;
      }
    }
    const std::string app_storage_folder = 
      profile::Profile::instance()->app_storage_folder();
    if (!file_system::DirectoryExists(app_storage_folder)) {
      LOG4CXX_WARN(logger_, "Storage directory doesn't exist");
      // if storage directory doesn't exist try to create it
      if (!file_system::CreateDirectoryRecursively(app_storage_folder)) {
        LOG4CXX_ERROR(logger_, "Unable to create Storage directory "
                                << app_storage_folder);
        init_result = false;
        break;
      }
    }
    if (!(file_system::IsWritingAllowed(app_storage_folder) &&
          file_system::IsReadingAllowed(app_storage_folder))) {
      LOG4CXX_ERROR(logger_,
                   "Storage directory doesn't have read/write permissions");
      init_result = false;
      break;
    }

    const std::string system_files_path = 
      profile::Profile::instance()->system_files_path();
    if (!file_system::DirectoryExists(system_files_path)) {
      LOG4CXX_WARN(logger_, "System files directory doesn't exist");
      // if system directory doesn't exist try to create it
      if (!file_system::CreateDirectoryRecursively(system_files_path)) {
        LOG4CXX_ERROR(logger_, "Unable to create System directory "
                                << system_files_path);
        init_result = false;
        break;
      }
    }
    if (!(file_system::IsWritingAllowed(system_files_path) &&
          file_system::IsReadingAllowed(system_files_path))) {
      LOG4CXX_ERROR(logger_,
                   "System directory doesn't have read/write permissions");
      init_result = false;
      break;
    }
    media_manager_ = media_manager::MediaManagerImpl::instance();
  } while (false);

  return init_result;
}

bool ApplicationManagerImpl::ConvertMessageToSO(
  const Message& message, smart_objects::SmartObject& output) {
  LOG4CXX_INFO(
    logger_,
    "\t\t\tMessage to convert: protocol " << message.protocol_version()
    << "; json " << message.json_message());

  switch (message.protocol_version()) {
    case ProtocolVersion::kV3:
    case ProtocolVersion::kV2: {
      if (!formatters::CFormatterJsonSDLRPCv2::fromString(
            message.json_message(),
            output,
            message.function_id(),
            message.type(),
            message.correlation_id())
          || !mobile_so_factory().attachSchema(output)
          || ((output.validate() != smart_objects::Errors::OK)
              && (output.validate() !=
                  smart_objects::Errors::UNEXPECTED_PARAMETER))) {
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
      if (output.validate() != smart_objects::Errors::OK &&
          output.validate() != smart_objects::Errors::UNEXPECTED_PARAMETER) {
        LOG4CXX_WARN(
          logger_,
          "Incorrect parameter from HMI");
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

          smart_objects::SmartObject* msg_to_send = new smart_objects::SmartObject(output);
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
  const RawMessagePtr message) {
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

  Message* convertion_result = NULL;
  if (message->protocol_version() == 1) {
    convertion_result =
      MobileMessageHandler::HandleIncomingMessageProtocolV1(message);
  } else if ((message->protocol_version() == 2) ||
             (message->protocol_version() == 3)) {
    convertion_result =
      MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
  } else {
    LOG4CXX_WARN(logger_, "Unknown protocol version.");
    return outgoing_message;
  }

  if (convertion_result) {
    outgoing_message = convertion_result;
  } else {
    LOG4CXX_ERROR(logger_, "Received invalid message");
  }
  return outgoing_message;
}

void ApplicationManagerImpl::ProcessMessageFromMobile(
  const utils::SharedPtr<Message>& message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::ProcessMessageFromMobile()");
#ifdef TIME_TESTER
  AMMetricObserver::MessageMetricSharedPtr metric(new AMMetricObserver::MessageMetric());
  metric->begin = date_time::DateTime::getCurrentTime();
#endif  // TIME_TESTER
  utils::SharedPtr<smart_objects::SmartObject> so_from_mobile(
    new smart_objects::SmartObject);

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

  if (!ManageMobileCommand(so_from_mobile)) {
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
  const utils::SharedPtr<Message>& message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::ProcessMessageFromHMI()");
  utils::SharedPtr<smart_objects::SmartObject> smart_object(
    new smart_objects::SmartObject);

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

#ifdef TIME_TESTER
void ApplicationManagerImpl::SetTimeMetricObserver(AMMetricObserver* observer) {
  metric_observer_ = observer;
}
#endif  // TIME_TESTER

void ApplicationManagerImpl::addNotification(const CommandSharedPtr& ptr) {
  notification_list_.push_back(ptr);
}

void ApplicationManagerImpl::removeNotification(const CommandSharedPtr& ptr) {
  std::list<CommandSharedPtr>::iterator it = notification_list_.begin();
  for (; notification_list_.end() != it; ++it) {
    if (*it == ptr) {
      notification_list_.erase(it);
      break;
    }
  }
}

void ApplicationManagerImpl::updateRequestTimeout(uint32_t connection_key,
    uint32_t mobile_correlation_id,
    uint32_t new_timeout_value) {
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

void ApplicationManagerImpl::SetUnregisterAllApplicationsReason(
  mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  unregister_reason_ = reason;
}

void ApplicationManagerImpl::HeadUnitReset(
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  switch (reason) {
    case mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET: {
      policy::PolicyHandler::instance()->ResetPolicyTable();
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
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::SendOnSDLClose");

  // must be sent to PASA HMI on shutdown synchronously
  smart_objects::SmartObject* msg = new smart_objects::SmartObject(
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

  delete msg;
  hmi_handler_->SendMessageToHMI(message_to_send);
}

void ApplicationManagerImpl::UnregisterAllApplications(bool generated_by_hmi) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::UnregisterAllApplications " <<
               unregister_reason_);

  hmi_cooperating_ = false;

  bool is_ignition_off =
      unregister_reason_ ==
      mobile_api::AppInterfaceUnregisteredReason::IGNITION_OFF ? true : false;

  bool is_unexpected_disconnect = (generated_by_hmi != true);
  ApplicationListAccessor accessor;
  std::set<ApplicationSharedPtr>::iterator it = accessor.applications().begin();
  while (it != accessor.applications().end()) {
    ApplicationSharedPtr app_to_remove = *it;
    MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
        app_to_remove->app_id(), unregister_reason_);
    UnregisterApplication(app_to_remove->app_id(),
                          mobile_apis::Result::INVALID_ENUM, is_ignition_off,
                          is_unexpected_disconnect);

    connection_handler_->CloseSession(app_to_remove->app_id());
    it = application_list_.begin();
  }

  if (is_ignition_off) {
   resume_controller().IgnitionOff();
  }
}

void ApplicationManagerImpl::UnregisterApplication(
  const uint32_t& app_id, mobile_apis::Result::eType reason,
  bool is_resuming, bool is_unexpected_disconnect) {
  LOG4CXX_INFO(logger_,
               "ApplicationManagerImpl::UnregisterApplication " << app_id);

  switch (reason) {
    case mobile_apis::Result::SUCCESS:break;
    case mobile_apis::Result::DISALLOWED: break;
    case mobile_apis::Result::USER_DISALLOWED:break;
    case mobile_apis::Result::INVALID_CERT: break;
    case mobile_apis::Result::EXPIRED_CERT: break;
    case mobile_apis::Result::TOO_MANY_PENDING_REQUESTS: {
      application(app_id)->usage_report().RecordRemovalsForBadBehavior();
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown unregister reason");
      break;
    }
  }

  ApplicationSharedPtr app_to_remove;
  {
    sync_primitives::AutoLock lock(applications_list_lock_);

    std::set<ApplicationSharedPtr>::const_iterator it = application_list_.begin();
    for (; it != application_list_.end(); ++it) {
      if ((*it)->app_id() == app_id) {
        app_to_remove = *it;
      }
    }
    application_list_.erase(app_to_remove);
  }

  if (!app_to_remove) {
    LOG4CXX_INFO(logger_, "Application is already unregistered.");
    return;
  }

  if (is_resuming) {
    resume_ctrl_.SaveApplication(app_to_remove);
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


void ApplicationManagerImpl::UnregisterRevokedApplication(
    const uint32_t& app_id, mobile_apis::Result::eType reason) {
  UnregisterApplication(app_id, reason);

  connection_handler_->CloseSession(app_id);

  if (application_list_.empty()) {
    connection_handler_->CloseRevokedConnection(app_id);
  }
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
  protocol_handler::RawMessage* rawMessage = 0;
  if (message->protocol_version() == application_manager::kV1) {
    rawMessage = MobileMessageHandler::HandleOutgoingMessageProtocolV1(message);
  } else if ((message->protocol_version() == application_manager::kV2) ||
             (message->protocol_version() == application_manager::kV3)) {
    rawMessage = MobileMessageHandler::HandleOutgoingMessageProtocolV2(message);
  } else {
    return;
  }
  if (!rawMessage) {
    LOG4CXX_ERROR(logger_, "Failed to create raw message.");
    return;
  }

  if (!protocol_handler_) {
    LOG4CXX_WARN(logger_,
                 "Protocol Handler is not set; cannot send message to mobile.");
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
    connection_handler_->CloseSession(message->connection_key());
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
  LOG4CXX_INFO(logger_, "Message from hmi given away.");
}

mobile_apis::Result::eType ApplicationManagerImpl::CheckPolicyPermissions(
    const std::string& policy_app_id,
    mobile_apis::HMILevel::eType hmi_level,
    mobile_apis::FunctionID::eType function_id,
    CommandParametersPermissions* params_permissions) {
  LOG4CXX_INFO(logger_, "CheckPolicyPermissions");
  // TODO(AOleynik): Remove check of policy_turn_off, when this flag will be
  // unused in config file
  if (profile::Profile::instance()->policy_turn_off()) {
    return mobile_apis::Result::SUCCESS;
  }
  mobile_apis::Result::eType check_result = mobile_apis::Result::DISALLOWED;
  if (!policy_manager_) {
    LOG4CXX_WARN(logger_, "Policy library is not loaded.");
    return check_result;
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
  policy::CheckPermissionResult result = policy_manager_->CheckPermissions(
      policy_app_id,
      stringified_hmi_level,
      stringified_functionID);

  if (NULL != params_permissions) {
    if (result.list_of_allowed_params.valid()) {
      params_permissions->allowed_params =
            *(result.list_of_allowed_params.get());
    }
    if (result.list_of_disallowed_params.valid()) {
      params_permissions->disallowed_params =
          *(result.list_of_disallowed_params.get());
    }
    if (result.list_of_undefined_params.valid()) {
      params_permissions->undefined_params =
          *(result.list_of_undefined_params.get());
    }
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
        return check_result = mobile_apis::Result::DISALLOWED;
      case policy::kRpcUserDisallowed:
        return check_result = mobile_apis::Result::USER_DISALLOWED;
      default:
        return check_result = mobile_apis::Result::INVALID_ENUM;
    }
  }
  LOG4CXX_INFO(logger_, "Request is allowed by policies. "+log_msg);
  return mobile_api::Result::SUCCESS;
}

void ApplicationManagerImpl::Mute(VRTTSSessionChanging changing_state) {
  mobile_apis::AudioStreamingState::eType state =
    hmi_capabilities_.attenuated_supported()
    ? mobile_apis::AudioStreamingState::ATTENUATED
    : mobile_apis::AudioStreamingState::NOT_AUDIBLE;
  std::set<ApplicationSharedPtr> local_app_list = application_list_;

  std::set<ApplicationSharedPtr>::const_iterator it = local_app_list.begin();
  std::set<ApplicationSharedPtr>::const_iterator itEnd = local_app_list.end();
  for (; it != itEnd; ++it) {
    if ((*it).valid()) {
      if ((*it)->is_media_application()) {
        if (kTTSSessionChanging == changing_state) {
          (*it)->set_tts_speak_state(true);
        }
        if ((*it)->audio_streaming_state() != state) {
          (*it)->set_audio_streaming_state(state);
          MessageHelper::SendHMIStatusNotification(*(*it));
        }
      }
    }
  }
}

void ApplicationManagerImpl::Unmute(VRTTSSessionChanging changing_state) {

  std::set<ApplicationSharedPtr> local_app_list = application_list_;
  std::set<ApplicationSharedPtr>::const_iterator it = local_app_list.begin();
  std::set<ApplicationSharedPtr>::const_iterator itEnd = local_app_list.end();
  //according with SDLAQ-CRS-839
  bool is_application_audible = false;

  for (; it != itEnd; ++it) {
    if ((*it).valid()) {
      if ((*it)->is_media_application()) {
        if (kTTSSessionChanging == changing_state) {
          (*it)->set_tts_speak_state(false);
        }
        if ((!is_application_audible) && (!(vr_session_started())) &&
            ((*it)->audio_streaming_state() !=
             mobile_apis::AudioStreamingState::AUDIBLE) &&
            (mobile_api::HMILevel::HMI_NONE != (*it)->hmi_level())) {
          //according with SDLAQ-CRS-839
          is_application_audible = true;
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

  std::list <uint32_t> applications_ids;

  applications_list_lock_.Acquire();
  for (std::set<ApplicationSharedPtr>::const_iterator i = application_list_.begin();
       i != application_list_.end(); ++i) {
    ApplicationSharedPtr application = *i;
    uint32_t app_id = application->app_id();
    applications_ids.push_back(app_id);
  }
  applications_list_lock_.Release();

  SendUpdateAppList(applications_ids);
}

}  // namespace application_manager
