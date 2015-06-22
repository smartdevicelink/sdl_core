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

#include "can_cooperation/can_tcp_connection.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/policy_helper.h"
#include "can_cooperation/can_app_extension.h"
#include "can_cooperation/module_helper.h"
#include "utils/logger.h"

namespace can_cooperation {

using functional_modules::ProcessResult;
using functional_modules::GenericModule;
using functional_modules::PluginInfo;
using functional_modules::MobileFunctionID;
using event_engine::EventDispatcher;
namespace hmi_api = functional_modules::hmi_api;

EXPORT_FUNCTION_IMPL(CANModule);
CREATE_LOGGERPTR_GLOBAL(logger_, "CanModule")

CANModule::CANModule()
  : GenericModule(kCANModuleID),
    can_connection_(new CANTCPConnection),
    from_can_("FromCan To Mobile", this),
    from_mobile_("FromMobile To Can", this),
    is_scan_started_(false) {
  can_connection_->set_observer(this);
  plugin_info_.name = "ReverseSDLPlugin";
  plugin_info_.version = 1;
  SubscribeOnFunctions();
}

void CANModule::SubscribeOnFunctions() {
 /* plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_RADIO);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_UP);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::TUNE_DOWN);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::START_SCAN);
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::STOP_SCAN);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::GET_SEAT_CONTROL);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::CLIMATE_CONTROL_ON);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::ON_RADIO_DETAILS);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::ON_PRESETS_CHANGED);*/
  plugin_info_.mobile_function_list.push_back(MobileFunctionID::BUTTON_PRESS);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::GET_INTERIOR_VEHICLE_DATA_CAPABILITIES);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::GET_INTERIOR_VEHICLE_DATA);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::SET_INTERIOR_VEHICLE_DATA);
  plugin_info_.mobile_function_list.push_back(
    MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);

  plugin_info_.hmi_function_list.push_back(
    hmi_api::get_interior_vehicle_data_capabilities);
  plugin_info_.hmi_function_list.push_back(hmi_api::get_interior_vehicle_data);
  plugin_info_.hmi_function_list.push_back(hmi_api::set_interior_vehicle_data);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_interior_vehicle_data);
  plugin_info_.hmi_function_list.push_back(hmi_api::button_press);

  plugin_info_.hmi_function_list.push_back(hmi_api::get_user_consent);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_reverse_apps_allowing);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_set_drivers_device);
  plugin_info_.hmi_function_list.push_back(hmi_api::on_app_deactivated);
  plugin_info_.hmi_function_list.push_back(hmi_api::sdl_activate_app);
}

CANModule::~CANModule() {
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

  msg->set_function_name(MessageHelper::GetMobileAPIName(
                           static_cast<functional_modules::MobileFunctionID>(msg->function_id())));

  commands::Command* command = MobileCommandFactory::CreateCommand(msg);
  if (command) {
    request_controller_.AddRequest(msg->correlation_id(), command);
    command->Run();
  } else {
    return ProcessResult::CANNOT_PROCESS;
  }

  return ProcessResult::PROCESSED;
}

void CANModule::SendMessageToCan(const MessageFromMobile& msg) {
  LOG4CXX_INFO(logger_, "Message to Can: " << msg);
  from_mobile_.PostMessage(msg);
}

ProcessResult CANModule::ProcessHMIMessage(
  application_manager::MessagePtr msg) {
  LOG4CXX_INFO(logger_, "HMI message: " << msg->json_message());
  return HandleMessage(msg);
}

void CANModule::OnCANMessageReceived(const CANMessage& message) {
  from_can_.PostMessage(MessageFromCAN(message));
}

void CANModule::OnCANConnectionError(ConnectionState state,
                                     const std::string& info) {
  if (ConnectionState::INVALID == state) {
    this->NotifyObservers(
      functional_modules::ModuleObserver::CAN_CONNECTION_FAILURE);
  }
  // TODO(PV): remove pending requests to CAN with error response.
}

void CANModule::Handle(const MessageFromMobile message) {
  if (ConnectionState::OPENED != can_connection_->SendMessage(message)) {
    LOG4CXX_ERROR(logger_, "Failed to send message to CAN");
  }
}

