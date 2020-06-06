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

#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_consent_request.h"

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

using ::testing::_;
using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;

typedef std::shared_ptr<
    rc_rpc_plugin::commands::GetInteriorVehicleDataConsentRequest>
    GIVDConsentPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kAppId = 2u;
const std::string kPolicyAppId = "policy_app_id";
const std::string kMacAddress = "device1";
const std::string kModuleId_1 = "34045662-a9dc-4823-8435-91056d4c26cb";
const std::string kModuleId_2 = "eb7739ea-b263-4fe1-af9c-9311d1acac2d";
const std::string kModuleId_3 = "06cdec22-920e-4865-bf2e-9518463edc68";
const mobile_apis::ModuleType::eType kModule_Type =
    mobile_apis::ModuleType::eType::RADIO;
}  // namespace

namespace rc_rpc_plugin_test {
namespace get_interior_vehicle_data_consent_request_test {
using namespace rc_rpc_plugin;

class GetInteriorVehicleDataConsentRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetInteriorVehicleDataConsentRequestTest() : mock_app_(CreateMockApp()) {}

  void SetUp() OVERRIDE {
    TestPrecondition();
  }

  void TestPrecondition() {
    message_ = CreateBasicMessage();
    ON_CALL(mock_rc_capabilities_manager_, CheckIfModuleExistsInCapabilities(_))
        .WillByDefault(Return(true));
    ON_CALL(mock_rc_capabilities_manager_, IsMultipleAccessAllowed(_))
        .WillByDefault(Return(true));
  }

  void PrepareNoConsentExistInCache() {
    ON_CALL(mock_allocation_manager_, GetAccessMode())
        .WillByDefault(Return(hmi_apis::Common_RCAccessMode::ASK_DRIVER));
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
    ON_CALL(*mock_app_, mac_address()).WillByDefault(ReturnRef(kMacAddress));
    ON_CALL(mock_rc_consent_manger_,
            GetModuleConsent(kPolicyAppId, kMacAddress, _))
        .WillByDefault(Return(rc_rpc_types::ModuleConsent::NOT_EXISTS));
    ON_CALL(mock_allocation_manager_,
            AcquireResource(_, _, mock_app_->app_id()))
        .WillByDefault(Return(AcquireResult::IN_USE));
  }

  void PrepareMobileMessage() {
    auto& msg_params = (*message_)[application_manager::strings::msg_params];
    msg_params[message_params::kModuleType] = kModule_Type;

    msg_params[message_params::kModuleIds][0] = kModuleId_1;
    msg_params[message_params::kModuleIds][1] = kModuleId_2;
    msg_params[message_params::kModuleIds][2] = kModuleId_3;
  }

  /**
   * @brief CreateBasicMessage creates message for
   * GetInteriorVehicleData request for app1
   * @return message shared ptr
   */
  MessageSharedPtr CreateBasicMessage() {
    MessageSharedPtr message = CreateMessage();
    (*message)[application_manager::strings::params]
              [application_manager::strings::function_id] =
                  mobile_apis::FunctionID::GetInteriorVehicleDataID;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kConnectionKey;
    (*message)[application_manager::strings::params]
              [application_manager::strings::app_id] = kAppId;
    return message;
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
  GIVDConsentPtr command_;
  MessageSharedPtr message_;

  MockAppPtr mock_app_;
  NiceMock<MockResourceAllocationManager> mock_allocation_manager_;
  NiceMock<MockInteriorDataCache> mock_interior_data_cache_;
  NiceMock<MockInteriorDataManager> mock_interior_data_manager_;
  NiceMock<MockRCCapabilitiesManager> mock_rc_capabilities_manager_;
  NiceMock<MockRCConsentManager> mock_rc_consent_manger_;
};

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       ModuleIDReturnsEmptyString_SUCCESS) {
  std::string empty_str;
  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);

