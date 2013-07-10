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

#include "application_manager/message_conversion.h"
#include "application_manager/smart_object_keys.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
//#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "formatters/formatter_json_rpc.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

Message SmartObjectToMessage
(NsSmartDeviceLink::NsSmartObjects::CSmartObject* smart_object)  {
  Message message;

  /*message.set_connection_key(
      (*smart_object)[strings::params][strings::connection_key]);
  message.set_protocol_version(static_cast<ProtocolVersion>(
      ((*smart_object)[strings::params][strings::protocol_version].asInt())));

  if ((*smart_object)[strings::msg_params].keyExists(strings::binary_data))  {
    BinaryData* binaryData = new BinaryData(
        (*smart_object)[strings::msg_params][strings::binary_data].asBinary());

    message.set_binary_data(binaryData);
  }

  std::string output_json_string;
  bool result = false;

  namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

  if (ProtocolVersion::kV2 ==
      (*smart_object)[strings::params][strings::protocol_version].asInt()) {
    result = formatters::CFormatterJsonSDLRPCv2::toString(*smart_object,
                                                         output_json_string);

    message.set_correlation_id(
        (*smart_object)[strings::params][strings::correlation_id]);
    message.set_function_id(
        (*smart_object)[strings::params][strings::function_id]);
    message.set_message_type(static_cast<MessageType>(
        (*smart_object)[strings::params][strings::message_type].asInt()));
  } else if (ProtocolVersion::kV1 ==
      (*smart_object)[strings::params][strings::protocol_version].asInt()) {
    result = formatters::CFormatterJsonALRPCv1::toString(*smart_object,
                                                         output_json_string);
  }

  if (result) {
    message.set_json_message(output_json_string);
  }*/

  return message;
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject MessageToSmartObject
(const Message& message)  {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject smart_object;

  if (ProtocolVersion::kV2 == message.protocol_version())  {
    /*formatters::CFormatterJsonSDLRPCv2::fromString<int, MessageType>(
      message.json_message(),
      smart_object,
      message.function_id(),
      message.type(),
      message.correlation_id());*/
  } else if (ProtocolVersion::kHMI == message.protocol_version())  {
    /*formatters::FormatterJsonRpc::FromString < hmi_apis::FunctionID::eType,
               hmi_apis::messageType::eType > (message.json_message(),
                   smart_object);*/
  }

  smart_object[strings::params][strings::connection_key] =
    message.connection_key();

  if (message.has_binary_data())  {
    smart_object[strings::msg_params][strings::binary_data] =
      message.binary_data();
  }

  return smart_object;
}
}  // namespace application_manager

