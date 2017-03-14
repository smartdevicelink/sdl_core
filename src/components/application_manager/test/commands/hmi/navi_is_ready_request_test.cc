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

#include "application_manager/commands/hmi/navi_is_ready_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace navi_is_ready_request {

using ::testing::_;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::commands::NaviIsReadyRequest;
using am::event_engine::Event;

namespace {
const hmi_apis::FunctionID::eType kEventID =
    hmi_apis::FunctionID::Navigation_IsReady;
}  // namespace

typedef SharedPtr<NaviIsReadyRequest> NaviIsReadyRequestPtr;

class NaviIsReadyRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  NaviIsReadyRequestTest() : command_(CreateCommand<NaviIsReadyRequest>()) {
    ON_CALL(app_mngr_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
  }

  NaviIsReadyRequestPtr command_;
  MOCK(am::MockHmiInterfaces) mock_hmi_interfaces_;
  MOCK(application_manager_test::MockHMICapabilities) mock_hmi_capabilities_;
};

TEST_F(NaviIsReadyRequestTest,
       OnEvent_HmiInterfaceIsAvailable_NaviCooperatingIsAvailable) {
  const bool is_hmi_interface_available = true;

  MessageSharedPtr event_msg = CreateMessage();
  (*event_msg)[am::strings::msg_params][am::strings::available] =
      is_hmi_interface_available;

  Event event(kEventID);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_hmi_interfaces_,
              SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_Navigation,
                                am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_is_navi_cooperating(is_hmi_interface_available));

  command_->on_event(event);
}

TEST_F(NaviIsReadyRequestTest,
       OnEvent_HmiInterfaceIsNotAvailable_NaviCooperatingIsNotAvailable) {
  const bool is_hmi_interface_available = false;

  MessageSharedPtr event_msg = CreateMessage();
  (*event_msg)[am::strings::msg_params][am::strings::available] =
      is_hmi_interface_available;

  Event event(kEventID);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_hmi_interfaces_,
              SetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_Navigation,
                                am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_is_navi_cooperating(is_hmi_interface_available));

  command_->on_event(event);
}

TEST_F(NaviIsReadyRequestTest,
       OnEvent_AvailabilityFieldMissing_NaviCooperatingIsNotAvailable) {
  const bool is_hmi_interface_available = false;

  MessageSharedPtr event_msg = CreateMessage();
  Event event(kEventID);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_hmi_capabilities_,
              set_is_navi_cooperating(is_hmi_interface_available));

  command_->on_event(event);
}

}  // namespace navi_is_ready_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
