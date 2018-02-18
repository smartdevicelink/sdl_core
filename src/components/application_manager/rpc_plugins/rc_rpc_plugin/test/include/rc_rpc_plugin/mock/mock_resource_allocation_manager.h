#ifndef SRC_COMPONENTS_REMOTE_CONTROL_TEST_INCLUDE_MOCK_RESOURCE_ALLOCATION_MANAGER_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_TEST_INCLUDE_MOCK_RESOURCE_ALLOCATION_MANAGER_H_

#include "gmock/gmock.h"
#include "application_manager/rpc_plugins/rc_rpc_plugin/include/rc_rpc_plugin/resource_allocation_manager.h"

namespace rc_rpc_plugin_test{

class MockResourceAllocationManager
    : public rc_rpc_plugin::ResourceAllocationManager {
 public:
  MOCK_METHOD2(AcquireResource,
                 rc_rpc_plugin::AcquireResult::eType
                     (const std::string& module_type, const uint32_t app_id));
  MOCK_METHOD2(ForceAcquireResource,
               void(const std::string& module_type, const uint32_t app_id));
  MOCK_METHOD2(OnDriverDisallowed,
               void(const std::string& module_type, const uint32_t app_id));
  MOCK_METHOD2(OnApplicationEvent,
               void(application_manager::plugin_manager::ApplicationEvent event,
                    application_manager::ApplicationSharedPtr application));
  MOCK_METHOD1(OnPolicyEvent, void(application_manager::plugin_manager::PolicyEvent event));
  MOCK_METHOD1(SetAccessMode,
               void(const hmi_apis::Common_RCAccessMode::eType access_mode));
  MOCK_CONST_METHOD0(GetAccessMode, hmi_apis::Common_RCAccessMode::eType());
  MOCK_METHOD3(SetResourceState,
               void(const std::string& module_type,
                    const uint32_t app_id,
                    const rc_rpc_plugin::ResourceState::eType state));
  MOCK_CONST_METHOD1(IsResourceFree, bool(const std::string& module_type));
  MOCK_METHOD1(GetApplicationExtention,
               rc_rpc_plugin::RCAppExtensionPtr(application_manager::ApplicationSharedPtr application));
  MOCK_METHOD0(ResetAllAllocations, void());
};

}  // namespace rc_rpc_plugin_test

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_TEST_INCLUDE_MOCK_RESOURCE_ALLOCATION_MANAGER_H_
