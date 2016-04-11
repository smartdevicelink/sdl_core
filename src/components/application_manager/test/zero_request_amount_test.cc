/*
 * Copyright (c) 2015, Ford Motor Company
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

#include <string>
#include <algorithm>
#include "gtest/gtest.h"
#include "config_profile/profile.h"
#include "application_manager/request_controller.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/commands/mobile/put_file_request.h"
#include "application_manager/commands/mobile/register_app_interface_request.h"
#include "application_manager/commands/mobile/unregister_app_interface_request.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace application {
using namespace ::profile;

using namespace application_manager::request_controller;
using namespace application_manager;
using namespace NsSmartDeviceLink::NsSmartObjects;

commands::Command* RegisterApplication() {

  SmartObjectSPtr resultsmart = application_manager::MessageHelper::CreateModuleInfoSO(1);
  SmartObject& test_message = *resultsmart;
  uint32_t connection_key = 0;
  test_message[strings::params][strings::connection_key] = connection_key;
  test_message[strings::msg_params][strings::language_desired] = 0;
  test_message[strings::msg_params][strings::hmi_display_language_desired] = 0;
  commands::Command* testregCommand = new commands::RegisterAppInterfaceRequest(resultsmart);
  return testregCommand;
}

commands::Command* UnregisterApplication() {
  SmartObjectSPtr resultsmart = application_manager::MessageHelper::CreateModuleInfoSO(2);
  commands::Command *testregCommand =
      new commands::UnregisterAppInterfaceRequest(resultsmart);
  return testregCommand;
}

commands::Command* PutFileCommand(uint32_t &correlation_id,
                                  uint32_t &connection_key) {

  SmartObjectSPtr resultsmart = application_manager::MessageHelper::CreateModuleInfoSO(32);
  SmartObject& test_message = *resultsmart;
  test_message[strings::params][strings::correlation_id] = correlation_id;
  test_message[strings::params][strings::connection_key] = connection_key;
  test_message[strings::msg_params][strings::sync_file_name] = "file.png";
  test_message[strings::msg_params][strings::file_type] =
      mobile_apis::FileType::GRAPHIC_PNG;

  commands::Command* testCommand = new commands::PutFileRequest(resultsmart);
  return testCommand;
}

TEST(RequestControlTest, ZeroValuePendingRequestsAmount) {
  // Default value
  uint32_t pending_requests_amount = 0;
  EXPECT_EQ(pending_requests_amount,
            Profile::instance()->pending_requests_amount());

  const uint32_t big_count_of_requests_for_test_ = 10;  //bigger than pending_requests_amount
  uint32_t correlation_id = 0;
  uint32_t connection_key = 0;
  RequestController::TResult result;
  RequestController request_ctrl_;

  commands::Command * reg = RegisterApplication();
  request_ctrl_.addMobileRequest(reg, mobile_apis::HMILevel::HMI_FULL);

  for (uint32_t i = 0; i < big_count_of_requests_for_test_; ++i) {
    correlation_id = i;
    commands::Command* testCommand = PutFileCommand(correlation_id,
                                                     connection_key);
    result = request_ctrl_.addMobileRequest(testCommand,
                                            mobile_apis::HMILevel::HMI_FULL);
    EXPECT_EQ(RequestController::SUCCESS, result);
  }

  commands::Command * unreg = UnregisterApplication();
  request_ctrl_.addMobileRequest(unreg, mobile_apis::HMILevel::HMI_FULL);
}

TEST(RequestControlTest, ZeroValueAppRequestsTimeScale) {
  // Default value
  uint32_t app_requests_time_scale = 0;
  EXPECT_EQ(app_requests_time_scale, Profile::instance()->app_time_scale());

  const uint32_t big_count_of_requests_for_test_ = 10;  //bigger than pending_requests_amount
  uint32_t correlation_id = 0;
  uint32_t connection_key = 0;
  RequestController::TResult result;
  RequestController request_ctrl_;

  commands::Command * reg = RegisterApplication();
  request_ctrl_.addMobileRequest(reg, mobile_apis::HMILevel::HMI_FULL);

  for (uint32_t i = 0; i < big_count_of_requests_for_test_; ++i) {
    correlation_id = i;
    commands::Command * testCommand = PutFileCommand(correlation_id,
                                                     connection_key);
    result = request_ctrl_.addMobileRequest(testCommand,
                                            mobile_apis::HMILevel::HMI_FULL);
    EXPECT_EQ(RequestController::SUCCESS, result);
  }


  commands::Command* unreg = UnregisterApplication();
  request_ctrl_.addMobileRequest(unreg, mobile_apis::HMILevel::HMI_FULL);
}

TEST(RequestControlTest, ZeroValueAppTimeScaleMaxRequests) {
  // Default value
  uint32_t app_time_scale_max_requests = 0;
  EXPECT_EQ(app_time_scale_max_requests,
            Profile::instance()->app_time_scale_max_requests());

  const uint32_t big_count_of_requests_for_test_ = 10;  //bigger than pending_requests_amount
  uint32_t correlation_id = 0;
  uint32_t connection_key = 0;
  RequestController::TResult result;
  RequestController request_ctrl_;

  commands::Command * reg = RegisterApplication();
  request_ctrl_.addMobileRequest(reg, mobile_apis::HMILevel::HMI_FULL);

  for (uint32_t i = 0; i < big_count_of_requests_for_test_; ++i) {
    correlation_id = i;
    commands::Command* testCommand = PutFileCommand(correlation_id,
                                                     connection_key);
    result = request_ctrl_.addMobileRequest(testCommand,
                                            mobile_apis::HMILevel::HMI_FULL);
    EXPECT_EQ(RequestController::SUCCESS, result);
  }

  commands::Command* unreg = UnregisterApplication();
  request_ctrl_.addMobileRequest(unreg, mobile_apis::HMILevel::HMI_FULL);
}

}  // namespace application
}  // namespace components
}  // namespace test
