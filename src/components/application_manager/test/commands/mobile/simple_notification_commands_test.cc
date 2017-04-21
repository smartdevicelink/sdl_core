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
#include "application_manager/commands/command_impl.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/commands/mobile/on_app_interface_unregistered_notification.h"
#include "application_manager/commands/mobile/on_audio_pass_thru_notification.h"
#include "application_manager/commands/mobile/on_driver_distraction_notification.h"
#include "application_manager/commands/mobile/on_language_change_notification.h"
#include "application_manager/commands/mobile/on_permissions_change_notification.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace simple_notification_commands_test {

namespace am = ::application_manager;
namespace commands = am::commands;

using ::testing::_;
using ::testing::Types;

template <class Command>
class MobileNotificationCommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

typedef Types<commands::OnAppInterfaceUnregisteredNotification,
              commands::OnAudioPassThruNotification,
              commands::OnLanguageChangeNotification,
              commands::OnPermissionsChangeNotification,
              commands::mobile::OnDriverDistractionNotification>
    NotificationCommandsList;

MATCHER(CheckNotificationMessage, "") {
  return (*arg)[am::strings::params][am::strings::protocol_type].asInt() ==
             commands::CommandImpl::mobile_protocol_type_ &&
         (*arg)[am::strings::params][am::strings::protocol_version].asInt() ==
             commands::CommandImpl::protocol_version_ &&
         (*arg)[am::strings::params][am::strings::message_type].asInt() ==
             application_manager::MessageType::kNotification;
}

TYPED_TEST_CASE(MobileNotificationCommandsTest, NotificationCommandsList);

TYPED_TEST(MobileNotificationCommandsTest, Run_SendMessageToMobile_SUCCESS) {
  ::utils::SharedPtr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  EXPECT_CALL(this->app_mngr_,
              SendMessageToMobile(CheckNotificationMessage(), _));
  command->Run();
}

}  // namespace simple_notification_commands_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
