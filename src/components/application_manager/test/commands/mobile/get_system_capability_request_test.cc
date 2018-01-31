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

#include "application_manager/commands/mobile/get_system_capability_request.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/test/include/application_manager/mock_hmi_capabilities.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace get_vehicle_data_tequest {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace amt = application_manager_test;
using am::commands::GetSystemCapabilityRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
namespace mobile_result = mobile_apis::Result;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class GetSystemCapabilityRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateMessageParams() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    return msg;
  }
};

TEST_F(GetSystemCapabilityRequestTest, Run_NotExistApp_AppNotRegistered) {
  MockAppPtr mock_app;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MessageSharedPtr msg = CreateMessageParams();

  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::APPLICATION_NOT_REGISTERED),
          _)).WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest, Run_EmptyMessage_ExpectInvalidData) {
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  MessageSharedPtr msg = CreateMessageParams();
  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _))
      .WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest, Run_NAVIGATION_SUCCESS) {
  MockAppPtr mock_app = CreateMockApp();
  SmartObject sm(smart_objects::SmartType_Map);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MessageSharedPtr msg = CreateMessageParams();
  amt::MockHMICapabilities mock_hmi_cap;
  (*msg)[am::strings::msg_params][am::strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::NAVIGATION;
  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));
  EXPECT_CALL(app_mngr_, hmi_capabilities()).WillOnce(ReturnRef(mock_hmi_cap));
  EXPECT_CALL(mock_hmi_cap, navigation_capability())
      .WillRepeatedly(Return(&sm));
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                                  _)).WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest, Run_SystemCapabilityPhoneCall_SUCCESS) {
  MockAppPtr mock_app = CreateMockApp();
  SmartObject sm(smart_objects::SmartType_Map);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MessageSharedPtr msg = CreateMessageParams();
  amt::MockHMICapabilities mock_hmi_cap;
  (*msg)[am::strings::msg_params][am::strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::PHONE_CALL;
  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));
  EXPECT_CALL(app_mngr_, hmi_capabilities()).WillOnce(ReturnRef(mock_hmi_cap));
  EXPECT_CALL(mock_hmi_cap, phone_capability()).WillRepeatedly(Return(&sm));
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                                  _)).WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest,
       Run_SystemCapabilityRemoteControl_SUCCESS) {
  MockAppPtr mock_app = CreateMockApp();
  SmartObject sm(smart_objects::SmartType_Map);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MessageSharedPtr msg = CreateMessageParams();
  amt::MockHMICapabilities mock_hmi_cap;
  (*msg)[am::strings::msg_params][am::strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::REMOTE_CONTROL;
  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));
  EXPECT_CALL(app_mngr_, hmi_capabilities()).WillOnce(ReturnRef(mock_hmi_cap));
  EXPECT_CALL(mock_hmi_cap, rc_capability()).WillRepeatedly(Return(&sm));
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                                  _)).WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest,
       Run_SystemCapabilityVideoStreaming_SUCCESS) {
  MockAppPtr mock_app = CreateMockApp();
  SmartObject sm(smart_objects::SmartType_Map);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MessageSharedPtr msg = CreateMessageParams();
  amt::MockHMICapabilities mock_hmi_cap;
  (*msg)[am::strings::msg_params][am::strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::VIDEO_STREAMING;
  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));
  EXPECT_CALL(app_mngr_, hmi_capabilities()).WillOnce(ReturnRef(mock_hmi_cap));
  EXPECT_CALL(mock_hmi_cap, video_streaming_capability())
      .WillRepeatedly(Return(&sm));
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                                  _)).WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest, Run_DefaultCase_UnsupportedResource) {
  MockAppPtr mock_app = CreateMockApp();
  SmartObject sm(smart_objects::SmartType_Map);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MessageSharedPtr msg = CreateMessageParams();
  amt::MockHMICapabilities mock_hmi_cap;
  (*msg)[am::strings::msg_params][am::strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::INVALID_ENUM;
  SharedPtr<GetSystemCapabilityRequest> command(
      CreateCommand<GetSystemCapabilityRequest>(msg));
  EXPECT_CALL(app_mngr_, hmi_capabilities()).WillOnce(ReturnRef(mock_hmi_cap));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_result::UNSUPPORTED_RESOURCE), _))
      .WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetSystemCapabilityRequestTest,
       Run_HmiNotReturnSmartObject_DataNotAvailable) {
  MockAppPtr mock_app = CreateMockApp();
  MessageSharedPtr msg = CreateMessageParams();
  amt::MockHMICapabilities mock_hmi_cap;
  uint32_t size = sizeof(mobile_apis::SystemCapabilityType::eType);
  for (uint32_t type = 0; type < size; type++) {
    (*msg)[am::strings::msg_params][am::strings::system_capability_type] = type;
    SharedPtr<GetSystemCapabilityRequest> command(
        CreateCommand<GetSystemCapabilityRequest>(msg));
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app));

    EXPECT_CALL(app_mngr_, hmi_capabilities())
        .WillOnce(ReturnRef(mock_hmi_cap));

    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    MobileResultCodeIs(mobile_result::DATA_NOT_AVAILABLE), _))
        .WillOnce(Return(true));

    ASSERT_TRUE(command->Init());
    command->Run();
  }
}

}  // namespace get_vehicle_data_tequest
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
