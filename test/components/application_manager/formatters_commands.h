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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_FORMATTERS_COMMANDS_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_FORMATTERS_COMMANDS_H_

#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "application_manager/mobile_command_factory.h"
#include "utils/shared_ptr.h"

#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/formatter_json_rpc.h"

#include "interfaces/MOBILE_API.h"
#include "interfaces/MOBILE_API_schema.h"
#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"

namespace test_command {

}
namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace sos = NsSmartDeviceLink::NsJSONHandler::strings;

TEST(add_command, general) {
  std::string incoming_string =
    "{\
    \"menuID\" : 1000,\
    \"menuName\" : \"SubmenuName\"\
}";

  smart_objects::SmartObject incoming_message;

  formatters::CFormatterJsonSDLRPCv2::fromString<mobile_apis::FunctionID::eType, mobile_apis::messageType::eType>(
    incoming_string,
    incoming_message,
    mobile_apis::FunctionID::AddSubMenuID,
    mobile_apis::messageType::request,
    258);

  mobile_apis::MOBILE_API factory;
/*  std::cout << "Attach schema: " << factory.attachSchema(incoming_message) << std::endl;

  std::cout << "Is message valid? " << incoming_message.isValid() << std::endl;

  std::cout << "Internal message parameters: " << std::endl;
  std::set<std::string> internal_params = incoming_message["params"].enumerate();
  for (std::set<std::string>::const_iterator i = internal_params.begin(); i != internal_params.end(); ++i) {
    std::cout << (*i) << " : " << incoming_message["params"][*i].asString() << std::endl;
  }

  std::cout << "API parameters:" << std::endl;

  std::set<std::string> elements = incoming_message["msg_params"].enumerate();
  for (std::set<std::string>::const_iterator i = elements.begin(); i != elements.end(); ++i) {
    std::cout << (*i) << std::endl;
  }

  std::string str;
  formatters::CFormatterJsonSDLRPCv2::toString(incoming_message, str);
  std::cout << str << std::endl;
*/
}

TEST(json2_command, notification) {
  std::string incoming_string = "{\"jsonrpc\" : \"2.0\",\"method\" : \"BasicCommunication.OnDeviceListUpdated\", \"params\" : { \"deviceList\" : [ \"XT910\", \"GT-I9300\", \"HTC Explorer A310e\" ] }}";
  smart_objects::SmartObject incoming_message;

  formatters::FormatterJsonRpc::FromString<hmi_apis::FunctionID::eType, hmi_apis::messageType::eType>(
    incoming_string,
    incoming_message);

  hmi_apis::HMI_API factory;
/*
  std::cout << "Attach schema: " << factory.attachSchema(incoming_message) << std::endl;

  std::cout << "Is message valid? " << incoming_message.isValid() << std::endl;

  std::cout << "Internal message parameters: " << std::endl;
  std::set<std::string> internal_params = incoming_message["params"].enumerate();
  for (std::set<std::string>::const_iterator i = internal_params.begin(); i != internal_params.end(); ++i) {
    std::cout << (*i) << " : " << incoming_message["params"][*i].asString() << std::endl;
  }

  std::cout << "API parameters:" << std::endl;

  std::set<std::string> elements = incoming_message["msg_params"].enumerate();
  for (std::set<std::string>::const_iterator i = elements.begin(); i != elements.end(); ++i) {
    std::cout << (*i) << std::endl;
  }

  for (int i = 0; i < incoming_message["msg_params"]["deviceList"].length(); ++i) {
    std::cout << incoming_message["msg_params"]["deviceList"][i].asString() << std::endl;
  }

  std::cout << "Formatted message string: " << std::endl;

  std::string str;
  formatters::FormatterJsonRpc::ToString(incoming_message, str);
  std::cout << str << std::endl;
*/
}

TEST(json2_command, request_no_params) {
  std::string incoming_string = "{\"id\":5005,\"jsonrpc\":\"2.0\",\"method\":\"BasicCommunication.GeApplictionSet\"}";

  smart_objects::SmartObject incoming_message;

  formatters::FormatterJsonRpc::FromString<hmi_apis::FunctionID::eType, hmi_apis::messageType::eType>(
    incoming_string,
    incoming_message);

  hmi_apis::HMI_API factory;
/*
  std::cout << "Attach schema: " << factory.attachSchema(incoming_message) << std::endl;

  std::cout << "Is message valid? " << incoming_message.isValid() << std::endl;

  std::cout << "Internal message parameters: " << std::endl;
  std::set<std::string> internal_params = incoming_message["params"].enumerate();
  for (std::set<std::string>::const_iterator i = internal_params.begin(); i != internal_params.end(); ++i) {
    std::cout << (*i) << " : " << incoming_message["params"][*i].asString() << std::endl;
  }

  std::cout << "API parameters:" << std::endl;

  std::set<std::string> elements = incoming_message["msg_params"].enumerate();
  for (std::set<std::string>::const_iterator i = elements.begin(); i != elements.end(); ++i) {
    std::cout << (*i) << std::endl;
  }

  for (int i = 0; i < incoming_message["msg_params"]["deviceList"].length(); ++i) {
    std::cout << incoming_message["msg_params"]["deviceList"][i].asString() << std::endl;
  }

  std::cout << "Formatted message string: " << std::endl;

  std::string str;
  formatters::FormatterJsonRpc::ToString(incoming_message, str);
  std::cout << str << std::endl;
*/
}

