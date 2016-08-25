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

#include "application_manager/commands/hmi/vi_is_ready_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::commands::VIIsReadyResponse;

typedef SharedPtr<VIIsReadyResponse> VIIsReadyResponsePtr;

class VIIsReadyResponseTest : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  VIIsReadyResponseTest()
      : message_(CreateMessage())
      , command_(CreateCommand<VIIsReadyResponse>(message_)) {}

  void SetUpExpectations(bool is_vi_cooperating_available) {
    EXPECT_CALL(app_mngr_, hmi_capabilities())
        .WillOnce(ReturnRef(mock_hmi_capabilities_));
    EXPECT_CALL(mock_hmi_capabilities_,
                set_is_ivi_cooperating(is_vi_cooperating_available));

    EXPECT_CALL(app_mngr_, GetPolicyHandler())
        .WillOnce(ReturnRef(mock_policy_handler_interface_));
    EXPECT_CALL(mock_policy_handler_interface_, OnVIIsReady());
  }

  MessageSharedPtr message_;
  VIIsReadyResponsePtr command_;
  am::MockHmiInterfaces mock_hmi_interfaces_;
  application_manager_test::MockHMICapabilities mock_hmi_capabilities_;
  policy_test::MockPolicyHandlerInterface mock_policy_handler_interface_;
};

TEST_F(VIIsReadyResponseTest,
       Run_NoKeyAvailableInMessage_HmiInterfacesIgnored) {
  EXPECT_CALL(app_mngr_, hmi_interfaces()).Times(0);
  EXPECT_CALL(mock_hmi_interfaces_, SetInterfaceState(_, _)).Times(0);
  // VI cooperating by default is not available.
  const bool is_vi_cooperating_available = false;
  SetUpExpectations(is_vi_cooperating_available);
  command_->Run();
}

TEST_F(VIIsReadyResponseTest, Run_KeyAvailableEqualToFalse_StateNotAvailable) {
  const bool is_vi_cooperating_available = false;
  (*message_)[am::strings::msg_params][am::strings::available] =
      is_vi_cooperating_available;

  EXPECT_CALL(app_mngr_, hmi_interfaces())
      .WillOnce(ReturnRef(mock_hmi_interfaces_));
  EXPECT_CALL(mock_hmi_interfaces_,
              SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VehicleInfo,
                                am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SetUpExpectations(is_vi_cooperating_available);

  command_->Run();
}

TEST_F(VIIsReadyResponseTest, Run_KeyAvailableEqualToFalse_StateAvailable) {
  const bool is_vi_cooperating_available = true;
  (*message_)[am::strings::msg_params][am::strings::available] =
      is_vi_cooperating_available;

  EXPECT_CALL(app_mngr_, hmi_interfaces())
      .WillOnce(ReturnRef(mock_hmi_interfaces_));
  EXPECT_CALL(mock_hmi_interfaces_,
              SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VehicleInfo,
                                am::HmiInterfaces::STATE_AVAILABLE));

  SetUpExpectations(is_vi_cooperating_available);

  command_->Run();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
