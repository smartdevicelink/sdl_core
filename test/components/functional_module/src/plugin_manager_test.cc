#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/plugin_manager.h"
#include "mock_generic_module.h"

using application_manager::Message;
using application_manager::ProtocolVersion;

namespace functional_modules {

class PluginManagerTest : public ::testing::Test {
 protected:
  static PluginManager* manager;
  static MockGenericModule* module;

  static void SetUpTestCase() {
    manager = PluginManager::instance();
    ASSERT_EQ(1, manager->LoadPlugins("./plugins/"));
    const PluginManager::Modules& plugins = manager->plugins_;
    PluginManager::Modules::const_iterator i = plugins.begin();
    module = ModulePtr::static_pointer_cast<MockGenericModule>(i->second).get();
  }

  static void TearDownTestCase() {
//    PluginManager::destroy(); try to uncomment after fix PluginManager
  }
};

PluginManager* PluginManagerTest::manager = 0;
MockGenericModule* PluginManagerTest::module = 0;

TEST_F(PluginManagerTest, ChangePluginsState) {
  ModuleState kState = ModuleState::SUSPENDED;
  EXPECT_CALL(*module, ChangeModuleState(kState)).Times(1);
  manager->ChangePluginsState(kState);
}

TEST_F(PluginManagerTest, IsMessageForPluginFail) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  msg->set_protocol_version(ProtocolVersion::kUnknownProtocol);
  EXPECT_FALSE(manager->IsMessageForPlugin(msg));
}

TEST_F(PluginManagerTest, IsMessageForPluginPass) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  msg->set_protocol_version(ProtocolVersion::kV3);
  msg->set_function_id(101);  // see MockGenericModule
  EXPECT_TRUE(manager->IsMessageForPlugin(msg));
}

TEST_F(PluginManagerTest, IsHMIMessageForPluginFail) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  msg->set_protocol_version(ProtocolVersion::kUnknownProtocol);
  EXPECT_FALSE(manager->IsHMIMessageForPlugin(msg));
}

TEST_F(PluginManagerTest, IsHMIMessageForPluginPass) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  msg->set_protocol_version(ProtocolVersion::kHMI);
  std::string json = "{\"method\": \"HMI-Func-1\"}";  // see MockGenericModule
  msg->set_json_message(json);
  EXPECT_TRUE(manager->IsHMIMessageForPlugin(msg));
}

TEST_F(PluginManagerTest, RemoveAppExtension) {
  const uint32_t kAppId = 2;
  EXPECT_CALL(*module, RemoveAppExtension(kAppId)).Times(1);
  manager->RemoveAppExtension(kAppId);
}

TEST_F(PluginManagerTest, ProcessMessageFail) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  msg->set_protocol_version(ProtocolVersion::kUnknownProtocol);
  EXPECT_CALL(*module, ProcessMessage(message)).Times(0);
  manager->ProcessMessage(message);
}

TEST_F(PluginManagerTest, ProcessMessagePass) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  msg->set_protocol_version(ProtocolVersion::kV3);
  msg->set_function_id(101);  // see MockGenericModule
  EXPECT_CALL(*module, ProcessMessage(message)).Times(1).WillOnce(
      Return(ProcessResult::PROCESSED));
  manager->ProcessMessage(message);
}

TEST_F(PluginManagerTest, ProcessHMIMessageFail) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  message->set_protocol_version(ProtocolVersion::kUnknownProtocol);
  EXPECT_CALL(*module, ProcessHMIMessage(message)).Times(0);
  manager->ProcessHMIMessage(message);
}

TEST_F(PluginManagerTest, ProcessHMIMessagePass) {
  Message* msg = new Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  message->set_protocol_version(ProtocolVersion::kHMI);
  std::string json = "{\"method\": \"HMI-Func-1\"}"; // see MockGenericModule
  message->set_json_message(json);
  EXPECT_CALL(*module, ProcessHMIMessage(message)).Times(1).WillOnce(
      Return(ProcessResult::PROCESSED));
  manager->ProcessHMIMessage(message);
}

}  // namespace functional_modules
