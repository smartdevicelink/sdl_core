/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/application_manager_impl.h"

#include <climits>
#include <string>
#include <fstream>

#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_command_factory.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_notification_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/mobile_message_handler.h"
#include "connection_handler/connection_handler_impl.h"
#include "formatters/formatter_json_rpc.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/CFormatterJsonSDLRPCv1.hpp"
#include "config_profile/profile.h"
#include "utils/threads/thread.h"
#include "utils/file_system.h"
#include "policies/policy_manager.h"
#include "application_manager/application_impl.h"

namespace application_manager {

log4cxx::LoggerPtr ApplicationManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("ApplicationManager"));

uint32_t ApplicationManagerImpl::corelation_id_ = 0;
const uint32_t ApplicationManagerImpl::max_corelation_id_ = UINT_MAX;

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

ApplicationManagerImpl::ApplicationManagerImpl()
  : audio_pass_thru_active_(false),
    is_distracting_driver_(false),
    is_vr_session_strated_(false),
    hmi_cooperating_(false),
    is_all_apps_allowed_(true),
    hmi_handler_(NULL),
    connection_handler_(NULL),
    policy_manager_(NULL),
    hmi_so_factory_(NULL),
    mobile_so_factory_(NULL),
    protocol_handler_(NULL),
    messages_from_mobile_("application_manager::FromMobileThreadImpl", this),
    messages_to_mobile_("application_manager::ToMobileThreadImpl", this),
    messages_from_hmi_("application_manager::FromHMHThreadImpl", this),
    messages_to_hmi_("application_manager::ToHMHThreadImpl", this),
    request_ctrl_(),
    hmi_capabilities_(this),
    unregister_reason_(mobile_api::AppInterfaceUnregisteredReason::MASTER_RESET),
    media_manager_(NULL)
{
  LOG4CXX_INFO(logger_, "Creating ApplicationManager");
  if (!policies_manager_.Init()) {
    LOG4CXX_ERROR(logger_, "Policies manager initialization failed.");
    return;
  }

  media_manager_ = media_manager::MediaManagerImpl::instance();
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

  media_manager_ = NULL;
  hmi_handler_ = NULL;
  connection_handler_ = NULL;
  policy_manager_ = NULL;
  hmi_so_factory_ = NULL;
  mobile_so_factory_ = NULL;
  protocol_handler_ = NULL;
  media_manager_ = NULL;
}

bool ApplicationManagerImpl::Stop() {
  LOG4CXX_INFO(logger_, "Stop ApplicationManager.");
  try {
    UnregisterAllApplications();
  }
  catch(...) {
    LOG4CXX_ERROR(logger_,
                  "An error occured during unregistering applications.");
  }

  return true;
}

Application* ApplicationManagerImpl::application(int32_t app_id) const {
  std::map<int32_t, Application*>::const_iterator it =
      applications_.find(app_id);
  if (applications_.end() != it) {
    return it->second;
  } else {
    return NULL;
  }
}

Application* ApplicationManagerImpl::active_application() const {
  // TODO(DK) : check driver distraction
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->IsFullscreen()) {
      return *it;
    }
  }
  return NULL;
}

std::vector<Application*> ApplicationManagerImpl::applications_by_button(
  uint32_t button) {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it; ++it) {
    if ((*it)->IsSubscribedToButton(
          static_cast<mobile_apis::ButtonName::eType>(button))) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<Application*> ApplicationManagerImpl::applications_by_ivi(
  uint32_t vehicle_info) {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->IsSubscribedToIVI(vehicle_info)) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<Application*> ApplicationManagerImpl::applications_with_navi() {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->allowed_support_navigation()) {
      result.push_back(*it);
    }
  }
  return result;
}

