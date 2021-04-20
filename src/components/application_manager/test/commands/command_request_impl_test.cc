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
#include <algorithm>
#include <string>

#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/commands_test.h"
#include "gtest/gtest.h"
#include "utils/lock.h"

#include "application_manager/application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"
#include "utils/data_accessor.h"

#include "application_manager/mock_app_service_manager.h"
#include "resumption/last_state_impl.h"
#include "resumption/last_state_wrapper_impl.h"

namespace test {
namespace components {
namespace commands_test {
namespace command_request_impl {

namespace am = application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetArgReferee;

using am::ApplicationManager;
using am::ApplicationSet;
using am::CommandParametersPermissions;
using am::MockHmiInterfaces;
using am::RPCParams;
using am::commands::CommandImpl;
using am::commands::CommandRequestImpl;
using am::commands::MessageSharedPtr;
using am::event_engine::EventObserver;
using test::components::application_manager_test::MockAppServiceManager;

typedef am::commands::CommandRequestImpl::RequestState RequestState;

namespace {
const uint32_t kConnectionKey = 5u;
const uint32_t kCorrelationId = 3u;
const hmi_apis::FunctionID::eType kInvalidFunctionId =
    hmi_apis::FunctionID::INVALID_ENUM;
const std::string kPolicyAppId = "Test";
const mobile_apis::Result::eType kMobResultSuccess =
    mobile_apis::Result::SUCCESS;
const std::string kDisallowedParam1 = "disallowed_param1";
const std::string kDisallowedParam2 = "disallowed_param2";
const std::string kAllowedParam = "allowed_param";
const std::string kUndefinedParam = "undefined_params";
const std::string kMissedParam = "missed_param";
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppStorageFile = "./app_info.dat";
const std::string kAppInfoStorage = "app_info_storage";
}  // namespace

class CommandRequestImplTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  CommandRequestImplTest()
      : app_set_lock_ptr_(std::make_shared<sync_primitives::Lock>()) {}

  class UnwrappedCommandRequestImpl : public CommandRequestImpl {
   public:
    using CommandRequestImpl::AddDisallowedParameters;
    using CommandRequestImpl::CheckAllowedParameters;
    using CommandRequestImpl::HasDisallowedParams;
    using CommandRequestImpl::RemoveDisallowedParameters;

    UnwrappedCommandRequestImpl(const MessageSharedPtr& message,
                                ApplicationManager& am,
                                app_mngr::rpc_service::RPCService& rpc_service,
                                app_mngr::HMICapabilities& hmi_capabilities,
                                policy::PolicyHandlerInterface& policy_handler)
        : CommandRequestImpl(
              message, am, rpc_service, hmi_capabilities, policy_handler) {}

    const RequestState current_state() const {
      return current_state_;
    }
    void set_current_state(const RequestState state) {
      current_state_ = state;
    }

    CommandParametersPermissions& parameters_permissions() {
      return parameters_permissions_;
    }

    CommandParametersPermissions& removed_parameters_permissions() {
      return removed_parameters_permissions_;
    }

    void SetHashUpdateMode(HashUpdateMode mode) {
      hash_update_mode_ = mode;
    }
  };

  MockAppPtr InitAppSetDataAccessor(std::shared_ptr<ApplicationSet>& app_set) {
    app_set = (!app_set ? std::make_shared<ApplicationSet>() : app_set);
    MockAppPtr app(CreateMockApp());
    app_set->insert(app);
    EXPECT_CALL(app_mngr_, applications())
        .WillOnce(
            Return(DataAccessor<ApplicationSet>(*app_set, app_set_lock_ptr_)));
    return app;
  }

  std::shared_ptr<sync_primitives::Lock> app_set_lock_ptr_;
};

typedef CommandRequestImplTest::UnwrappedCommandRequestImpl UCommandRequestImpl;
typedef std::shared_ptr<UCommandRequestImpl> CommandPtr;

TEST_F(CommandRequestImplTest, WindowID_ExpectDefaultWindowID) {
  auto msg = CreateMessage();
  const auto command = CreateCommand<UCommandRequestImpl>(msg);
  EXPECT_EQ(mobile_apis::PredefinedWindows::DEFAULT_WINDOW,
            command->window_id());
}

TEST_F(CommandRequestImplTest, OnTimeOut_StateCompleted_UNSUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Should be called twice:
  // First -- on `onTimeOut` method call
  // Second -- on destruction;
  EXPECT_CALL(event_dispatcher_, remove_observer(_)).Times(2);

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);

