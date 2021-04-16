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

#include "application_manager/rpc_handler_impl.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/plugin_manager/plugin_keys.h"

namespace application_manager {
namespace rpc_handler {

SDL_CREATE_LOG_VARIABLE("RPCHandlerImpl")
namespace formatters = ns_smart_device_link::ns_json_handler::formatters;
namespace jhs = ns_smart_device_link::ns_json_handler::strings;
namespace plugin_names = application_manager::plugin_manager::plugin_names;

RPCHandlerImpl::RPCHandlerImpl(ApplicationManager& app_manager,
                               hmi_apis::HMI_API& hmi_so_factory,
                               mobile_apis::MOBILE_API& mobile_so_factory)
    : app_manager_(app_manager)
    , messages_from_mobile_("AM FromMobile", this)
    , messages_from_hmi_("AM FromHMI", this)
    , hmi_so_factory_(hmi_so_factory)
    , mobile_so_factory_(mobile_so_factory)
#ifdef TELEMETRY_MONITOR
    , metric_observer_(NULL)
#endif  // TELEMETRY_MONITOR
{
}

RPCHandlerImpl::~RPCHandlerImpl() {}

void RPCHandlerImpl::ProcessMessageFromMobile(
    const std::shared_ptr<Message> message) {
  SDL_LOG_AUTO_TRACE();
#ifdef TELEMETRY_MONITOR
  AMTelemetryObserver::MessageMetricSharedPtr metric(
      new AMTelemetryObserver::MessageMetric());
  metric->begin = date_time::getCurrentTime();
#endif  // TELEMETRY_MONITOR
  smart_objects::SmartObjectSPtr so_from_mobile =
      std::make_shared<smart_objects::SmartObject>();
  bool allow_unknown_parameters = false;
  DCHECK_OR_RETURN_VOID(so_from_mobile);
  if (!so_from_mobile) {
    SDL_LOG_ERROR("Null pointer");
    return;
  }

  if (message->type() == application_manager::MessageType::kRequest &&
      message->correlation_id() < 0) {
    SDL_LOG_ERROR("Request correlation id < 0. Returning INVALID_ID");
    std::shared_ptr<smart_objects::SmartObject> response(
        MessageHelper::CreateNegativeResponse(message->connection_key(),
                                              message->function_id(),
                                              0,
                                              mobile_apis::Result::INVALID_ID));
    // CreateNegativeResponse() takes a uint32_t for correlation_id, therefore a
    // negative number cannot be passed to that function or else it will be
    // improperly cast. correlation_id is reassigned below to its original
    // value.
    (*response)[strings::params][strings::correlation_id] =
        message->correlation_id();
    (*response)[strings::msg_params][strings::info] =
        "Invalid Correlation ID for RPC Request";
    app_manager_.GetRPCService().ManageMobileCommand(
        response, commands::Command::SOURCE_SDL);
    return;
  }

  bool rpc_passing = app_manager_.GetAppServiceManager()
                         .GetRPCPassingHandler()
                         .CanHandleFunctionID(message->function_id());
  if (app_manager_.GetRPCService().IsAppServiceRPC(
          message->function_id(), commands::Command::SOURCE_MOBILE) ||
      rpc_passing) {
    SDL_LOG_DEBUG("Allowing unknown parameters for request function "
                  << message->function_id());
    allow_unknown_parameters = true;
  }

  std::string warning_info;
  if (!ConvertMessageToSO(*message,
                          *so_from_mobile,
                          warning_info,
                          allow_unknown_parameters,
                          !rpc_passing)) {
    SDL_LOG_ERROR("Cannot create smart object from message");
    return;
  }

  if (rpc_passing) {
    uint32_t correlation_id =
        (*so_from_mobile)[strings::params][strings::correlation_id].asUInt();
    int32_t message_type =
        (*so_from_mobile)[strings::params][strings::message_type].asInt();
    RPCPassingHandler& handler =
        app_manager_.GetAppServiceManager().GetRPCPassingHandler();
    // Check permissions for requests, otherwise attempt passthrough
    if ((application_manager::MessageType::kRequest != message_type ||
         handler.IsPassthroughAllowed(*so_from_mobile)) &&
        handler.RPCPassThrough(*so_from_mobile)) {
      // RPC was forwarded. Skip handling by Core
      return;
    } else if (!handler.IsPassThroughMessage(correlation_id,
                                             commands::Command::SOURCE_MOBILE,
                                             message_type)) {
      // Since PassThrough failed, refiltering the message
      if (!ConvertMessageToSO(*message, *so_from_mobile, warning_info)) {
        SDL_LOG_ERROR("Cannot create smart object from message");
        return;
      }
    }
  }

#ifdef TELEMETRY_MONITOR
  metric->message = so_from_mobile;
#endif  // TELEMETRY_MONITOR

  if (!app_manager_.GetRPCService().ManageMobileCommand(
          so_from_mobile, commands::Command::SOURCE_MOBILE, warning_info)) {
    SDL_LOG_ERROR("Received command didn't run successfully");
  }
#ifdef TELEMETRY_MONITOR
  metric->end = date_time::getCurrentTime();
  if (metric_observer_) {
    metric_observer_->OnMessage(metric);
  }
#endif  // TELEMETRY_MONITOR
}

void RPCHandlerImpl::ProcessMessageFromHMI(
    const std::shared_ptr<Message> message) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObjectSPtr smart_object =
      std::make_shared<smart_objects::SmartObject>();
  bool allow_unknown_parameters = false;