Application* ApplicationManagerImpl::RegisterApplication(
  const utils::SharedPtr<smart_objects::SmartObject>&
  request_for_registration) {
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
    return NULL;
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
      return NULL;
    }
  }

  Application* application = new ApplicationImpl(app_id);
  if (!application) {
    utils::SharedPtr<smart_objects::SmartObject> response(
      MessageHelper::CreateNegativeResponse(
        connection_key, mobile_apis::FunctionID::RegisterAppInterfaceID,
        message[strings::params][strings::correlation_id].asUInt(),
        mobile_apis::Result::OUT_OF_MEMORY));
    ManageMobileCommand(response);
    return NULL;
  }

  const std::string& name =
    message[strings::msg_params][strings::app_name].asString();

  application->set_name(name);
  application->set_device(device_id);

  application->set_language(
    static_cast<mobile_api::Language::eType>(
      message[strings::msg_params][strings::language_desired].asInt()));

  application->set_ui_language(
    static_cast<mobile_api::Language::eType>(
      message[strings::msg_params][strings::hmi_display_language_desired]
      .asInt()));

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

  application->set_mobile_app_id(message[strings::msg_params][strings::app_id]);

  sync_primitives::AutoLock lock(applications_list_lock_);

  applications_.insert(std::pair<int32_t, Application*>(app_id, application));
  application_list_.insert(application);

  // TODO(PV): add asking user to allow application
  // BasicCommunication_AllowApp
  // application->set_app_allowed(result);
  return application;
}

bool ApplicationManagerImpl::RemoveAppDataFromHMI(Application* app) {
  return true;
}

bool ApplicationManagerImpl::LoadAppDataToHMI(Application* app) {
  return true;
}

bool ApplicationManagerImpl::ActivateApplication(Application* app) {
  if (!app) {
    LOG4CXX_ERROR(logger_, "Null-pointer application received.");
    NOTREACHED();
    return false;
  }

  bool is_new_app_media = app->is_media_application();

  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    Application* curr_app = *it;
    if (curr_app->app_id() == curr_app->app_id()) {
      if (curr_app->IsFullscreen()) {
        LOG4CXX_WARN(logger_, "Application is already active.");
        return false;
      }
      if (mobile_api::HMILevel::eType::HMI_LIMITED !=
          curr_app->hmi_level()) {
        if (curr_app->has_been_activated()) {
          MessageHelper::SendAppDataToHMI(curr_app);
        }
      }
      if (!curr_app->MakeFullscreen()) {
        return false;
      }
      MessageHelper::SendHMIStatusNotification(*curr_app);
    } else {
      if (is_new_app_media) {
        if (curr_app->IsAudible()) {
          curr_app->MakeNotAudible();
          MessageHelper::SendHMIStatusNotification(*curr_app);
        }
      }
      if (curr_app->IsFullscreen()) {
        MessageHelper::RemoveAppDataFromHMI(curr_app);
      }
    }
  }
  return true;
}

void ApplicationManagerImpl::DeactivateApplication(Application* app) {
  MessageHelper::SendDeleteCommandRequestToHMI(app);
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

  if (true == profile::Profile::instance()->launch_hmi()) {
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
  }

  if (!connection_handler_) {
    LOG4CXX_WARN(logger_, "Connection handler is not set.");
  } else {
    connection_handler_->StartTransportManager();
  }
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
      std::string("record.wav"),
      max_duration);
  }
}

void ApplicationManagerImpl::SendAudioPassThroughNotification(
  uint32_t session_key,
  std::vector<uint8_t> binaryData) {
  LOG4CXX_TRACE_ENTER(logger_);

  {
    sync_primitives::AutoLock lock(audio_pass_thru_lock_);
    if (!audio_pass_thru_active_) {
      LOG4CXX_ERROR(logger_, "Trying to send PassThroughNotification"
                    " when PassThrough is not active");
      return;
    }
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
  if (NULL != media_manager_) {
    media_manager_->StopMicrophoneRecording(application_key);
  }
}

std::string ApplicationManagerImpl::GetDeviceName(
  connection_handler::DeviceHandle handle) {
  DCHECK(connection_handler_ != 0);

  std::string device_name = "";
  std::list<uint32_t> applications_list;
  connection_handler::ConnectionHandlerImpl* con_handler_impl =
    static_cast<connection_handler::ConnectionHandlerImpl*>(
        connection_handler_);
  if (con_handler_impl->GetDataOnDeviceID(handle, &device_name,
                                          &applications_list) == -1) {
    LOG4CXX_ERROR(logger_, "Failed to extract device name for id " << handle);
  } else {
    LOG4CXX_INFO(logger_, "\t\t\t\t\tDevice name is " << device_name);
  }

  return device_name;
}

void ApplicationManagerImpl::OnMessageReceived(
  const protocol_handler::RawMessagePtr& message) {
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
  } else {
    LOG4CXX_WARN(logger_, "Incorrect message received");
  }
}