  // If `command` already done, then state should change to `kCompleted`.
  command->set_current_state(RequestState::kCompleted);

  command->onTimeOut();

  EXPECT_EQ(RequestState::kCompleted, command->current_state());
}

TEST_F(CommandRequestImplTest, OnTimeOut_StateAwaitingHMIResponse_SUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  (*msg)[strings::params][strings::function_id] = kInvalidFunctionId;
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  MessageSharedPtr dummy_msg(CreateMessage());
  EXPECT_CALL(mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillOnce(Return(dummy_msg));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(dummy_msg, Command::CommandSource::SOURCE_SDL));

  command->onTimeOut();

  // If `command` not done till now, then state should change to `kTimedOut`
  // and sent it to application manager to deal with it.
  EXPECT_EQ(RequestState::kTimedOut, command->current_state());
}

TEST_F(CommandRequestImplTest, CheckSyntax_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Checking message syntax.
  const std::string str1("\t\n");
  EXPECT_FALSE(command->CheckSyntax(str1, false));
  const std::string str2("\\n");
  EXPECT_FALSE(command->CheckSyntax(str2, false));
  const std::string str3("\\t");
  EXPECT_FALSE(command->CheckSyntax(str3, false));
  const std::string str4(" ");
  EXPECT_FALSE(command->CheckSyntax(str4, false));
  EXPECT_TRUE(command->CheckSyntax(str4, true));
}

TEST_F(CommandRequestImplTest, GetMobileResultCode_SUCCESS) {
  union ResultU {
    int32_t value_;
    hmi_apis::Common_Result::eType hmi_;
    mobile_apis::Result::eType mobile_;
  };

  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Run thru all possible accordance
  // of HMI and Mobile result codes.
  ResultU result_it;
  for (result_it.hmi_ = hmi_apis::Common_Result::SUCCESS;
       result_it.value_ < hmi_apis::Common_Result::TRUNCATED_DATA;
       ++result_it.value_) {
    if (result_it.hmi_ != hmi_apis::Common_Result::NO_DEVICES_CONNECTED &&
        result_it.hmi_ != hmi_apis::Common_Result::NO_APPS_REGISTERED) {
      EXPECT_EQ(result_it.mobile_,
                command->GetMobileResultCode(result_it.hmi_));
    }
  }
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            command->GetMobileResultCode(
                hmi_apis::Common_Result::NO_DEVICES_CONNECTED));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            command->GetMobileResultCode(
                hmi_apis::Common_Result::NO_APPS_REGISTERED));
  EXPECT_EQ(
      mobile_apis::Result::GENERIC_ERROR,
      command->GetMobileResultCode(hmi_apis::Common_Result::TRUNCATED_DATA));
}

TEST_F(CommandRequestImplTest, BasicMethodsOverloads_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
  EXPECT_NO_THROW(command->Run());
  am::event_engine::Event event(kInvalidFunctionId);
  EXPECT_NO_THROW(command->on_event(event));
}

TEST_F(CommandRequestImplTest, CreateHMINotification_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  const std::string kTestParamsKey = "test_msg_params";

  MessageSharedPtr msg_params = CreateMessage();
  (*msg_params)[kTestParamsKey] = 0;

  MessageSharedPtr result;

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  command->CreateHMINotification(kInvalidFunctionId, *msg_params);

  // Check if message been formed and sent to application manager.
  EXPECT_TRUE((*result).keyExists(strings::msg_params));
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(kTestParamsKey));
}