TEST(json2_command, response_params) {
  std::string incoming_string = "{\"id\" : 19,\"jsonrpc\" : \"2.0\",\"result\" : {\"capabilities\" : [{\
          \"longPressAvailable\" : true,\
          \"name\" : \"PRESET_0\",\
          \"shortPressAvailable\" : true,\
          \"upDownAvailable\" : true\
        },\
        {\
          \"longPressAvailable\" : true,\
          \"name\" : \"PRESET_1\",\
          \"shortPressAvailable\" : true,\
          \"upDownAvailable\" : true\
        }\
      ],\
      \"presetBankCapabilities\" : \
      {\
        \"onScreenPresetsAvailable\" : true\
      },\
      \"code\" : 0,\
      \"method\" : \"Buttons.GetCapabilities\"\
    }\
  }";

  smart_objects::SmartObject incoming_message;

  formatters::FormatterJsonRpc::FromString<hmi_apis::FunctionID::eType, hmi_apis::messageType::eType>(
    incoming_string,
    incoming_message);

  hmi_apis::HMI_API factory;
/*
  std::cout << "Attach schema: " << factory.attachSchema(incoming_message) << std::endl;

  std::cout << "Is message valid? " << incoming_message.isValid() << std::endl;

  std::cout << "Internal message parameters: " << std::endl;
  std::set<std::string> internal_params = incoming_message["params"].enumerate();
  for (std::set<std::string>::const_iterator i = internal_params.begin(); i != internal_params.end(); ++i) {
    std::cout << (*i) << " : " << incoming_message["params"][*i].asString() << std::endl;
  }

  std::cout << "API parameters:" << std::endl;

  std::set<std::string> elements = incoming_message["msg_params"].enumerate();
  for (std::set<std::string>::const_iterator i = elements.begin(); i != elements.end(); ++i) {
    std::cout << (*i) << std::endl;
    std::set<std::string> sub_params = incoming_message["msg_params"][*i].enumerate();
    for (std::set<std::string>::const_iterator j = sub_params.begin();
         sub_params.end() != j;
         ++j) {
      std::cout << (*j) << " : " << incoming_message["msg_params"][*i][*j].asString() << std::endl;
    }
  }

  for (int i = 0; i < incoming_message["msg_params"]["deviceList"].length(); ++i) {
    std::cout << incoming_message["msg_params"]["deviceList"][i].asString() << std::endl;
  }

  std::cout << "Formatted message string: " << std::endl;

  std::string str;
  formatters::FormatterJsonRpc::ToString(incoming_message, str);
  std::cout << str << std::endl;
*/
}

TEST(json2_command, response_error_params) {
  std::string incoming_string = "{\
  \"id\" : 35,\
  \"jsonrpc\" : \"2.0\",\
  \"error\" : \
  {\
    \"code\" : 12,\
    \"message\" : \"File with this name is not available\",\
    \"data\" : {\
      \"method\" : \"Buttons.GetCapabilities\"\
    }\
  }\
}";

  smart_objects::SmartObject incoming_message;

  formatters::FormatterJsonRpc::
  FromString<hmi_apis::FunctionID::eType, hmi_apis::messageType::eType>(
    incoming_string,
    incoming_message);

  hmi_apis::HMI_API factory;
/*
  std::cout << "Attach schema: " << factory.attachSchema(incoming_message) << std::endl;

  std::cout << "Is message valid? " << incoming_message.isValid() << std::endl;

  std::cout << "Internal message parameters: " << std::endl;
  std::set<std::string> internal_params = incoming_message["params"].enumerate();
  for (std::set<std::string>::const_iterator i = internal_params.begin(); i != internal_params.end(); ++i) {
    std::cout << (*i) << " : " << incoming_message["params"][*i].asString() << std::endl;
  }

  std::cout << "API parameters:" << std::endl;

  std::set<std::string> elements = incoming_message["msg_params"].enumerate();
  for (std::set<std::string>::const_iterator i = elements.begin(); i != elements.end(); ++i) {
    std::cout << (*i) << std::endl;
  }

  std::cout << "Formatted message string: " << std::endl;

  std::string str;
  formatters::FormatterJsonRpc::ToString(incoming_message, str);
  std::cout << str << std::endl;
*/
}