void ApplicationManagerImpl::OnMobileMessageSent(
  const protocol_handler::RawMessagePtr& message) {
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
  const connection_handler::DeviceList& device_list) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnDeviceListUpdated");

  smart_objects::SmartObject* update_list = new smart_objects::SmartObject;
  smart_objects::SmartObject& so_to_send = *update_list;
  so_to_send[jhs::S_PARAMS][jhs::S_FUNCTION_ID] =
    hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList;
  so_to_send[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
    hmi_apis::messageType::request;
  so_to_send[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = 2;
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

void ApplicationManagerImpl::RemoveDevice(
  const connection_handler::DeviceHandle& device_handle) {
}


bool ApplicationManagerImpl::IsStreamingAllowed(uint32_t connection_key) const {
  Application* app = application(connection_key);

  if (!app) {
    LOG4CXX_INFO(logger_, "An application is not registered.");
    return false;
  }

  const mobile_api::HMILevel::eType& hmi_level = app->hmi_level();

  if (mobile_api::HMILevel::HMI_FULL == hmi_level ||
      mobile_api::HMILevel::HMI_LIMITED == hmi_level) {
    return true;
  }

  return false;
}

bool ApplicationManagerImpl::OnServiceStartedCallback(
    const connection_handler::DeviceHandle& device_handle,
    const int32_t& session_key,
    const protocol_handler::ServiceType& type) {
  LOG4CXX_INFO(logger_,
      "OnServiceStartedCallback " << type << " in session " << session_key);

  Application* app = application(session_key);

  switch (type) {
    case protocol_handler::kRpc: {
      LOG4CXX_INFO(logger_, "RPC service is about to be started.");
      break;
    }
    case protocol_handler::kMovileNav: {
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

bool ApplicationManagerImpl::OnServiceResumedCallback(
    const connection_handler::DeviceHandle& device_handle,
    const int32_t& old_session_key, const int32_t& new_session_key,
    const protocol_handler::ServiceType& type) {
  LOG4CXX_INFO_EXT(logger_,
                   "OnServiceResumedCallback previous session key= "
                   << old_session_key << " new session key= "
                   << new_session_key);

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
      UnregisterApplication(session_key, true);
      break;
    }
    case protocol_handler::kMovileNav: {
      LOG4CXX_INFO(logger_, "Stop video streaming.");
      if (media_manager_) {
        media_manager_->StopVideoStreaming(session_key);
      }
      break;
    }
    case protocol_handler::kAudio:{
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

void ApplicationManagerImpl::set_policy_manager(
  policies::PolicyManager* policy_manager) {
  policy_manager_ = policy_manager;
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
  if (msg_to_mobile[strings::params].keyExists(strings::correlation_id)) {
    request_ctrl_.terminateRequest(
      msg_to_mobile[strings::params][strings::correlation_id].asUInt());
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

  uint32_t protocol_type =
    (*message)[strings::params][strings::protocol_type].asUInt();

  Application* app = NULL;

  if (((mobile_apis::FunctionID::RegisterAppInterfaceID != function_id) &&
       (protocol_type == commands::CommandImpl::mobile_protocol_type_)) &&
      (mobile_apis::FunctionID::UnregisterAppInterfaceID != function_id)) {
    app = ApplicationManagerImpl::instance()->application(connection_key);
    if (NULL == app) {
      LOG4CXX_ERROR_EXT(logger_, "APPLICATION_NOT_REGISTERED");
      smart_objects::SmartObject* response =
        MessageHelper::CreateNegativeResponse(
            connection_key,
            static_cast<int32_t>(function_id),
            correlation_id,
            static_cast<int32_t>(mobile_apis::Result::APPLICATION_NOT_REGISTERED));

      ApplicationManagerImpl::instance()->SendMessageToMobile(response);
      return false;
    }

    // Message for "CheckPermission" must be with attached schema
    mobile_so_factory().attachSchema(*message);

    policies::CheckPermissionResult result =
      policy_manager_->CheckPermission(app->app_id(),
                                       *message,
                                       app->hmi_level());

    if (policies::PermissionResult::PERMISSION_ALLOWED != result.result) {
      LOG4CXX_WARN(
        logger_,
        "Request blocked by policies. " << "FunctionID: "
        << static_cast<int32_t>(function_id) << " Application HMI status: "
        << static_cast<int32_t>(app->hmi_level()));

      smart_objects::SmartObject* response =
        MessageHelper::CreateBlockedByPoliciesResponse(function_id,
            mobile_apis::Result::REJECTED, correlation_id, connection_key);

      ApplicationManagerImpl::instance()->SendMessageToMobile(response);
      return true;
    }
  }

  if (command->Init()) {
    if ((*message)[strings::params][strings::message_type].asInt() ==
        mobile_apis::messageType::request) {
      // get application hmi level
      mobile_api::HMILevel::eType app_hmi_level =
        mobile_api::HMILevel::INVALID_ENUM;
      if (NULL != app) {
        app_hmi_level = app->hmi_level();
      }

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

        ApplicationManagerImpl::instance()->SendMessageToMobile(response);
        return false;
      } else if (result ==
                 request_controller::RequestController::TOO_MANY_REQUESTS) {
        LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: " <<
                          "TOO_MANY_REQUESTS");

        MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
          connection_key,
          mobile_api::AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS);

        UnregisterApplication(connection_key, true);
        return false;
      } else if (result ==
                 request_controller::RequestController::
                 NONE_HMI_LEVEL_MANY_REQUESTS) {
        LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: " <<
                          "REQUEST_WHILE_IN_NONE_HMI_LEVEL");

        MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
          connection_key, mobile_api::AppInterfaceUnregisteredReason::
          REQUEST_WHILE_IN_NONE_HMI_LEVEL);

        UnregisterApplication(connection_key, true);
        return false;
      } else {
        LOG4CXX_ERROR_EXT(logger_, "Unable to perform request: Unknown case");
        return false;
      }
    }

    command->Run();
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

#ifdef WEB_HMI
  if (!ConvertSOtoMessage(*message, *message_to_send)) {
    LOG4CXX_WARN(logger_,
                 "Cannot send message to HMI: failed to create string");
    return;
  }
#endif  // WEB_HMI

#ifdef QT_HMI
  message_to_send->set_smart_object(*message);
#endif  // QT_HMI

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

void ApplicationManagerImpl::CreateHMIMatrix(HMIMatrix* matrix) {
}

void ApplicationManagerImpl::CreatePoliciesManager(PoliciesManager* managaer) {
}

bool ApplicationManagerImpl::CheckPolicies(smart_objects::SmartObject* message,
    Application* app) {
  return true;
}

bool ApplicationManagerImpl::CheckHMIMatrix(
  smart_objects::SmartObject* message) {
  return true;
}

bool ApplicationManagerImpl::ConvertMessageToSO(
  const Message& message, smart_objects::SmartObject& output) {
  LOG4CXX_INFO(
    logger_,
    "\t\t\tMessage to convert: protocol " << message.protocol_version()
    << "; json " << message.json_message());

  switch (message.protocol_version()) {
    case ProtocolVersion::kV2: {
      if (message.function_id() == 0 || message.type() == kUnknownType) {
        LOG4CXX_ERROR(logger_, "Message received: UNSUPPORTED_VERSION");
        int32_t conversation_result =
          formatters::CFormatterJsonSDLRPCv1::fromString <
          NsSmartDeviceLinkRPC::V1::FunctionID::eType,
          NsSmartDeviceLinkRPC::V1::messageType::eType > (
            message.json_message(), output);
        if (formatters::CFormatterJsonSDLRPCv1::kSuccess
            == conversation_result) {
          utils::SharedPtr<smart_objects::SmartObject> response(
            MessageHelper::CreateNegativeResponse(
              message.connection_key(), message.function_id(),
              message.correlation_id(),
              mobile_apis::Result::UNSUPPORTED_VERSION));
          ManageMobileCommand(response);
          return false;
        }
      }

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
        LOG4CXX_WARN(logger_, "Failed to parse string to smart object");
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
      if (message.binary_data()) {
        output[strings::params][strings::binary_data] =
          *(message.binary_data());
      }
      break;
    }
    case ProtocolVersion::kHMI: {
      int32_t result = formatters::FormatterJsonRpc::FromString <
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
    default:
      // TODO(PV):
      //  removed NOTREACHED() because some app can still have vesion 1.
      LOG4CXX_WARN(
        logger_,
        "Application used unsupported protocol " << message.protocol_version()
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
      if (!formatters::CFormatterJsonSDLRPCv2::toString(message,
          output_string)) {
        LOG4CXX_WARN(logger_, "Failed to serialize smart object");
        return false;
      }
      output.set_protocol_version(application_manager::kV2);
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
  const protocol_handler::RawMessagePtr& message) {
  DCHECK(message);
  utils::SharedPtr<Message> outgoing_message;

  if (message->service_type() != protocol_handler::kRpc
      &&
      message->service_type() != protocol_handler::kBulk) {
    // skip this message, not under handling of ApplicationManager
    LOG4CXX_INFO(logger_, "Skipping message; not the under AM handling.");
    return outgoing_message;
  }

  Message* convertion_result = NULL;
  if (message->protocol_version() == 1) {
    convertion_result =
      MobileMessageHandler::HandleIncomingMessageProtocolV1(message);
  } else if (message->protocol_version() == 2) {
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

  if (!ManageMobileCommand(so_from_mobile)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
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

#ifdef WEB_HMI
  if (!ConvertMessageToSO(*message, *smart_object)) {
    LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
    return;
  }
#endif  // WEB_HMI

#ifdef QT_HMI
  *smart_object = message->smart_object();
#endif  // QT_HMI

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

void ApplicationManagerImpl::UnregisterAllApplications() {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::UnregisterAllApplications "  <<
               unregister_reason_);

  hmi_cooperating_ = false;

  std::set<Application*>::iterator it = application_list_.begin();
  while (it != application_list_.end()) {
    MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
      (*it)->app_id(), unregister_reason_);

    UnregisterApplication((*it)->app_id(), true);
    it = application_list_.begin();
  }
}

void ApplicationManagerImpl::UnregisterApplication(const uint32_t& app_id, bool is_resuming) {
  LOG4CXX_INFO(logger_,
               "ApplicationManagerImpl::UnregisterApplication " << app_id);

  sync_primitives::AutoLock lock(applications_list_lock_);

  std::map<int32_t, Application*>::iterator it = applications_.find(app_id);
  if (applications_.end() == it) {
    LOG4CXX_INFO(logger_, "Application is already unregistered.");
    return;
  }

  MessageHelper::RemoveAppDataFromHMI(it->second);
  MessageHelper::SendOnAppUnregNotificationToHMI(it->second, is_resuming);
  Application* app_to_remove = it->second;
  applications_.erase(it);
  application_list_.erase(app_to_remove);
  request_ctrl_.terminateAppRequests(app_id);
  delete app_to_remove;

  return;
}

void ApplicationManagerImpl::Handle(const impl::MessageFromMobile& message) {
  LOG4CXX_INFO(logger_, "Received message from Mobile side");

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }
  ProcessMessageFromMobile(message);
}

void ApplicationManagerImpl::Handle(const impl::MessageToMobile& message) {
  protocol_handler::RawMessage* rawMessage = 0;
  if (message->protocol_version() == application_manager::kV1) {
    rawMessage = MobileMessageHandler::HandleOutgoingMessageProtocolV1(
                   message);
  } else if (message->protocol_version() == application_manager::kV2) {
    rawMessage = MobileMessageHandler::HandleOutgoingMessageProtocolV2(
                   message);
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

  protocol_handler_->SendMessageToMobileApp(rawMessage, message.is_final);

  LOG4CXX_INFO(logger_, "Message for mobile given away.");
}

void ApplicationManagerImpl::Handle(const impl::MessageFromHmi& message) {
  LOG4CXX_INFO(logger_, "Received message from hmi");

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }

  ProcessMessageFromHMI(message);
}

void ApplicationManagerImpl::Handle(const impl::MessageToHmi& message) {
  LOG4CXX_INFO(logger_, "Received message to hmi");
  if (!hmi_handler_) {
    LOG4CXX_ERROR(logger_, "Observer is not set for HMIMessageHandler");
    return;
  }

  hmi_handler_->SendMessageToHMI(message);
  LOG4CXX_INFO(logger_, "Message from hmi given away.");
}

ResumeCtrl* ApplicationManagerImpl::GetResumeController() {
  return &resume_controler;
}

void ApplicationManagerImpl::Mute() {
  mobile_apis::AudioStreamingState::eType state =
      hmi_capabilities_.attenuated_supported()
      ? mobile_apis::AudioStreamingState::ATTENUATED
      : mobile_apis::AudioStreamingState::NOT_AUDIBLE;

  std::set<Application*>::const_iterator it = application_list_.begin();
  std::set<Application*>::const_iterator itEnd = application_list_.end();
  for (; it != itEnd; ++it) {
    if ((*it)->is_media_application()) {
      (*it)->set_audio_streaming_state(state);
      (*it)->set_tts_speak_state(true);
      MessageHelper::SendHMIStatusNotification(*(*it));
    }
  }
}

void ApplicationManagerImpl::Unmute() {
  std::set<Application*>::const_iterator it = application_list_.begin();
  std::set<Application*>::const_iterator itEnd = application_list_.end();
  for (; it != itEnd; ++it) {
    if ((*it)->is_media_application()) {
      (*it)->set_audio_streaming_state(
        mobile_apis::AudioStreamingState::AUDIBLE);
      (*it)->set_tts_speak_state(false);
      MessageHelper::SendHMIStatusNotification(*(*it));
    }
  }
}

mobile_apis::Result::eType ApplicationManagerImpl::SaveBinary(const std::string& app_name,
														    const std::vector<uint8_t>& binary_data,
                                                            const std::string& save_path,
                                                            const uint32_t offset) {
  if (binary_data.size() > file_system::GetAvailableSpaceForApp(app_name)) {
    return mobile_apis::Result::OUT_OF_MEMORY;
  }
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::SaveBinaryWithOffset  binary_size = "
               << binary_data.size() << " offset = " << offset);
  uint32_t file_size = file_system::FileSize(file_system::FullPath(save_path));
  std::ofstream* file_stream;
  if (offset != 0) {
      if (file_size != offset) {
          LOG4CXX_INFO(logger_, "ApplicationManagerImpl::SaveBinaryWithOffset offset does'n match existing filesize");
          return mobile_apis::Result::INVALID_DATA;
        }
      file_stream = file_system::Open(file_system::FullPath(save_path),
                                      std::ios_base::app);
    }  else {
      LOG4CXX_INFO(logger_, "ApplicationManagerImpl::SaveBinaryWithOffset offset is 0, rewrite");
       // if offset == 0: rewrite file
      file_stream = file_system::Open(file_system::FullPath(save_path),
                                      std::ios_base::out);
    }

  if (!file_system::Write(file_stream,binary_data.data(), binary_data.size())) {
      file_stream->close();
      return mobile_apis::Result::GENERIC_ERROR;
    }
  file_stream->close();
  LOG4CXX_INFO(logger_, "Successfully write data to file");
  return mobile_apis::Result::SUCCESS;
}

}  // namespace application_manager
