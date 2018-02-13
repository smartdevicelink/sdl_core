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

#include "gtest/gtest.h"
#include "hmi/button_get_capabilities_response.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_hmi_capabilities.h"
namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace button_get_capabilities_response {

using application_manager::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::ButtonGetCapabilitiesResponse;
using ::testing::ReturnRef;
using ::testing::NiceMock;

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

typedef ::utils::SharedPtr<ButtonGetCapabilitiesResponse> ResponsePtr;

typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

class ButtonGetCapabilitiesResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateMsgParams() {
    capabilities_[strings::name] = hmi_apis::Common_ButtonName::OK;
    preset_bank_capabilities_ = true;

    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::msg_params][hmi_response::capabilities] = (capabilities_);
    (*msg)[strings::msg_params][hmi_response::preset_bank_capabilities] =
        (preset_bank_capabilities_);

    return msg;
  }

  MockHMICapabilities mock_hmi_capabilities_;
  SmartObject capabilities_;
  SmartObject preset_bank_capabilities_;
};

TEST_F(ButtonGetCapabilitiesResponseTest, Run_CodeSuccess_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  ResponsePtr command(CreateCommand<ButtonGetCapabilitiesResponse>(msg));

  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));
  EXPECT_CALL(mock_hmi_capabilities_, set_button_capabilities(capabilities_));
  EXPECT_CALL(mock_hmi_capabilities_,
              set_preset_bank_capabilities(preset_bank_capabilities_));

  command->Run();
}

TEST_F(ButtonGetCapabilitiesResponseTest, Run_CodeAborted_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;

  ResponsePtr command(CreateCommand<ButtonGetCapabilitiesResponse>(msg));

  EXPECT_CALL(app_mngr_, hmi_capabilities()).Times(0);
  EXPECT_CALL(mock_hmi_capabilities_, set_button_capabilities(capabilities_))
      .Times(0);
  EXPECT_CALL(mock_hmi_capabilities_,
              set_preset_bank_capabilities(preset_bank_capabilities_)).Times(0);

  command->Run();
}

}  // namespace button_get_capabilities_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
