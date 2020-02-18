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

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "rc_rpc_plugin/commands/mobile/release_interior_vehicle_data_module_request.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "rc_rpc_plugin/mock/mock_interior_data_cache.h"
#include "rc_rpc_plugin/mock/mock_interior_data_manager.h"
#include "rc_rpc_plugin/mock/mock_rc_capabilities_manager.h"
#include "rc_rpc_plugin/mock/mock_rc_consent_manager.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "rc_rpc_plugin/rc_module_constants.h"

using application_manager::commands::MessageSharedPtr;
using rc_rpc_plugin_test::MockInteriorDataCache;
using rc_rpc_plugin_test::MockInteriorDataManager;
using rc_rpc_plugin_test::MockRCCapabilitiesManager;
using rc_rpc_plugin_test::MockRCConsentManager;
using rc_rpc_plugin_test::MockResourceAllocationManager;
using test::components::application_manager_test::MockApplication;
using test::components::commands_test::CommandRequestTest;
using test::components::commands_test::CommandsTestMocks;

using testing::_;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;

typedef std::shared_ptr<
    rc_rpc_plugin::commands::ReleaseInteriorVehicleDataModuleRequest>
    ReleaseCommandPtr;

namespace {
std::string kPolicyAppID = "app_id";
std::uint32_t kAppID = 2u;
std::uint32_t kConnectionKey = 1u;
std::string kModuleType = "RADIO";
std::string kModuleID = "76149d9b-5317-4cf5-9196-b8fac690fec5";
std::string kDefaultModuleID = "eb7739ea-b263-4fe1-af9c-9311d1acac2d";
}  // namespace

namespace rc_rpc_plugin_test {
namespace release_interior_vehicle_data_module_request {
using namespace rc_rpc_plugin;

class ReleaseInteriorVehicleDataModuleRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ReleaseInteriorVehicleDataModuleRequestTest()
      : mock_app_(CreateMockApp())
      , rc_capabilities_(std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType::SmartType_Array)) {}

  void SetUp() OVERRIDE {
    TestPreCondition();
    ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppID));
    ON_CALL(mock_hmi_capabilities_, rc_capability())
        .WillByDefault(Return(rc_capabilities_));
    ON_CALL(mock_rc_capabilities_manager_, CheckIfModuleExistsInCapabilities(_))
        .WillByDefault(Return(true));
    ON_CALL(mock_rc_capabilities_manager_,
            GetDefaultModuleIdFromCapabilities(kModuleType))
        .WillByDefault(Return(kDefaultModuleID));
  }

  MessageSharedPtr CreateBasicMessage() {
    MessageSharedPtr message = CreateMessage();
    (*message)[application_manager::strings::params]
              [application_manager::strings::function_id] =
                  mobile_apis::FunctionID::ReleaseInteriorVehicleDataModuleID;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kConnectionKey;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kAppID;
    (*message)[application_manager::strings::msg_params]
              [message_params::kModuleType] = mobile_apis::ModuleType::RADIO;
    (*message)[application_manager::strings::msg_params]
              [message_params::kModuleId] = kModuleID;
    return message;
  }

  void TestPreCondition() {
    message_ = CreateBasicMessage();
    command_ =
        CreateRCCommand<commands::ReleaseInteriorVehicleDataModuleRequest>(
            message_);
  }

  template <class Command>
  std::shared_ptr<Command> CreateRCCommand(MessageSharedPtr& msg) {
    InitCommand(kDefaultTimeout_);
    RCCommandParams params{app_mngr_,
                           mock_rpc_service_,
                           mock_hmi_capabilities_,
                           mock_policy_handler_,
                           mock_allocation_manager_,
                           mock_interior_data_cache_,
                           mock_interior_data_manager_,
                           mock_rc_capabilities_manager_,
                           mock_rc_consent_manger_};
    return std::make_shared<Command>(msg ? msg : msg = CreateMessage(), params);
  }

 protected:
  MessageSharedPtr message_;
  ReleaseCommandPtr command_;

  MockAppPtr mock_app_;
  NiceMock<MockResourceAllocationManager> mock_allocation_manager_;
  MockInteriorDataCache mock_interior_data_cache_;
  MockInteriorDataManager mock_interior_data_manager_;
  NiceMock<MockRCCapabilitiesManager> mock_rc_capabilities_manager_;
  MockRCConsentManager mock_rc_consent_manger_;
  smart_objects::SmartObjectSPtr rc_capabilities_;
};

