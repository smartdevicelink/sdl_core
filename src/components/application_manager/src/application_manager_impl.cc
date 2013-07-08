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

#include "application_manager/application.h"
#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "application_manager/mobile_command_factory.h"
#include "application_manager/hmi_command_factory.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "application_manager/message_chaining.h"
#include "application_manager/audio_pass_thru_thread_impl.h"
#include "utils/threads/thread.h"
#include "JSONHandler/formatters/formatter_json_rpc.h"
#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"
#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "utils/logger.h"

namespace application_manager {

log4cxx::LoggerPtr ApplicationManagerImpl::logger_   =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("ApplicationManager"));

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

ApplicationManagerImpl::ApplicationManagerImpl()
  : audio_pass_thru_flag_(false),
    perform_audio_thread_(NULL),
    is_distracting_driver_(false),
    is_vr_session_strated_(false),
    hmi_cooperating_(false),
    is_all_apps_allowed_(true),
    ui_language_(hmi_apis::Common_Language::INVALID_ENUM),
    vr_language_(hmi_apis::Common_Language::INVALID_ENUM),
    tts_language_(hmi_apis::Common_Language::INVALID_ENUM),
    vehicle_type_(NULL) {
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  message_chaining_.clear();
  delete perform_audio_thread_;
  if (vehicle_type_) {
    delete vehicle_type_;
  }
}

ApplicationManagerImpl* ApplicationManagerImpl::instance() {
  static ApplicationManagerImpl instance;
  return &instance;
}

Application* ApplicationManagerImpl::application(int app_id) {
  std::map<int, Application*>::iterator it = applications_.find(app_id);
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
  unsigned int button) {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->IsSubscribedToButton(static_cast<mobile_apis::ButtonName::eType>(
                                      button))) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<Application*> ApplicationManagerImpl::applications_by_ivi(
  unsigned int vehicle_info) {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it; ++it) {
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
    if ((*it)->SupportsNavigation()) {
      result.push_back(*it);
    }
  }
  return result;
}

std::set<connection_handler::Device>& ApplicationManagerImpl::device_list() {
  std::set<connection_handler::Device> devices;
  return devices;
}

bool ApplicationManagerImpl::RegisterApplication(Application* application) {
  DCHECK(application);
  if (NULL == application) {
    return false;
  }

  if (false == is_all_apps_allowed_) {
    LOG4CXX_INFO(logger_,
                 "RegisterApplication: access to app's disabled by user");
    return false;
  }

  std::map<int, Application*>::iterator it = applications_.find(
        application->app_id());
  if (applications_.end() != it) {
    return false;
  }
  applications_.insert(std::pair<int, Application*>(
                         application->app_id(), application));
  application_list_.insert(application);
  return true;
}

bool ApplicationManagerImpl::UnregisterApplication(int app_id) {
  std::map<int, Application*>::iterator it = applications_.find(app_id);
  if (applications_.end() == it) {
    return false;
  }
  Application* app_to_remove = it->second;
  applications_.erase(it);
  application_list_.erase(app_to_remove);
  return true;
}

void ApplicationManagerImpl::UnregisterAllApplications(bool hmi_off) {
  applications_.clear();
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    delete(*it);
  }
  application_list_.clear();

  if (hmi_off) {
    hmi_cooperating_ = false;
  }
}

bool ApplicationManagerImpl::RemoveAppDataFromHMI(Application* application) {
  return true;
}

bool ApplicationManagerImpl::LoadAppDataToHMI(Application* application) {
  return true;
}

bool ApplicationManagerImpl::ActivateApplication(Application* application) {
  return true;
}

void ApplicationManagerImpl::ConnectToDevice(unsigned int id) {
  // TODO(VS): Call function from ConnectionHandler
}

void ApplicationManagerImpl::OnHMIStartedCooperation() {
  hmi_cooperating_ = true;
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnHMIStartedCooperation()");

  hmi_apis::HMI_API factory;
  LOG4CXX_INFO(logger_, "factory");
  smart_objects::CSmartObject is_vr_ready =
    factory.CreateSmartObject(hmi_apis::FunctionID::VR_IsReady,
                              hmi_apis::messageType::request);

  LOG4CXX_INFO(logger_, "Sending vr is ready");

  /*CommandSharedPtr command = HMICommandFactory::CreateCommand(is_vr_ready);
  command->Init();
  command->Run();
  command->CleanUp();*/
}

