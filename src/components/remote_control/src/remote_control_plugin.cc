/*
 Copyright (c) 2017, Ford Motor Company
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

#include "remote_control/remote_control_plugin.h"
#include "remote_control/rc_command_factory.h"
#include "remote_control/remote_control_event.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/rc_app_extension.h"
#include "remote_control/message_helper.h"
#include "utils/logger.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"
#include "utils/make_shared.h"
#include "application_manager/smart_object_keys.h"

namespace remote_control {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::RCFunctionID;
namespace hmi_api = functional_modules::hmi_api;

using namespace json_keys;

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControl");

EXPORT_FUNCTION_IMPL(remote_control::RemoteControlPlugin);

RemoteControlPlugin::RemoteControlPlugin()
    : is_scan_started_(false), resource_allocation_manager_(*this) {
  plugin_info_.name = "RemoteControlPlugin";
  plugin_info_.version = 1;
  SubscribeOnFunctions();
}

void RemoteControlPlugin::SubscribeOnFunctions() {
  plugin_info_.rc_function_list.push_back(RCFunctionID::BUTTON_PRESS);
  plugin_info_.rc_function_list.push_back(
      RCFunctionID::GET_INTERIOR_VEHICLE_DATA);
  plugin_info_.rc_function_list.push_back(
      RCFunctionID::SET_INTERIOR_VEHICLE_DATA);
  plugin_info_.rc_function_list.push_back(
      RCFunctionID::ON_INTERIOR_VEHICLE_DATA);

  plugin_info_.hmi_function_list.push_back(hmi_api::get_interior_vehicle_data);
  plugin_info_.hmi_function_list.push_back(hmi_api::set_interior_vehicle_data);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_interior_vehicle_data);
  plugin_info_.hmi_function_list.push_back(hmi_api::button_press);
  plugin_info_.hmi_function_list.push_back(hmi_api::get_user_consent);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_remote_control_settings);
}

RemoteControlPlugin::~RemoteControlPlugin() {
  RemoveAppExtensions();
}

functional_modules::PluginInfo RemoteControlPlugin::GetPluginInfo() const {
  return plugin_info_;
}

const std::string ExtractFunctionAndAddMetadata(
    const Json::Value& value, application_manager::Message& out_msg) {
  if (value.isMember(json_keys::kMethod)) {
    const std::string& function_name =
        value.get(json_keys::kMethod, "").asCString();

    // Existence of method name must be guaranteed by plugin manager
    DCHECK_OR_RETURN(!function_name.empty(), "");

    if (value.isMember(json_keys::kId)) {
      out_msg.set_correlation_id(value.get(json_keys::kId, "").asInt());
      out_msg.set_message_type(application_manager::MessageType::kRequest);
    } else {
      out_msg.set_message_type(application_manager::MessageType::kNotification);
    }
    return function_name;
  }

  if (value.isMember(json_keys::kResult)) {
    const Json::Value& result = value.get(json_keys::kResult, Json::Value());
    const std::string& function_name =
        result.get(json_keys::kMethod, "").asCString();
    out_msg.set_correlation_id(value.get(json_keys::kId, "").asInt());

    // Existence of method name must be guaranteed by plugin manager
    DCHECK_OR_RETURN(!function_name.empty(), "");

    out_msg.set_message_type(application_manager::MessageType::kResponse);
    return function_name;
  }

  if (value.isMember(json_keys::kError)) {
    const Json::Value& error = value.get(json_keys::kError, Json::Value());
    const Json::Value& data = error.get(json_keys::kData, Json::Value());
    const std::string& function_name =
        data.get(json_keys::kMethod, "").asCString();

    // Existence of method name must be guaranteed by plugin manager
    DCHECK_OR_RETURN(!function_name.empty(), "");

    out_msg.set_message_type(application_manager::MessageType::kErrorResponse);
    out_msg.set_correlation_id(value.get(json_keys::kId, "").asInt());
    return function_name;
  }
  return std::string();
}

ProcessResult RemoteControlPlugin::ProcessMessage(
    application_manager::MessagePtr msg) {
  DCHECK_OR_RETURN(msg, ProcessResult::FAILED);

  const std::string& function_name = MessageHelper::GetMobileAPIName(
      static_cast<functional_modules::RCFunctionID>(msg->function_id()));

  LOG4CXX_DEBUG(logger_, "Function name to set : " << function_name);
  msg->set_function_name(function_name);

  LOG4CXX_DEBUG(logger_, "Mobile message: " << msg->json_message());

  request_controller::MobileRequestPtr command(
      RCCommandFactory::CreateCommand(msg, *this));
  if (command) {
    request_controller_.AddRequest(msg->correlation_id(), command);
    command->Run();
  } else {
    return ProcessResult::CANNOT_PROCESS;
  }

  return ProcessResult::PROCESSED;
}

ProcessResult RemoteControlPlugin::ProcessHMIMessage(
    application_manager::MessagePtr msg) {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value value;
  Json::Reader reader;
  reader.parse(msg->json_message(), value);
  LOG4CXX_TRACE(logger_, "Process " << msg->json_message());
  const std::string& function_name = ExtractFunctionAndAddMetadata(value, *msg);

  // Existence of method name must be guaranteed by plugin manager
  DCHECK_OR_RETURN(!function_name.empty(), ProcessResult::FAILED);

  LOG4CXX_DEBUG(logger_, "Process " << function_name);

  switch (msg->type()) {
    case application_manager::MessageType::kResponse:
    case application_manager::MessageType::kErrorResponse: {
      RCPluginEvent event(msg, function_name);
      LOG4CXX_DEBUG(logger_, "Response received");
      event_dispatcher_.raise_event(event);
      return ProcessResult::PROCESSED;
    }
    case application_manager::MessageType::kRequest:
    case application_manager::MessageType::kNotification: {
      if (hmi_api::on_interior_vehicle_data == function_name) {
        msg->set_function_id(functional_modules::ON_INTERIOR_VEHICLE_DATA);
      }
      if (hmi_api::on_remote_control_settings == function_name) {
        msg->set_function_id(functional_modules::ON_REMOTE_CONTROL_SETTINGS);
      }
      const application_manager::MessageValidationResult validation_result =
          service()->ValidateMessageBySchema(*msg);
      utils::SharedPtr<commands::Command> command =
          RCCommandFactory::CreateCommand(msg, *this);
      if ((validation_result ==
           application_manager::MessageValidationResult::SUCCESS) &&
          command) {
        command->Run();
        return ProcessResult::PROCESSED;
      }
      LOG4CXX_DEBUG(logger_, "Message validation failed");
      break;
    }
    default: { LOG4CXX_DEBUG(logger_, "Unknown message type"); }
  }
  return ProcessResult::CANNOT_PROCESS;
}

void RemoteControlPlugin::SendHmiStatusNotification(
    application_manager::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::MessagePtr msg = new application_manager::Message(
      protocol_handler::MessagePriority::kDefault);
  Json::Value msg_params;

  msg->set_function_id(mobile_apis::FunctionID::OnHMIStatusID);
  msg->set_message_type(application_manager::MessageType::kNotification);

  msg->set_connection_key(app->app_id());
  msg->set_protocol_version(app->protocol_version());

  msg_params["hmiLevel"] = static_cast<uint32_t>(app->hmi_level());

  msg_params["audioStreamingState"] =
      static_cast<uint32_t>(app->audio_streaming_state());

  msg_params["systemContext"] = static_cast<uint32_t>(app->system_context());

  msg->set_json_message(MessageHelper::ValueToString(msg_params));

  service()->SendMessageToMobile(msg);
}

void RemoteControlPlugin::SendResponseToMobile(
    application_manager::MessagePtr msg) {
  LOG4CXX_DEBUG(logger_, "Response to mobile: " << msg->json_message());
  service()->SendMessageToMobile(msg);
  request_controller_.DeleteRequest(msg->correlation_id());
}

void RemoteControlPlugin::SendTimeoutResponseToMobile(
    application_manager::MessagePtr msg) {
  LOG4CXX_DEBUG(
      logger_,
      "Timeout is expired. Response to mobile: " << msg->json_message());
  service()->SendMessageToMobile(msg);
}

void RemoteControlPlugin::RemoveAppExtensions() {
  std::vector<application_manager::ApplicationSharedPtr> applications =
      service()->GetApplications(GetModuleID());

  std::vector<application_manager::ApplicationSharedPtr>::iterator it =
      applications.begin();

  for (; it != applications.end(); ++it) {
    application_manager::ApplicationSharedPtr app = *it;
    if (app) {
      app->RemoveExtension(GetModuleID());
    }
  }
}

void RemoteControlPlugin::RemoveAppExtension(uint32_t app_id) {
  application_manager::ApplicationSharedPtr app =
      service()->GetApplication(app_id);

  if (app) {
    app->RemoveExtension(GetModuleID());
  }
}

bool RemoteControlPlugin::IsAppForPlugin(
    application_manager::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::AppExtensionPtr app_extension =
      app->QueryInterface(GetModuleID());
  if (app_extension) {
    return true;
  }

  if (service()->IsRemoteControlApplication(app)) {
    RCAppExtensionPtr rc_app_extension = new RCAppExtension(GetModuleID());
    app->AddExtension(rc_app_extension);
    service()->NotifyHMIAboutHMILevel(app, app->hmi_level());
    return true;
  }
  return false;
}

void RemoteControlPlugin::OnAppHMILevelChanged(
    application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType) {
  LOG4CXX_DEBUG(logger_,
                "RSDL application " << app->name().AsMBString()
                                    << " has changed hmi level to "
                                    << app->hmi_level());
  service()->NotifyHMIAboutHMILevel(app, app->hmi_level());
}

void RemoteControlPlugin::OnUnregisterApplication(const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  resource_allocation_manager_.OnUnregisterApplication(app_id);
}

void RemoteControlPlugin::OnPluginServiceChanged() {
  LOG4CXX_AUTO_TRACE(logger_);
  const functional_modules::TimeUnit timeout_msec =
      service()->GetSettings().default_timeout();
  request_controller_.SetRequestTimeout(timeout_msec / 1000);
}

void RemoteControlPlugin::set_service(application_manager::ServicePtr service) {
  RemotePluginInterface::set_service(service);
  OnPluginServiceChanged();
}

RCEventDispatcher& RemoteControlPlugin::event_dispatcher() {
  return event_dispatcher_;
}

ResourceAllocationManager& RemoteControlPlugin::resource_allocation_manager() {
  return resource_allocation_manager_;
}

}  //  namespace remote_control
