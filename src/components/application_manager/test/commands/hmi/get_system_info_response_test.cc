/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "commands/commands_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/commands/hmi/response_from_hmi.h"
#include "application_manager/commands/hmi/get_system_info_response.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace get_system_info_response {

using ::testing::Return;
using ::utils::SharedPtr;
using ::testing::NiceMock;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
namespace hmi_response = am::hmi_response;
using am::commands::ResponseFromHMI;
using am::commands::GetSystemInfoResponse;
using am::commands::CommandImpl;
using am::commands::SystemInfo;

typedef SharedPtr<ResponseFromHMI> ResponseFromHMIPtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 2u;
const std::string ccpu_version("4.1.3.B_EB355B");
const std::string wers_country_code("WAEGB");
const uint32_t lang_code = 0u;
const std::string kLanguage = "";
}  // namespace

class GetSystemInfoResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateCommandMsg() {
    MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
    (*command_msg)[strings::msg_params][strings::number] = "123";
    (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*command_msg)[strings::msg_params]["ccpu_version"] = ccpu_version;
    (*command_msg)[strings::msg_params]["wersCountryCode"] = wers_country_code;
    (*command_msg)[strings::msg_params]["language"] = lang_code;

    return command_msg;
  }

  void SetUp() OVERRIDE {
    message_helper_mock_ =
        application_manager::MockMessageHelper::message_helper_mock();
  }

  am::MockMessageHelper* message_helper_mock_;
  MockHMICapabilities mock_hmi_capabilities_;
  SmartObject capabilities_;
};

TEST_F(GetSystemInfoResponseTest, GetSystemInfo_SUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*command_msg)[strings::msg_params][hmi_response::capabilities] =
      (capabilities_);

  ResponseFromHMIPtr command(CreateCommand<GetSystemInfoResponse>(command_msg));
  policy_test::MockPolicyHandlerInterface policy_handler;

  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));

  std::string language;
  EXPECT_CALL(*message_helper_mock_,
              CommonLanguageToString(
                  static_cast<hmi_apis::Common_Language::eType>(lang_code)))
      .WillOnce(Return(language));
  EXPECT_EQ(kLanguage, language);

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler));
  EXPECT_CALL(policy_handler,
              OnGetSystemInfo(ccpu_version, wers_country_code, kLanguage));

  command->Run();
}

TEST_F(GetSystemInfoResponseTest, GetSystemInfo_UNSUCCESS) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::WRONG_LANGUAGE;
  (*command_msg)[strings::msg_params][hmi_response::capabilities] =
      (capabilities_);

  ResponseFromHMIPtr command(CreateCommand<GetSystemInfoResponse>(command_msg));
  policy_test::MockPolicyHandlerInterface policy_handler;

  EXPECT_CALL(app_mngr_, hmi_capabilities()).Times(0);

  EXPECT_CALL(*message_helper_mock_,
              CommonLanguageToString(
                  static_cast<hmi_apis::Common_Language::eType>(lang_code)))
      .Times(0);

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler));
  EXPECT_CALL(policy_handler, OnGetSystemInfo("", "", ""));

  command->Run();
}

}  // namespace get_system_info_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
