/*
 * Copyright (c) 2019, Ford Motor Company
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

#include "hmi/sdl_get_policy_configuration_data_request.h"
#include "application_manager/commands/command_request_test.h"
#include "gtest/gtest.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace sdl_get_policy_configuration_data_request {

using testing::Return;

using sdl_rpc_plugin::commands::SDLGetPolicyConfigurationDataRequest;
using test::components::policy_test::MockPolicyHandlerInterface;

namespace strings = ::app_mngr::strings;
namespace hmi_response = ::app_mngr::hmi_response;

class SDLGetPolicyConfigurationDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

MATCHER_P(GetPolicyConfigurationDataFirstElementMatches,
          string_first_element_value,
          "") {
  auto message = static_cast<smart_objects::SmartObject>(*arg);
  if (!message.keyExists(strings::msg_params) ||
      !message[strings::msg_params].keyExists(strings::value)) {
    return false;
  }

  if (message[strings::msg_params][strings::value].getType() !=
          smart_objects::SmartType::SmartType_Array ||
      message[strings::msg_params][strings::value].empty()) {
    return false;
  }

  Json::Reader reader;
  Json::Value msg_json_value(Json::ValueType::arrayValue);

  auto msg_value_first = message[strings::msg_params][strings::value][0];
  return msg_value_first.asString() == string_first_element_value;
}

MATCHER_P(HMIResultCodeIs, result_code, "") {
  return result_code ==
         static_cast<hmi_apis::Common_Result::eType>(
             (*arg)[strings::params][hmi_response::code].asInt());
}

TEST_F(SDLGetPolicyConfigurationDataRequestTest, Run_Fail_DataNotAvailable) {
  std::string kInvalidKey = "invalid_key";

  PolicyTable pt;
  ON_CALL(mock_policy_handler_, GetPolicyTableData())
      .WillByDefault(Return(pt.ToJsonValue()));

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][strings::policy_type] = kInvalidKey;
  (*msg)[strings::msg_params][strings::property] = kInvalidKey;

  std::shared_ptr<SDLGetPolicyConfigurationDataRequest> command(
      CreateCommand<SDLGetPolicyConfigurationDataRequest>(msg));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(hmi_apis::Common_Result::DATA_NOT_AVAILABLE),
                  Command::SOURCE_SDL_TO_HMI));

  command->Run();
}

void clear_new_line_symbol(std::string& str_to_clear) {
  str_to_clear.erase(
      std::remove_if(str_to_clear.begin(),
                     str_to_clear.end(),
                     [](char character) { return '\n' == character; }),
      str_to_clear.end());
}

TEST_F(SDLGetPolicyConfigurationDataRequestTest, Run_Success) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][strings::policy_type] = "module_config";
  (*msg)[strings::msg_params][strings::property] = "endpoints";

  std::shared_ptr<SDLGetPolicyConfigurationDataRequest> command(
      CreateCommand<SDLGetPolicyConfigurationDataRequest>(msg));

  policy_table::ModuleConfig module_config_with_endpoints;
  policy_table::URLList endpoint_url_list;
  policy_table::URL urls;
  urls.push_back("https://url1.com");
  urls.push_back("https://url2.com");
  endpoint_url_list["default"] = urls;
  module_config_with_endpoints.endpoints["0x9"] = endpoint_url_list;

  PolicyTable pt;
  pt.mark_initialized();
  pt.module_config.mark_initialized();
  pt.module_config = module_config_with_endpoints;

  ON_CALL(mock_policy_handler_, GetPolicyTableData())
      .WillByDefault(Return(pt.ToJsonValue()));

  auto json_val = module_config_with_endpoints.endpoints.ToJsonValue();
  Json::FastWriter writer;
  std::string expected_string = writer.write(json_val);
  clear_new_line_symbol(expected_string);

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(GetPolicyConfigurationDataFirstElementMatches(
                                   expected_string),
                               Command::SOURCE_SDL_TO_HMI));
  command->Run();
}

TEST_F(SDLGetPolicyConfigurationDataRequestTest,
       Run_RetriveStringValueFromPolicy) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][strings::policy_type] =
      "consumer_friendly_messages";
  (*msg)[strings::msg_params][strings::property] = "version";

  auto command = CreateCommand<SDLGetPolicyConfigurationDataRequest>(msg);

  const std::string version_test_value("version string");
  PolicyTable pt;
  pt.mark_initialized();
  pt.consumer_friendly_messages->mark_initialized();
  pt.consumer_friendly_messages->version =
      rpc::String<1, 100>(version_test_value);

  ON_CALL(mock_policy_handler_, GetPolicyTableData())
      .WillByDefault(Return(pt.ToJsonValue()));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(GetPolicyConfigurationDataFirstElementMatches(
                                   version_test_value),
                               Command::SOURCE_SDL_TO_HMI));

  command->Run();
}

}  // namespace sdl_get_policy_configuration_data_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
