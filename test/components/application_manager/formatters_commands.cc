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

#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "application_manager/basic_command_factory.h"
#include "utils/shared_ptr.h"

#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"
#include "JSONHandler/formatters/formatter_json_rpc.h"

#include "interfaces/v4_protocol_v2_0_revT.h"
#include "interfaces/HMI_API.h"

namespace test_command {

}
namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace mobile_api = NsSmartDeviceLinkRPC::V2;
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

TEST(add_command, general) {
  std::string incoming_string = "{\"parameters\":{\"cmdID\": 1, \"menuParams\" : {\"menuName\":\"Vasya\" }}}";
  smart_objects::CSmartObject incoming_message;

  formatters::CFormatterJsonSDLRPCv2::fromString<mobile_api::FunctionID::eType, mobile_api::messageType::eType>(
    incoming_string,
    incoming_message,
    mobile_api::FunctionID::AddCommandID,
    mobile_api::messageType::request);

  std::string str;
  formatters::CFormatterJsonSDLRPCv2::toString(incoming_message, str);
  std::cout << str << std::endl;

}

/*{
  "jsonrpc" : "2.0",
  "method" : "BasicCommunication.OnDeviceListUpdated",
  "params" :
  {
    "deviceList" : [ "XT910", "GT-I9300", "HTC Explorer A310e" ]
  }
}*/

TEST(json2_command, general1) {
  std::string incoming_string = "{\"jsonrpc\" : \"2.0\",\"method\" : \"BasicCommunication.OnDeviceListUpdated\", \"params\" : { \"deviceList\" : [ \"XT910\", \"GT-I9300\", \"HTC Explorer A310e\" ] }}";
  smart_objects::CSmartObject incoming_message;

  formatters::FormatterJsonRpc::FromString<hmi_apis::FunctionID::eType, hmi_apis::messageType::eType>(
    incoming_string,
    incoming_message);

  std::set<std::string> elements = incoming_message["msg_params"].enumerate();
  std::set<std::string>::const_iterator i = elements.begin();
  for (; i != elements.end(); ++i) {
    std::cout << (*i) << std::endl;
  }

  for (int i = 0; i < incoming_message["msg_params"]["deviceList"].length(); ++i) {
    std::cout << incoming_message["msg_params"]["deviceList"][i].asString() << std::endl;
  }

  std::string str;
  formatters::FormatterJsonRpc::ToString(incoming_message, str);
  std::cout << str << std::endl;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