// TODO(VS) : Remove function_id from function parameters(it isn't used)
MessageChaining* ApplicationManagerImpl::AddMessageChain(MessageChaining* chain,
    unsigned int connection_key,
    unsigned int correlation_id,
    unsigned int function_id,
    const NsSmartDeviceLink::NsSmartObjects::CSmartObject* data) {
  if (!chain) {
    chain = new MessageChaining(
      connection_key,
      correlation_id);
    if (data) {
      chain->set_data(*data);
    }

    MessageChainPtr ptr(chain);
    message_chaining_[correlation_id] = ptr;
    return chain;
  } else  {
    MessageChains::const_iterator it = message_chaining_.begin();
    for (; it != message_chaining_.end(); ++it) {
      if ((*it->second) == *chain) {
        it->second->IncrementCounter();
        message_chaining_[correlation_id] = it->second;
        break;
      }
    }
  }
  return chain;
}

bool ApplicationManagerImpl::DecreaseMessageChain(
  unsigned int correlation_id) {
  bool result = false;
  MessageChains::iterator it =
    message_chaining_.find(correlation_id);

  if (message_chaining_.end() != it) {
    (*it->second).DecrementCounter();
    if (!(*it->second).counter()) {
      result = true;
    }
    message_chaining_.erase(it);
  }
  return result;
}

MessageChaining* ApplicationManagerImpl::GetMessageChain(
  unsigned int correlation_id) const {
  MessageChains::const_iterator it =
    message_chaining_.find(correlation_id);
  if (message_chaining_.end() != it) {
    return &(*it->second);
  }

  return NULL;
}

bool ApplicationManagerImpl::audio_pass_thru_flag() const {
  return audio_pass_thru_flag_;
}

void ApplicationManagerImpl::set_audio_pass_thru_flag(bool flag) {
  audio_pass_thru_flag_ = flag;
}

void ApplicationManagerImpl::set_driver_distraction(
  bool is_distracting) {
  is_distracting_driver_ = is_distracting;
}

void ApplicationManagerImpl::set_vr_session_started(const bool& state) {
  is_vr_session_strated_ = state;
}

void ApplicationManagerImpl::set_active_ui_language(
  const hmi_apis::Common_Language::eType& language) {
  ui_language_ = language;
}

void ApplicationManagerImpl::set_active_vr_language(
  const hmi_apis::Common_Language::eType& language) {
  vr_language_ = language;
}

void ApplicationManagerImpl::set_active_tts_language(
  const hmi_apis::Common_Language::eType& language) {
  tts_language_ = language;
}

void ApplicationManagerImpl::set_vehicle_type(
  const smart_objects::CSmartObject& vehicle_type) {
  if (vehicle_type_) {
    delete vehicle_type_;
  }
  vehicle_type_ = new smart_objects::CSmartObject(vehicle_type);
}

const smart_objects::CSmartObject*
ApplicationManagerImpl::vehicle_type() const {
  return vehicle_type_;
}

void ApplicationManagerImpl::set_all_apps_allowed(const bool& allowed) {
  is_all_apps_allowed_ = allowed;
}

void ApplicationManagerImpl::StartAudioPassThruThread(int session_key,
    int correlation_id, int max_duration, int sampling_rate,
    int bits_per_sample, int audio_type) {
  AudioPassThruThreadImpl* thread_impl = new AudioPassThruThreadImpl(
    static_cast<unsigned int>(session_key),
    static_cast<unsigned int>(correlation_id),
    static_cast<unsigned int>(max_duration),
    static_cast<SamplingRate>(sampling_rate),
    static_cast<AudioCaptureQuality>(bits_per_sample),
    static_cast<AudioType>(audio_type));

  thread_impl->Init();
  perform_audio_thread_ = new threads::Thread("AudioPassThru thread",
      thread_impl);

  perform_audio_thread_->startWithOptions(
    threads::ThreadOptions(threads::Thread::kMinStackSize));
}

void ApplicationManagerImpl::StopAudioPassThruThread() {
  perform_audio_thread_->stop();
  delete perform_audio_thread_;
  perform_audio_thread_ = NULL;
}