TEST_F(ReleaseInteriorVehicleDataModuleRequestTest,
       ModuleIDIsAbsnetInMessage_ReturnsDefaultFromCapabilities) {
  (*message_)[application_manager::strings::msg_params].erase(
      message_params::kModuleId);

  auto module_id = command_->ModuleId();
  EXPECT_EQ(kDefaultModuleID, module_id);
}

TEST_F(ReleaseInteriorVehicleDataModuleRequestTest,
       ResourceIsNotAllocated_ResponseToMobile_UNSUCCESS_IGNORED) {
  ON_CALL(mock_allocation_manager_,
          ReleaseResource(kModuleType, kModuleID, kAppID))
      .WillByDefault(Return(ResourceReleasedState::eType::NOT_ALLOCATED));

  MessageSharedPtr message_to_mobile = CreateMessage();

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          _, application_manager::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mobile), Return(true)));

  command_->Execute();

  const bool success =
      (*message_to_mobile)[application_manager::strings::msg_params]
                          [application_manager::strings::success]
                              .asBool();
  auto result_code = static_cast<mobile_apis::Result::eType>(
      (*message_to_mobile)[application_manager::strings::msg_params]
                          [application_manager::strings::result_code]
                              .asUInt());

  EXPECT_FALSE(success);
  EXPECT_EQ(mobile_apis::Result::eType::IGNORED, result_code);
}

TEST_F(ReleaseInteriorVehicleDataModuleRequestTest,
       ResourceIsAllocated_ResponseToMobile_UNSUCCESS_REJECTED) {
  ON_CALL(mock_allocation_manager_,
          ReleaseResource(kModuleType, kModuleID, kAppID))
      .WillByDefault(Return(ResourceReleasedState::eType::IS_ALLOCATED));
  MessageSharedPtr message_to_mobile = CreateMessage();

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          _, application_manager::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mobile), Return(true)));

  command_->Execute();

  const bool success =
      (*message_to_mobile)[application_manager::strings::msg_params]
                          [application_manager::strings::success]
                              .asBool();
  auto result_code = static_cast<mobile_apis::Result::eType>(
      (*message_to_mobile)[application_manager::strings::msg_params]
                          [application_manager::strings::result_code]
                              .asUInt());

  EXPECT_FALSE(success);
  EXPECT_EQ(mobile_apis::Result::eType::REJECTED, result_code);
}

TEST_F(ReleaseInteriorVehicleDataModuleRequestTest,
       ResourceIsReleased_ResponseToMobile_SUCCESS_SUCCESS) {
  ON_CALL(mock_allocation_manager_,
          ReleaseResource(kModuleType, kModuleID, kAppID))
      .WillByDefault(Return(ResourceReleasedState::eType::IS_RELEASED));

  MessageSharedPtr message_to_mobile = CreateMessage();

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          _, application_manager::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mobile), Return(true)));

  command_->Execute();

  const bool success =
      (*message_to_mobile)[application_manager::strings::msg_params]
                          [application_manager::strings::success]
                              .asBool();
  auto result_code = static_cast<mobile_apis::Result::eType>(
      (*message_to_mobile)[application_manager::strings::msg_params]
                          [application_manager::strings::result_code]
                              .asUInt());

  EXPECT_TRUE(success);
  EXPECT_EQ(mobile_apis::Result::eType::SUCCESS, result_code);
}
}  // namespace release_interior_vehicle_data_module_request
}  // namespace rc_rpc_plugin_test
