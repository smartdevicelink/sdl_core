#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "functional_module/plugin_manager.h"
#include "mock_generic_module.h"
#include "mock_service.h"
#include "application_manager/mock_application.h"
#include "utils/shared_ptr.h"
#include <memory>

using application_manager::Message;
using protocol_handler::MajorProtocolVersion;
using application_manager::MockService;
using test::components::application_manager_test::MockApplication;
using ::testing::NiceMock;
using ::testing::Expectation;
using ::testing::ReturnRef;

namespace functional_modules {

class PluginManagerTest : public ::testing::Test {
 public:
  PluginManagerTest()
      : manager(std::make_shared<PluginManager>())
      , service(std::make_shared<MockService>()) {}

 protected:
  std::shared_ptr<PluginManager> manager;
  std::shared_ptr<MockService> service;
  MockGenericModule* module;

  void SetUp() OVERRIDE {
    manager->SetServiceHandler(service);

    ASSERT_EQ(1, manager->LoadPlugins("./plugins/"));
    const PluginManager::Modules& plugins = manager->plugins();
    PluginManager::Modules::const_iterator i = plugins.begin();
    module = static_cast<MockGenericModule*>(i->second.get());
  }
};

TEST_F(PluginManagerTest, ChangePluginsState) {
  ServiceState kState = ServiceState::SUSPENDED;
  EXPECT_CALL(*module, OnServiceStateChanged(kState));
  manager->OnServiceStateChanged(kState);
}

TEST_F(PluginManagerTest, RemoveAppExtension) {
  const uint32_t kAppId = 2;
  EXPECT_CALL(*module, RemoveAppExtension(kAppId)).Times(1);
  manager->RemoveAppExtension(kAppId);
}

TEST_F(PluginManagerTest, ProcessMessageFail) {
  Message* msg = new Message(protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  msg->set_protocol_version(MajorProtocolVersion::PROTOCOL_VERSION_UNKNOWN);
  EXPECT_CALL(*module, ProcessMessage(message)).Times(0);
  manager->ProcessMessage(message);
}

TEST_F(PluginManagerTest, ProcessMessagePass) {
  Message* msg = new Message(protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  msg->set_protocol_version(MajorProtocolVersion::PROTOCOL_VERSION_3);
  msg->set_function_id(101);  // see MockGenericModule
  EXPECT_CALL(*module, ProcessMessage(message))
      .Times(1)
      .WillOnce(Return(ProcessResult::PROCESSED));
  manager->ProcessMessage(message);
}

TEST_F(PluginManagerTest, SDL_events_triggers_module) {
  using namespace functional_modules;
  std::vector<ApplicationEvent> app_events;
  app_events.push_back(ApplicationEvent::kApplicationExit);
  app_events.push_back(ApplicationEvent::kApplicationUnregistered);

  application_manager::ApplicationSharedPtr dummy_ptr =
      std::make_shared<MockApplication>();
  std::vector<ApplicationEvent>::const_iterator ev = app_events.begin();
  for (; app_events.end() != ev; ++ev) {
    EXPECT_CALL(*module, OnApplicationEvent(*ev, dummy_ptr));
    manager->OnApplicationEvent(*ev, dummy_ptr);
  }

  std::vector<PolicyEvent> policy_events;
  policy_events.push_back(PolicyEvent::kApplicationPolicyUpdated);
  policy_events.push_back(PolicyEvent::kApplicationsDisabled);

  std::vector<PolicyEvent>::const_iterator ev_policy = policy_events.begin();
  for (; policy_events.end() != ev_policy; ++ev_policy) {
    EXPECT_CALL(*module, OnPolicyEvent(*ev_policy));
    manager->OnPolicyEvent(*ev_policy);
  }
}

TEST_F(PluginManagerTest, ProcessHMIMessageFail) {
  Message* msg = new Message(protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  message->set_protocol_version(MajorProtocolVersion::PROTOCOL_VERSION_UNKNOWN);
  EXPECT_CALL(*module, ProcessHMIMessage(message)).Times(0);
  manager->ProcessHMIMessage(message);
}

TEST_F(PluginManagerTest, ProcessHMIMessagePass) {
  Message* msg = new Message(protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  application_manager::MessagePtr message(msg);
  message->set_protocol_version(MajorProtocolVersion::PROTOCOL_VERSION_HMI);
  std::string json = "{\"method\": \"HMI-Func-1\"}";  // see MockGenericModule
  message->set_json_message(json);
  EXPECT_CALL(*module, ProcessHMIMessage(message))
      .Times(1)
      .WillOnce(Return(ProcessResult::PROCESSED));
  manager->ProcessHMIMessage(message);
}

TEST_F(PluginManagerTest, IsAppForPlugins) {
  using test::components::application_manager_test::MockApplication;
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  EXPECT_CALL(*module, IsAppForPlugin(app_ptr)).Times(1);
  manager->IsAppForPlugins(app_ptr);
}

TEST_F(PluginManagerTest, OnAppHMILevelChanged) {
  using test::components::application_manager_test::MockApplication;
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);

  const application_manager::custom_str::CustomString name("name");
  ON_CALL(*app, name()).WillByDefault(ReturnRef(name));
  mobile_apis::HMILevel::eType level = mobile_apis::HMILevel::eType::HMI_NONE;
  ON_CALL(*app, hmi_level()).WillByDefault(Return(level));

  Expectation is_for_plugin =
      EXPECT_CALL(*module, IsAppForPlugin(app_ptr)).WillOnce(Return(true));
  EXPECT_CALL(*module, OnAppHMILevelChanged(app_ptr, _))
      .Times(1)
      .After(is_for_plugin);
  manager->OnAppHMILevelChanged(app_ptr,
                                mobile_apis::HMILevel::eType::HMI_FULL);
}

}  // namespace functional_modules