void ApplicationManagerImpl::OnMobileMessageReceived(
    const MobileMessage& message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::OnMobileMessageReceived");

  DCHECK(message);
  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }

  utils::SharedPtr<smart_objects::CSmartObject> smart_object(
    new smart_objects::CSmartObject);

  if (!ConvertMessageToSO(*message, *smart_object)) {
    LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
    return;
  }

  LOG4CXX_INFO(logger_, "Converted message, trying to create mobile command");
  if (!ManageMobileCommand(smart_object)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
}

void ApplicationManagerImpl::onMessageReceived(
  utils::SharedPtr<application_manager::Message> message) {
  LOG4CXX_INFO(logger_, "ApplicationManagerImpl::onMessageReceived");

  DCHECK(message);
  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }

  utils::SharedPtr<smart_objects::CSmartObject> smart_object(
    new smart_objects::CSmartObject);
  if (!ConvertMessageToSO(*message, *smart_object)) {
    LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
    return;
  }

  LOG4CXX_INFO(logger_, "Converted message, trying to create hmi command");
  if (!ManageHMICommand(smart_object)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
}

void ApplicationManagerImpl::onErrorSending(
  utils::SharedPtr<application_manager::Message> message) {
  return;
}

void ApplicationManagerImpl::OnDeviceListUpdated(
  const connection_handler::DeviceList& device_list) {
  // TODO(DK): HMI StartDeviceDiscovery response
}

void ApplicationManagerImpl::RemoveDevice(
  const connection_handler::DeviceHandle device_handle) {
}

void ApplicationManagerImpl::OnSessionStartedCallback(
  connection_handler::DeviceHandle device_handle, int session_key,
  int first_session_key) {
}

void ApplicationManagerImpl::OnSessionEndedCallback(int session_key,
    int first_session_key) {
}

void ApplicationManagerImpl::onTimeoutExpired(request_watchdog::RequestInfo) {
}

void ApplicationManagerImpl::set_hmi_message_handler(
  hmi_message_handler::HMIMessageHandler* handler) {
  hmi_handler_ = handler;
}

void ApplicationManagerImpl::set_mobile_message_handler(
  mobile_message_handler::MobileMessageHandler* handler) {
  mobile_handler_ = handler;
}

void ApplicationManagerImpl::set_connection_handler(
  connection_handler::ConnectionHandler* handler) {
  connection_handler_ = handler;
}

void ApplicationManagerImpl::set_watchdog(
  request_watchdog::Watchdog* watchdog) {
  watchdog_ = watchdog;
}

void ApplicationManagerImpl::StartDevicesDiscovery() {
  connection_handler::ConnectionHandlerImpl::instance()->
  StartDevicesDiscovery();
}

void ApplicationManagerImpl::SendMessageToMobile(
  const utils::SharedPtr<smart_objects::CSmartObject>& message) {

  DCHECK(message);
  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }

  if (!mobile_handler_) {
    LOG4CXX_WARN(logger_, "No Mobile Handler set");
    return;
  }

  utils::SharedPtr<Message> message_to_send(new Message);
  if (!ConvertSOtoMessage((*message), (*message_to_send))) {
    LOG4CXX_WARN(logger_,
                 "Cannot send message to Mobile: failed to create string");
  }

  mobile_handler_->SendMessageToMobileApp(message_to_send);
}

bool ApplicationManagerImpl::ManageMobileCommand(
  const utils::SharedPtr<smart_objects::CSmartObject>& message) {
  CommandSharedPtr command = MobileCommandFactory::CreateCommand(message);

  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create mobile command from smart object");
  }

  if (command->Init()) {
    command->Run();
    if (command->CleanUp()) {
      return true;
    }
  }
  return false;
}

void ApplicationManagerImpl::SendMessageToHMI(
  const utils::SharedPtr<smart_objects::CSmartObject>& message) {
  if (!hmi_handler_) {
    LOG4CXX_WARN(logger_, "No HMI Handler set");
    return;
  }
  utils::SharedPtr<Message> message_to_send(new Message);
  if (!ConvertSOtoMessage(*message, *message_to_send)) {
    LOG4CXX_WARN(logger_,
                 "Cannot send message to HMI: failed to create string");
  }
  hmi_handler_->sendMessageToHMI(message_to_send);
}

