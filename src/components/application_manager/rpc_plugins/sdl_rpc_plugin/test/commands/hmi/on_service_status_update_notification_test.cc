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
#include <stdint.h>

#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/smart_object_keys.h"
#include "gtest/gtest.h"
#include "hmi/on_service_status_update_notification.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace on_service_status_update_notification {

using namespace application_manager;
using sdl_rpc_plugin::commands::hmi::OnServiceStatusUpdateNotification;

typedef std::shared_ptr<OnServiceStatusUpdateNotification> NotificationPtr;
typedef hmi_apis::Common_ServiceType::eType ServiceType;
typedef hmi_apis::Common_ServiceEvent::eType ServiceEvent;

namespace {
const uint32_t kConnectionKey = 1232u;
const uint32_t kHmi_app_id = 321u;
}  // namespace

class OnServiceStatusUpdateNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnServiceStatusUpdateNotificationTest()
      : message_(CreateMessage(smart_objects::SmartType_Map)) {}

 public:
  MessageSharedPtr message_;
  NotificationPtr command_;
};

TEST_F(OnServiceStatusUpdateNotificationTest, SendNotificationToHMI) {
  (*message_)[strings::msg_params][hmi_notification::service_type] =
      ServiceType::AUDIO;
  (*message_)[strings::msg_params][hmi_notification::service_event] =
      ServiceEvent::REQUEST_ACCEPTED;
  (*message_)[strings::msg_params][strings::app_id] = kConnectionKey;
  command_ = CreateCommand<OnServiceStatusUpdateNotification>(message_);

  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(message_)).Times(1);

  auto mock_app = std::make_shared<NiceMock<MockApplication> >();

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  ON_CALL(*mock_app, hmi_app_id()).WillByDefault(Return(kHmi_app_id));

  command_->Init();
  command_->Run();
}

}  // namespace on_service_status_update_notification
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