TEST_F(CommandRequestImplTest, SendHMIRequest_NoUseEvent_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillRepeatedly(
          Return(am::HmiInterfaces::HMI_INTERFACE_BasicCommunication));
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  // Return `true` prevents call of `SendResponse` method;

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));

  EXPECT_EQ(kCorrelationId,
            command->SendHMIRequest(kInvalidFunctionId, NULL, false));
}

TEST_F(CommandRequestImplTest, SendHMIRequest_UseEvent_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillRepeatedly(
          Return(am::HmiInterfaces::HMI_INTERFACE_BasicCommunication));
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  // Return `true` prevents call of `SendResponse` method;

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));
  EXPECT_CALL(event_dispatcher_, add_observer(_, _, _));

  EXPECT_EQ(kCorrelationId,
            command->SendHMIRequest(kInvalidFunctionId, NULL, true));
}

TEST_F(CommandRequestImplTest, RemoveDisallowedParameters_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][kDisallowedParam1] = 0u;
  (*msg)[strings::msg_params][kDisallowedParam2] = 0u;
  (*msg)[strings::msg_params][kAllowedParam] = 0u;
  (*msg)[strings::msg_params][kUndefinedParam] = 0u;
  (*msg)[strings::msg_params][kMissedParam] = 0u;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  CommandParametersPermissions& permission = command->parameters_permissions();
  permission.disallowed_params.insert(kDisallowedParam1);
  permission.disallowed_params.insert(kDisallowedParam2);
  permission.allowed_params.insert(kAllowedParam);
  permission.undefined_params.insert(kUndefinedParam);

  command->RemoveDisallowedParameters();

  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kDisallowedParam1));
  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kDisallowedParam2));
  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kUndefinedParam));
  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kMissedParam));
  EXPECT_TRUE((*msg)[strings::msg_params].keyExists(kAllowedParam));
  EXPECT_TRUE(command->HasDisallowedParams());
}

TEST_F(CommandRequestImplTest,
       CheckAllowedParameters_RegisterAppInterface_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::RegisterAppInterfaceID;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  EXPECT_CALL(app_mngr_, applications()).Times(0);
  EXPECT_TRUE(command->CheckPermissions());
}

TEST_F(CommandRequestImplTest,
       CheckAllowedParameters_NoAppWithSameConnectionKey_UNSUCCESS) {
  MessageSharedPtr message = CreateMessage();
  CommandPtr command = CreateCommand<UCommandRequestImpl>(message);
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(MockAppPtr()));
  EXPECT_FALSE(command->CheckPermissions());
}

TEST_F(CommandRequestImplTest, CheckAllowedParameters_NoMsgParamsMap_SUCCESS) {
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, GetWindowIds())
      .WillByDefault(Return(
          am::WindowIds(1, mobile_apis::PredefinedWindows::DEFAULT_WINDOW)));
  ON_CALL(*mock_app,
          WindowIdExists(mobile_apis::PredefinedWindows::DEFAULT_WINDOW))
      .WillByDefault(Return(true));

  MessageSharedPtr message = CreateMessage();
  (*message)[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  CommandPtr command = CreateCommand<UCommandRequestImpl>(message);

  EXPECT_CALL(app_mngr_, CheckPolicyPermissions(_, _, _, _, _))
      .WillOnce(Return(kMobResultSuccess));

  EXPECT_TRUE(command->CheckPermissions());
}

TEST_F(CommandRequestImplTest,
       CheckAllowedParameters_WrongPolicyPermissions_UNSUCCESS) {
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, GetWindowIds())
      .WillByDefault(Return(
          am::WindowIds(1, mobile_apis::PredefinedWindows::DEFAULT_WINDOW)));
  ON_CALL(*mock_app,
          WindowIdExists(mobile_apis::PredefinedWindows::DEFAULT_WINDOW))
      .WillByDefault(Return(true));

  MessageSharedPtr message = CreateMessage();
  (*message)[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*message)[strings::params][strings::message_type] =
      mobile_apis::messageType::request;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(message);

  EXPECT_CALL(*mock_app, app_id())
      .Times(1)
      .WillRepeatedly(Return(kConnectionKey));

  EXPECT_CALL(app_mngr_, CheckPolicyPermissions(_, _, _, _, _))
      .WillOnce(Return(mobile_apis::Result::INVALID_ENUM));

  EXPECT_CALL(mock_message_helper_, CreateBlockedByPoliciesResponse(_, _, _, _))
      .WillOnce(Return(smart_objects::SmartObjectSPtr()));

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, _));
  EXPECT_FALSE(command->CheckPermissions());
}