  EXPECT_EQ(empty_str, command_->ModuleId());
}

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       Execute_ModuleIdsIsAbsentInMessage_Response_INVALID_DATA) {
  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);

  auto response_to_mobile = CreateMessage();

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command_->Execute();

  const bool result =
      (*response_to_mobile)[application_manager::strings::msg_params]
                           [application_manager::strings::success]
                               .asBool();
  const auto result_code = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[application_manager::strings::msg_params]
                           [application_manager::strings::result_code]
                               .asUInt());
  EXPECT_FALSE(result);
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, result_code);
}

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       Execute_ModuleIdsIsEmpty_AddDefaultModuleID) {
  (*message_)[application_manager::strings::msg_params]
             [message_params::kModuleIds] = smart_objects::SmartObject(
                 smart_objects::SmartType::SmartType_Array);
  (*message_)[application_manager::strings::msg_params]
             [message_params::kModuleType] = kModule_Type;
  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);

  auto message_to_hmi = CreateMessage();

  const std::string module_type = "RADIO";
  const std::string default_module_id = "34045662-a9dc-4823-8435-91056d4c26cb";
  ON_CALL(mock_rc_capabilities_manager_,
          GetDefaultModuleIdFromCapabilities(module_type))
      .WillByDefault(Return(default_module_id));

  PrepareNoConsentExistInCache();

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));

  command_->Execute();

  ASSERT_FALSE((*message_to_hmi)[application_manager::strings::msg_params]
                                [message_params::kModuleIds]
                                    .empty());
  const auto sent_module_id =
      (*message_to_hmi)[application_manager::strings::msg_params]
                       [message_params::kModuleIds][0]
                           .asString();
  EXPECT_EQ(sent_module_id, default_module_id);
}

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       Execute_SDLForwardsMobileMessageToHMI_SUCCESS) {
  auto message_to_hmi = CreateMessage();

  PrepareMobileMessage();
  PrepareNoConsentExistInCache();

  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));

  command_->Execute();

  auto outgoing_msg_params =
      (*message_to_hmi)[application_manager::strings::msg_params];

  auto module_type = static_cast<mobile_apis::ModuleType::eType>(
      outgoing_msg_params[message_params::kModuleType].asUInt());

  EXPECT_EQ(kModule_Type, module_type);
  EXPECT_EQ(kModuleId_1,
            outgoing_msg_params[message_params::kModuleIds][0].asString());
  EXPECT_EQ(kModuleId_2,
            outgoing_msg_params[message_params::kModuleIds][1].asString());
  EXPECT_EQ(kModuleId_3,
            outgoing_msg_params[message_params::kModuleIds][2].asString());
}

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       On_Event_ConsentCollectionIsAbsentOrEmpty_Response_GENERIC_ERROR) {
  PrepareMobileMessage();

  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);

  // Consent collection in HMI response is absent
  auto event_message = CreateMessage();
  application_manager::event_engine::Event event_collection_is_absent(
      hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent);
  event_collection_is_absent.set_smart_object(*event_message);

  auto response_to_mobile = CreateMessage();

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command_->on_event(event_collection_is_absent);

  const auto result_code_is_absent = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[application_manager::strings::msg_params]
                           [application_manager::strings::result_code]
                               .asUInt());
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, result_code_is_absent);

  // Consent coolection in HMI response isn't absent but is empty
  event_message = CreateMessage();
  application_manager::event_engine::MobileEvent event_collection_is_empty(
      mobile_apis::FunctionID::GetInteriorVehicleDataConsentID);
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed] = smart_objects::SmartObject(
                      smart_objects::SmartType::SmartType_Array);
  event_collection_is_empty.set_smart_object(*event_message);

  response_to_mobile = CreateMessage();

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command_->on_event(event_collection_is_absent);

  const auto result_code_is_empty = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[application_manager::strings::msg_params]
                           [application_manager::strings::result_code]
                               .asUInt());
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, result_code_is_empty);
}

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       On_Event_ConsentsSizeIsNotEqualModuleIdsSize_Response_GENERIC_ERROR) {
  PrepareMobileMessage();
  PrepareNoConsentExistInCache();

  // ModuleIds collection will be saved
  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);
  command_->Execute();

  auto event_message = CreateMessage();
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed] = smart_objects::SmartObject(
                      smart_objects::SmartType::SmartType_Array);

  // Consent collection will contain only two items in collection
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed][0] = true;
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed][1] = false;

  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent);
  event.set_smart_object(*event_message);

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command_->on_event(event);

  const auto result_code_is_empty = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[application_manager::strings::msg_params]
                           [application_manager::strings::result_code]
                               .asUInt());
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, result_code_is_empty);
}

TEST_F(GetInteriorVehicleDataConsentRequestTest,
       On_Event_SaveModuleConsentsToLastState_Response_SUCCESS) {
  PrepareMobileMessage();
  PrepareNoConsentExistInCache();

  // ModuleIds collection will be saved
  command_ =
      CreateRCCommand<commands::GetInteriorVehicleDataConsentRequest>(message_);
  command_->Execute();

  // Consent collection will have same size as moduelIds collection
  auto event_message = CreateMessage();
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed][0] = true;
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed][1] = false;
  (*event_message)[application_manager::strings::msg_params]
                  [message_params::kAllowed][2] = false;

  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent);
  event.set_smart_object(*event_message);

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
  ON_CALL(*mock_app_, mac_address()).WillByDefault(ReturnRef(kMacAddress));

  EXPECT_CALL(mock_rc_consent_manger_, SaveModuleConsents(kPolicyAppId, _, _));

  auto response_to_mobile = CreateMessage();
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command_->on_event(event);

  const auto result_code_is_empty = static_cast<mobile_apis::Result::eType>(
      (*response_to_mobile)[application_manager::strings::msg_params]
                           [application_manager::strings::result_code]
                               .asUInt());
  EXPECT_EQ(mobile_apis::Result::SUCCESS, result_code_is_empty);
}

}  // namespace get_interior_vehicle_data_consent_request_test
}  // namespace rc_rpc_plugin_test
