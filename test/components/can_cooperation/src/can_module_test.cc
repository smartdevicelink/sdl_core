#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_app_extension.h"
#include "functional_module/module_observer.h"
#include "mock_application.h"
#include "mock_service.h"
#include "mock_can_connection.h"

using functional_modules::PluginInfo;
using functional_modules::ProcessResult;
using functional_modules::MobileFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using application_manager::MockApplication;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;

namespace can_cooperation {

class CanModuleTest : public ::testing::Test {
 protected:
  static CANModule* module;
  static MockService* mock_service;
  static MockCANConnection* mock_conn;

  static void SetUpTestCase() {
    module = CANModule::instance();
    mock_conn = new MockCANConnection();
    ::can_cooperation::CANConnectionSPtr conn(mock_conn);
    module->can_connection_ = conn;

    mock_service = new MockService();
    ServicePtr exp_service(mock_service);
    module->SetServiceHandler(exp_service);
    ServicePtr out_service = module->GetServiceHandler();
    EXPECT_EQ(exp_service.get(), out_service.get());
  }

  static void TearDownTestCase() {
    const ConnectionState kRet = ConnectionState::CLOSED;
    EXPECT_CALL(*mock_conn, CloseConnection()).Times(1).WillOnce(Return(kRet));
    CANModule::destroy();
  }
};

CANModule* CanModuleTest::module = 0;
MockService* CanModuleTest::mock_service = 0;
MockCANConnection* CanModuleTest::mock_conn = 0;

TEST_F(CanModuleTest, Create) {
  EXPECT_EQ(153, module->GetModuleID());
  PluginInfo plugin = module->GetPluginInfo();
  EXPECT_EQ(plugin.name, "ReverseSDLPlugin");
  EXPECT_EQ(plugin.version, 1);
}

TEST_F(CanModuleTest, ProcessMessageWrongMessage) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_id(-1);
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessageEmptyAppsList) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_RADIO_DETAILS);

  std::set<application_manager::ApplicationSharedPtr> apps;
  EXPECT_CALL(*mock_service, GetApplications()).Times(1).WillOnce(Return(apps));
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::PROCESSED, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessagePass) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_RADIO_DETAILS);

  std::set<application_manager::ApplicationSharedPtr> apps;
  MockApplication* app = new MockApplication();
  apps.insert(app);
  application_manager::AppExtensionUID uid = module->GetModuleID();
  CANAppExtension* can_ext = new CANAppExtension(uid);
  can_ext->GiveControl(true);
  application_manager::AppExtensionPtr ext(can_ext);

  EXPECT_CALL(*app, QueryInterface(uid)).Times(1).WillOnce(Return(ext));
  EXPECT_CALL(*app, app_id()).Times(1).WillOnce(Return(1));
  EXPECT_CALL(*mock_service, GetApplications()).Times(1).WillOnce(Return(apps));
  EXPECT_CALL(*mock_service, SendMessageToMobile(message)).Times(1);

  EXPECT_EQ(ProcessResult::PROCESSED, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, RemoveAppExtensionPassWay) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr valid_app(app);
  const application_manager::AppExtensionUID kUid = module->GetModuleID();
  CANAppExtension* ext = new CANAppExtension(kUid);
  application_manager::AppExtensionPtr valid_ext(ext);
  const application_manager::ApplicationId kAppId = 1;

  EXPECT_CALL(*mock_service, GetApplication(kAppId)).Times(1).WillOnce(
      Return(valid_app));
  EXPECT_CALL(*app, QueryInterface(kUid)).Times(1).WillOnce(Return(valid_ext));
  EXPECT_CALL(*app, RemoveExtension(kUid)).Times(1);

  module->RemoveAppExtension(kAppId);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfAppNoExist) {
  application_manager::ApplicationSharedPtr invalid_app;
  const application_manager::ApplicationId kAppId = 1;

  EXPECT_CALL(*mock_service, GetApplication(kAppId)).Times(1).WillOnce(
      Return(invalid_app));

  module->RemoveAppExtension(kAppId);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfExtNoExist) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr valid_app(app);
  const application_manager::AppExtensionUID kUid = module->GetModuleID();
  application_manager::AppExtensionPtr invalid_ext;
  const application_manager::ApplicationId kAppId = 1;

  EXPECT_CALL(*mock_service, GetApplication(kAppId)).Times(1).WillOnce(
      Return(valid_app));
  EXPECT_CALL(*app, QueryInterface(kUid)).Times(1).WillOnce(
      Return(invalid_ext));
  EXPECT_CALL(*app, RemoveExtension(kUid)).Times(0);

  module->RemoveAppExtension(kAppId);
}

TEST_F(CanModuleTest, SendResponseToMobile) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));

  EXPECT_CALL(*mock_service, SendMessageToMobile(message)).Times(1);

  module->SendResponseToMobile(message);
}

TEST_F(CanModuleTest, DISABLED_HandleMessage) {
  std::string kMessage = "Message to handle";
  const ConnectionState kRet = ConnectionState::INVALID;

  EXPECT_CALL(*mock_conn, Flash()).Times(1).WillOnce(Return(kRet));

  module->Handle(kMessage);
}

}  // namespace can_cooperation
