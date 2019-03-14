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

#include "hmi/as_app_service_activation_request.h"
#include "hmi/as_app_service_activation_response.h"
#include "hmi/as_get_active_service_consent_request.h"
#include "hmi/as_get_active_service_consent_response.h"
#include "hmi/as_get_app_service_data_request_from_hmi.h"
#include "hmi/as_get_app_service_data_request_to_hmi.h"
#include "hmi/as_get_app_service_data_response_from_hmi.h"
#include "hmi/as_get_app_service_data_response_to_hmi.h"
#include "hmi/as_get_app_service_records_request.h"
#include "hmi/as_get_app_service_records_response.h"
#include "hmi/as_perform_app_service_interaction_request_from_hmi.h"
#include "hmi/as_perform_app_service_interaction_request_to_hmi.h"
#include "hmi/as_perform_app_service_interaction_response_from_hmi.h"
#include "hmi/as_perform_app_service_interaction_response_to_hmi.h"
#include "hmi/as_publish_app_service_request.h"
#include "hmi/as_publish_app_service_response.h"
#include "hmi/on_as_app_service_data_notification.h"
#include "hmi/on_as_app_service_data_notification_from_hmi.h"

#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"

namespace am = application_manager;

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace dummy_hmi_commands_test {

namespace commands = app_service_rpc_plugin::commands;

using ::testing::_;
using ::testing::NotNull;
using ::testing::Types;
using application_manager::commands::MessageSharedPtr;
using ::test::components::event_engine_test::MockEventDispatcher;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using ::application_manager::ApplicationSharedPtr;
using ::test::components::application_manager_test::MockApplication;

template <class Command>
class HMICommandsTest : public components::commands_test::CommandRequestTest<
                            CommandsTestMocks::kIsNice> {
 public:
  typedef Command CommandType;

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    stream_retry_.first = 0;
    stream_retry_.second = 0;
    EXPECT_CALL(app_mngr_settings_, default_timeout())
        .WillOnce(ReturnRef(timeout));
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, start_stream_retry_amount())
        .WillByDefault(ReturnRef(stream_retry_));
  }

 protected:
  std::pair<uint32_t, int32_t> stream_retry_;
};

template <class Command>
class HMICommandsTestFirst : public HMICommandsTest<Command> {
 public:
  using typename HMICommandsTest<Command>::CommandType;
};

/* macro TYPED_TEST_CASE takes max 50 args. That is why there are few
 * TYPED_TEST_CASE for HMI and mobile commands
 */

typedef Types<commands::ASAppServiceActivationRequest,
              commands::ASAppServiceActivationResponse,
              commands::ASGetActiveServiceConsentRequest,
              commands::ASGetActiveServiceConsentResponse,
              commands::ASPerformAppServiceInteractionRequestFromHMI,
              commands::ASGetAppServiceDataRequestToHMI,
              commands::ASGetAppServiceDataResponseFromHMI,
              commands::ASGetAppServiceDataResponseToHMI,
              commands::ASGetAppServiceRecordsRequest,
              commands::ASGetAppServiceRecordsResponse,
              commands::ASPerformAppServiceInteractionRequestFromHMI,
              commands::ASPerformAppServiceInteractionRequestToHMI,
              commands::ASPerformAppServiceInteractionResponseFromHMI,
              commands::ASPerformAppServiceInteractionResponseToHMI,
              commands::ASPublishAppServiceRequest,
              commands::ASPublishAppServiceResponse,
              commands::OnASAppServiceDataNotification,
              commands::OnASAppServiceDataNotificationFromHMI>
    HMICommandsListFirst;

TYPED_TEST_CASE(HMICommandsTestFirst, HMICommandsListFirst);

TYPED_TEST(HMICommandsTestFirst, CtorAndDtorCall) {
  std::shared_ptr<typename TestFixture::CommandType> command =
      this->template CreateCommand<typename TestFixture::CommandType>();
  UNUSED(command);
}

}  // namespace dummy_hmi_commands_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
