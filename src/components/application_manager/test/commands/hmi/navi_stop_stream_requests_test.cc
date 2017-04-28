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

#include "application_manager/commands/hmi/navi_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_audio_stop_stream_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace navi_stop_stream_requests {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace commands = am::commands;
using commands::MessageSharedPtr;
using am::event_engine::Event;

namespace {
const am::HmiInterfaces::InterfaceID kHmiInterface =
    am::HmiInterfaces::HMI_INTERFACE_Navigation;
}  // namespace

template <typename Command>
class NaviStopStreamRequestsTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  NaviStopStreamRequestsTest() {
    ON_CALL(this->app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    msg_ = CreateMessage();
    command_ = CreateCommand<Command>(msg_);
  }

  MessageSharedPtr msg_;
  SharedPtr<Command> command_;
  MOCK(am::MockHmiInterfaces) mock_hmi_interfaces_;
};

typedef testing::Types<commands::AudioStopStreamRequest,
                       commands::NaviStopStreamRequest> RequestCommandsList;
TYPED_TEST_CASE(NaviStopStreamRequestsTest, RequestCommandsList);

TYPED_TEST(NaviStopStreamRequestsTest, Run_HmiInterfaceNotAvailable_NoRequest) {
  EXPECT_CALL(TestFixture::mock_hmi_interfaces_,
              GetInterfaceState(kHmiInterface))
      .WillOnce(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(TestFixture::app_mngr_, SendMessageToHMI(_)).Times(0);

  TestFixture::command_->Run();
}

TYPED_TEST(NaviStopStreamRequestsTest, Run_HmiInterfaceAvailable_SentRequest) {
  EXPECT_CALL(TestFixture::mock_hmi_interfaces_,
              GetInterfaceState(kHmiInterface))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(TestFixture::app_mngr_, SendMessageToHMI(TestFixture::msg_));

  TestFixture::command_->Run();
}

}  // namespace navi_stop_stream_requests
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
