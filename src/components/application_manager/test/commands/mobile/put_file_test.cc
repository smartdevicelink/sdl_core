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

#include "application_manager/commands/commands_test.h"

#include "mobile/put_file_response.h"
#include "mobile/put_file_request.h"

#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"

#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using namespace application_manager;

using ::testing::Return;
using ::testing::_;

namespace {
const int32_t kConnectionKey = 1;
}

class PutFileResponceTest : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  void SetUp() OVERRIDE {
    message_ = utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map);
    (*message_)[strings::msg_params] =
        ::smart_objects::SmartObject(::smart_objects::SmartType_Map);

    command_sptr_ =
        CreateCommand<application_manager::commands::PutFileResponse>(message_);
  }

  MessageSharedPtr message_;
  utils::SharedPtr<commands::PutFileResponse> command_sptr_;
};

TEST_F(PutFileResponceTest, Run_InvalidApp_ApplicationNotRegisteredResponce) {
  ::smart_objects::SmartObject& message_ref = *message_;

  message_ref[strings::params][strings::connection_key] = kConnectionKey;

  utils::SharedPtr<Application> null_application_sptr;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(null_application_sptr));
  EXPECT_CALL(
      mock_app_manager_,
      SendMessageToMobile(
          MobileResultCodeIs(mobile_api::Result::APPLICATION_NOT_REGISTERED),
          _));
  command_sptr_->Run();
}

TEST_F(PutFileResponceTest, Run_ApplicationRegistered_Success) {
  ::smart_objects::SmartObject& message_ref = *message_;

  message_ref[strings::params][strings::connection_key] = kConnectionKey;
  message_ref[strings::msg_params][strings::success] = true;

  utils::SharedPtr<Application> application_sptr =
      utils::MakeShared<MockApplication>();

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(application_sptr));
  EXPECT_CALL(
      mock_app_manager_,
      SendMessageToMobile(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));
  command_sptr_->Run();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