TEST(json2_command, create_object) {
  hmi_apis::HMI_API factory;
  smart_objects::SmartObject is_vr_ready =
    factory.CreateSmartObject(hmi_apis::FunctionID::VR_IsReady,
                              hmi_apis::messageType::request);

  ASSERT_EQ(smart_objects::SmartType_Map, is_vr_ready.getType());
  /*std::cout << "FunctionID " << is_vr_ready[sos::S_PARAMS][sos::S_FUNCTION_ID].asInt()
            << "; message type " << is_vr_ready[sos::S_PARAMS][sos::S_MESSAGE_TYPE].asInt()
            << "; protocol version " << is_vr_ready[sos::S_PARAMS][sos::S_PROTOCOL_VERSION].asInt()
            << "; protocol type " << is_vr_ready[sos::S_PARAMS][sos::S_PROTOCOL_TYPE].asInt()
            << "; correlation id " << is_vr_ready[sos::S_PARAMS][sos::S_CORRELATION_ID].asInt() << std::endl;
	    */

  is_vr_ready[sos::S_PARAMS][sos::S_FUNCTION_ID] =
    hmi_apis::FunctionID::VR_IsReady;
  is_vr_ready[sos::S_PARAMS][sos::S_MESSAGE_TYPE] =
    hmi_apis::messageType::request;
  is_vr_ready[sos::S_PARAMS][sos::S_PROTOCOL_VERSION] = 2;
  is_vr_ready[sos::S_PARAMS][sos::S_PROTOCOL_TYPE] = 1;
  is_vr_ready[sos::S_PARAMS][sos::S_CORRELATION_ID] = 4444;
  is_vr_ready[sos::S_MSG_PARAMS] = smart_objects::SmartObject(smart_objects::SmartType_Map);
/*
  std::cout << "FunctionID " << is_vr_ready[sos::S_PARAMS][sos::S_FUNCTION_ID].asInt()
            << "; message type " << is_vr_ready[sos::S_PARAMS][sos::S_MESSAGE_TYPE].asInt()
            << "; protocol version " << is_vr_ready[sos::S_PARAMS][sos::S_PROTOCOL_VERSION].asInt()
            << "; protocol type " << is_vr_ready[sos::S_PARAMS][sos::S_PROTOCOL_TYPE].asInt()
            << "; correlation id " << is_vr_ready[sos::S_PARAMS][sos::S_CORRELATION_ID].asInt() << std::endl;

  ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, is_vr_ready.validate());

  std::string str;
  formatters::FormatterJsonRpc::ToString(is_vr_ready, str);
  std::cout << str << std::endl;
*/
}

TEST(json2_command, without_factory_create) {
  smart_objects::SmartObject so_to_send;
  so_to_send[sos::S_PARAMS][sos::S_FUNCTION_ID] =
    hmi_apis::FunctionID::VR_IsReady;
  so_to_send[sos::S_PARAMS][sos::S_MESSAGE_TYPE] =
    hmi_apis::messageType::request;
  so_to_send[sos::S_PARAMS][sos::S_PROTOCOL_VERSION] = 2;
  so_to_send[sos::S_PARAMS][sos::S_PROTOCOL_TYPE] = 1;
  so_to_send[sos::S_PARAMS][sos::S_CORRELATION_ID] = 4444;
  so_to_send[sos::S_MSG_PARAMS] = smart_objects::SmartObject(smart_objects::SmartType_Map);

  /*std::cout << "FunctionID " << so_to_send[sos::S_PARAMS][sos::S_FUNCTION_ID].asInt()
            << "; message type " << so_to_send[sos::S_PARAMS][sos::S_MESSAGE_TYPE].asInt()
            << "; protocol version " << so_to_send[sos::S_PARAMS][sos::S_PROTOCOL_VERSION].asInt()
            << "; protocol type " << so_to_send[sos::S_PARAMS][sos::S_PROTOCOL_TYPE].asInt()
            << "; correlation id " << so_to_send[sos::S_PARAMS][sos::S_CORRELATION_ID].asInt() << std::endl;
	    */

  hmi_apis::HMI_API factory;
  factory.attachSchema(so_to_send);
  ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, so_to_send.validate());
/*
  std::string str;
  formatters::FormatterJsonRpc::ToString(so_to_send, str);
  std::cout << str << std::endl;
*/
}

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_FORMATTERS_COMMANDS_H_
