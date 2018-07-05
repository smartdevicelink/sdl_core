/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "application_manager/rpc_service_impl.h"

namespace application_manager {
namespace rpc_service {

CREATE_LOGGERPTR_LOCAL(logger_, "RPCServiceImpl")
namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

RPCServiceImpl::RPCServiceImpl(
    ApplicationManager& app_manager,
    request_controller::RequestController& request_ctrl,
    protocol_handler::ProtocolHandler* protocol_handler,
    hmi_message_handler::HMIMessageHandler* hmi_handler,
    CommandHolder& commands_holder)
    : app_manager_(app_manager)
    , request_ctrl_(request_ctrl)
    , protocol_handler_(protocol_handler)
    , hmi_handler_(hmi_handler)
    , commands_holder_(commands_holder)
    , messages_to_mobile_("AM ToMobile", this)
    , messages_to_hmi_("AM ToHMI", this)
    , hmi_so_factory_(hmi_apis::HMI_API())
    , mobile_so_factory_(mobile_apis::MOBILE_API()) {}

RPCServiceImpl::~RPCServiceImpl() {}

bool RPCServiceImpl::ManageMobileCommand(
    const commands::MessageSharedPtr message,
    commands::Command::CommandSource source) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    return false;
  }

  if (app_manager_.IsLowVoltage()) {
    LOG4CXX_WARN(logger_, "Low Voltage is active");
    return false;
  }
#ifdef DEBUG
  MessageHelper::PrintSmartObject(*message);
#endif

  const uint32_t connection_key = static_cast<uint32_t>(
      (*message)[strings::params][strings::connection_key].asUInt());

  auto app_ptr = app_manager_.application(connection_key);
  if (app_ptr && app_manager_.IsAppInReconnectMode(app_ptr->policy_app_id())) {
    commands_holder_.Suspend(
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
      (*message)[strings::params][strings::protocol_type].asInt();

  ApplicationSharedPtr app;
  if (((mobile_apis::FunctionID::RegisterAppInterfaceID != function_id) &&
       (protocol_type == commands::CommandImpl::mobile_protocol_type_)) &&
      (mobile_apis::FunctionID::UnregisterAppInterfaceID != function_id)) {
    app = app_manager_.application(connection_key);
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

  auto plugin =
      app_manager_.GetPluginManager().FindPluginToProcess(function_id, source);
  if (!plugin) {
    LOG4CXX_WARN(logger_, "Filed to find plugin : " << plugin.error());
    return false;
  }
  application_manager::CommandFactory& factory = (*plugin).GetCommandFactory();
  auto command = factory.CreateCommand(message, source);

  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create mobile command from smart object");
    return false;
  }

  int32_t message_type =
      (*message)[strings::params][strings::message_type].asInt();
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
      ApplicationSharedPtr app_ptr = app_manager_.application(connection_key);
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
          commands::Command::SOURCE_SDL);

      app_manager_.UnregisterApplication(
          connection_key,
          mobile_apis::Result::TOO_MANY_PENDING_REQUESTS,
          false);
      ApplicationSharedPtr app_ptr = app_manager_.application(connection_key);
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
          commands::Command::SOURCE_SDL);

      ApplicationSharedPtr app_ptr = app_manager_.application(connection_key);
      if (app_ptr) {
        app_ptr->usage_report().RecordRemovalsForBadBehavior();
      }
      app_manager_.UnregisterApplication(
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

bool RPCServiceImpl::ManageHMICommand(
    const commands::MessageSharedPtr message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_WARN(logger_, "Null-pointer message received.");
    return false;
  }

  if (app_manager_.IsLowVoltage()) {
    LOG4CXX_WARN(logger_, "Low Voltage is active");
    return false;
  }

  MessageHelper::PrintSmartObject(*message);
  const int32_t function_id =
      (*(message.get()))[strings::params][strings::function_id].asInt();
  auto plugin = app_manager_.GetPluginManager().FindPluginToProcess(
      function_id, commands::Command::SOURCE_HMI);
  if (!plugin) {
    LOG4CXX_WARN(logger_, "Filed to find plugin : " << plugin.error());
    return false;
  }

  application_manager::CommandFactory& factory = (*plugin).GetCommandFactory();
  auto command = factory.CreateCommand(message, commands::Command::SOURCE_HMI);

  if (!command) {
    LOG4CXX_WARN(logger_, "Failed to create command from smart object");
    return false;
  }

  if ((*message).keyExists(strings::msg_params) &&
      (*message)[strings::msg_params].keyExists(strings::app_id)) {
    const auto connection_key =
        (*message)[strings::msg_params][strings::app_id].asUInt();

    auto app = app_manager_.application(static_cast<uint32_t>(connection_key));
    if (app && app_manager_.IsAppInReconnectMode(app->policy_app_id())) {
      commands_holder_.Suspend(
          app, CommandHolder::CommandType::kHmiCommand, message);
      return true;
    }
  }

  int32_t message_type =
      (*(message.get()))[strings::params][strings::message_type].asInt();

  if (kRequest == message_type) {
    LOG4CXX_DEBUG(logger_, "ManageHMICommand");
    request_ctrl_.addHMIRequest(command);
  }

  if (command->Init()) {
    command->Run();
    if (kResponse == message_type) {
      const uint32_t correlation_id =
          (*(message.get()))[strings::params][strings::correlation_id].asUInt();
      const int32_t function_id =
          (*(message.get()))[strings::params][strings::function_id].asInt();
      request_ctrl_.OnHMIResponse(correlation_id, function_id);
    }
    return true;
  }
  return false;
}

void RPCServiceImpl::Handle(const impl::MessageToHmi message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!hmi_handler_) {
    LOG4CXX_ERROR(logger_, "Observer is not set for HMIMessageHandler");
    return;
  }

  hmi_handler_->SendMessageToHMI(message);
  LOG4CXX_INFO(logger_, "Message for HMI given away");
}

void RPCServiceImpl::Handle(const impl::MessageToMobile message) {
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
    if (1 < app_manager_.connection_handler().GetConnectionSessionsCount(
                message->connection_key())) {
      is_final = false;
      close_session = true;
    }
  }

  protocol_handler_->SendMessageToMobileApp(rawMessage, is_final);
  LOG4CXX_INFO(logger_, "Message for mobile given away");

  if (close_session) {
    app_manager_.connection_handler().CloseSession(message->connection_key(),
                                                   connection_handler::kCommon);
  }
}

void RPCServiceImpl::SendMessageToMobile(
    const application_manager::commands::MessageSharedPtr message,
    bool final_message) {
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

  ApplicationSharedPtr app = app_manager_.application(
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
          app_manager_.SupportedSDLVersion();
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
        app_manager_.CheckPolicyPermissions(app, string_functionID, params);
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
        app_manager_.GetPolicyHandler().OnUpdateRequestSentToMobile();
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

void RPCServiceImpl::SendMessageToHMI(
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

void RPCServiceImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler* handler) {
  protocol_handler_ = handler;
}

void RPCServiceImpl::set_hmi_message_handler(
    hmi_message_handler::HMIMessageHandler* handler) {
  hmi_handler_ = handler;
}

bool RPCServiceImpl::ConvertSOtoMessage(
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& message,
    Message& output) {
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

hmi_apis::HMI_API& RPCServiceImpl::hmi_so_factory() {
  return hmi_so_factory_;
}

mobile_apis::MOBILE_API& RPCServiceImpl::mobile_so_factory() {
  return mobile_so_factory_;
}

}  // namespace rpc_service

}  // namespace application_manager
