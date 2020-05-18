#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_TEST_INCLUDE_VEHICLE_INFO_PLUGIN_MOCK_CUSTOM_VEHICLE_DATA_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_TEST_INCLUDE_VEHICLE_INFO_PLUGIN_MOCK_CUSTOM_VEHICLE_DATA_MANAGER_H_

#include "gmock/gmock.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager.h"

namespace vehicle_info_plugin {
namespace plugin_manager = application_manager::plugin_manager;
class MockCustomVehicleDataManager : public CustomVehicleDataManager {
 public:
  MOCK_METHOD1(
      CreateHMIMessageParams,
      smart_objects::SmartObject(const std::set<std::string>& msg_params));
  MOCK_METHOD1(CreateMobileMessageParams,
               void(smart_objects::SmartObject& msg_params));
  MOCK_CONST_METHOD1(GetVehicleDataItemType,
                     std::string(const std::string& vehicle_data_item_name));
  MOCK_METHOD1(OnPolicyEvent, void(plugin_manager::PolicyEvent policy_event));
  MOCK_CONST_METHOD1(IsValidCustomVehicleDataName,
                     bool(const std::string& name));
  MOCK_CONST_METHOD1(IsRemovedCustomVehicleDataName,
                     bool(const std::string& name));
};

}  // namespace vehicle_info_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_TEST_INCLUDE_VEHICLE_INFO_PLUGIN_MOCK_CUSTOM_VEHICLE_DATA_MANAGER_H_
