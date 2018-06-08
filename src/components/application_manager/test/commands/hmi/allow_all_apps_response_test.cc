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
#include "application_manager/commands/hmi/allow_all_apps_response.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/mock_application.h"
#include "application_manager/commands/command_impl.h"
#include "commands/commands_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace allow_all_apps_response {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::AllowAllAppsResponse;

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

typedef ::std::shared_ptr<AllowAllAppsResponse> ResponsePtr;

namespace {
const bool kResponseIsAllowed = true;
}  //

class AllowAllAppsResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(AllowAllAppsResponseTest, Run_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][hmi_response::allowed] = kResponseIsAllowed;

  ResponsePtr command(CreateCommand<AllowAllAppsResponse>(msg));

  EXPECT_CALL(app_mngr_, SetAllAppsAllowed(kResponseIsAllowed));

  command->Run();
}

}  // namespace allow_all_apps_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
