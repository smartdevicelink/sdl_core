#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_CUSTOM_VEHICLE_DATA_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_CUSTOM_VEHICLE_DATA_MANAGER_H_
#include <set>
#include <string>

#include "application_manager/plugin_manager/rpc_plugin.h"
#include "application_manager/rpc_handler.h"
#include "smart_objects/smart_object.h"

namespace vehicle_info_plugin {

namespace plugin_manager = application_manager::plugin_manager;

class CustomVehicleDataManager {
 public:
  /**
   * @brief Creates message params (nested if needed) to be sent to HMI
   * according to vehicle data item schema
   * @param item_names set of names of vehicle data items to be processed
   * @return smartMap with prepared message params
   */
  virtual smart_objects::SmartObject CreateHMIMessageParams(
      const std::set<std::string>& item_names) = 0;

  /**
   * @brief Gets data type of vehicle data item
   * @param vehicle_data_item_name name of vehicle data
   * @return data type of vehicle_data if one found, item name otherwise
   */
  virtual std::string GetVehicleDataItemType(
      const std::string& vehicle_data_item_name) const = 0;

  /**
   * @brief Creates message params (nested if needed) to be sent to Mobile
   * according to vehicle data item schema
   * @param input_params message params received from hmi
   */
  virtual void CreateMobileMessageParams(
      smart_objects::SmartObject& msg_params) = 0;

  virtual void OnPolicyEvent(plugin_manager::PolicyEvent policy_event) = 0;

  virtual bool IsValidCustomVehicleDataName(const std::string& name) const = 0;
};
}  // namespace vehicle_info_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_CUSTOM_VEHICLE_DATA_MANAGER_H_
