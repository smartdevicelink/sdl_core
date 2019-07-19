#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_CUSTOM_VEHICLE_DATA_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_CUSTOM_VEHICLE_DATA_MANAGER_H_
#include <set>
#include <string>

#include "smart_objects/smart_object.h"

namespace vehicle_info_plugin {
class CustomVehicleDataManager {
 public:
  /**
   * @brief Validates vehicle data items
   * @param msg_params received message params (msg_params key)
   * @return true, if vehicle data items within msg_params are valid,
   * otherwise - false
   */
  virtual bool ValidateVehicleDataItems(
      const smart_objects::SmartObject& msg_params) = 0;

  /**
   * @brief Creates message params (nested if needed) to be sent to HMI
   * according to vehicle data item schema
   * @param item_names set of names of vehicle data items to be processed
   * @return smartMap with prepared message params
   */
  virtual smart_objects::SmartObject CreateHMIMessageParams(
      const std::set<std::string>& item_names) = 0;

  /**
   * @brief Creates message params (nested if needed) to be sent to Mobile
   * according to vehicle data item schema
   * @param input_params message params received from hmi
   */
  virtual void CreateMobileMessageParams(
      smart_objects::SmartObject& msg_params) = 0;

  /**
   * @brief Checks whether name stands for valid custom vehicle data item
   * @param name for custom vehicle data item
   */
  virtual bool IsVehicleDataName(const std::string& name) = 0;

  /**
   * @brief Checks whether key stands for valid custom vehicle data item
   * @param key for custom vehicle data item
   */
  virtual bool IsVehicleDataKey(const std::string& key) = 0;
};
}  // namespace vehicle_info_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_CUSTOM_VEHICLE_DATA_MANAGER_H_