ACTION_P(GetArg3, output) {
  *output = arg3;
}

TEST_F(CommandRequestImplTest, CheckAllowedParameters_MsgParamsMap_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][kPolicyAppId] = true;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  MockAppPtr app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));
  ON_CALL(*app, GetWindowIds())
      .WillByDefault(Return(
          am::WindowIds(1, mobile_apis::PredefinedWindows::DEFAULT_WINDOW)));
  ON_CALL(*app, WindowIdExists(mobile_apis::PredefinedWindows::DEFAULT_WINDOW))
      .WillByDefault(Return(true));

  RPCParams params;
  EXPECT_CALL(app_mngr_, CheckPolicyPermissions(_, _, _, _, _))
      .WillOnce(DoAll(GetArg3(&params), Return(kMobResultSuccess)));

  EXPECT_TRUE(command->CheckPermissions());
  EXPECT_TRUE(params.end() !=
              std::find(params.begin(), params.end(), kPolicyAppId));
}

TEST_F(CommandRequestImplTest, AddDisallowedParameters_SUCCESS) {
  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(
      kDisallowedParam1, mobile_apis::VehicleDataType::VEHICLEDATA_MYKEY));

  MessageSharedPtr msg;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  command->removed_parameters_permissions().disallowed_params.insert(
      kDisallowedParam1);

  command->AddDisallowedParameters(*msg);

  EXPECT_TRUE((*msg)[strings::msg_params].keyExists(kDisallowedParam1));
}

TEST_F(CommandRequestImplTest, SendResponse_TimedOut_UNSUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  command->set_current_state(RequestState::kTimedOut);

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);

  // Args do not affect on anything in this case;
  command->SendResponse(true, kMobResultSuccess, NULL, NULL);

  EXPECT_EQ(RequestState::kTimedOut, command->current_state());
}

TEST_F(CommandRequestImplTest, SendResponse_SUCCESS) {
  MessageSharedPtr msg;
  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  EXPECT_TRUE(smart_objects::SmartType_Null == (*msg).getType());

  MessageSharedPtr result;

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  // Args do not affect on anything in this case;
  command->SendResponse(true, kMobResultSuccess, NULL, NULL);

  EXPECT_EQ(RequestState::kCompleted, command->current_state());

  EXPECT_TRUE(smart_objects::SmartType_Map == (*msg).getType());
}

TEST_F(CommandRequestImplTest,
       SendResponse_AddDisallowedParametersToInfo_SUCCESS) {
  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(
      kDisallowedParam1, mobile_apis::VehicleDataType::VEHICLEDATA_MYKEY));

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::SubscribeVehicleDataID;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  command->removed_parameters_permissions().disallowed_params.insert(
      kDisallowedParam1);

  MessageSharedPtr result;

  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  command->SendResponse(true, kMobResultSuccess, NULL, NULL);

  EXPECT_EQ(RequestState::kCompleted, command->current_state());

  EXPECT_TRUE((*result)[strings::msg_params].keyExists(strings::info));
  EXPECT_FALSE(
      (*result)[strings::msg_params][strings::info].asString().empty());
}

TEST_F(CommandRequestImplTest, HashUpdateAllowed_UpdateExpected) {
  MessageSharedPtr msg;
  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SetHashUpdateMode(CommandRequestImpl::HashUpdateMode::kDoHashUpdate);

  MessageSharedPtr result;

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  const bool is_succeeded = true;
  command->SendResponse(is_succeeded, kMobResultSuccess, NULL, NULL);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, UpdateHash());

  command.reset();
}

