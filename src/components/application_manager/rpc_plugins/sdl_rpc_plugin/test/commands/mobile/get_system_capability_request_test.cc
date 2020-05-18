/*
 Copyright (c) 2019, Ford Motor Company
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

#include "mobile/get_system_capability_request.h"

#include "application_manager/commands/command_request_test.h"
#include "gtest/gtest.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace get_system_capability_request_test {

using sdl_rpc_plugin::commands::GetSystemCapabilityRequest;
using ::testing::_;
using ::testing::Return;
typedef std::shared_ptr<GetSystemCapabilityRequest>
    GetSystemCapabilityRequestPtr;

namespace strings = application_manager::strings;
namespace {
const uint32_t kConnectionKey = 1u;
}

class GetSystemCapabilityRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    (*message_)[strings::params][strings::connection_key] = kConnectionKey;

    command_ = CreateCommand<GetSystemCapabilityRequest>(message_);
    mock_app_ = CreateMockApp();

    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
  }

  GetSystemCapabilityRequestPtr command_;
  MessageSharedPtr message_;
  MockAppPtr mock_app_;
};

TEST_F(
    GetSystemCapabilityRequestTest,
    Run_GetSystemDisplayCapabilities_SendMessageToMobileWithSUCCESSResultCode) {
  (*message_)[strings::msg_params][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::DISPLAYS;

  smart_objects::SmartObjectSPtr system_display_capabilities(
      std::make_shared<smart_objects::SmartObject>());

  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(system_display_capabilities));

  ON_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .WillByDefault(Return(system_display_capabilities));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS),
                          Command::CommandSource::SOURCE_SDL));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(
    GetSystemCapabilityRequestTest,
    Run_GetSystemDisplayCapabilities_CapabilitiesNotExistSendMessageToMobileWithDATA_NOT_AVAILABLEResultCode) {
  (*message_)[strings::msg_params][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::DISPLAYS;

  EXPECT_CALL(*mock_app_, display_capabilities()).WillOnce(Return(nullptr));

  ON_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .WillByDefault(Return(nullptr));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DATA_NOT_AVAILABLE),
                  Command::CommandSource::SOURCE_SDL));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

}  // namespace get_system_capability_request_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
