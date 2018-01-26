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
#include "application_manager/commands/hmi/activate_app_request.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/command_impl.h"
#include "commands/commands_test.h"
#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace activate_app_request {

using ::testing::_;
using ::utils::SharedPtr;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using am::commands::MessageSharedPtr;
using am::commands::ActivateAppRequest;
using am::commands::CommandImpl;

using ::test::components::application_manager_test::MockApplication;

typedef SharedPtr<MockApplication> MockAppPtr;
typedef ::utils::SharedPtr<ActivateAppRequest> ActivateAppRequestPtr;

MATCHER_P(CheckMessage, level, "") {
  return level ==
         static_cast<mobile_apis::HMILevel::eType>(
             (*arg)[strings::msg_params][strings::activate_app_hmi_level]
                 .asInt());
}

namespace {
const uint32_t kAppId = 1u;
const uint32_t kCorrelationId = 2u;
}  // namespace

class ActivateAppRequestTest : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::app_id] = kAppId;
    msg_params[strings::correlation_id] = kCorrelationId;
    (*msg)[strings::msg_params] = msg_params;
    (*msg)[strings::params][strings::app_id] = kAppId;
    (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
    (*msg)[strings::app_id] = kAppId;
    return msg;
  }
};

TEST_F(ActivateAppRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(app));

  ON_CALL(*app, hmi_app_id()).WillByDefault(Return(kAppId));

// TODO(OKozlov) Investigate and fix issue with using log
#ifdef ENABLE_LOG
  (*msg)[strings::msg_params][strings::activate_app_hmi_level] =
      mobile_apis::HMILevel::HMI_FULL;
#endif
  ActivateAppRequestPtr command(CreateCommand<ActivateAppRequest>(msg));

  EXPECT_CALL(app_mngr_, set_application_id(kCorrelationId, kAppId));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
#ifdef ENABLE_LOG
  EXPECT_CALL(rpc_service_,
              SendMessageToHMI(CheckMessage(mobile_apis::HMILevel::HMI_FULL)));
#else
  EXPECT_CALL(rpc_service_,
              SendMessageToHMI(msg));
#endif
  command->Run();

#ifndef ENABLE_LOG
  EXPECT_EQ(CommandImpl::hmi_protocol_type_,
            (*msg)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(CommandImpl::protocol_version_,
            (*msg)[strings::params][strings::protocol_version].asInt());
#endif
}

}  // namespace activate_app_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
