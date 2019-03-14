/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "application_manager/commands/command_request_test.h"

#include <stdint.h>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#include "mobile/get_app_service_data_request.h"
#include "mobile/get_app_service_data_request_to_mobile.h"
#include "mobile/get_app_service_data_response.h"
#include "mobile/get_app_service_data_response_from_mobile.h"
#include "mobile/on_app_service_data_notification.h"
#include "mobile/on_app_service_data_notification_from_mobile.h"
#include "mobile/perform_app_service_interaction_request.h"
#include "mobile/perform_app_service_interaction_request_to_mobile.h"
#include "mobile/perform_app_service_interaction_response.h"
#include "mobile/perform_app_service_interaction_response_from_mobile.h"
#include "mobile/publish_app_service_request.h"
#include "mobile/publish_app_service_response.h"

#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"

namespace am = application_manager;

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace dummy_mobile_commands_test {

namespace commands = app_service_rpc_plugin::commands;
using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;
using am::commands::MessageSharedPtr;
using ::test::components::event_engine_test::MockEventDispatcher;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using ::application_manager::ApplicationSharedPtr;
using ::test::components::application_manager_test::MockApplication;

namespace {
const std::string kEmptyString_ = "";
}  // namespace

template <class Command>
class MobileCommandsTest : public components::commands_test::CommandRequestTest<
                               CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    EXPECT_CALL(app_mngr_settings_, default_timeout())
        .WillOnce(ReturnRef(timeout));
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(kEmptyString_));
  }
};

template <class Command>
class MobileCommandsTestFirst : public MobileCommandsTest<Command> {
 public:
  using typename MobileCommandsTest<Command>::CommandType;
};

/* macro TYPED_TEST_CASE takes max 50 args. That is why there are few
 * TYPED_TEST_CASE for HMI and mobile commands
 */

typedef Types<commands::GetAppServiceDataRequest,
              commands::GetAppServiceDataRequestToMobile,
              commands::GetAppServiceDataResponse,
              commands::GetAppServiceDataResponseFromMobile,
              commands::OnAppServiceDataNotification,
              commands::OnAppServiceDataNotificationFromMobile,
              commands::PerformAppServiceInteractionRequest,
              commands::PerformAppServiceInteractionRequestToMobile,
              commands::PerformAppServiceInteractionResponse,
              commands::PerformAppServiceInteractionResponseFromMobile,
              commands::PublishAppServiceRequest,
              commands::PublishAppServiceResponse> MobileCommandsListFirst;

TYPED_TEST_CASE(MobileCommandsTestFirst, MobileCommandsListFirst);

TYPED_TEST(MobileCommandsTestFirst, CtorAndDtorCall) {
  std::shared_ptr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

}  // namespace dummy_mobile_commands_test
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
