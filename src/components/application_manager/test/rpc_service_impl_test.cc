/*
 * Copyright (c) 2020, Ford Motor Company
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

#include <gmock/gmock.h>
#include "gtest/gtest.h"

#include <string>
#include "application_manager/rpc_service_impl.h"

#include "application_manager/commands/command.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/mock_app_service_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_command_factory.h"
#include "application_manager/mock_command_holder.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_request.h"
#include "application_manager/mock_request_controller_settings.h"
#include "application_manager/mock_rpc_plugin.h"
#include "application_manager/mock_rpc_plugin_manager.h"
#include "application_manager/mock_rpc_protection_manager.h"
#include "application_manager/plugin_manager/plugin_keys.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "connection_handler/mock_connection_handler.h"
#include "hmi_message_handler/mock_hmi_message_handler.h"
#include "include/test/protocol_handler/mock_protocol_handler.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace rpc_service = application_manager::rpc_service;
namespace am = application_manager;
using test::components::hmi_message_handler_test::MockHMIMessageHandler;
using test::components::protocol_handler_test::MockProtocolHandler;
typedef smart_objects::SmartObjectSPtr MessageSharedPtr;
typedef utils::Optional<am::plugin_manager::RPCPlugin> PluginOpt;
using test::components::application_manager_test::MockAppServiceManager;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const uint32_t kConnectionKey = 1u;
const connection_handler::DeviceHandle kDeviceHandle = 1u;
const std::string kPolicyAppId = "policy_app_id";
const uint32_t kCorrelationId = 1u;
const uint32_t kFunctionId = 1u;
const uint32_t kAppId = 1u;
const int32_t kMobileProtocolType = 0;
const int32_t kProtocolVersion = 1;
const int32_t kConnectionSessionsCount = 2;
}  // namespace

class RPCServiceImplTest : public ::testing::Test {
 public:
  RPCServiceImplTest()
      : request_controller_(mock_request_controler_)
      , mock_rpc_protection_manager_(
            std::make_shared<
                testing::NiceMock<am::MockRPCProtectionManager> >())
      , mock_message_helper_(*am::MockMessageHelper::message_helper_mock())
      , mock_app_service_nmgr_(mock_app_mngr_, nullptr) {
    rpc_service_ = std::make_shared<rpc_service::RPCServiceImpl>(
        mock_app_mngr_,
        request_controller_,
        &mock_protocol_handler_,
        &mock_hmi_handler_,
        mock_command_holder_,
        mock_rpc_protection_manager_,
        hmi_so_factory_,
        mobile_so_factory_);
  }

  MessageSharedPtr CreateMessage(
      const smart_objects::SmartType type = smart_objects::SmartType_Null) {
    return std::make_shared<smart_objects::SmartObject>(type);
  }
  void PrepareBasicMessage(MessageSharedPtr& message) {
    (*message)[am::strings::params][am::strings::function_id] =
        static_cast<mobile_apis::FunctionID::eType>(
            mobile_apis::FunctionID::RESERVED);
    (*message)[am::strings::params][am::strings::correlation_id] =
        kCorrelationId;
    (*message)[am::strings::params][am::strings::protocol_type] =
        kMobileProtocolType;
    (*message)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
  }

  // This class is created to allow access to protected method
  // InitFunctionSchema from MOBILE_API
  class MobileAPIWithPublicSchemaSetting : public mobile_apis::MOBILE_API {
   public:
    void SetSchema(mobile_apis::FunctionID::eType function_id,
                   mobile_apis::messageType::eType message_type) {
      InitFunctionSchema(function_id, message_type);
    }
  };

  // This class is created to allow access to protected method
  // InitFunctionSchema from HMI_API
  class HMIAPIWithPublicSchemaSetting : public hmi_apis::HMI_API {
   public:
    void SetSchema(hmi_apis::FunctionID::eType function_id,
                   hmi_apis::messageType::eType message_type) {
      InitFunctionSchema(function_id, message_type);
    }
  };

 protected:
  hmi_apis::HMI_API hmi_so_factory_;
  mobile_apis::MOBILE_API mobile_so_factory_;
  testing::NiceMock<MockApplicationManager> mock_app_mngr_;
  testing::NiceMock<MockRequestControlerSettings> mock_request_controler_;
  testing::NiceMock<MockProtocolHandler> mock_protocol_handler_;
  am::request_controller::RequestController request_controller_;
  testing::NiceMock<MockHMIMessageHandler> mock_hmi_handler_;
  testing::NiceMock<MockCommandHolder> mock_command_holder_;
  std::shared_ptr<am::MockRPCProtectionManager> mock_rpc_protection_manager_;
  std::shared_ptr<rpc_service::RPCService> rpc_service_;
  std::shared_ptr<MockApplication> mock_app_ptr_;
  am::MockMessageHelper& mock_message_helper_;
  MockAppServiceManager mock_app_service_nmgr_;
  testing::NiceMock<am::plugin_manager::MockRPCPluginManager>
      mock_rpc_plugin_manager_;
  testing::NiceMock<am::plugin_manager::MockRPCPlugin> mock_rpc_plugin_;
  testing::NiceMock<MockCommandFactory> mock_command_factory_;
};

TEST_F(RPCServiceImplTest, ManageMobileCommand_MessageIsNullPtr_False) {
  MessageSharedPtr message;
  EXPECT_FALSE(rpc_service_->ManageMobileCommand(
      message, am::commands::Command::CommandSource::SOURCE_MOBILE));
}

TEST_F(RPCServiceImplTest, ManageMobileCommand_IsLowVoltage_False) {
  auto message = CreateMessage();
  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(true));
  EXPECT_FALSE(rpc_service_->ManageMobileCommand(
      message, am::commands::Command::CommandSource::SOURCE_MOBILE));
}

TEST_F(RPCServiceImplTest, ManageMobileCommand_AppInReconnectMode) {
  auto message = CreateMessage(smart_objects::SmartType_Map);
  auto source = am::commands::Command::CommandSource::SOURCE_MOBILE;
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  mock_app_ptr_ = std::make_shared<NiceMock<MockApplication> >();
  ON_CALL(mock_app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_ptr_));
  ON_CALL(*mock_app_ptr_, device()).WillByDefault(Return(kDeviceHandle));
  ON_CALL(*mock_app_ptr_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
  ON_CALL(mock_app_mngr_, IsAppInReconnectMode(kDeviceHandle, kPolicyAppId))
      .WillByDefault(Return(true));
  EXPECT_CALL(mock_command_holder_,
              Suspend(static_cast<am::ApplicationSharedPtr>(mock_app_ptr_),
                      am::CommandHolder::CommandType::kMobileCommand,
                      source,
                      message))
      .WillOnce(Return());

  EXPECT_TRUE(rpc_service_->ManageMobileCommand(message, source));
}

TEST_F(RPCServiceImplTest,
       ManageMobileCommand_SourceIsCheckedForUnsupportedRequest) {
  typedef am::plugin_manager::RPCPlugin RPCPlugin;

  auto message = CreateMessage(smart_objects::SmartType_Map);
  auto source = am::commands::Command::CommandSource::SOURCE_MOBILE;
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*message)[am::strings::params][am::strings::function_id] = kFunctionId;
  (*message)[am::strings::params][am::strings::protocol_type] =
      kMobileProtocolType;

  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(false));
  mock_app_ptr_ = std::make_shared<NiceMock<MockApplication> >();
  ON_CALL(mock_app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_ptr_));
  ON_CALL(*mock_app_ptr_, device()).WillByDefault(Return(kDeviceHandle));
  ON_CALL(*mock_app_ptr_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
  ON_CALL(mock_app_mngr_, IsAppInReconnectMode(kDeviceHandle, kPolicyAppId))
      .WillByDefault(Return(false));
  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  auto empty_plugin(utils::Optional<RPCPlugin>::OptionalEmpty::EMPTY);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(empty_plugin));

  EXPECT_FALSE(rpc_service_->ManageMobileCommand(message, source));
}

TEST_F(RPCServiceImplTest, ManageHMICommand_MessageIsNullPtr_False) {
  MessageSharedPtr message;
  EXPECT_FALSE(rpc_service_->ManageHMICommand(
      message, am::commands::Command::CommandSource::SOURCE_HMI));
}

TEST_F(RPCServiceImplTest, ManageHMICommand_IsLowVoltage_ReturnFalse) {
  auto message = CreateMessage();
  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(true));
  EXPECT_FALSE(rpc_service_->ManageHMICommand(
      message, am::commands::Command::CommandSource::SOURCE_HMI));
}

TEST_F(RPCServiceImplTest, ManageHMICommand_PluginIsEmpty_False) {
  auto message = CreateMessage();
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;
  (*message)[am::strings::params][am::strings::function_id] = kFunctionId;

  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(false));
  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  typedef am::plugin_manager::RPCPlugin RPCPlugin;
  utils::Optional<RPCPlugin> mock_rpc_plugin_opt(
      utils::Optional<RPCPlugin>::OptionalEmpty::EMPTY);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));

  EXPECT_FALSE(rpc_service_->ManageHMICommand(message, source));
}

TEST_F(RPCServiceImplTest, ManageHMICommand_FailedCreateCommand_False) {
  typedef am::plugin_manager::RPCPlugin RPCPlugin;

  auto message = CreateMessage();
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;
  (*message)[am::strings::params][am::strings::function_id] = kFunctionId;

  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(false));
  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  utils::Optional<RPCPlugin> mock_rpc_plugin_opt = mock_rpc_plugin_;
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));
  std::shared_ptr<MockRequest> cmd;
  ON_CALL(mock_command_factory_, CreateCommand(message, source))
      .WillByDefault(Return(cmd));

  EXPECT_FALSE(rpc_service_->ManageHMICommand(message, source));
}

TEST_F(RPCServiceImplTest, ManageHMICommand_IsAppInReconnectMode_True) {
  typedef am::plugin_manager::RPCPlugin RPCPlugin;

  auto message = CreateMessage();
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;
  (*message)[am::strings::params][am::strings::function_id] = kFunctionId;
  (*message)[am::strings::msg_params][am::strings::app_id] = kAppId;

  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(false));
  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  utils::Optional<RPCPlugin> mock_rpc_plugin_opt = mock_rpc_plugin_;
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));
  std::shared_ptr<MockRequest> cmd =
      std::make_shared<MockRequest>(kConnectionKey, kCorrelationId);
  ON_CALL(mock_command_factory_, CreateCommand(message, source))
      .WillByDefault(Return(cmd));

  auto mock_app = std::make_shared<NiceMock<MockApplication> >();
  ON_CALL(mock_app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));
  const connection_handler::DeviceHandle device_id1 = 1u;
  ON_CALL(*mock_app, device()).WillByDefault(Return(device_id1));
  ON_CALL(*mock_app, policy_app_id()).WillByDefault(Return(kPolicyAppId));
  ON_CALL(mock_app_mngr_, IsAppInReconnectMode(device_id1, kPolicyAppId))
      .WillByDefault(Return(true));

  EXPECT_CALL(mock_command_holder_,
              Suspend(static_cast<am::ApplicationSharedPtr>(mock_app),
                      am::CommandHolder::CommandType::kHmiCommand,
                      source,
                      message))
      .WillOnce(Return());
  EXPECT_TRUE(rpc_service_->ManageHMICommand(message, source));
}

TEST_F(RPCServiceImplTest,
       ManageHMICommand_MessageTypeUnknownTypeCommandNotInit_ReturnFalse) {
  typedef am::plugin_manager::RPCPlugin RPCPlugin;

  auto message = CreateMessage();
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;
  (*message)[am::strings::params][am::strings::function_id] = kFunctionId;
  (*message)[am::strings::params][am::strings::message_type] = am::kUnknownType;

  ON_CALL(mock_app_mngr_, IsLowVoltage()).WillByDefault(Return(false));
  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  utils::Optional<RPCPlugin> mock_rpc_plugin_opt = mock_rpc_plugin_;
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));
  std::shared_ptr<MockRequest> cmd =
      std::make_shared<MockRequest>(kConnectionKey, kCorrelationId);
  ON_CALL(mock_command_factory_, CreateCommand(message, source))
      .WillByDefault(Return(cmd));

  EXPECT_CALL(*cmd, Init()).WillOnce(Return(false));
  EXPECT_FALSE(rpc_service_->ManageHMICommand(message, source));
}

TEST_F(RPCServiceImplTest,
       IsAppServiceRPC_SourceMobileWithValidFuncId_ReturnTrue) {
  auto source_mobile = am::commands::Command::CommandSource::SOURCE_MOBILE;
  auto source_sdl = am::commands::Command::CommandSource::SOURCE_SDL;

  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(
      mobile_apis::FunctionID::GetSystemCapabilityID, source_mobile));
  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID, source_sdl));
  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(
      mobile_apis::FunctionID::GetSystemCapabilityID, source_sdl));
  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID, source_sdl));
}

TEST_F(RPCServiceImplTest,
       IsAppServiceRPC_SourceHMIWithValidFuncId_ReturnTrue) {
  auto source_hmi = am::commands::Command::CommandSource::SOURCE_HMI;
  auto source_sdl_to_hmi =
      am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI;
  int32_t function_id =
      hmi_apis::FunctionID::BasicCommunication_OnSystemCapabilityUpdated;

  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(function_id, source_hmi));
  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(function_id, source_sdl_to_hmi));
}

TEST_F(RPCServiceImplTest, IsAppServiceRPC_EmptyPlugin_ReturnFalse) {
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;

  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  typedef am::plugin_manager::RPCPlugin RPCPlugin;
  utils::Optional<RPCPlugin> mock_rpc_plugin_opt(
      utils::Optional<RPCPlugin>::OptionalEmpty::EMPTY);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));

  EXPECT_FALSE(rpc_service_->IsAppServiceRPC(kFunctionId, source));
}

TEST_F(RPCServiceImplTest, IsAppServiceRPC_PluginWithWrongName_ReturnFalse) {
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;

  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  PluginOpt mock_rpc_plugin_opt(mock_rpc_plugin_);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, PluginName()).WillByDefault(Return(""));

  EXPECT_FALSE(rpc_service_->IsAppServiceRPC(kFunctionId, source));
}

TEST_F(RPCServiceImplTest,
       IsAppServiceRPC_CommandFactoryIsAbleToProcess_ReturnTrue) {
  auto source = am::commands::Command::CommandSource::SOURCE_HMI;
  using namespace application_manager::plugin_manager::plugin_names;

  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  PluginOpt mock_rpc_plugin_opt(mock_rpc_plugin_);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(kFunctionId, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, PluginName())
      .WillByDefault(Return(app_service_rpc_plugin));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));
  ON_CALL(mock_command_factory_, IsAbleToProcess(kFunctionId, source))
      .WillByDefault(Return(true));

  EXPECT_TRUE(rpc_service_->IsAppServiceRPC(kFunctionId, source));
}

TEST_F(RPCServiceImplTest,
       UpdateMobileRPCParams_FunctionSchemaIsResettedAndAddedCustomMember) {
  using namespace ns_smart_device_link::ns_smart_objects;

  // Get initial state of schema
  auto function_id = mobile_apis::FunctionID::OnHMIStatusID;
  auto message_type = mobile_apis::messageType::notification;
  MobileAPIWithPublicSchemaSetting extended_mobile_api;
  extended_mobile_api.SetSchema(function_id, message_type);

  auto rpc_service_with_extended_api =
      std::make_shared<rpc_service::RPCServiceImpl>(
          mock_app_mngr_,
          request_controller_,
          &mock_protocol_handler_,
          &mock_hmi_handler_,
          mock_command_holder_,
          mock_rpc_protection_manager_,
          hmi_so_factory_,
          extended_mobile_api);

  CSmartSchema initial_schema;
  extended_mobile_api.GetSchema(function_id, message_type, initial_schema);
  auto initial_schema_item = initial_schema.getSchemaItem();

  // Change state of schema and add custom parameter
  auto new_function_id = mobile_apis::FunctionID::OnButtonEventID;
  std::map<std::string, SMember> members;
  members["custom member"] = SMember();
  rpc_service_with_extended_api->UpdateMobileRPCParams(
      new_function_id, message_type, members);
  CSmartSchema updated_schema;
  extended_mobile_api.GetSchema(new_function_id, message_type, updated_schema);

  // Check that schema is changed
  EXPECT_NE(initial_schema_item, updated_schema.getSchemaItem());
}

TEST_F(RPCServiceImplTest,
       UpdateHMIRPCParams_FunctionSchemaIsResettedAndAddedCustomMember) {
  using namespace ns_smart_device_link::ns_smart_objects;

  // Get initial state of schema
  auto function_id = hmi_apis::FunctionID::Buttons_OnButtonEvent;
  auto message_type = hmi_apis::messageType::notification;
  HMIAPIWithPublicSchemaSetting extended_hmi_api;
  extended_hmi_api.SetSchema(function_id, message_type);

  auto rpc_service_with_extended_api =
      std::make_shared<rpc_service::RPCServiceImpl>(
          mock_app_mngr_,
          request_controller_,
          &mock_protocol_handler_,
          &mock_hmi_handler_,
          mock_command_holder_,
          mock_rpc_protection_manager_,
          extended_hmi_api,
          mobile_so_factory_);

  CSmartSchema initial_schema;
  extended_hmi_api.GetSchema(function_id, message_type, initial_schema);
  auto initial_schema_item = initial_schema.getSchemaItem();

  // Change state of schema and add custom parameter
  auto new_function_id = hmi_apis::FunctionID::Buttons_OnButtonPress;
  std::map<std::string, SMember> members;
  members["custom member"] = SMember();
  rpc_service_with_extended_api->UpdateHMIRPCParams(
      function_id, message_type, members);
  CSmartSchema updated_schema;
  extended_hmi_api.GetSchema(new_function_id, message_type, updated_schema);

  // Check that schema is changed
  EXPECT_NE(initial_schema_item, updated_schema.getSchemaItem());
}

TEST_F(RPCServiceImplTest, SendMessageToHMI_HappyPath) {
  using namespace application_manager::plugin_manager::plugin_names;
  auto source = am::commands::Command::CommandSource::SOURCE_SDL_TO_HMI;
  auto func_id = mobile_apis::FunctionID::RESERVED;
  auto message = CreateMessage(smart_objects::SmartType_Map);
  PrepareBasicMessage(message);

  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  PluginOpt mock_rpc_plugin_opt(mock_rpc_plugin_);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(func_id, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, PluginName())
      .WillByDefault(Return(app_service_rpc_plugin));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));

  EXPECT_CALL(mock_command_factory_, IsAbleToProcess(func_id, source))
      .WillOnce(Return(true));
  rpc_service_->SendMessageToHMI(message);
}

TEST_F(RPCServiceImplTest, SendMessageToHMI_NoHMIHandler_ExecutionAborted) {
  auto message = CreateMessage(smart_objects::SmartType_Map);
  PrepareBasicMessage(message);

  rpc_service_->set_hmi_message_handler(nullptr);
  EXPECT_CALL(mock_app_mngr_, GetPluginManager()).Times(0);
  rpc_service_->SendMessageToHMI(message);
}

TEST_F(RPCServiceImplTest, Handle_NoHMIHandler_ExecutionAborted) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);
  message_handler->set_hmi_message_handler(nullptr);

  auto msg = std::shared_ptr<am::Message>();
  auto msg_to_hmi = am::rpc_service::impl::MessageToHmi(msg);

  EXPECT_CALL(mock_hmi_handler_, SendMessageToHMI(msg_to_hmi)).Times(0);
  message_handler->Handle(msg_to_hmi);
}

TEST_F(RPCServiceImplTest, Handle_ValidHMIHandler) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);
  auto msg = std::shared_ptr<am::Message>();
  auto msg_to_hmi = am::rpc_service::impl::MessageToHmi(msg);

  EXPECT_CALL(mock_hmi_handler_, SendMessageToHMI(msg_to_hmi));
  message_handler->Handle(msg_to_hmi);
}

TEST_F(RPCServiceImplTest, Handle_NoMobileProtocolHandler_ExecutionAborted) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);

  message_handler->set_protocol_handler(nullptr);
  auto message_to_send = std::make_shared<am::Message>(
      protocol_handler::MessagePriority::kDefault);

  EXPECT_CALL(mock_protocol_handler_, IsRPCServiceSecure(_)).Times(0);
  message_handler->Handle(
      am::rpc_service::impl::MessageToMobile(message_to_send, false));
}

TEST_F(RPCServiceImplTest, Handle_NoRawMessage_ExecutionAborted) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);
  auto message_to_send = std::make_shared<am::Message>(
      protocol_handler::MessagePriority::kDefault);

  EXPECT_CALL(mock_protocol_handler_, IsRPCServiceSecure(_)).Times(0);
  message_handler->Handle(
      am::rpc_service::impl::MessageToMobile(message_to_send, false));
}

TEST_F(RPCServiceImplTest,
       Handle_NeedsEncryptionAndServiceNotSecure_ExecutionAborted) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);
  auto message_to_send = std::make_shared<am::Message>(
      protocol_handler::MessagePriority::kDefault);
  message_to_send->set_protocol_version(
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1);
  message_to_send->set_connection_key(kConnectionKey);
  message_to_send->set_correlation_id(kCorrelationId);
  message_to_send->set_function_id(kFunctionId);
  message_to_send->set_message_type(static_cast<am::MessageType>(0));
  message_to_send->set_json_message(std::string("message"));

  EXPECT_CALL(mock_protocol_handler_, IsRPCServiceSecure(kConnectionKey))
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_rpc_protection_manager_, IsInEncryptionNeededCache(_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_protocol_handler_, SendMessageToMobileApp(_, _, _)).Times(0);
  message_handler->Handle(
      am::rpc_service::impl::MessageToMobile(message_to_send, false));
}

TEST_F(RPCServiceImplTest, Handle_HappyPathWithoutClosingSession) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);
  auto message_to_send = std::make_shared<am::Message>(
      protocol_handler::MessagePriority::kDefault);
  message_to_send->set_protocol_version(
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1);
  message_to_send->set_connection_key(kConnectionKey);
  message_to_send->set_correlation_id(kCorrelationId);
  message_to_send->set_function_id(kFunctionId);
  message_to_send->set_message_type(am::MessageType::kNotification);
  message_to_send->set_json_message(std::string("message"));

  EXPECT_CALL(mock_protocol_handler_, IsRPCServiceSecure(kConnectionKey))
      .WillOnce(Return(true));
  ON_CALL(mock_app_mngr_, application(_)).WillByDefault(Return(nullptr));
  EXPECT_CALL(*mock_rpc_protection_manager_, CheckPolicyEncryptionFlag(_, _, _))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_protocol_handler_, SendMessageToMobileApp(_, _, _))
      .WillOnce(Return());
  EXPECT_CALL(*mock_rpc_protection_manager_,
              RemoveFromEncryptionNeededCache(_, _))
      .WillOnce(Return());
  message_handler->Handle(
      am::rpc_service::impl::MessageToMobile(message_to_send, false));
}

TEST_F(RPCServiceImplTest, Handle_HappyPathWithClosedSession) {
  auto message_handler = std::make_shared<rpc_service::RPCServiceImpl>(
      mock_app_mngr_,
      request_controller_,
      &mock_protocol_handler_,
      &mock_hmi_handler_,
      mock_command_holder_,
      mock_rpc_protection_manager_,
      hmi_so_factory_,
      mobile_so_factory_);
  auto message_to_send = std::make_shared<am::Message>(
      protocol_handler::MessagePriority::kDefault);
  message_to_send->set_protocol_version(
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1);
  message_to_send->set_connection_key(kConnectionKey);
  message_to_send->set_correlation_id(kCorrelationId);
  message_to_send->set_function_id(kFunctionId);
  message_to_send->set_message_type(am::MessageType::kNotification);
  message_to_send->set_json_message(std::string("message"));

  NiceMock<test::components::connection_handler_test::MockConnectionHandler>
      mock_conn_handler;
  EXPECT_CALL(mock_app_mngr_, connection_handler())
      .WillRepeatedly(ReturnRef(mock_conn_handler));
  ON_CALL(mock_conn_handler, GetConnectionSessionsCount(_))
      .WillByDefault(Return(kConnectionSessionsCount));
  ON_CALL(mock_protocol_handler_, IsRPCServiceSecure(_))
      .WillByDefault(Return(true));
  ON_CALL(mock_app_mngr_, application(_)).WillByDefault(Return(nullptr));
  EXPECT_CALL(*mock_rpc_protection_manager_, CheckPolicyEncryptionFlag(_, _, _))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_protocol_handler_, SendMessageToMobileApp(_, _, _))
      .WillOnce(Return());
  EXPECT_CALL(*mock_rpc_protection_manager_,
              RemoveFromEncryptionNeededCache(_, _))
      .WillOnce(Return());
  EXPECT_CALL(mock_conn_handler, CloseSession(_, _)).WillOnce(Return());

  message_handler->Handle(
      am::rpc_service::impl::MessageToMobile(message_to_send, true));
}

TEST_F(RPCServiceImplTest, SendMessageToMobile_HappyPath) {
  using namespace application_manager::plugin_manager::plugin_names;
  namespace jhs = ns_smart_device_link::ns_json_handler::strings;
  auto func_id = mobile_apis::FunctionID::OnSystemRequestID;
  auto source = am::commands::Command::CommandSource::SOURCE_SDL;

  auto message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*message)[am::strings::params][am::strings::function_id] = func_id;
  (*message)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::PROPRIETARY;
  (*message)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::SUCCESS;

  (*message)[jhs::S_PARAMS][jhs::S_FUNCTION_ID] = func_id;
  (*message)[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
      mobile_apis::messageType::request;
  (*message)[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = kMobileProtocolType;
  (*message)[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = kProtocolVersion;

  mock_app_ptr_ = std::make_shared<NiceMock<MockApplication> >();
  ON_CALL(mock_app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_ptr_));
  ON_CALL(*mock_app_ptr_, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1));

  EXPECT_CALL(mock_app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(mock_app_service_nmgr_));
  auto rpc_pass_handler = std::make_shared<am::RPCPassingHandler>(
      mock_app_service_nmgr_, mock_app_mngr_);
  EXPECT_CALL(mock_app_service_nmgr_, GetRPCPassingHandler())
      .WillRepeatedly(ReturnRef(*rpc_pass_handler));
  EXPECT_CALL(mock_app_service_nmgr_, GetActiveServices())
      .WillRepeatedly(Return(std::vector<am::AppService>()));

  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  PluginOpt mock_rpc_plugin_opt(mock_rpc_plugin_);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(func_id, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, PluginName())
      .WillByDefault(Return(app_service_rpc_plugin));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));
  ON_CALL(mock_command_factory_, IsAbleToProcess(func_id, source))
      .WillByDefault(Return(true));

  auto window_id = 0;
  ON_CALL(mock_message_helper_, ExtractWindowIdFromSmartObject(_))
      .WillByDefault(Return(window_id));
  ON_CALL(mock_message_helper_, StringifiedFunctionID(func_id))
      .WillByDefault(Return("OnSystemRequest"));
  EXPECT_CALL(mock_app_mngr_,
              CheckPolicyPermissions(_, window_id, "OnSystemRequest", _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  policy_test::MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(mock_app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler));
  EXPECT_CALL(mock_policy_handler, OnUpdateRequestSentToMobile());

  rpc_service_->SendMessageToMobile(message);
}

TEST_F(RPCServiceImplTest,
       SendMessageToMobile_PolicyPermissionsFailed_ExecutionAborted) {
  using namespace application_manager::plugin_manager::plugin_names;

  namespace jhs = ns_smart_device_link::ns_json_handler::strings;
  auto func_id = mobile_apis::FunctionID::OnSystemRequestID;
  auto source = am::commands::Command::CommandSource::SOURCE_SDL;

  auto message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*message)[am::strings::params][am::strings::function_id] = func_id;
  (*message)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::PROPRIETARY;
  (*message)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::SUCCESS;

  (*message)[jhs::S_PARAMS][jhs::S_FUNCTION_ID] = func_id;
  (*message)[jhs::S_PARAMS][jhs::S_MESSAGE_TYPE] =
      mobile_apis::messageType::request;
  (*message)[jhs::S_PARAMS][jhs::S_PROTOCOL_TYPE] = kMobileProtocolType;
  (*message)[jhs::S_PARAMS][jhs::S_PROTOCOL_VERSION] = kProtocolVersion;

  mock_app_ptr_ = std::make_shared<NiceMock<MockApplication> >();
  ON_CALL(mock_app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_ptr_));
  ON_CALL(*mock_app_ptr_, protocol_version())
      .WillByDefault(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1));

  EXPECT_CALL(mock_app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(mock_app_service_nmgr_));
  auto rpc_pass_handler = std::make_shared<am::RPCPassingHandler>(
      mock_app_service_nmgr_, mock_app_mngr_);
  EXPECT_CALL(mock_app_service_nmgr_, GetRPCPassingHandler())
      .WillRepeatedly(ReturnRef(*rpc_pass_handler));
  EXPECT_CALL(mock_app_service_nmgr_, GetActiveServices())
      .WillRepeatedly(Return(std::vector<am::AppService>()));

  ON_CALL(mock_app_mngr_, GetPluginManager())
      .WillByDefault(ReturnRef(mock_rpc_plugin_manager_));
  PluginOpt mock_rpc_plugin_opt(mock_rpc_plugin_);
  ON_CALL(mock_rpc_plugin_manager_, FindPluginToProcess(func_id, source))
      .WillByDefault(Return(mock_rpc_plugin_opt));
  ON_CALL(mock_rpc_plugin_, PluginName())
      .WillByDefault(Return(app_service_rpc_plugin));
  ON_CALL(mock_rpc_plugin_, GetCommandFactory())
      .WillByDefault(ReturnRef(mock_command_factory_));
  ON_CALL(mock_command_factory_, IsAbleToProcess(func_id, source))
      .WillByDefault(Return(true));

  auto window_id = -1;  // Assume that this ID is invalid;
  ON_CALL(mock_message_helper_, ExtractWindowIdFromSmartObject(_))
      .WillByDefault(Return(window_id));
  ON_CALL(mock_message_helper_, StringifiedFunctionID(func_id))
      .WillByDefault(Return("OnSystemRequest"));
  EXPECT_CALL(mock_app_mngr_,
              CheckPolicyPermissions(_, window_id, "OnSystemRequest", _, _))
      .WillOnce(Return(mobile_apis::Result::INVALID_ENUM));

  policy_test::MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(mock_app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_policy_handler, OnUpdateRequestSentToMobile()).Times(0);

  rpc_service_->SendMessageToMobile(message);
}
}  // namespace application_manager_test

}  // namespace components

}  // namespace test
