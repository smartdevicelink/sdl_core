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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_TEST_INCLUDE_VI_COMMAND_TEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_TEST_INCLUDE_VI_COMMAND_TEST_H_

#include <stdint.h>
#include "gtest/gtest.h"

#include "application_manager/commands/command.h"
#include "smart_objects/smart_object.h"

#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "vehicle_info_plugin/mock_custom_vehicle_data_manager.h"
#include "vehicle_info_plugin/vehicle_info_command_params.h"

namespace test {
namespace components {
namespace commands_test {

namespace am = ::application_manager;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using am::commands::MessageSharedPtr;
using ::smart_objects::SmartObject;

template <const CommandsTestMocks kIsNice = CommandsTestMocks::kNotNice>
class VICommandsTest : public CommandsTest<kIsNice> {
 public:
  enum { kDefaultTimeout_ = 100 };

  template <class Command>
  std::shared_ptr<Command> CreateCommandVI() {
    auto msg = CommandsTest<kIsNice>::CreateMessage();
    return CreateCommandVI<Command>(msg);
  }

  template <class Command>
  std::shared_ptr<Command> CreateCommandVI(MessageSharedPtr& msg) {
    InitCommandVI(kDefaultTimeout_);
    vehicle_info_plugin::VehicleInfoCommandParams params = {
        CommandsTest<kIsNice>::app_mngr_,
        CommandsTest<kIsNice>::mock_rpc_service_,
        CommandsTest<kIsNice>::mock_hmi_capabilities_,
        CommandsTest<kIsNice>::mock_policy_handler_,
        mock_custom_vehicle_data_manager_};
    return std::make_shared<Command>(msg, params);
  }

  testing::NiceMock<vehicle_info_plugin::MockCustomVehicleDataManager>
      mock_custom_vehicle_data_manager_;

 protected:
  void InitCommandVI(const uint32_t& timeout) {
    ON_CALL(CommandsTest<kIsNice>::app_mngr_, get_settings())
        .WillByDefault(ReturnRef(CommandsTest<kIsNice>::app_mngr_settings_));
    ON_CALL(CommandsTest<kIsNice>::app_mngr_settings_, default_timeout())
        .WillByDefault(ReturnRef(timeout));
  }
};
}  // namespace commands_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_TEST_INCLUDE_VI_COMMAND_TEST_H_
