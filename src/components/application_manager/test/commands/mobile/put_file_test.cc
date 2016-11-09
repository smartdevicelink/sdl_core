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

#include "interfaces/MOBILE_API.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "mobile/put_file_response.h"
#include "mobile/put_file_request.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"

#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace put_file {

using namespace application_manager;

using ::testing::Return;
using ::testing::_;

class PutFilesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(PutFilesRequestTest, Run_TooManyHmiNone_UNSUCCESS) {
  MockAppPtr app(CreateMockApp());
  SharedPtr<commands::PutFileRequest> command(
      CreateCommand<commands::PutFileRequest>());

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_NONE));

  const uint32_t put_files_in_none_allowed = 1u;
  const uint32_t put_files_in_none_count = 2u;

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, put_file_in_none())
      .WillByDefault(ReturnRef(put_files_in_none_allowed));
  ON_CALL(*app, put_file_in_none_count())
      .WillByDefault(Return(put_files_in_none_count));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

}  // namespace put_file
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
