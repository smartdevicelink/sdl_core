/*
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

#include "can_cooperation/can_module.h"
#include "can_cooperation/mobile_command_factory.h"
#include "can_cooperation/can_module_event.h"
#include "can_cooperation/event_engine/event_dispatcher.h"
#include "can_cooperation/can_module_constants.h"
#include "application_manager/application.h"
#include "can_cooperation/can_tcp_connection.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"


namespace can_cooperation {

using namespace json_keys;

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;
using event_engine::EventDispatcher;
namespace hmi_api = functional_modules::hmi_api;

EXPORT_FUNCTION_IMPL(CANModule);
CREATE_LOGGERPTR_GLOBAL(logger_, "CanModule");

class TCPClientDelegate : public threads::ThreadDelegate {
 public:
  explicit TCPClientDelegate(CANModule* can_module);
  ~TCPClientDelegate();
  void threadMain();
  bool exitThreadMain();
 private:
  CANModule* can_module_;
  bool stop_flag_;
};

TCPClientDelegate::TCPClientDelegate(CANModule* can_module)
: can_module_(can_module)
, stop_flag_(false) {
  DCHECK(can_module);
}

TCPClientDelegate::~TCPClientDelegate() {
  can_module_ = NULL;
  stop_flag_ = true;
}

void TCPClientDelegate::threadMain() {
  while (!stop_flag_) {
    while (ConnectionState::OPENED ==
        can_module_->can_connection->GetData()) {
      can_module_->from_can_.PostMessage(
        static_cast<CANTCPConnection*>(can_module_->can_connection.get())
          ->ReadData());
    }
  }
}

bool TCPClientDelegate::exitThreadMain() {
  stop_flag_ = true;
  return false;
}

CANModule::CANModule()
  : GenericModule(kCANModuleID)
  , can_connection()
  , from_can_("FromCan To Mobile", this)
  , from_mobile_("FromMobile To Can", this)
  , thread_(NULL)
  , is_scan_started_(false) {
    can_connection = new CANTCPConnection;
    if (ConnectionState::OPENED != can_connection->OpenConnection()) {
      LOG4CXX_ERROR(logger_, "Failed to connect to CAN");
    } else {
      thread_ = new threads::Thread("CANClientListener",
                                    new TCPClientDelegate(this));
      const size_t kStackSize = 16384;
      thread_->startWithOptions(threads::ThreadOptions(kStackSize));
    }
  plugin_info_.name = "ReverseSDLPlugin";
  plugin_info_.version = 1;
  SubscribeOnFunctions();
}

void CANModule::SubscribeOnFunctions() {
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_RADIO);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_UP);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_DOWN);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::GRANT_ACCESS);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::CANCEL_ACCESS);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::START_SCAN);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::STOP_SCAN);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::GET_SEAT_CONTROL);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::CLIMATE_CONTROL_ON);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::ON_CONTROL_CHANGED);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::ON_RADIO_DETAILS);
  plugin_info_.mobile_function_list.push_back(
      MobileFunctionID::ON_PRESETS_CHANGED);

  plugin_info_.hmi_function_list.push_back(hmi_api::grant_access);
  plugin_info_.hmi_function_list.push_back(hmi_api::cancel_access);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_control_changed);
}

CANModule::~CANModule() {
  if (can_connection) {
    can_connection->CloseConnection();
  }
  if (thread_) {
    thread_->stop();
    delete thread_;
  }
  RemoveAppExtensions();
}

functional_modules::PluginInfo CANModule::GetPluginInfo() const {
  return plugin_info_;
}

ProcessResult CANModule::ProcessMessage(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message received.");
    return ProcessResult::FAILED;
  }

  commands::Command* command = MobileCommandFactory::CreateCommand(msg);
  if (command) {
    request_controller_.AddRequest(msg->correlation_id(), command);
    command->Run();
  } else {
    return ProcessResult::CANNOT_PROCESS;
  }

  return ProcessResult::PROCESSED;
}

void CANModule::SendMessageToCan(const std::string& msg) {
  LOG4CXX_INFO(logger_, "Message to Can: " << msg);
  from_mobile_.PostMessage(msg);
}

ProcessResult CANModule::ProcessHMIMessage(
              application_manager::MessagePtr msg) {
  LOG4CXX_INFO(logger_, "HMI message: " << msg->json_message());
  return HandleMessage(msg);
}

void CANModule::ProcessCANMessage(const MessageFromCAN& can_msg) {
  DCHECK(Json::ValueType::objectValue == can_msg.type());
  from_can_.PostMessage(can_msg);
}

void CANModule::Handle(const std::string message) {
  static_cast<CANTCPConnection*>(can_connection.get())->WriteData(
      message);

  if (ConnectionState::OPENED != can_connection->Flash()) {
    LOG4CXX_ERROR(logger_, "Failed to send message to CAN");
  }
}

void CANModule::Handle(const MessageFromCAN can_msg) {
  application_manager::MessagePtr msg(new application_manager::Message(
      protocol_handler::MessagePriority::kDefault));

  Json::FastWriter writer;
  std::string json_string = writer.write(can_msg);
  msg->set_json_message(json_string);

  LOG4CXX_INFO(logger_, "Can message: " << json_string);

  if (ProcessResult::PROCESSED != HandleMessage(msg)) {
    LOG4CXX_ERROR(logger_, "Failed process CAN message!");
  }
}

functional_modules::ProcessResult CANModule::HandleMessage(
    application_manager::MessagePtr msg) {

  LOG4CXX_INFO(logger_, "CANModule::HandleMessage");

  Json::Value value;
  Json::Reader reader;
  reader.parse(msg->json_message(), value);

  std::string function_name;

  // Request or notification
  if (value.isMember(kMethod)) {
    function_name = value[kMethod].asCString();

    if (value.isMember(kId)) {
      msg->set_message_type(application_manager::MessageType::kRequest);
    } else {
      msg->set_message_type(application_manager::MessageType::kNotification);
    }
  // Response
  } else if (value.isMember(kResult) && value[kResult].isMember(kMethod)) {
    function_name = value[kResult][kMethod].asCString();
    msg->set_message_type(application_manager::MessageType::kResponse);
  // Error response
  }  else if (value.isMember(kError) && value[kError].isMember(kData) &&
              value[kError][kData].isMember(kMethod)) {
    function_name = value[kError][kData][kMethod].asCString();
    msg->set_message_type(application_manager::MessageType::kErrorResponse);
  } else {
    DCHECK(false);
    return ProcessResult::FAILED;
  }


  if (value.isMember(kId)) {
    msg->set_correlation_id(value[kId].asInt());
  } else if (application_manager::MessageType::kNotification !=  msg->type()) {
    DCHECK(false);
    return ProcessResult::FAILED;
  }

  msg->set_protocol_version(application_manager::ProtocolVersion::kV3);

  switch (msg->type()) {
    case application_manager::MessageType::kResponse:
    case application_manager::MessageType::kErrorResponse: {
      CanModuleEvent event(msg, function_name);
      EventDispatcher<application_manager::MessagePtr,
                      std::string>::instance()->
          raise_event(event);
      break;
    }
    case application_manager::MessageType::kNotification: {
      if (functional_modules::can_api::on_preset_changed ==
          function_name) {
        msg->set_function_id(MobileFunctionID::ON_PRESETS_CHANGED);
      } else if (functional_modules::hmi_api::on_control_changed ==
                 function_name) {
        msg->set_function_id(MobileFunctionID::ON_CONTROL_CHANGED);
      } else if (functional_modules::can_api::on_radio_details ==
                 function_name) {
        msg->set_function_id(MobileFunctionID::ON_RADIO_DETAILS);
      }

      commands::Command* command = MobileCommandFactory::CreateCommand(msg);
      if (command) {
        command->Run();
      }

      break;
    }
    case application_manager::MessageType::kRequest:
    default: {
      return  ProcessResult::FAILED;;
    }
  }

  return ProcessResult::PROCESSED;
}

void CANModule::SendResponseToMobile(application_manager::MessagePtr msg) {
  service_->SendMessageToMobile(msg);
  request_controller_.DeleteRequest(msg->correlation_id());
}

bool CANModule::IsScanStarted() const {
  return is_scan_started_;
}

void CANModule::SetScanStarted(bool is_scan_started) {
  is_scan_started_ = is_scan_started;
}

void CANModule::RemoveAppExtensions() {
  // TODO(VS): fill this method or get rid of it
}

void CANModule::RemoveAppExtension(uint32_t app_id) {
  application_manager::ApplicationSharedPtr app =
      service_->GetApplication(app_id);

  if (app.valid()) {
    application_manager::AppExtensionPtr extension =
        app->QueryInterface(kCANModuleID);
    if (extension.valid()) {
      // TOD(VS) : memory leak
      app->RemoveExtension(kCANModuleID);
    }
  }
}

application_manager::ServicePtr CANModule::GetServiceHandler() {
  return service_;
}

}  //  namespace can_cooperation