  smart_objects::SmartObject converted_result;
  formatters::FormatterJsonRpc::FromString<hmi_apis::FunctionID::eType,
                                           hmi_apis::messageType::eType>(
      message->json_message(), converted_result);

  const auto function_id = static_cast<int32_t>(
      converted_result[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
  if (app_manager_.GetRPCService().IsAppServiceRPC(
          function_id, commands::Command::SOURCE_HMI)) {
    SDL_LOG_DEBUG("Allowing unknown parameters for request function "
                  << function_id);
    allow_unknown_parameters = true;
  }

  std::string warning_info;
  if (!ConvertMessageToSO(
          *message, *smart_object, warning_info, allow_unknown_parameters)) {
    if (application_manager::MessageType::kResponse ==
        (*smart_object)[strings::params][strings::message_type].asInt()) {
      (*smart_object).erase(strings::msg_params);
      (*smart_object)[strings::params][hmi_response::code] =
          hmi_apis::Common_Result::GENERIC_ERROR;
      (*smart_object)[strings::msg_params][strings::info] =
          std::string("Invalid message received from vehicle");
    } else {
      SDL_LOG_ERROR("Cannot create smart object from message");
      return;
    }
  }

  if (!warning_info.empty()) {
    SDL_LOG_WARN("Warning while parsing HMI message: " << warning_info);
  }

  SDL_LOG_DEBUG("Converted message, trying to create hmi command");
  if (!app_manager_.GetRPCService().ManageHMICommand(
          smart_object, commands::Command::SOURCE_HMI, warning_info)) {
    SDL_LOG_ERROR("Received command didn't run successfully");
  }
}

void RPCHandlerImpl::Handle(const impl::MessageFromMobile message) {
  SDL_LOG_AUTO_TRACE();

  if (!message) {
    SDL_LOG_ERROR("Null-pointer message received.");
    return;
  }
  if (app_manager_.is_stopping()) {
    SDL_LOG_INFO("Application manager is stopping");
    return;
  }
  if (app_manager_.IsLowVoltage()) {
    SDL_LOG_ERROR("Low Voltage is active.");
    return;
  }

  ProcessMessageFromMobile(message);
}

void RPCHandlerImpl::Handle(const impl::MessageFromHmi message) {
  SDL_LOG_AUTO_TRACE();

  if (!message) {
    SDL_LOG_ERROR("Null-pointer message received.");
    return;
  }
  if (app_manager_.IsLowVoltage()) {
    SDL_LOG_ERROR("Low Voltage is active.");
    return;
  }

  ProcessMessageFromHMI(message);
}

void RPCHandlerImpl::OnMessageReceived(
    const protocol_handler::RawMessagePtr message) {
  SDL_LOG_AUTO_TRACE();

  if (app_manager_.IsLowVoltage()) {
    SDL_LOG_ERROR("Low Voltage is active.");
    return;
  }

  if (!message) {
    SDL_LOG_ERROR("Null-pointer message received.");
    NOTREACHED();
    return;
  }

  std::shared_ptr<Message> outgoing_message = ConvertRawMsgToMessage(message);

  if (outgoing_message) {
    SDL_LOG_DEBUG("Posting new Message");
    messages_from_mobile_.PostMessage(
        impl::MessageFromMobile(outgoing_message));
  }
}

void RPCHandlerImpl::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr message) {
  SDL_LOG_AUTO_TRACE();
}

void RPCHandlerImpl::OnMessageReceived(
    hmi_message_handler::MessageSharedPointer message) {
  SDL_LOG_AUTO_TRACE();

  if (!message) {
    SDL_LOG_ERROR("Null-pointer message received.");
    NOTREACHED();
    return;
  }

  messages_from_hmi_.PostMessage(impl::MessageFromHmi(message));
}

void RPCHandlerImpl::OnErrorSending(
    hmi_message_handler::MessageSharedPointer message) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObjectSPtr smart_object =
      std::make_shared<smart_objects::SmartObject>();
  std::string warning_info;
  bool allow_unknown_parameters = true;
  if (ConvertMessageToSO(
          *message, *smart_object, warning_info, allow_unknown_parameters)) {
    (*smart_object)[strings::params][strings::message_type] =
        application_manager::MessageType::kResponse;
    (*smart_object).erase(strings::msg_params);
    (*smart_object)[strings::params][hmi_response::code] =
        hmi_apis::Common_Result::GENERIC_ERROR;
    (*smart_object)[strings::params][strings::error_msg] =
        std::string("Error sending message, mb component not registered");

    if (!app_manager_.GetRPCService().ManageHMICommand(
            smart_object, commands::Command::SOURCE_HMI, warning_info)) {
      SDL_LOG_ERROR("Received command didn't run successfully");
    }
  }
  return;
}

#ifdef TELEMETRY_MONITOR
void RPCHandlerImpl::SetTelemetryObserver(AMTelemetryObserver* observer) {
  metric_observer_ = observer;
}

#endif  // TELEMETRY_MONITOR

void RPCHandlerImpl::GetMessageVersion(
    ns_smart_device_link::ns_smart_objects::SmartObject& output,
    utils::SemanticVersion& message_version) {
  if (output.keyExists(
          ns_smart_device_link::ns_json_handler::strings::S_MSG_PARAMS) &&
      output[ns_smart_device_link::ns_json_handler::strings::S_MSG_PARAMS]
          .keyExists(strings::sync_msg_version)) {
    // SyncMsgVersion exists, check if it is valid.
    auto sync_msg_version =
        output[ns_smart_device_link::ns_json_handler::strings::S_MSG_PARAMS]
              [strings::sync_msg_version];
    uint16_t major = 0;
    uint16_t minor = 0;
    uint16_t patch = 0;
    if (sync_msg_version.keyExists(strings::major_version)) {
      major = sync_msg_version[strings::major_version].asUInt();
    }
    if (sync_msg_version.keyExists(strings::minor_version)) {
      minor = sync_msg_version[strings::minor_version].asUInt();
    }
    if (sync_msg_version.keyExists(strings::patch_version)) {
      patch = sync_msg_version[strings::patch_version].asUInt();
    }

    utils::SemanticVersion temp_version(major, minor, patch);
    if (temp_version.isValid()) {
      message_version = (temp_version >= utils::rpc_version_5)
                            ? temp_version
                            : utils::base_rpc_version;
    }
  }
}

bool RPCHandlerImpl::ConvertMessageToSO(
    const Message& message,
    ns_smart_device_link::ns_smart_objects::SmartObject& output,
    std::string& out_warning_info,
    const bool allow_unknown_parameters,
    const bool validate_params) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("\t\t\tMessage to convert: protocol "
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

      // Attach RPC version to SmartObject if it does not exist yet.
      auto app_ptr = app_manager_.application(message.connection_key());
      utils::SemanticVersion msg_version(0, 0, 0);
      if (app_ptr) {
        msg_version = app_ptr->msg_version();
      } else if (mobile_apis::FunctionID::RegisterAppInterfaceID ==
                 static_cast<mobile_apis::FunctionID::eType>(
                     output[strings::params][strings::function_id].asInt())) {
        GetMessageVersion(output, msg_version);
      }

      if (!conversion_result ||
          (validate_params &&
           !ValidateRpcSO(
               output, msg_version, report, allow_unknown_parameters))) {
        SDL_LOG_WARN("Failed to parse string to smart object with API version "
                     << msg_version.toString() << " : "
                     << message.json_message());

        std::shared_ptr<smart_objects::SmartObject> response(
            MessageHelper::CreateNegativeResponse(
                message.connection_key(),
                message.function_id(),
                message.correlation_id(),
                mobile_apis::Result::INVALID_DATA));

        (*response)[strings::msg_params][strings::info] =
            rpc::PrettyFormat(report);
        app_manager_.GetRPCService().ManageMobileCommand(
            response, commands::Command::SOURCE_SDL);

        return false;
      }

      SDL_LOG_DEBUG("Convertion result for sdl object is true function_id "
                    << output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());

      // Populate any warning info generated during the validation process
      out_warning_info = rpc::PrettyFormat(report);

      output[strings::params][strings::connection_key] =
          message.connection_key();
      output[strings::params][strings::protocol_version] =
          message.protocol_version();
      if (message.binary_data()) {
        if (message.payload_size() < message.data_size()) {
          SDL_LOG_ERROR("Incomplete binary"
                        << " binary size should be  " << message.data_size()
                        << " payload data size is " << message.payload_size());
          std::shared_ptr<smart_objects::SmartObject> response(
              MessageHelper::CreateNegativeResponse(
                  message.connection_key(),
                  message.function_id(),
                  message.correlation_id(),
                  mobile_apis::Result::INVALID_DATA));
          app_manager_.GetRPCService().ManageMobileCommand(
              response, commands::Command::SOURCE_SDL);
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
      SDL_LOG_DEBUG("Convertion result: "
                    << result << " function id "
                    << output[jhs::S_PARAMS][jhs::S_FUNCTION_ID].asInt());
      if (!hmi_so_factory().attachSchema(output, true)) {
        SDL_LOG_WARN("Failed to attach schema to object.");
        return false;
      }

      rpc::ValidationReport report("RPC");

      utils::SemanticVersion empty_version;
      if (validate_params &&
          smart_objects::errors::OK !=
              output.validate(
                  &report, empty_version, allow_unknown_parameters)) {
        SDL_LOG_ERROR("Incorrect parameter from HMI - "
                      << rpc::PrettyFormat(report));

        return HandleWrongMessageType(output, report);
      }
      break;
    }
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1: {
      if (message.function_id() == 0 || message.type() == kUnknownType) {
        SDL_LOG_ERROR("Message received: UNSUPPORTED_VERSION");

        int32_t conversation_result =
            formatters::CFormatterJsonSDLRPCv1::fromString<
                ns_smart_device_link_rpc::V1::FunctionID::eType,
                ns_smart_device_link_rpc::V1::messageType::eType>(
                message.json_message(), output);

        if (formatters::CFormatterJsonSDLRPCv1::kSuccess ==
            conversation_result) {
          smart_objects::SmartObject params = smart_objects::SmartObject(
              smart_objects::SmartType::SmartType_Map);

          output[strings::params][strings::message_type] =
              ns_smart_device_link_rpc::V1::messageType::response;
          output[strings::params][strings::connection_key] =
              message.connection_key();

          output[strings::msg_params] = smart_objects::SmartObject(
              smart_objects::SmartType::SmartType_Map);
          output[strings::msg_params][strings::success] = false;
          output[strings::msg_params][strings::result_code] =
              ns_smart_device_link_rpc::V1::Result::UNSUPPORTED_VERSION;

          smart_objects::SmartObjectSPtr msg_to_send =
              std::make_shared<smart_objects::SmartObject>(output);
          app_manager_.mobile_v4_protocol_so_factory().attachSchema(
              *msg_to_send, false);
          app_manager_.GetRPCService().SendMessageToMobile(msg_to_send);
          return false;
        }
      }
      break;
    }
    default:
      SDL_LOG_WARN("Application used unsupported protocol :"
                   << message.protocol_version() << ".");
      return false;
  }
  output[strings::params][strings::protection] = message.is_message_encrypted();

  SDL_LOG_DEBUG("Successfully parsed message into smart object");
  return true;
}

bool RPCHandlerImpl::HandleWrongMessageType(
    smart_objects::SmartObject& output, rpc::ValidationReport report) const {
  SDL_LOG_AUTO_TRACE();
  switch (output[strings::params][strings::message_type].asInt()) {
    case application_manager::MessageType::kNotification: {
      SDL_LOG_ERROR("Ignore wrong HMI notification");
      return false;
    }
    case application_manager::MessageType::kRequest: {
      SDL_LOG_ERROR("Received invalid data on HMI request");
      output.erase(strings::msg_params);
      output[strings::params].erase(hmi_response::message);
      output[strings::params][hmi_response::code] =
          hmi_apis::Common_Result::INVALID_DATA;
      output[strings::params][strings::message_type] =
          MessageType::kErrorResponse;
      output[strings::params][strings::error_msg] = rpc::PrettyFormat(report);
      return true;
    }
    case application_manager::MessageType::kResponse: {
      SDL_LOG_ERROR("Received invalid data on HMI response");
      break;
    }
    case application_manager::MessageType::kUnknownType: {
      SDL_LOG_ERROR("Received unknown type data on HMI");
      break;
    }
    default: {
      SDL_LOG_ERROR("Received error response on HMI");
      break;
    }
  }
  output.erase(strings::msg_params);
  output[strings::params].erase(hmi_response::message);
  output[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::GENERIC_ERROR;
  output[strings::msg_params][strings::info] =
      std::string("Invalid message received from vehicle");
  return true;
}

bool RPCHandlerImpl::ValidateRpcSO(smart_objects::SmartObject& message,
                                   utils::SemanticVersion& msg_version,
                                   rpc::ValidationReport& report_out,
                                   bool allow_unknown_parameters) {
  if (!mobile_so_factory().attachSchema(
          message, !allow_unknown_parameters, msg_version, &report_out) ||
      message.validate(&report_out, msg_version, allow_unknown_parameters) !=
          smart_objects::errors::OK) {
    SDL_LOG_WARN("Failed to parse string to smart object");
    return false;
  }
  return true;
}

std::shared_ptr<Message> RPCHandlerImpl::ConvertRawMsgToMessage(
    const protocol_handler::RawMessagePtr message) {
  SDL_LOG_AUTO_TRACE();
  DCHECK(message);
  std::shared_ptr<Message> outgoing_message;

  SDL_LOG_DEBUG("Service type." << message->service_type());
  if (message->service_type() != protocol_handler::kRpc &&
      message->service_type() != protocol_handler::kBulk) {
    // skip this message, not under handling of ApplicationManager
    SDL_LOG_TRACE("Skipping message; not the under AM handling.");
    return outgoing_message;
  }

  Message* convertion_result =
      MobileMessageHandler::HandleIncomingMessageProtocol(message);

  if (convertion_result) {
    outgoing_message = std::shared_ptr<Message>(convertion_result);
  } else {
    SDL_LOG_ERROR("Received invalid message");
  }

  return outgoing_message;
}

hmi_apis::HMI_API& RPCHandlerImpl::hmi_so_factory() {
  return hmi_so_factory_;
}

mobile_apis::MOBILE_API& RPCHandlerImpl::mobile_so_factory() {
  return mobile_so_factory_;
}

}  // namespace rpc_handler
}  // namespace application_manager