bool ApplicationManagerImpl::ManageHMICommand(
  const utils::SharedPtr<smart_objects::CSmartObject>& message) {
  CommandSharedPtr command = HMICommandFactory::CreateCommand(message);

  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create command from smart object");
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

bool ApplicationManagerImpl::CheckPolicies(smart_objects::CSmartObject* message,
    Application* application) {
  return true;
}

bool ApplicationManagerImpl::CheckHMIMatrix(
  smart_objects::CSmartObject* message) {
  return true;
}

bool ApplicationManagerImpl::ConvertMessageToSO(
  const Message& message, smart_objects::CSmartObject& output) {
  LOG4CXX_INFO(logger_, "Message to convert: protocol " <<
               message.protocol_version() <<
               "; json " << message.json_message());

  switch (message.protocol_version()) {
    case ProtocolVersion::kV2: {
      if (!formatters::CFormatterJsonSDLRPCv2::fromString(
            message.json_message(),
            output,
            message.function_id(),
            message.type(),
            message.correlation_id())) {
        LOG4CXX_WARN(logger_, "Failed to parse string to smart object");
        return false;
      }

      LOG4CXX_INFO(logger_, "Convertion result for sdl object is true" <<
                   " function_id " <<
                   output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
      break;
    }
    case ProtocolVersion::kHMI: {
      int result = formatters::FormatterJsonRpc::FromString <
                   hmi_apis::FunctionID::eType, hmi_apis::messageType::eType > (
                     message.json_message(),
                     output);
      LOG4CXX_INFO(logger_, "Convertion result: " <<
                   result << " function id " <<
                   output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
      /*hmi_apis::HMI_API factory;
      factory.attachSchema(output);
      LOG4CXX_INFO(logger_, "Is object valid? " << output.isValid());*/
      break;
    }
    default:
      NOTREACHED();
      return false;
  }
  LOG4CXX_INFO(logger_, "Successfully parsed message into smart object");
  return true;
}

bool ApplicationManagerImpl::ConvertSOtoMessage(
  const smart_objects::CSmartObject& message, Message& output) {
  LOG4CXX_INFO(logger_, "Message to convert");

  if (smart_objects::SmartType_Null == message.getType() ||
      smart_objects::SmartType_Invalid == message.getType()) {
    LOG4CXX_WARN(logger_, "Invalid smart object received.");
    return false;
  }

  LOG4CXX_INFO(logger_, "Message with protocol: " <<
               message.getElement(
                 jhs::S_PARAMS).getElement(jhs::S_PROTOCOL_VERSION).asInt());

  std::string output_string;
  switch (message.getElement(
            jhs::S_PARAMS).getElement(jhs::S_PROTOCOL_VERSION).asInt()) {
    case 0: {
      if (!formatters::CFormatterJsonSDLRPCv2::toString(
            message,
            output_string)) {
        LOG4CXX_WARN(logger_, "Failed to serialize smart object");
        return false;
      }
      break;
    }
    case 1: {
      if (!formatters::FormatterJsonRpc::ToString(
            message,
            output_string)) {
        LOG4CXX_WARN(logger_, "Failed to serialize smart object");
        return false;
      }
      break;
    }
    default:
      NOTREACHED();
      return false;
  }

  LOG4CXX_INFO(logger_, "Convertion result: " <<
               output_string);

  output.set_function_id(message.getElement(
                           jhs::S_PARAMS).getElement(
                           jhs::S_FUNCTION_ID).asInt());
  output.set_correlation_id(
    message.getElement(jhs::S_PARAMS).getElement(
      jhs::S_CORRELATION_ID).asInt());
  output.set_message_type(
    static_cast<MessageType>(
      message.getElement(jhs::S_PARAMS).getElement(
        jhs::S_MESSAGE_TYPE).asInt()));
  output.set_json_message(output_string);
  output.set_protocol_version(application_manager::kHMI);

  if (message.keyExists(strings::binary_data)) {
    application_manager::BinaryData* binaryData =
        new application_manager::BinaryData(
        message.getElement(strings::binary_data).asBinary());
    output.set_binary_data(binaryData);
  }

  LOG4CXX_INFO(logger_, "Successfully parsed message into smart object");
  return true;
}

}  // namespace application_manager
