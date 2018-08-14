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

namespace application_manager {
namespace rpc_handler {

CREATE_LOGGERPTR_LOCAL(logger_, "RPCHandlerImpl")
namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace jhs = NsSmartDeviceLink::NsJSONHandler::strings;

RPCHandlerImpl::RPCHandlerImpl(ApplicationManager& app_manager)
    : app_manager_(app_manager)
    , messages_from_mobile_("AM FromMobile", this)
    , messages_from_hmi_("AM FromHMI", this)
    , hmi_so_factory_(hmi_apis::HMI_API())
    , mobile_so_factory_(mobile_apis::MOBILE_API())
#ifdef TELEMETRY_MONITOR
    , metric_observer_(NULL)
#endif  // TELEMETRY_MONITOR
{
}

RPCHandlerImpl::~RPCHandlerImpl() {}

void RPCHandlerImpl::ProcessMessageFromMobile(
    const std::shared_ptr<Message> message) {
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef TELEMETRY_MONITOR
  AMTelemetryObserver::MessageMetricSharedPtr metric(
      new AMTelemetryObserver::MessageMetric());
  metric->begin = date_time::DateTime::getCurrentTime();
#endif  // TELEMETRY_MONITOR
  smart_objects::SmartObjectSPtr so_from_mobile =
      std::make_shared<smart_objects::SmartObject>();

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

  if (!app_manager_.GetRPCService().ManageMobileCommand(
          so_from_mobile, commands::Command::SOURCE_MOBILE)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
#ifdef TELEMETRY_MONITOR
  metric->end = date_time::DateTime::getCurrentTime();
  if (metric_observer_) {
    metric_observer_->OnMessage(metric);
  }
#endif  // TELEMETRY_MONITOR
}

void RPCHandlerImpl::ProcessMessageFromHMI(
    const std::shared_ptr<Message> message) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr smart_object =
      std::make_shared<smart_objects::SmartObject>();

  if (!smart_object) {
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

  if (!ConvertMessageToSO(*message, *smart_object)) {
    if (application_manager::MessageType::kResponse ==
        (*smart_object)[strings::params][strings::message_type].asInt()) {
      (*smart_object).erase(strings::msg_params);
      (*smart_object)[strings::params][hmi_response::code] =
          hmi_apis::Common_Result::GENERIC_ERROR;
      (*smart_object)[strings::msg_params][strings::info] =
          std::string("Invalid message received from vehicle");
    } else {
      LOG4CXX_ERROR(logger_, "Cannot create smart object from message");
      return;
    }
  }

  LOG4CXX_DEBUG(logger_, "Converted message, trying to create hmi command");
  if (!app_manager_.GetRPCService().ManageHMICommand(smart_object)) {
    LOG4CXX_ERROR(logger_, "Received command didn't run successfully");
  }
}
void RPCHandlerImpl::Handle(const impl::MessageFromMobile message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }
  if (app_manager_.is_stopping()) {
    LOG4CXX_INFO(logger_, "Application manager is stopping");
    return;
  }

  ProcessMessageFromMobile(message);
}

void RPCHandlerImpl::Handle(const impl::MessageFromHmi message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    return;
  }
  ProcessMessageFromHMI(message);
}

void RPCHandlerImpl::OnMessageReceived(
    const protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  std::shared_ptr<Message> outgoing_message = ConvertRawMsgToMessage(message);

  if (outgoing_message) {
    LOG4CXX_DEBUG(logger_, "Posting new Message");
    messages_from_mobile_.PostMessage(
        impl::MessageFromMobile(outgoing_message));
  }
}

void RPCHandlerImpl::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
}

void RPCHandlerImpl::OnMessageReceived(
    hmi_message_handler::MessageSharedPointer message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Null-pointer message received.");
    NOTREACHED();
    return;
  }

  messages_from_hmi_.PostMessage(impl::MessageFromHmi(message));
}

void RPCHandlerImpl::OnErrorSending(
    hmi_message_handler::MessageSharedPointer message) {
  return;
}

#ifdef TELEMETRY_MONITOR
void RPCHandlerImpl::SetTelemetryObserver(AMTelemetryObserver* observer) {
  metric_observer_ = observer;
}

#endif  // TELEMETRY_MONITOR

bool RPCHandlerImpl::ConvertMessageToSO(
    const Message& message,
    NsSmartDeviceLink::NsSmartObjects::SmartObject& output) {
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

      // Attach RPC version to SmartObject if it does not exist yet.
      auto app_ptr = app_manager_.application(message.connection_key());
      utils::SemanticVersion msg_version(0, 0, 0);
      if (app_ptr &&
          (output[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS]
               .keyExists(NsSmartDeviceLink::NsJSONHandler::strings::
                              S_RPC_MSG_VERSION) == false)) {
        msg_version = app_ptr->msg_version();
      } else if (mobile_apis::FunctionID::RegisterAppInterfaceID ==
                 static_cast<mobile_apis::FunctionID::eType>(
                     output[strings::params][strings::function_id].asInt())) {
        if (output.keyExists(
                NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS) &&
                output[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS]
                    .keyExists(strings::sync_msg_version)) {
          // SyncMsgVersion exists, check if it is valid.
          std::string str_msg_version =
              output[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS]
                    [strings::sync_msg_version].asString();
          utils::SemanticVersion temp_version(str_msg_version);
          if (temp_version.isValid()) {
            msg_version = temp_version;
          }
        }
      }

      if (!conversion_result ||
          !mobile_so_factory().attachSchema(output, true, msg_version) ||
          ((output.validate(&report, msg_version) !=
            smart_objects::Errors::OK))) {
        LOG4CXX_WARN(logger_,
                     "Failed to parse string to smart object :"
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
              std::make_shared<smart_objects::SmartObject>(output);
          v1_shema.attachSchema(*msg_to_send, false);
          app_manager_.GetRPCService().SendMessageToMobile(msg_to_send);
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

std::shared_ptr<Message> RPCHandlerImpl::ConvertRawMsgToMessage(
    const protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(message);
  std::shared_ptr<Message> outgoing_message;

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
    outgoing_message = std::shared_ptr<Message>(convertion_result);
  } else {
    LOG4CXX_ERROR(logger_, "Received invalid message");
  }
  return outgoing_message;
}

hmi_apis::HMI_API& RPCHandlerImpl::hmi_so_factory() {
  return hmi_so_factory_;
}

mobile_apis::MOBILE_API& RPCHandlerImpl::mobile_so_factory() {
  return mobile_so_factory_;
}
}
}
