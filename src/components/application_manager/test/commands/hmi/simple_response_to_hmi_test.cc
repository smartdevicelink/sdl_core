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
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/hmi/sdl_activate_app_response.h"
#include "application_manager/commands/hmi/sdl_get_list_of_permissions_response.h"
#include "application_manager/commands/hmi/sdl_get_status_update_response.h"
#include "application_manager/commands/hmi/sdl_get_user_friendly_message_response.h"
#include "application_manager/commands/hmi/response_to_hmi.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace simple_response_to_hmi_test {

using ::testing::_;
using ::testing::Types;
using ::testing::NotNull;
using ::std::shared_ptr;

namespace commands = ::application_manager::commands;
using commands::MessageSharedPtr;

template <class Command>
class ResponseToHMICommandsTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;
};

typedef Types<commands::SDLActivateAppResponse,
              commands::SDLGetListOfPermissionsResponse,
              commands::SDLGetStatusUpdateResponse,
              commands::SDLGetUserFriendlyMessageResponse> ResponseCommandsList;

TYPED_TEST_CASE(ResponseToHMICommandsTest, ResponseCommandsList);

TYPED_TEST(ResponseToHMICommandsTest, Run_SendMessageToHMI_SUCCESS) {
  typedef typename TestFixture::CommandType CommandType;

  std::shared_ptr<CommandType> command = this->template CreateCommand<CommandType>();

  EXPECT_CALL(this->app_mngr_, SendMessageToHMI(NotNull()));

  command->Run();
}

class ResponseToHMITest : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(ResponseToHMITest, BasicMethodsOverloads_SUCCESS) {
  std::shared_ptr<commands::ResponseToHMI> command(
      CreateCommand<commands::ResponseToHMI>());

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
}

TEST_F(ResponseToHMITest, Run_SUCCESS) {
  std::shared_ptr<commands::ResponseToHMI> command(
      CreateCommand<commands::ResponseToHMI>());

  EXPECT_CALL(app_mngr_, SendMessageToHMI(NotNull()));

  command->Run();
}

}  // namespace simple_response_to_hmi_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