TEST_F(CommandRequestImplTest, HashUpdateDisallowed_HashUpdateNotExpected) {
  MessageSharedPtr msg;
  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SetHashUpdateMode(
      CommandRequestImpl::HashUpdateMode::kSkipHashUpdate);

  MessageSharedPtr result;

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  const bool is_succeeded = true;
  command->SendResponse(is_succeeded, kMobResultSuccess, NULL, NULL);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(*mock_app, UpdateHash()).Times(0);

  command.reset();
}

TEST_F(CommandRequestImplTest, RequestFailed_HashUpdateNotExpected) {
  MessageSharedPtr msg;
  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SetHashUpdateMode(CommandRequestImpl::HashUpdateMode::kDoHashUpdate);

  MessageSharedPtr result;

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  const bool is_succeeded = false;
  command->SendResponse(is_succeeded, kMobResultSuccess, NULL, NULL);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(*mock_app, UpdateHash()).Times(0);

  command.reset();
}

TEST_F(CommandRequestImplTest, AppNotFound_HashUpdateNotExpected) {
  MessageSharedPtr msg;
  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SetHashUpdateMode(CommandRequestImpl::HashUpdateMode::kDoHashUpdate);

  MessageSharedPtr result;

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  const bool is_succeeded = true;
  command->SendResponse(is_succeeded, kMobResultSuccess, NULL, NULL);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app, UpdateHash()).Times(0);

  command.reset();
}

TEST_F(CommandRequestImplTest, SendProviderRequest_ByServiceType) {
  auto last_state = std::make_shared<resumption::LastStateWrapperImpl>(
      std::make_shared<resumption::LastStateImpl>(kAppStorageFolder,
                                                  kAppInfoStorage));

  MockAppServiceManager app_service_manager(app_mngr_, last_state);
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(app_service_manager));
  EXPECT_CALL(app_service_manager, GetProviderByType("MEDIA", _, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(mock_app), SetArgReferee<3>(false)));

  MessageSharedPtr result;
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::GetAppServiceDataID;
  (*msg)[strings::msg_params][strings::service_type] = "MEDIA";

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SendProviderRequest(
      mobile_apis::FunctionID::GetAppServiceDataID,
      hmi_apis::FunctionID::AppService_GetAppServiceData,
      &(*msg),
      true);
}

TEST_F(CommandRequestImplTest, SendProviderRequest_ByProviderID) {
  auto last_state = std::make_shared<resumption::LastStateWrapperImpl>(
      std::make_shared<resumption::LastStateImpl>(kAppStorageFolder,
                                                  kAppInfoStorage));

  MockAppServiceManager app_service_manager(app_mngr_, last_state);
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(app_service_manager));
  EXPECT_CALL(app_service_manager, GetProviderByID("serviceid12345", _, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(mock_app), SetArgReferee<3>(false)));

  MessageSharedPtr result;
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::GetAppServiceDataID;
  (*msg)[strings::msg_params][strings::service_id] = "serviceid12345";

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SendProviderRequest(
      mobile_apis::FunctionID::GetAppServiceDataID,
      hmi_apis::FunctionID::AppService_GetAppServiceData,
      &(*msg),
      true);
}

TEST_F(CommandRequestImplTest, SendProviderRequestToHMI_ByProviderID) {
  auto last_state = std::make_shared<resumption::LastStateWrapperImpl>(
      std::make_shared<resumption::LastStateImpl>(kAppStorageFolder,
                                                  kAppInfoStorage));

  MockAppServiceManager app_service_manager(app_mngr_, last_state);
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(app_service_manager));
  EXPECT_CALL(app_service_manager, GetProviderByID("serviceid12345", _, _, _))
      .WillOnce(DoAll(SetArgReferee<2>(mock_app), SetArgReferee<3>(true)));

  MessageSharedPtr result;
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::GetAppServiceDataID;
  (*msg)[strings::msg_params][strings::service_id] = "serviceid12345";

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);
  command->SendProviderRequest(
      mobile_apis::FunctionID::GetAppServiceDataID,
      hmi_apis::FunctionID::AppService_GetAppServiceData,
      &(*msg),
      true);
}

}  // namespace command_request_impl
}  // namespace commands_test
}  // namespace components
}  // namespace test