void CANModule::Handle(const MessageFromCAN can_msg) {
  application_manager::MessagePtr msg(
    new application_manager::Message(
      protocol_handler::MessagePriority::kDefault));

  Json::FastWriter writer;
  msg->set_json_message(writer.write(can_msg));

  LOG4CXX_INFO(logger_, "Can message: " << can_msg);

  if (HandleMessage(msg) != ProcessResult::PROCESSED) {
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
  if (value.isMember(json_keys::kMethod)) {
    function_name = value[json_keys::kMethod].asCString();

    if (value.isMember(json_keys::kId)) {
      msg->set_message_type(application_manager::MessageType::kRequest);
    } else {
      msg->set_message_type(application_manager::MessageType::kNotification);
    }
    // Response
  } else if (value.isMember(json_keys::kResult)
             && value[json_keys::kResult].isMember(json_keys::kMethod)) {
    function_name = value[json_keys::kResult][json_keys::kMethod].asCString();
    msg->set_message_type(application_manager::MessageType::kResponse);
    // Error response
  } else if (value.isMember(json_keys::kError)
             && value[json_keys::kError].isMember(json_keys::kData)
             && value[json_keys::kError][json_keys::kData]
             .isMember(json_keys::kMethod)) {
    function_name = value[json_keys::kError][json_keys::kData]
                    [json_keys::kMethod].asCString();
    msg->set_message_type(application_manager::MessageType::kErrorResponse);
  } else {
    DCHECK(false);
    return ProcessResult::FAILED;
  }

  if (value.isMember(json_keys::kId)) {
    msg->set_correlation_id(value[json_keys::kId].asInt());
  } else if (application_manager::MessageType::kNotification != msg->type()) {
    DCHECK(false);
    return ProcessResult::FAILED;
  }

  msg->set_protocol_version(application_manager::ProtocolVersion::kV3);

  switch (msg->type()) {
    case application_manager::MessageType::kResponse:
    case application_manager::MessageType::kErrorResponse: {
      CanModuleEvent event(msg, function_name);
      EventDispatcher<application_manager::MessagePtr, std::string>::instance()
      ->raise_event(event);
      break;
    }
    case application_manager::MessageType::kNotification: {
      if (functional_modules::hmi_api::on_interior_vehicle_data
                       == function_name) {
        msg->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);
/*     } else if (functional_modules::can_api::on_preset_changed == function_name) {
        msg->set_function_id(MobileFunctionID::ON_PRESETS_CHANGED);
      } else if (functional_modules::can_api::on_radio_details
                 == function_name) {
        msg->set_function_id(MobileFunctionID::ON_RADIO_DETAILS);*/
      } else if (functional_modules::hmi_api::on_reverse_apps_allowing
                 == function_name) {
        if (value.isMember(json_keys::kParams)) {
          if (value[json_keys::kParams].isMember(message_params::kAllowed)) {
            PolicyHelper::OnRSDLFunctionalityAllowing(
              value[json_keys::kParams][message_params::kAllowed].asBool());
          } else {
            LOG4CXX_ERROR(logger_,
                          "Invalid OnReverseAppsAllowing notification");
          }
        }
        break;
      } else if (functional_modules::hmi_api::on_set_drivers_device
          == function_name) {
        if (value.isMember(json_keys::kParams)) {
          if (value[json_keys::kParams].isMember(message_params::kDevice)) {
            PolicyHelper::SetPrimaryDevice(
              value[json_keys::kParams][message_params::kDevice]
              [json_keys::kId].asUInt());
          } else {
            LOG4CXX_ERROR(logger_, "Invalid RC.OnSetDriverDevice notification");
          }
        }
        return ProcessResult::PROCESSED;
      } else if (functional_modules::hmi_api::on_app_deactivated ==
                 function_name) {
        return ProcessOnAppDeactivation(value);
      }

      int32_t func_id = msg->function_id();
      std::string func_name = MessageHelper::GetMobileAPIName(
                                static_cast<functional_modules::MobileFunctionID>(func_id));
      msg->set_function_name(func_name);

      NotifyMobiles(msg);
      break;
    }
    case application_manager::MessageType::kRequest: {
      if (function_name == functional_modules::hmi_api::sdl_activate_app) {
        msg->set_protocol_version(application_manager::ProtocolVersion::kHMI);
        return ProcessSDLActivateApp(value);
      }
      return ProcessResult::CANNOT_PROCESS;
    }
    default: {
      return ProcessResult::FAILED;
    }
  }

  return ProcessResult::PROCESSED;
}

void CANModule::NotifyMobiles(application_manager::MessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  commands::Command* command = MobileCommandFactory::CreateCommand(message);
  if (command) {
    command->Run();
  }
  delete command;
}

void CANModule::SendResponseToMobile(application_manager::MessagePtr msg) {
  service()->SendMessageToMobile(msg);
  request_controller_.DeleteRequest(msg->correlation_id());
}

void CANModule::SendTimeoutResponseToMobile(
  application_manager::MessagePtr msg) {
  service()->SendMessageToMobile(msg);
}

bool CANModule::IsScanStarted() const {
  return is_scan_started_;
}

void CANModule::SetScanStarted(bool is_scan_started) {
  is_scan_started_ = is_scan_started;
}

void CANModule::RemoveAppExtensions() {
  std::vector<application_manager::ApplicationSharedPtr> applications =
    service()->GetApplications(this->GetModuleID());

  std::vector<application_manager::ApplicationSharedPtr>::iterator it =
    applications.begin();

  for (; it != applications.end(); ++it) {
    application_manager::ApplicationSharedPtr app = *it;
    if (app) {
      service()->ResetAccess(app->app_id());
      app->RemoveExtension(CANModule::instance()->GetModuleID());
    }
  }
}

void CANModule::RemoveAppExtension(uint32_t app_id) {
  application_manager::ApplicationSharedPtr app = service()->GetApplication(
        app_id);

  if (app) {
    service()->ResetAccess(app->app_id());
    app->RemoveExtension(kCANModuleID);
  }
}

bool CANModule::IsAppForPlugin(
  application_manager::ApplicationSharedPtr app) {
  if (app->app_types()) {
    std::vector<int> hmi_types =
      application_manager::SmartObjToArrayInt(app->app_types());
    if (hmi_types.end() != std::find(hmi_types.begin(), hmi_types.end(),
                                     mobile_apis::AppHMIType::eType::REMOTE_CONTROL)) {
      CANAppExtensionPtr can_app_extension = new CANAppExtension(GetModuleID());
      app->AddExtension(can_app_extension);
      return true;
    }
  }
  return false;
}

}  //  namespace can_cooperation

