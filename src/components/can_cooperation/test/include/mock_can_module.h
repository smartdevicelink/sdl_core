#ifndef SRC_COMPONENTS_CAN_COOPERATION_TEST_INCLUDE_MOCK_CAN_MODULE_H
#define SRC_COMPONENTS_CAN_COOPERATION_TEST_INCLUDE_MOCK_CAN_MODULE_H

#include <stdint.h>
#include "can_cooperation/can_module_interface.h"
#include "application_manager/message.h"
#include "mock_application.h"

namespace test {
namespace components {
namespace can_cooperation_test {

class CANAppExtension;
typedef utils::SharedPtr<CANAppExtension> CANAppExtensionPtr;

class MockCANModuleInterface : public can_cooperation::CANModuleInterface {
 public:
  MOCK_CONST_METHOD0(GetPluginInfo, functional_modules::PluginInfo());
  MOCK_METHOD1(
      ProcessMessage,
      functional_modules::ProcessResult(application_manager::MessagePtr msg));
  MOCK_METHOD1(
      ProcessHMIMessage,
      functional_modules::ProcessResult(application_manager::MessagePtr msg));
  MOCK_METHOD1(SendResponseToMobile, void(application_manager::MessagePtr msg));
  MOCK_METHOD1(SendTimeoutResponseToMobile,
               void(application_manager::MessagePtr msg));
  MOCK_CONST_METHOD0(IsScanStarted, bool());
  MOCK_METHOD1(SetScanStarted, void(bool is_scan_started));
  MOCK_METHOD1(RemoveAppExtension, void(uint32_t app_id));
  MOCK_METHOD1(IsAppForPlugin,
               bool(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD2(OnAppHMILevelChanged,
               void(application_manager::ApplicationSharedPtr app,
                    mobile_apis::HMILevel::eType old_level));
  MOCK_METHOD2(CanAppChangeHMILevel,
               bool(application_manager::ApplicationSharedPtr app,
                    mobile_apis::HMILevel::eType new_level));
  MOCK_METHOD1(OnDeviceRemoved,
               void(const connection_handler::DeviceHandle& device));
  MOCK_METHOD1(SendHmiStatusNotification,
               void(application_manager::ApplicationSharedPtr app));
  MOCK_METHOD0(event_dispatcher, CanModuleEventDispatcher&());
  MOCK_METHOD0(RemoveAppExtensions, void());
  MOCK_METHOD0(service, application_manager::ServicePtr());
  MOCK_CONST_METHOD0(GetModuleID, functional_modules::ModuleID());
};

}  // namespace can_cooperation_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_CAN_COOPERATION_TEST_INCLUDE_MOCK_CAN_MODULE_H
